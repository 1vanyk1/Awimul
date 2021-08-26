#ifndef NIR_CONTROL_FLOW_PRIVATE_H
#define NIR_CONTROL_FLOW_PRIVATE_H

#include "nir_control_flow.h"


/* Internal control-flow modification functions used when inserting/removing
 * instructions.
 */

void nir_handle_add_jump(nir_block *block);
void nir_handle_remove_jump(nir_block *block, nir_jump_type type);

#endif /* NIR_CONTROL_FLOW_PRIVATE_H */
