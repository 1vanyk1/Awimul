#include "ir.h"
#include "ir_rvalue_visitor.h"
#include "ir_optimization.h"
#include "../../compiler/glsl_types.h"
#include "../../main/macros.h"

namespace {

    class ir_vec_index_to_swizzle_visitor : public ir_rvalue_visitor {
    public:
        ir_vec_index_to_swizzle_visitor()
        {
            progress = false;
        }

        ir_rvalue *convert_vector_extract_to_swizzle(ir_rvalue *val);

        virtual void handle_rvalue(ir_rvalue **);

        bool progress;
    };

} /* anonymous namespace */

void
ir_vec_index_to_swizzle_visitor::handle_rvalue(ir_rvalue **rv)
{
    if (*rv == NULL)
        return;

    ir_expression *const expr = (*rv)->as_expression();
    if (expr == NULL || expr->operation != ir_binop_vector_extract)
        return;

    void *mem_ctx = ralloc_parent(expr);
    ir_constant *const idx =
            expr->operands[1]->constant_expression_value(mem_ctx);
    if (idx == NULL)
        return;

    this->progress = true;

    /* Page 40 of the GLSL 1.20 spec says:
     *
     *     "When indexing with non-constant expressions, behavior is undefined
     *     if the index is negative, or greater than or equal to the size of
     *     the vector."
     *
     * The quoted spec text mentions non-constant expressions, but this code
     * operates on constants.  These constants are the result of non-constant
     * expressions that have been optimized to constants.  The common case here
     * is a loop counter from an unrolled loop that is used to index a vector.
     *
     * The ir_swizzle constructor gets angry if the index is negative or too
     * large.  For simplicity sake, just clamp the index to [0, size-1].
     */
    const int i = CLAMP(idx->value.i[0], 0,
                        (int) expr->operands[0]->type->vector_elements - 1);

    *rv = new(mem_ctx) ir_swizzle(expr->operands[0], i, 0, 0, 0, 1);
}

bool
do_vec_index_to_swizzle(exec_list *instructions)
{
    ir_vec_index_to_swizzle_visitor v;

    v.run(instructions);

    return v.progress;
}
