#ifndef U_DEBUG_DUMP_H_
#define U_DEBUG_DUMP_H_


#include "../include/pipe/p_compiler.h"
#include "../include/pipe/p_state.h"

#include <stdio.h>


#ifdef	__cplusplus
extern "C" {
#endif


#define UTIL_DUMP_INVALID_NAME "<invalid>"


/*
 * p_defines.h
 */

const char *
util_str_blend_factor(unsigned value, boolean shortened);

const char *
util_str_blend_func(unsigned value, boolean shortened);

const char *
util_str_logicop(unsigned value, boolean shortened);

const char *
util_str_func(unsigned value, boolean shortened);

const char *
util_str_stencil_op(unsigned value, boolean shortened);

const char *
util_str_tex_target(unsigned value, boolean shortened);

const char *
util_str_tex_wrap(unsigned value, boolean shortened);

const char *
util_str_tex_mipfilter(unsigned value, boolean shortened);

const char *
util_str_tex_filter(unsigned value, boolean shortened);

const char *
util_str_query_type(unsigned value, boolean shortened);

const char *
util_str_query_value_type(unsigned value, boolean shortened);

const char *
util_str_prim_mode(unsigned value, boolean shortened);

void
util_dump_ns(FILE *f, uint64_t time);

void
util_dump_ptr(FILE *stream, const void *value);

void
util_dump_query_type(FILE *stream, unsigned value);

void
util_dump_query_value_type(FILE *stream, unsigned value);

void
util_dump_transfer_usage(FILE *stream, unsigned value);

/*
 * p_state.h, through a FILE
 */

void
util_dump_resource(FILE *stream, const struct pipe_resource *state);

void
util_dump_rasterizer_state(FILE *stream,
                           const struct pipe_rasterizer_state *state);

void
util_dump_poly_stipple(FILE *stream,
                       const struct pipe_poly_stipple *state);

void
util_dump_viewport_state(FILE *stream,
                         const struct pipe_viewport_state *state);

void
util_dump_scissor_state(FILE *stream,
                        const struct pipe_scissor_state *state);

void
util_dump_clip_state(FILE *stream,
                     const struct pipe_clip_state *state);

void
util_dump_shader_state(FILE *stream,
                       const struct pipe_shader_state *state);

void
util_dump_depth_stencil_alpha_state(FILE *stream,
                                    const struct pipe_depth_stencil_alpha_state *state);

void
util_dump_rt_blend_state(FILE *stream,
                         const struct pipe_rt_blend_state *state);

void
util_dump_blend_state(FILE *stream,
                      const struct pipe_blend_state *state);

void
util_dump_blend_color(FILE *stream,
                      const struct pipe_blend_color *state);

void
util_dump_stencil_ref(FILE *stream,
                      const struct pipe_stencil_ref *state);

void
util_dump_framebuffer_state(FILE *stream,
                            const struct pipe_framebuffer_state *state);

void
util_dump_sampler_state(FILE *stream,
                        const struct pipe_sampler_state *state);

void
util_dump_surface(FILE *stream,
                  const struct pipe_surface *state);

void
util_dump_image_view(FILE *stream, const struct pipe_image_view *state);

void
util_dump_shader_buffer(FILE *stream, const struct pipe_shader_buffer *state);

void
util_dump_sampler_view(FILE *stream, const struct pipe_sampler_view *state);

void
util_dump_transfer(FILE *stream,
                   const struct pipe_transfer *state);

void
util_dump_constant_buffer(FILE *stream,
                          const struct pipe_constant_buffer *state);

void
util_dump_vertex_buffer(FILE *stream,
                        const struct pipe_vertex_buffer *state);

void
util_dump_vertex_element(FILE *stream,
                         const struct pipe_vertex_element *state);

void
util_dump_stream_output_target(FILE *stream,
                               const struct pipe_stream_output_target *state);

void
util_dump_draw_info(FILE *stream, const struct pipe_draw_info *state);

void
util_dump_draw_start_count(FILE *stream, const struct pipe_draw_start_count *state);

void
util_dump_draw_indirect_info(FILE *stream,
                             const struct pipe_draw_indirect_info *indirect);

void
util_dump_grid_info(FILE *stream, const struct pipe_grid_info *state);

void
util_dump_box(FILE *stream, const struct pipe_box *box);

void
util_dump_blit_info(FILE *stream, const struct pipe_blit_info *info);

/* FIXME: Move the other debug_dump_xxx functions out of u_debug.h into here. */


#ifdef	__cplusplus
}
#endif

#endif /* U_DEBUG_H_ */
