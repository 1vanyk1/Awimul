#include "../glsl_types.h"
#include "ir.h"
#include "ir_builder.h"
#include "ir_optimization.h"
#include "ir_rvalue_visitor.h"

using namespace ir_builder;

class lower_offset_array_visitor : public ir_rvalue_visitor {
public:
    lower_offset_array_visitor()
    {
        progress = false;
    }

    void handle_rvalue(ir_rvalue **rv);

    bool progress;
};

void
lower_offset_array_visitor::handle_rvalue(ir_rvalue **rv)
{
    if (*rv == NULL || (*rv)->ir_type != ir_type_texture)
        return;

    ir_texture *ir = (ir_texture *) *rv;
    if (ir->op != ir_tg4 || !ir->offset || !ir->offset->type->is_array())
        return;

    void *mem_ctx = ralloc_parent(ir);

    ir_variable *var =
            new (mem_ctx) ir_variable(ir->type, "result", ir_var_temporary);
    base_ir->insert_before(var);

    for (int i = 0; i < 4; i++) {
        ir_texture *tex = ir->clone(mem_ctx, NULL);
        tex->offset = new (mem_ctx) ir_dereference_array(tex->offset,
                                                         new (mem_ctx) ir_constant(i));

        base_ir->insert_before(assign(var, swizzle_w(tex), 1 << i));
    }

    *rv = new (mem_ctx) ir_dereference_variable(var);

    progress = true;
}

bool
lower_offset_arrays(exec_list *instructions)
{
    lower_offset_array_visitor v;

    visit_list_elements(&v, instructions);

    return v.progress;
}
