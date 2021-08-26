#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include "glheader.h"
#include "menums.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;
struct gl_framebuffer;
struct gl_renderbuffer;

extern void
_mesa_init_renderbuffer(struct gl_renderbuffer *rb, GLuint name);

extern struct gl_renderbuffer *
_mesa_new_renderbuffer(struct gl_context *ctx, GLuint name);

extern void
_mesa_delete_renderbuffer(struct gl_context *ctx, struct gl_renderbuffer *rb);

extern void
_mesa_attach_and_own_rb(struct gl_framebuffer *fb,
                        gl_buffer_index bufferName,
                        struct gl_renderbuffer *rb);

extern void
_mesa_attach_and_reference_rb(struct gl_framebuffer *fb,
                              gl_buffer_index bufferName,
                              struct gl_renderbuffer *rb);

extern void
_mesa_remove_renderbuffer(struct gl_framebuffer *fb,
                          gl_buffer_index bufferName);

extern void
_mesa_reference_renderbuffer_(struct gl_renderbuffer **ptr,
                              struct gl_renderbuffer *rb);

static inline void
_mesa_reference_renderbuffer(struct gl_renderbuffer **ptr,
                             struct gl_renderbuffer *rb)
{
    if (*ptr != rb)
        _mesa_reference_renderbuffer_(ptr, rb);
}

#ifdef __cplusplus
}
#endif

#endif /* RENDERBUFFER_H */
