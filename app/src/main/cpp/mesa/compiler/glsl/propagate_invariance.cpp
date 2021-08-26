#include "ir.h"
#include "ir_visitor.h"
#include "ir_rvalue_visitor.h"
#include "ir_optimization.h"
#include "../glsl_types.h"

namespace {

    class ir_invariance_propagation_visitor : public ir_hierarchical_visitor {
    public:
        ir_invariance_propagation_visitor()
        {
            this->progress = false;
            this->dst_var = NULL;
        }

        virtual ~ir_invariance_propagation_visitor()
        {
            /* empty */
        }

        virtual ir_visitor_status visit_enter(ir_assignment *ir);
        virtual ir_visitor_status visit_leave(ir_assignment *ir);
        virtual ir_visitor_status visit(ir_dereference_variable *ir);

        ir_variable *dst_var;
        bool progress;
    };

} /* unnamed namespace */

ir_visitor_status
ir_invariance_propagation_visitor::visit_enter(ir_assignment *ir)
{
    assert(this->dst_var == NULL);
    ir_variable *var = ir->lhs->variable_referenced();
    if (var->data.invariant || var->data.precise) {
        this->dst_var = var;
        return visit_continue;
    } else {
        return visit_continue_with_parent;
    }
}

ir_visitor_status
ir_invariance_propagation_visitor::visit_leave(ir_assignment *)
{
    this->dst_var = NULL;

    return visit_continue;
}

ir_visitor_status
ir_invariance_propagation_visitor::visit(ir_dereference_variable *ir)
{
    if (this->dst_var == NULL)
        return visit_continue;

    if (this->dst_var->data.invariant) {
        if (!ir->var->data.invariant)
            this->progress = true;

        ir->var->data.invariant = true;
    }

    if (this->dst_var->data.precise) {
        if (!ir->var->data.precise)
            this->progress = true;

        ir->var->data.precise = true;
    }

    return visit_continue;
}

void
propagate_invariance(exec_list *instructions)
{
    ir_invariance_propagation_visitor visitor;

    do {
        visitor.progress = false;
        visit_list_elements(&visitor, instructions);
    } while (visitor.progress);
}
