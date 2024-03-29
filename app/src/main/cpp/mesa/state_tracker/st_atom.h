#ifndef ST_ATOM_H
#define ST_ATOM_H

#include "../main/glheader.h"

#ifdef __cplusplus
extern "C" {
#endif

struct st_context;
struct st_vertex_program;
struct st_common_variant;
struct pipe_vertex_buffer;
struct pipe_vertex_element;
struct cso_velems_state;

/**
 * Enumeration of state tracker pipelines.
 */
enum st_pipeline {
    ST_PIPELINE_RENDER,
    ST_PIPELINE_RENDER_NO_VARRAYS,
    ST_PIPELINE_CLEAR,
    ST_PIPELINE_META,
    ST_PIPELINE_UPDATE_FRAMEBUFFER,
    ST_PIPELINE_COMPUTE,
};

void st_init_atoms( struct st_context *st );
void st_destroy_atoms( struct st_context *st );
void st_validate_state( struct st_context *st, enum st_pipeline pipeline );
void st_update_edgeflags(struct st_context *st, bool per_vertex_edgeflags);

void
st_setup_arrays(struct st_context *st,
                const struct st_vertex_program *vp,
                const struct st_common_variant *vp_variant,
                struct cso_velems_state *velements,
                struct pipe_vertex_buffer *vbuffer, unsigned *num_vbuffers,
                bool *has_user_vertex_buffers);

void
st_setup_current_user(struct st_context *st,
                      const struct st_vertex_program *vp,
                      const struct st_common_variant *vp_variant,
                      struct cso_velems_state *velements,
                      struct pipe_vertex_buffer *vbuffer, unsigned *num_vbuffers);

void
st_update_array_with_popcnt(struct st_context *st);

struct pipe_vertex_state *
st_create_gallium_vertex_state(struct gl_context *ctx,
                               const struct gl_vertex_array_object *vao,
                               struct gl_buffer_object *indexbuf,
                               uint32_t enabled_attribs);

/* Define ST_NEW_xxx_INDEX */
enum {
#define ST_STATE(FLAG, st_update) FLAG##_INDEX,
#include "st_atom_list.h"
#undef ST_STATE
    ST_NUM_ATOMS,
};

/* Define ST_NEW_xxx values as static const uint64_t values.
 * We can't use an enum type because MSVC doesn't allow 64-bit enum values.
 */
#define ST_STATE(FLAG, st_update) static const uint64_t FLAG = 1ull << FLAG##_INDEX;
#include "st_atom_list.h"
#undef ST_STATE

/* Declare function prototypes. */
#define ST_STATE(FLAG, st_update) void st_update(struct st_context *st);
#include "st_atom_list.h"
#undef ST_STATE

/* Combined state flags. */
#define ST_NEW_SAMPLERS         (ST_NEW_VS_SAMPLERS | \
                                 ST_NEW_TCS_SAMPLERS | \
                                 ST_NEW_TES_SAMPLERS | \
                                 ST_NEW_GS_SAMPLERS | \
                                 ST_NEW_FS_SAMPLERS | \
                                 ST_NEW_CS_SAMPLERS)

#define ST_NEW_FRAMEBUFFER      (ST_NEW_FB_STATE | \
                                 ST_NEW_SAMPLE_STATE | \
                                 ST_NEW_SAMPLE_SHADING)

#define ST_NEW_VERTEX_PROGRAM(st, p) ((p)->affected_states | \
                                      (st_user_clip_planes_enabled(st->ctx) ? \
                                       ST_NEW_CLIP_STATE : 0))

#define ST_NEW_CONSTANTS        (ST_NEW_VS_CONSTANTS | \
                                 ST_NEW_TCS_CONSTANTS | \
                                 ST_NEW_TES_CONSTANTS | \
                                 ST_NEW_FS_CONSTANTS | \
                                 ST_NEW_GS_CONSTANTS | \
                                 ST_NEW_CS_CONSTANTS)

#define ST_NEW_UNIFORM_BUFFER   (ST_NEW_VS_UBOS | \
                                 ST_NEW_TCS_UBOS | \
                                 ST_NEW_TES_UBOS | \
                                 ST_NEW_FS_UBOS | \
                                 ST_NEW_GS_UBOS | \
                                 ST_NEW_CS_UBOS)

#define ST_NEW_SAMPLER_VIEWS    (ST_NEW_VS_SAMPLER_VIEWS | \
                                 ST_NEW_FS_SAMPLER_VIEWS | \
                                 ST_NEW_GS_SAMPLER_VIEWS | \
                                 ST_NEW_TCS_SAMPLER_VIEWS | \
                                 ST_NEW_TES_SAMPLER_VIEWS | \
                                 ST_NEW_CS_SAMPLER_VIEWS)

#define ST_NEW_ATOMIC_BUFFER    (ST_NEW_VS_ATOMICS | \
                                 ST_NEW_TCS_ATOMICS | \
                                 ST_NEW_TES_ATOMICS | \
                                 ST_NEW_FS_ATOMICS | \
                                 ST_NEW_GS_ATOMICS | \
                                 ST_NEW_CS_ATOMICS)

#define ST_NEW_STORAGE_BUFFER   (ST_NEW_VS_SSBOS | \
                                 ST_NEW_TCS_SSBOS | \
                                 ST_NEW_TES_SSBOS | \
                                 ST_NEW_FS_SSBOS | \
                                 ST_NEW_GS_SSBOS | \
                                 ST_NEW_CS_SSBOS)

#define ST_NEW_IMAGE_UNITS      (ST_NEW_VS_IMAGES | \
                                 ST_NEW_TCS_IMAGES | \
                                 ST_NEW_TES_IMAGES | \
                                 ST_NEW_GS_IMAGES | \
                                 ST_NEW_FS_IMAGES | \
                                 ST_NEW_CS_IMAGES)

#define ST_ALL_SHADER_RESOURCES (ST_NEW_SAMPLER_VIEWS | \
                                 ST_NEW_SAMPLERS | \
                                 ST_NEW_CONSTANTS | \
                                 ST_NEW_UNIFORM_BUFFER | \
                                 ST_NEW_ATOMIC_BUFFER | \
                                 ST_NEW_STORAGE_BUFFER | \
                                 ST_NEW_IMAGE_UNITS)

/* All state flags within each group: */
#define ST_PIPELINE_RENDER_STATE_MASK  (ST_NEW_CS_STATE - 1)
#define ST_PIPELINE_RENDER_STATE_MASK_NO_VARRAYS \
   (ST_PIPELINE_RENDER_STATE_MASK & ~ST_NEW_VERTEX_ARRAYS)
#define ST_PIPELINE_COMPUTE_STATE_MASK (0xffull << ST_NEW_CS_STATE_INDEX)
#define ST_PIPELINE_CLEAR_STATE_MASK (ST_NEW_FB_STATE | \
                                      ST_NEW_SCISSOR | \
                                      ST_NEW_WINDOW_RECTANGLES)
#define ST_PIPELINE_META_STATE_MASK ST_PIPELINE_RENDER_STATE_MASK_NO_VARRAYS
/* For ReadPixels, ReadBuffer, GetSamplePosition: */
#define ST_PIPELINE_UPDATE_FB_STATE_MASK (ST_NEW_FB_STATE)

#define ST_ALL_STATES_MASK (ST_PIPELINE_RENDER_STATE_MASK | \
                            ST_PIPELINE_COMPUTE_STATE_MASK)

#ifdef __cplusplus
}
#endif

#endif
