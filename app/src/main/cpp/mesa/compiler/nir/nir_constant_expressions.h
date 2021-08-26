#ifndef NIR_CONSTANT_EXPRESSIONS_H
#define NIR_CONSTANT_EXPRESSIONS_H

#include "nir.h"

void nir_eval_const_opcode(nir_op op, nir_const_value *dest,
                           unsigned num_components, unsigned bit_size,
                           nir_const_value **src,
                           unsigned float_controls_execution_mode);

#endif /* NIR_CONSTANT_EXPRESSIONS_H */
