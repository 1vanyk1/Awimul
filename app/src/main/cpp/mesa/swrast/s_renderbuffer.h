#ifndef S_RENDERBUFFER_H
#define S_RENDERBUFFER_H

#include "../main/glheader.h"


struct gl_context;
struct gl_framebuffer;
struct gl_renderbuffer;


extern struct gl_renderbuffer *
_swrast_new_soft_renderbuffer(struct gl_context *ctx, GLuint name);

extern void
_swrast_map_soft_renderbuffer(struct gl_context *ctx,
                              struct gl_renderbuffer *rb,
                              GLuint x, GLuint y, GLuint w, GLuint h,
                              GLbitfield mode,
                              GLubyte **out_map,
                              GLint *out_stride,
                              bool flip_y);

extern void
_swrast_unmap_soft_renderbuffer(struct gl_context *ctx,
                                struct gl_renderbuffer *rb);

extern void
_swrast_set_renderbuffer_accessors(struct gl_renderbuffer *rb);


extern void
_swrast_add_soft_renderbuffers(struct gl_framebuffer *fb,
                               GLboolean color,
                               GLboolean depth,
                               GLboolean stencil,
                               GLboolean accum,
                               GLboolean alpha);


#endif /* S_RENDERBUFFER_H */
