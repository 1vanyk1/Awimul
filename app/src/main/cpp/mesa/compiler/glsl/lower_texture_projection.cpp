#include "ir.h"

namespace {

    class lower_texture_projection_visitor : public ir_hierarchical_visitor {
    public:
        lower_texture_projection_visitor()
        {
            progress = false;
        }

        ir_visitor_status visit_leave(ir_texture *ir);

        bool progress;
    };

} /* anonymous namespace */

ir_visitor_status
lower_texture_projection_visitor::visit_leave(ir_texture *ir)
{
    if (!ir->projector)
        return visit_continue;

    void *mem_ctx = ralloc_parent(ir);

    ir_variable *var = new(mem_ctx) ir_variable(ir->projector->type,
                                                "projector", ir_var_temporary);
    base_ir->insert_before(var);
    ir_dereference *deref = new(mem_ctx) ir_dereference_variable(var);
    ir_expression *expr = new(mem_ctx) ir_expression(ir_unop_rcp,
                                                     ir->projector->type,
                                                     ir->projector,
                                                     NULL);
    ir_assignment *assign = new(mem_ctx) ir_assignment(deref, expr);
    base_ir->insert_before(assign);

    deref = new(mem_ctx) ir_dereference_variable(var);
    ir->coordinate = new(mem_ctx) ir_expression(ir_binop_mul,
                                                ir->coordinate->type,
                                                ir->coordinate,
                                                deref);

    if (ir->shadow_comparator) {
        deref = new(mem_ctx) ir_dereference_variable(var);
        ir->shadow_comparator = new(mem_ctx) ir_expression(ir_binop_mul,
                                                           ir->shadow_comparator->type,
                                                           ir->shadow_comparator,
                                                           deref);
    }

    ir->projector = NULL;

    progress = true;
    return visit_continue;
}

bool
do_lower_texture_projection(exec_list *instructions)
{
    lower_texture_projection_visitor v;

    visit_list_elements(&v, instructions);

    return v.progress;
}
