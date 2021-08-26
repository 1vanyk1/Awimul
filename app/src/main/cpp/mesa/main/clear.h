#ifndef CLEAR_H
#define CLEAR_H


#include "glheader.h"


extern void GLAPIENTRY
_mesa_ClearIndex( GLfloat c );

extern void GLAPIENTRY
_mesa_ClearColor( GLclampf red, GLclampf green,
                  GLclampf blue, GLclampf alpha );

extern void GLAPIENTRY
_mesa_ClearColorIiEXT(GLint r, GLint g, GLint b, GLint a);

extern void GLAPIENTRY
_mesa_ClearColorIuiEXT(GLuint r, GLuint g, GLuint b, GLuint a);

void GLAPIENTRY
_mesa_Clear_no_error(GLbitfield mask);

extern void GLAPIENTRY
_mesa_Clear( GLbitfield mask );

void GLAPIENTRY
_mesa_ClearBufferiv_no_error(GLenum buffer, GLint drawbuffer,
                             const GLint *value);

extern void GLAPIENTRY
_mesa_ClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint *value);

extern void GLAPIENTRY
_mesa_ClearNamedFramebufferiv(GLuint framebuffer, GLenum buffer,
                              GLint drawbuffer, const GLint *value);

void GLAPIENTRY
_mesa_ClearBufferuiv_no_error(GLenum buffer, GLint drawbuffer,
                              const GLuint *value);

extern void GLAPIENTRY
_mesa_ClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint *value);

extern void GLAPIENTRY
_mesa_ClearNamedFramebufferuiv(GLuint framebuffer, GLenum buffer,
                               GLint drawbuffer, const GLuint *value);

void GLAPIENTRY
_mesa_ClearBufferfv_no_error(GLenum buffer, GLint drawbuffer,
                             const GLfloat *value);

extern void GLAPIENTRY
_mesa_ClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value);

extern void GLAPIENTRY
_mesa_ClearNamedFramebufferfv(GLuint framebuffer, GLenum buffer,
                              GLint drawbuffer, const GLfloat *value);

void GLAPIENTRY
_mesa_ClearBufferfi_no_error(GLenum buffer, GLint drawbuffer,
                             GLfloat depth, GLint stencil);

extern void GLAPIENTRY
_mesa_ClearBufferfi(GLenum buffer, GLint drawbuffer,
                    GLfloat depth, GLint stencil);

extern void GLAPIENTRY
_mesa_ClearNamedFramebufferfi(GLuint framebuffer, GLenum buffer,
                              GLint drawbuffer, GLfloat depth, GLint stencil);

#endif
