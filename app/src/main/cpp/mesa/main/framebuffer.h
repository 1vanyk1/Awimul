#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "mtypes.h"

struct gl_config;
struct gl_context;
struct gl_renderbuffer;

extern struct gl_framebuffer *
_mesa_new_framebuffer(struct gl_context *ctx, GLuint name);

extern void
_mesa_initialize_window_framebuffer(struct gl_framebuffer *fb,
                                    const struct gl_config *visual);

extern void
_mesa_initialize_user_framebuffer(struct gl_framebuffer *fb, GLuint name);

extern void
_mesa_destroy_framebuffer(struct gl_framebuffer *buffer);

extern void
_mesa_free_framebuffer_data(struct gl_framebuffer *buffer);

extern void
_mesa_reference_framebuffer_(struct gl_framebuffer **ptr,
                             struct gl_framebuffer *fb);

static inline void
_mesa_reference_framebuffer(struct gl_framebuffer **ptr,
                            struct gl_framebuffer *fb)
{
    if (*ptr != fb)
        _mesa_reference_framebuffer_(ptr, fb);
}

extern void
_mesa_resize_framebuffer(struct gl_context *ctx, struct gl_framebuffer *fb,
                         GLuint width, GLuint height);


extern void
_mesa_resizebuffers( struct gl_context *ctx );

extern void
_mesa_intersect_scissor_bounding_box(const struct gl_context *ctx,
                                     unsigned idx, int *bbox);

static inline GLuint
_mesa_geometric_width(const struct gl_framebuffer *buffer)
{
    return buffer->_HasAttachments ?
           buffer->Width : buffer->DefaultGeometry.Width;
}

static inline GLuint
_mesa_geometric_height(const struct gl_framebuffer *buffer)
{
    return buffer->_HasAttachments ?
           buffer->Height : buffer->DefaultGeometry.Height;
}

static inline GLuint
_mesa_geometric_samples(const struct gl_framebuffer *buffer)
{
    return buffer->_HasAttachments ?
           buffer->Visual.samples :
           buffer->DefaultGeometry._NumSamples;
}

static inline GLuint
_mesa_geometric_layers(const struct gl_framebuffer *buffer)
{
    return buffer->_HasAttachments ?
           buffer->MaxNumLayers : buffer->DefaultGeometry.Layers;
}

extern void
_mesa_update_draw_buffer_bounds(struct gl_context *ctx,
                                struct gl_framebuffer *drawFb);

extern void
_mesa_update_framebuffer_visual(struct gl_context *ctx,
                                struct gl_framebuffer *fb);

extern void
_mesa_update_framebuffer(struct gl_context *ctx,
                         struct gl_framebuffer *readFb,
                         struct gl_framebuffer *drawFb);

extern GLboolean
_mesa_source_buffer_exists(struct gl_context *ctx, GLenum format);

extern GLboolean
_mesa_dest_buffer_exists(struct gl_context *ctx, GLenum format);

extern GLenum
_mesa_get_color_read_type(struct gl_context *ctx,
                          struct gl_framebuffer *fb,
                          const char *caller);

extern GLenum
_mesa_get_color_read_format(struct gl_context *ctx,
                            struct gl_framebuffer *fb,
                            const char *caller);

extern struct gl_renderbuffer *
_mesa_get_read_renderbuffer_for_format(const struct gl_context *ctx,
                                       GLenum format);

extern void
_mesa_print_framebuffer(const struct gl_framebuffer *fb);

extern bool
_mesa_is_front_buffer_reading(const struct gl_framebuffer *fb);

extern bool
_mesa_is_front_buffer_drawing(const struct gl_framebuffer *fb);

extern bool
_mesa_is_multisample_enabled(const struct gl_context *ctx);

extern bool
_mesa_is_alpha_test_enabled(const struct gl_context *ctx);

extern bool
_mesa_is_alpha_to_coverage_enabled(const struct gl_context *ctx);

#endif /* FRAMEBUFFER_H */
