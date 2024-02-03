#ifndef ST_CB_FBO_H
#define ST_CB_FBO_H

#include "../main/glheader.h"
#include "../main/mtypes.h"
#include "../main/fbobject.h"

#include "../include/pipe/p_compiler.h"
#include "../include/pipe/p_format.h"

struct dd_function_table;
struct pipe_context;

/**
 * Derived renderbuffer class.  Just need to add a pointer to the
 * pipe surface.
 */
struct st_renderbuffer
{
    struct gl_renderbuffer Base;
    struct pipe_resource *texture;
    /* This points to either "surface_linear" or "surface_srgb".
     * It doesn't hold the pipe_surface reference. The other two do.
     */
    struct pipe_surface *surface;
    struct pipe_surface *surface_linear;
    struct pipe_surface *surface_srgb;
    GLboolean defined;        /**< defined contents? */

    struct pipe_transfer *transfer; /**< only used when mapping the resource */

    /**
     * Used only when hardware accumulation buffers are not supported.
     */
    boolean software;
    void *data;

    bool use_readpix_cache;

    /* Inputs from Driver.RenderTexture, don't use directly. */
    boolean is_rtt; /**< whether Driver.RenderTexture was called */
    unsigned rtt_face, rtt_slice;
    boolean rtt_layered; /**< whether glFramebufferTexture was called */
    unsigned rtt_nr_samples; /**< from FramebufferTexture2DMultisampleEXT */
};


static inline struct st_renderbuffer *
st_renderbuffer(struct gl_renderbuffer *rb)
{
    return (struct st_renderbuffer *) rb;
}

static inline struct pipe_resource *
st_get_renderbuffer_resource(struct gl_renderbuffer *rb)
{
    return st_renderbuffer(rb)->texture;
}

/**
 * Cast wrapper to convert a struct gl_framebuffer to an st_framebuffer.
 * Return NULL if the struct gl_framebuffer is a user-created framebuffer.
 * We'll only return non-null for window system framebuffers.
 * Note that this function may fail.
 */
static inline struct st_framebuffer *
st_ws_framebuffer(struct gl_framebuffer *fb)
{
    /* FBO cannot be casted.  See st_new_framebuffer */
    if (fb && _mesa_is_winsys_fbo(fb) &&
        fb != _mesa_get_incomplete_framebuffer())
        return (struct st_framebuffer *) fb;
    return NULL;
}


extern struct gl_renderbuffer *
st_new_renderbuffer_fb(enum pipe_format format, unsigned samples, boolean sw);

extern void
st_update_renderbuffer_surface(struct st_context *st,
                               struct st_renderbuffer *strb);

extern void
st_regen_renderbuffer_surface(struct st_context *st,
                              struct st_renderbuffer *strb);

struct gl_renderbuffer *st_new_renderbuffer(struct gl_context *ctx, GLuint name);
void st_render_texture(struct gl_context *ctx,
                       struct gl_framebuffer *fb,
                       struct gl_renderbuffer_attachment *att);
void st_finish_render_texture(struct gl_context *ctx, struct gl_renderbuffer *rb);
void st_validate_framebuffer(struct gl_context *ctx, struct gl_framebuffer *fb);
void st_discard_framebuffer(struct gl_context *ctx, struct gl_framebuffer *fb,
                            struct gl_renderbuffer_attachment *att);
void st_DrawBufferAllocate(struct gl_context *ctx);
void st_ReadBuffer(struct gl_context *ctx, GLenum buffer);

void st_MapRenderbuffer(struct gl_context *ctx,
                        struct gl_renderbuffer *rb,
                        GLuint x, GLuint y, GLuint w, GLuint h,
                        GLbitfield mode,
                        GLubyte **mapOut, GLint *rowStrideOut,
                        bool flip_y);
void st_UnmapRenderbuffer(struct gl_context *ctx,
                          struct gl_renderbuffer *rb);
void st_EvaluateDepthValues(struct gl_context *ctx);
#endif /* ST_CB_FBO_H */
