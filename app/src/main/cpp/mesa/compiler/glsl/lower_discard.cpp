#include "../glsl_types.h"
#include "ir.h"

namespace {

    class lower_discard_visitor : public ir_hierarchical_visitor {
    public:
        lower_discard_visitor()
        {
            this->progress = false;
        }

        ir_visitor_status visit_leave(ir_if *);

        bool progress;
    };

} /* anonymous namespace */

bool
lower_discard(exec_list *instructions)
{
    lower_discard_visitor v;

    visit_list_elements(&v, instructions);

    return v.progress;
}


static ir_discard *
find_discard(exec_list &instructions)
{
    foreach_in_list(ir_instruction, node, &instructions) {
        ir_discard *ir = node->as_discard();
        if (ir != NULL)
            return ir;
    }
    return NULL;
}


static void
replace_discard(void *mem_ctx, ir_variable *var, ir_discard *ir)
{
    ir_rvalue *condition = ir->condition;

    /* For unconditional discards, use "true" as the condition. */
    if (condition == NULL)
        condition = new(mem_ctx) ir_constant(true);

    ir_assignment *assignment =
            new(mem_ctx) ir_assignment(new(mem_ctx) ir_dereference_variable(var),
                                       condition);

    ir->replace_with(assignment);
}


ir_visitor_status
lower_discard_visitor::visit_leave(ir_if *ir)
{
    ir_discard *then_discard = find_discard(ir->then_instructions);
    ir_discard *else_discard = find_discard(ir->else_instructions);

    if (then_discard == NULL && else_discard == NULL)
        return visit_continue;

    void *mem_ctx = ralloc_parent(ir);

    ir_variable *temp = new(mem_ctx) ir_variable(glsl_type::bool_type,
                                                 "discard_cond_temp",
                                                 ir_var_temporary);
    ir_assignment *temp_initializer =
            new(mem_ctx) ir_assignment(new(mem_ctx) ir_dereference_variable(temp),
                                       new(mem_ctx) ir_constant(false));

    ir->insert_before(temp);
    ir->insert_before(temp_initializer);

    if (then_discard != NULL)
        replace_discard(mem_ctx, temp, then_discard);

    if (else_discard != NULL)
        replace_discard(mem_ctx, temp, else_discard);

    ir_discard *discard = then_discard != NULL ? then_discard : else_discard;
    discard->condition = new(mem_ctx) ir_dereference_variable(temp);
    ir->insert_after(discard);

    this->progress = true;

    return visit_continue;
}
