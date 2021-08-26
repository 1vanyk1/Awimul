#include "ir.h"
#include "ir_visitor.h"
#include "ir_optimization.h"
#include "../../compiler/glsl_types.h"
#include "ir_builder.h"

using namespace ir_builder;

namespace {

/**
 * Visitor class for replacing expressions with ir_constant values.
 */

    class ir_vec_index_to_cond_assign_visitor : public ir_hierarchical_visitor {
    public:
        ir_vec_index_to_cond_assign_visitor()
                : progress(false)
        {
            /* empty */
        }

        ir_rvalue *convert_vec_index_to_cond_assign(void *mem_ctx,
                                                    ir_rvalue *orig_vector,
                                                    ir_rvalue *orig_index,
                                                    const glsl_type *type);

        ir_rvalue *convert_vector_extract_to_cond_assign(ir_rvalue *ir);

        virtual ir_visitor_status visit_enter(ir_expression *);
        virtual ir_visitor_status visit_enter(ir_swizzle *);
        virtual ir_visitor_status visit_leave(ir_assignment *);
        virtual ir_visitor_status visit_enter(ir_return *);
        virtual ir_visitor_status visit_enter(ir_call *);
        virtual ir_visitor_status visit_enter(ir_if *);

        bool progress;
    };

} /* anonymous namespace */

ir_rvalue *
ir_vec_index_to_cond_assign_visitor::convert_vec_index_to_cond_assign(void *mem_ctx,
                                                                      ir_rvalue *orig_vector,
                                                                      ir_rvalue *orig_index,
                                                                      const glsl_type *type)
{
    exec_list list;
    ir_factory body(&list, base_ir);

    /* Store the index to a temporary to avoid reusing its tree. */
    assert(orig_index->type == glsl_type::int_type ||
           orig_index->type == glsl_type::uint_type);
    ir_variable *const index =
            body.make_temp(orig_index->type, "vec_index_tmp_i");

    body.emit(assign(index, orig_index));

    /* Store the value inside a temp, thus avoiding matrixes duplication */
    ir_variable *const value =
            body.make_temp(orig_vector->type, "vec_value_tmp");

    body.emit(assign(value, orig_vector));


    /* Temporary where we store whichever value we swizzle out. */
    ir_variable *const var = body.make_temp(type, "vec_index_tmp_v");

    /* Generate a single comparison condition "mask" for all of the components
     * in the vector.
     */
    ir_variable *const cond =
            compare_index_block(body, index, 0, orig_vector->type->vector_elements);

    /* Generate a conditional move of each vector element to the temp. */
    for (unsigned i = 0; i < orig_vector->type->vector_elements; i++)
        body.emit(assign(var, swizzle(value, i, 1), swizzle(cond, i, 1)));

    /* Put all of the new instructions in the IR stream before the old
     * instruction.
     */
    base_ir->insert_before(&list);

    this->progress = true;
    return deref(var).val;
}

ir_rvalue *
ir_vec_index_to_cond_assign_visitor::convert_vector_extract_to_cond_assign(ir_rvalue *ir)
{
    ir_expression *const expr = ir->as_expression();

    if (expr == NULL)
        return ir;

    if (expr->operation == ir_unop_interpolate_at_centroid ||
        expr->operation == ir_binop_interpolate_at_offset ||
        expr->operation == ir_binop_interpolate_at_sample) {
        /* Lower interpolateAtXxx(some_vec[idx], ...) to
         * interpolateAtXxx(some_vec, ...)[idx] before lowering to conditional
         * assignments, to maintain the rule that the interpolant is an l-value
         * referring to a (part of a) shader input.
         *
         * This is required when idx is dynamic (otherwise it gets lowered to
         * a swizzle).
         */
        ir_expression *const interpolant = expr->operands[0]->as_expression();
        if (!interpolant || interpolant->operation != ir_binop_vector_extract)
            return ir;

        ir_rvalue *vec_input = interpolant->operands[0];
        ir_expression *const vec_interpolate =
                new(base_ir) ir_expression(expr->operation, vec_input->type,
                                           vec_input, expr->operands[1]);

        return convert_vec_index_to_cond_assign(ralloc_parent(ir),
                                                vec_interpolate,
                                                interpolant->operands[1],
                                                ir->type);
    }

    if (expr->operation != ir_binop_vector_extract)
        return ir;

    return convert_vec_index_to_cond_assign(ralloc_parent(ir),
                                            expr->operands[0],
                                            expr->operands[1],
                                            ir->type);
}

ir_visitor_status
ir_vec_index_to_cond_assign_visitor::visit_enter(ir_expression *ir)
{
    for (unsigned i = 0; i < ir->num_operands; i++)
        ir->operands[i] = convert_vector_extract_to_cond_assign(ir->operands[i]);

    return visit_continue;
}

ir_visitor_status
ir_vec_index_to_cond_assign_visitor::visit_enter(ir_swizzle *ir)
{
    /* Can't be hit from normal GLSL, since you can't swizzle a scalar (which
     * the result of indexing a vector is.  But maybe at some point we'll end up
     * using swizzling of scalars for vector construction.
     */
    ir->val = convert_vector_extract_to_cond_assign(ir->val);

    return visit_continue;
}

ir_visitor_status
ir_vec_index_to_cond_assign_visitor::visit_leave(ir_assignment *ir)
{
    ir->rhs = convert_vector_extract_to_cond_assign(ir->rhs);

    if (ir->condition)
        ir->condition = convert_vector_extract_to_cond_assign(ir->condition);

    return visit_continue;
}

ir_visitor_status
ir_vec_index_to_cond_assign_visitor::visit_enter(ir_call *ir)
{
    foreach_in_list_safe(ir_rvalue, param, &ir->actual_parameters) {
        ir_rvalue *new_param = convert_vector_extract_to_cond_assign(param);

        if (new_param != param) {
            param->replace_with(new_param);
        }
    }

    return visit_continue;
}

ir_visitor_status
ir_vec_index_to_cond_assign_visitor::visit_enter(ir_return *ir)
{
    if (ir->value)
        ir->value = convert_vector_extract_to_cond_assign(ir->value);

    return visit_continue;
}

ir_visitor_status
ir_vec_index_to_cond_assign_visitor::visit_enter(ir_if *ir)
{
    ir->condition = convert_vector_extract_to_cond_assign(ir->condition);

    return visit_continue;
}

bool
do_vec_index_to_cond_assign(exec_list *instructions)
{
    ir_vec_index_to_cond_assign_visitor v;

    visit_list_elements(&v, instructions);

    return v.progress;
}
