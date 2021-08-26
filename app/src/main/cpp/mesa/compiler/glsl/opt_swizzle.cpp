#include "ir.h"
#include "ir_visitor.h"
#include "ir_rvalue_visitor.h"
#include "../glsl_types.h"

namespace {

    class ir_opt_swizzle_visitor : public ir_rvalue_visitor {
    public:
        ir_opt_swizzle_visitor()
        {
            this->progress = false;
        }

        void handle_rvalue(ir_rvalue **rvalue);
        bool progress;
    };

} /* unnamed namespace */

void
ir_opt_swizzle_visitor::handle_rvalue(ir_rvalue **rvalue)
{
    if (!*rvalue)
        return;

    ir_swizzle *swiz = (*rvalue)->as_swizzle();

    if (!swiz)
        return;

    ir_swizzle *swiz2;

    while ((swiz2 = swiz->val->as_swizzle()) != NULL) {
        int mask2[4];

        memset(&mask2, 0, sizeof(mask2));
        if (swiz2->mask.num_components >= 1)
            mask2[0] = swiz2->mask.x;
        if (swiz2->mask.num_components >= 2)
            mask2[1] = swiz2->mask.y;
        if (swiz2->mask.num_components >= 3)
            mask2[2] = swiz2->mask.z;
        if (swiz2->mask.num_components >= 4)
            mask2[3] = swiz2->mask.w;

        if (swiz->mask.num_components >= 1)
            swiz->mask.x = mask2[swiz->mask.x];
        if (swiz->mask.num_components >= 2)
            swiz->mask.y = mask2[swiz->mask.y];
        if (swiz->mask.num_components >= 3)
            swiz->mask.z = mask2[swiz->mask.z];
        if (swiz->mask.num_components >= 4)
            swiz->mask.w = mask2[swiz->mask.w];

        swiz->val = swiz2->val;

        this->progress = true;
    }

    if (swiz->type != swiz->val->type)
        return;

    int elems = swiz->val->type->vector_elements;
    if (swiz->mask.x != 0)
        return;
    if (elems >= 2 && swiz->mask.y != 1)
        return;
    if (elems >= 3 && swiz->mask.z != 2)
        return;
    if (elems >= 4 && swiz->mask.w != 3)
        return;

    this->progress = true;
    *rvalue = swiz->val;
}

bool
optimize_swizzles(exec_list *instructions)
{
    ir_opt_swizzle_visitor v;
    visit_list_elements(&v, instructions);

    return v.progress;
}
