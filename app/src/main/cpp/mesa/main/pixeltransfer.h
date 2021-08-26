#ifndef PIXELTRANSFER_H
#define PIXELTRANSFER_H


#include "glheader.h"

struct gl_context;


extern void
_mesa_scale_and_bias_rgba(GLuint n, GLfloat rgba[][4],
                          GLfloat rScale, GLfloat gScale,
                          GLfloat bScale, GLfloat aScale,
                          GLfloat rBias, GLfloat gBias,
                          GLfloat bBias, GLfloat aBias);

extern void
_mesa_map_rgba(const struct gl_context *ctx, GLuint n, GLfloat rgba[][4]);

extern void
_mesa_map_ci_to_rgba(const struct gl_context *ctx,
                     GLuint n, const GLuint index[], GLfloat rgba[][4]);


extern void
_mesa_scale_and_bias_depth(const struct gl_context *ctx, GLuint n,
                           GLfloat depthValues[]);

extern void
_mesa_scale_and_bias_depth_uint(const struct gl_context *ctx, GLuint n,
                                GLuint depthValues[]);

extern void
_mesa_apply_rgba_transfer_ops(struct gl_context *ctx, GLbitfield transferOps,
                              GLuint n, GLfloat rgba[][4]);

extern void
_mesa_shift_and_offset_ci(const struct gl_context *ctx,
                          GLuint n, GLuint indexes[]);

extern void
_mesa_apply_ci_transfer_ops(const struct gl_context *ctx,
                            GLbitfield transferOps,
                            GLuint n, GLuint indexes[]);


extern void
_mesa_apply_stencil_transfer_ops(const struct gl_context *ctx, GLuint n,
                                 GLubyte stencil[]);


#endif
