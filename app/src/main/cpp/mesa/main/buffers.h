#ifndef BUFFERS_H
#define BUFFERS_H


#include "glheader.h"
#include "menums.h"

struct gl_context;
struct gl_framebuffer;


void GLAPIENTRY
_mesa_DrawBuffer_no_error(GLenum mode);

extern void GLAPIENTRY
_mesa_DrawBuffer( GLenum mode );

void GLAPIENTRY
_mesa_NamedFramebufferDrawBuffer_no_error(GLuint framebuffer, GLenum buf);

extern void GLAPIENTRY
_mesa_NamedFramebufferDrawBuffer(GLuint framebuffer, GLenum buf);

void GLAPIENTRY
_mesa_DrawBuffers_no_error(GLsizei n, const GLenum *buffers);

extern void GLAPIENTRY
_mesa_DrawBuffers(GLsizei n, const GLenum *buffers);

void GLAPIENTRY
_mesa_NamedFramebufferDrawBuffers_no_error(GLuint framebuffer, GLsizei n,
                                           const GLenum *bufs);

extern void GLAPIENTRY
_mesa_NamedFramebufferDrawBuffers(GLuint framebuffer, GLsizei n,
                                  const GLenum *bufs);

extern void
_mesa_drawbuffers(struct gl_context *ctx, struct gl_framebuffer *fb,
                  GLuint n, const GLenum16 *buffers,
                  const GLbitfield *destMask);

extern void
_mesa_readbuffer(struct gl_context *ctx, struct gl_framebuffer *fb,
                 GLenum buffer, gl_buffer_index bufferIndex);

extern void
_mesa_update_draw_buffers(struct gl_context *ctx);

extern GLenum
_mesa_back_to_front_if_single_buffered(const struct gl_framebuffer *fb,
                                       GLenum buffer);

void GLAPIENTRY
_mesa_ReadBuffer_no_error(GLenum mode);

extern void GLAPIENTRY
_mesa_ReadBuffer( GLenum mode );

void GLAPIENTRY
_mesa_NamedFramebufferReadBuffer_no_error(GLuint framebuffer, GLenum src);

extern void GLAPIENTRY
_mesa_NamedFramebufferReadBuffer(GLuint framebuffer, GLenum src);

extern void GLAPIENTRY
_mesa_FramebufferDrawBufferEXT(GLuint framebuffer, GLenum buf);

extern void GLAPIENTRY
_mesa_FramebufferReadBufferEXT(GLuint framebuffer, GLenum buf);

extern void GLAPIENTRY
_mesa_FramebufferDrawBuffersEXT(GLuint framebuffer, GLsizei n,
                                const GLenum *bufs);
#endif
