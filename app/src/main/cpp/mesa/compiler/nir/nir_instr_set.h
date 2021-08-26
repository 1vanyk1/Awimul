#ifndef NIR_INSTR_SET_H
#define NIR_INSTR_SET_H

#include "nir.h"

/**
 * This file defines functions for creating, destroying, and manipulating an
 * "instruction set," which is an abstraction for finding duplicate
 * instructions using a hash set. Note that the question of whether an
 * instruction is actually a duplicate (e.g. whether it has any side effects)
 * is handled transparently. The user can pass any instruction to
 * nir_instr_set_add_or_rewrite() and nir_instr_set_remove(), and if the
 * instruction isn't safe to rewrite or isn't supported, it's silently
 * removed.
 */

/*@{*/

/** Creates an instruction set, using a given ralloc mem_ctx */
struct set *nir_instr_set_create(void *mem_ctx);

/** Destroys an instruction set. */
void nir_instr_set_destroy(struct set *instr_set);

/**
 * Adds an instruction to an instruction set if it doesn't exist, or if it
 * does already exist, rewrites all uses of it to point to the other
 * already-inserted instruction. Returns 'true' if the uses of the instruction
 * were rewritten.
 */
bool nir_instr_set_add_or_rewrite(struct set *instr_set, nir_instr *instr);

/**
 * Removes an instruction from an instruction set, so that other instructions
 * won't be merged with it.
 */
void nir_instr_set_remove(struct set *instr_set, nir_instr *instr);

/*@}*/

#endif /* NIR_INSTR_SET_H */
