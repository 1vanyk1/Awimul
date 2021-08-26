#include "ir.h"
#include "ir_optimization.h"

namespace {

    class lower_builtins_visitor : public ir_hierarchical_visitor {
    public:
        lower_builtins_visitor() : progress(false) { }
        ir_visitor_status visit_leave(ir_call *);
        bool progress;
    };

}

bool
lower_builtins(exec_list *instructions)
{
    lower_builtins_visitor v;
    visit_list_elements(&v, instructions);
    return v.progress;
}

ir_visitor_status
lower_builtins_visitor::visit_leave(ir_call *ir)
{
    /* lower_precision() also inlines some intrinsics, which can't be inlined
     * further.
     */
    if (!ir->callee->is_builtin() || ir->callee->is_intrinsic())
        return visit_continue;

    ir->generate_inline(ir);
    ir->remove();

    this->progress = true;

    return visit_continue;
}
