#ifndef NIR_LOOP_ANALYZE_H
#define NIR_LOOP_ANALYZE_H

#include "nir.h"

/* Returns true if nir_cf_node contains a jump other than the expected_jump
 * parameter.
 */
static inline bool
contains_other_jump(nir_cf_node *node, nir_instr *expected_jump)
{
    switch (node->type) {
        case nir_cf_node_block: {
            nir_instr *lst_instr = nir_block_last_instr(nir_cf_node_as_block(node));

            /* dead_cf should have eliminated any instruction after the first break
             */
            nir_foreach_instr(instr, nir_cf_node_as_block(node))
                assert(instr->type != nir_instr_type_jump || instr == lst_instr);

            if (lst_instr && lst_instr->type == nir_instr_type_jump &&
                lst_instr != expected_jump)
                return true;
            else
                return false;
        }
        case nir_cf_node_if: {
            nir_if *if_stmt = nir_cf_node_as_if(node);

            foreach_list_typed_safe(nir_cf_node, node, node, &if_stmt->then_list) {
                if (contains_other_jump(node, expected_jump))
                    return true;
            }

            foreach_list_typed_safe(nir_cf_node, node, node, &if_stmt->else_list) {
                if (contains_other_jump(node, expected_jump))
                    return true;
            }

            return false;
        }
        case nir_cf_node_loop:
            return true;

        default:
            unreachable("Unhandled cf node type");
    }
}

/* Here we define a trivial if as containing only a single break that must be
 * located at the end of either the then or else branch of the top level if,
 * there must be no other breaks or any other type of jump.  Or we pass NULL
 * to break_block the if must contains no jumps at all.
 */
static inline bool
nir_is_trivial_loop_if(nir_if *nif, nir_block *break_block)
{
    nir_instr *last_instr = NULL;

    if (break_block) {
        last_instr = nir_block_last_instr(break_block);
        assert(last_instr && last_instr->type == nir_instr_type_jump &&
               nir_instr_as_jump(last_instr)->type == nir_jump_break);
    }

    if (contains_other_jump(&nif->cf_node, last_instr))
        return false;

    return true;
}

static inline bool
nir_block_ends_in_break(nir_block *block)
{
    if (exec_list_is_empty(&block->instr_list))
        return false;

    nir_instr *instr = nir_block_last_instr(block);
    return instr->type == nir_instr_type_jump &&
           nir_instr_as_jump(instr)->type == nir_jump_break;
}

#endif /* NIR_LOOP_ANALYZE_H */
