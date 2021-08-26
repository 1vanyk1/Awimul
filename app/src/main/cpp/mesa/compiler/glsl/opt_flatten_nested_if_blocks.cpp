#include "ir.h"
#include "ir_builder.h"

using namespace ir_builder;

namespace {

    class nested_if_flattener : public ir_hierarchical_visitor {
    public:
        nested_if_flattener()
        {
            progress = false;
        }

        ir_visitor_status visit_leave(ir_if *);
        ir_visitor_status visit_enter(ir_assignment *);

        bool progress;
    };

} /* unnamed namespace */

/* We only care about the top level "if" instructions, so don't
 * descend into expressions.
 */
ir_visitor_status
nested_if_flattener::visit_enter(ir_assignment *ir)
{
    (void) ir;
    return visit_continue_with_parent;
}

bool
opt_flatten_nested_if_blocks(exec_list *instructions)
{
    nested_if_flattener v;

    v.run(instructions);
    return v.progress;
}


ir_visitor_status
nested_if_flattener::visit_leave(ir_if *ir)
{
    /* Only handle a single ir_if within the then clause of an ir_if.  No extra
     * instructions, no else clauses, nothing.
     */
    if (ir->then_instructions.is_empty() || !ir->else_instructions.is_empty())
        return visit_continue;

    ir_if *inner = ((ir_instruction *) ir->then_instructions.get_head_raw())->as_if();
    if (!inner || !inner->next->is_tail_sentinel() ||
        !inner->else_instructions.is_empty())
        return visit_continue;

    ir->condition = logic_and(ir->condition, inner->condition);
    inner->then_instructions.move_nodes_to(&ir->then_instructions);

    progress = true;
    return visit_continue;
}
