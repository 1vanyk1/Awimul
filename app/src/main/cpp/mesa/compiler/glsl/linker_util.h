#ifndef GLSL_LINKER_UTIL_H
#define GLSL_LINKER_UTIL_H

#include "../../util/bitset.h"

struct gl_context;
struct gl_shader_program;
struct gl_uniform_storage;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Sometimes there are empty slots left over in UniformRemapTable after we
 * allocate slots to explicit locations. This struct represents a single
 * continouous block of empty slots in UniformRemapTable.
 */
struct empty_uniform_block {
    struct exec_node link;
    /* The start location of the block */
    unsigned start;
    /* The number of slots in the block */
    unsigned slots;
};

/**
 * Describes an access of an array element or an access of the whole array
 */
struct array_deref_range {
    /**
     * Index that was accessed.
     *
     * All valid array indices are less than the size of the array.  If index
     * is equal to the size of the array, this means the entire array has been
     * accessed (e.g., due to use of a non-constant index).
     */
    unsigned index;

    /** Size of the array.  Used for offset calculations. */
    unsigned size;
};

void
linker_error(struct gl_shader_program *prog, const char *fmt, ...);

void
linker_warning(struct gl_shader_program *prog, const char *fmt, ...);

bool
link_util_should_add_buffer_variable(struct gl_shader_program *prog,
                                     struct gl_uniform_storage *uniform,
                                     int top_level_array_base_offset,
                                     int top_level_array_size_in_bytes,
                                     int second_element_offset,
                                     int block_index);

bool
link_util_add_program_resource(struct gl_shader_program *prog,
                               struct set *resource_set,
                               GLenum type, const void *data, uint8_t stages);

int
link_util_find_empty_block(struct gl_shader_program *prog,
                           struct gl_uniform_storage *uniform);

void
link_util_update_empty_uniform_locations(struct gl_shader_program *prog);

void
link_util_check_subroutine_resources(struct gl_shader_program *prog);

void
link_util_check_uniform_resources(struct gl_context *ctx,
                                  struct gl_shader_program *prog);

void
link_util_calculate_subroutine_compat(struct gl_shader_program *prog);

void
link_util_mark_array_elements_referenced(const struct array_deref_range *dr,
                                         unsigned count, unsigned array_depth,
                                         BITSET_WORD *bits);

#ifdef __cplusplus
}
#endif

#endif /* GLSL_LINKER_UTIL_H */
