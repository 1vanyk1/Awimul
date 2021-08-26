#include "../glsl_types.h"
#include "ir.h"

namespace {

    class lower_discard_flow_visitor : public ir_hierarchical_visitor {
    public:
        lower_discard_flow_visitor(ir_variable *discarded)
                : discarded(discarded)
        {
            mem_ctx = ralloc_parent(discarded);
        }

        ~lower_discard_flow_visitor()
        {
        }

        ir_visitor_status visit(ir_loop_jump *ir);
        ir_visitor_status visit_enter(ir_discard *ir);
        ir_visitor_status visit_enter(ir_loop *ir);
        ir_visitor_status visit_enter(ir_function_signature *ir);

        ir_if *generate_discard_break();

        ir_variable *discarded;
        void *mem_ctx;
    };

} /* anonymous namespace */

ir_visitor_status
lower_discard_flow_visitor::visit(ir_loop_jump *ir)
{
    if (ir->mode != ir_loop_jump::jump_continue)
        return visit_continue;

    ir->insert_before(generate_discard_break());

    return visit_continue;
}

ir_visitor_status
lower_discard_flow_visitor::visit_enter(ir_discard *ir)
{
    ir_dereference *lhs = new(mem_ctx) ir_dereference_variable(discarded);
    ir_rvalue *rhs;
    if (ir->condition) {
        /* discarded <- condition, use (var_ref discarded) as the condition */
        rhs = ir->condition;
        ir->condition = new(mem_ctx) ir_dereference_variable(discarded);
    } else {
        rhs = new(mem_ctx) ir_constant(true);
    }
    ir_assignment *assign = new(mem_ctx) ir_assignment(lhs, rhs);
    ir->insert_before(assign);

    return visit_continue;
}

ir_visitor_status
lower_discard_flow_visitor::visit_enter(ir_loop *ir)
{
    ir->body_instructions.push_tail(generate_discard_break());

    return visit_continue;
}

ir_visitor_status
lower_discard_flow_visitor::visit_enter(ir_function_signature *ir)
{
    if (strcmp(ir->function_name(), "main") != 0)
        return visit_continue;

    ir_dereference *lhs = new(mem_ctx) ir_dereference_variable(discarded);
    ir_rvalue *rhs = new(mem_ctx) ir_constant(false);
    ir_assignment *assign = new(mem_ctx) ir_assignment(lhs, rhs);
    ir->body.push_head(assign);

    return visit_continue;
}

ir_if *
lower_discard_flow_visitor::generate_discard_break()
{
    ir_rvalue *if_condition = new(mem_ctx) ir_dereference_variable(discarded);
    ir_if *if_inst = new(mem_ctx) ir_if(if_condition);

    ir_instruction *br = new(mem_ctx) ir_loop_jump(ir_loop_jump::jump_break);
    if_inst->then_instructions.push_tail(br);

    return if_inst;
}

void
lower_discard_flow(exec_list *ir)
{
    void *mem_ctx = ir;

    ir_variable *var = new(mem_ctx) ir_variable(glsl_type::bool_type,
                                                "discarded",
                                                ir_var_temporary);

    ir->push_head(var);

    lower_discard_flow_visitor v(var);

    visit_list_elements(&v, ir);
}
