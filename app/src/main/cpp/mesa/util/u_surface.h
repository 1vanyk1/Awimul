#ifndef U_SURFACE_H
#define U_SURFACE_H


#include "../include/pipe/p_compiler.h"
#include "../include/pipe/p_state.h"

#include "u_pack_color.h"


#ifdef __cplusplus
extern "C" {
#endif


extern void
u_surface_default_template(struct pipe_surface *view,
                           const struct pipe_resource *texture);

extern void
util_copy_box(ubyte * dst,
              enum pipe_format format,
              unsigned dst_stride, unsigned dst_slice_stride,
              unsigned dst_x, unsigned dst_y, unsigned dst_z,
              unsigned width, unsigned height, unsigned depth,
              const ubyte * src,
              int src_stride, unsigned src_slice_stride,
              unsigned src_x, unsigned src_y, unsigned src_z);

extern void
util_fill_rect(ubyte * dst, enum pipe_format format,
               unsigned dst_stride, unsigned dst_x, unsigned dst_y,
               unsigned width, unsigned height, union util_color *uc);

extern void
util_fill_box(ubyte * dst, enum pipe_format format,
              unsigned stride, unsigned layer_stride,
              unsigned x, unsigned y, unsigned z,
              unsigned width, unsigned height, unsigned depth,
              union util_color *uc);

extern void
util_fill_zs_box(ubyte *dst, enum pipe_format format,
                 bool need_rmw, unsigned clear_flags, unsigned stride,
                 unsigned layer_stride, unsigned width,
                 unsigned height, unsigned depth,
                 uint64_t zstencil);

extern void
util_resource_copy_region(struct pipe_context *pipe,
                          struct pipe_resource *dst,
                          unsigned dst_level,
                          unsigned dst_x, unsigned dst_y, unsigned dst_z,
                          struct pipe_resource *src,
                          unsigned src_level,
                          const struct pipe_box *src_box);

extern void
util_clear_texture(struct pipe_context *pipe,
                   struct pipe_resource *tex,
                   unsigned level,
                   const struct pipe_box *box,
                   const void *data);

extern void
util_clear_render_target(struct pipe_context *pipe,
                         struct pipe_surface *dst,
                         const union pipe_color_union *color,
                         unsigned dstx, unsigned dsty,
                         unsigned width, unsigned height);

extern void
util_clear_depth_stencil(struct pipe_context *pipe,
                         struct pipe_surface *dst,
                         unsigned clear_flags,
                         double depth,
                         unsigned stencil,
                         unsigned dstx, unsigned dsty,
                         unsigned width, unsigned height);

bool
util_can_blit_via_copy_region(const struct pipe_blit_info *blit,
                              bool tight_format_check,
                              bool render_condition_bound);

extern bool
util_try_blit_via_copy_region(struct pipe_context *ctx,
                              const struct pipe_blit_info *blit,
                              bool render_condition_bound);


#ifdef __cplusplus
}
#endif


#endif /* U_SURFACE_H */
