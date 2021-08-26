#ifndef OPT_ADD_NEG_TO_SUB_H
#define OPT_ADD_NEG_TO_SUB_H

#include "ir.h"
#include "ir_hierarchical_visitor.h"

class add_neg_to_sub_visitor : public ir_hierarchical_visitor {
public:
    add_neg_to_sub_visitor()
    {
        /* empty */
    }

    ir_visitor_status visit_leave(ir_expression *ir)
    {
        if (ir->operation != ir_binop_add)
            return visit_continue;

        for (unsigned i = 0; i < 2; i++) {
            ir_expression *const op = ir->operands[i]->as_expression();

            if (op != NULL && op->operation == ir_unop_neg) {
                ir->operation = ir_binop_sub;

                /* This ensures that -a + b becomes b - a. */
                if (i == 0)
                    ir->operands[0] = ir->operands[1];

                ir->operands[1] = op->operands[0];
                break;
            }
        }

        return visit_continue;
    }
};

#endif /* OPT_ADD_NEG_TO_SUB_H */
