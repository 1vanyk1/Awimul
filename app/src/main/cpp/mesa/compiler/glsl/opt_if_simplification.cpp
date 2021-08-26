#include "ir.h"

namespace {

    class ir_if_simplification_visitor : public ir_hierarchical_visitor {
    public:
        ir_if_simplification_visitor()
        {
            this->made_progress = false;
        }

        ir_visitor_status visit_leave(ir_if *);
        ir_visitor_status visit_enter(ir_assignment *);

        bool made_progress;
    };

} /* unnamed namespace */

/* We only care about the top level "if" instructions, so don't
 * descend into expressions.
 */
ir_visitor_status
ir_if_simplification_visitor::visit_enter(ir_assignment *ir)
{
    (void) ir;
    return visit_continue_with_parent;
}

bool
do_if_simplification(exec_list *instructions)
{
    ir_if_simplification_visitor v;

    v.run(instructions);
    return v.made_progress;
}


ir_visitor_status
ir_if_simplification_visitor::visit_leave(ir_if *ir)
{
    /* If the if statement has nothing on either side, remove it. */
    if (ir->then_instructions.is_empty() &&
        ir->else_instructions.is_empty()) {
        ir->remove();
        this->made_progress = true;
        return visit_continue;
    }

    /* FINISHME: Ideally there would be a way to note that the condition results
     * FINISHME: in a constant before processing both of the other subtrees.
     * FINISHME: This can probably be done with some flags, but it would take
     * FINISHME: some work to get right.
     */
    ir_constant *condition_constant =
            ir->condition->constant_expression_value(ralloc_parent(ir));
    if (condition_constant) {
        /* Move the contents of the one branch of the conditional
         * that matters out.
         */
        if (condition_constant->value.b[0]) {
            ir->insert_before(&ir->then_instructions);
        } else {
            ir->insert_before(&ir->else_instructions);
        }
        ir->remove();
        this->made_progress = true;
        return visit_continue;
    }

    /* Turn:
     *
     *     if (cond) {
     *     } else {
     *         do_work();
     *     }
     *
     * into :
     *
     *     if (!cond)
     *         do_work();
     *
     * which avoids control flow for "else" (which is usually more
     * expensive than normal operations), and the "not" can usually be
     * folded into the generation of "cond" anyway.
     */
    if (ir->then_instructions.is_empty()) {
        ir->condition = new(ralloc_parent(ir->condition))
                ir_expression(ir_unop_logic_not, ir->condition);
        ir->else_instructions.move_nodes_to(&ir->then_instructions);
        this->made_progress = true;
    }

    return visit_continue;
}
