#include "../glsl_types.h"
#include "ir.h"

namespace {

    class opt_conditional_discard_visitor : public ir_hierarchical_visitor {
    public:
        opt_conditional_discard_visitor()
        {
            progress = false;
        }

        ir_visitor_status visit_leave(ir_if *);

        bool progress;
    };

} /* anonymous namespace */

bool
opt_conditional_discard(exec_list *instructions)
{
    opt_conditional_discard_visitor v;
    v.run(instructions);
    return v.progress;
}

ir_visitor_status
opt_conditional_discard_visitor::visit_leave(ir_if *ir)
{
    /* Look for "if (...) discard" with no else clause or extra statements. */
    if (ir->then_instructions.is_empty() ||
        !ir->then_instructions.get_head_raw()->next->is_tail_sentinel() ||
        !((ir_instruction *) ir->then_instructions.get_head_raw())->as_discard() ||
        !ir->else_instructions.is_empty())
        return visit_continue;

    /* Move the condition and replace the ir_if with the ir_discard. */
    ir_discard *discard = (ir_discard *) ir->then_instructions.get_head_raw();
    if (!discard->condition)
        discard->condition = ir->condition;
    else {
        void *ctx = ralloc_parent(ir);
        discard->condition = new(ctx) ir_expression(ir_binop_logic_and,
                                                    ir->condition,
                                                    discard->condition);
    }
    ir->replace_with(discard);

    progress = true;

    return visit_continue;
}
