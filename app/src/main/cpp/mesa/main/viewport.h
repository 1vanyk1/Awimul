#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "glheader.h"

struct gl_context;

void GLAPIENTRY
_mesa_Viewport_no_error(GLint x, GLint y, GLsizei width, GLsizei height);

extern void GLAPIENTRY
_mesa_Viewport(GLint x, GLint y, GLsizei width, GLsizei height);

void GLAPIENTRY
_mesa_ViewportArrayv_no_error(GLuint first, GLsizei count, const GLfloat * v);

extern void GLAPIENTRY
_mesa_ViewportArrayv(GLuint first, GLsizei count, const GLfloat * v);

void GLAPIENTRY
_mesa_ViewportIndexedf_no_error(GLuint index, GLfloat x, GLfloat y, GLfloat w,
                                GLfloat h);

extern void GLAPIENTRY
_mesa_ViewportIndexedf(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);

void GLAPIENTRY
_mesa_ViewportIndexedfv_no_error(GLuint index, const GLfloat * v);

extern void GLAPIENTRY
_mesa_ViewportIndexedfv(GLuint index, const GLfloat * v);

extern void
_mesa_set_viewport(struct gl_context *ctx, unsigned idx, GLfloat x, GLfloat y,
                   GLfloat width, GLfloat height);


extern void GLAPIENTRY
_mesa_DepthRange(GLclampd nearval, GLclampd farval);

extern void GLAPIENTRY
_mesa_DepthRangef(GLclampf nearval, GLclampf farval);

void GLAPIENTRY
_mesa_DepthRangeArrayv_no_error(GLuint first, GLsizei count,
                                const GLclampd * v);

extern void GLAPIENTRY
_mesa_DepthRangeArrayv(GLuint first, GLsizei count, const GLclampd * v);

extern void GLAPIENTRY
_mesa_DepthRangeArrayfvOES(GLuint first, GLsizei count, const GLfloat * v);

void GLAPIENTRY
_mesa_DepthRangeIndexed_no_error(GLuint index, GLclampd n, GLclampd f);

extern void GLAPIENTRY
_mesa_DepthRangeIndexed(GLuint index, GLclampd n, GLclampd f);

extern void GLAPIENTRY
_mesa_DepthRangeIndexedfOES(GLuint index, GLfloat n, GLfloat f);

extern void
_mesa_set_depth_range(struct gl_context *ctx, unsigned idx,
                      GLclampd nearval, GLclampd farval);

extern void
_mesa_init_viewport(struct gl_context *ctx);


void GLAPIENTRY
_mesa_ClipControl_no_error(GLenum origin, GLenum depth);

extern void GLAPIENTRY
_mesa_ClipControl(GLenum origin, GLenum depth);

extern void
_mesa_get_viewport_xform(struct gl_context *ctx, unsigned i,
                         float scale[3], float translate[3]);

extern void GLAPIENTRY
_mesa_SubpixelPrecisionBiasNV_no_error(GLuint xbits, GLuint ybits);

extern void GLAPIENTRY
_mesa_SubpixelPrecisionBiasNV(GLuint xbits, GLuint ybits);

extern void GLAPIENTRY
_mesa_ViewportSwizzleNV_no_error(GLuint index,
                                 GLenum swizzlex, GLenum swizzley,
                                 GLenum swizzlez, GLenum swizzlew);

extern void GLAPIENTRY
_mesa_ViewportSwizzleNV(GLuint index,
                        GLenum swizzlex, GLenum swizzley,
                        GLenum swizzlez, GLenum swizzlew);

#endif
