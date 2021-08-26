#ifndef SCISSOR_H
#define SCISSOR_H


#include "glheader.h"

struct gl_context;

void GLAPIENTRY
_mesa_Scissor_no_error(GLint x, GLint y, GLsizei width, GLsizei height);

extern void GLAPIENTRY
_mesa_Scissor( GLint x, GLint y, GLsizei width, GLsizei height );

void GLAPIENTRY
_mesa_ScissorArrayv_no_error(GLuint first, GLsizei count, const GLint * v);

extern void GLAPIENTRY
_mesa_ScissorArrayv(GLuint first, GLsizei count, const GLint * v);

void GLAPIENTRY
_mesa_ScissorIndexed_no_error(GLuint index, GLint left, GLint bottom,
                              GLsizei width, GLsizei height);

extern void GLAPIENTRY
_mesa_ScissorIndexed(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);

void GLAPIENTRY
_mesa_ScissorIndexedv_no_error(GLuint index, const GLint * v);

extern void GLAPIENTRY
_mesa_ScissorIndexedv(GLuint index, const GLint * v);

extern void GLAPIENTRY
_mesa_WindowRectanglesEXT(GLenum mode, GLsizei count, const GLint *box);

extern void
_mesa_set_scissor(struct gl_context *ctx, unsigned idx,
                  GLint x, GLint y, GLsizei width, GLsizei height);


extern void
_mesa_init_scissor(struct gl_context *ctx);


#endif
