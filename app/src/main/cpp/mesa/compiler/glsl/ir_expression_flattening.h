#ifndef GLSL_IR_EXPRESSION_FLATTENING_H
#define GLSL_IR_EXPRESSION_FLATTENING_H

void do_expression_flattening(exec_list *instructions,
                              bool (*predicate)(ir_instruction *ir));

#endif /* GLSL_IR_EXPRESSION_FLATTENING_H */
