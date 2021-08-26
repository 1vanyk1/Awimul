#ifndef NIR_DEREF_H
#define NIR_DEREF_H

#include "nir.h"
#include "nir_builder.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    /** Short path so we can keep it on the stack most of the time. */
    nir_deref_instr *_short_path[7];

    /** A null-terminated array view of a deref chain
     *
     * The first element of this array will be the variable dereference
     * followed by every deref_instr on the path to the final one.  The last
     * element in the array is a NULL pointer which acts as a terminator.
     */
    nir_deref_instr **path;
} nir_deref_path;

typedef struct {
    nir_deref_instr *instr;
    nir_deref_path *_path;
} nir_deref_and_path;

void nir_deref_path_init(nir_deref_path *path,
                         nir_deref_instr *deref, void *mem_ctx);
void nir_deref_path_finish(nir_deref_path *path);

unsigned nir_deref_instr_get_const_offset(nir_deref_instr *deref,
                                          glsl_type_size_align_func size_align);

nir_ssa_def *nir_build_deref_offset(nir_builder *b, nir_deref_instr *deref,
                                    glsl_type_size_align_func size_align);

nir_deref_path *nir_get_deref_path(void *mem_ctx, nir_deref_and_path *deref);

typedef enum {
    nir_derefs_do_not_alias     = 0,
    nir_derefs_equal_bit        = (1 << 0),
    nir_derefs_may_alias_bit    = (1 << 1),
    nir_derefs_a_contains_b_bit = (1 << 2),
    nir_derefs_b_contains_a_bit = (1 << 3),
} nir_deref_compare_result;

nir_deref_compare_result nir_compare_deref_paths(nir_deref_path *a_path, nir_deref_path *b_path);
nir_deref_compare_result nir_compare_derefs(nir_deref_instr *a, nir_deref_instr *b);
nir_deref_compare_result nir_compare_derefs_and_paths(void *mem_ctx,
                                                      nir_deref_and_path *a,
                                                      nir_deref_and_path *b);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NIR_DEREF_H */
