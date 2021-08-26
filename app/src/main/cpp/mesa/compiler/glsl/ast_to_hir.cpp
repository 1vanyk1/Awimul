#include "glsl_symbol_table.h"
#include "glsl_parser_extras.h"
#include "ast.h"
#include "../glsl_types.h"
#include "../../util/hash_table.h"
#include "../../main/mtypes.h"
#include "../../main/macros.h"
#include "../../main/shaderobj.h"
#include "ir.h"
#include "ir_builder.h"
#include "builtin_functions.h"

using namespace ir_builder;

static void
detect_conflicting_assignments(struct _mesa_glsl_parse_state *state,
                               exec_list *instructions);
static void
verify_subroutine_associated_funcs(struct _mesa_glsl_parse_state *state);

static void
remove_per_vertex_blocks(exec_list *instructions,
                         _mesa_glsl_parse_state *state, ir_variable_mode mode);

/**
 * Visitor class that finds the first instance of any write-only variable that
 * is ever read, if any
 */
class read_from_write_only_variable_visitor : public ir_hierarchical_visitor
{
public:
    read_from_write_only_variable_visitor() : found(NULL)
    {
    }

    virtual ir_visitor_status visit(ir_dereference_variable *ir)
    {
        if (this->in_assignee)
            return visit_continue;

        ir_variable *var = ir->variable_referenced();
        /* We can have memory_write_only set on both images and buffer variables,
       * but in the former there is a distinction between reads from
       * the variable itself (write_only) and from the memory they point to
       * (memory_write_only), while in the case of buffer variables there is
       * no such distinction, that is why this check here is limited to
       * buffer variables alone.
       */
        if (!var || var->data.mode != ir_var_shader_storage)
            return visit_continue;

        if (var->data.memory_write_only) {
            found = var;
            return visit_stop;
        }

        return visit_continue;
    }

    ir_variable *get_variable() {
        return found;
    }

    virtual ir_visitor_status visit_enter(ir_expression *ir)
    {
        /* .length() doesn't actually read anything */
        if (ir->operation == ir_unop_ssbo_unsized_array_length)
            return visit_continue_with_parent;

        return visit_continue;
    }

private:
    ir_variable *found;
};

void
_mesa_ast_to_hir(exec_list *instructions, struct _mesa_glsl_parse_state *state)
{
    _mesa_glsl_initialize_variables(instructions, state);

    state->symbols->separate_function_namespace = state->language_version == 110;

    state->current_function = NULL;

    state->toplevel_ir = instructions;

    state->gs_input_prim_type_specified = false;
    state->tcs_output_vertices_specified = false;
    state->cs_input_local_size_specified = false;

    /* Section 4.2 of the GLSL 1.20 specification states:
    * "The built-in functions are scoped in a scope outside the global scope
    *  users declare global variables in.  That is, a shader's global scope,
    *  available for user-defined functions and global variables, is nested
    *  inside the scope containing the built-in functions."
    *
    * Since built-in functions like ftransform() access built-in variables,
    * it follows that those must be in the outer scope as well.
    *
    * We push scope here to create this nesting effect...but don't pop.
    * This way, a shader's globals are still in the symbol table for use
    * by the linker.
    */
    state->symbols->push_scope();

    foreach_list_typed (ast_node, ast, link, & state->translation_unit)
        ast->hir(instructions, state);

    verify_subroutine_associated_funcs(state);
    detect_recursion_unlinked(state, instructions);
    detect_conflicting_assignments(state, instructions);

    state->toplevel_ir = NULL;

    /* Move all of the variable declarations to the front of the IR list, and
    * reverse the order.  This has the (intended!) side effect that vertex
    * shader inputs and fragment shader outputs will appear in the IR in the
    * same order that they appeared in the shader code.  This results in the
    * locations being assigned in the declared order.  Many (arguably buggy)
    * applications depend on this behavior, and it matches what nearly all
    * other drivers do.
    */
    foreach_in_list_safe(ir_instruction, node, instructions) {
        ir_variable *const var = node->as_variable();

        if (var == NULL)
            continue;

        var->remove();
        instructions->push_head(var);
    }

    /* Figure out if gl_FragCoord is actually used in fragment shader */
    ir_variable *const var = state->symbols->get_variable("gl_FragCoord");
    if (var != NULL)
        state->fs_uses_gl_fragcoord = var->data.used;

    /* From section 7.1 (Built-In Language Variables) of the GLSL 4.10 spec:
    *
    *     If multiple shaders using members of a built-in block belonging to
    *     the same interface are linked together in the same program, they
    *     must all redeclare the built-in block in the same way, as described
    *     in section 4.3.7 "Interface Blocks" for interface block matching, or
    *     a link error will result.
    *
    * The phrase "using members of a built-in block" implies that if two
    * shaders are linked together and one of them *does not use* any members
    * of the built-in block, then that shader does not need to have a matching
    * redeclaration of the built-in block.
    *
    * This appears to be a clarification to the behaviour established for
    * gl_PerVertex by GLSL 1.50, therefore implement it regardless of GLSL
    * version.
    *
    * The definition of "interface" in section 4.3.7 that applies here is as
    * follows:
    *
    *     The boundary between adjacent programmable pipeline stages: This
    *     spans all the outputs in all compilation units of the first stage
    *     and all the inputs in all compilation units of the second stage.
    *
    * Therefore this rule applies to both inter- and intra-stage linking.
    *
    * The easiest way to implement this is to check whether the shader uses
    * gl_PerVertex right after ast-to-ir conversion, and if it doesn't, simply
    * remove all the relevant variable declaration from the IR, so that the
    * linker won't see them and complain about mismatches.
    */
    remove_per_vertex_blocks(instructions, state, ir_var_shader_in);
    remove_per_vertex_blocks(instructions, state, ir_var_shader_out);

    /* Check that we don't have reads from write-only variables */
    read_from_write_only_variable_visitor v;
    v.run(instructions);
    ir_variable *error_var = v.get_variable();
    if (error_var) {
        /* It would be nice to have proper location information, but for that
       * we would need to check this as we process each kind of AST node
       */
        YYLTYPE loc;
        memset(&loc, 0, sizeof(loc));
        _mesa_glsl_error(&loc, state, "Read from write-only variable `%s'",
                         error_var->name);
    }
}


static ir_expression_operation
get_implicit_conversion_operation(const glsl_type *to, const glsl_type *from,
                                  struct _mesa_glsl_parse_state *state)
{
    switch (to->base_type) {
        case GLSL_TYPE_FLOAT:
            switch (from->base_type) {
                case GLSL_TYPE_INT: return ir_unop_i2f;
                case GLSL_TYPE_UINT: return ir_unop_u2f;
                default: return (ir_expression_operation)0;
            }

        case GLSL_TYPE_UINT:
            if (!state->has_implicit_int_to_uint_conversion())
                return (ir_expression_operation)0;
            switch (from->base_type) {
                case GLSL_TYPE_INT: return ir_unop_i2u;
                default: return (ir_expression_operation)0;
            }

        case GLSL_TYPE_DOUBLE:
            if (!state->has_double())
                return (ir_expression_operation)0;
            switch (from->base_type) {
                case GLSL_TYPE_INT: return ir_unop_i2d;
                case GLSL_TYPE_UINT: return ir_unop_u2d;
                case GLSL_TYPE_FLOAT: return ir_unop_f2d;
                case GLSL_TYPE_INT64: return ir_unop_i642d;
                case GLSL_TYPE_UINT64: return ir_unop_u642d;
                default: return (ir_expression_operation)0;
            }

        case GLSL_TYPE_UINT64:
            if (!state->has_int64())
                return (ir_expression_operation)0;
            switch (from->base_type) {
                case GLSL_TYPE_INT: return ir_unop_i2u64;
                case GLSL_TYPE_UINT: return ir_unop_u2u64;
                case GLSL_TYPE_INT64: return ir_unop_i642u64;
                default: return (ir_expression_operation)0;
            }

        case GLSL_TYPE_INT64:
            if (!state->has_int64())
                return (ir_expression_operation)0;
            switch (from->base_type) {
                case GLSL_TYPE_INT: return ir_unop_i2i64;
                default: return (ir_expression_operation)0;
            }

        default: return (ir_expression_operation)0;
    }
}


/**
 * If a conversion is available, convert one operand to a different type
 *
 * The \c from \c ir_rvalue is converted "in place".
 *
 * \param to     Type that the operand it to be converted to
 * \param from   Operand that is being converted
 * \param state  GLSL compiler state
 *
 * \return
 * If a conversion is possible (or unnecessary), \c true is returned.
 * Otherwise \c false is returned.
 */
static bool
apply_implicit_conversion(const glsl_type *to, ir_rvalue * &from,
                          struct _mesa_glsl_parse_state *state)
{
    void *ctx = state;
    if (to->base_type == from->type->base_type)
        return true;

    /* Prior to GLSL 1.20, there are no implicit conversions */
    if (!state->has_implicit_conversions())
        return false;

    /* From page 27 (page 33 of the PDF) of the GLSL 1.50 spec:
    *
    *    "There are no implicit array or structure conversions. For
    *    example, an array of int cannot be implicitly converted to an
    *    array of float.
    */
    if (!to->is_numeric() || !from->type->is_numeric())
        return false;

    /* We don't actually want the specific type `to`, we want a type
    * with the same base type as `to`, but the same vector width as
    * `from`.
    */
    to = glsl_type::get_instance(to->base_type, from->type->vector_elements,
                                 from->type->matrix_columns);

    ir_expression_operation op = get_implicit_conversion_operation(to, from->type, state);
    if (op) {
        from = new(ctx) ir_expression(op, to, from, NULL);
        return true;
    } else {
        return false;
    }
}


static const struct glsl_type *
arithmetic_result_type(ir_rvalue * &value_a, ir_rvalue * &value_b,
                       bool multiply,
                       struct _mesa_glsl_parse_state *state, YYLTYPE *loc)
{
    const glsl_type *type_a = value_a->type;
    const glsl_type *type_b = value_b->type;

    /* From GLSL 1.50 spec, page 56:
    *
    *    "The arithmetic binary operators add (+), subtract (-),
    *    multiply (*), and divide (/) operate on integer and
    *    floating-point scalars, vectors, and matrices."
    */
    if (!type_a->is_numeric() || !type_b->is_numeric()) {
        _mesa_glsl_error(loc, state,
                         "operands to arithmetic operators must be numeric");
        return glsl_type::error_type;
    }


    /*    "If one operand is floating-point based and the other is
    *    not, then the conversions from Section 4.1.10 "Implicit
    *    Conversions" are applied to the non-floating-point-based operand."
    */
    if (!apply_implicit_conversion(type_a, value_b, state)
        && !apply_implicit_conversion(type_b, value_a, state)) {
        _mesa_glsl_error(loc, state,
                         "could not implicitly convert operands to "
                         "arithmetic operator");
        return glsl_type::error_type;
    }
    type_a = value_a->type;
    type_b = value_b->type;

    /*    "If the operands are integer types, they must both be signed or
    *    both be unsigned."
    *
    * From this rule and the preceeding conversion it can be inferred that
    * both types must be GLSL_TYPE_FLOAT, or GLSL_TYPE_UINT, or GLSL_TYPE_INT.
    * The is_numeric check above already filtered out the case where either
    * type is not one of these, so now the base types need only be tested for
    * equality.
    */
    if (type_a->base_type != type_b->base_type) {
        _mesa_glsl_error(loc, state,
                         "base type mismatch for arithmetic operator");
        return glsl_type::error_type;
    }

    /*    "All arithmetic binary operators result in the same fundamental type
    *    (signed integer, unsigned integer, or floating-point) as the
    *    operands they operate on, after operand type conversion. After
    *    conversion, the following cases are valid
    *
    *    * The two operands are scalars. In this case the operation is
    *      applied, resulting in a scalar."
    */
    if (type_a->is_scalar() && type_b->is_scalar())
        return type_a;

    /*   "* One operand is a scalar, and the other is a vector or matrix.
    *      In this case, the scalar operation is applied independently to each
    *      component of the vector or matrix, resulting in the same size
    *      vector or matrix."
    */
    if (type_a->is_scalar()) {
        if (!type_b->is_scalar())
            return type_b;
    } else if (type_b->is_scalar()) {
        return type_a;
    }

    /* All of the combinations of <scalar, scalar>, <vector, scalar>,
    * <scalar, vector>, <scalar, matrix>, and <matrix, scalar> have been
    * handled.
    */
    assert(!type_a->is_scalar());
    assert(!type_b->is_scalar());

    /*   "* The two operands are vectors of the same size. In this case, the
    *      operation is done component-wise resulting in the same size
    *      vector."
    */
    if (type_a->is_vector() && type_b->is_vector()) {
        if (type_a == type_b) {
            return type_a;
        } else {
            _mesa_glsl_error(loc, state,
                             "vector size mismatch for arithmetic operator");
            return glsl_type::error_type;
        }
    }

    /* All of the combinations of <scalar, scalar>, <vector, scalar>,
    * <scalar, vector>, <scalar, matrix>, <matrix, scalar>, and
    * <vector, vector> have been handled.  At least one of the operands must
    * be matrix.  Further, since there are no integer matrix types, the base
    * type of both operands must be float.
    */
    assert(type_a->is_matrix() || type_b->is_matrix());
    assert(type_a->is_float() || type_a->is_double());
    assert(type_b->is_float() || type_b->is_double());

    /*   "* The operator is add (+), subtract (-), or divide (/), and the
    *      operands are matrices with the same number of rows and the same
    *      number of columns. In this case, the operation is done component-
    *      wise resulting in the same size matrix."
    *    * The operator is multiply (*), where both operands are matrices or
    *      one operand is a vector and the other a matrix. A right vector
    *      operand is treated as a column vector and a left vector operand as a
    *      row vector. In all these cases, it is required that the number of
    *      columns of the left operand is equal to the number of rows of the
    *      right operand. Then, the multiply (*) operation does a linear
    *      algebraic multiply, yielding an object that has the same number of
    *      rows as the left operand and the same number of columns as the right
    *      operand. Section 5.10 "Vector and Matrix Operations" explains in
    *      more detail how vectors and matrices are operated on."
    */
    if (! multiply) {
        if (type_a == type_b)
            return type_a;
    } else {
        const glsl_type *type = glsl_type::get_mul_type(type_a, type_b);

        if (type == glsl_type::error_type) {
            _mesa_glsl_error(loc, state,
                             "size mismatch for matrix multiplication");
        }

        return type;
    }


    /*    "All other cases are illegal."
    */
    _mesa_glsl_error(loc, state, "type mismatch");
    return glsl_type::error_type;
}


static const struct glsl_type *
unary_arithmetic_result_type(const struct glsl_type *type,
                             struct _mesa_glsl_parse_state *state, YYLTYPE *loc)
{
    /* From GLSL 1.50 spec, page 57:
    *
    *    "The arithmetic unary operators negate (-), post- and pre-increment
    *     and decrement (-- and ++) operate on integer or floating-point
    *     values (including vectors and matrices). All unary operators work
    *     component-wise on their operands. These result with the same type
    *     they operated on."
    */
    if (!type->is_numeric()) {
        _mesa_glsl_error(loc, state,
                         "operands to arithmetic operators must be numeric");
        return glsl_type::error_type;
    }

    return type;
}

/**
 * \brief Return the result type of a bit-logic operation.
 *
 * If the given types to the bit-logic operator are invalid, return
 * glsl_type::error_type.
 *
 * \param value_a LHS of bit-logic op
 * \param value_b RHS of bit-logic op
 */
static const struct glsl_type *
bit_logic_result_type(ir_rvalue * &value_a, ir_rvalue * &value_b,
                      ast_operators op,
                      struct _mesa_glsl_parse_state *state, YYLTYPE *loc)
{
    const glsl_type *type_a = value_a->type;
    const glsl_type *type_b = value_b->type;

    if (!state->check_bitwise_operations_allowed(loc)) {
        return glsl_type::error_type;
    }

    /* From page 50 (page 56 of PDF) of GLSL 1.30 spec:
    *
    *     "The bitwise operators and (&), exclusive-or (^), and inclusive-or
    *     (|). The operands must be of type signed or unsigned integers or
    *     integer vectors."
    */
    if (!type_a->is_integer_32_64()) {
        _mesa_glsl_error(loc, state, "LHS of `%s' must be an integer",
                         ast_expression::operator_string(op));
        return glsl_type::error_type;
    }
    if (!type_b->is_integer_32_64()) {
        _mesa_glsl_error(loc, state, "RHS of `%s' must be an integer",
                         ast_expression::operator_string(op));
        return glsl_type::error_type;
    }

    /* Prior to GLSL 4.0 / GL_ARB_gpu_shader5, implicit conversions didn't
    * make sense for bitwise operations, as they don't operate on floats.
    *
    * GLSL 4.0 added implicit int -> uint conversions, which are relevant
    * here.  It wasn't clear whether or not we should apply them to bitwise
    * operations.  However, Khronos has decided that they should in future
    * language revisions.  Applications also rely on this behavior.  We opt
    * to apply them in general, but issue a portability warning.
    *
    * See https://www.khronos.org/bugzilla/show_bug.cgi?id=1405
    */
    if (type_a->base_type != type_b->base_type) {
        if (!apply_implicit_conversion(type_a, value_b, state)
            && !apply_implicit_conversion(type_b, value_a, state)) {
            _mesa_glsl_error(loc, state,
                             "could not implicitly convert operands to "
                             "`%s` operator",
                             ast_expression::operator_string(op));
            return glsl_type::error_type;
        } else {
            _mesa_glsl_warning(loc, state,
                               "some implementations may not support implicit "
                               "int -> uint conversions for `%s' operators; "
                               "consider casting explicitly for portability",
                               ast_expression::operator_string(op));
        }
        type_a = value_a->type;
        type_b = value_b->type;
    }

    /*     "The fundamental types of the operands (signed or unsigned) must
    *     match,"
    */
    if (type_a->base_type != type_b->base_type) {
        _mesa_glsl_error(loc, state, "operands of `%s' must have the same "
                                     "base type", ast_expression::operator_string(op));
        return glsl_type::error_type;
    }

    /*     "The operands cannot be vectors of differing size." */
    if (type_a->is_vector() &&
        type_b->is_vector() &&
        type_a->vector_elements != type_b->vector_elements) {
        _mesa_glsl_error(loc, state, "operands of `%s' cannot be vectors of "
                                     "different sizes", ast_expression::operator_string(op));
        return glsl_type::error_type;
    }

    /*     "If one operand is a scalar and the other a vector, the scalar is
    *     applied component-wise to the vector, resulting in the same type as
    *     the vector. The fundamental types of the operands [...] will be the
    *     resulting fundamental type."
    */
    if (type_a->is_scalar())
        return type_b;
    else
        return type_a;
}

static const struct glsl_type *
modulus_result_type(ir_rvalue * &value_a, ir_rvalue * &value_b,
                    struct _mesa_glsl_parse_state *state, YYLTYPE *loc)
{
    const glsl_type *type_a = value_a->type;
    const glsl_type *type_b = value_b->type;

    if (!state->EXT_gpu_shader4_enable &&
        !state->check_version(130, 300, loc, "operator '%%' is reserved")) {
        return glsl_type::error_type;
    }

    /* Section 5.9 (Expressions) of the GLSL 4.00 specification says:
    *
    *    "The operator modulus (%) operates on signed or unsigned integers or
    *    integer vectors."
    */
    if (!type_a->is_integer_32_64()) {
        _mesa_glsl_error(loc, state, "LHS of operator %% must be an integer");
        return glsl_type::error_type;
    }
    if (!type_b->is_integer_32_64()) {
        _mesa_glsl_error(loc, state, "RHS of operator %% must be an integer");
        return glsl_type::error_type;
    }

    /*    "If the fundamental types in the operands do not match, then the
    *    conversions from section 4.1.10 "Implicit Conversions" are applied
    *    to create matching types."
    *
    * Note that GLSL 4.00 (and GL_ARB_gpu_shader5) introduced implicit
    * int -> uint conversion rules.  Prior to that, there were no implicit
    * conversions.  So it's harmless to apply them universally - no implicit
    * conversions will exist.  If the types don't match, we'll receive false,
    * and raise an error, satisfying the GLSL 1.50 spec, page 56:
    *
    *    "The operand types must both be signed or unsigned."
    */
    if (!apply_implicit_conversion(type_a, value_b, state) &&
        !apply_implicit_conversion(type_b, value_a, state)) {
        _mesa_glsl_error(loc, state,
                         "could not implicitly convert operands to "
                         "modulus (%%) operator");
        return glsl_type::error_type;
    }
    type_a = value_a->type;
    type_b = value_b->type;

    /*    "The operands cannot be vectors of differing size. If one operand is
    *    a scalar and the other vector, then the scalar is applied component-
    *    wise to the vector, resulting in the same type as the vector. If both
    *    are vectors of the same size, the result is computed component-wise."
    */
    if (type_a->is_vector()) {
        if (!type_b->is_vector()
            || (type_a->vector_elements == type_b->vector_elements))
            return type_a;
    } else
        return type_b;

    /*    "The operator modulus (%) is not defined for any other data types
    *    (non-integer types)."
    */
    _mesa_glsl_error(loc, state, "type mismatch");
    return glsl_type::error_type;
}


static const struct glsl_type *
relational_result_type(ir_rvalue * &value_a, ir_rvalue * &value_b,
                       struct _mesa_glsl_parse_state *state, YYLTYPE *loc)
{
    const glsl_type *type_a = value_a->type;
    const glsl_type *type_b = value_b->type;

    /* From GLSL 1.50 spec, page 56:
    *    "The relational operators greater than (>), less than (<), greater
    *    than or equal (>=), and less than or equal (<=) operate only on
    *    scalar integer and scalar floating-point expressions."
    */
    if (!type_a->is_numeric()
        || !type_b->is_numeric()
        || !type_a->is_scalar()
        || !type_b->is_scalar()) {
        _mesa_glsl_error(loc, state,
                         "operands to relational operators must be scalar and "
                         "numeric");
        return glsl_type::error_type;
    }

    /*    "Either the operands' types must match, or the conversions from
    *    Section 4.1.10 "Implicit Conversions" will be applied to the integer
    *    operand, after which the types must match."
    */
    if (!apply_implicit_conversion(type_a, value_b, state)
        && !apply_implicit_conversion(type_b, value_a, state)) {
        _mesa_glsl_error(loc, state,
                         "could not implicitly convert operands to "
                         "relational operator");
        return glsl_type::error_type;
    }
    type_a = value_a->type;
    type_b = value_b->type;

    if (type_a->base_type != type_b->base_type) {
        _mesa_glsl_error(loc, state, "base type mismatch");
        return glsl_type::error_type;
    }

    /*    "The result is scalar Boolean."
    */
    return glsl_type::bool_type;
}

/**
 * \brief Return the result type of a bit-shift operation.
 *
 * If the given types to the bit-shift operator are invalid, return
 * glsl_type::error_type.
 *
 * \param type_a Type of LHS of bit-shift op
 * \param type_b Type of RHS of bit-shift op
 */
static const struct glsl_type *
shift_result_type(const struct glsl_type *type_a,
                  const struct glsl_type *type_b,
                  ast_operators op,
                  struct _mesa_glsl_parse_state *state, YYLTYPE *loc)
{
    if (!state->check_bitwise_operations_allowed(loc)) {
        return glsl_type::error_type;
    }

    /* From page 50 (page 56 of the PDF) of the GLSL 1.30 spec:
    *
    *     "The shift operators (<<) and (>>). For both operators, the operands
    *     must be signed or unsigned integers or integer vectors. One operand
    *     can be signed while the other is unsigned."
    */
    if (!type_a->is_integer_32_64()) {
        _mesa_glsl_error(loc, state, "LHS of operator %s must be an integer or "
                                     "integer vector", ast_expression::operator_string(op));
        return glsl_type::error_type;

    }
    if (!type_b->is_integer_32()) {
        _mesa_glsl_error(loc, state, "RHS of operator %s must be an integer or "
                                     "integer vector", ast_expression::operator_string(op));
        return glsl_type::error_type;
    }

    /*     "If the first operand is a scalar, the second operand has to be
    *     a scalar as well."
    */
    if (type_a->is_scalar() && !type_b->is_scalar()) {
        _mesa_glsl_error(loc, state, "if the first operand of %s is scalar, the "
                                     "second must be scalar as well",
                         ast_expression::operator_string(op));
        return glsl_type::error_type;
    }

    /* If both operands are vectors, check that they have same number of
    * elements.
    */
    if (type_a->is_vector() &&
        type_b->is_vector() &&
        type_a->vector_elements != type_b->vector_elements) {
        _mesa_glsl_error(loc, state, "vector operands to operator %s must "
                                     "have same number of elements",
                         ast_expression::operator_string(op));
        return glsl_type::error_type;
    }

    /*     "In all cases, the resulting type will be the same type as the left
    *     operand."
    */
    return type_a;
}

/**
 * Returns the innermost array index expression in an rvalue tree.
 * This is the largest indexing level -- if an array of blocks, then
 * it is the block index rather than an indexing expression for an
 * array-typed member of an array of blocks.
 */
static ir_rvalue *
find_innermost_array_index(ir_rvalue *rv)
{
    ir_dereference_array *last = NULL;
    while (rv) {
        if (rv->as_dereference_array()) {
            last = rv->as_dereference_array();
            rv = last->array;
        } else if (rv->as_dereference_record())
            rv = rv->as_dereference_record()->record;
        else if (rv->as_swizzle())
            rv = rv->as_swizzle()->val;
        else
            rv = NULL;
    }

    if (last)
        return last->array_index;

    return NULL;
}

/**
 * Validates that a value can be assigned to a location with a specified type
 *
 * Validates that \c rhs can be assigned to some location.  If the types are
 * not an exact match but an automatic conversion is possible, \c rhs will be
 * converted.
 *
 * \return
 * \c NULL if \c rhs cannot be assigned to a location with type \c lhs_type.
 * Otherwise the actual RHS to be assigned will be returned.  This may be
 * \c rhs, or it may be \c rhs after some type conversion.
 *
 * \note
 * In addition to being used for assignments, this function is used to
 * type-check return values.
 */
static ir_rvalue *
validate_assignment(struct _mesa_glsl_parse_state *state,
                    YYLTYPE loc, ir_rvalue *lhs,
                    ir_rvalue *rhs, bool is_initializer)
{
    /* If there is already some error in the RHS, just return it.  Anything
    * else will lead to an avalanche of error message back to the user.
    */
    if (rhs->type->is_error())
        return rhs;

    /* In the Tessellation Control Shader:
    * If a per-vertex output variable is used as an l-value, it is an error
    * if the expression indicating the vertex number is not the identifier
    * `gl_InvocationID`.
    */
    if (state->stage == MESA_SHADER_TESS_CTRL && !lhs->type->is_error()) {
        ir_variable *var = lhs->variable_referenced();
        if (var && var->data.mode == ir_var_shader_out && !var->data.patch) {
            ir_rvalue *index = find_innermost_array_index(lhs);
            ir_variable *index_var = index ? index->variable_referenced() : NULL;
            if (!index_var || strcmp(index_var->name, "gl_InvocationID") != 0) {
                _mesa_glsl_error(&loc, state,
                                 "Tessellation control shader outputs can only "
                                 "be indexed by gl_InvocationID");
                return NULL;
            }
        }
    }

    /* If the types are identical, the assignment can trivially proceed.
    */
    if (rhs->type == lhs->type)
        return rhs;

    /* If the array element types are the same and the LHS is unsized,
    * the assignment is okay for initializers embedded in variable
    * declarations.
    *
    * Note: Whole-array assignments are not permitted in GLSL 1.10, but this
    * is handled by ir_dereference::is_lvalue.
    */
    const glsl_type *lhs_t = lhs->type;
    const glsl_type *rhs_t = rhs->type;
    bool unsized_array = false;
    while(lhs_t->is_array()) {
        if (rhs_t == lhs_t)
            break; /* the rest of the inner arrays match so break out early */
        if (!rhs_t->is_array()) {
            unsized_array = false;
            break; /* number of dimensions mismatch */
        }
        if (lhs_t->length == rhs_t->length) {
            lhs_t = lhs_t->fields.array;
            rhs_t = rhs_t->fields.array;
            continue;
        } else if (lhs_t->is_unsized_array()) {
            unsized_array = true;
        } else {
            unsized_array = false;
            break; /* sized array mismatch */
        }
        lhs_t = lhs_t->fields.array;
        rhs_t = rhs_t->fields.array;
    }
    if (unsized_array) {
        if (is_initializer) {
            if (rhs->type->get_scalar_type() == lhs->type->get_scalar_type())
                return rhs;
        } else {
            _mesa_glsl_error(&loc, state,
                             "implicitly sized arrays cannot be assigned");
            return NULL;
        }
    }

    /* Check for implicit conversion in GLSL 1.20 */
    if (apply_implicit_conversion(lhs->type, rhs, state)) {
        if (rhs->type == lhs->type)
            return rhs;
    }

    _mesa_glsl_error(&loc, state,
                     "%s of type %s cannot be assigned to "
                     "variable of type %s",
                     is_initializer ? "initializer" : "value",
                     rhs->type->name, lhs->type->name);

    return NULL;
}

static void
mark_whole_array_access(ir_rvalue *access)
{
    ir_dereference_variable *deref = access->as_dereference_variable();

    if (deref && deref->var) {
        deref->var->data.max_array_access = deref->type->length - 1;
    }
}

static bool
do_assignment(exec_list *instructions, struct _mesa_glsl_parse_state *state,
              const char *non_lvalue_description,
              ir_rvalue *lhs, ir_rvalue *rhs,
              ir_rvalue **out_rvalue, bool needs_rvalue,
              bool is_initializer,
              YYLTYPE lhs_loc)
{
    void *ctx = state;
    bool error_emitted = (lhs->type->is_error() || rhs->type->is_error());

    ir_variable *lhs_var = lhs->variable_referenced();
    if (lhs_var)
        lhs_var->data.assigned = true;

    if (!error_emitted) {
        if (non_lvalue_description != NULL) {
            _mesa_glsl_error(&lhs_loc, state,
                             "assignment to %s",
                             non_lvalue_description);
            error_emitted = true;
        } else if (lhs_var != NULL && (lhs_var->data.read_only ||
                                       (lhs_var->data.mode == ir_var_shader_storage &&
                                        lhs_var->data.memory_read_only))) {
            /* We can have memory_read_only set on both images and buffer variables,
          * but in the former there is a distinction between assignments to
          * the variable itself (read_only) and to the memory they point to
          * (memory_read_only), while in the case of buffer variables there is
          * no such distinction, that is why this check here is limited to
          * buffer variables alone.
          */
            _mesa_glsl_error(&lhs_loc, state,
                             "assignment to read-only variable '%s'",
                             lhs_var->name);
            error_emitted = true;
        } else if (lhs->type->is_array() &&
                   !state->check_version(state->allow_glsl_120_subset_in_110 ? 110 : 120,
                                         300, &lhs_loc,
                                         "whole array assignment forbidden")) {
            /* From page 32 (page 38 of the PDF) of the GLSL 1.10 spec:
          *
          *    "Other binary or unary expressions, non-dereferenced
          *     arrays, function names, swizzles with repeated fields,
          *     and constants cannot be l-values."
          *
          * The restriction on arrays is lifted in GLSL 1.20 and GLSL ES 3.00.
          */
            error_emitted = true;
        } else if (!lhs->is_lvalue(state)) {
            _mesa_glsl_error(& lhs_loc, state, "non-lvalue in assignment");
            error_emitted = true;
        }
    }

    ir_rvalue *new_rhs =
            validate_assignment(state, lhs_loc, lhs, rhs, is_initializer);
    if (new_rhs != NULL) {
        rhs = new_rhs;

        /* If the LHS array was not declared with a size, it takes it size from
       * the RHS.  If the LHS is an l-value and a whole array, it must be a
       * dereference of a variable.  Any other case would require that the LHS
       * is either not an l-value or not a whole array.
       */
        if (lhs->type->is_unsized_array()) {
            ir_dereference *const d = lhs->as_dereference();

            assert(d != NULL);

            ir_variable *const var = d->variable_referenced();

            assert(var != NULL);

            if (var->data.max_array_access >= rhs->type->array_size()) {
                /* FINISHME: This should actually log the location of the RHS. */
                _mesa_glsl_error(& lhs_loc, state, "array size must be > %u due to "
                                                   "previous access",
                                 var->data.max_array_access);
            }

            var->type = glsl_type::get_array_instance(lhs->type->fields.array,
                                                      rhs->type->array_size());
            d->type = var->type;
        }
        if (lhs->type->is_array()) {
            mark_whole_array_access(rhs);
            mark_whole_array_access(lhs);
        }
    } else {
        error_emitted = true;
    }

    /* Most callers of do_assignment (assign, add_assign, pre_inc/dec,
    * but not post_inc) need the converted assigned value as an rvalue
    * to handle things like:
    *
    * i = j += 1;
    */
    if (needs_rvalue) {
        ir_rvalue *rvalue;
        if (!error_emitted) {
            ir_variable *var = new(ctx) ir_variable(rhs->type, "assignment_tmp",
                                                    ir_var_temporary);
            instructions->push_tail(var);
            instructions->push_tail(assign(var, rhs));

            ir_dereference_variable *deref_var =
                    new(ctx) ir_dereference_variable(var);
            instructions->push_tail(new(ctx) ir_assignment(lhs, deref_var));
            rvalue = new(ctx) ir_dereference_variable(var);
        } else {
            rvalue = ir_rvalue::error_value(ctx);
        }
        *out_rvalue = rvalue;
    } else {
        if (!error_emitted)
            instructions->push_tail(new(ctx) ir_assignment(lhs, rhs));
        *out_rvalue = NULL;
    }

    return error_emitted;
}

static ir_rvalue *
get_lvalue_copy(exec_list *instructions, ir_rvalue *lvalue)
{
    void *ctx = ralloc_parent(lvalue);
    ir_variable *var;

    var = new(ctx) ir_variable(lvalue->type, "_post_incdec_tmp",
                               ir_var_temporary);
    instructions->push_tail(var);

    instructions->push_tail(new(ctx) ir_assignment(new(ctx) ir_dereference_variable(var),
                                                   lvalue));

    return new(ctx) ir_dereference_variable(var);
}


ir_rvalue *
ast_node::hir(exec_list *instructions, struct _mesa_glsl_parse_state *state)
{
    (void) instructions;
    (void) state;

    return NULL;
}

bool
ast_node::has_sequence_subexpression() const
{
    return false;
}

void
ast_node::set_is_lhs(bool /* new_value */)
{
}

void
ast_function_expression::hir_no_rvalue(exec_list *instructions,
                                       struct _mesa_glsl_parse_state *state)
{
    (void)hir(instructions, state);
}

void
ast_aggregate_initializer::hir_no_rvalue(exec_list *instructions,
                                         struct _mesa_glsl_parse_state *state)
{
    (void)hir(instructions, state);
}

static ir_rvalue *
do_comparison(void *mem_ctx, int operation, ir_rvalue *op0, ir_rvalue *op1)
{
    int join_op;
    ir_rvalue *cmp = NULL;

    if (operation == ir_binop_all_equal)
        join_op = ir_binop_logic_and;
    else
        join_op = ir_binop_logic_or;

    switch (op0->type->base_type) {
        case GLSL_TYPE_FLOAT:
        case GLSL_TYPE_FLOAT16:
        case GLSL_TYPE_UINT:
        case GLSL_TYPE_INT:
        case GLSL_TYPE_BOOL:
        case GLSL_TYPE_DOUBLE:
        case GLSL_TYPE_UINT64:
        case GLSL_TYPE_INT64:
        case GLSL_TYPE_UINT16:
        case GLSL_TYPE_INT16:
        case GLSL_TYPE_UINT8:
        case GLSL_TYPE_INT8:
            return new(mem_ctx) ir_expression(operation, op0, op1);

        case GLSL_TYPE_ARRAY: {
            for (unsigned int i = 0; i < op0->type->length; i++) {
                ir_rvalue *e0, *e1, *result;

                e0 = new(mem_ctx) ir_dereference_array(op0->clone(mem_ctx, NULL),
                                                       new(mem_ctx) ir_constant(i));
                e1 = new(mem_ctx) ir_dereference_array(op1->clone(mem_ctx, NULL),
                                                       new(mem_ctx) ir_constant(i));
                result = do_comparison(mem_ctx, operation, e0, e1);

                if (cmp) {
                    cmp = new(mem_ctx) ir_expression(join_op, cmp, result);
                } else {
                    cmp = result;
                }
            }

            mark_whole_array_access(op0);
            mark_whole_array_access(op1);
            break;
        }

        case GLSL_TYPE_STRUCT: {
            for (unsigned int i = 0; i < op0->type->length; i++) {
                ir_rvalue *e0, *e1, *result;
                const char *field_name = op0->type->fields.structure[i].name;

                e0 = new(mem_ctx) ir_dereference_record(op0->clone(mem_ctx, NULL),
                                                        field_name);
                e1 = new(mem_ctx) ir_dereference_record(op1->clone(mem_ctx, NULL),
                                                        field_name);
                result = do_comparison(mem_ctx, operation, e0, e1);

                if (cmp) {
                    cmp = new(mem_ctx) ir_expression(join_op, cmp, result);
                } else {
                    cmp = result;
                }
            }
            break;
        }

        case GLSL_TYPE_ERROR:
        case GLSL_TYPE_VOID:
        case GLSL_TYPE_SAMPLER:
        case GLSL_TYPE_IMAGE:
        case GLSL_TYPE_INTERFACE:
        case GLSL_TYPE_ATOMIC_UINT:
        case GLSL_TYPE_SUBROUTINE:
        case GLSL_TYPE_FUNCTION:
            /* I assume a comparison of a struct containing a sampler just
       * ignores the sampler present in the type.
       */
            break;
    }

    if (cmp == NULL)
        cmp = new(mem_ctx) ir_constant(true);

    return cmp;
}

/* For logical operations, we want to ensure that the operands are
 * scalar booleans.  If it isn't, emit an error and return a constant
 * boolean to avoid triggering cascading error messages.
 */
static ir_rvalue *
get_scalar_boolean_operand(exec_list *instructions,
                           struct _mesa_glsl_parse_state *state,
                           ast_expression *parent_expr,
                           int operand,
                           const char *operand_name,
                           bool *error_emitted)
{
    ast_expression *expr = parent_expr->subexpressions[operand];
    void *ctx = state;
    ir_rvalue *val = expr->hir(instructions, state);

    if (val->type->is_boolean() && val->type->is_scalar())
        return val;

    if (!*error_emitted) {
        YYLTYPE loc = expr->get_location();
        _mesa_glsl_error(&loc, state, "%s of `%s' must be scalar boolean",
                         operand_name,
                         parent_expr->operator_string(parent_expr->oper));
        *error_emitted = true;
    }

    return new(ctx) ir_constant(true);
}

/**
 * If name refers to a builtin array whose maximum allowed size is less than
 * size, report an error and return true.  Otherwise return false.
 */
void
check_builtin_array_max_size(const char *name, unsigned size,
                             YYLTYPE loc, struct _mesa_glsl_parse_state *state)
{
    if ((strcmp("gl_TexCoord", name) == 0)
        && (size > state->Const.MaxTextureCoords)) {
        /* From page 54 (page 60 of the PDF) of the GLSL 1.20 spec:
       *
       *     "The size [of gl_TexCoord] can be at most
       *     gl_MaxTextureCoords."
       */
        _mesa_glsl_error(&loc, state, "`gl_TexCoord' array size cannot "
                                      "be larger than gl_MaxTextureCoords (%u)",
                         state->Const.MaxTextureCoords);
    } else if (strcmp("gl_ClipDistance", name) == 0) {
        state->clip_dist_size = size;
        if (size + state->cull_dist_size > state->Const.MaxClipPlanes) {
            /* From section 7.1 (Vertex Shader Special Variables) of the
          * GLSL 1.30 spec:
          *
          *   "The gl_ClipDistance array is predeclared as unsized and
          *   must be sized by the shader either redeclaring it with a
          *   size or indexing it only with integral constant
          *   expressions. ... The size can be at most
          *   gl_MaxClipDistances."
          */
            _mesa_glsl_error(&loc, state, "`gl_ClipDistance' array size cannot "
                                          "be larger than gl_MaxClipDistances (%u)",
                             state->Const.MaxClipPlanes);
        }
    } else if (strcmp("gl_CullDistance", name) == 0) {
        state->cull_dist_size = size;
        if (size + state->clip_dist_size > state->Const.MaxClipPlanes) {
            /* From the ARB_cull_distance spec:
          *
          *   "The gl_CullDistance array is predeclared as unsized and
          *    must be sized by the shader either redeclaring it with
          *    a size or indexing it only with integral constant
          *    expressions. The size determines the number and set of
          *    enabled cull distances and can be at most
          *    gl_MaxCullDistances."
          */
            _mesa_glsl_error(&loc, state, "`gl_CullDistance' array size cannot "
                                          "be larger than gl_MaxCullDistances (%u)",
                             state->Const.MaxClipPlanes);
        }
    }
}

/**
 * Create the constant 1, of a which is appropriate for incrementing and
 * decrementing values of the given GLSL type.  For example, if type is vec4,
 * this creates a constant value of 1.0 having type float.
 *
 * If the given type is invalid for increment and decrement operators, return
 * a floating point 1--the error will be detected later.
 */
static ir_rvalue *
constant_one_for_inc_dec(void *ctx, const glsl_type *type)
{
    switch (type->base_type) {
        case GLSL_TYPE_UINT:
            return new(ctx) ir_constant((unsigned) 1);
        case GLSL_TYPE_INT:
            return new(ctx) ir_constant(1);
        case GLSL_TYPE_UINT64:
            return new(ctx) ir_constant((uint64_t) 1);
        case GLSL_TYPE_INT64:
            return new(ctx) ir_constant((int64_t) 1);
        default:
        case GLSL_TYPE_FLOAT:
            return new(ctx) ir_constant(1.0f);
    }
}

ir_rvalue *
ast_expression::hir(exec_list *instructions,
                    struct _mesa_glsl_parse_state *state)
{
    return do_hir(instructions, state, true);
}

void
ast_expression::hir_no_rvalue(exec_list *instructions,
                              struct _mesa_glsl_parse_state *state)
{
    do_hir(instructions, state, false);
}

void
ast_expression::set_is_lhs(bool new_value)
{
    /* is_lhs is tracked only to print "variable used uninitialized" warnings,
    * if we lack an identifier we can just skip it.
    */
    if (this->primary_expression.identifier == NULL)
        return;

    this->is_lhs = new_value;

    /* We need to go through the subexpressions tree to cover cases like
    * ast_field_selection
    */
    if (this->subexpressions[0] != NULL)
        this->subexpressions[0]->set_is_lhs(new_value);
}

ir_rvalue *
ast_expression::do_hir(exec_list *instructions,
                       struct _mesa_glsl_parse_state *state,
                       bool needs_rvalue)
{
    void *ctx = state;
    static const int operations[AST_NUM_OPERATORS] = {
            -1,               /* ast_assign doesn't convert to ir_expression. */
            -1,               /* ast_plus doesn't convert to ir_expression. */
            ir_unop_neg,
            ir_binop_add,
            ir_binop_sub,
            ir_binop_mul,
            ir_binop_div,
            ir_binop_mod,
            ir_binop_lshift,
            ir_binop_rshift,
            ir_binop_less,
            ir_binop_less,    /* This is correct.  See the ast_greater case below. */
            ir_binop_gequal,  /* This is correct.  See the ast_lequal case below. */
            ir_binop_gequal,
            ir_binop_all_equal,
            ir_binop_any_nequal,
            ir_binop_bit_and,
            ir_binop_bit_xor,
            ir_binop_bit_or,
            ir_unop_bit_not,
            ir_binop_logic_and,
            ir_binop_logic_xor,
            ir_binop_logic_or,
            ir_unop_logic_not,

            /* Note: The following block of expression types actually convert
       * to multiple IR instructions.
       */
            ir_binop_mul,     /* ast_mul_assign */
            ir_binop_div,     /* ast_div_assign */
            ir_binop_mod,     /* ast_mod_assign */
            ir_binop_add,     /* ast_add_assign */
            ir_binop_sub,     /* ast_sub_assign */
            ir_binop_lshift,  /* ast_ls_assign */
            ir_binop_rshift,  /* ast_rs_assign */
            ir_binop_bit_and, /* ast_and_assign */
            ir_binop_bit_xor, /* ast_xor_assign */
            ir_binop_bit_or,  /* ast_or_assign */

            -1,               /* ast_conditional doesn't convert to ir_expression. */
            ir_binop_add,     /* ast_pre_inc. */
            ir_binop_sub,     /* ast_pre_dec. */
            ir_binop_add,     /* ast_post_inc. */
            ir_binop_sub,     /* ast_post_dec. */
            -1,               /* ast_field_selection doesn't conv to ir_expression. */
            -1,               /* ast_array_index doesn't convert to ir_expression. */
            -1,               /* ast_function_call doesn't conv to ir_expression. */
            -1,               /* ast_identifier doesn't convert to ir_expression. */
            -1,               /* ast_int_constant doesn't convert to ir_expression. */
            -1,               /* ast_uint_constant doesn't conv to ir_expression. */
            -1,               /* ast_float_constant doesn't conv to ir_expression. */
            -1,               /* ast_bool_constant doesn't conv to ir_expression. */
            -1,               /* ast_sequence doesn't convert to ir_expression. */
            -1,               /* ast_aggregate shouldn't ever even get here. */
    };
    ir_rvalue *result = NULL;
    ir_rvalue *op[3];
    const struct glsl_type *type, *orig_type;
    bool error_emitted = false;
    YYLTYPE loc;

    loc = this->get_location();

    switch (this->oper) {
        case ast_aggregate:
            unreachable("ast_aggregate: Should never get here.");

        case ast_assign: {
            this->subexpressions[0]->set_is_lhs(true);
            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = this->subexpressions[1]->hir(instructions, state);

            error_emitted =
                    do_assignment(instructions, state,
                                  this->subexpressions[0]->non_lvalue_description,
                                  op[0], op[1], &result, needs_rvalue, false,
                                  this->subexpressions[0]->get_location());
            break;
        }

        case ast_plus:
            op[0] = this->subexpressions[0]->hir(instructions, state);

            type = unary_arithmetic_result_type(op[0]->type, state, & loc);

            error_emitted = type->is_error();

            result = op[0];
            break;

        case ast_neg:
            op[0] = this->subexpressions[0]->hir(instructions, state);

            type = unary_arithmetic_result_type(op[0]->type, state, & loc);

            error_emitted = type->is_error();

            result = new(ctx) ir_expression(operations[this->oper], type,
                                            op[0], NULL);
            break;

        case ast_add:
        case ast_sub:
        case ast_mul:
        case ast_div:
            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = this->subexpressions[1]->hir(instructions, state);

            type = arithmetic_result_type(op[0], op[1],
                                          (this->oper == ast_mul),
                                          state, & loc);
            error_emitted = type->is_error();

            result = new(ctx) ir_expression(operations[this->oper], type,
                                            op[0], op[1]);
            break;

        case ast_mod:
            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = this->subexpressions[1]->hir(instructions, state);

            type = modulus_result_type(op[0], op[1], state, &loc);

            assert(operations[this->oper] == ir_binop_mod);

            result = new(ctx) ir_expression(operations[this->oper], type,
                                            op[0], op[1]);
            error_emitted = type->is_error();
            break;

        case ast_lshift:
        case ast_rshift:
            if (!state->check_bitwise_operations_allowed(&loc)) {
                error_emitted = true;
            }

            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = this->subexpressions[1]->hir(instructions, state);
            type = shift_result_type(op[0]->type, op[1]->type, this->oper, state,
                                     &loc);
            result = new(ctx) ir_expression(operations[this->oper], type,
                                            op[0], op[1]);
            error_emitted = op[0]->type->is_error() || op[1]->type->is_error();
            break;

        case ast_less:
        case ast_greater:
        case ast_lequal:
        case ast_gequal:
            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = this->subexpressions[1]->hir(instructions, state);

            type = relational_result_type(op[0], op[1], state, & loc);

            /* The relational operators must either generate an error or result
       * in a scalar boolean.  See page 57 of the GLSL 1.50 spec.
       */
            assert(type->is_error()
                   || (type->is_boolean() && type->is_scalar()));

            /* Like NIR, GLSL IR does not have opcodes for > or <=.  Instead, swap
       * the arguments and use < or >=.
       */
            if (this->oper == ast_greater || this->oper == ast_lequal) {
                ir_rvalue *const tmp = op[0];
                op[0] = op[1];
                op[1] = tmp;
            }

            result = new(ctx) ir_expression(operations[this->oper], type,
                                            op[0], op[1]);
            error_emitted = type->is_error();
            break;

        case ast_nequal:
        case ast_equal:
            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = this->subexpressions[1]->hir(instructions, state);

            /* From page 58 (page 64 of the PDF) of the GLSL 1.50 spec:
       *
       *    "The equality operators equal (==), and not equal (!=)
       *    operate on all types. They result in a scalar Boolean. If
       *    the operand types do not match, then there must be a
       *    conversion from Section 4.1.10 "Implicit Conversions"
       *    applied to one operand that can make them match, in which
       *    case this conversion is done."
       */

            if (op[0]->type == glsl_type::void_type || op[1]->type == glsl_type::void_type) {
                _mesa_glsl_error(& loc, state, "`%s':  wrong operand types: "
                                               "no operation `%1$s' exists that takes a left-hand "
                                               "operand of type 'void' or a right operand of type "
                                               "'void'", (this->oper == ast_equal) ? "==" : "!=");
                error_emitted = true;
            } else if ((!apply_implicit_conversion(op[0]->type, op[1], state)
                        && !apply_implicit_conversion(op[1]->type, op[0], state))
                       || (op[0]->type != op[1]->type)) {
                _mesa_glsl_error(& loc, state, "operands of `%s' must have the same "
                                               "type", (this->oper == ast_equal) ? "==" : "!=");
                error_emitted = true;
            } else if ((op[0]->type->is_array() || op[1]->type->is_array()) &&
                       !state->check_version(120, 300, &loc,
                                             "array comparisons forbidden")) {
                error_emitted = true;
            } else if ((op[0]->type->contains_subroutine() ||
                        op[1]->type->contains_subroutine())) {
                _mesa_glsl_error(&loc, state, "subroutine comparisons forbidden");
                error_emitted = true;
            } else if ((op[0]->type->contains_opaque() ||
                        op[1]->type->contains_opaque())) {
                _mesa_glsl_error(&loc, state, "opaque type comparisons forbidden");
                error_emitted = true;
            }

            if (error_emitted) {
                result = new(ctx) ir_constant(false);
            } else {
                result = do_comparison(ctx, operations[this->oper], op[0], op[1]);
                assert(result->type == glsl_type::bool_type);
            }
            break;

        case ast_bit_and:
        case ast_bit_xor:
        case ast_bit_or:
            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = this->subexpressions[1]->hir(instructions, state);
            type = bit_logic_result_type(op[0], op[1], this->oper, state, &loc);
            result = new(ctx) ir_expression(operations[this->oper], type,
                                            op[0], op[1]);
            error_emitted = op[0]->type->is_error() || op[1]->type->is_error();
            break;

        case ast_bit_not:
            op[0] = this->subexpressions[0]->hir(instructions, state);

            if (!state->check_bitwise_operations_allowed(&loc)) {
                error_emitted = true;
            }

            if (!op[0]->type->is_integer_32_64()) {
                _mesa_glsl_error(&loc, state, "operand of `~' must be an integer");
                error_emitted = true;
            }

            type = error_emitted ? glsl_type::error_type : op[0]->type;
            result = new(ctx) ir_expression(ir_unop_bit_not, type, op[0], NULL);
            break;

        case ast_logic_and: {
            exec_list rhs_instructions;
            op[0] = get_scalar_boolean_operand(instructions, state, this, 0,
                                               "LHS", &error_emitted);
            op[1] = get_scalar_boolean_operand(&rhs_instructions, state, this, 1,
                                               "RHS", &error_emitted);

            if (rhs_instructions.is_empty()) {
                result = new(ctx) ir_expression(ir_binop_logic_and, op[0], op[1]);
            } else {
                ir_variable *const tmp = new(ctx) ir_variable(glsl_type::bool_type,
                                                              "and_tmp",
                                                              ir_var_temporary);
                instructions->push_tail(tmp);

                ir_if *const stmt = new(ctx) ir_if(op[0]);
                instructions->push_tail(stmt);

                stmt->then_instructions.append_list(&rhs_instructions);
                ir_dereference *const then_deref = new(ctx) ir_dereference_variable(tmp);
                ir_assignment *const then_assign =
                        new(ctx) ir_assignment(then_deref, op[1]);
                stmt->then_instructions.push_tail(then_assign);

                ir_dereference *const else_deref = new(ctx) ir_dereference_variable(tmp);
                ir_assignment *const else_assign =
                        new(ctx) ir_assignment(else_deref, new(ctx) ir_constant(false));
                stmt->else_instructions.push_tail(else_assign);

                result = new(ctx) ir_dereference_variable(tmp);
            }
            break;
        }

        case ast_logic_or: {
            exec_list rhs_instructions;
            op[0] = get_scalar_boolean_operand(instructions, state, this, 0,
                                               "LHS", &error_emitted);
            op[1] = get_scalar_boolean_operand(&rhs_instructions, state, this, 1,
                                               "RHS", &error_emitted);

            if (rhs_instructions.is_empty()) {
                result = new(ctx) ir_expression(ir_binop_logic_or, op[0], op[1]);
            } else {
                ir_variable *const tmp = new(ctx) ir_variable(glsl_type::bool_type,
                                                              "or_tmp",
                                                              ir_var_temporary);
                instructions->push_tail(tmp);

                ir_if *const stmt = new(ctx) ir_if(op[0]);
                instructions->push_tail(stmt);

                ir_dereference *const then_deref = new(ctx) ir_dereference_variable(tmp);
                ir_assignment *const then_assign =
                        new(ctx) ir_assignment(then_deref, new(ctx) ir_constant(true));
                stmt->then_instructions.push_tail(then_assign);

                stmt->else_instructions.append_list(&rhs_instructions);
                ir_dereference *const else_deref = new(ctx) ir_dereference_variable(tmp);
                ir_assignment *const else_assign =
                        new(ctx) ir_assignment(else_deref, op[1]);
                stmt->else_instructions.push_tail(else_assign);

                result = new(ctx) ir_dereference_variable(tmp);
            }
            break;
        }

        case ast_logic_xor:
            /* From page 33 (page 39 of the PDF) of the GLSL 1.10 spec:
       *
       *    "The logical binary operators and (&&), or ( | | ), and
       *     exclusive or (^^). They operate only on two Boolean
       *     expressions and result in a Boolean expression."
       */
            op[0] = get_scalar_boolean_operand(instructions, state, this, 0, "LHS",
                                               &error_emitted);
            op[1] = get_scalar_boolean_operand(instructions, state, this, 1, "RHS",
                                               &error_emitted);

            result = new(ctx) ir_expression(operations[this->oper], glsl_type::bool_type,
                                            op[0], op[1]);
            break;

        case ast_logic_not:
            op[0] = get_scalar_boolean_operand(instructions, state, this, 0,
                                               "operand", &error_emitted);

            result = new(ctx) ir_expression(operations[this->oper], glsl_type::bool_type,
                                            op[0], NULL);
            break;

        case ast_mul_assign:
        case ast_div_assign:
        case ast_add_assign:
        case ast_sub_assign: {
            this->subexpressions[0]->set_is_lhs(true);
            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = this->subexpressions[1]->hir(instructions, state);

            orig_type = op[0]->type;

            /* Break out if operand types were not parsed successfully. */
            if ((op[0]->type == glsl_type::error_type ||
                 op[1]->type == glsl_type::error_type)) {
                error_emitted = true;
                break;
            }

            type = arithmetic_result_type(op[0], op[1],
                                          (this->oper == ast_mul_assign),
                                          state, & loc);

            if (type != orig_type) {
                _mesa_glsl_error(& loc, state,
                                 "could not implicitly convert "
                                 "%s to %s", type->name, orig_type->name);
                type = glsl_type::error_type;
            }

            ir_rvalue *temp_rhs = new(ctx) ir_expression(operations[this->oper], type,
                                                         op[0], op[1]);

            error_emitted =
                    do_assignment(instructions, state,
                                  this->subexpressions[0]->non_lvalue_description,
                                  op[0]->clone(ctx, NULL), temp_rhs,
                                  &result, needs_rvalue, false,
                                  this->subexpressions[0]->get_location());

            /* GLSL 1.10 does not allow array assignment.  However, we don't have to
       * explicitly test for this because none of the binary expression
       * operators allow array operands either.
       */

            break;
        }

        case ast_mod_assign: {
            this->subexpressions[0]->set_is_lhs(true);
            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = this->subexpressions[1]->hir(instructions, state);

            orig_type = op[0]->type;
            type = modulus_result_type(op[0], op[1], state, &loc);

            if (type != orig_type) {
                _mesa_glsl_error(& loc, state,
                                 "could not implicitly convert "
                                 "%s to %s", type->name, orig_type->name);
                type = glsl_type::error_type;
            }

            assert(operations[this->oper] == ir_binop_mod);

            ir_rvalue *temp_rhs;
            temp_rhs = new(ctx) ir_expression(operations[this->oper], type,
                                              op[0], op[1]);

            error_emitted =
                    do_assignment(instructions, state,
                                  this->subexpressions[0]->non_lvalue_description,
                                  op[0]->clone(ctx, NULL), temp_rhs,
                                  &result, needs_rvalue, false,
                                  this->subexpressions[0]->get_location());
            break;
        }

        case ast_ls_assign:
        case ast_rs_assign: {
            this->subexpressions[0]->set_is_lhs(true);
            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = this->subexpressions[1]->hir(instructions, state);
            type = shift_result_type(op[0]->type, op[1]->type, this->oper, state,
                                     &loc);
            ir_rvalue *temp_rhs = new(ctx) ir_expression(operations[this->oper],
                                                         type, op[0], op[1]);
            error_emitted =
                    do_assignment(instructions, state,
                                  this->subexpressions[0]->non_lvalue_description,
                                  op[0]->clone(ctx, NULL), temp_rhs,
                                  &result, needs_rvalue, false,
                                  this->subexpressions[0]->get_location());
            break;
        }

        case ast_and_assign:
        case ast_xor_assign:
        case ast_or_assign: {
            this->subexpressions[0]->set_is_lhs(true);
            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = this->subexpressions[1]->hir(instructions, state);

            orig_type = op[0]->type;
            type = bit_logic_result_type(op[0], op[1], this->oper, state, &loc);

            if (type != orig_type) {
                _mesa_glsl_error(& loc, state,
                                 "could not implicitly convert "
                                 "%s to %s", type->name, orig_type->name);
                type = glsl_type::error_type;
            }

            ir_rvalue *temp_rhs = new(ctx) ir_expression(operations[this->oper],
                                                         type, op[0], op[1]);
            error_emitted =
                    do_assignment(instructions, state,
                                  this->subexpressions[0]->non_lvalue_description,
                                  op[0]->clone(ctx, NULL), temp_rhs,
                                  &result, needs_rvalue, false,
                                  this->subexpressions[0]->get_location());
            break;
        }

        case ast_conditional: {
            /* From page 59 (page 65 of the PDF) of the GLSL 1.50 spec:
       *
       *    "The ternary selection operator (?:). It operates on three
       *    expressions (exp1 ? exp2 : exp3). This operator evaluates the
       *    first expression, which must result in a scalar Boolean."
       */
            op[0] = get_scalar_boolean_operand(instructions, state, this, 0,
                                               "condition", &error_emitted);

            /* The :? operator is implemented by generating an anonymous temporary
       * followed by an if-statement.  The last instruction in each branch of
       * the if-statement assigns a value to the anonymous temporary.  This
       * temporary is the r-value of the expression.
       */
            exec_list then_instructions;
            exec_list else_instructions;

            op[1] = this->subexpressions[1]->hir(&then_instructions, state);
            op[2] = this->subexpressions[2]->hir(&else_instructions, state);

            /* From page 59 (page 65 of the PDF) of the GLSL 1.50 spec:
       *
       *     "The second and third expressions can be any type, as
       *     long their types match, or there is a conversion in
       *     Section 4.1.10 "Implicit Conversions" that can be applied
       *     to one of the expressions to make their types match. This
       *     resulting matching type is the type of the entire
       *     expression."
       */
            if ((!apply_implicit_conversion(op[1]->type, op[2], state)
                 && !apply_implicit_conversion(op[2]->type, op[1], state))
                || (op[1]->type != op[2]->type)) {
                YYLTYPE loc = this->subexpressions[1]->get_location();

                _mesa_glsl_error(& loc, state, "second and third operands of ?: "
                                               "operator must have matching types");
                error_emitted = true;
                type = glsl_type::error_type;
            } else {
                type = op[1]->type;
            }

            /* From page 33 (page 39 of the PDF) of the GLSL 1.10 spec:
       *
       *    "The second and third expressions must be the same type, but can
       *    be of any type other than an array."
       */
            if (type->is_array() &&
                !state->check_version(120, 300, &loc,
                                      "second and third operands of ?: operator "
                                      "cannot be arrays")) {
                error_emitted = true;
            }

            /* From section 4.1.7 of the GLSL 4.50 spec (Opaque Types):
       *
       *  "Except for array indexing, structure member selection, and
       *   parentheses, opaque variables are not allowed to be operands in
       *   expressions; such use results in a compile-time error."
       */
            if (type->contains_opaque()) {
                if (!(state->has_bindless() && (type->is_image() || type->is_sampler()))) {
                    _mesa_glsl_error(&loc, state, "variables of type %s cannot be "
                                                  "operands of the ?: operator", type->name);
                    error_emitted = true;
                }
            }

            ir_constant *cond_val = op[0]->constant_expression_value(ctx);

            if (then_instructions.is_empty()
                && else_instructions.is_empty()
                && cond_val != NULL) {
                result = cond_val->value.b[0] ? op[1] : op[2];
            } else {
                /* The copy to conditional_tmp reads the whole array. */
                if (type->is_array()) {
                    mark_whole_array_access(op[1]);
                    mark_whole_array_access(op[2]);
                }

                ir_variable *const tmp =
                        new(ctx) ir_variable(type, "conditional_tmp", ir_var_temporary);
                instructions->push_tail(tmp);

                ir_if *const stmt = new(ctx) ir_if(op[0]);
                instructions->push_tail(stmt);

                then_instructions.move_nodes_to(& stmt->then_instructions);
                ir_dereference *const then_deref =
                        new(ctx) ir_dereference_variable(tmp);
                ir_assignment *const then_assign =
                        new(ctx) ir_assignment(then_deref, op[1]);
                stmt->then_instructions.push_tail(then_assign);

                else_instructions.move_nodes_to(& stmt->else_instructions);
                ir_dereference *const else_deref =
                        new(ctx) ir_dereference_variable(tmp);
                ir_assignment *const else_assign =
                        new(ctx) ir_assignment(else_deref, op[2]);
                stmt->else_instructions.push_tail(else_assign);

                result = new(ctx) ir_dereference_variable(tmp);
            }
            break;
        }

        case ast_pre_inc:
        case ast_pre_dec: {
            this->non_lvalue_description = (this->oper == ast_pre_inc)
                                           ? "pre-increment operation" : "pre-decrement operation";

            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = constant_one_for_inc_dec(ctx, op[0]->type);

            type = arithmetic_result_type(op[0], op[1], false, state, & loc);

            ir_rvalue *temp_rhs;
            temp_rhs = new(ctx) ir_expression(operations[this->oper], type,
                                              op[0], op[1]);

            error_emitted =
                    do_assignment(instructions, state,
                                  this->subexpressions[0]->non_lvalue_description,
                                  op[0]->clone(ctx, NULL), temp_rhs,
                                  &result, needs_rvalue, false,
                                  this->subexpressions[0]->get_location());
            break;
        }

        case ast_post_inc:
        case ast_post_dec: {
            this->non_lvalue_description = (this->oper == ast_post_inc)
                                           ? "post-increment operation" : "post-decrement operation";
            op[0] = this->subexpressions[0]->hir(instructions, state);
            op[1] = constant_one_for_inc_dec(ctx, op[0]->type);

            error_emitted = op[0]->type->is_error() || op[1]->type->is_error();

            if (error_emitted) {
                result = ir_rvalue::error_value(ctx);
                break;
            }

            type = arithmetic_result_type(op[0], op[1], false, state, & loc);

            ir_rvalue *temp_rhs;
            temp_rhs = new(ctx) ir_expression(operations[this->oper], type,
                                              op[0], op[1]);

            /* Get a temporary of a copy of the lvalue before it's modified.
       * This may get thrown away later.
       */
            result = get_lvalue_copy(instructions, op[0]->clone(ctx, NULL));

            ir_rvalue *junk_rvalue;
            error_emitted =
                    do_assignment(instructions, state,
                                  this->subexpressions[0]->non_lvalue_description,
                                  op[0]->clone(ctx, NULL), temp_rhs,
                                  &junk_rvalue, false, false,
                                  this->subexpressions[0]->get_location());

            break;
        }

        case ast_field_selection:
            result = _mesa_ast_field_selection_to_hir(this, instructions, state);
            break;

        case ast_array_index: {
            YYLTYPE index_loc = subexpressions[1]->get_location();

            /* Getting if an array is being used uninitialized is beyond what we get
       * from ir_value.data.assigned. Setting is_lhs as true would force to
       * not raise a uninitialized warning when using an array
       */
            subexpressions[0]->set_is_lhs(true);
            op[0] = subexpressions[0]->hir(instructions, state);
            op[1] = subexpressions[1]->hir(instructions, state);

            result = _mesa_ast_array_index_to_hir(ctx, state, op[0], op[1],
                                                  loc, index_loc);

            if (result->type->is_error())
                error_emitted = true;

            break;
        }

        case ast_unsized_array_dim:
            unreachable("ast_unsized_array_dim: Should never get here.");

        case ast_function_call:
            /* Should *NEVER* get here.  ast_function_call should always be handled
       * by ast_function_expression::hir.
       */
            unreachable("ast_function_call: handled elsewhere ");

        case ast_identifier: {
            /* ast_identifier can appear several places in a full abstract syntax
       * tree.  This particular use must be at location specified in the grammar
       * as 'variable_identifier'.
       */
            ir_variable *var =
                    state->symbols->get_variable(this->primary_expression.identifier);

            if (var == NULL) {
                /* the identifier might be a subroutine name */
                char *sub_name;
                sub_name = ralloc_asprintf(ctx, "%s_%s", _mesa_shader_stage_to_subroutine_prefix(state->stage), this->primary_expression.identifier);
                var = state->symbols->get_variable(sub_name);
                ralloc_free(sub_name);
            }

            if (var != NULL) {
                var->data.used = true;
                result = new(ctx) ir_dereference_variable(var);

                if ((var->data.mode == ir_var_auto || var->data.mode == ir_var_shader_out)
                    && !this->is_lhs
                    && result->variable_referenced()->data.assigned != true
                    && !is_gl_identifier(var->name)) {
                    _mesa_glsl_warning(&loc, state, "`%s' used uninitialized",
                                       this->primary_expression.identifier);
                }

                /* From the EXT_shader_framebuffer_fetch spec:
          *
          *   "Unless the GL_EXT_shader_framebuffer_fetch extension has been
          *    enabled in addition, it's an error to use gl_LastFragData if it
          *    hasn't been explicitly redeclared with layout(noncoherent)."
          */
                if (var->data.fb_fetch_output && var->data.memory_coherent &&
                    !state->EXT_shader_framebuffer_fetch_enable) {
                    _mesa_glsl_error(&loc, state,
                                     "invalid use of framebuffer fetch output not "
                                     "qualified with layout(noncoherent)");
                }

            } else {
                _mesa_glsl_error(& loc, state, "`%s' undeclared",
                                 this->primary_expression.identifier);

                result = ir_rvalue::error_value(ctx);
                error_emitted = true;
            }
            break;
        }

        case ast_int_constant:
            result = new(ctx) ir_constant(this->primary_expression.int_constant);
            break;

        case ast_uint_constant:
            result = new(ctx) ir_constant(this->primary_expression.uint_constant);
            break;

        case ast_float_constant:
            result = new(ctx) ir_constant(this->primary_expression.float_constant);
            break;

        case ast_bool_constant:
            result = new(ctx) ir_constant(bool(this->primary_expression.bool_constant));
            break;

        case ast_double_constant:
            result = new(ctx) ir_constant(this->primary_expression.double_constant);
            break;

        case ast_uint64_constant:
            result = new(ctx) ir_constant(this->primary_expression.uint64_constant);
            break;

        case ast_int64_constant:
            result = new(ctx) ir_constant(this->primary_expression.int64_constant);
            break;

        case ast_sequence: {
            /* It should not be possible to generate a sequence in the AST without
       * any expressions in it.
       */
            assert(!this->expressions.is_empty());

            /* The r-value of a sequence is the last expression in the sequence.  If
       * the other expressions in the sequence do not have side-effects (and
       * therefore add instructions to the instruction list), they get dropped
       * on the floor.
       */
            exec_node *previous_tail = NULL;
            YYLTYPE previous_operand_loc = loc;

            foreach_list_typed (ast_node, ast, link, &this->expressions) {
                /* If one of the operands of comma operator does not generate any
          * code, we want to emit a warning.  At each pass through the loop
          * previous_tail will point to the last instruction in the stream
          * *before* processing the previous operand.  Naturally,
          * instructions->get_tail_raw() will point to the last instruction in
          * the stream *after* processing the previous operand.  If the two
          * pointers match, then the previous operand had no effect.
          *
          * The warning behavior here differs slightly from GCC.  GCC will
          * only emit a warning if none of the left-hand operands have an
          * effect.  However, it will emit a warning for each.  I believe that
          * there are some cases in C (especially with GCC extensions) where
          * it is useful to have an intermediate step in a sequence have no
          * effect, but I don't think these cases exist in GLSL.  Either way,
          * it would be a giant hassle to replicate that behavior.
          */
                if (previous_tail == instructions->get_tail_raw()) {
                    _mesa_glsl_warning(&previous_operand_loc, state,
                                       "left-hand operand of comma expression has "
                                       "no effect");
                }

                /* The tail is directly accessed instead of using the get_tail()
          * method for performance reasons.  get_tail() has extra code to
          * return NULL when the list is empty.  We don't care about that
          * here, so using get_tail_raw() is fine.
          */
                previous_tail = instructions->get_tail_raw();
                previous_operand_loc = ast->get_location();

                result = ast->hir(instructions, state);
            }

            /* Any errors should have already been emitted in the loop above.
       */
            error_emitted = true;
            break;
        }
    }
    type = NULL; /* use result->type, not type. */
    assert(error_emitted || (result != NULL || !needs_rvalue));

    if (result && result->type->is_error() && !error_emitted)
        _mesa_glsl_error(& loc, state, "type mismatch");

    return result;
}

bool
ast_expression::has_sequence_subexpression() const
{
    switch (this->oper) {
        case ast_plus:
        case ast_neg:
        case ast_bit_not:
        case ast_logic_not:
        case ast_pre_inc:
        case ast_pre_dec:
        case ast_post_inc:
        case ast_post_dec:
            return this->subexpressions[0]->has_sequence_subexpression();

        case ast_assign:
        case ast_add:
        case ast_sub:
        case ast_mul:
        case ast_div:
        case ast_mod:
        case ast_lshift:
        case ast_rshift:
        case ast_less:
        case ast_greater:
        case ast_lequal:
        case ast_gequal:
        case ast_nequal:
        case ast_equal:
        case ast_bit_and:
        case ast_bit_xor:
        case ast_bit_or:
        case ast_logic_and:
        case ast_logic_or:
        case ast_logic_xor:
        case ast_array_index:
        case ast_mul_assign:
        case ast_div_assign:
        case ast_add_assign:
        case ast_sub_assign:
        case ast_mod_assign:
        case ast_ls_assign:
        case ast_rs_assign:
        case ast_and_assign:
        case ast_xor_assign:
        case ast_or_assign:
            return this->subexpressions[0]->has_sequence_subexpression() ||
                   this->subexpressions[1]->has_sequence_subexpression();

        case ast_conditional:
            return this->subexpressions[0]->has_sequence_subexpression() ||
                   this->subexpressions[1]->has_sequence_subexpression() ||
                   this->subexpressions[2]->has_sequence_subexpression();

        case ast_sequence:
            return true;

        case ast_field_selection:
        case ast_identifier:
        case ast_int_constant:
        case ast_uint_constant:
        case ast_float_constant:
        case ast_bool_constant:
        case ast_double_constant:
        case ast_int64_constant:
        case ast_uint64_constant:
            return false;

        case ast_aggregate:
            return false;

        case ast_function_call:
            unreachable("should be handled by ast_function_expression::hir");

        case ast_unsized_array_dim:
            unreachable("ast_unsized_array_dim: Should never get here.");
    }

    return false;
}

ir_rvalue *
ast_expression_statement::hir(exec_list *instructions,
                              struct _mesa_glsl_parse_state *state)
{
    /* It is possible to have expression statements that don't have an
    * expression.  This is the solitary semicolon:
    *
    * for (i = 0; i < 5; i++)
    *     ;
    *
    * In this case the expression will be NULL.  Test for NULL and don't do
    * anything in that case.
    */
    if (expression != NULL)
        expression->hir_no_rvalue(instructions, state);

    /* Statements do not have r-values.
    */
    return NULL;
}


ir_rvalue *
ast_compound_statement::hir(exec_list *instructions,
                            struct _mesa_glsl_parse_state *state)
{
    if (new_scope)
        state->symbols->push_scope();

    foreach_list_typed (ast_node, ast, link, &this->statements)
        ast->hir(instructions, state);

    if (new_scope)
        state->symbols->pop_scope();

    /* Compound statements do not have r-values.
    */
    return NULL;
}

/**
 * Evaluate the given exec_node (which should be an ast_node representing
 * a single array dimension) and return its integer value.
 */
static unsigned
process_array_size(exec_node *node,
                   struct _mesa_glsl_parse_state *state)
{
    void *mem_ctx = state;

    exec_list dummy_instructions;

    ast_node *array_size = exec_node_data(ast_node, node, link);

    /**
    * Dimensions other than the outermost dimension can by unsized if they
    * are immediately sized by a constructor or initializer.
    */
    if (((ast_expression*)array_size)->oper == ast_unsized_array_dim)
        return 0;

    ir_rvalue *const ir = array_size->hir(& dummy_instructions, state);
    YYLTYPE loc = array_size->get_location();

    if (ir == NULL) {
        _mesa_glsl_error(& loc, state,
                         "array size could not be resolved");
        return 0;
    }

    if (!ir->type->is_integer_32()) {
        _mesa_glsl_error(& loc, state,
                         "array size must be integer type");
        return 0;
    }

    if (!ir->type->is_scalar()) {
        _mesa_glsl_error(& loc, state,
                         "array size must be scalar type");
        return 0;
    }

    ir_constant *const size = ir->constant_expression_value(mem_ctx);
    if (size == NULL ||
        (state->is_version(120, 300) &&
         array_size->has_sequence_subexpression())) {
        _mesa_glsl_error(& loc, state, "array size must be a "
                                       "constant valued expression");
        return 0;
    }

    if (size->value.i[0] <= 0) {
        _mesa_glsl_error(& loc, state, "array size must be > 0");
        return 0;
    }

    assert(size->type == ir->type);

    /* If the array size is const (and we've verified that
    * it is) then no instructions should have been emitted
    * when we converted it to HIR. If they were emitted,
    * then either the array size isn't const after all, or
    * we are emitting unnecessary instructions.
    */
    assert(dummy_instructions.is_empty());

    return size->value.u[0];
}

static const glsl_type *
process_array_type(YYLTYPE *loc, const glsl_type *base,
                   ast_array_specifier *array_specifier,
                   struct _mesa_glsl_parse_state *state)
{
    const glsl_type *array_type = base;

    if (array_specifier != NULL) {
        if (base->is_array()) {

            /* From page 19 (page 25) of the GLSL 1.20 spec:
          *
          * "Only one-dimensional arrays may be declared."
          */
            if (!state->check_arrays_of_arrays_allowed(loc)) {
                return glsl_type::error_type;
            }
        }

        for (exec_node *node = array_specifier->array_dimensions.get_tail_raw();
             !node->is_head_sentinel(); node = node->prev) {
            unsigned array_size = process_array_size(node, state);
            array_type = glsl_type::get_array_instance(array_type, array_size);
        }
    }

    return array_type;
}

static bool
precision_qualifier_allowed(const glsl_type *type)
{
    /* Precision qualifiers apply to floating point, integer and opaque
    * types.
    *
    * Section 4.5.2 (Precision Qualifiers) of the GLSL 1.30 spec says:
    *    "Any floating point or any integer declaration can have the type
    *    preceded by one of these precision qualifiers [...] Literal
    *    constants do not have precision qualifiers. Neither do Boolean
    *    variables.
    *
    * Section 4.5 (Precision and Precision Qualifiers) of the GLSL 1.30
    * spec also says:
    *
    *     "Precision qualifiers are added for code portability with OpenGL
    *     ES, not for functionality. They have the same syntax as in OpenGL
    *     ES."
    *
    * Section 8 (Built-In Functions) of the GLSL ES 1.00 spec says:
    *
    *     "uniform lowp sampler2D sampler;
    *     highp vec2 coord;
    *     ...
    *     lowp vec4 col = texture2D (sampler, coord);
    *                                            // texture2D returns lowp"
    *
    * From this, we infer that GLSL 1.30 (and later) should allow precision
    * qualifiers on sampler types just like float and integer types.
    */
    const glsl_type *const t = type->without_array();

    return (t->is_float() || t->is_integer_32() || t->contains_opaque()) &&
           !t->is_struct();
}

const glsl_type *
ast_type_specifier::glsl_type(const char **name,
                              struct _mesa_glsl_parse_state *state) const
{
    const struct glsl_type *type;

    if (this->type != NULL)
        type = this->type;
    else if (structure)
        type = structure->type;
    else
        type = state->symbols->get_type(this->type_name);
    *name = this->type_name;

    YYLTYPE loc = this->get_location();
    type = process_array_type(&loc, type, this->array_specifier, state);

    return type;
}

/**
 * From the OpenGL ES 3.0 spec, 4.5.4 Default Precision Qualifiers:
 *
 * "The precision statement
 *
 *    precision precision-qualifier type;
 *
 *  can be used to establish a default precision qualifier. The type field can
 *  be either int or float or any of the sampler types, (...) If type is float,
 *  the directive applies to non-precision-qualified floating point type
 *  (scalar, vector, and matrix) declarations. If type is int, the directive
 *  applies to all non-precision-qualified integer type (scalar, vector, signed,
 *  and unsigned) declarations."
 *
 * We use the symbol table to keep the values of the default precisions for
 * each 'type' in each scope and we use the 'type' string from the precision
 * statement as key in the symbol table. When we want to retrieve the default
 * precision associated with a given glsl_type we need to know the type string
 * associated with it. This is what this function returns.
 */
static const char *
get_type_name_for_precision_qualifier(const glsl_type *type)
{
    switch (type->base_type) {
        case GLSL_TYPE_FLOAT:
            return "float";
        case GLSL_TYPE_UINT:
        case GLSL_TYPE_INT:
            return "int";
        case GLSL_TYPE_ATOMIC_UINT:
            return "atomic_uint";
        case GLSL_TYPE_IMAGE:
            /* fallthrough */
        case GLSL_TYPE_SAMPLER: {
            const unsigned type_idx =
                    type->sampler_array + 2 * type->sampler_shadow;
            const unsigned offset = type->is_sampler() ? 0 : 4;
            assert(type_idx < 4);
            switch (type->sampled_type) {
                case GLSL_TYPE_FLOAT:
                    switch (type->sampler_dimensionality) {
                        case GLSL_SAMPLER_DIM_1D: {
                            assert(type->is_sampler());
                            static const char *const names[4] = {
                                    "sampler1D", "sampler1DArray",
                                    "sampler1DShadow", "sampler1DArrayShadow"
                            };
                            return names[type_idx];
                        }
                        case GLSL_SAMPLER_DIM_2D: {
                            static const char *const names[8] = {
                                    "sampler2D", "sampler2DArray",
                                    "sampler2DShadow", "sampler2DArrayShadow",
                                    "image2D", "image2DArray", NULL, NULL
                            };
                            return names[offset + type_idx];
                        }
                        case GLSL_SAMPLER_DIM_3D: {
                            static const char *const names[8] = {
                                    "sampler3D", NULL, NULL, NULL,
                                    "image3D", NULL, NULL, NULL
                            };
                            return names[offset + type_idx];
                        }
                        case GLSL_SAMPLER_DIM_CUBE: {
                            static const char *const names[8] = {
                                    "samplerCube", "samplerCubeArray",
                                    "samplerCubeShadow", "samplerCubeArrayShadow",
                                    "imageCube", NULL, NULL, NULL
                            };
                            return names[offset + type_idx];
                        }
                        case GLSL_SAMPLER_DIM_MS: {
                            assert(type->is_sampler());
                            static const char *const names[4] = {
                                    "sampler2DMS", "sampler2DMSArray", NULL, NULL
                            };
                            return names[type_idx];
                        }
                        case GLSL_SAMPLER_DIM_RECT: {
                            assert(type->is_sampler());
                            static const char *const names[4] = {
                                    "samplerRect", NULL, "samplerRectShadow", NULL
                            };
                            return names[type_idx];
                        }
                        case GLSL_SAMPLER_DIM_BUF: {
                            static const char *const names[8] = {
                                    "samplerBuffer", NULL, NULL, NULL,
                                    "imageBuffer", NULL, NULL, NULL
                            };
                            return names[offset + type_idx];
                        }
                        case GLSL_SAMPLER_DIM_EXTERNAL: {
                            assert(type->is_sampler());
                            static const char *const names[4] = {
                                    "samplerExternalOES", NULL, NULL, NULL
                            };
                            return names[type_idx];
                        }
                        default:
                            unreachable("Unsupported sampler/image dimensionality");
                    } /* sampler/image float dimensionality */
                    break;
                case GLSL_TYPE_INT:
                    switch (type->sampler_dimensionality) {
                        case GLSL_SAMPLER_DIM_1D: {
                            assert(type->is_sampler());
                            static const char *const names[4] = {
                                    "isampler1D", "isampler1DArray", NULL, NULL
                            };
                            return names[type_idx];
                        }
                        case GLSL_SAMPLER_DIM_2D: {
                            static const char *const names[8] = {
                                    "isampler2D", "isampler2DArray", NULL, NULL,
                                    "iimage2D", "iimage2DArray", NULL, NULL
                            };
                            return names[offset + type_idx];
                        }
                        case GLSL_SAMPLER_DIM_3D: {
                            static const char *const names[8] = {
                                    "isampler3D", NULL, NULL, NULL,
                                    "iimage3D", NULL, NULL, NULL
                            };
                            return names[offset + type_idx];
                        }
                        case GLSL_SAMPLER_DIM_CUBE: {
                            static const char *const names[8] = {
                                    "isamplerCube", "isamplerCubeArray", NULL, NULL,
                                    "iimageCube", NULL, NULL, NULL
                            };
                            return names[offset + type_idx];
                        }
                        case GLSL_SAMPLER_DIM_MS: {
                            assert(type->is_sampler());
                            static const char *const names[4] = {
                                    "isampler2DMS", "isampler2DMSArray", NULL, NULL
                            };
                            return names[type_idx];
                        }
                        case GLSL_SAMPLER_DIM_RECT: {
                            assert(type->is_sampler());
                            static const char *const names[4] = {
                                    "isamplerRect", NULL, "isamplerRectShadow", NULL
                            };
                            return names[type_idx];
                        }
                        case GLSL_SAMPLER_DIM_BUF: {
                            static const char *const names[8] = {
                                    "isamplerBuffer", NULL, NULL, NULL,
                                    "iimageBuffer", NULL, NULL, NULL
                            };
                            return names[offset + type_idx];
                        }
                        default:
                            unreachable("Unsupported isampler/iimage dimensionality");
                    } /* sampler/image int dimensionality */
                    break;
                case GLSL_TYPE_UINT:
                    switch (type->sampler_dimensionality) {
                        case GLSL_SAMPLER_DIM_1D: {
                            assert(type->is_sampler());
                            static const char *const names[4] = {
                                    "usampler1D", "usampler1DArray", NULL, NULL
                            };
                            return names[type_idx];
                        }
                        case GLSL_SAMPLER_DIM_2D: {
                            static const char *const names[8] = {
                                    "usampler2D", "usampler2DArray", NULL, NULL,
                                    "uimage2D", "uimage2DArray", NULL, NULL
                            };
                            return names[offset + type_idx];
                        }
                        case GLSL_SAMPLER_DIM_3D: {
                            static const char *const names[8] = {
                                    "usampler3D", NULL, NULL, NULL,
                                    "uimage3D", NULL, NULL, NULL
                            };
                            return names[offset + type_idx];
                        }
                        case GLSL_SAMPLER_DIM_CUBE: {
                            static const char *const names[8] = {
                                    "usamplerCube", "usamplerCubeArray", NULL, NULL,
                                    "uimageCube", NULL, NULL, NULL
                            };
                            return names[offset + type_idx];
                        }
                        case GLSL_SAMPLER_DIM_MS: {
                            assert(type->is_sampler());
                            static const char *const names[4] = {
                                    "usampler2DMS", "usampler2DMSArray", NULL, NULL
                            };
                            return names[type_idx];
                        }
                        case GLSL_SAMPLER_DIM_RECT: {
                            assert(type->is_sampler());
                            static const char *const names[4] = {
                                    "usamplerRect", NULL, "usamplerRectShadow", NULL
                            };
                            return names[type_idx];
                        }
                        case GLSL_SAMPLER_DIM_BUF: {
                            static const char *const names[8] = {
                                    "usamplerBuffer", NULL, NULL, NULL,
                                    "uimageBuffer", NULL, NULL, NULL
                            };
                            return names[offset + type_idx];
                        }
                        default:
                            unreachable("Unsupported usampler/uimage dimensionality");
                    } /* sampler/image uint dimensionality */
                    break;
                default:
                    unreachable("Unsupported sampler/image type");
            } /* sampler/image type */
            break;
        } /* GLSL_TYPE_SAMPLER/GLSL_TYPE_IMAGE */
            break;
        default:
            unreachable("Unsupported type");
    } /* base type */
}

static unsigned
select_gles_precision(unsigned qual_precision,
                      const glsl_type *type,
                      struct _mesa_glsl_parse_state *state, YYLTYPE *loc)
{
    /* Precision qualifiers do not have any meaning in Desktop GLSL.
    * In GLES we take the precision from the type qualifier if present,
    * otherwise, if the type of the variable allows precision qualifiers at
    * all, we look for the default precision qualifier for that type in the
    * current scope.
    */
    assert(state->es_shader);

    unsigned precision = GLSL_PRECISION_NONE;
    if (qual_precision) {
        precision = qual_precision;
    } else if (precision_qualifier_allowed(type)) {
        const char *type_name =
                get_type_name_for_precision_qualifier(type->without_array());
        assert(type_name != NULL);

        precision =
                state->symbols->get_default_precision_qualifier(type_name);
        if (precision == ast_precision_none) {
            _mesa_glsl_error(loc, state,
                             "No precision specified in this scope for type `%s'",
                             type->name);
        }
    }


    /* Section 4.1.7.3 (Atomic Counters) of the GLSL ES 3.10 spec says:
    *
    *    "The default precision of all atomic types is highp. It is an error to
    *    declare an atomic type with a different precision or to specify the
    *    default precision for an atomic type to be lowp or mediump."
    */
    if (type->is_atomic_uint() && precision != ast_precision_high) {
        _mesa_glsl_error(loc, state,
                         "atomic_uint can only have highp precision qualifier");
    }

    return precision;
}

const glsl_type *
ast_fully_specified_type::glsl_type(const char **name,
                                    struct _mesa_glsl_parse_state *state) const
{
    return this->specifier->glsl_type(name, state);
}

/**
 * Determine whether a toplevel variable declaration declares a varying.  This
 * function operates by examining the variable's mode and the shader target,
 * so it correctly identifies linkage variables regardless of whether they are
 * declared using the deprecated "varying" syntax or the new "in/out" syntax.
 *
 * Passing a non-toplevel variable declaration (e.g. a function parameter) to
 * this function will produce undefined results.
 */
static bool
is_varying_var(ir_variable *var, gl_shader_stage target)
{
    switch (target) {
        case MESA_SHADER_VERTEX:
            return var->data.mode == ir_var_shader_out;
        case MESA_SHADER_FRAGMENT:
            return var->data.mode == ir_var_shader_in ||
                   (var->data.mode == ir_var_system_value &&
                    var->data.location == SYSTEM_VALUE_FRAG_COORD);
        default:
            return var->data.mode == ir_var_shader_out || var->data.mode == ir_var_shader_in;
    }
}

static bool
is_allowed_invariant(ir_variable *var, struct _mesa_glsl_parse_state *state)
{
    if (is_varying_var(var, state->stage))
        return true;

    /* From Section 4.6.1 ("The Invariant Qualifier") GLSL 1.20 spec:
    * "Only variables output from a vertex shader can be candidates
    * for invariance".
    */
    if (!state->is_version(130, 100))
        return false;

    /*
    * Later specs remove this language - so allowed invariant
    * on fragment shader outputs as well.
    */
    if (state->stage == MESA_SHADER_FRAGMENT &&
        var->data.mode == ir_var_shader_out)
        return true;
    return false;
}

static void
validate_component_layout_for_type(struct _mesa_glsl_parse_state *state,
                                   YYLTYPE *loc, const glsl_type *type,
                                   unsigned qual_component)
{
    type = type->without_array();
    unsigned components = type->component_slots();

    if (type->is_matrix() || type->is_struct()) {
        _mesa_glsl_error(loc, state, "component layout qualifier "
                                     "cannot be applied to a matrix, a structure, "
                                     "a block, or an array containing any of these.");
    } else if (components > 4 && type->is_64bit()) {
        _mesa_glsl_error(loc, state, "component layout qualifier "
                                     "cannot be applied to dvec%u.",
                         components / 2);
    } else if (qual_component != 0 && (qual_component + components - 1) > 3) {
        _mesa_glsl_error(loc, state, "component overflow (%u > 3)",
                         (qual_component + components - 1));
    } else if (qual_component == 1 && type->is_64bit()) {
        /* We don't bother checking for 3 as it should be caught by the
       * overflow check above.
       */
        _mesa_glsl_error(loc, state, "doubles cannot begin at component 1 or 3");
    }
}

/**
 * Matrix layout qualifiers are only allowed on certain types
 */
static void
validate_matrix_layout_for_type(struct _mesa_glsl_parse_state *state,
                                YYLTYPE *loc,
                                const glsl_type *type,
                                ir_variable *var)
{
    if (var && !var->is_in_buffer_block()) {
        /* Layout qualifiers may only apply to interface blocks and fields in
       * them.
       */
        _mesa_glsl_error(loc, state,
                         "uniform block layout qualifiers row_major and "
                         "column_major may not be applied to variables "
                         "outside of uniform blocks");
    } else if (!type->without_array()->is_matrix()) {
        /* The OpenGL ES 3.0 conformance tests did not originally allow
       * matrix layout qualifiers on non-matrices.  However, the OpenGL
       * 4.4 and OpenGL ES 3.0 (revision TBD) specifications were
       * amended to specifically allow these layouts on all types.  Emit
       * a warning so that people know their code may not be portable.
       */
        _mesa_glsl_warning(loc, state,
                           "uniform block layout qualifiers row_major and "
                           "column_major applied to non-matrix types may "
                           "be rejected by older compilers");
    }
}

static bool
validate_xfb_buffer_qualifier(YYLTYPE *loc,
                              struct _mesa_glsl_parse_state *state,
                              unsigned xfb_buffer) {
    if (xfb_buffer >= state->Const.MaxTransformFeedbackBuffers) {
        _mesa_glsl_error(loc, state,
                         "invalid xfb_buffer specified %d is larger than "
                         "MAX_TRANSFORM_FEEDBACK_BUFFERS - 1 (%d).",
                         xfb_buffer,
                         state->Const.MaxTransformFeedbackBuffers - 1);
        return false;
    }

    return true;
}

/* From the ARB_enhanced_layouts spec:
 *
 *    "Variables and block members qualified with *xfb_offset* can be
 *    scalars, vectors, matrices, structures, and (sized) arrays of these.
 *    The offset must be a multiple of the size of the first component of
 *    the first qualified variable or block member, or a compile-time error
 *    results.  Further, if applied to an aggregate containing a double,
 *    the offset must also be a multiple of 8, and the space taken in the
 *    buffer will be a multiple of 8.
 */
static bool
validate_xfb_offset_qualifier(YYLTYPE *loc,
                              struct _mesa_glsl_parse_state *state,
                              int xfb_offset, const glsl_type *type,
                              unsigned component_size) {
    const glsl_type *t_without_array = type->without_array();

    if (xfb_offset != -1 && type->is_unsized_array()) {
        _mesa_glsl_error(loc, state,
                         "xfb_offset can't be used with unsized arrays.");
        return false;
    }

    /* Make sure nested structs don't contain unsized arrays, and validate
    * any xfb_offsets on interface members.
    */
    if (t_without_array->is_struct() || t_without_array->is_interface())
        for (unsigned int i = 0; i < t_without_array->length; i++) {
            const glsl_type *member_t = t_without_array->fields.structure[i].type;

            /* When the interface block doesn't have an xfb_offset qualifier then
          * we apply the component size rules at the member level.
          */
            if (xfb_offset == -1)
                component_size = member_t->contains_double() ? 8 : 4;

            int xfb_offset = t_without_array->fields.structure[i].offset;
            validate_xfb_offset_qualifier(loc, state, xfb_offset, member_t,
                                          component_size);
        }

    /* Nested structs or interface block without offset may not have had an
   * offset applied yet so return.
   */
    if (xfb_offset == -1) {
        return true;
    }

    if (xfb_offset % component_size) {
        _mesa_glsl_error(loc, state,
                         "invalid qualifier xfb_offset=%d must be a multiple "
                         "of the first component size of the first qualified "
                         "variable or block member. Or double if an aggregate "
                         "that contains a double (%d).",
                         xfb_offset, component_size);
        return false;
    }

    return true;
}

static bool
validate_stream_qualifier(YYLTYPE *loc, struct _mesa_glsl_parse_state *state,
                          unsigned stream)
{
    if (stream >= state->ctx->Const.MaxVertexStreams) {
        _mesa_glsl_error(loc, state,
                         "invalid stream specified %d is larger than "
                         "MAX_VERTEX_STREAMS - 1 (%d).",
                         stream, state->ctx->Const.MaxVertexStreams - 1);
        return false;
    }

    return true;
}

static void
apply_explicit_binding(struct _mesa_glsl_parse_state *state,
                       YYLTYPE *loc,
                       ir_variable *var,
                       const glsl_type *type,
                       const ast_type_qualifier *qual)
{
    if (!qual->flags.q.uniform && !qual->flags.q.buffer) {
        _mesa_glsl_error(loc, state,
                         "the \"binding\" qualifier only applies to uniforms and "
                         "shader storage buffer objects");
        return;
    }

    unsigned qual_binding;
    if (!process_qualifier_constant(state, loc, "binding", qual->binding,
                                    &qual_binding)) {
        return;
    }

    const struct gl_context *const ctx = state->ctx;
    unsigned elements = type->is_array() ? type->arrays_of_arrays_size() : 1;
    unsigned max_index = qual_binding + elements - 1;
    const glsl_type *base_type = type->without_array();

    if (base_type->is_interface()) {
        /* UBOs.  From page 60 of the GLSL 4.20 specification:
       * "If the binding point for any uniform block instance is less than zero,
       *  or greater than or equal to the implementation-dependent maximum
       *  number of uniform buffer bindings, a compilation error will occur.
       *  When the binding identifier is used with a uniform block instanced as
       *  an array of size N, all elements of the array from binding through
       *  binding + N – 1 must be within this range."
       *
       * The implementation-dependent maximum is GL_MAX_UNIFORM_BUFFER_BINDINGS.
       */
        if (qual->flags.q.uniform &&
            max_index >= ctx->Const.MaxUniformBufferBindings) {
            _mesa_glsl_error(loc, state, "layout(binding = %u) for %d UBOs exceeds "
                                         "the maximum number of UBO binding points (%d)",
                             qual_binding, elements,
                             ctx->Const.MaxUniformBufferBindings);
            return;
        }

        /* SSBOs. From page 67 of the GLSL 4.30 specification:
       * "If the binding point for any uniform or shader storage block instance
       *  is less than zero, or greater than or equal to the
       *  implementation-dependent maximum number of uniform buffer bindings, a
       *  compile-time error will occur. When the binding identifier is used
       *  with a uniform or shader storage block instanced as an array of size
       *  N, all elements of the array from binding through binding + N – 1 must
       *  be within this range."
       */
        if (qual->flags.q.buffer &&
            max_index >= ctx->Const.MaxShaderStorageBufferBindings) {
            _mesa_glsl_error(loc, state, "layout(binding = %u) for %d SSBOs exceeds "
                                         "the maximum number of SSBO binding points (%d)",
                             qual_binding, elements,
                             ctx->Const.MaxShaderStorageBufferBindings);
            return;
        }
    } else if (base_type->is_sampler()) {
        /* Samplers.  From page 63 of the GLSL 4.20 specification:
       * "If the binding is less than zero, or greater than or equal to the
       *  implementation-dependent maximum supported number of units, a
       *  compilation error will occur. When the binding identifier is used
       *  with an array of size N, all elements of the array from binding
       *  through binding + N - 1 must be within this range."
       */
        unsigned limit = ctx->Const.MaxCombinedTextureImageUnits;

        if (max_index >= limit) {
            _mesa_glsl_error(loc, state, "layout(binding = %d) for %d samplers "
                                         "exceeds the maximum number of texture image units "
                                         "(%u)", qual_binding, elements, limit);

            return;
        }
    } else if (base_type->contains_atomic()) {
        assert(ctx->Const.MaxAtomicBufferBindings <= MAX_COMBINED_ATOMIC_BUFFERS);
        if (qual_binding >= ctx->Const.MaxAtomicBufferBindings) {
            _mesa_glsl_error(loc, state, "layout(binding = %d) exceeds the "
                                         "maximum number of atomic counter buffer bindings "
                                         "(%u)", qual_binding,
                             ctx->Const.MaxAtomicBufferBindings);

            return;
        }
    } else if ((state->is_version(420, 310) ||
                state->ARB_shading_language_420pack_enable) &&
               base_type->is_image()) {
        assert(ctx->Const.MaxImageUnits <= MAX_IMAGE_UNITS);
        if (max_index >= ctx->Const.MaxImageUnits) {
            _mesa_glsl_error(loc, state, "Image binding %d exceeds the "
                                         "maximum number of image units (%d)", max_index,
                             ctx->Const.MaxImageUnits);
            return;
        }

    } else {
        _mesa_glsl_error(loc, state,
                         "the \"binding\" qualifier only applies to uniform "
                         "blocks, storage blocks, opaque variables, or arrays "
                         "thereof");
        return;
    }

    var->data.explicit_binding = true;
    var->data.binding = qual_binding;

    return;
}

static void
validate_fragment_flat_interpolation_input(struct _mesa_glsl_parse_state *state,
                                           YYLTYPE *loc,
                                           const glsl_interp_mode interpolation,
                                           const struct glsl_type *var_type,
                                           ir_variable_mode mode)
{
    if (state->stage != MESA_SHADER_FRAGMENT ||
        interpolation == INTERP_MODE_FLAT ||
        mode != ir_var_shader_in)
        return;

    /* Integer fragment inputs must be qualified with 'flat'.  In GLSL ES,
    * so must integer vertex outputs.
    *
    * From section 4.3.4 ("Inputs") of the GLSL 1.50 spec:
    *    "Fragment shader inputs that are signed or unsigned integers or
    *    integer vectors must be qualified with the interpolation qualifier
    *    flat."
    *
    * From section 4.3.4 ("Input Variables") of the GLSL 3.00 ES spec:
    *    "Fragment shader inputs that are, or contain, signed or unsigned
    *    integers or integer vectors must be qualified with the
    *    interpolation qualifier flat."
    *
    * From section 4.3.6 ("Output Variables") of the GLSL 3.00 ES spec:
    *    "Vertex shader outputs that are, or contain, signed or unsigned
    *    integers or integer vectors must be qualified with the
    *    interpolation qualifier flat."
    *
    * Note that prior to GLSL 1.50, this requirement applied to vertex
    * outputs rather than fragment inputs.  That creates problems in the
    * presence of geometry shaders, so we adopt the GLSL 1.50 rule for all
    * desktop GL shaders.  For GLSL ES shaders, we follow the spec and
    * apply the restriction to both vertex outputs and fragment inputs.
    *
    * Note also that the desktop GLSL specs are missing the text "or
    * contain"; this is presumably an oversight, since there is no
    * reasonable way to interpolate a fragment shader input that contains
    * an integer. See Khronos bug #15671.
    */
    if ((state->is_version(130, 300) || state->EXT_gpu_shader4_enable)
        && var_type->contains_integer()) {
        _mesa_glsl_error(loc, state, "if a fragment input is (or contains) "
                                     "an integer, then it must be qualified with 'flat'");
    }

    /* Double fragment inputs must be qualified with 'flat'.
    *
    * From the "Overview" of the ARB_gpu_shader_fp64 extension spec:
    *    "This extension does not support interpolation of double-precision
    *    values; doubles used as fragment shader inputs must be qualified as
    *    "flat"."
    *
    * From section 4.3.4 ("Inputs") of the GLSL 4.00 spec:
    *    "Fragment shader inputs that are signed or unsigned integers, integer
    *    vectors, or any double-precision floating-point type must be
    *    qualified with the interpolation qualifier flat."
    *
    * Note that the GLSL specs are missing the text "or contain"; this is
    * presumably an oversight. See Khronos bug #15671.
    *
    * The 'double' type does not exist in GLSL ES so far.
    */
    if (state->has_double()
        && var_type->contains_double()) {
        _mesa_glsl_error(loc, state, "if a fragment input is (or contains) "
                                     "a double, then it must be qualified with 'flat'");
    }

    /* Bindless sampler/image fragment inputs must be qualified with 'flat'.
    *
    * From section 4.3.4 of the ARB_bindless_texture spec:
    *
    *    "(modify last paragraph, p. 35, allowing samplers and images as
    *     fragment shader inputs) ... Fragment inputs can only be signed and
    *     unsigned integers and integer vectors, floating point scalars,
    *     floating-point vectors, matrices, sampler and image types, or arrays
    *     or structures of these.  Fragment shader inputs that are signed or
    *     unsigned integers, integer vectors, or any double-precision floating-
    *     point type, or any sampler or image type must be qualified with the
    *     interpolation qualifier "flat"."
    */
    if (state->has_bindless()
        && (var_type->contains_sampler() || var_type->contains_image())) {
        _mesa_glsl_error(loc, state, "if a fragment input is (or contains) "
                                     "a bindless sampler (or image), then it must be "
                                     "qualified with 'flat'");
    }
}

static void
validate_interpolation_qualifier(struct _mesa_glsl_parse_state *state,
                                 YYLTYPE *loc,
                                 const glsl_interp_mode interpolation,
                                 const struct ast_type_qualifier *qual,
                                 const struct glsl_type *var_type,
                                 ir_variable_mode mode)
{
    /* Interpolation qualifiers can only apply to shader inputs or outputs, but
    * not to vertex shader inputs nor fragment shader outputs.
    *
    * From section 4.3 ("Storage Qualifiers") of the GLSL 1.30 spec:
    *    "Outputs from a vertex shader (out) and inputs to a fragment
    *    shader (in) can be further qualified with one or more of these
    *    interpolation qualifiers"
    *    ...
    *    "These interpolation qualifiers may only precede the qualifiers in,
    *    centroid in, out, or centroid out in a declaration. They do not apply
    *    to the deprecated storage qualifiers varying or centroid
    *    varying. They also do not apply to inputs into a vertex shader or
    *    outputs from a fragment shader."
    *
    * From section 4.3 ("Storage Qualifiers") of the GLSL ES 3.00 spec:
    *    "Outputs from a shader (out) and inputs to a shader (in) can be
    *    further qualified with one of these interpolation qualifiers."
    *    ...
    *    "These interpolation qualifiers may only precede the qualifiers
    *    in, centroid in, out, or centroid out in a declaration. They do
    *    not apply to inputs into a vertex shader or outputs from a
    *    fragment shader."
    */
    if ((state->is_version(130, 300) || state->EXT_gpu_shader4_enable)
        && interpolation != INTERP_MODE_NONE) {
        const char *i = interpolation_string(interpolation);
        if (mode != ir_var_shader_in && mode != ir_var_shader_out)
            _mesa_glsl_error(loc, state,
                             "interpolation qualifier `%s' can only be applied to "
                             "shader inputs or outputs.", i);

        switch (state->stage) {
            case MESA_SHADER_VERTEX:
                if (mode == ir_var_shader_in) {
                    _mesa_glsl_error(loc, state,
                                     "interpolation qualifier '%s' cannot be applied to "
                                     "vertex shader inputs", i);
                }
                break;
            case MESA_SHADER_FRAGMENT:
                if (mode == ir_var_shader_out) {
                    _mesa_glsl_error(loc, state,
                                     "interpolation qualifier '%s' cannot be applied to "
                                     "fragment shader outputs", i);
                }
                break;
            default:
                break;
        }
    }

    /* Interpolation qualifiers cannot be applied to 'centroid' and
    * 'centroid varying'.
    *
    * From section 4.3 ("Storage Qualifiers") of the GLSL 1.30 spec:
    *    "interpolation qualifiers may only precede the qualifiers in,
    *    centroid in, out, or centroid out in a declaration. They do not apply
    *    to the deprecated storage qualifiers varying or centroid varying."
    *
    * These deprecated storage qualifiers do not exist in GLSL ES 3.00.
    *
    * GL_EXT_gpu_shader4 allows this.
    */
    if (state->is_version(130, 0) && !state->EXT_gpu_shader4_enable
        && interpolation != INTERP_MODE_NONE
        && qual->flags.q.varying) {

        const char *i = interpolation_string(interpolation);
        const char *s;
        if (qual->flags.q.centroid)
            s = "centroid varying";
        else
            s = "varying";

        _mesa_glsl_error(loc, state,
                         "qualifier '%s' cannot be applied to the "
                         "deprecated storage qualifier '%s'", i, s);
    }

    validate_fragment_flat_interpolation_input(state, loc, interpolation,
                                               var_type, mode);
}

static glsl_interp_mode
interpret_interpolation_qualifier(const struct ast_type_qualifier *qual,
                                  const struct glsl_type *var_type,
                                  ir_variable_mode mode,
                                  struct _mesa_glsl_parse_state *state,
                                  YYLTYPE *loc)
{
    glsl_interp_mode interpolation;
    if (qual->flags.q.flat)
        interpolation = INTERP_MODE_FLAT;
    else if (qual->flags.q.noperspective)
        interpolation = INTERP_MODE_NOPERSPECTIVE;
    else if (qual->flags.q.smooth)
        interpolation = INTERP_MODE_SMOOTH;
    else
        interpolation = INTERP_MODE_NONE;

    validate_interpolation_qualifier(state, loc,
                                     interpolation,
                                     qual, var_type, mode);

    return interpolation;
}


static void
apply_explicit_location(const struct ast_type_qualifier *qual,
                        ir_variable *var,
                        struct _mesa_glsl_parse_state *state,
                        YYLTYPE *loc)
{
    bool fail = false;

    unsigned qual_location;
    if (!process_qualifier_constant(state, loc, "location", qual->location,
                                    &qual_location)) {
        return;
    }

    /* Checks for GL_ARB_explicit_uniform_location. */
    if (qual->flags.q.uniform) {
        if (!state->check_explicit_uniform_location_allowed(loc, var))
            return;

        const struct gl_context *const ctx = state->ctx;
        unsigned max_loc = qual_location + var->type->uniform_locations() - 1;

        if (max_loc >= ctx->Const.MaxUserAssignableUniformLocations) {
            _mesa_glsl_error(loc, state, "location(s) consumed by uniform %s "
                                         ">= MAX_UNIFORM_LOCATIONS (%u)", var->name,
                             ctx->Const.MaxUserAssignableUniformLocations);
            return;
        }

        var->data.explicit_location = true;
        var->data.location = qual_location;
        return;
    }

    /* Between GL_ARB_explicit_attrib_location an
    * GL_ARB_separate_shader_objects, the inputs and outputs of any shader
    * stage can be assigned explicit locations.  The checking here associates
    * the correct extension with the correct stage's input / output:
    *
    *                     input            output
    *                     -----            ------
    * vertex              explicit_loc     sso
    * tess control        sso              sso
    * tess eval           sso              sso
    * geometry            sso              sso
    * fragment            sso              explicit_loc
    */
    switch (state->stage) {
        case MESA_SHADER_VERTEX:
            if (var->data.mode == ir_var_shader_in) {
                if (!state->check_explicit_attrib_location_allowed(loc, var))
                    return;

                break;
            }

            if (var->data.mode == ir_var_shader_out) {
                if (!state->check_separate_shader_objects_allowed(loc, var))
                    return;

                break;
            }

            fail = true;
            break;

        case MESA_SHADER_TESS_CTRL:
        case MESA_SHADER_TESS_EVAL:
        case MESA_SHADER_GEOMETRY:
            if (var->data.mode == ir_var_shader_in || var->data.mode == ir_var_shader_out) {
                if (!state->check_separate_shader_objects_allowed(loc, var))
                    return;

                break;
            }

            fail = true;
            break;

        case MESA_SHADER_FRAGMENT:
            if (var->data.mode == ir_var_shader_in) {
                if (!state->check_separate_shader_objects_allowed(loc, var))
                    return;

                break;
            }

            if (var->data.mode == ir_var_shader_out) {
                if (!state->check_explicit_attrib_location_allowed(loc, var))
                    return;

                break;
            }

            fail = true;
            break;

        case MESA_SHADER_COMPUTE:
            _mesa_glsl_error(loc, state,
                             "compute shader variables cannot be given "
                             "explicit locations");
            return;
        default:
            fail = true;
            break;
    };

    if (fail) {
        _mesa_glsl_error(loc, state,
                         "%s cannot be given an explicit location in %s shader",
                         mode_string(var),
                         _mesa_shader_stage_to_string(state->stage));
    } else {
        var->data.explicit_location = true;

        switch (state->stage) {
            case MESA_SHADER_VERTEX:
                var->data.location = (var->data.mode == ir_var_shader_in)
                                     ? (qual_location + VERT_ATTRIB_GENERIC0)
                                     : (qual_location + VARYING_SLOT_VAR0);
                break;

            case MESA_SHADER_TESS_CTRL:
            case MESA_SHADER_TESS_EVAL:
            case MESA_SHADER_GEOMETRY:
                if (var->data.patch)
                    var->data.location = qual_location + VARYING_SLOT_PATCH0;
                else
                    var->data.location = qual_location + VARYING_SLOT_VAR0;
                break;

            case MESA_SHADER_FRAGMENT:
                var->data.location = (var->data.mode == ir_var_shader_out)
                                     ? (qual_location + FRAG_RESULT_DATA0)
                                     : (qual_location + VARYING_SLOT_VAR0);
                break;
            default:
                assert(!"Unexpected shader type");
                break;
        }

        /* Check if index was set for the uniform instead of the function */
        if (qual->flags.q.explicit_index && qual->is_subroutine_decl()) {
            _mesa_glsl_error(loc, state, "an index qualifier can only be "
                                         "used with subroutine functions");
            return;
        }

        unsigned qual_index;
        if (qual->flags.q.explicit_index &&
            process_qualifier_constant(state, loc, "index", qual->index,
                                       &qual_index)) {
            /* From the GLSL 4.30 specification, section 4.4.2 (Output
          * Layout Qualifiers):
          *
          * "It is also a compile-time error if a fragment shader
          *  sets a layout index to less than 0 or greater than 1."
          *
          * Older specifications don't mandate a behavior; we take
          * this as a clarification and always generate the error.
          */
            if (qual_index > 1) {
                _mesa_glsl_error(loc, state,
                                 "explicit index may only be 0 or 1");
            } else {
                var->data.explicit_index = true;
                var->data.index = qual_index;
            }
        }
    }
}

static bool
validate_storage_for_sampler_image_types(ir_variable *var,
                                         struct _mesa_glsl_parse_state *state,
                                         YYLTYPE *loc)
{
    /* From section 4.1.7 of the GLSL 4.40 spec:
    *
    *    "[Opaque types] can only be declared as function
    *     parameters or uniform-qualified variables."
    *
    * From section 4.1.7 of the ARB_bindless_texture spec:
    *
    *    "Samplers may be declared as shader inputs and outputs, as uniform
    *     variables, as temporary variables, and as function parameters."
    *
    * From section 4.1.X of the ARB_bindless_texture spec:
    *
    *    "Images may be declared as shader inputs and outputs, as uniform
    *     variables, as temporary variables, and as function parameters."
    */
    if (state->has_bindless()) {
        if (var->data.mode != ir_var_auto &&
            var->data.mode != ir_var_uniform &&
            var->data.mode != ir_var_shader_in &&
            var->data.mode != ir_var_shader_out &&
            var->data.mode != ir_var_function_in &&
            var->data.mode != ir_var_function_out &&
            var->data.mode != ir_var_function_inout) {
            _mesa_glsl_error(loc, state, "bindless image/sampler variables may "
                                         "only be declared as shader inputs and outputs, as "
                                         "uniform variables, as temporary variables and as "
                                         "function parameters");
            return false;
        }
    } else {
        if (var->data.mode != ir_var_uniform &&
            var->data.mode != ir_var_function_in) {
            _mesa_glsl_error(loc, state, "image/sampler variables may only be "
                                         "declared as function parameters or "
                                         "uniform-qualified global variables");
            return false;
        }
    }
    return true;
}

static bool
validate_memory_qualifier_for_type(struct _mesa_glsl_parse_state *state,
                                   YYLTYPE *loc,
                                   const struct ast_type_qualifier *qual,
                                   const glsl_type *type)
{
    /* From Section 4.10 (Memory Qualifiers) of the GLSL 4.50 spec:
    *
    * "Memory qualifiers are only supported in the declarations of image
    *  variables, buffer variables, and shader storage blocks; it is an error
    *  to use such qualifiers in any other declarations.
    */
    if (!type->is_image() && !qual->flags.q.buffer) {
        if (qual->flags.q.read_only ||
            qual->flags.q.write_only ||
            qual->flags.q.coherent ||
            qual->flags.q._volatile ||
            qual->flags.q.restrict_flag) {
            _mesa_glsl_error(loc, state, "memory qualifiers may only be applied "
                                         "in the declarations of image variables, buffer "
                                         "variables, and shader storage blocks");
            return false;
        }
    }
    return true;
}

static bool
validate_image_format_qualifier_for_type(struct _mesa_glsl_parse_state *state,
                                         YYLTYPE *loc,
                                         const struct ast_type_qualifier *qual,
                                         const glsl_type *type)
{
    /* From section 4.4.6.2 (Format Layout Qualifiers) of the GLSL 4.50 spec:
    *
    * "Format layout qualifiers can be used on image variable declarations
    *  (those declared with a basic type  having “image ” in its keyword)."
    */
    if (!type->is_image() && qual->flags.q.explicit_image_format) {
        _mesa_glsl_error(loc, state, "format layout qualifiers may only be "
                                     "applied to images");
        return false;
    }
    return true;
}

static void
apply_image_qualifier_to_variable(const struct ast_type_qualifier *qual,
                                  ir_variable *var,
                                  struct _mesa_glsl_parse_state *state,
                                  YYLTYPE *loc)
{
    const glsl_type *base_type = var->type->without_array();

    if (!validate_image_format_qualifier_for_type(state, loc, qual, base_type) ||
        !validate_memory_qualifier_for_type(state, loc, qual, base_type))
        return;

    if (!base_type->is_image())
        return;

    if (!validate_storage_for_sampler_image_types(var, state, loc))
        return;

    var->data.memory_read_only |= qual->flags.q.read_only;
    var->data.memory_write_only |= qual->flags.q.write_only;
    var->data.memory_coherent |= qual->flags.q.coherent;
    var->data.memory_volatile |= qual->flags.q._volatile;
    var->data.memory_restrict |= qual->flags.q.restrict_flag;

    if (qual->flags.q.explicit_image_format) {
        if (var->data.mode == ir_var_function_in) {
            _mesa_glsl_error(loc, state, "format qualifiers cannot be used on "
                                         "image function parameters");
        }

        if (qual->image_base_type != base_type->sampled_type) {
            _mesa_glsl_error(loc, state, "format qualifier doesn't match the base "
                                         "data type of the image");
        }

        var->data.image_format = qual->image_format;
    } else if (state->has_image_load_formatted()) {
        if (var->data.mode == ir_var_uniform &&
            state->EXT_shader_image_load_formatted_warn) {
            _mesa_glsl_warning(loc, state, "GL_EXT_image_load_formatted used");
        }
    } else {
        if (var->data.mode == ir_var_uniform) {
            if (state->es_shader ||
                !(state->is_version(420, 310) || state->ARB_shader_image_load_store_enable)) {
                _mesa_glsl_error(loc, state, "all image uniforms must have a "
                                             "format layout qualifier");
            } else if (!qual->flags.q.write_only) {
                _mesa_glsl_error(loc, state, "image uniforms not qualified with "
                                             "`writeonly' must have a format layout qualifier");
            }
        }
        var->data.image_format = PIPE_FORMAT_NONE;
    }

    /* From page 70 of the GLSL ES 3.1 specification:
    *
    * "Except for image variables qualified with the format qualifiers r32f,
    *  r32i, and r32ui, image variables must specify either memory qualifier
    *  readonly or the memory qualifier writeonly."
    */
    if (state->es_shader &&
        var->data.image_format != PIPE_FORMAT_R32_FLOAT &&
        var->data.image_format != PIPE_FORMAT_R32_SINT &&
        var->data.image_format != PIPE_FORMAT_R32_UINT &&
        !var->data.memory_read_only &&
        !var->data.memory_write_only) {
        _mesa_glsl_error(loc, state, "image variables of format other than r32f, "
                                     "r32i or r32ui must be qualified `readonly' or "
                                     "`writeonly'");
    }
}

static inline const char*
get_layout_qualifier_string(bool origin_upper_left, bool pixel_center_integer)
{
    if (origin_upper_left && pixel_center_integer)
        return "origin_upper_left, pixel_center_integer";
    else if (origin_upper_left)
        return "origin_upper_left";
    else if (pixel_center_integer)
        return "pixel_center_integer";
    else
        return " ";
}

static inline bool
is_conflicting_fragcoord_redeclaration(struct _mesa_glsl_parse_state *state,
                                       const struct ast_type_qualifier *qual)
{
    /* If gl_FragCoord was previously declared, and the qualifiers were
    * different in any way, return true.
    */
    if (state->fs_redeclares_gl_fragcoord) {
        return (state->fs_pixel_center_integer != qual->flags.q.pixel_center_integer
                || state->fs_origin_upper_left != qual->flags.q.origin_upper_left);
    }

    return false;
}

static inline bool
is_conflicting_layer_redeclaration(struct _mesa_glsl_parse_state *state,
                                   const struct ast_type_qualifier *qual)
{
    if (state->redeclares_gl_layer) {
        return state->layer_viewport_relative != qual->flags.q.viewport_relative;
    }
    return false;
}

static inline void
validate_array_dimensions(const glsl_type *t,
                          struct _mesa_glsl_parse_state *state,
                          YYLTYPE *loc) {
    if (t->is_array()) {
        t = t->fields.array;
        while (t->is_array()) {
            if (t->is_unsized_array()) {
                _mesa_glsl_error(loc, state,
                                 "only the outermost array dimension can "
                                 "be unsized",
                                 t->name);
                break;
            }
            t = t->fields.array;
        }
    }
}

static void
apply_bindless_qualifier_to_variable(const struct ast_type_qualifier *qual,
                                     ir_variable *var,
                                     struct _mesa_glsl_parse_state *state,
                                     YYLTYPE *loc)
{
    bool has_local_qualifiers = qual->flags.q.bindless_sampler ||
                                qual->flags.q.bindless_image ||
                                qual->flags.q.bound_sampler ||
                                qual->flags.q.bound_image;

    /* The ARB_bindless_texture spec says:
    *
    * "Modify Section 4.4.6 Opaque-Uniform Layout Qualifiers of the GLSL 4.30
    *  spec"
    *
    * "If these layout qualifiers are applied to other types of default block
    *  uniforms, or variables with non-uniform storage, a compile-time error
    *  will be generated."
    */
    if (has_local_qualifiers && !qual->flags.q.uniform) {
        _mesa_glsl_error(loc, state, "ARB_bindless_texture layout qualifiers "
                                     "can only be applied to default block uniforms or "
                                     "variables with uniform storage");
        return;
    }

    /* The ARB_bindless_texture spec doesn't state anything in this situation,
    * but it makes sense to only allow bindless_sampler/bound_sampler for
    * sampler types, and respectively bindless_image/bound_image for image
    * types.
    */
    if ((qual->flags.q.bindless_sampler || qual->flags.q.bound_sampler) &&
        !var->type->contains_sampler()) {
        _mesa_glsl_error(loc, state, "bindless_sampler or bound_sampler can only "
                                     "be applied to sampler types");
        return;
    }

    if ((qual->flags.q.bindless_image || qual->flags.q.bound_image) &&
        !var->type->contains_image()) {
        _mesa_glsl_error(loc, state, "bindless_image or bound_image can only be "
                                     "applied to image types");
        return;
    }

    /* The bindless_sampler/bindless_image (and respectively
    * bound_sampler/bound_image) layout qualifiers can be set at global and at
    * local scope.
    */
    if (var->type->contains_sampler() || var->type->contains_image()) {
        var->data.bindless = qual->flags.q.bindless_sampler ||
                             qual->flags.q.bindless_image ||
                             state->bindless_sampler_specified ||
                             state->bindless_image_specified;

        var->data.bound = qual->flags.q.bound_sampler ||
                          qual->flags.q.bound_image ||
                          state->bound_sampler_specified ||
                          state->bound_image_specified;
    }
}

static void
apply_layout_qualifier_to_variable(const struct ast_type_qualifier *qual,
                                   ir_variable *var,
                                   struct _mesa_glsl_parse_state *state,
                                   YYLTYPE *loc)
{
    if (var->name != NULL && strcmp(var->name, "gl_FragCoord") == 0) {

        /* Section 4.3.8.1, page 39 of GLSL 1.50 spec says:
       *
       *    "Within any shader, the first redeclarations of gl_FragCoord
       *     must appear before any use of gl_FragCoord."
       *
       * Generate a compiler error if above condition is not met by the
       * fragment shader.
       */
        ir_variable *earlier = state->symbols->get_variable("gl_FragCoord");
        if (earlier != NULL &&
            earlier->data.used &&
            !state->fs_redeclares_gl_fragcoord) {
            _mesa_glsl_error(loc, state,
                             "gl_FragCoord used before its first redeclaration "
                             "in fragment shader");
        }

        /* Make sure all gl_FragCoord redeclarations specify the same layout
       * qualifiers.
       */
        if (is_conflicting_fragcoord_redeclaration(state, qual)) {
            const char *const qual_string =
                    get_layout_qualifier_string(qual->flags.q.origin_upper_left,
                                                qual->flags.q.pixel_center_integer);

            const char *const state_string =
                    get_layout_qualifier_string(state->fs_origin_upper_left,
                                                state->fs_pixel_center_integer);

            _mesa_glsl_error(loc, state,
                             "gl_FragCoord redeclared with different layout "
                             "qualifiers (%s) and (%s) ",
                             state_string,
                             qual_string);
        }
        state->fs_origin_upper_left = qual->flags.q.origin_upper_left;
        state->fs_pixel_center_integer = qual->flags.q.pixel_center_integer;
        state->fs_redeclares_gl_fragcoord_with_no_layout_qualifiers =
                !qual->flags.q.origin_upper_left && !qual->flags.q.pixel_center_integer;
        state->fs_redeclares_gl_fragcoord =
                state->fs_origin_upper_left ||
                state->fs_pixel_center_integer ||
                state->fs_redeclares_gl_fragcoord_with_no_layout_qualifiers;
    }

    if ((qual->flags.q.origin_upper_left || qual->flags.q.pixel_center_integer)
        && (strcmp(var->name, "gl_FragCoord") != 0)) {
        const char *const qual_string = (qual->flags.q.origin_upper_left)
                                        ? "origin_upper_left" : "pixel_center_integer";

        _mesa_glsl_error(loc, state,
                         "layout qualifier `%s' can only be applied to "
                         "fragment shader input `gl_FragCoord'",
                         qual_string);
    }

    if (qual->flags.q.explicit_location) {
        apply_explicit_location(qual, var, state, loc);

        if (qual->flags.q.explicit_component) {
            unsigned qual_component;
            if (process_qualifier_constant(state, loc, "component",
                                           qual->component, &qual_component)) {
                validate_component_layout_for_type(state, loc, var->type,
                                                   qual_component);
                var->data.explicit_component = true;
                var->data.location_frac = qual_component;
            }
        }
    } else if (qual->flags.q.explicit_index) {
        if (!qual->subroutine_list)
            _mesa_glsl_error(loc, state,
                             "explicit index requires explicit location");
    } else if (qual->flags.q.explicit_component) {
        _mesa_glsl_error(loc, state,
                         "explicit component requires explicit location");
    }

    if (qual->flags.q.explicit_binding) {
        apply_explicit_binding(state, loc, var, var->type, qual);
    }

    if (state->stage == MESA_SHADER_GEOMETRY &&
        qual->flags.q.out && qual->flags.q.stream) {
        unsigned qual_stream;
        if (process_qualifier_constant(state, loc, "stream", qual->stream,
                                       &qual_stream) &&
            validate_stream_qualifier(loc, state, qual_stream)) {
            var->data.stream = qual_stream;
        }
    }

    if (qual->flags.q.out && qual->flags.q.xfb_buffer) {
        unsigned qual_xfb_buffer;
        if (process_qualifier_constant(state, loc, "xfb_buffer",
                                       qual->xfb_buffer, &qual_xfb_buffer) &&
            validate_xfb_buffer_qualifier(loc, state, qual_xfb_buffer)) {
            var->data.xfb_buffer = qual_xfb_buffer;
            if (qual->flags.q.explicit_xfb_buffer)
                var->data.explicit_xfb_buffer = true;
        }
    }

    if (qual->flags.q.explicit_xfb_offset) {
        unsigned qual_xfb_offset;
        unsigned component_size = var->type->contains_double() ? 8 : 4;

        if (process_qualifier_constant(state, loc, "xfb_offset",
                                       qual->offset, &qual_xfb_offset) &&
            validate_xfb_offset_qualifier(loc, state, (int) qual_xfb_offset,
                                          var->type, component_size)) {
            var->data.offset = qual_xfb_offset;
            var->data.explicit_xfb_offset = true;
        }
    }

    if (qual->flags.q.explicit_xfb_stride) {
        unsigned qual_xfb_stride;
        if (process_qualifier_constant(state, loc, "xfb_stride",
                                       qual->xfb_stride, &qual_xfb_stride)) {
            var->data.xfb_stride = qual_xfb_stride;
            var->data.explicit_xfb_stride = true;
        }
    }

    if (var->type->contains_atomic()) {
        if (var->data.mode == ir_var_uniform) {
            if (var->data.explicit_binding) {
                unsigned *offset =
                        &state->atomic_counter_offsets[var->data.binding];

                if (*offset % ATOMIC_COUNTER_SIZE)
                    _mesa_glsl_error(loc, state,
                                     "misaligned atomic counter offset");

                var->data.offset = *offset;
                *offset += var->type->atomic_size();

            } else {
                _mesa_glsl_error(loc, state,
                                 "atomic counters require explicit binding point");
            }
        } else if (var->data.mode != ir_var_function_in) {
            _mesa_glsl_error(loc, state, "atomic counters may only be declared as "
                                         "function parameters or uniform-qualified "
                                         "global variables");
        }
    }

    if (var->type->contains_sampler() &&
        !validate_storage_for_sampler_image_types(var, state, loc))
        return;

    /* Is the 'layout' keyword used with parameters that allow relaxed checking.
    * Many implementations of GL_ARB_fragment_coord_conventions_enable and some
    * implementations (only Mesa?) GL_ARB_explicit_attrib_location_enable
    * allowed the layout qualifier to be used with 'varying' and 'attribute'.
    * These extensions and all following extensions that add the 'layout'
    * keyword have been modified to require the use of 'in' or 'out'.
    *
    * The following extension do not allow the deprecated keywords:
    *
    *    GL_AMD_conservative_depth
    *    GL_ARB_conservative_depth
    *    GL_ARB_gpu_shader5
    *    GL_ARB_separate_shader_objects
    *    GL_ARB_tessellation_shader
    *    GL_ARB_transform_feedback3
    *    GL_ARB_uniform_buffer_object
    *
    * It is unknown whether GL_EXT_shader_image_load_store or GL_NV_gpu_shader5
    * allow layout with the deprecated keywords.
    */
    const bool relaxed_layout_qualifier_checking =
            state->ARB_fragment_coord_conventions_enable;

    const bool uses_deprecated_qualifier = qual->flags.q.attribute
                                           || qual->flags.q.varying;
    if (qual->has_layout() && uses_deprecated_qualifier) {
        if (relaxed_layout_qualifier_checking) {
            _mesa_glsl_warning(loc, state,
                               "`layout' qualifier may not be used with "
                               "`attribute' or `varying'");
        } else {
            _mesa_glsl_error(loc, state,
                             "`layout' qualifier may not be used with "
                             "`attribute' or `varying'");
        }
    }

    /* Layout qualifiers for gl_FragDepth, which are enabled by extension
    * AMD_conservative_depth.
    */
    if (qual->flags.q.depth_type
        && !state->is_version(420, 0)
        && !state->AMD_conservative_depth_enable
        && !state->ARB_conservative_depth_enable) {
        _mesa_glsl_error(loc, state,
                         "extension GL_AMD_conservative_depth or "
                         "GL_ARB_conservative_depth must be enabled "
                         "to use depth layout qualifiers");
    } else if (qual->flags.q.depth_type
               && strcmp(var->name, "gl_FragDepth") != 0) {
        _mesa_glsl_error(loc, state,
                         "depth layout qualifiers can be applied only to "
                         "gl_FragDepth");
    }

    switch (qual->depth_type) {
        case ast_depth_any:
            var->data.depth_layout = ir_depth_layout_any;
            break;
        case ast_depth_greater:
            var->data.depth_layout = ir_depth_layout_greater;
            break;
        case ast_depth_less:
            var->data.depth_layout = ir_depth_layout_less;
            break;
        case ast_depth_unchanged:
            var->data.depth_layout = ir_depth_layout_unchanged;
            break;
        default:
            var->data.depth_layout = ir_depth_layout_none;
            break;
    }

    if (qual->flags.q.std140 ||
        qual->flags.q.std430 ||
        qual->flags.q.packed ||
        qual->flags.q.shared) {
        _mesa_glsl_error(loc, state,
                         "uniform and shader storage block layout qualifiers "
                         "std140, std430, packed, and shared can only be "
                         "applied to uniform or shader storage blocks, not "
                         "members");
    }

    if (qual->flags.q.row_major || qual->flags.q.column_major) {
        validate_matrix_layout_for_type(state, loc, var->type, var);
    }

    /* From section 4.4.1.3 of the GLSL 4.50 specification (Fragment Shader
    * Inputs):
    *
    *  "Fragment shaders also allow the following layout qualifier on in only
    *   (not with variable declarations)
    *     layout-qualifier-id
    *        early_fragment_tests
    *   [...]"
    */
    if (qual->flags.q.early_fragment_tests) {
        _mesa_glsl_error(loc, state, "early_fragment_tests layout qualifier only "
                                     "valid in fragment shader input layout declaration.");
    }

    if (qual->flags.q.inner_coverage) {
        _mesa_glsl_error(loc, state, "inner_coverage layout qualifier only "
                                     "valid in fragment shader input layout declaration.");
    }

    if (qual->flags.q.post_depth_coverage) {
        _mesa_glsl_error(loc, state, "post_depth_coverage layout qualifier only "
                                     "valid in fragment shader input layout declaration.");
    }

    if (state->has_bindless())
        apply_bindless_qualifier_to_variable(qual, var, state, loc);

    if (qual->flags.q.pixel_interlock_ordered ||
        qual->flags.q.pixel_interlock_unordered ||
        qual->flags.q.sample_interlock_ordered ||
        qual->flags.q.sample_interlock_unordered) {
        _mesa_glsl_error(loc, state, "interlock layout qualifiers: "
                                     "pixel_interlock_ordered, pixel_interlock_unordered, "
                                     "sample_interlock_ordered and sample_interlock_unordered, "
                                     "only valid in fragment shader input layout declaration.");
    }

    if (var->name != NULL && strcmp(var->name, "gl_Layer") == 0) {
        if (is_conflicting_layer_redeclaration(state, qual)) {
            _mesa_glsl_error(loc, state, "gl_Layer redeclaration with "
                                         "different viewport_relative setting than earlier");
        }
        state->redeclares_gl_layer = 1;
        if (qual->flags.q.viewport_relative) {
            state->layer_viewport_relative = 1;
        }
    } else if (qual->flags.q.viewport_relative) {
        _mesa_glsl_error(loc, state,
                         "viewport_relative qualifier "
                         "can only be applied to gl_Layer.");
    }
}

static void
apply_type_qualifier_to_variable(const struct ast_type_qualifier *qual,
                                 ir_variable *var,
                                 struct _mesa_glsl_parse_state *state,
                                 YYLTYPE *loc,
                                 bool is_parameter)
{
    STATIC_ASSERT(sizeof(qual->flags.q) <= sizeof(qual->flags.i));

    if (qual->flags.q.invariant) {
        if (var->data.used) {
            _mesa_glsl_error(loc, state,
                             "variable `%s' may not be redeclared "
                             "`invariant' after being used",
                             var->name);
        } else {
            var->data.explicit_invariant = true;
            var->data.invariant = true;
        }
    }

    if (qual->flags.q.precise) {
        if (var->data.used) {
            _mesa_glsl_error(loc, state,
                             "variable `%s' may not be redeclared "
                             "`precise' after being used",
                             var->name);
        } else {
            var->data.precise = 1;
        }
    }

    if (qual->is_subroutine_decl() && !qual->flags.q.uniform) {
        _mesa_glsl_error(loc, state,
                         "`subroutine' may only be applied to uniforms, "
                         "subroutine type declarations, or function definitions");
    }

    if (qual->flags.q.constant || qual->flags.q.attribute
        || qual->flags.q.uniform
        || (qual->flags.q.varying && (state->stage == MESA_SHADER_FRAGMENT)))
        var->data.read_only = 1;

    if (qual->flags.q.centroid)
        var->data.centroid = 1;

    if (qual->flags.q.sample)
        var->data.sample = 1;

    /* Precision qualifiers do not hold any meaning in Desktop GLSL */
    if (state->es_shader) {
        var->data.precision =
                select_gles_precision(qual->precision, var->type, state, loc);
    }

    if (qual->flags.q.patch)
        var->data.patch = 1;

    if (qual->flags.q.attribute && state->stage != MESA_SHADER_VERTEX) {
        var->type = glsl_type::error_type;
        _mesa_glsl_error(loc, state,
                         "`attribute' variables may not be declared in the "
                         "%s shader",
                         _mesa_shader_stage_to_string(state->stage));
    }

    /* Disallow layout qualifiers which may only appear on layout declarations. */
    if (qual->flags.q.prim_type) {
        _mesa_glsl_error(loc, state,
                         "Primitive type may only be specified on GS input or output "
                         "layout declaration, not on variables.");
    }

    /* Section 6.1.1 (Function Calling Conventions) of the GLSL 1.10 spec says:
    *
    *     "However, the const qualifier cannot be used with out or inout."
    *
    * The same section of the GLSL 4.40 spec further clarifies this saying:
    *
    *     "The const qualifier cannot be used with out or inout, or a
    *     compile-time error results."
    */
    if (is_parameter && qual->flags.q.constant && qual->flags.q.out) {
        _mesa_glsl_error(loc, state,
                         "`const' may not be applied to `out' or `inout' "
                         "function parameters");
    }

    /* If there is no qualifier that changes the mode of the variable, leave
    * the setting alone.
    */
    assert(var->data.mode != ir_var_temporary);
    if (qual->flags.q.in && qual->flags.q.out)
        var->data.mode = is_parameter ? ir_var_function_inout : ir_var_shader_out;
    else if (qual->flags.q.in)
        var->data.mode = is_parameter ? ir_var_function_in : ir_var_shader_in;
    else if (qual->flags.q.attribute
             || (qual->flags.q.varying && (state->stage == MESA_SHADER_FRAGMENT)))
        var->data.mode = ir_var_shader_in;
    else if (qual->flags.q.out)
        var->data.mode = is_parameter ? ir_var_function_out : ir_var_shader_out;
    else if (qual->flags.q.varying && (state->stage == MESA_SHADER_VERTEX))
        var->data.mode = ir_var_shader_out;
    else if (qual->flags.q.uniform)
        var->data.mode = ir_var_uniform;
    else if (qual->flags.q.buffer)
        var->data.mode = ir_var_shader_storage;
    else if (qual->flags.q.shared_storage)
        var->data.mode = ir_var_shader_shared;

    if (!is_parameter && state->has_framebuffer_fetch() &&
        state->stage == MESA_SHADER_FRAGMENT) {
        if (state->is_version(130, 300))
            var->data.fb_fetch_output = qual->flags.q.in && qual->flags.q.out;
        else
            var->data.fb_fetch_output = (strcmp(var->name, "gl_LastFragData") == 0);
    }

    if (var->data.fb_fetch_output) {
        var->data.assigned = true;
        var->data.memory_coherent = !qual->flags.q.non_coherent;

        /* From the EXT_shader_framebuffer_fetch spec:
       *
       *   "It is an error to declare an inout fragment output not qualified
       *    with layout(noncoherent) if the GL_EXT_shader_framebuffer_fetch
       *    extension hasn't been enabled."
       */
        if (var->data.memory_coherent &&
            !state->EXT_shader_framebuffer_fetch_enable)
            _mesa_glsl_error(loc, state,
                             "invalid declaration of framebuffer fetch output not "
                             "qualified with layout(noncoherent)");

    } else {
        /* From the EXT_shader_framebuffer_fetch spec:
       *
       *   "Fragment outputs declared inout may specify the following layout
       *    qualifier: [...] noncoherent"
       */
        if (qual->flags.q.non_coherent)
            _mesa_glsl_error(loc, state,
                             "invalid layout(noncoherent) qualifier not part of "
                             "framebuffer fetch output declaration");
    }

    if (!is_parameter && is_varying_var(var, state->stage)) {
        /* User-defined ins/outs are not permitted in compute shaders. */
        if (state->stage == MESA_SHADER_COMPUTE) {
            _mesa_glsl_error(loc, state,
                             "user-defined input and output variables are not "
                             "permitted in compute shaders");
        }

        /* This variable is being used to link data between shader stages (in
       * pre-glsl-1.30 parlance, it's a "varying").  Check that it has a type
       * that is allowed for such purposes.
       *
       * From page 25 (page 31 of the PDF) of the GLSL 1.10 spec:
       *
       *     "The varying qualifier can be used only with the data types
       *     float, vec2, vec3, vec4, mat2, mat3, and mat4, or arrays of
       *     these."
       *
       * This was relaxed in GLSL version 1.30 and GLSL ES version 3.00.  From
       * page 31 (page 37 of the PDF) of the GLSL 1.30 spec:
       *
       *     "Fragment inputs can only be signed and unsigned integers and
       *     integer vectors, float, floating-point vectors, matrices, or
       *     arrays of these. Structures cannot be input.
       *
       * Similar text exists in the section on vertex shader outputs.
       *
       * Similar text exists in the GLSL ES 3.00 spec, except that the GLSL ES
       * 3.00 spec allows structs as well.  Varying structs are also allowed
       * in GLSL 1.50.
       *
       * From section 4.3.4 of the ARB_bindless_texture spec:
       *
       *     "(modify third paragraph of the section to allow sampler and image
       *     types) ...  Vertex shader inputs can only be float,
       *     single-precision floating-point scalars, single-precision
       *     floating-point vectors, matrices, signed and unsigned integers
       *     and integer vectors, sampler and image types."
       *
       * From section 4.3.6 of the ARB_bindless_texture spec:
       *
       *     "Output variables can only be floating-point scalars,
       *     floating-point vectors, matrices, signed or unsigned integers or
       *     integer vectors, sampler or image types, or arrays or structures
       *     of any these."
       */
        switch (var->type->without_array()->base_type) {
            case GLSL_TYPE_FLOAT:
                /* Ok in all GLSL versions */
                break;
            case GLSL_TYPE_UINT:
            case GLSL_TYPE_INT:
                if (state->is_version(130, 300) || state->EXT_gpu_shader4_enable)
                    break;
                _mesa_glsl_error(loc, state,
                                 "varying variables must be of base type float in %s",
                                 state->get_version_string());
                break;
            case GLSL_TYPE_STRUCT:
                if (state->is_version(150, 300))
                    break;
                _mesa_glsl_error(loc, state,
                                 "varying variables may not be of type struct");
                break;
            case GLSL_TYPE_DOUBLE:
            case GLSL_TYPE_UINT64:
            case GLSL_TYPE_INT64:
                break;
            case GLSL_TYPE_SAMPLER:
            case GLSL_TYPE_IMAGE:
                if (state->has_bindless())
                    break;
                /* fallthrough */
            default:
                _mesa_glsl_error(loc, state, "illegal type for a varying variable");
                break;
        }
    }

    if (state->all_invariant && var->data.mode == ir_var_shader_out) {
        var->data.explicit_invariant = true;
        var->data.invariant = true;
    }

    var->data.interpolation =
            interpret_interpolation_qualifier(qual, var->type,
                                              (ir_variable_mode) var->data.mode,
                                              state, loc);

    /* Does the declaration use the deprecated 'attribute' or 'varying'
    * keywords?
    */
    const bool uses_deprecated_qualifier = qual->flags.q.attribute
                                           || qual->flags.q.varying;


    /* Validate auxiliary storage qualifiers */

    /* From section 4.3.4 of the GLSL 1.30 spec:
    *    "It is an error to use centroid in in a vertex shader."
    *
    * From section 4.3.4 of the GLSL ES 3.00 spec:
    *    "It is an error to use centroid in or interpolation qualifiers in
    *    a vertex shader input."
    */

    /* Section 4.3.6 of the GLSL 1.30 specification states:
    * "It is an error to use centroid out in a fragment shader."
    *
    * The GL_ARB_shading_language_420pack extension specification states:
    * "It is an error to use auxiliary storage qualifiers or interpolation
    *  qualifiers on an output in a fragment shader."
    */
    if (qual->flags.q.sample && (!is_varying_var(var, state->stage) || uses_deprecated_qualifier)) {
        _mesa_glsl_error(loc, state,
                         "sample qualifier may only be used on `in` or `out` "
                         "variables between shader stages");
    }
    if (qual->flags.q.centroid && !is_varying_var(var, state->stage)) {
        _mesa_glsl_error(loc, state,
                         "centroid qualifier may only be used with `in', "
                         "`out' or `varying' variables between shader stages");
    }

    if (qual->flags.q.shared_storage && state->stage != MESA_SHADER_COMPUTE) {
        _mesa_glsl_error(loc, state,
                         "the shared storage qualifiers can only be used with "
                         "compute shaders");
    }

    apply_image_qualifier_to_variable(qual, var, state, loc);
}

/**
 * Get the variable that is being redeclared by this declaration or if it
 * does not exist, the current declared variable.
 *
 * Semantic checks to verify the validity of the redeclaration are also
 * performed.  If semantic checks fail, compilation error will be emitted via
 * \c _mesa_glsl_error, but a non-\c NULL pointer will still be returned.
 *
 * \returns
 * A pointer to an existing variable in the current scope if the declaration
 * is a redeclaration, current variable otherwise. \c is_declared boolean
 * will return \c true if the declaration is a redeclaration, \c false
 * otherwise.
 */
static ir_variable *
get_variable_being_redeclared(ir_variable **var_ptr, YYLTYPE loc,
                              struct _mesa_glsl_parse_state *state,
                              bool allow_all_redeclarations,
                              bool *is_redeclaration)
{
    ir_variable *var = *var_ptr;

    /* Check if this declaration is actually a re-declaration, either to
    * resize an array or add qualifiers to an existing variable.
    *
    * This is allowed for variables in the current scope, or when at
    * global scope (for built-ins in the implicit outer scope).
    */
    ir_variable *earlier = state->symbols->get_variable(var->name);
    if (earlier == NULL ||
        (state->current_function != NULL &&
         !state->symbols->name_declared_this_scope(var->name))) {
        *is_redeclaration = false;
        return var;
    }

    *is_redeclaration = true;

    if (earlier->data.how_declared == ir_var_declared_implicitly) {
        /* Verify that the redeclaration of a built-in does not change the
       * storage qualifier.  There are a couple special cases.
       *
       * 1. Some built-in variables that are defined as 'in' in the
       *    specification are implemented as system values.  Allow
       *    ir_var_system_value -> ir_var_shader_in.
       *
       * 2. gl_LastFragData is implemented as a ir_var_shader_out, but the
       *    specification requires that redeclarations omit any qualifier.
       *    Allow ir_var_shader_out -> ir_var_auto for this one variable.
       */
        if (earlier->data.mode != var->data.mode &&
            !(earlier->data.mode == ir_var_system_value &&
              var->data.mode == ir_var_shader_in) &&
            !(strcmp(var->name, "gl_LastFragData") == 0 &&
              var->data.mode == ir_var_auto)) {
            _mesa_glsl_error(&loc, state,
                             "redeclaration cannot change qualification of `%s'",
                             var->name);
        }
    }

    /* From page 24 (page 30 of the PDF) of the GLSL 1.50 spec,
    *
    * "It is legal to declare an array without a size and then
    *  later re-declare the same name as an array of the same
    *  type and specify a size."
    */
    if (earlier->type->is_unsized_array() && var->type->is_array()
        && (var->type->fields.array == earlier->type->fields.array)) {
        const int size = var->type->array_size();
        check_builtin_array_max_size(var->name, size, loc, state);
        if ((size > 0) && (size <= earlier->data.max_array_access)) {
            _mesa_glsl_error(& loc, state, "array size must be > %u due to "
                                           "previous access",
                             earlier->data.max_array_access);
        }

        earlier->type = var->type;
        delete var;
        var = NULL;
        *var_ptr = NULL;
    } else if (earlier->type != var->type) {
        _mesa_glsl_error(&loc, state,
                         "redeclaration of `%s' has incorrect type",
                         var->name);
    } else if ((state->ARB_fragment_coord_conventions_enable ||
                state->is_version(150, 0))
               && strcmp(var->name, "gl_FragCoord") == 0) {
        /* Allow redeclaration of gl_FragCoord for ARB_fcc layout
       * qualifiers.
       *
       * We don't really need to do anything here, just allow the
       * redeclaration. Any error on the gl_FragCoord is handled on the ast
       * level at apply_layout_qualifier_to_variable using the
       * ast_type_qualifier and _mesa_glsl_parse_state, or later at
       * linker.cpp.
       */
        /* According to section 4.3.7 of the GLSL 1.30 spec,
       * the following built-in varaibles can be redeclared with an
       * interpolation qualifier:
       *    * gl_FrontColor
       *    * gl_BackColor
       *    * gl_FrontSecondaryColor
       *    * gl_BackSecondaryColor
       *    * gl_Color
       *    * gl_SecondaryColor
       */
    } else if (state->is_version(130, 0)
               && (strcmp(var->name, "gl_FrontColor") == 0
                   || strcmp(var->name, "gl_BackColor") == 0
                   || strcmp(var->name, "gl_FrontSecondaryColor") == 0
                   || strcmp(var->name, "gl_BackSecondaryColor") == 0
                   || strcmp(var->name, "gl_Color") == 0
                   || strcmp(var->name, "gl_SecondaryColor") == 0)) {
        earlier->data.interpolation = var->data.interpolation;

        /* Layout qualifiers for gl_FragDepth. */
    } else if ((state->is_version(420, 0) ||
                state->AMD_conservative_depth_enable ||
                state->ARB_conservative_depth_enable)
               && strcmp(var->name, "gl_FragDepth") == 0) {

        /** From the AMD_conservative_depth spec:
       *     Within any shader, the first redeclarations of gl_FragDepth
       *     must appear before any use of gl_FragDepth.
       */
        if (earlier->data.used) {
            _mesa_glsl_error(&loc, state,
                             "the first redeclaration of gl_FragDepth "
                             "must appear before any use of gl_FragDepth");
        }

        /* Prevent inconsistent redeclaration of depth layout qualifier. */
        if (earlier->data.depth_layout != ir_depth_layout_none
            && earlier->data.depth_layout != var->data.depth_layout) {
            _mesa_glsl_error(&loc, state,
                             "gl_FragDepth: depth layout is declared here "
                             "as '%s, but it was previously declared as "
                             "'%s'",
                             depth_layout_string(var->data.depth_layout),
                             depth_layout_string(earlier->data.depth_layout));
        }

        earlier->data.depth_layout = var->data.depth_layout;

    } else if (state->has_framebuffer_fetch() &&
               strcmp(var->name, "gl_LastFragData") == 0 &&
               var->data.mode == ir_var_auto) {
        /* According to the EXT_shader_framebuffer_fetch spec:
       *
       *   "By default, gl_LastFragData is declared with the mediump precision
       *    qualifier. This can be changed by redeclaring the corresponding
       *    variables with the desired precision qualifier."
       *
       *   "Fragment shaders may specify the following layout qualifier only for
       *    redeclaring the built-in gl_LastFragData array [...]: noncoherent"
       */
        earlier->data.precision = var->data.precision;
        earlier->data.memory_coherent = var->data.memory_coherent;

    } else if (state->NV_viewport_array2_enable &&
               strcmp(var->name, "gl_Layer") == 0 &&
               earlier->data.how_declared == ir_var_declared_implicitly) {
        /* No need to do anything, just allow it. Qualifier is stored in state */

    } else if (state->is_version(0, 300) &&
               state->has_separate_shader_objects() &&
               (strcmp(var->name, "gl_Position") == 0 ||
                strcmp(var->name, "gl_PointSize") == 0)) {

        /*  EXT_separate_shader_objects spec says:
       *
       *  "The following vertex shader outputs may be redeclared
       *   at global scope to specify a built-in output interface,
       *   with or without special qualifiers:
       *
       *    gl_Position
       *    gl_PointSize
       *
       *    When compiling shaders using either of the above variables,
       *    both such variables must be redeclared prior to use."
       */
        if (earlier->data.used) {
            _mesa_glsl_error(&loc, state, "the first redeclaration of "
                                          "%s must appear before any use", var->name);
        }
    } else if ((earlier->data.how_declared == ir_var_declared_implicitly &&
                state->allow_builtin_variable_redeclaration) ||
               allow_all_redeclarations) {
        /* Allow verbatim redeclarations of built-in variables. Not explicitly
       * valid, but some applications do it.
       */
    } else {
        _mesa_glsl_error(&loc, state, "`%s' redeclared", var->name);
    }

    return earlier;
}

/**
 * Generate the IR for an initializer in a variable declaration
 */
static ir_rvalue *
process_initializer(ir_variable *var, ast_declaration *decl,
                    ast_fully_specified_type *type,
                    exec_list *initializer_instructions,
                    struct _mesa_glsl_parse_state *state)
{
    void *mem_ctx = state;
    ir_rvalue *result = NULL;

    YYLTYPE initializer_loc = decl->initializer->get_location();

    /* From page 24 (page 30 of the PDF) of the GLSL 1.10 spec:
    *
    *    "All uniform variables are read-only and are initialized either
    *    directly by an application via API commands, or indirectly by
    *    OpenGL."
    */
    if (var->data.mode == ir_var_uniform) {
        state->check_version(120, 0, &initializer_loc,
                             "cannot initialize uniform %s",
                             var->name);
    }

    /* Section 4.3.7 "Buffer Variables" of the GLSL 4.30 spec:
    *
    *    "Buffer variables cannot have initializers."
    */
    if (var->data.mode == ir_var_shader_storage) {
        _mesa_glsl_error(&initializer_loc, state,
                         "cannot initialize buffer variable %s",
                         var->name);
    }

    /* From section 4.1.7 of the GLSL 4.40 spec:
    *
    *    "Opaque variables [...] are initialized only through the
    *     OpenGL API; they cannot be declared with an initializer in a
    *     shader."
    *
    * From section 4.1.7 of the ARB_bindless_texture spec:
    *
    *    "Samplers may be declared as shader inputs and outputs, as uniform
    *     variables, as temporary variables, and as function parameters."
    *
    * From section 4.1.X of the ARB_bindless_texture spec:
    *
    *    "Images may be declared as shader inputs and outputs, as uniform
    *     variables, as temporary variables, and as function parameters."
    */
    if (var->type->contains_atomic() ||
        (!state->has_bindless() && var->type->contains_opaque())) {
        _mesa_glsl_error(&initializer_loc, state,
                         "cannot initialize %s variable %s",
                         var->name, state->has_bindless() ? "atomic" : "opaque");
    }

    if ((var->data.mode == ir_var_shader_in) && (state->current_function == NULL)) {
        _mesa_glsl_error(&initializer_loc, state,
                         "cannot initialize %s shader input / %s %s",
                         _mesa_shader_stage_to_string(state->stage),
                         (state->stage == MESA_SHADER_VERTEX)
                         ? "attribute" : "varying",
                         var->name);
    }

    if (var->data.mode == ir_var_shader_out && state->current_function == NULL) {
        _mesa_glsl_error(&initializer_loc, state,
                         "cannot initialize %s shader output %s",
                         _mesa_shader_stage_to_string(state->stage),
                         var->name);
    }

    /* If the initializer is an ast_aggregate_initializer, recursively store
    * type information from the LHS into it, so that its hir() function can do
    * type checking.
    */
    if (decl->initializer->oper == ast_aggregate)
        _mesa_ast_set_aggregate_type(var->type, decl->initializer);

    ir_dereference *const lhs = new(state) ir_dereference_variable(var);
    ir_rvalue *rhs = decl->initializer->hir(initializer_instructions, state);

    /* Calculate the constant value if this is a const or uniform
    * declaration.
    *
    * Section 4.3 (Storage Qualifiers) of the GLSL ES 1.00.17 spec says:
    *
    *     "Declarations of globals without a storage qualifier, or with
    *     just the const qualifier, may include initializers, in which case
    *     they will be initialized before the first line of main() is
    *     executed.  Such initializers must be a constant expression."
    *
    * The same section of the GLSL ES 3.00.4 spec has similar language.
    */
    if (type->qualifier.flags.q.constant
        || type->qualifier.flags.q.uniform
        || (state->es_shader && state->current_function == NULL)) {
        ir_rvalue *new_rhs = validate_assignment(state, initializer_loc,
                                                 lhs, rhs, true);
        if (new_rhs != NULL) {
            rhs = new_rhs;

            /* Section 4.3.3 (Constant Expressions) of the GLSL ES 3.00.4 spec
          * says:
          *
          *     "A constant expression is one of
          *
          *        ...
          *
          *        - an expression formed by an operator on operands that are
          *          all constant expressions, including getting an element of
          *          a constant array, or a field of a constant structure, or
          *          components of a constant vector.  However, the sequence
          *          operator ( , ) and the assignment operators ( =, +=, ...)
          *          are not included in the operators that can create a
          *          constant expression."
          *
          * Section 12.43 (Sequence operator and constant expressions) says:
          *
          *     "Should the following construct be allowed?
          *
          *         float a[2,3];
          *
          *     The expression within the brackets uses the sequence operator
          *     (',') and returns the integer 3 so the construct is declaring
          *     a single-dimensional array of size 3.  In some languages, the
          *     construct declares a two-dimensional array.  It would be
          *     preferable to make this construct illegal to avoid confusion.
          *
          *     One possibility is to change the definition of the sequence
          *     operator so that it does not return a constant-expression and
          *     hence cannot be used to declare an array size.
          *
          *     RESOLUTION: The result of a sequence operator is not a
          *     constant-expression."
          *
          * Section 4.3.3 (Constant Expressions) of the GLSL 4.30.9 spec
          * contains language almost identical to the section 4.3.3 in the
          * GLSL ES 3.00.4 spec.  This is a new limitation for these GLSL
          * versions.
          */
            ir_constant *constant_value =
                    rhs->constant_expression_value(mem_ctx);

            if (!constant_value ||
                (state->is_version(430, 300) &&
                 decl->initializer->has_sequence_subexpression())) {
                const char *const variable_mode =
                        (type->qualifier.flags.q.constant)
                        ? "const"
                        : ((type->qualifier.flags.q.uniform) ? "uniform" : "global");

                /* If ARB_shading_language_420pack is enabled, initializers of
             * const-qualified local variables do not have to be constant
             * expressions. Const-qualified global variables must still be
             * initialized with constant expressions.
             */
                if (!state->has_420pack()
                    || state->current_function == NULL) {
                    _mesa_glsl_error(& initializer_loc, state,
                                     "initializer of %s variable `%s' must be a "
                                     "constant expression",
                                     variable_mode,
                                     decl->identifier);
                    if (var->type->is_numeric()) {
                        /* Reduce cascading errors. */
                        var->constant_value = type->qualifier.flags.q.constant
                                              ? ir_constant::zero(state, var->type) : NULL;
                    }
                }
            } else {
                rhs = constant_value;
                var->constant_value = type->qualifier.flags.q.constant
                                      ? constant_value : NULL;
            }
        } else {
            if (var->type->is_numeric()) {
                /* Reduce cascading errors. */
                rhs = var->constant_value = type->qualifier.flags.q.constant
                                            ? ir_constant::zero(state, var->type) : NULL;
            }
        }
    }

    if (rhs && !rhs->type->is_error()) {
        bool temp = var->data.read_only;
        if (type->qualifier.flags.q.constant)
            var->data.read_only = false;

        /* Never emit code to initialize a uniform.
       */
        const glsl_type *initializer_type;
        bool error_emitted = false;
        if (!type->qualifier.flags.q.uniform) {
            error_emitted =
                    do_assignment(initializer_instructions, state,
                                  NULL, lhs, rhs,
                                  &result, true, true,
                                  type->get_location());
            initializer_type = result->type;
        } else
            initializer_type = rhs->type;

        if (!error_emitted) {
            var->constant_initializer = rhs->constant_expression_value(mem_ctx);
            var->data.has_initializer = true;
            var->data.is_implicit_initializer = false;

            /* If the declared variable is an unsized array, it must inherrit
         * its full type from the initializer.  A declaration such as
         *
         *     uniform float a[] = float[](1.0, 2.0, 3.0, 3.0);
         *
         * becomes
         *
         *     uniform float a[4] = float[](1.0, 2.0, 3.0, 3.0);
         *
         * The assignment generated in the if-statement (below) will also
         * automatically handle this case for non-uniforms.
         *
         * If the declared variable is not an array, the types must
         * already match exactly.  As a result, the type assignment
         * here can be done unconditionally.  For non-uniforms the call
         * to do_assignment can change the type of the initializer (via
         * the implicit conversion rules).  For uniforms the initializer
         * must be a constant expression, and the type of that expression
         * was validated above.
         */
            var->type = initializer_type;
        }

        var->data.read_only = temp;
    }

    return result;
}

static void
validate_layout_qualifier_vertex_count(struct _mesa_glsl_parse_state *state,
                                       YYLTYPE loc, ir_variable *var,
                                       unsigned num_vertices,
                                       unsigned *size,
                                       const char *var_category)
{
    if (var->type->is_unsized_array()) {
        /* Section 4.3.8.1 (Input Layout Qualifiers) of the GLSL 1.50 spec says:
       *
       *   All geometry shader input unsized array declarations will be
       *   sized by an earlier input layout qualifier, when present, as per
       *   the following table.
       *
       * Followed by a table mapping each allowed input layout qualifier to
       * the corresponding input length.
       *
       * Similarly for tessellation control shader outputs.
       */
        if (num_vertices != 0)
            var->type = glsl_type::get_array_instance(var->type->fields.array,
                                                      num_vertices);
    } else {
        /* Section 4.3.8.1 (Input Layout Qualifiers) of the GLSL 1.50 spec
       * includes the following examples of compile-time errors:
       *
       *   // code sequence within one shader...
       *   in vec4 Color1[];    // size unknown
       *   ...Color1.length()...// illegal, length() unknown
       *   in vec4 Color2[2];   // size is 2
       *   ...Color1.length()...// illegal, Color1 still has no size
       *   in vec4 Color3[3];   // illegal, input sizes are inconsistent
       *   layout(lines) in;    // legal, input size is 2, matching
       *   in vec4 Color4[3];   // illegal, contradicts layout
       *   ...
       *
       * To detect the case illustrated by Color3, we verify that the size of
       * an explicitly-sized array matches the size of any previously declared
       * explicitly-sized array.  To detect the case illustrated by Color4, we
       * verify that the size of an explicitly-sized array is consistent with
       * any previously declared input layout.
       */
        if (num_vertices != 0 && var->type->length != num_vertices) {
            _mesa_glsl_error(&loc, state,
                             "%s size contradicts previously declared layout "
                             "(size is %u, but layout requires a size of %u)",
                             var_category, var->type->length, num_vertices);
        } else if (*size != 0 && var->type->length != *size) {
            _mesa_glsl_error(&loc, state,
                             "%s sizes are inconsistent (size is %u, but a "
                             "previous declaration has size %u)",
                             var_category, var->type->length, *size);
        } else {
            *size = var->type->length;
        }
    }
}

static void
handle_tess_ctrl_shader_output_decl(struct _mesa_glsl_parse_state *state,
                                    YYLTYPE loc, ir_variable *var)
{
    unsigned num_vertices = 0;

    if (state->tcs_output_vertices_specified) {
        if (!state->out_qualifier->vertices->
                process_qualifier_constant(state, "vertices",
                                           &num_vertices, false)) {
            return;
        }

        if (num_vertices > state->Const.MaxPatchVertices) {
            _mesa_glsl_error(&loc, state, "vertices (%d) exceeds "
                                          "GL_MAX_PATCH_VERTICES", num_vertices);
            return;
        }
    }

    if (!var->type->is_array() && !var->data.patch) {
        _mesa_glsl_error(&loc, state,
                         "tessellation control shader outputs must be arrays");

        /* To avoid cascading failures, short circuit the checks below. */
        return;
    }

    if (var->data.patch)
        return;

    validate_layout_qualifier_vertex_count(state, loc, var, num_vertices,
                                           &state->tcs_output_size,
                                           "tessellation control shader output");
}

/**
 * Do additional processing necessary for tessellation control/evaluation shader
 * input declarations. This covers both interface block arrays and bare input
 * variables.
 */
static void
handle_tess_shader_input_decl(struct _mesa_glsl_parse_state *state,
                              YYLTYPE loc, ir_variable *var)
{
    if (!var->type->is_array() && !var->data.patch) {
        _mesa_glsl_error(&loc, state,
                         "per-vertex tessellation shader inputs must be arrays");
        /* Avoid cascading failures. */
        return;
    }

    if (var->data.patch)
        return;

    /* The ARB_tessellation_shader spec says:
    *
    *    "Declaring an array size is optional.  If no size is specified, it
    *     will be taken from the implementation-dependent maximum patch size
    *     (gl_MaxPatchVertices).  If a size is specified, it must match the
    *     maximum patch size; otherwise, a compile or link error will occur."
    *
    * This text appears twice, once for TCS inputs, and again for TES inputs.
    */
    if (var->type->is_unsized_array()) {
        var->type = glsl_type::get_array_instance(var->type->fields.array,
                                                  state->Const.MaxPatchVertices);
    } else if (var->type->length != state->Const.MaxPatchVertices) {
        _mesa_glsl_error(&loc, state,
                         "per-vertex tessellation shader input arrays must be "
                         "sized to gl_MaxPatchVertices (%d).",
                         state->Const.MaxPatchVertices);
    }
}


/**
 * Do additional processing necessary for geometry shader input declarations
 * (this covers both interface blocks arrays and bare input variables).
 */
static void
handle_geometry_shader_input_decl(struct _mesa_glsl_parse_state *state,
                                  YYLTYPE loc, ir_variable *var)
{
    unsigned num_vertices = 0;

    if (state->gs_input_prim_type_specified) {
        num_vertices = vertices_per_prim(state->in_qualifier->prim_type);
    }

    /* Geometry shader input variables must be arrays.  Caller should have
    * reported an error for this.
    */
    if (!var->type->is_array()) {
        assert(state->error);

        /* To avoid cascading failures, short circuit the checks below. */
        return;
    }

    validate_layout_qualifier_vertex_count(state, loc, var, num_vertices,
                                           &state->gs_input_size,
                                           "geometry shader input");
}

static void
validate_identifier(const char *identifier, YYLTYPE loc,
                    struct _mesa_glsl_parse_state *state)
{
    /* From page 15 (page 21 of the PDF) of the GLSL 1.10 spec,
    *
    *   "Identifiers starting with "gl_" are reserved for use by
    *   OpenGL, and may not be declared in a shader as either a
    *   variable or a function."
    */
    if (is_gl_identifier(identifier)) {
        _mesa_glsl_error(&loc, state,
                         "identifier `%s' uses reserved `gl_' prefix",
                         identifier);
    } else if (strstr(identifier, "__")) {
        /* From page 14 (page 20 of the PDF) of the GLSL 1.10
       * spec:
       *
       *     "In addition, all identifiers containing two
       *      consecutive underscores (__) are reserved as
       *      possible future keywords."
       *
       * The intention is that names containing __ are reserved for internal
       * use by the implementation, and names prefixed with GL_ are reserved
       * for use by Khronos.  Names simply containing __ are dangerous to use,
       * but should be allowed.
       *
       * A future version of the GLSL specification will clarify this.
       */
        _mesa_glsl_warning(&loc, state,
                           "identifier `%s' uses reserved `__' string",
                           identifier);
    }
}

ir_rvalue *
ast_declarator_list::hir(exec_list *instructions,
                         struct _mesa_glsl_parse_state *state)
{
    void *ctx = state;
    const struct glsl_type *decl_type;
    const char *type_name = NULL;
    ir_rvalue *result = NULL;
    YYLTYPE loc = this->get_location();

    /* From page 46 (page 52 of the PDF) of the GLSL 1.50 spec:
    *
    *     "To ensure that a particular output variable is invariant, it is
    *     necessary to use the invariant qualifier. It can either be used to
    *     qualify a previously declared variable as being invariant
    *
    *         invariant gl_Position; // make existing gl_Position be invariant"
    *
    * In these cases the parser will set the 'invariant' flag in the declarator
    * list, and the type will be NULL.
    */
    if (this->invariant) {
        assert(this->type == NULL);

        if (state->current_function != NULL) {
            _mesa_glsl_error(& loc, state,
                             "all uses of `invariant' keyword must be at global "
                             "scope");
        }

        foreach_list_typed (ast_declaration, decl, link, &this->declarations) {
            assert(decl->array_specifier == NULL);
            assert(decl->initializer == NULL);

            ir_variable *const earlier =
                    state->symbols->get_variable(decl->identifier);
            if (earlier == NULL) {
                _mesa_glsl_error(& loc, state,
                                 "undeclared variable `%s' cannot be marked "
                                 "invariant", decl->identifier);
            } else if (!is_allowed_invariant(earlier, state)) {
                _mesa_glsl_error(&loc, state,
                                 "`%s' cannot be marked invariant; interfaces between "
                                 "shader stages only.", decl->identifier);
            } else if (earlier->data.used) {
                _mesa_glsl_error(& loc, state,
                                 "variable `%s' may not be redeclared "
                                 "`invariant' after being used",
                                 earlier->name);
            } else {
                earlier->data.explicit_invariant = true;
                earlier->data.invariant = true;
            }
        }

        /* Invariant redeclarations do not have r-values.
       */
        return NULL;
    }

    if (this->precise) {
        assert(this->type == NULL);

        foreach_list_typed (ast_declaration, decl, link, &this->declarations) {
            assert(decl->array_specifier == NULL);
            assert(decl->initializer == NULL);

            ir_variable *const earlier =
                    state->symbols->get_variable(decl->identifier);
            if (earlier == NULL) {
                _mesa_glsl_error(& loc, state,
                                 "undeclared variable `%s' cannot be marked "
                                 "precise", decl->identifier);
            } else if (state->current_function != NULL &&
                       !state->symbols->name_declared_this_scope(decl->identifier)) {
                /* Note: we have to check if we're in a function, since
             * builtins are treated as having come from another scope.
             */
                _mesa_glsl_error(& loc, state,
                                 "variable `%s' from an outer scope may not be "
                                 "redeclared `precise' in this scope",
                                 earlier->name);
            } else if (earlier->data.used) {
                _mesa_glsl_error(& loc, state,
                                 "variable `%s' may not be redeclared "
                                 "`precise' after being used",
                                 earlier->name);
            } else {
                earlier->data.precise = true;
            }
        }

        /* Precise redeclarations do not have r-values either. */
        return NULL;
    }

    assert(this->type != NULL);
    assert(!this->invariant);
    assert(!this->precise);

    /* GL_EXT_shader_image_load_store base type uses GLSL_TYPE_VOID as a special value to
    * indicate that it needs to be updated later (see glsl_parser.yy).
    * This is done here, based on the layout qualifier and the type of the image var
    */
    if (this->type->qualifier.flags.q.explicit_image_format &&
        this->type->specifier->type->is_image() &&
        this->type->qualifier.image_base_type == GLSL_TYPE_VOID) {
        /*     "The ARB_shader_image_load_store says:
       *     If both extensions are enabled in the shading language, the "size*" layout
       *     qualifiers are treated as format qualifiers, and are mapped to equivalent
       *     format qualifiers in the table below, according to the type of image
       *     variable.
       *                     image*    iimage*   uimage*
       *                     --------  --------  --------
       *       size1x8       n/a       r8i       r8ui
       *       size1x16      r16f      r16i      r16ui
       *       size1x32      r32f      r32i      r32ui
       *       size2x32      rg32f     rg32i     rg32ui
       *       size4x32      rgba32f   rgba32i   rgba32ui"
       */
        if (strncmp(this->type->specifier->type_name, "image", strlen("image")) == 0) {
            switch (this->type->qualifier.image_format) {
                case PIPE_FORMAT_R8_SINT:
                    /* The GL_EXT_shader_image_load_store spec says:
             *    A layout of "size1x8" is illegal for image variables associated
             *    with floating-point data types.
             */
                    _mesa_glsl_error(& loc, state,
                                     "size1x8 is illegal for image variables "
                                     "with floating-point data types.");
                    return NULL;
                case PIPE_FORMAT_R16_SINT:
                    this->type->qualifier.image_format = PIPE_FORMAT_R16_FLOAT;
                    break;
                case PIPE_FORMAT_R32_SINT:
                    this->type->qualifier.image_format = PIPE_FORMAT_R32_FLOAT;
                    break;
                case PIPE_FORMAT_R32G32_SINT:
                    this->type->qualifier.image_format = PIPE_FORMAT_R32G32_FLOAT;
                    break;
                case PIPE_FORMAT_R32G32B32A32_SINT:
                    this->type->qualifier.image_format = PIPE_FORMAT_R32G32B32A32_FLOAT;
                    break;
                default:
                    unreachable("Unknown image format");
            }
            this->type->qualifier.image_base_type = GLSL_TYPE_FLOAT;
        } else if (strncmp(this->type->specifier->type_name, "uimage", strlen("uimage")) == 0) {
            switch (this->type->qualifier.image_format) {
                case PIPE_FORMAT_R8_SINT:
                    this->type->qualifier.image_format = PIPE_FORMAT_R8_UINT;
                    break;
                case PIPE_FORMAT_R16_SINT:
                    this->type->qualifier.image_format = PIPE_FORMAT_R16_UINT;
                    break;
                case PIPE_FORMAT_R32_SINT:
                    this->type->qualifier.image_format = PIPE_FORMAT_R32_UINT;
                    break;
                case PIPE_FORMAT_R32G32_SINT:
                    this->type->qualifier.image_format = PIPE_FORMAT_R32G32_UINT;
                    break;
                case PIPE_FORMAT_R32G32B32A32_SINT:
                    this->type->qualifier.image_format = PIPE_FORMAT_R32G32B32A32_UINT;
                    break;
                default:
                    unreachable("Unknown image format");
            }
            this->type->qualifier.image_base_type = GLSL_TYPE_UINT;
        } else if (strncmp(this->type->specifier->type_name, "iimage", strlen("iimage")) == 0) {
            this->type->qualifier.image_base_type = GLSL_TYPE_INT;
        } else {
            assert(false);
        }
    }

    /* The type specifier may contain a structure definition.  Process that
    * before any of the variable declarations.
    */
    (void) this->type->specifier->hir(instructions, state);

    decl_type = this->type->glsl_type(& type_name, state);

    /* Section 4.3.7 "Buffer Variables" of the GLSL 4.30 spec:
    *    "Buffer variables may only be declared inside interface blocks
    *    (section 4.3.9 “Interface Blocks”), which are then referred to as
    *    shader storage blocks. It is a compile-time error to declare buffer
    *    variables at global scope (outside a block)."
    */
    if (type->qualifier.flags.q.buffer && !decl_type->is_interface()) {
        _mesa_glsl_error(&loc, state,
                         "buffer variables cannot be declared outside "
                         "interface blocks");
    }

    /* An offset-qualified atomic counter declaration sets the default
    * offset for the next declaration within the same atomic counter
    * buffer.
    */
    if (decl_type && decl_type->contains_atomic()) {
        if (type->qualifier.flags.q.explicit_binding &&
            type->qualifier.flags.q.explicit_offset) {
            unsigned qual_binding;
            unsigned qual_offset;
            if (process_qualifier_constant(state, &loc, "binding",
                                           type->qualifier.binding,
                                           &qual_binding)
                && process_qualifier_constant(state, &loc, "offset",
                                              type->qualifier.offset,
                                              &qual_offset)) {
                if (qual_binding < ARRAY_SIZE(state->atomic_counter_offsets))
                    state->atomic_counter_offsets[qual_binding] = qual_offset;
            }
        }

        ast_type_qualifier allowed_atomic_qual_mask;
        allowed_atomic_qual_mask.flags.i = 0;
        allowed_atomic_qual_mask.flags.q.explicit_binding = 1;
        allowed_atomic_qual_mask.flags.q.explicit_offset = 1;
        allowed_atomic_qual_mask.flags.q.uniform = 1;

        type->qualifier.validate_flags(&loc, state, allowed_atomic_qual_mask,
                                       "invalid layout qualifier for",
                                       "atomic_uint");
    }

    if (this->declarations.is_empty()) {
        /* If there is no structure involved in the program text, there are two
       * possible scenarios:
       *
       * - The program text contained something like 'vec4;'.  This is an
       *   empty declaration.  It is valid but weird.  Emit a warning.
       *
       * - The program text contained something like 'S;' and 'S' is not the
       *   name of a known structure type.  This is both invalid and weird.
       *   Emit an error.
       *
       * - The program text contained something like 'mediump float;'
       *   when the programmer probably meant 'precision mediump
       *   float;' Emit a warning with a description of what they
       *   probably meant to do.
       *
       * Note that if decl_type is NULL and there is a structure involved,
       * there must have been some sort of error with the structure.  In this
       * case we assume that an error was already generated on this line of
       * code for the structure.  There is no need to generate an additional,
       * confusing error.
       */
        assert(this->type->specifier->structure == NULL || decl_type != NULL
               || state->error);

        if (decl_type == NULL) {
            _mesa_glsl_error(&loc, state,
                             "invalid type `%s' in empty declaration",
                             type_name);
        } else {
            if (decl_type->is_array()) {
                /* From Section 13.22 (Array Declarations) of the GLSL ES 3.2
             * spec:
             *
             *    "... any declaration that leaves the size undefined is
             *    disallowed as this would add complexity and there are no
             *    use-cases."
             */
                if (state->es_shader && decl_type->is_unsized_array()) {
                    _mesa_glsl_error(&loc, state, "array size must be explicitly "
                                                  "or implicitly defined");
                }

                /* From Section 4.12 (Empty Declarations) of the GLSL 4.5 spec:
             *
             *    "The combinations of types and qualifiers that cause
             *    compile-time or link-time errors are the same whether or not
             *    the declaration is empty."
             */
                validate_array_dimensions(decl_type, state, &loc);
            }

            if (decl_type->is_atomic_uint()) {
                /* Empty atomic counter declarations are allowed and useful
             * to set the default offset qualifier.
             */
                return NULL;
            } else if (this->type->qualifier.precision != ast_precision_none) {
                if (this->type->specifier->structure != NULL) {
                    _mesa_glsl_error(&loc, state,
                                     "precision qualifiers can't be applied "
                                     "to structures");
                } else {
                    static const char *const precision_names[] = {
                            "highp",
                            "highp",
                            "mediump",
                            "lowp"
                    };

                    _mesa_glsl_warning(&loc, state,
                                       "empty declaration with precision "
                                       "qualifier, to set the default precision, "
                                       "use `precision %s %s;'",
                                       precision_names[this->type->
                                               qualifier.precision],
                                       type_name);
                }
            } else if (this->type->specifier->structure == NULL) {
                _mesa_glsl_warning(&loc, state, "empty declaration");
            }
        }
    }

    foreach_list_typed (ast_declaration, decl, link, &this->declarations) {
        const struct glsl_type *var_type;
        ir_variable *var;
        const char *identifier = decl->identifier;
        /* FINISHME: Emit a warning if a variable declaration shadows a
       * FINISHME: declaration at a higher scope.
       */

        if ((decl_type == NULL) || decl_type->is_void()) {
            if (type_name != NULL) {
                _mesa_glsl_error(& loc, state,
                                 "invalid type `%s' in declaration of `%s'",
                                 type_name, decl->identifier);
            } else {
                _mesa_glsl_error(& loc, state,
                                 "invalid type in declaration of `%s'",
                                 decl->identifier);
            }
            continue;
        }

        if (this->type->qualifier.is_subroutine_decl()) {
            const glsl_type *t;
            const char *name;

            t = state->symbols->get_type(this->type->specifier->type_name);
            if (!t)
                _mesa_glsl_error(& loc, state,
                                 "invalid type in declaration of `%s'",
                                 decl->identifier);
            name = ralloc_asprintf(ctx, "%s_%s", _mesa_shader_stage_to_subroutine_prefix(state->stage), decl->identifier);

            identifier = name;

        }
        var_type = process_array_type(&loc, decl_type, decl->array_specifier,
                                      state);

        var = new(ctx) ir_variable(var_type, identifier, ir_var_auto);

        /* The 'varying in' and 'varying out' qualifiers can only be used with
       * ARB_geometry_shader4 and EXT_geometry_shader4, which we don't support
       * yet.
       */
        if (this->type->qualifier.flags.q.varying) {
            if (this->type->qualifier.flags.q.in) {
                _mesa_glsl_error(& loc, state,
                                 "`varying in' qualifier in declaration of "
                                 "`%s' only valid for geometry shaders using "
                                 "ARB_geometry_shader4 or EXT_geometry_shader4",
                                 decl->identifier);
            } else if (this->type->qualifier.flags.q.out) {
                _mesa_glsl_error(& loc, state,
                                 "`varying out' qualifier in declaration of "
                                 "`%s' only valid for geometry shaders using "
                                 "ARB_geometry_shader4 or EXT_geometry_shader4",
                                 decl->identifier);
            }
        }

        /* From page 22 (page 28 of the PDF) of the GLSL 1.10 specification;
       *
       *     "Global variables can only use the qualifiers const,
       *     attribute, uniform, or varying. Only one may be
       *     specified.
       *
       *     Local variables can only use the qualifier const."
       *
       * This is relaxed in GLSL 1.30 and GLSL ES 3.00.  It is also relaxed by
       * any extension that adds the 'layout' keyword.
       */
        if (!state->is_version(130, 300)
            && !state->has_explicit_attrib_location()
            && !state->has_separate_shader_objects()
            && !state->ARB_fragment_coord_conventions_enable) {
            /* GL_EXT_gpu_shader4 only allows "varying out" on fragment shader
          * outputs. (the varying flag is not set by the parser)
          */
            if (this->type->qualifier.flags.q.out &&
                (!state->EXT_gpu_shader4_enable ||
                 state->stage != MESA_SHADER_FRAGMENT)) {
                _mesa_glsl_error(& loc, state,
                                 "`out' qualifier in declaration of `%s' "
                                 "only valid for function parameters in %s",
                                 decl->identifier, state->get_version_string());
            }
            if (this->type->qualifier.flags.q.in) {
                _mesa_glsl_error(& loc, state,
                                 "`in' qualifier in declaration of `%s' "
                                 "only valid for function parameters in %s",
                                 decl->identifier, state->get_version_string());
            }
            /* FINISHME: Test for other invalid qualifiers. */
        }

        apply_type_qualifier_to_variable(& this->type->qualifier, var, state,
                                         & loc, false);
        apply_layout_qualifier_to_variable(&this->type->qualifier, var, state,
                                           &loc);

        if ((state->zero_init & (1u << var->data.mode)) &&
            (var->type->is_numeric() || var->type->is_boolean())) {
            const ir_constant_data data = { { 0 } };
            var->data.has_initializer = true;
            var->data.is_implicit_initializer = true;
            var->constant_initializer = new(var) ir_constant(var->type, &data);
        }

        if (this->type->qualifier.flags.q.invariant) {
            if (!is_allowed_invariant(var, state)) {
                _mesa_glsl_error(&loc, state,
                                 "`%s' cannot be marked invariant; interfaces between "
                                 "shader stages only", var->name);
            }
        }

        if (state->current_function != NULL) {
            const char *mode = NULL;
            const char *extra = "";

            /* There is no need to check for 'inout' here because the parser will
          * only allow that in function parameter lists.
          */
            if (this->type->qualifier.flags.q.attribute) {
                mode = "attribute";
            } else if (this->type->qualifier.is_subroutine_decl()) {
                mode = "subroutine uniform";
            } else if (this->type->qualifier.flags.q.uniform) {
                mode = "uniform";
            } else if (this->type->qualifier.flags.q.varying) {
                mode = "varying";
            } else if (this->type->qualifier.flags.q.in) {
                mode = "in";
                extra = " or in function parameter list";
            } else if (this->type->qualifier.flags.q.out) {
                mode = "out";
                extra = " or in function parameter list";
            }

            if (mode) {
                _mesa_glsl_error(& loc, state,
                                 "%s variable `%s' must be declared at "
                                 "global scope%s",
                                 mode, var->name, extra);
            }
        } else if (var->data.mode == ir_var_shader_in) {
            var->data.read_only = true;

            if (state->stage == MESA_SHADER_VERTEX) {
                /* From page 31 (page 37 of the PDF) of the GLSL 1.50 spec:
             *
             *    "Vertex shader inputs can only be float, floating-point
             *    vectors, matrices, signed and unsigned integers and integer
             *    vectors. Vertex shader inputs can also form arrays of these
             *    types, but not structures."
             *
             * From page 31 (page 27 of the PDF) of the GLSL 1.30 spec:
             *
             *    "Vertex shader inputs can only be float, floating-point
             *    vectors, matrices, signed and unsigned integers and integer
             *    vectors. They cannot be arrays or structures."
             *
             * From page 23 (page 29 of the PDF) of the GLSL 1.20 spec:
             *
             *    "The attribute qualifier can be used only with float,
             *    floating-point vectors, and matrices. Attribute variables
             *    cannot be declared as arrays or structures."
             *
             * From page 33 (page 39 of the PDF) of the GLSL ES 3.00 spec:
             *
             *    "Vertex shader inputs can only be float, floating-point
             *    vectors, matrices, signed and unsigned integers and integer
             *    vectors. Vertex shader inputs cannot be arrays or
             *    structures."
             *
             * From section 4.3.4 of the ARB_bindless_texture spec:
             *
             *    "(modify third paragraph of the section to allow sampler and
             *    image types) ...  Vertex shader inputs can only be float,
             *    single-precision floating-point scalars, single-precision
             *    floating-point vectors, matrices, signed and unsigned
             *    integers and integer vectors, sampler and image types."
             */
                const glsl_type *check_type = var->type->without_array();

                bool error = false;
                switch (check_type->base_type) {
                    case GLSL_TYPE_FLOAT:
                        break;
                    case GLSL_TYPE_UINT64:
                    case GLSL_TYPE_INT64:
                        break;
                    case GLSL_TYPE_UINT:
                    case GLSL_TYPE_INT:
                        error = !state->is_version(120, 300) && !state->EXT_gpu_shader4_enable;
                        break;
                    case GLSL_TYPE_DOUBLE:
                        error = !state->is_version(410, 0) && !state->ARB_vertex_attrib_64bit_enable;
                        break;
                    case GLSL_TYPE_SAMPLER:
                    case GLSL_TYPE_IMAGE:
                        error = !state->has_bindless();
                        break;
                    default:
                        error = true;
                }

                if (error) {
                    _mesa_glsl_error(& loc, state,
                                     "vertex shader input / attribute cannot have "
                                     "type %s`%s'",
                                     var->type->is_array() ? "array of " : "",
                                     check_type->name);
                } else if (var->type->is_array() &&
                           !state->check_version(150, 0, &loc,
                                                 "vertex shader input / attribute "
                                                 "cannot have array type")) {
                }
            } else if (state->stage == MESA_SHADER_GEOMETRY) {
                /* From section 4.3.4 (Inputs) of the GLSL 1.50 spec:
             *
             *     Geometry shader input variables get the per-vertex values
             *     written out by vertex shader output variables of the same
             *     names. Since a geometry shader operates on a set of
             *     vertices, each input varying variable (or input block, see
             *     interface blocks below) needs to be declared as an array.
             */
                if (!var->type->is_array()) {
                    _mesa_glsl_error(&loc, state,
                                     "geometry shader inputs must be arrays");
                }

                handle_geometry_shader_input_decl(state, loc, var);
            } else if (state->stage == MESA_SHADER_FRAGMENT) {
                /* From section 4.3.4 (Input Variables) of the GLSL ES 3.10 spec:
             *
             *     It is a compile-time error to declare a fragment shader
             *     input with, or that contains, any of the following types:
             *
             *     * A boolean type
             *     * An opaque type
             *     * An array of arrays
             *     * An array of structures
             *     * A structure containing an array
             *     * A structure containing a structure
             */
                if (state->es_shader) {
                    const glsl_type *check_type = var->type->without_array();
                    if (check_type->is_boolean() ||
                        check_type->contains_opaque()) {
                        _mesa_glsl_error(&loc, state,
                                         "fragment shader input cannot have type %s",
                                         check_type->name);
                    }
                    if (var->type->is_array() &&
                        var->type->fields.array->is_array()) {
                        _mesa_glsl_error(&loc, state,
                                         "%s shader output "
                                         "cannot have an array of arrays",
                                         _mesa_shader_stage_to_string(state->stage));
                    }
                    if (var->type->is_array() &&
                        var->type->fields.array->is_struct()) {
                        _mesa_glsl_error(&loc, state,
                                         "fragment shader input "
                                         "cannot have an array of structs");
                    }
                    if (var->type->is_struct()) {
                        for (unsigned i = 0; i < var->type->length; i++) {
                            if (var->type->fields.structure[i].type->is_array() ||
                                var->type->fields.structure[i].type->is_struct())
                                _mesa_glsl_error(&loc, state,
                                                 "fragment shader input cannot have "
                                                 "a struct that contains an "
                                                 "array or struct");
                        }
                    }
                }
            } else if (state->stage == MESA_SHADER_TESS_CTRL ||
                       state->stage == MESA_SHADER_TESS_EVAL) {
                handle_tess_shader_input_decl(state, loc, var);
            }
        } else if (var->data.mode == ir_var_shader_out) {
            const glsl_type *check_type = var->type->without_array();

            /* From section 4.3.6 (Output variables) of the GLSL 4.40 spec:
          *
          *     It is a compile-time error to declare a fragment shader output
          *     that contains any of the following:
          *
          *     * A Boolean type (bool, bvec2 ...)
          *     * A double-precision scalar or vector (double, dvec2 ...)
          *     * An opaque type
          *     * Any matrix type
          *     * A structure
          */
            if (state->stage == MESA_SHADER_FRAGMENT) {
                if (check_type->is_struct() || check_type->is_matrix())
                    _mesa_glsl_error(&loc, state,
                                     "fragment shader output "
                                     "cannot have struct or matrix type");
                switch (check_type->base_type) {
                    case GLSL_TYPE_UINT:
                    case GLSL_TYPE_INT:
                    case GLSL_TYPE_FLOAT:
                        break;
                    default:
                        _mesa_glsl_error(&loc, state,
                                         "fragment shader output cannot have "
                                         "type %s", check_type->name);
                }
            }

            /* From section 4.3.6 (Output Variables) of the GLSL ES 3.10 spec:
          *
          *     It is a compile-time error to declare a vertex shader output
          *     with, or that contains, any of the following types:
          *
          *     * A boolean type
          *     * An opaque type
          *     * An array of arrays
          *     * An array of structures
          *     * A structure containing an array
          *     * A structure containing a structure
          *
          *     It is a compile-time error to declare a fragment shader output
          *     with, or that contains, any of the following types:
          *
          *     * A boolean type
          *     * An opaque type
          *     * A matrix
          *     * A structure
          *     * An array of array
          *
          * ES 3.20 updates this to apply to tessellation and geometry shaders
          * as well.  Because there are per-vertex arrays in the new stages,
          * it strikes the "array of..." rules and replaces them with these:
          *
          *     * For per-vertex-arrayed variables (applies to tessellation
          *       control, tessellation evaluation and geometry shaders):
          *
          *       * Per-vertex-arrayed arrays of arrays
          *       * Per-vertex-arrayed arrays of structures
          *
          *     * For non-per-vertex-arrayed variables:
          *
          *       * An array of arrays
          *       * An array of structures
          *
          * which basically says to unwrap the per-vertex aspect and apply
          * the old rules.
          */
            if (state->es_shader) {
                if (var->type->is_array() &&
                    var->type->fields.array->is_array()) {
                    _mesa_glsl_error(&loc, state,
                                     "%s shader output "
                                     "cannot have an array of arrays",
                                     _mesa_shader_stage_to_string(state->stage));
                }
                if (state->stage <= MESA_SHADER_GEOMETRY) {
                    const glsl_type *type = var->type;

                    if (state->stage == MESA_SHADER_TESS_CTRL &&
                        !var->data.patch && var->type->is_array()) {
                        type = var->type->fields.array;
                    }

                    if (type->is_array() && type->fields.array->is_struct()) {
                        _mesa_glsl_error(&loc, state,
                                         "%s shader output cannot have "
                                         "an array of structs",
                                         _mesa_shader_stage_to_string(state->stage));
                    }
                    if (type->is_struct()) {
                        for (unsigned i = 0; i < type->length; i++) {
                            if (type->fields.structure[i].type->is_array() ||
                                type->fields.structure[i].type->is_struct())
                                _mesa_glsl_error(&loc, state,
                                                 "%s shader output cannot have a "
                                                 "struct that contains an "
                                                 "array or struct",
                                                 _mesa_shader_stage_to_string(state->stage));
                        }
                    }
                }
            }

            if (state->stage == MESA_SHADER_TESS_CTRL) {
                handle_tess_ctrl_shader_output_decl(state, loc, var);
            }
        } else if (var->type->contains_subroutine()) {
            /* declare subroutine uniforms as hidden */
            var->data.how_declared = ir_var_hidden;
        }

        /* From section 4.3.4 of the GLSL 4.00 spec:
       *    "Input variables may not be declared using the patch in qualifier
       *    in tessellation control or geometry shaders."
       *
       * From section 4.3.6 of the GLSL 4.00 spec:
       *    "It is an error to use patch out in a vertex, tessellation
       *    evaluation, or geometry shader."
       *
       * This doesn't explicitly forbid using them in a fragment shader, but
       * that's probably just an oversight.
       */
        if (state->stage != MESA_SHADER_TESS_EVAL
            && this->type->qualifier.flags.q.patch
            && this->type->qualifier.flags.q.in) {

            _mesa_glsl_error(&loc, state, "'patch in' can only be used in a "
                                          "tessellation evaluation shader");
        }

        if (state->stage != MESA_SHADER_TESS_CTRL
            && this->type->qualifier.flags.q.patch
            && this->type->qualifier.flags.q.out) {

            _mesa_glsl_error(&loc, state, "'patch out' can only be used in a "
                                          "tessellation control shader");
        }

        /* Precision qualifiers exists only in GLSL versions 1.00 and >= 1.30.
       */
        if (this->type->qualifier.precision != ast_precision_none) {
            state->check_precision_qualifiers_allowed(&loc);
        }

        if (this->type->qualifier.precision != ast_precision_none &&
            !precision_qualifier_allowed(var->type)) {
            _mesa_glsl_error(&loc, state,
                             "precision qualifiers apply only to floating point"
                             ", integer and opaque types");
        }

        /* From section 4.1.7 of the GLSL 4.40 spec:
       *
       *    "[Opaque types] can only be declared as function
       *     parameters or uniform-qualified variables."
       *
       * From section 4.1.7 of the ARB_bindless_texture spec:
       *
       *    "Samplers may be declared as shader inputs and outputs, as uniform
       *     variables, as temporary variables, and as function parameters."
       *
       * From section 4.1.X of the ARB_bindless_texture spec:
       *
       *    "Images may be declared as shader inputs and outputs, as uniform
       *     variables, as temporary variables, and as function parameters."
       */
        if (!this->type->qualifier.flags.q.uniform &&
            (var_type->contains_atomic() ||
             (!state->has_bindless() && var_type->contains_opaque()))) {
            _mesa_glsl_error(&loc, state,
                             "%s variables must be declared uniform",
                             state->has_bindless() ? "atomic" : "opaque");
        }

        /* Process the initializer and add its instructions to a temporary
       * list.  This list will be added to the instruction stream (below) after
       * the declaration is added.  This is done because in some cases (such as
       * redeclarations) the declaration may not actually be added to the
       * instruction stream.
       */
        exec_list initializer_instructions;

        /* Examine var name here since var may get deleted in the next call */
        bool var_is_gl_id = is_gl_identifier(var->name);

        bool is_redeclaration;
        var = get_variable_being_redeclared(&var, decl->get_location(), state,
                                            false /* allow_all_redeclarations */,
                                            &is_redeclaration);
        if (is_redeclaration) {
            if (var_is_gl_id &&
                var->data.how_declared == ir_var_declared_in_block) {
                _mesa_glsl_error(&loc, state,
                                 "`%s' has already been redeclared using "
                                 "gl_PerVertex", var->name);
            }
            var->data.how_declared = ir_var_declared_normally;
        }

        if (decl->initializer != NULL) {
            result = process_initializer(var,
                                         decl, this->type,
                                         &initializer_instructions, state);
        } else {
            validate_array_dimensions(var_type, state, &loc);
        }

        /* From page 23 (page 29 of the PDF) of the GLSL 1.10 spec:
       *
       *     "It is an error to write to a const variable outside of
       *      its declaration, so they must be initialized when
       *      declared."
       */
        if (this->type->qualifier.flags.q.constant && decl->initializer == NULL) {
            _mesa_glsl_error(& loc, state,
                             "const declaration of `%s' must be initialized",
                             decl->identifier);
        }

        if (state->es_shader) {
            const glsl_type *const t = var->type;

            /* Skip the unsized array check for TCS/TES/GS inputs & TCS outputs.
          *
          * The GL_OES_tessellation_shader spec says about inputs:
          *
          *    "Declaring an array size is optional. If no size is specified,
          *     it will be taken from the implementation-dependent maximum
          *     patch size (gl_MaxPatchVertices)."
          *
          * and about TCS outputs:
          *
          *    "If no size is specified, it will be taken from output patch
          *     size declared in the shader."
          *
          * The GL_OES_geometry_shader spec says:
          *
          *    "All geometry shader input unsized array declarations will be
          *     sized by an earlier input primitive layout qualifier, when
          *     present, as per the following table."
          */
            const bool implicitly_sized =
                    (var->data.mode == ir_var_shader_in &&
                     state->stage >= MESA_SHADER_TESS_CTRL &&
                     state->stage <= MESA_SHADER_GEOMETRY) ||
                    (var->data.mode == ir_var_shader_out &&
                     state->stage == MESA_SHADER_TESS_CTRL);

            if (t->is_unsized_array() && !implicitly_sized)
                /* Section 10.17 of the GLSL ES 1.00 specification states that
             * unsized array declarations have been removed from the language.
             * Arrays that are sized using an initializer are still explicitly
             * sized.  However, GLSL ES 1.00 does not allow array
             * initializers.  That is only allowed in GLSL ES 3.00.
             *
             * Section 4.1.9 (Arrays) of the GLSL ES 3.00 spec says:
             *
             *     "An array type can also be formed without specifying a size
             *     if the definition includes an initializer:
             *
             *         float x[] = float[2] (1.0, 2.0);     // declares an array of size 2
             *         float y[] = float[] (1.0, 2.0, 3.0); // declares an array of size 3
             *
             *         float a[5];
             *         float b[] = a;"
             */
                _mesa_glsl_error(& loc, state,
                                 "unsized array declarations are not allowed in "
                                 "GLSL ES");
        }

        /* Section 4.4.6.1 Atomic Counter Layout Qualifiers of the GLSL 4.60 spec:
       *
       *    "It is a compile-time error to declare an unsized array of
       *     atomic_uint"
       */
        if (var->type->is_unsized_array() &&
            var->type->without_array()->base_type == GLSL_TYPE_ATOMIC_UINT) {
            _mesa_glsl_error(& loc, state,
                             "Unsized array of atomic_uint is not allowed");
        }

        /* If the declaration is not a redeclaration, there are a few additional
       * semantic checks that must be applied.  In addition, variable that was
       * created for the declaration should be added to the IR stream.
       */
        if (!is_redeclaration) {
            validate_identifier(decl->identifier, loc, state);

            /* Add the variable to the symbol table.  Note that the initializer's
          * IR was already processed earlier (though it hasn't been emitted
          * yet), without the variable in scope.
          *
          * This differs from most C-like languages, but it follows the GLSL
          * specification.  From page 28 (page 34 of the PDF) of the GLSL 1.50
          * spec:
          *
          *     "Within a declaration, the scope of a name starts immediately
          *     after the initializer if present or immediately after the name
          *     being declared if not."
          */
            if (!state->symbols->add_variable(var)) {
                YYLTYPE loc = this->get_location();
                _mesa_glsl_error(&loc, state, "name `%s' already taken in the "
                                              "current scope", decl->identifier);
                continue;
            }

            /* Push the variable declaration to the top.  It means that all the
          * variable declarations will appear in a funny last-to-first order,
          * but otherwise we run into trouble if a function is prototyped, a
          * global var is decled, then the function is defined with usage of
          * the global var.  See glslparsertest's CorrectModule.frag.
          */
            instructions->push_head(var);
        }

        instructions->append_list(&initializer_instructions);
    }


    /* Generally, variable declarations do not have r-values.  However,
    * one is used for the declaration in
    *
    * while (bool b = some_condition()) {
    *   ...
    * }
    *
    * so we return the rvalue from the last seen declaration here.
    */
    return result;
}


ir_rvalue *
ast_parameter_declarator::hir(exec_list *instructions,
                              struct _mesa_glsl_parse_state *state)
{
    void *ctx = state;
    const struct glsl_type *type;
    const char *name = NULL;
    YYLTYPE loc = this->get_location();

    type = this->type->glsl_type(& name, state);

    if (type == NULL) {
        if (name != NULL) {
            _mesa_glsl_error(& loc, state,
                             "invalid type `%s' in declaration of `%s'",
                             name, this->identifier);
        } else {
            _mesa_glsl_error(& loc, state,
                             "invalid type in declaration of `%s'",
                             this->identifier);
        }

        type = glsl_type::error_type;
    }

    /* From page 62 (page 68 of the PDF) of the GLSL 1.50 spec:
    *
    *    "Functions that accept no input arguments need not use void in the
    *    argument list because prototypes (or definitions) are required and
    *    therefore there is no ambiguity when an empty argument list "( )" is
    *    declared. The idiom "(void)" as a parameter list is provided for
    *    convenience."
    *
    * Placing this check here prevents a void parameter being set up
    * for a function, which avoids tripping up checks for main taking
    * parameters and lookups of an unnamed symbol.
    */
    if (type->is_void()) {
        if (this->identifier != NULL)
            _mesa_glsl_error(& loc, state,
                             "named parameter cannot have type `void'");

        is_void = true;
        return NULL;
    }

    if (formal_parameter && (this->identifier == NULL)) {
        _mesa_glsl_error(& loc, state, "formal parameter lacks a name");
        return NULL;
    }

    /* This only handles "vec4 foo[..]".  The earlier specifier->glsl_type(...)
    * call already handled the "vec4[..] foo" case.
    */
    type = process_array_type(&loc, type, this->array_specifier, state);

    if (!type->is_error() && type->is_unsized_array()) {
        _mesa_glsl_error(&loc, state, "arrays passed as parameters must have "
                                      "a declared size");
        type = glsl_type::error_type;
    }

    is_void = false;
    ir_variable *var = new(ctx)
            ir_variable(type, this->identifier, ir_var_function_in);

    /* Apply any specified qualifiers to the parameter declaration.  Note that
    * for function parameters the default mode is 'in'.
    */
    apply_type_qualifier_to_variable(& this->type->qualifier, var, state, & loc,
                                     true);

    if (((1u << var->data.mode) & state->zero_init) &&
        (var->type->is_numeric() || var->type->is_boolean())) {
        const ir_constant_data data = { { 0 } };
        var->data.has_initializer = true;
        var->data.is_implicit_initializer = true;
        var->constant_initializer = new(var) ir_constant(var->type, &data);
    }

    /* From section 4.1.7 of the GLSL 4.40 spec:
    *
    *   "Opaque variables cannot be treated as l-values; hence cannot
    *    be used as out or inout function parameters, nor can they be
    *    assigned into."
    *
    * From section 4.1.7 of the ARB_bindless_texture spec:
    *
    *   "Samplers can be used as l-values, so can be assigned into and used
    *    as "out" and "inout" function parameters."
    *
    * From section 4.1.X of the ARB_bindless_texture spec:
    *
    *   "Images can be used as l-values, so can be assigned into and used as
    *    "out" and "inout" function parameters."
    */
    if ((var->data.mode == ir_var_function_inout || var->data.mode == ir_var_function_out)
        && (type->contains_atomic() ||
            (!state->has_bindless() && type->contains_opaque()))) {
        _mesa_glsl_error(&loc, state, "out and inout parameters cannot "
                                      "contain %s variables",
                         state->has_bindless() ? "atomic" : "opaque");
        type = glsl_type::error_type;
    }

    /* From page 39 (page 45 of the PDF) of the GLSL 1.10 spec:
    *
    *    "When calling a function, expressions that do not evaluate to
    *     l-values cannot be passed to parameters declared as out or inout."
    *
    * From page 32 (page 38 of the PDF) of the GLSL 1.10 spec:
    *
    *    "Other binary or unary expressions, non-dereferenced arrays,
    *     function names, swizzles with repeated fields, and constants
    *     cannot be l-values."
    *
    * So for GLSL 1.10, passing an array as an out or inout parameter is not
    * allowed.  This restriction is removed in GLSL 1.20, and in GLSL ES.
    */
    if ((var->data.mode == ir_var_function_inout || var->data.mode == ir_var_function_out)
        && type->is_array()
        && !state->check_version(120, 100, &loc,
                                 "arrays cannot be out or inout parameters")) {
        type = glsl_type::error_type;
    }

    instructions->push_tail(var);

    /* Parameter declarations do not have r-values.
    */
    return NULL;
}


void
ast_parameter_declarator::parameters_to_hir(exec_list *ast_parameters,
                                            bool formal,
                                            exec_list *ir_parameters,
                                            _mesa_glsl_parse_state *state)
{
    ast_parameter_declarator *void_param = NULL;
    unsigned count = 0;

    foreach_list_typed (ast_parameter_declarator, param, link, ast_parameters) {
        param->formal_parameter = formal;
        param->hir(ir_parameters, state);

        if (param->is_void)
            void_param = param;

        count++;
    }

    if ((void_param != NULL) && (count > 1)) {
        YYLTYPE loc = void_param->get_location();

        _mesa_glsl_error(& loc, state,
                         "`void' parameter must be only parameter");
    }
}


void
emit_function(_mesa_glsl_parse_state *state, ir_function *f)
{
    /* IR invariants disallow function declarations or definitions
    * nested within other function definitions.  But there is no
    * requirement about the relative order of function declarations
    * and definitions with respect to one another.  So simply insert
    * the new ir_function block at the end of the toplevel instruction
    * list.
    */
    state->toplevel_ir->push_tail(f);
}


ir_rvalue *
ast_function::hir(exec_list *instructions,
                  struct _mesa_glsl_parse_state *state)
{
    void *ctx = state;
    ir_function *f = NULL;
    ir_function_signature *sig = NULL;
    exec_list hir_parameters;
    YYLTYPE loc = this->get_location();

    const char *const name = identifier;

    /* New functions are always added to the top-level IR instruction stream,
    * so this instruction list pointer is ignored.  See also emit_function
    * (called below).
    */
    (void) instructions;

    /* From page 21 (page 27 of the PDF) of the GLSL 1.20 spec,
    *
    *   "Function declarations (prototypes) cannot occur inside of functions;
    *   they must be at global scope, or for the built-in functions, outside
    *   the global scope."
    *
    * From page 27 (page 33 of the PDF) of the GLSL ES 1.00.16 spec,
    *
    *   "User defined functions may only be defined within the global scope."
    *
    * Note that this language does not appear in GLSL 1.10.
    */
    if ((state->current_function != NULL) &&
        state->is_version(120, 100)) {
        YYLTYPE loc = this->get_location();
        _mesa_glsl_error(&loc, state,
                         "declaration of function `%s' not allowed within "
                         "function body", name);
    }

    validate_identifier(name, this->get_location(), state);

    /* Convert the list of function parameters to HIR now so that they can be
    * used below to compare this function's signature with previously seen
    * signatures for functions with the same name.
    */
    ast_parameter_declarator::parameters_to_hir(& this->parameters,
                                                is_definition,
                                                & hir_parameters, state);

    const char *return_type_name;
    const glsl_type *return_type =
            this->return_type->glsl_type(& return_type_name, state);

    if (!return_type) {
        YYLTYPE loc = this->get_location();
        _mesa_glsl_error(&loc, state,
                         "function `%s' has undeclared return type `%s'",
                         name, return_type_name);
        return_type = glsl_type::error_type;
    }

    /* ARB_shader_subroutine states:
    *  "Subroutine declarations cannot be prototyped. It is an error to prepend
    *   subroutine(...) to a function declaration."
    */
    if (this->return_type->qualifier.subroutine_list && !is_definition) {
        YYLTYPE loc = this->get_location();
        _mesa_glsl_error(&loc, state,
                         "function declaration `%s' cannot have subroutine prepended",
                         name);
    }

    /* From page 56 (page 62 of the PDF) of the GLSL 1.30 spec:
    * "No qualifier is allowed on the return type of a function."
    */
    if (this->return_type->has_qualifiers(state)) {
        YYLTYPE loc = this->get_location();
        _mesa_glsl_error(& loc, state,
                         "function `%s' return type has qualifiers", name);
    }

    /* Section 6.1 (Function Definitions) of the GLSL 1.20 spec says:
    *
    *     "Arrays are allowed as arguments and as the return type. In both
    *     cases, the array must be explicitly sized."
    */
    if (return_type->is_unsized_array()) {
        YYLTYPE loc = this->get_location();
        _mesa_glsl_error(& loc, state,
                         "function `%s' return type array must be explicitly "
                         "sized", name);
    }

    /* From Section 6.1 (Function Definitions) of the GLSL 1.00 spec:
    *
    *     "Arrays are allowed as arguments, but not as the return type. [...]
    *      The return type can also be a structure if the structure does not
    *      contain an array."
    */
    if (state->language_version == 100 && return_type->contains_array()) {
        YYLTYPE loc = this->get_location();
        _mesa_glsl_error(& loc, state,
                         "function `%s' return type contains an array", name);
    }

    /* From section 4.1.7 of the GLSL 4.40 spec:
    *
    *    "[Opaque types] can only be declared as function parameters
    *     or uniform-qualified variables."
    *
    * The ARB_bindless_texture spec doesn't clearly state this, but as it says
    * "Replace Section 4.1.7 (Samplers), p. 25" and, "Replace Section 4.1.X,
    * (Images)", this should be allowed.
    */
    if (return_type->contains_atomic() ||
        (!state->has_bindless() && return_type->contains_opaque())) {
        YYLTYPE loc = this->get_location();
        _mesa_glsl_error(&loc, state,
                         "function `%s' return type can't contain an %s type",
                         name, state->has_bindless() ? "atomic" : "opaque");
    }

    /**/
    if (return_type->is_subroutine()) {
        YYLTYPE loc = this->get_location();
        _mesa_glsl_error(&loc, state,
                         "function `%s' return type can't be a subroutine type",
                         name);
    }

    /* Get the precision for the return type */
    unsigned return_precision;

    if (state->es_shader) {
        YYLTYPE loc = this->get_location();
        return_precision =
                select_gles_precision(this->return_type->qualifier.precision,
                                      return_type,
                                      state,
                                      &loc);
    } else {
        return_precision = GLSL_PRECISION_NONE;
    }

    /* Create an ir_function if one doesn't already exist. */
    f = state->symbols->get_function(name);
    if (f == NULL) {
        f = new(ctx) ir_function(name);
        if (!this->return_type->qualifier.is_subroutine_decl()) {
            if (!state->symbols->add_function(f)) {
                /* This function name shadows a non-function use of the same name. */
                YYLTYPE loc = this->get_location();
                _mesa_glsl_error(&loc, state, "function name `%s' conflicts with "
                                              "non-function", name);
                return NULL;
            }
        }
        emit_function(state, f);
    }

    /* From GLSL ES 3.0 spec, chapter 6.1 "Function Definitions", page 71:
    *
    * "A shader cannot redefine or overload built-in functions."
    *
    * While in GLSL ES 1.0 specification, chapter 8 "Built-in Functions":
    *
    * "User code can overload the built-in functions but cannot redefine
    * them."
    */
    if (state->es_shader) {
        /* Local shader has no exact candidates; check the built-ins. */
        if (state->language_version >= 300 &&
            _mesa_glsl_has_builtin_function(state, name)) {
            YYLTYPE loc = this->get_location();
            _mesa_glsl_error(& loc, state,
                             "A shader cannot redefine or overload built-in "
                             "function `%s' in GLSL ES 3.00", name);
            return NULL;
        }

        if (state->language_version == 100) {
            ir_function_signature *sig =
                    _mesa_glsl_find_builtin_function(state, name, &hir_parameters);
            if (sig && sig->is_builtin()) {
                _mesa_glsl_error(& loc, state,
                                 "A shader cannot redefine built-in "
                                 "function `%s' in GLSL ES 1.00", name);
            }
        }
    }

    /* Verify that this function's signature either doesn't match a previously
    * seen signature for a function with the same name, or, if a match is found,
    * that the previously seen signature does not have an associated definition.
    */
    if (state->es_shader || f->has_user_signature()) {
        sig = f->exact_matching_signature(state, &hir_parameters);
        if (sig != NULL) {
            const char *badvar = sig->qualifiers_match(&hir_parameters);
            if (badvar != NULL) {
                YYLTYPE loc = this->get_location();

                _mesa_glsl_error(&loc, state, "function `%s' parameter `%s' "
                                              "qualifiers don't match prototype", name, badvar);
            }

            if (sig->return_type != return_type) {
                YYLTYPE loc = this->get_location();

                _mesa_glsl_error(&loc, state, "function `%s' return type doesn't "
                                              "match prototype", name);
            }

            if (sig->return_precision != return_precision) {
                YYLTYPE loc = this->get_location();

                _mesa_glsl_error(&loc, state, "function `%s' return type precision "
                                              "doesn't match prototype", name);
            }

            if (sig->is_defined) {
                if (is_definition) {
                    YYLTYPE loc = this->get_location();
                    _mesa_glsl_error(& loc, state, "function `%s' redefined", name);
                } else {
                    /* We just encountered a prototype that exactly matches a
                * function that's already been defined.  This is redundant,
                * and we should ignore it.
                */
                    return NULL;
                }
            } else if (state->language_version == 100 && !is_definition) {
                /* From the GLSL 1.00 spec, section 4.2.7:
             *
             *     "A particular variable, structure or function declaration
             *      may occur at most once within a scope with the exception
             *      that a single function prototype plus the corresponding
             *      function definition are allowed."
             */
                YYLTYPE loc = this->get_location();
                _mesa_glsl_error(&loc, state, "function `%s' redeclared", name);
            }
        }
    }

    /* Verify the return type of main() */
    if (strcmp(name, "main") == 0) {
        if (! return_type->is_void()) {
            YYLTYPE loc = this->get_location();

            _mesa_glsl_error(& loc, state, "main() must return void");
        }

        if (!hir_parameters.is_empty()) {
            YYLTYPE loc = this->get_location();

            _mesa_glsl_error(& loc, state, "main() must not take any parameters");
        }
    }

    /* Finish storing the information about this new function in its signature.
    */
    if (sig == NULL) {
        sig = new(ctx) ir_function_signature(return_type);
        sig->return_precision = return_precision;
        f->add_signature(sig);
    }

    sig->replace_parameters(&hir_parameters);
    signature = sig;

    if (this->return_type->qualifier.subroutine_list) {
        int idx;

        if (this->return_type->qualifier.flags.q.explicit_index) {
            unsigned qual_index;
            if (process_qualifier_constant(state, &loc, "index",
                                           this->return_type->qualifier.index,
                                           &qual_index)) {
                if (!state->has_explicit_uniform_location()) {
                    _mesa_glsl_error(&loc, state, "subroutine index requires "
                                                  "GL_ARB_explicit_uniform_location or "
                                                  "GLSL 4.30");
                } else if (qual_index >= MAX_SUBROUTINES) {
                    _mesa_glsl_error(&loc, state,
                                     "invalid subroutine index (%d) index must "
                                     "be a number between 0 and "
                                     "GL_MAX_SUBROUTINES - 1 (%d)", qual_index,
                                     MAX_SUBROUTINES - 1);
                } else {
                    f->subroutine_index = qual_index;
                }
            }
        }

        f->num_subroutine_types = this->return_type->qualifier.subroutine_list->declarations.length();
        f->subroutine_types = ralloc_array(state, const struct glsl_type *,
                                           f->num_subroutine_types);
        idx = 0;
        foreach_list_typed(ast_declaration, decl, link, &this->return_type->qualifier.subroutine_list->declarations) {
            const struct glsl_type *type;
            /* the subroutine type must be already declared */
            type = state->symbols->get_type(decl->identifier);
            if (!type) {
                _mesa_glsl_error(& loc, state, "unknown type '%s' in subroutine function definition", decl->identifier);
            }

            for (int i = 0; i < state->num_subroutine_types; i++) {
                ir_function *fn = state->subroutine_types[i];
                ir_function_signature *tsig = NULL;

                if (strcmp(fn->name, decl->identifier))
                    continue;

                tsig = fn->matching_signature(state, &sig->parameters,
                                              false);
                if (!tsig) {
                    _mesa_glsl_error(& loc, state, "subroutine type mismatch '%s' - signatures do not match\n", decl->identifier);
                } else {
                    if (tsig->return_type != sig->return_type) {
                        _mesa_glsl_error(& loc, state, "subroutine type mismatch '%s' - return types do not match\n", decl->identifier);
                    }
                }
            }
            f->subroutine_types[idx++] = type;
        }
        state->subroutines = (ir_function **)reralloc(state, state->subroutines,
                                                      ir_function *,
                                                      state->num_subroutines + 1);
        state->subroutines[state->num_subroutines] = f;
        state->num_subroutines++;

    }

    if (this->return_type->qualifier.is_subroutine_decl()) {
        if (!state->symbols->add_type(this->identifier, glsl_type::get_subroutine_instance(this->identifier))) {
            _mesa_glsl_error(& loc, state, "type '%s' previously defined", this->identifier);
            return NULL;
        }
        state->subroutine_types = (ir_function **)reralloc(state, state->subroutine_types,
                                                           ir_function *,
                                                           state->num_subroutine_types + 1);
        state->subroutine_types[state->num_subroutine_types] = f;
        state->num_subroutine_types++;

        f->is_subroutine = true;
    }

    /* Function declarations (prototypes) do not have r-values.
    */
    return NULL;
}


ir_rvalue *
ast_function_definition::hir(exec_list *instructions,
                             struct _mesa_glsl_parse_state *state)
{
    prototype->is_definition = true;
    prototype->hir(instructions, state);

    ir_function_signature *signature = prototype->signature;
    if (signature == NULL)
        return NULL;

    assert(state->current_function == NULL);
    state->current_function = signature;
    state->found_return = false;
    state->found_begin_interlock = false;
    state->found_end_interlock = false;

    /* Duplicate parameters declared in the prototype as concrete variables.
    * Add these to the symbol table.
    */
    state->symbols->push_scope();
    foreach_in_list(ir_variable, var, &signature->parameters) {
        assert(var->as_variable() != NULL);

        /* The only way a parameter would "exist" is if two parameters have
       * the same name.
       */
        if (state->symbols->name_declared_this_scope(var->name)) {
            YYLTYPE loc = this->get_location();

            _mesa_glsl_error(& loc, state, "parameter `%s' redeclared", var->name);
        } else {
            state->symbols->add_variable(var);
        }
    }

    /* Convert the body of the function to HIR. */
    this->body->hir(&signature->body, state);
    signature->is_defined = true;

    state->symbols->pop_scope();

    assert(state->current_function == signature);
    state->current_function = NULL;

    if (!signature->return_type->is_void() && !state->found_return) {
        YYLTYPE loc = this->get_location();
        _mesa_glsl_error(& loc, state, "function `%s' has non-void return type "
                                       "%s, but no return statement",
                         signature->function_name(),
                         signature->return_type->name);
    }

    /* Function definitions do not have r-values.
    */
    return NULL;
}


ir_rvalue *
ast_jump_statement::hir(exec_list *instructions,
                        struct _mesa_glsl_parse_state *state)
{
    void *ctx = state;

    switch (mode) {
        case ast_return: {
            ir_return *inst;
            assert(state->current_function);

            if (opt_return_value) {
                ir_rvalue *ret = opt_return_value->hir(instructions, state);

                /* The value of the return type can be NULL if the shader says
          * 'return foo();' and foo() is a function that returns void.
          *
          * NOTE: The GLSL spec doesn't say that this is an error.  The type
          * of the return value is void.  If the return type of the function is
          * also void, then this should compile without error.  Seriously.
          */
                const glsl_type *const ret_type =
                        (ret == NULL) ? glsl_type::void_type : ret->type;

                /* Implicit conversions are not allowed for return values prior to
          * ARB_shading_language_420pack.
          */
                if (state->current_function->return_type != ret_type) {
                    YYLTYPE loc = this->get_location();

                    if (state->has_420pack()) {
                        if (!apply_implicit_conversion(state->current_function->return_type,
                                                       ret, state)
                            || (ret->type != state->current_function->return_type)) {
                            _mesa_glsl_error(& loc, state,
                                             "could not implicitly convert return value "
                                             "to %s, in function `%s'",
                                             state->current_function->return_type->name,
                                             state->current_function->function_name());
                        }
                    } else {
                        _mesa_glsl_error(& loc, state,
                                         "`return' with wrong type %s, in function `%s' "
                                         "returning %s",
                                         ret_type->name,
                                         state->current_function->function_name(),
                                         state->current_function->return_type->name);
                    }
                } else if (state->current_function->return_type->base_type ==
                           GLSL_TYPE_VOID) {
                    YYLTYPE loc = this->get_location();

                    /* The ARB_shading_language_420pack, GLSL ES 3.0, and GLSL 4.20
             * specs add a clarification:
             *
             *    "A void function can only use return without a return argument, even if
             *     the return argument has void type. Return statements only accept values:
             *
             *         void func1() { }
             *         void func2() { return func1(); } // illegal return statement"
             */
                    _mesa_glsl_error(& loc, state,
                                     "void functions can only use `return' without a "
                                     "return argument");
                }

                inst = new(ctx) ir_return(ret);
            } else {
                if (state->current_function->return_type->base_type !=
                    GLSL_TYPE_VOID) {
                    YYLTYPE loc = this->get_location();

                    _mesa_glsl_error(& loc, state,
                                     "`return' with no value, in function %s returning "
                                     "non-void",
                                     state->current_function->function_name());
                }
                inst = new(ctx) ir_return;
            }

            state->found_return = true;
            instructions->push_tail(inst);
            break;
        }

        case ast_discard:
            if (state->stage != MESA_SHADER_FRAGMENT) {
                YYLTYPE loc = this->get_location();

                _mesa_glsl_error(& loc, state,
                                 "`discard' may only appear in a fragment shader");
            }
            instructions->push_tail(new(ctx) ir_discard);
            break;

        case ast_break:
        case ast_continue:
            if (mode == ast_continue &&
                state->loop_nesting_ast == NULL) {
                YYLTYPE loc = this->get_location();

                _mesa_glsl_error(& loc, state, "continue may only appear in a loop");
            } else if (mode == ast_break &&
                       state->loop_nesting_ast == NULL &&
                       state->switch_state.switch_nesting_ast == NULL) {
                YYLTYPE loc = this->get_location();

                _mesa_glsl_error(& loc, state,
                                 "break may only appear in a loop or a switch");
            } else {
                /* For a loop, inline the for loop expression again, since we don't
          * know where near the end of the loop body the normal copy of it is
          * going to be placed.  Same goes for the condition for a do-while
          * loop.
          */
                if (state->loop_nesting_ast != NULL &&
                    mode == ast_continue && !state->switch_state.is_switch_innermost) {
                    if (state->loop_nesting_ast->rest_expression) {
                        state->loop_nesting_ast->rest_expression->hir(instructions,
                                                                      state);
                    }
                    if (state->loop_nesting_ast->mode ==
                        ast_iteration_statement::ast_do_while) {
                        state->loop_nesting_ast->condition_to_hir(instructions, state);
                    }
                }

                if (state->switch_state.is_switch_innermost &&
                    mode == ast_continue) {
                    /* Set 'continue_inside' to true. */
                    ir_rvalue *const true_val = new (ctx) ir_constant(true);
                    ir_dereference_variable *deref_continue_inside_var =
                            new(ctx) ir_dereference_variable(state->switch_state.continue_inside);
                    instructions->push_tail(new(ctx) ir_assignment(deref_continue_inside_var,
                                                                   true_val));

                    /* Break out from the switch, continue for the loop will
             * be called right after switch. */
                    ir_loop_jump *const jump =
                            new(ctx) ir_loop_jump(ir_loop_jump::jump_break);
                    instructions->push_tail(jump);

                } else if (state->switch_state.is_switch_innermost &&
                           mode == ast_break) {
                    /* Force break out of switch by inserting a break. */
                    ir_loop_jump *const jump =
                            new(ctx) ir_loop_jump(ir_loop_jump::jump_break);
                    instructions->push_tail(jump);
                } else {
                    ir_loop_jump *const jump =
                            new(ctx) ir_loop_jump((mode == ast_break)
                                                  ? ir_loop_jump::jump_break
                                                  : ir_loop_jump::jump_continue);
                    instructions->push_tail(jump);
                }
            }

            break;
    }

    /* Jump instructions do not have r-values.
    */
    return NULL;
}


ir_rvalue *
ast_demote_statement::hir(exec_list *instructions,
                          struct _mesa_glsl_parse_state *state)
{
    void *ctx = state;

    if (state->stage != MESA_SHADER_FRAGMENT) {
        YYLTYPE loc = this->get_location();

        _mesa_glsl_error(& loc, state,
                         "`demote' may only appear in a fragment shader");
    }

    instructions->push_tail(new(ctx) ir_demote);

    return NULL;
}


ir_rvalue *
ast_selection_statement::hir(exec_list *instructions,
                             struct _mesa_glsl_parse_state *state)
{
    void *ctx = state;

    ir_rvalue *const condition = this->condition->hir(instructions, state);

    /* From page 66 (page 72 of the PDF) of the GLSL 1.50 spec:
    *
    *    "Any expression whose type evaluates to a Boolean can be used as the
    *    conditional expression bool-expression. Vector types are not accepted
    *    as the expression to if."
    *
    * The checks are separated so that higher quality diagnostics can be
    * generated for cases where both rules are violated.
    */
    if (!condition->type->is_boolean() || !condition->type->is_scalar()) {
        YYLTYPE loc = this->condition->get_location();

        _mesa_glsl_error(& loc, state, "if-statement condition must be scalar "
                                       "boolean");
    }

    ir_if *const stmt = new(ctx) ir_if(condition);

    if (then_statement != NULL) {
        state->symbols->push_scope();
        then_statement->hir(& stmt->then_instructions, state);
        state->symbols->pop_scope();
    }

    if (else_statement != NULL) {
        state->symbols->push_scope();
        else_statement->hir(& stmt->else_instructions, state);
        state->symbols->pop_scope();
    }

    instructions->push_tail(stmt);

    /* if-statements do not have r-values.
    */
    return NULL;
}


struct case_label {
    /** Value of the case label. */
    unsigned value;

    /** Does this label occur after the default? */
    bool after_default;

    /**
    * AST for the case label.
    *
    * This is only used to generate error messages for duplicate labels.
    */
    ast_expression *ast;
};

/* Used for detection of duplicate case values, compare
 * given contents directly.
 */
static bool
compare_case_value(const void *a, const void *b)
{
    return ((struct case_label *) a)->value == ((struct case_label *) b)->value;
}


/* Used for detection of duplicate case values, just
 * returns key contents as is.
 */
static unsigned
key_contents(const void *key)
{
    return ((struct case_label *) key)->value;
}


ir_rvalue *
ast_switch_statement::hir(exec_list *instructions,
                          struct _mesa_glsl_parse_state *state)
{
    void *ctx = state;

    ir_rvalue *const test_expression =
            this->test_expression->hir(instructions, state);

    /* From page 66 (page 55 of the PDF) of the GLSL 1.50 spec:
    *
    *    "The type of init-expression in a switch statement must be a
    *     scalar integer."
    */
    if (!test_expression->type->is_scalar() ||
        !test_expression->type->is_integer_32()) {
        YYLTYPE loc = this->test_expression->get_location();

        _mesa_glsl_error(& loc,
                         state,
                         "switch-statement expression must be scalar "
                         "integer");
        return NULL;
    }

    /* Track the switch-statement nesting in a stack-like manner.
    */
    struct glsl_switch_state saved = state->switch_state;

    state->switch_state.is_switch_innermost = true;
    state->switch_state.switch_nesting_ast = this;
    state->switch_state.labels_ht =
            _mesa_hash_table_create(NULL, key_contents,
                                    compare_case_value);
    state->switch_state.previous_default = NULL;

    /* Initalize is_fallthru state to false.
    */
    ir_rvalue *const is_fallthru_val = new (ctx) ir_constant(false);
    state->switch_state.is_fallthru_var =
            new(ctx) ir_variable(glsl_type::bool_type,
                                 "switch_is_fallthru_tmp",
                                 ir_var_temporary);
    instructions->push_tail(state->switch_state.is_fallthru_var);

    ir_dereference_variable *deref_is_fallthru_var =
            new(ctx) ir_dereference_variable(state->switch_state.is_fallthru_var);
    instructions->push_tail(new(ctx) ir_assignment(deref_is_fallthru_var,
                                                   is_fallthru_val));

    /* Initialize continue_inside state to false.
    */
    state->switch_state.continue_inside =
            new(ctx) ir_variable(glsl_type::bool_type,
                                 "continue_inside_tmp",
                                 ir_var_temporary);
    instructions->push_tail(state->switch_state.continue_inside);

    ir_rvalue *const false_val = new (ctx) ir_constant(false);
    ir_dereference_variable *deref_continue_inside_var =
            new(ctx) ir_dereference_variable(state->switch_state.continue_inside);
    instructions->push_tail(new(ctx) ir_assignment(deref_continue_inside_var,
                                                   false_val));

    state->switch_state.run_default =
            new(ctx) ir_variable(glsl_type::bool_type,
                                 "run_default_tmp",
                                 ir_var_temporary);
    instructions->push_tail(state->switch_state.run_default);

    /* Loop around the switch is used for flow control. */
    ir_loop * loop = new(ctx) ir_loop();
    instructions->push_tail(loop);

    /* Cache test expression.
    */
    test_to_hir(&loop->body_instructions, state);

    /* Emit code for body of switch stmt.
    */
    body->hir(&loop->body_instructions, state);

    /* Insert a break at the end to exit loop. */
    ir_loop_jump *jump = new(ctx) ir_loop_jump(ir_loop_jump::jump_break);
    loop->body_instructions.push_tail(jump);

    /* If we are inside loop, check if continue got called inside switch. */
    if (state->loop_nesting_ast != NULL) {
        ir_dereference_variable *deref_continue_inside =
                new(ctx) ir_dereference_variable(state->switch_state.continue_inside);
        ir_if *irif = new(ctx) ir_if(deref_continue_inside);
        ir_loop_jump *jump = new(ctx) ir_loop_jump(ir_loop_jump::jump_continue);

        if (state->loop_nesting_ast != NULL) {
            if (state->loop_nesting_ast->rest_expression) {
                state->loop_nesting_ast->rest_expression->hir(&irif->then_instructions,
                                                              state);
            }
            if (state->loop_nesting_ast->mode ==
                ast_iteration_statement::ast_do_while) {
                state->loop_nesting_ast->condition_to_hir(&irif->then_instructions, state);
            }
        }
        irif->then_instructions.push_tail(jump);
        instructions->push_tail(irif);
    }

    _mesa_hash_table_destroy(state->switch_state.labels_ht, NULL);

    state->switch_state = saved;

    /* Switch statements do not have r-values. */
    return NULL;
}


void
ast_switch_statement::test_to_hir(exec_list *instructions,
                                  struct _mesa_glsl_parse_state *state)
{
    void *ctx = state;

    /* set to true to avoid a duplicate "use of uninitialized variable" warning
    * on the switch test case. The first one would be already raised when
    * getting the test_expression at ast_switch_statement::hir
    */
    test_expression->set_is_lhs(true);
    /* Cache value of test expression. */
    ir_rvalue *const test_val = test_expression->hir(instructions, state);

    state->switch_state.test_var = new(ctx) ir_variable(test_val->type,
                                                        "switch_test_tmp",
                                                        ir_var_temporary);
    ir_dereference_variable *deref_test_var =
            new(ctx) ir_dereference_variable(state->switch_state.test_var);

    instructions->push_tail(state->switch_state.test_var);
    instructions->push_tail(new(ctx) ir_assignment(deref_test_var, test_val));
}


ir_rvalue *
ast_switch_body::hir(exec_list *instructions,
                     struct _mesa_glsl_parse_state *state)
{
    if (stmts != NULL)
        stmts->hir(instructions, state);

    /* Switch bodies do not have r-values. */
    return NULL;
}

ir_rvalue *
ast_case_statement_list::hir(exec_list *instructions,
                             struct _mesa_glsl_parse_state *state)
{
    exec_list default_case, after_default, tmp;

    foreach_list_typed (ast_case_statement, case_stmt, link, & this->cases) {
        case_stmt->hir(&tmp, state);

        /* Default case. */
        if (state->switch_state.previous_default && default_case.is_empty()) {
            default_case.append_list(&tmp);
            continue;
        }

        /* If default case found, append 'after_default' list. */
        if (!default_case.is_empty())
            after_default.append_list(&tmp);
        else
            instructions->append_list(&tmp);
    }

    /* Handle the default case. This is done here because default might not be
    * the last case. We need to add checks against following cases first to see
    * if default should be chosen or not.
    */
    if (!default_case.is_empty()) {
        ir_factory body(instructions, state);

        ir_expression *cmp = NULL;

        hash_table_foreach(state->switch_state.labels_ht, entry) {
            const struct case_label *const l = (struct case_label *) entry->data;

            /* If the switch init-value is the value of one of the labels that
          * occurs after the default case, disable execution of the default
          * case.
          */
            if (l->after_default) {
                ir_constant *const cnst =
                        state->switch_state.test_var->type->base_type == GLSL_TYPE_UINT
                        ? body.constant(unsigned(l->value))
                        : body.constant(int(l->value));

                cmp = cmp == NULL
                      ? equal(cnst, state->switch_state.test_var)
                      : logic_or(cmp, equal(cnst, state->switch_state.test_var));
            }
        }

        if (cmp != NULL)
            body.emit(assign(state->switch_state.run_default, logic_not(cmp)));
        else
            body.emit(assign(state->switch_state.run_default, body.constant(true)));

        /* Append default case and all cases after it. */
        instructions->append_list(&default_case);
        instructions->append_list(&after_default);
    }

    /* Case statements do not have r-values. */
    return NULL;
}

ir_rvalue *
ast_case_statement::hir(exec_list *instructions,
                        struct _mesa_glsl_parse_state *state)
{
    labels->hir(instructions, state);

    /* Guard case statements depending on fallthru state. */
    ir_dereference_variable *const deref_fallthru_guard =
            new(state) ir_dereference_variable(state->switch_state.is_fallthru_var);
    ir_if *const test_fallthru = new(state) ir_if(deref_fallthru_guard);

    foreach_list_typed (ast_node, stmt, link, & this->stmts)
        stmt->hir(& test_fallthru->then_instructions, state);

    instructions->push_tail(test_fallthru);

    /* Case statements do not have r-values. */
    return NULL;
}


ir_rvalue *
ast_case_label_list::hir(exec_list *instructions,
                         struct _mesa_glsl_parse_state *state)
{
    foreach_list_typed (ast_case_label, label, link, & this->labels)
        label->hir(instructions, state);

    /* Case labels do not have r-values. */
    return NULL;
}

ir_rvalue *
ast_case_label::hir(exec_list *instructions,
                    struct _mesa_glsl_parse_state *state)
{
    ir_factory body(instructions, state);

    ir_variable *const fallthru_var = state->switch_state.is_fallthru_var;

    /* If not default case, ... */
    if (this->test_value != NULL) {
        /* Conditionally set fallthru state based on
       * comparison of cached test expression value to case label.
       */
        ir_rvalue *const label_rval = this->test_value->hir(instructions, state);
        ir_constant *label_const =
                label_rval->constant_expression_value(body.mem_ctx);

        if (!label_const) {
            YYLTYPE loc = this->test_value->get_location();

            _mesa_glsl_error(& loc, state,
                             "switch statement case label must be a "
                             "constant expression");

            /* Stuff a dummy value in to allow processing to continue. */
            label_const = body.constant(0);
        } else {
            hash_entry *entry =
                    _mesa_hash_table_search(state->switch_state.labels_ht,
                                            &label_const->value.u[0]);

            if (entry) {
                const struct case_label *const l =
                        (struct case_label *) entry->data;
                const ast_expression *const previous_label = l->ast;
                YYLTYPE loc = this->test_value->get_location();

                _mesa_glsl_error(& loc, state, "duplicate case value");

                loc = previous_label->get_location();
                _mesa_glsl_error(& loc, state, "this is the previous case label");
            } else {
                struct case_label *l = ralloc(state->switch_state.labels_ht,
                                              struct case_label);

                l->value = label_const->value.u[0];
                l->after_default = state->switch_state.previous_default != NULL;
                l->ast = this->test_value;

                _mesa_hash_table_insert(state->switch_state.labels_ht,
                                        &label_const->value.u[0],
                                        l);
            }
        }

        /* Create an r-value version of the ir_constant label here (after we may
       * have created a fake one in error cases) that can be passed to
       * apply_implicit_conversion below.
       */
        ir_rvalue *label = label_const;

        ir_rvalue *deref_test_var =
                new(body.mem_ctx) ir_dereference_variable(state->switch_state.test_var);

        /*
       * From GLSL 4.40 specification section 6.2 ("Selection"):
       *
       *     "The type of the init-expression value in a switch statement must
       *     be a scalar int or uint. The type of the constant-expression value
       *     in a case label also must be a scalar int or uint. When any pair
       *     of these values is tested for "equal value" and the types do not
       *     match, an implicit conversion will be done to convert the int to a
       *     uint (see section 4.1.10 “Implicit Conversions”) before the compare
       *     is done."
       */
        if (label->type != state->switch_state.test_var->type) {
            YYLTYPE loc = this->test_value->get_location();

            const glsl_type *type_a = label->type;
            const glsl_type *type_b = state->switch_state.test_var->type;

            /* Check if int->uint implicit conversion is supported. */
            bool integer_conversion_supported =
                    glsl_type::int_type->can_implicitly_convert_to(glsl_type::uint_type,
                                                                   state);

            if ((!type_a->is_integer_32() || !type_b->is_integer_32()) ||
                !integer_conversion_supported) {
                _mesa_glsl_error(&loc, state, "type mismatch with switch "
                                              "init-expression and case label (%s != %s)",
                                 type_a->name, type_b->name);
            } else {
                /* Conversion of the case label. */
                if (type_a->base_type == GLSL_TYPE_INT) {
                    if (!apply_implicit_conversion(glsl_type::uint_type,
                                                   label, state))
                        _mesa_glsl_error(&loc, state, "implicit type conversion error");
                } else {
                    /* Conversion of the init-expression value. */
                    if (!apply_implicit_conversion(glsl_type::uint_type,
                                                   deref_test_var, state))
                        _mesa_glsl_error(&loc, state, "implicit type conversion error");
                }
            }

            /* If the implicit conversion was allowed, the types will already be
          * the same.  If the implicit conversion wasn't allowed, smash the
          * type of the label anyway.  This will prevent the expression
          * constructor (below) from failing an assertion.
          */
            label->type = deref_test_var->type;
        }

        body.emit(assign(fallthru_var,
                         logic_or(fallthru_var, equal(label, deref_test_var))));
    } else { /* default case */
        if (state->switch_state.previous_default) {
            YYLTYPE loc = this->get_location();
            _mesa_glsl_error(& loc, state,
                             "multiple default labels in one switch");

            loc = state->switch_state.previous_default->get_location();
            _mesa_glsl_error(& loc, state, "this is the first default label");
        }
        state->switch_state.previous_default = this;

        /* Set fallthru condition on 'run_default' bool. */
        body.emit(assign(fallthru_var,
                         logic_or(fallthru_var,
                                  state->switch_state.run_default)));
    }

    /* Case statements do not have r-values. */
    return NULL;
}

void
ast_iteration_statement::condition_to_hir(exec_list *instructions,
                                          struct _mesa_glsl_parse_state *state)
{
    void *ctx = state;

    if (condition != NULL) {
        ir_rvalue *const cond =
                condition->hir(instructions, state);

        if ((cond == NULL)
            || !cond->type->is_boolean() || !cond->type->is_scalar()) {
            YYLTYPE loc = condition->get_location();

            _mesa_glsl_error(& loc, state,
                             "loop condition must be scalar boolean");
        } else {
            /* As the first code in the loop body, generate a block that looks
          * like 'if (!condition) break;' as the loop termination condition.
          */
            ir_rvalue *const not_cond =
                    new(ctx) ir_expression(ir_unop_logic_not, cond);

            ir_if *const if_stmt = new(ctx) ir_if(not_cond);

            ir_jump *const break_stmt =
                    new(ctx) ir_loop_jump(ir_loop_jump::jump_break);

            if_stmt->then_instructions.push_tail(break_stmt);
            instructions->push_tail(if_stmt);
        }
    }
}


ir_rvalue *
ast_iteration_statement::hir(exec_list *instructions,
                             struct _mesa_glsl_parse_state *state)
{
    void *ctx = state;

    /* For-loops and while-loops start a new scope, but do-while loops do not.
    */
    if (mode != ast_do_while)
        state->symbols->push_scope();

    if (init_statement != NULL)
        init_statement->hir(instructions, state);

    ir_loop *const stmt = new(ctx) ir_loop();
    instructions->push_tail(stmt);

    /* Track the current loop nesting. */
    ast_iteration_statement *nesting_ast = state->loop_nesting_ast;

    state->loop_nesting_ast = this;

    /* Likewise, indicate that following code is closest to a loop,
    * NOT closest to a switch.
    */
    bool saved_is_switch_innermost = state->switch_state.is_switch_innermost;
    state->switch_state.is_switch_innermost = false;

    if (mode != ast_do_while)
        condition_to_hir(&stmt->body_instructions, state);

    if (body != NULL)
        body->hir(& stmt->body_instructions, state);

    if (rest_expression != NULL)
        rest_expression->hir(& stmt->body_instructions, state);

    if (mode == ast_do_while)
        condition_to_hir(&stmt->body_instructions, state);

    if (mode != ast_do_while)
        state->symbols->pop_scope();

    /* Restore previous nesting before returning. */
    state->loop_nesting_ast = nesting_ast;
    state->switch_state.is_switch_innermost = saved_is_switch_innermost;

    /* Loops do not have r-values.
    */
    return NULL;
}


/**
 * Determine if the given type is valid for establishing a default precision
 * qualifier.
 *
 * From GLSL ES 3.00 section 4.5.4 ("Default Precision Qualifiers"):
 *
 *     "The precision statement
 *
 *         precision precision-qualifier type;
 *
 *     can be used to establish a default precision qualifier. The type field
 *     can be either int or float or any of the sampler types, and the
 *     precision-qualifier can be lowp, mediump, or highp."
 *
 * GLSL ES 1.00 has similar language.  GLSL 1.30 doesn't allow precision
 * qualifiers on sampler types, but this seems like an oversight (since the
 * intention of including these in GLSL 1.30 is to allow compatibility with ES
 * shaders).  So we allow int, float, and all sampler types regardless of GLSL
 * version.
 */
static bool
is_valid_default_precision_type(const struct glsl_type *const type)
{
    if (type == NULL)
        return false;

    switch (type->base_type) {
        case GLSL_TYPE_INT:
        case GLSL_TYPE_FLOAT:
            /* "int" and "float" are valid, but vectors and matrices are not. */
            return type->vector_elements == 1 && type->matrix_columns == 1;
        case GLSL_TYPE_SAMPLER:
        case GLSL_TYPE_IMAGE:
        case GLSL_TYPE_ATOMIC_UINT:
            return true;
        default:
            return false;
    }
}


ir_rvalue *
ast_type_specifier::hir(exec_list *instructions,
                        struct _mesa_glsl_parse_state *state)
{
    if (this->default_precision == ast_precision_none && this->structure == NULL)
        return NULL;

    YYLTYPE loc = this->get_location();

    /* If this is a precision statement, check that the type to which it is
    * applied is either float or int.
    *
    * From section 4.5.3 of the GLSL 1.30 spec:
    *    "The precision statement
    *       precision precision-qualifier type;
    *    can be used to establish a default precision qualifier. The type
    *    field can be either int or float [...].  Any other types or
    *    qualifiers will result in an error.
    */
    if (this->default_precision != ast_precision_none) {
        if (!state->check_precision_qualifiers_allowed(&loc))
            return NULL;

        if (this->structure != NULL) {
            _mesa_glsl_error(&loc, state,
                             "precision qualifiers do not apply to structures");
            return NULL;
        }

        if (this->array_specifier != NULL) {
            _mesa_glsl_error(&loc, state,
                             "default precision statements do not apply to "
                             "arrays");
            return NULL;
        }

        const struct glsl_type *const type =
                state->symbols->get_type(this->type_name);
        if (!is_valid_default_precision_type(type)) {
            _mesa_glsl_error(&loc, state,
                             "default precision statements apply only to "
                             "float, int, and opaque types");
            return NULL;
        }

        if (state->es_shader) {
            /* Section 4.5.3 (Default Precision Qualifiers) of the GLSL ES 1.00
          * spec says:
          *
          *     "Non-precision qualified declarations will use the precision
          *     qualifier specified in the most recent precision statement
          *     that is still in scope. The precision statement has the same
          *     scoping rules as variable declarations. If it is declared
          *     inside a compound statement, its effect stops at the end of
          *     the innermost statement it was declared in. Precision
          *     statements in nested scopes override precision statements in
          *     outer scopes. Multiple precision statements for the same basic
          *     type can appear inside the same scope, with later statements
          *     overriding earlier statements within that scope."
          *
          * Default precision specifications follow the same scope rules as
          * variables.  So, we can track the state of the default precision
          * qualifiers in the symbol table, and the rules will just work.  This
          * is a slight abuse of the symbol table, but it has the semantics
          * that we want.
          */
            state->symbols->add_default_precision_qualifier(this->type_name,
                                                            this->default_precision);
        }

        /* FINISHME: Translate precision statements into IR. */
        return NULL;
    }

    /* _mesa_ast_set_aggregate_type() sets the <structure> field so that
    * process_record_constructor() can do type-checking on C-style initializer
    * expressions of structs, but ast_struct_specifier should only be translated
    * to HIR if it is declaring the type of a structure.
    *
    * The ->is_declaration field is false for initializers of variables
    * declared separately from the struct's type definition.
    *
    *    struct S { ... };              (is_declaration = true)
    *    struct T { ... } t = { ... };  (is_declaration = true)
    *    S s = { ... };                 (is_declaration = false)
    */
    if (this->structure != NULL && this->structure->is_declaration)
        return this->structure->hir(instructions, state);

    return NULL;
}


/**
 * Process a structure or interface block tree into an array of structure fields
 *
 * After parsing, where there are some syntax differnces, structures and
 * interface blocks are almost identical.  They are similar enough that the
 * AST for each can be processed the same way into a set of
 * \c glsl_struct_field to describe the members.
 *
 * If we're processing an interface block, var_mode should be the type of the
 * interface block (ir_var_shader_in, ir_var_shader_out, ir_var_uniform or
 * ir_var_shader_storage).  If we're processing a structure, var_mode should be
 * ir_var_auto.
 *
 * \return
 * The number of fields processed.  A pointer to the array structure fields is
 * stored in \c *fields_ret.
 */
static unsigned
ast_process_struct_or_iface_block_members(exec_list *instructions,
                                          struct _mesa_glsl_parse_state *state,
                                          exec_list *declarations,
                                          glsl_struct_field **fields_ret,
                                          bool is_interface,
                                          enum glsl_matrix_layout matrix_layout,
                                          bool allow_reserved_names,
                                          ir_variable_mode var_mode,
                                          ast_type_qualifier *layout,
                                          unsigned block_stream,
                                          unsigned block_xfb_buffer,
                                          unsigned block_xfb_offset,
                                          unsigned expl_location,
                                          unsigned expl_align)
{
    unsigned decl_count = 0;
    unsigned next_offset = 0;

    /* Make an initial pass over the list of fields to determine how
    * many there are.  Each element in this list is an ast_declarator_list.
    * This means that we actually need to count the number of elements in the
    * 'declarations' list in each of the elements.
    */
    foreach_list_typed (ast_declarator_list, decl_list, link, declarations) {
        decl_count += decl_list->declarations.length();
    }

    /* Allocate storage for the fields and process the field
    * declarations.  As the declarations are processed, try to also convert
    * the types to HIR.  This ensures that structure definitions embedded in
    * other structure definitions or in interface blocks are processed.
    */
    glsl_struct_field *const fields = rzalloc_array(state, glsl_struct_field,
                                                    decl_count);

    bool first_member = true;
    bool first_member_has_explicit_location = false;

    unsigned i = 0;
    foreach_list_typed (ast_declarator_list, decl_list, link, declarations) {
        const char *type_name;
        YYLTYPE loc = decl_list->get_location();

        decl_list->type->specifier->hir(instructions, state);

        /* Section 4.1.8 (Structures) of the GLSL 1.10 spec says:
       *
       *    "Anonymous structures are not supported; so embedded structures
       *    must have a declarator. A name given to an embedded struct is
       *    scoped at the same level as the struct it is embedded in."
       *
       * The same section of the  GLSL 1.20 spec says:
       *
       *    "Anonymous structures are not supported. Embedded structures are
       *    not supported."
       *
       * The GLSL ES 1.00 and 3.00 specs have similar langauge. So, we allow
       * embedded structures in 1.10 only.
       */
        if (state->language_version != 110 &&
            decl_list->type->specifier->structure != NULL)
            _mesa_glsl_error(&loc, state,
                             "embedded structure declarations are not allowed");

        const glsl_type *decl_type =
                decl_list->type->glsl_type(& type_name, state);

        const struct ast_type_qualifier *const qual =
                &decl_list->type->qualifier;

        /* From section 4.3.9 of the GLSL 4.40 spec:
       *
       *    "[In interface blocks] opaque types are not allowed."
       *
       * It should be impossible for decl_type to be NULL here.  Cases that
       * might naturally lead to decl_type being NULL, especially for the
       * is_interface case, will have resulted in compilation having
       * already halted due to a syntax error.
       */
        assert(decl_type);

        if (is_interface) {
            /* From section 4.3.7 of the ARB_bindless_texture spec:
          *
          *    "(remove the following bullet from the last list on p. 39,
          *     thereby permitting sampler types in interface blocks; image
          *     types are also permitted in blocks by this extension)"
          *
          *     * sampler types are not allowed
          */
            if (decl_type->contains_atomic() ||
                (!state->has_bindless() && decl_type->contains_opaque())) {
                _mesa_glsl_error(&loc, state, "uniform/buffer in non-default "
                                              "interface block contains %s variable",
                                 state->has_bindless() ? "atomic" : "opaque");
            }
        } else {
            if (decl_type->contains_atomic()) {
                /* From section 4.1.7.3 of the GLSL 4.40 spec:
             *
             *    "Members of structures cannot be declared as atomic counter
             *     types."
             */
                _mesa_glsl_error(&loc, state, "atomic counter in structure");
            }

            if (!state->has_bindless() && decl_type->contains_image()) {
                /* FINISHME: Same problem as with atomic counters.
             * FINISHME: Request clarification from Khronos and add
             * FINISHME: spec quotation here.
             */
                _mesa_glsl_error(&loc, state, "image in structure");
            }
        }

        if (qual->flags.q.explicit_binding) {
            _mesa_glsl_error(&loc, state,
                             "binding layout qualifier cannot be applied "
                             "to struct or interface block members");
        }

        if (is_interface) {
            if (!first_member) {
                if (!layout->flags.q.explicit_location &&
                    ((first_member_has_explicit_location &&
                      !qual->flags.q.explicit_location) ||
                     (!first_member_has_explicit_location &&
                      qual->flags.q.explicit_location))) {
                    _mesa_glsl_error(&loc, state,
                                     "when block-level location layout qualifier "
                                     "is not supplied either all members must "
                                     "have a location layout qualifier or all "
                                     "members must not have a location layout "
                                     "qualifier");
                }
            } else {
                first_member = false;
                first_member_has_explicit_location =
                        qual->flags.q.explicit_location;
            }
        }

        if (qual->flags.q.std140 ||
            qual->flags.q.std430 ||
            qual->flags.q.packed ||
            qual->flags.q.shared) {
            _mesa_glsl_error(&loc, state,
                             "uniform/shader storage block layout qualifiers "
                             "std140, std430, packed, and shared can only be "
                             "applied to uniform/shader storage blocks, not "
                             "members");
        }

        if (qual->flags.q.constant) {
            _mesa_glsl_error(&loc, state,
                             "const storage qualifier cannot be applied "
                             "to struct or interface block members");
        }

        validate_memory_qualifier_for_type(state, &loc, qual, decl_type);
        validate_image_format_qualifier_for_type(state, &loc, qual, decl_type);

        /* From Section 4.4.2.3 (Geometry Outputs) of the GLSL 4.50 spec:
       *
       *   "A block member may be declared with a stream identifier, but
       *   the specified stream must match the stream associated with the
       *   containing block."
       */
        if (qual->flags.q.explicit_stream) {
            unsigned qual_stream;
            if (process_qualifier_constant(state, &loc, "stream",
                                           qual->stream, &qual_stream) &&
                qual_stream != block_stream) {
                _mesa_glsl_error(&loc, state, "stream layout qualifier on "
                                              "interface block member does not match "
                                              "the interface block (%u vs %u)", qual_stream,
                                 block_stream);
            }
        }

        int xfb_buffer;
        unsigned explicit_xfb_buffer = 0;
        if (qual->flags.q.explicit_xfb_buffer) {
            unsigned qual_xfb_buffer;
            if (process_qualifier_constant(state, &loc, "xfb_buffer",
                                           qual->xfb_buffer, &qual_xfb_buffer)) {
                explicit_xfb_buffer = 1;
                if (qual_xfb_buffer != block_xfb_buffer)
                    _mesa_glsl_error(&loc, state, "xfb_buffer layout qualifier on "
                                                  "interface block member does not match "
                                                  "the interface block (%u vs %u)",
                                     qual_xfb_buffer, block_xfb_buffer);
            }
            xfb_buffer = (int) qual_xfb_buffer;
        } else {
            if (layout)
                explicit_xfb_buffer = layout->flags.q.explicit_xfb_buffer;
            xfb_buffer = (int) block_xfb_buffer;
        }

        int xfb_stride = -1;
        if (qual->flags.q.explicit_xfb_stride) {
            unsigned qual_xfb_stride;
            if (process_qualifier_constant(state, &loc, "xfb_stride",
                                           qual->xfb_stride, &qual_xfb_stride)) {
                xfb_stride = (int) qual_xfb_stride;
            }
        }

        if (qual->flags.q.uniform && qual->has_interpolation()) {
            _mesa_glsl_error(&loc, state,
                             "interpolation qualifiers cannot be used "
                             "with uniform interface blocks");
        }

        if ((qual->flags.q.uniform || !is_interface) &&
            qual->has_auxiliary_storage()) {
            _mesa_glsl_error(&loc, state,
                             "auxiliary storage qualifiers cannot be used "
                             "in uniform blocks or structures.");
        }

        if (qual->flags.q.row_major || qual->flags.q.column_major) {
            if (!qual->flags.q.uniform && !qual->flags.q.buffer) {
                _mesa_glsl_error(&loc, state,
                                 "row_major and column_major can only be "
                                 "applied to interface blocks");
            } else
                validate_matrix_layout_for_type(state, &loc, decl_type, NULL);
        }

        foreach_list_typed (ast_declaration, decl, link,
                            &decl_list->declarations) {
            YYLTYPE loc = decl->get_location();

            if (!allow_reserved_names)
                validate_identifier(decl->identifier, loc, state);

            const struct glsl_type *field_type =
                    process_array_type(&loc, decl_type, decl->array_specifier, state);
            validate_array_dimensions(field_type, state, &loc);
            fields[i].type = field_type;
            fields[i].name = decl->identifier;
            fields[i].interpolation =
                    interpret_interpolation_qualifier(qual, field_type,
                                                      var_mode, state, &loc);
            fields[i].centroid = qual->flags.q.centroid ? 1 : 0;
            fields[i].sample = qual->flags.q.sample ? 1 : 0;
            fields[i].patch = qual->flags.q.patch ? 1 : 0;
            fields[i].offset = -1;
            fields[i].explicit_xfb_buffer = explicit_xfb_buffer;
            fields[i].xfb_buffer = xfb_buffer;
            fields[i].xfb_stride = xfb_stride;

            if (qual->flags.q.explicit_location) {
                unsigned qual_location;
                if (process_qualifier_constant(state, &loc, "location",
                                               qual->location, &qual_location)) {
                    fields[i].location = qual_location +
                                         (fields[i].patch ? VARYING_SLOT_PATCH0 : VARYING_SLOT_VAR0);
                    expl_location = fields[i].location +
                                    fields[i].type->count_attribute_slots(false);
                }
            } else {
                if (layout && layout->flags.q.explicit_location) {
                    fields[i].location = expl_location;
                    expl_location += fields[i].type->count_attribute_slots(false);
                } else {
                    fields[i].location = -1;
                }
            }

            if (qual->flags.q.explicit_component) {
                unsigned qual_component;
                if (process_qualifier_constant(state, &loc, "component",
                                               qual->component, &qual_component)) {
                    validate_component_layout_for_type(state, &loc, fields[i].type,
                                                       qual_component);
                    fields[i].component = qual_component;
                }
            } else {
                fields[i].component = -1;
            }

            /* Offset can only be used with std430 and std140 layouts an initial
          * value of 0 is used for error detection.
          */
            unsigned align = 0;
            unsigned size = 0;
            if (layout) {
                bool row_major;
                if (qual->flags.q.row_major ||
                    matrix_layout == GLSL_MATRIX_LAYOUT_ROW_MAJOR) {
                    row_major = true;
                } else {
                    row_major = false;
                }

                if(layout->flags.q.std140) {
                    align = field_type->std140_base_alignment(row_major);
                    size = field_type->std140_size(row_major);
                } else if (layout->flags.q.std430) {
                    align = field_type->std430_base_alignment(row_major);
                    size = field_type->std430_size(row_major);
                }
            }

            if (qual->flags.q.explicit_offset) {
                unsigned qual_offset;
                if (process_qualifier_constant(state, &loc, "offset",
                                               qual->offset, &qual_offset)) {
                    if (align != 0 && size != 0) {
                        if (next_offset > qual_offset)
                            _mesa_glsl_error(&loc, state, "layout qualifier "
                                                          "offset overlaps previous member");

                        if (qual_offset % align) {
                            _mesa_glsl_error(&loc, state, "layout qualifier offset "
                                                          "must be a multiple of the base "
                                                          "alignment of %s", field_type->name);
                        }
                        fields[i].offset = qual_offset;
                        next_offset = qual_offset + size;
                    } else {
                        _mesa_glsl_error(&loc, state, "offset can only be used "
                                                      "with std430 and std140 layouts");
                    }
                }
            }

            if (qual->flags.q.explicit_align || expl_align != 0) {
                unsigned offset = fields[i].offset != -1 ? fields[i].offset :
                                  next_offset;
                if (align == 0 || size == 0) {
                    _mesa_glsl_error(&loc, state, "align can only be used with "
                                                  "std430 and std140 layouts");
                } else if (qual->flags.q.explicit_align) {
                    unsigned member_align;
                    if (process_qualifier_constant(state, &loc, "align",
                                                   qual->align, &member_align)) {
                        if (member_align == 0 ||
                            member_align & (member_align - 1)) {
                            _mesa_glsl_error(&loc, state, "align layout qualifier "
                                                          "is not a power of 2");
                        } else {
                            fields[i].offset = glsl_align(offset, member_align);
                            next_offset = fields[i].offset + size;
                        }
                    }
                } else {
                    fields[i].offset = glsl_align(offset, expl_align);
                    next_offset = fields[i].offset + size;
                }
            } else if (!qual->flags.q.explicit_offset) {
                if (align != 0 && size != 0)
                    next_offset = glsl_align(next_offset, align) + size;
            }

            /* From the ARB_enhanced_layouts spec:
          *
          *    "The given offset applies to the first component of the first
          *    member of the qualified entity.  Then, within the qualified
          *    entity, subsequent components are each assigned, in order, to
          *    the next available offset aligned to a multiple of that
          *    component's size.  Aggregate types are flattened down to the
          *    component level to get this sequence of components."
          */
            if (qual->flags.q.explicit_xfb_offset) {
                unsigned xfb_offset;
                if (process_qualifier_constant(state, &loc, "xfb_offset",
                                               qual->offset, &xfb_offset)) {
                    fields[i].offset = xfb_offset;
                    block_xfb_offset = fields[i].offset +
                                       4 * field_type->component_slots();
                }
            } else {
                if (layout && layout->flags.q.explicit_xfb_offset) {
                    unsigned align = field_type->is_64bit() ? 8 : 4;
                    fields[i].offset = glsl_align(block_xfb_offset, align);
                    block_xfb_offset += 4 * field_type->component_slots();
                }
            }

            /* Propogate row- / column-major information down the fields of the
          * structure or interface block.  Structures need this data because
          * the structure may contain a structure that contains ... a matrix
          * that need the proper layout.
          */
            if (is_interface && layout &&
                (layout->flags.q.uniform || layout->flags.q.buffer) &&
                (field_type->without_array()->is_matrix()
                 || field_type->without_array()->is_struct())) {
                /* If no layout is specified for the field, inherit the layout
             * from the block.
             */
                fields[i].matrix_layout = matrix_layout;

                if (qual->flags.q.row_major)
                    fields[i].matrix_layout = GLSL_MATRIX_LAYOUT_ROW_MAJOR;
                else if (qual->flags.q.column_major)
                    fields[i].matrix_layout = GLSL_MATRIX_LAYOUT_COLUMN_MAJOR;

                /* If we're processing an uniform or buffer block, the matrix
             * layout must be decided by this point.
             */
                assert(fields[i].matrix_layout == GLSL_MATRIX_LAYOUT_ROW_MAJOR
                       || fields[i].matrix_layout == GLSL_MATRIX_LAYOUT_COLUMN_MAJOR);
            }

            /* Memory qualifiers are allowed on buffer and image variables, while
          * the format qualifier is only accepted for images.
          */
            if (var_mode == ir_var_shader_storage ||
                field_type->without_array()->is_image()) {
                /* For readonly and writeonly qualifiers the field definition,
             * if set, overwrites the layout qualifier.
             */
                if (qual->flags.q.read_only || qual->flags.q.write_only) {
                    fields[i].memory_read_only = qual->flags.q.read_only;
                    fields[i].memory_write_only = qual->flags.q.write_only;
                } else {
                    fields[i].memory_read_only =
                            layout ? layout->flags.q.read_only : 0;
                    fields[i].memory_write_only =
                            layout ? layout->flags.q.write_only : 0;
                }

                /* For other qualifiers, we set the flag if either the layout
             * qualifier or the field qualifier are set
             */
                fields[i].memory_coherent = qual->flags.q.coherent ||
                                            (layout && layout->flags.q.coherent);
                fields[i].memory_volatile = qual->flags.q._volatile ||
                                            (layout && layout->flags.q._volatile);
                fields[i].memory_restrict = qual->flags.q.restrict_flag ||
                                            (layout && layout->flags.q.restrict_flag);

                if (field_type->without_array()->is_image()) {
                    if (qual->flags.q.explicit_image_format) {
                        if (qual->image_base_type !=
                            field_type->without_array()->sampled_type) {
                            _mesa_glsl_error(&loc, state, "format qualifier doesn't "
                                                          "match the base data type of the image");
                        }

                        fields[i].image_format = qual->image_format;
                    } else {
                        if (!qual->flags.q.write_only) {
                            _mesa_glsl_error(&loc, state, "image not qualified with "
                                                          "`writeonly' must have a format layout "
                                                          "qualifier");
                        }

                        fields[i].image_format = PIPE_FORMAT_NONE;
                    }
                }
            }

            /* Precision qualifiers do not hold any meaning in Desktop GLSL */
            if (state->es_shader) {
                fields[i].precision = select_gles_precision(qual->precision,
                                                            field_type,
                                                            state,
                                                            &loc);
            } else {
                fields[i].precision = qual->precision;
            }

            i++;
        }
    }

    assert(i == decl_count);

    *fields_ret = fields;
    return decl_count;
}


ir_rvalue *
ast_struct_specifier::hir(exec_list *instructions,
                          struct _mesa_glsl_parse_state *state)
{
    YYLTYPE loc = this->get_location();

    unsigned expl_location = 0;
    if (layout && layout->flags.q.explicit_location) {
        if (!process_qualifier_constant(state, &loc, "location",
                                        layout->location, &expl_location)) {
            return NULL;
        } else {
            expl_location = VARYING_SLOT_VAR0 + expl_location;
        }
    }

    glsl_struct_field *fields;
    unsigned decl_count =
            ast_process_struct_or_iface_block_members(instructions,
                                                      state,
                                                      &this->declarations,
                                                      &fields,
                                                      false,
                                                      GLSL_MATRIX_LAYOUT_INHERITED,
                                                      false /* allow_reserved_names */,
                                                      ir_var_auto,
                                                      layout,
                                                      0, /* for interface only */
                                                      0, /* for interface only */
                                                      0, /* for interface only */
                                                      expl_location,
                                                      0 /* for interface only */);

    validate_identifier(this->name, loc, state);

    type = glsl_type::get_struct_instance(fields, decl_count, this->name);

    if (!type->is_anonymous() && !state->symbols->add_type(name, type)) {
        const glsl_type *match = state->symbols->get_type(name);
        /* allow struct matching for desktop GL - older UE4 does this */
        if (match != NULL && state->is_version(130, 0) && match->record_compare(type, true, false))
            _mesa_glsl_warning(& loc, state, "struct `%s' previously defined", name);
        else
            _mesa_glsl_error(& loc, state, "struct `%s' previously defined", name);
    } else {
        const glsl_type **s = reralloc(state, state->user_structures,
                                       const glsl_type *,
                                       state->num_user_structures + 1);
        if (s != NULL) {
            s[state->num_user_structures] = type;
            state->user_structures = s;
            state->num_user_structures++;
        }
    }

    /* Structure type definitions do not have r-values.
    */
    return NULL;
}


/**
 * Visitor class which detects whether a given interface block has been used.
 */
class interface_block_usage_visitor : public ir_hierarchical_visitor
{
public:
    interface_block_usage_visitor(ir_variable_mode mode, const glsl_type *block)
            : mode(mode), block(block), found(false)
    {
    }

    virtual ir_visitor_status visit(ir_dereference_variable *ir)
    {
        if (ir->var->data.mode == mode && ir->var->get_interface_type() == block) {
            found = true;
            return visit_stop;
        }
        return visit_continue;
    }

    bool usage_found() const
    {
        return this->found;
    }

private:
    ir_variable_mode mode;
    const glsl_type *block;
    bool found;
};

static bool
is_unsized_array_last_element(ir_variable *v)
{
    const glsl_type *interface_type = v->get_interface_type();
    int length = interface_type->length;

    assert(v->type->is_unsized_array());

    /* Check if it is the last element of the interface */
    if (strcmp(interface_type->fields.structure[length-1].name, v->name) == 0)
        return true;
    return false;
}

static void
apply_memory_qualifiers(ir_variable *var, glsl_struct_field field)
{
    var->data.memory_read_only = field.memory_read_only;
    var->data.memory_write_only = field.memory_write_only;
    var->data.memory_coherent = field.memory_coherent;
    var->data.memory_volatile = field.memory_volatile;
    var->data.memory_restrict = field.memory_restrict;
}

ir_rvalue *
ast_interface_block::hir(exec_list *instructions,
                         struct _mesa_glsl_parse_state *state)
{
    YYLTYPE loc = this->get_location();

    /* Interface blocks must be declared at global scope */
    if (state->current_function != NULL) {
        _mesa_glsl_error(&loc, state,
                         "Interface block `%s' must be declared "
                         "at global scope",
                         this->block_name);
    }

    /* Validate qualifiers:
    *
    * - Layout Qualifiers as per the table in Section 4.4
    *   ("Layout Qualifiers") of the GLSL 4.50 spec.
    *
    * - Memory Qualifiers as per Section 4.10 ("Memory Qualifiers") of the
    *   GLSL 4.50 spec:
    *
    *     "Additionally, memory qualifiers may also be used in the declaration
    *      of shader storage blocks"
    *
    * Note the table in Section 4.4 says std430 is allowed on both uniform and
    * buffer blocks however Section 4.4.5 (Uniform and Shader Storage Block
    * Layout Qualifiers) of the GLSL 4.50 spec says:
    *
    *    "The std430 qualifier is supported only for shader storage blocks;
    *    using std430 on a uniform block will result in a compile-time error."
    */
    ast_type_qualifier allowed_blk_qualifiers;
    allowed_blk_qualifiers.flags.i = 0;
    if (this->layout.flags.q.buffer || this->layout.flags.q.uniform) {
        allowed_blk_qualifiers.flags.q.shared = 1;
        allowed_blk_qualifiers.flags.q.packed = 1;
        allowed_blk_qualifiers.flags.q.std140 = 1;
        allowed_blk_qualifiers.flags.q.row_major = 1;
        allowed_blk_qualifiers.flags.q.column_major = 1;
        allowed_blk_qualifiers.flags.q.explicit_align = 1;
        allowed_blk_qualifiers.flags.q.explicit_binding = 1;
        if (this->layout.flags.q.buffer) {
            allowed_blk_qualifiers.flags.q.buffer = 1;
            allowed_blk_qualifiers.flags.q.std430 = 1;
            allowed_blk_qualifiers.flags.q.coherent = 1;
            allowed_blk_qualifiers.flags.q._volatile = 1;
            allowed_blk_qualifiers.flags.q.restrict_flag = 1;
            allowed_blk_qualifiers.flags.q.read_only = 1;
            allowed_blk_qualifiers.flags.q.write_only = 1;
        } else {
            allowed_blk_qualifiers.flags.q.uniform = 1;
        }
    } else {
        /* Interface block */
        assert(this->layout.flags.q.in || this->layout.flags.q.out);

        allowed_blk_qualifiers.flags.q.explicit_location = 1;
        if (this->layout.flags.q.out) {
            allowed_blk_qualifiers.flags.q.out = 1;
            if (state->stage == MESA_SHADER_GEOMETRY ||
                state->stage == MESA_SHADER_TESS_CTRL ||
                state->stage == MESA_SHADER_TESS_EVAL ||
                state->stage == MESA_SHADER_VERTEX ) {
                allowed_blk_qualifiers.flags.q.explicit_xfb_offset = 1;
                allowed_blk_qualifiers.flags.q.explicit_xfb_buffer = 1;
                allowed_blk_qualifiers.flags.q.xfb_buffer = 1;
                allowed_blk_qualifiers.flags.q.explicit_xfb_stride = 1;
                allowed_blk_qualifiers.flags.q.xfb_stride = 1;
                if (state->stage == MESA_SHADER_GEOMETRY) {
                    allowed_blk_qualifiers.flags.q.stream = 1;
                    allowed_blk_qualifiers.flags.q.explicit_stream = 1;
                }
                if (state->stage == MESA_SHADER_TESS_CTRL) {
                    allowed_blk_qualifiers.flags.q.patch = 1;
                }
            }
        } else {
            allowed_blk_qualifiers.flags.q.in = 1;
            if (state->stage == MESA_SHADER_TESS_EVAL) {
                allowed_blk_qualifiers.flags.q.patch = 1;
            }
        }
    }

    this->layout.validate_flags(&loc, state, allowed_blk_qualifiers,
                                "invalid qualifier for block",
                                this->block_name);

    enum glsl_interface_packing packing;
    if (this->layout.flags.q.std140) {
        packing = GLSL_INTERFACE_PACKING_STD140;
    } else if (this->layout.flags.q.packed) {
        packing = GLSL_INTERFACE_PACKING_PACKED;
    } else if (this->layout.flags.q.std430) {
        packing = GLSL_INTERFACE_PACKING_STD430;
    } else {
        /* The default layout is shared.
       */
        packing = GLSL_INTERFACE_PACKING_SHARED;
    }

    ir_variable_mode var_mode;
    const char *iface_type_name;
    if (this->layout.flags.q.in) {
        var_mode = ir_var_shader_in;
        iface_type_name = "in";
    } else if (this->layout.flags.q.out) {
        var_mode = ir_var_shader_out;
        iface_type_name = "out";
    } else if (this->layout.flags.q.uniform) {
        var_mode = ir_var_uniform;
        iface_type_name = "uniform";
    } else if (this->layout.flags.q.buffer) {
        var_mode = ir_var_shader_storage;
        iface_type_name = "buffer";
    } else {
        var_mode = ir_var_auto;
        iface_type_name = "UNKNOWN";
        assert(!"interface block layout qualifier not found!");
    }

    enum glsl_matrix_layout matrix_layout = GLSL_MATRIX_LAYOUT_INHERITED;
    if (this->layout.flags.q.row_major)
        matrix_layout = GLSL_MATRIX_LAYOUT_ROW_MAJOR;
    else if (this->layout.flags.q.column_major)
        matrix_layout = GLSL_MATRIX_LAYOUT_COLUMN_MAJOR;

    bool redeclaring_per_vertex = strcmp(this->block_name, "gl_PerVertex") == 0;
    exec_list declared_variables;
    glsl_struct_field *fields;

    /* For blocks that accept memory qualifiers (i.e. shader storage), verify
    * that we don't have incompatible qualifiers
    */
    if (this->layout.flags.q.read_only && this->layout.flags.q.write_only) {
        _mesa_glsl_error(&loc, state,
                         "Interface block sets both readonly and writeonly");
    }

    unsigned qual_stream;
    if (!process_qualifier_constant(state, &loc, "stream", this->layout.stream,
                                    &qual_stream) ||
        !validate_stream_qualifier(&loc, state, qual_stream)) {
        /* If the stream qualifier is invalid it doesn't make sense to continue
       * on and try to compare stream layouts on member variables against it
       * so just return early.
       */
        return NULL;
    }

    unsigned qual_xfb_buffer;
    if (!process_qualifier_constant(state, &loc, "xfb_buffer",
                                    layout.xfb_buffer, &qual_xfb_buffer) ||
        !validate_xfb_buffer_qualifier(&loc, state, qual_xfb_buffer)) {
        return NULL;
    }

    unsigned qual_xfb_offset = 0;
    if (layout.flags.q.explicit_xfb_offset) {
        if (!process_qualifier_constant(state, &loc, "xfb_offset",
                                        layout.offset, &qual_xfb_offset)) {
            return NULL;
        }
    }

    unsigned qual_xfb_stride = 0;
    if (layout.flags.q.explicit_xfb_stride) {
        if (!process_qualifier_constant(state, &loc, "xfb_stride",
                                        layout.xfb_stride, &qual_xfb_stride)) {
            return NULL;
        }
    }

    unsigned expl_location = 0;
    if (layout.flags.q.explicit_location) {
        if (!process_qualifier_constant(state, &loc, "location",
                                        layout.location, &expl_location)) {
            return NULL;
        } else {
            expl_location += this->layout.flags.q.patch ? VARYING_SLOT_PATCH0
                                                        : VARYING_SLOT_VAR0;
        }
    }

    unsigned expl_align = 0;
    if (layout.flags.q.explicit_align) {
        if (!process_qualifier_constant(state, &loc, "align",
                                        layout.align, &expl_align)) {
            return NULL;
        } else {
            if (expl_align == 0 || expl_align & (expl_align - 1)) {
                _mesa_glsl_error(&loc, state, "align layout qualifier is not a "
                                              "power of 2.");
                return NULL;
            }
        }
    }

    unsigned int num_variables =
            ast_process_struct_or_iface_block_members(&declared_variables,
                                                      state,
                                                      &this->declarations,
                                                      &fields,
                                                      true,
                                                      matrix_layout,
                                                      redeclaring_per_vertex,
                                                      var_mode,
                                                      &this->layout,
                                                      qual_stream,
                                                      qual_xfb_buffer,
                                                      qual_xfb_offset,
                                                      expl_location,
                                                      expl_align);

    if (!redeclaring_per_vertex) {
        validate_identifier(this->block_name, loc, state);

        /* From section 4.3.9 ("Interface Blocks") of the GLSL 4.50 spec:
       *
       *     "Block names have no other use within a shader beyond interface
       *     matching; it is a compile-time error to use a block name at global
       *     scope for anything other than as a block name."
       */
        ir_variable *var = state->symbols->get_variable(this->block_name);
        if (var && !var->type->is_interface()) {
            _mesa_glsl_error(&loc, state, "Block name `%s' is "
                                          "already used in the scope.",
                             this->block_name);
        }
    }

    const glsl_type *earlier_per_vertex = NULL;
    if (redeclaring_per_vertex) {
        /* Find the previous declaration of gl_PerVertex.  If we're redeclaring
       * the named interface block gl_in, we can find it by looking at the
       * previous declaration of gl_in.  Otherwise we can find it by looking
       * at the previous decalartion of any of the built-in outputs,
       * e.g. gl_Position.
       *
       * Also check that the instance name and array-ness of the redeclaration
       * are correct.
       */
        switch (var_mode) {
            case ir_var_shader_in:
                if (ir_variable *earlier_gl_in =
                            state->symbols->get_variable("gl_in")) {
                    earlier_per_vertex = earlier_gl_in->get_interface_type();
                } else {
                    _mesa_glsl_error(&loc, state,
                                     "redeclaration of gl_PerVertex input not allowed "
                                     "in the %s shader",
                                     _mesa_shader_stage_to_string(state->stage));
                }
                if (this->instance_name == NULL ||
                    strcmp(this->instance_name, "gl_in") != 0 || this->array_specifier == NULL ||
                    !this->array_specifier->is_single_dimension()) {
                    _mesa_glsl_error(&loc, state,
                                     "gl_PerVertex input must be redeclared as "
                                     "gl_in[]");
                }
                break;
            case ir_var_shader_out:
                if (ir_variable *earlier_gl_Position =
                            state->symbols->get_variable("gl_Position")) {
                    earlier_per_vertex = earlier_gl_Position->get_interface_type();
                } else if (ir_variable *earlier_gl_out =
                                   state->symbols->get_variable("gl_out")) {
                    earlier_per_vertex = earlier_gl_out->get_interface_type();
                } else {
                    _mesa_glsl_error(&loc, state,
                                     "redeclaration of gl_PerVertex output not "
                                     "allowed in the %s shader",
                                     _mesa_shader_stage_to_string(state->stage));
                }
                if (state->stage == MESA_SHADER_TESS_CTRL) {
                    if (this->instance_name == NULL ||
                        strcmp(this->instance_name, "gl_out") != 0 || this->array_specifier == NULL) {
                        _mesa_glsl_error(&loc, state,
                                         "gl_PerVertex output must be redeclared as "
                                         "gl_out[]");
                    }
                } else {
                    if (this->instance_name != NULL) {
                        _mesa_glsl_error(&loc, state,
                                         "gl_PerVertex output may not be redeclared with "
                                         "an instance name");
                    }
                }
                break;
            default:
                _mesa_glsl_error(&loc, state,
                                 "gl_PerVertex must be declared as an input or an "
                                 "output");
                break;
        }

        if (earlier_per_vertex == NULL) {
            /* An error has already been reported.  Bail out to avoid null
          * dereferences later in this function.
          */
            return NULL;
        }

        /* Copy locations from the old gl_PerVertex interface block. */
        for (unsigned i = 0; i < num_variables; i++) {
            int j = earlier_per_vertex->field_index(fields[i].name);
            if (j == -1) {
                _mesa_glsl_error(&loc, state,
                                 "redeclaration of gl_PerVertex must be a subset "
                                 "of the built-in members of gl_PerVertex");
            } else {
                fields[i].location =
                        earlier_per_vertex->fields.structure[j].location;
                fields[i].offset =
                        earlier_per_vertex->fields.structure[j].offset;
                fields[i].interpolation =
                        earlier_per_vertex->fields.structure[j].interpolation;
                fields[i].centroid =
                        earlier_per_vertex->fields.structure[j].centroid;
                fields[i].sample =
                        earlier_per_vertex->fields.structure[j].sample;
                fields[i].patch =
                        earlier_per_vertex->fields.structure[j].patch;
                fields[i].precision =
                        earlier_per_vertex->fields.structure[j].precision;
                fields[i].explicit_xfb_buffer =
                        earlier_per_vertex->fields.structure[j].explicit_xfb_buffer;
                fields[i].xfb_buffer =
                        earlier_per_vertex->fields.structure[j].xfb_buffer;
                fields[i].xfb_stride =
                        earlier_per_vertex->fields.structure[j].xfb_stride;
            }
        }

        /* From section 7.1 ("Built-in Language Variables") of the GLSL 4.10
       * spec:
       *
       *     If a built-in interface block is redeclared, it must appear in
       *     the shader before any use of any member included in the built-in
       *     declaration, or a compilation error will result.
       *
       * This appears to be a clarification to the behaviour established for
       * gl_PerVertex by GLSL 1.50, therefore we implement this behaviour
       * regardless of GLSL version.
       */
        interface_block_usage_visitor v(var_mode, earlier_per_vertex);
        v.run(instructions);
        if (v.usage_found()) {
            _mesa_glsl_error(&loc, state,
                             "redeclaration of a built-in interface block must "
                             "appear before any use of any member of the "
                             "interface block");
        }
    }

    const glsl_type *block_type =
            glsl_type::get_interface_instance(fields,
                                              num_variables,
                                              packing,
                                              matrix_layout ==
                                              GLSL_MATRIX_LAYOUT_ROW_MAJOR,
                                              this->block_name);

    unsigned component_size = block_type->contains_double() ? 8 : 4;
    int xfb_offset =
            layout.flags.q.explicit_xfb_offset ? (int) qual_xfb_offset : -1;
    validate_xfb_offset_qualifier(&loc, state, xfb_offset, block_type,
                                  component_size);

    if (!state->symbols->add_interface(block_type->name, block_type, var_mode)) {
        YYLTYPE loc = this->get_location();
        _mesa_glsl_error(&loc, state, "interface block `%s' with type `%s' "
                                      "already taken in the current scope",
                         this->block_name, iface_type_name);
    }

    /* Since interface blocks cannot contain statements, it should be
    * impossible for the block to generate any instructions.
    */
    assert(declared_variables.is_empty());

    /* From section 4.3.4 (Inputs) of the GLSL 1.50 spec:
    *
    *     Geometry shader input variables get the per-vertex values written
    *     out by vertex shader output variables of the same names. Since a
    *     geometry shader operates on a set of vertices, each input varying
    *     variable (or input block, see interface blocks below) needs to be
    *     declared as an array.
    */
    if (state->stage == MESA_SHADER_GEOMETRY && this->array_specifier == NULL &&
        var_mode == ir_var_shader_in) {
        _mesa_glsl_error(&loc, state, "geometry shader inputs must be arrays");
    } else if ((state->stage == MESA_SHADER_TESS_CTRL ||
                state->stage == MESA_SHADER_TESS_EVAL) &&
               !this->layout.flags.q.patch &&
               this->array_specifier == NULL &&
               var_mode == ir_var_shader_in) {
        _mesa_glsl_error(&loc, state, "per-vertex tessellation shader inputs must be arrays");
    } else if (state->stage == MESA_SHADER_TESS_CTRL &&
               !this->layout.flags.q.patch &&
               this->array_specifier == NULL &&
               var_mode == ir_var_shader_out) {
        _mesa_glsl_error(&loc, state, "tessellation control shader outputs must be arrays");
    }


    /* Page 39 (page 45 of the PDF) of section 4.3.7 in the GLSL ES 3.00 spec
    * says:
    *
    *     "If an instance name (instance-name) is used, then it puts all the
    *     members inside a scope within its own name space, accessed with the
    *     field selector ( . ) operator (analogously to structures)."
    */
    if (this->instance_name) {
        if (redeclaring_per_vertex) {
            /* When a built-in in an unnamed interface block is redeclared,
          * get_variable_being_redeclared() calls
          * check_builtin_array_max_size() to make sure that built-in array
          * variables aren't redeclared to illegal sizes.  But we're looking
          * at a redeclaration of a named built-in interface block.  So we
          * have to manually call check_builtin_array_max_size() for all parts
          * of the interface that are arrays.
          */
            for (unsigned i = 0; i < num_variables; i++) {
                if (fields[i].type->is_array()) {
                    const unsigned size = fields[i].type->array_size();
                    check_builtin_array_max_size(fields[i].name, size, loc, state);
                }
            }
        } else {
            validate_identifier(this->instance_name, loc, state);
        }

        ir_variable *var;

        if (this->array_specifier != NULL) {
            const glsl_type *block_array_type =
                    process_array_type(&loc, block_type, this->array_specifier, state);

            /* Section 4.3.7 (Interface Blocks) of the GLSL 1.50 spec says:
          *
          *     For uniform blocks declared an array, each individual array
          *     element corresponds to a separate buffer object backing one
          *     instance of the block. As the array size indicates the number
          *     of buffer objects needed, uniform block array declarations
          *     must specify an array size.
          *
          * And a few paragraphs later:
          *
          *     Geometry shader input blocks must be declared as arrays and
          *     follow the array declaration and linking rules for all
          *     geometry shader inputs. All other input and output block
          *     arrays must specify an array size.
          *
          * The same applies to tessellation shaders.
          *
          * The upshot of this is that the only circumstance where an
          * interface array size *doesn't* need to be specified is on a
          * geometry shader input, tessellation control shader input,
          * tessellation control shader output, and tessellation evaluation
          * shader input.
          */
            if (block_array_type->is_unsized_array()) {
                bool allow_inputs = state->stage == MESA_SHADER_GEOMETRY ||
                                    state->stage == MESA_SHADER_TESS_CTRL ||
                                    state->stage == MESA_SHADER_TESS_EVAL;
                bool allow_outputs = state->stage == MESA_SHADER_TESS_CTRL;

                if (this->layout.flags.q.in) {
                    if (!allow_inputs)
                        _mesa_glsl_error(&loc, state,
                                         "unsized input block arrays not allowed in "
                                         "%s shader",
                                         _mesa_shader_stage_to_string(state->stage));
                } else if (this->layout.flags.q.out) {
                    if (!allow_outputs)
                        _mesa_glsl_error(&loc, state,
                                         "unsized output block arrays not allowed in "
                                         "%s shader",
                                         _mesa_shader_stage_to_string(state->stage));
                } else {
                    /* by elimination, this is a uniform block array */
                    _mesa_glsl_error(&loc, state,
                                     "unsized uniform block arrays not allowed in "
                                     "%s shader",
                                     _mesa_shader_stage_to_string(state->stage));
                }
            }

            /* From section 4.3.9 (Interface Blocks) of the GLSL ES 3.10 spec:
          *
          *     * Arrays of arrays of blocks are not allowed
          */
            if (state->es_shader && block_array_type->is_array() &&
                block_array_type->fields.array->is_array()) {
                _mesa_glsl_error(&loc, state,
                                 "arrays of arrays interface blocks are "
                                 "not allowed");
            }

            var = new(state) ir_variable(block_array_type,
                                         this->instance_name,
                                         var_mode);
        } else {
            var = new(state) ir_variable(block_type,
                                         this->instance_name,
                                         var_mode);
        }

        var->data.matrix_layout = matrix_layout == GLSL_MATRIX_LAYOUT_INHERITED
                                  ? GLSL_MATRIX_LAYOUT_COLUMN_MAJOR : matrix_layout;

        if (var_mode == ir_var_shader_in || var_mode == ir_var_uniform)
            var->data.read_only = true;

        var->data.patch = this->layout.flags.q.patch;

        if (state->stage == MESA_SHADER_GEOMETRY && var_mode == ir_var_shader_in)
            handle_geometry_shader_input_decl(state, loc, var);
        else if ((state->stage == MESA_SHADER_TESS_CTRL ||
                  state->stage == MESA_SHADER_TESS_EVAL) && var_mode == ir_var_shader_in)
            handle_tess_shader_input_decl(state, loc, var);
        else if (state->stage == MESA_SHADER_TESS_CTRL && var_mode == ir_var_shader_out)
            handle_tess_ctrl_shader_output_decl(state, loc, var);

        for (unsigned i = 0; i < num_variables; i++) {
            if (var->data.mode == ir_var_shader_storage)
                apply_memory_qualifiers(var, fields[i]);
        }

        if (ir_variable *earlier =
                    state->symbols->get_variable(this->instance_name)) {
            if (!redeclaring_per_vertex) {
                _mesa_glsl_error(&loc, state, "`%s' redeclared",
                                 this->instance_name);
            }
            earlier->data.how_declared = ir_var_declared_normally;
            earlier->type = var->type;
            earlier->reinit_interface_type(block_type);
            delete var;
        } else {
            if (this->layout.flags.q.explicit_binding) {
                apply_explicit_binding(state, &loc, var, var->type,
                                       &this->layout);
            }

            var->data.stream = qual_stream;
            if (layout.flags.q.explicit_location) {
                var->data.location = expl_location;
                var->data.explicit_location = true;
            }

            state->symbols->add_variable(var);
            instructions->push_tail(var);
        }
    } else {
        /* In order to have an array size, the block must also be declared with
       * an instance name.
       */
        assert(this->array_specifier == NULL);

        for (unsigned i = 0; i < num_variables; i++) {
            ir_variable *var =
                    new(state) ir_variable(fields[i].type,
                                           ralloc_strdup(state, fields[i].name),
                                           var_mode);
            var->data.interpolation = fields[i].interpolation;
            var->data.centroid = fields[i].centroid;
            var->data.sample = fields[i].sample;
            var->data.patch = fields[i].patch;
            var->data.stream = qual_stream;
            var->data.location = fields[i].location;

            if (fields[i].location != -1)
                var->data.explicit_location = true;

            var->data.explicit_xfb_buffer = fields[i].explicit_xfb_buffer;
            var->data.xfb_buffer = fields[i].xfb_buffer;

            if (fields[i].offset != -1)
                var->data.explicit_xfb_offset = true;
            var->data.offset = fields[i].offset;

            var->init_interface_type(block_type);

            if (var_mode == ir_var_shader_in || var_mode == ir_var_uniform)
                var->data.read_only = true;

            /* Precision qualifiers do not have any meaning in Desktop GLSL */
            if (state->es_shader) {
                var->data.precision =
                        select_gles_precision(fields[i].precision, fields[i].type,
                                              state, &loc);
            }

            if (fields[i].matrix_layout == GLSL_MATRIX_LAYOUT_INHERITED) {
                var->data.matrix_layout = matrix_layout == GLSL_MATRIX_LAYOUT_INHERITED
                                          ? GLSL_MATRIX_LAYOUT_COLUMN_MAJOR : matrix_layout;
            } else {
                var->data.matrix_layout = fields[i].matrix_layout;
            }

            if (var->data.mode == ir_var_shader_storage)
                apply_memory_qualifiers(var, fields[i]);

            /* Examine var name here since var may get deleted in the next call */
            bool var_is_gl_id = is_gl_identifier(var->name);

            if (redeclaring_per_vertex) {
                bool is_redeclaration;
                var =
                        get_variable_being_redeclared(&var, loc, state,
                                                      true /* allow_all_redeclarations */,
                                                      &is_redeclaration);
                if (!var_is_gl_id || !is_redeclaration) {
                    _mesa_glsl_error(&loc, state,
                                     "redeclaration of gl_PerVertex can only "
                                     "include built-in variables");
                } else if (var->data.how_declared == ir_var_declared_normally) {
                    _mesa_glsl_error(&loc, state,
                                     "`%s' has already been redeclared",
                                     var->name);
                } else {
                    var->data.how_declared = ir_var_declared_in_block;
                    var->reinit_interface_type(block_type);
                }
                continue;
            }

            if (state->symbols->get_variable(var->name) != NULL)
                _mesa_glsl_error(&loc, state, "`%s' redeclared", var->name);

            /* Propagate the "binding" keyword into this UBO/SSBO's fields.
          * The UBO declaration itself doesn't get an ir_variable unless it
          * has an instance name.  This is ugly.
          */
            if (this->layout.flags.q.explicit_binding) {
                apply_explicit_binding(state, &loc, var,
                                       var->get_interface_type(), &this->layout);
            }

            if (var->type->is_unsized_array()) {
                if (var->is_in_shader_storage_block() &&
                    is_unsized_array_last_element(var)) {
                    var->data.from_ssbo_unsized_array = true;
                } else {
                    /* From GLSL ES 3.10 spec, section 4.1.9 "Arrays":
                *
                * "If an array is declared as the last member of a shader storage
                * block and the size is not specified at compile-time, it is
                * sized at run-time. In all other cases, arrays are sized only
                * at compile-time."
                *
                * In desktop GLSL it is allowed to have unsized-arrays that are
                * not last, as long as we can determine that they are implicitly
                * sized.
                */
                    if (state->es_shader) {
                        _mesa_glsl_error(&loc, state, "unsized array `%s' "
                                                      "definition: only last member of a shader "
                                                      "storage block can be defined as unsized "
                                                      "array", fields[i].name);
                    }
                }
            }

            state->symbols->add_variable(var);
            instructions->push_tail(var);
        }

        if (redeclaring_per_vertex && block_type != earlier_per_vertex) {
            /* From section 7.1 ("Built-in Language Variables") of the GLSL 4.10 spec:
          *
          *     It is also a compilation error ... to redeclare a built-in
          *     block and then use a member from that built-in block that was
          *     not included in the redeclaration.
          *
          * This appears to be a clarification to the behaviour established
          * for gl_PerVertex by GLSL 1.50, therefore we implement this
          * behaviour regardless of GLSL version.
          *
          * To prevent the shader from using a member that was not included in
          * the redeclaration, we disable any ir_variables that are still
          * associated with the old declaration of gl_PerVertex (since we've
          * already updated all of the variables contained in the new
          * gl_PerVertex to point to it).
          *
          * As a side effect this will prevent
          * validate_intrastage_interface_blocks() from getting confused and
          * thinking there are conflicting definitions of gl_PerVertex in the
          * shader.
          */
            foreach_in_list_safe(ir_instruction, node, instructions) {
                ir_variable *const var = node->as_variable();
                if (var != NULL &&
                    var->get_interface_type() == earlier_per_vertex &&
                    var->data.mode == var_mode) {
                    if (var->data.how_declared == ir_var_declared_normally) {
                        _mesa_glsl_error(&loc, state,
                                         "redeclaration of gl_PerVertex cannot "
                                         "follow a redeclaration of `%s'",
                                         var->name);
                    }
                    state->symbols->disable_variable(var->name);
                    var->remove();
                }
            }
        }
    }

    return NULL;
}


ir_rvalue *
ast_tcs_output_layout::hir(exec_list *instructions,
                           struct _mesa_glsl_parse_state *state)
{
    YYLTYPE loc = this->get_location();

    unsigned num_vertices;
    if (!state->out_qualifier->vertices->
            process_qualifier_constant(state, "vertices", &num_vertices,
                                       false)) {
        /* return here to stop cascading incorrect error messages */
        return NULL;
    }

    /* If any shader outputs occurred before this declaration and specified an
    * array size, make sure the size they specified is consistent with the
    * primitive type.
    */
    if (state->tcs_output_size != 0 && state->tcs_output_size != num_vertices) {
        _mesa_glsl_error(&loc, state,
                         "this tessellation control shader output layout "
                         "specifies %u vertices, but a previous output "
                         "is declared with size %u",
                         num_vertices, state->tcs_output_size);
        return NULL;
    }

    state->tcs_output_vertices_specified = true;

    /* If any shader outputs occurred before this declaration and did not
    * specify an array size, their size is determined now.
    */
    foreach_in_list (ir_instruction, node, instructions) {
        ir_variable *var = node->as_variable();
        if (var == NULL || var->data.mode != ir_var_shader_out)
            continue;

        /* Note: Not all tessellation control shader output are arrays. */
        if (!var->type->is_unsized_array() || var->data.patch)
            continue;

        if (var->data.max_array_access >= (int)num_vertices) {
            _mesa_glsl_error(&loc, state,
                             "this tessellation control shader output layout "
                             "specifies %u vertices, but an access to element "
                             "%u of output `%s' already exists", num_vertices,
                             var->data.max_array_access, var->name);
        } else {
            var->type = glsl_type::get_array_instance(var->type->fields.array,
                                                      num_vertices);
        }
    }

    return NULL;
}


ir_rvalue *
ast_gs_input_layout::hir(exec_list *instructions,
                         struct _mesa_glsl_parse_state *state)
{
    YYLTYPE loc = this->get_location();

    /* Should have been prevented by the parser. */
    assert(!state->gs_input_prim_type_specified
           || state->in_qualifier->prim_type == this->prim_type);

    /* If any shader inputs occurred before this declaration and specified an
    * array size, make sure the size they specified is consistent with the
    * primitive type.
    */
    unsigned num_vertices = vertices_per_prim(this->prim_type);
    if (state->gs_input_size != 0 && state->gs_input_size != num_vertices) {
        _mesa_glsl_error(&loc, state,
                         "this geometry shader input layout implies %u vertices"
                         " per primitive, but a previous input is declared"
                         " with size %u", num_vertices, state->gs_input_size);
        return NULL;
    }

    state->gs_input_prim_type_specified = true;

    /* If any shader inputs occurred before this declaration and did not
    * specify an array size, their size is determined now.
    */
    foreach_in_list(ir_instruction, node, instructions) {
        ir_variable *var = node->as_variable();
        if (var == NULL || var->data.mode != ir_var_shader_in)
            continue;

        /* Note: gl_PrimitiveIDIn has mode ir_var_shader_in, but it's not an
       * array; skip it.
       */

        if (var->type->is_unsized_array()) {
            if (var->data.max_array_access >= (int)num_vertices) {
                _mesa_glsl_error(&loc, state,
                                 "this geometry shader input layout implies %u"
                                 " vertices, but an access to element %u of input"
                                 " `%s' already exists", num_vertices,
                                 var->data.max_array_access, var->name);
            } else {
                var->type = glsl_type::get_array_instance(var->type->fields.array,
                                                          num_vertices);
            }
        }
    }

    return NULL;
}


ir_rvalue *
ast_cs_input_layout::hir(exec_list *instructions,
                         struct _mesa_glsl_parse_state *state)
{
    YYLTYPE loc = this->get_location();

    /* From the ARB_compute_shader specification:
    *
    *     If the local size of the shader in any dimension is greater
    *     than the maximum size supported by the implementation for that
    *     dimension, a compile-time error results.
    *
    * It is not clear from the spec how the error should be reported if
    * the total size of the work group exceeds
    * MAX_COMPUTE_WORK_GROUP_INVOCATIONS, but it seems reasonable to
    * report it at compile time as well.
    */
    GLuint64 total_invocations = 1;
    unsigned qual_local_size[3];
    for (int i = 0; i < 3; i++) {

        char *local_size_str = ralloc_asprintf(NULL, "invalid local_size_%c",
                                               'x' + i);
        /* Infer a local_size of 1 for unspecified dimensions */
        if (this->local_size[i] == NULL) {
            qual_local_size[i] = 1;
        } else if (!this->local_size[i]->
                process_qualifier_constant(state, local_size_str,
                                           &qual_local_size[i], false)) {
            ralloc_free(local_size_str);
            return NULL;
        }
        ralloc_free(local_size_str);

        if (qual_local_size[i] > state->ctx->Const.MaxComputeWorkGroupSize[i]) {
            _mesa_glsl_error(&loc, state,
                             "local_size_%c exceeds MAX_COMPUTE_WORK_GROUP_SIZE"
                             " (%d)", 'x' + i,
                             state->ctx->Const.MaxComputeWorkGroupSize[i]);
            break;
        }
        total_invocations *= qual_local_size[i];
        if (total_invocations >
            state->ctx->Const.MaxComputeWorkGroupInvocations) {
            _mesa_glsl_error(&loc, state,
                             "product of local_sizes exceeds "
                             "MAX_COMPUTE_WORK_GROUP_INVOCATIONS (%d)",
                             state->ctx->Const.MaxComputeWorkGroupInvocations);
            break;
        }
    }

    /* If any compute input layout declaration preceded this one, make sure it
    * was consistent with this one.
    */
    if (state->cs_input_local_size_specified) {
        for (int i = 0; i < 3; i++) {
            if (state->cs_input_local_size[i] != qual_local_size[i]) {
                _mesa_glsl_error(&loc, state,
                                 "compute shader input layout does not match"
                                 " previous declaration");
                return NULL;
            }
        }
    }

    /* The ARB_compute_variable_group_size spec says:
    *
    *     If a compute shader including a *local_size_variable* qualifier also
    *     declares a fixed local group size using the *local_size_x*,
    *     *local_size_y*, or *local_size_z* qualifiers, a compile-time error
    *     results
    */
    if (state->cs_input_local_size_variable_specified) {
        _mesa_glsl_error(&loc, state,
                         "compute shader can't include both a variable and a "
                         "fixed local group size");
        return NULL;
    }

    state->cs_input_local_size_specified = true;
    for (int i = 0; i < 3; i++)
        state->cs_input_local_size[i] = qual_local_size[i];

    /* We may now declare the built-in constant gl_WorkGroupSize (see
    * builtin_variable_generator::generate_constants() for why we didn't
    * declare it earlier).
    */
    ir_variable *var = new(state->symbols)
            ir_variable(glsl_type::uvec3_type, "gl_WorkGroupSize", ir_var_auto);
    var->data.how_declared = ir_var_declared_implicitly;
    var->data.read_only = true;
    instructions->push_tail(var);
    state->symbols->add_variable(var);
    ir_constant_data data;
    memset(&data, 0, sizeof(data));
    for (int i = 0; i < 3; i++)
        data.u[i] = qual_local_size[i];
    var->constant_value = new(var) ir_constant(glsl_type::uvec3_type, &data);
    var->constant_initializer =
            new(var) ir_constant(glsl_type::uvec3_type, &data);
    var->data.has_initializer = true;
    var->data.is_implicit_initializer = false;

    return NULL;
}


static void
detect_conflicting_assignments(struct _mesa_glsl_parse_state *state,
                               exec_list *instructions)
{
    bool gl_FragColor_assigned = false;
    bool gl_FragData_assigned = false;
    bool gl_FragSecondaryColor_assigned = false;
    bool gl_FragSecondaryData_assigned = false;
    bool user_defined_fs_output_assigned = false;
    ir_variable *user_defined_fs_output = NULL;

    /* It would be nice to have proper location information. */
    YYLTYPE loc;
    memset(&loc, 0, sizeof(loc));

    foreach_in_list(ir_instruction, node, instructions) {
        ir_variable *var = node->as_variable();

        if (!var || !var->data.assigned)
            continue;

        if (strcmp(var->name, "gl_FragColor") == 0) {
            gl_FragColor_assigned = true;
            if (!var->constant_initializer && state->zero_init) {
                const ir_constant_data data = { { 0 } };
                var->data.has_initializer = true;
                var->data.is_implicit_initializer = true;
                var->constant_initializer = new(var) ir_constant(var->type, &data);
            }
        }
        else if (strcmp(var->name, "gl_FragData") == 0)
            gl_FragData_assigned = true;
        else if (strcmp(var->name, "gl_SecondaryFragColorEXT") == 0)
            gl_FragSecondaryColor_assigned = true;
        else if (strcmp(var->name, "gl_SecondaryFragDataEXT") == 0)
            gl_FragSecondaryData_assigned = true;
        else if (!is_gl_identifier(var->name)) {
            if (state->stage == MESA_SHADER_FRAGMENT &&
                var->data.mode == ir_var_shader_out) {
                user_defined_fs_output_assigned = true;
                user_defined_fs_output = var;
            }
        }
    }

    /* From the GLSL 1.30 spec:
    *
    *     "If a shader statically assigns a value to gl_FragColor, it
    *      may not assign a value to any element of gl_FragData. If a
    *      shader statically writes a value to any element of
    *      gl_FragData, it may not assign a value to
    *      gl_FragColor. That is, a shader may assign values to either
    *      gl_FragColor or gl_FragData, but not both. Multiple shaders
    *      linked together must also consistently write just one of
    *      these variables.  Similarly, if user declared output
    *      variables are in use (statically assigned to), then the
    *      built-in variables gl_FragColor and gl_FragData may not be
    *      assigned to. These incorrect usages all generate compile
    *      time errors."
    */
    if (gl_FragColor_assigned && gl_FragData_assigned) {
        _mesa_glsl_error(&loc, state, "fragment shader writes to both "
                                      "`gl_FragColor' and `gl_FragData'");
    } else if (gl_FragColor_assigned && user_defined_fs_output_assigned) {
        _mesa_glsl_error(&loc, state, "fragment shader writes to both "
                                      "`gl_FragColor' and `%s'",
                         user_defined_fs_output->name);
    } else if (gl_FragSecondaryColor_assigned && gl_FragSecondaryData_assigned) {
        _mesa_glsl_error(&loc, state, "fragment shader writes to both "
                                      "`gl_FragSecondaryColorEXT' and"
                                      " `gl_FragSecondaryDataEXT'");
    } else if (gl_FragColor_assigned && gl_FragSecondaryData_assigned) {
        _mesa_glsl_error(&loc, state, "fragment shader writes to both "
                                      "`gl_FragColor' and"
                                      " `gl_FragSecondaryDataEXT'");
    } else if (gl_FragData_assigned && gl_FragSecondaryColor_assigned) {
        _mesa_glsl_error(&loc, state, "fragment shader writes to both "
                                      "`gl_FragData' and"
                                      " `gl_FragSecondaryColorEXT'");
    } else if (gl_FragData_assigned && user_defined_fs_output_assigned) {
        _mesa_glsl_error(&loc, state, "fragment shader writes to both "
                                      "`gl_FragData' and `%s'",
                         user_defined_fs_output->name);
    }

    if ((gl_FragSecondaryColor_assigned || gl_FragSecondaryData_assigned) &&
        !state->EXT_blend_func_extended_enable) {
        _mesa_glsl_error(&loc, state,
                         "Dual source blending requires EXT_blend_func_extended");
    }
}

static void
verify_subroutine_associated_funcs(struct _mesa_glsl_parse_state *state)
{
    YYLTYPE loc;
    memset(&loc, 0, sizeof(loc));

    /* Section 6.1.2 (Subroutines) of the GLSL 4.00 spec says:
    *
    *   "A program will fail to compile or link if any shader
    *    or stage contains two or more functions with the same
    *    name if the name is associated with a subroutine type."
    */

    for (int i = 0; i < state->num_subroutines; i++) {
        unsigned definitions = 0;
        ir_function *fn = state->subroutines[i];
        /* Calculate number of function definitions with the same name */
        foreach_in_list(ir_function_signature, sig, &fn->signatures) {
            if (sig->is_defined) {
                if (++definitions > 1) {
                    _mesa_glsl_error(&loc, state,
                                     "%s shader contains two or more function "
                                     "definitions with name `%s', which is "
                                     "associated with a subroutine type.\n",
                                     _mesa_shader_stage_to_string(state->stage),
                                     fn->name);
                    return;
                }
            }
        }
    }
}

static void
remove_per_vertex_blocks(exec_list *instructions,
                         _mesa_glsl_parse_state *state, ir_variable_mode mode)
{
    /* Find the gl_PerVertex interface block of the appropriate (in/out) mode,
    * if it exists in this shader type.
    */
    const glsl_type *per_vertex = NULL;
    switch (mode) {
        case ir_var_shader_in:
            if (ir_variable *gl_in = state->symbols->get_variable("gl_in"))
                per_vertex = gl_in->get_interface_type();
            break;
        case ir_var_shader_out:
            if (ir_variable *gl_Position =
                        state->symbols->get_variable("gl_Position")) {
                per_vertex = gl_Position->get_interface_type();
            }
            break;
        default:
            assert(!"Unexpected mode");
            break;
    }

    /* If we didn't find a built-in gl_PerVertex interface block, then we don't
    * need to do anything.
    */
    if (per_vertex == NULL)
        return;

    /* If the interface block is used by the shader, then we don't need to do
    * anything.
    */
    interface_block_usage_visitor v(mode, per_vertex);
    v.run(instructions);
    if (v.usage_found())
        return;

    /* Remove any ir_variable declarations that refer to the interface block
    * we're removing.
    */
    foreach_in_list_safe(ir_instruction, node, instructions) {
        ir_variable *const var = node->as_variable();
        if (var != NULL && var->get_interface_type() == per_vertex &&
            var->data.mode == mode) {
            state->symbols->disable_variable(var->name);
            var->remove();
        }
    }
}

ir_rvalue *
ast_warnings_toggle::hir(exec_list *,
                         struct _mesa_glsl_parse_state *state)
{
    state->warnings_enabled = enable;
    return NULL;
}
