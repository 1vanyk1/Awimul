#ifndef U_DRAW_H
#define U_DRAW_H


#include "../include/pipe/p_compiler.h"
#include "../include/pipe/p_context.h"
#include "../include/pipe/p_state.h"


#ifdef __cplusplus
extern "C" {
#endif


static inline void
util_draw_init_info(struct pipe_draw_info *info)
{
    memset(info, 0, sizeof(*info));
    info->instance_count = 1;
    info->max_index = 0xffffffff;
}


static inline void
util_draw_arrays(struct pipe_context *pipe,
                 enum pipe_prim_type mode,
                 uint start,
                 uint count)
{
    struct pipe_draw_info info;
    struct pipe_draw_start_count draw;

    util_draw_init_info(&info);
    info.mode = mode;
    info.min_index = start;
    info.max_index = start + count - 1;

    draw.start = start;
    draw.count = count;

    pipe->draw_vbo(pipe, &info, NULL, &draw, 1);
}

static inline void
util_draw_elements(struct pipe_context *pipe,
                   void *indices,
                   unsigned index_size,
                   int index_bias, enum pipe_prim_type mode,
                   uint start,
                   uint count)
{
    struct pipe_draw_info info;
    struct pipe_draw_start_count draw;

    util_draw_init_info(&info);
    info.index.user = indices;
    info.has_user_indices = true;
    info.index_size = index_size;
    info.mode = mode;
    info.index_bias = index_bias;

    draw.start = start;
    draw.count = count;

    pipe->draw_vbo(pipe, &info, NULL, &draw, 1);
}

static inline void
util_draw_arrays_instanced(struct pipe_context *pipe,
                           enum pipe_prim_type mode,
                           uint start,
                           uint count,
                           uint start_instance,
                           uint instance_count)
{
    struct pipe_draw_info info;
    struct pipe_draw_start_count draw;

    util_draw_init_info(&info);
    info.mode = mode;
    info.start_instance = start_instance;
    info.instance_count = instance_count;
    info.index_bounds_valid = true;
    info.min_index = start;
    info.max_index = start + count - 1;

    draw.start = start;
    draw.count = count;

    pipe->draw_vbo(pipe, &info, NULL, &draw, 1);
}

static inline void
util_draw_elements_instanced(struct pipe_context *pipe,
                             void *indices,
                             unsigned index_size,
                             int index_bias,
                             enum pipe_prim_type mode,
                             uint start,
                             uint count,
                             uint start_instance,
                             uint instance_count)
{
    struct pipe_draw_info info;
    struct pipe_draw_start_count draw;

    util_draw_init_info(&info);
    info.index.user = indices;
    info.has_user_indices = true;
    info.index_size = index_size;
    info.mode = mode;
    info.index_bias = index_bias;
    info.start_instance = start_instance;
    info.instance_count = instance_count;

    draw.start = start;
    draw.count = count;

    pipe->draw_vbo(pipe, &info, NULL, &draw, 1);
}


/* This converts an indirect draw into a direct draw by mapping the indirect
 * buffer, extracting its arguments, and calling pipe->draw_vbo.
 */
void
util_draw_indirect(struct pipe_context *pipe,
                   const struct pipe_draw_info *info,
                   const struct pipe_draw_indirect_info *indirect);

/* Helper to handle multi-draw by splitting into individual draws.  You
 * don't want to call this if num_draws==1
 */
void
util_draw_multi(struct pipe_context *pctx, const struct pipe_draw_info *info,
                const struct pipe_draw_indirect_info *indirect,
                const struct pipe_draw_start_count *draws,
                unsigned num_draws);

unsigned
util_draw_max_index(
        const struct pipe_vertex_buffer *vertex_buffers,
        const struct pipe_vertex_element *vertex_elements,
        unsigned nr_vertex_elements,
        const struct pipe_draw_info *info);


#ifdef __cplusplus
}
#endif

#endif /* !U_DRAW_H */
