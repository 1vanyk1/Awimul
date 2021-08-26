#ifndef READPIXELS_H
#define READPIXELS_H


#include "glheader.h"

struct gl_context;
struct gl_pixelstore_attrib;


extern GLboolean
_mesa_readpixels_needs_slow_path(const struct gl_context *ctx, GLenum format,
                                 GLenum type, GLboolean uses_blit);

extern GLboolean
_mesa_need_rgb_to_luminance_conversion(GLenum srcBaseFormat,
                                       GLenum dstBaseFormat);

extern GLboolean
_mesa_need_luminance_to_rgb_conversion(GLenum srcBaseFormat,
                                       GLenum dstBaseFormat);

extern GLbitfield
_mesa_get_readpixels_transfer_ops(const struct gl_context *ctx,
                                  mesa_format texFormat,
                                  GLenum format, GLenum type,
                                  GLboolean uses_blit);

extern void
_mesa_readpixels(struct gl_context *ctx,
                 GLint x, GLint y, GLsizei width, GLsizei height,
                 GLenum format, GLenum type,
                 const struct gl_pixelstore_attrib *packing,
                 GLvoid *pixels);

void GLAPIENTRY
_mesa_ReadPixels_no_error(GLint x, GLint y, GLsizei width, GLsizei height,
                          GLenum format, GLenum type, GLvoid *pixels);

extern void GLAPIENTRY
_mesa_ReadPixels( GLint x, GLint y, GLsizei width, GLsizei height,
                  GLenum format, GLenum type, GLvoid *pixels );

void GLAPIENTRY
_mesa_ReadnPixelsARB_no_error(GLint x, GLint y, GLsizei width, GLsizei height,
                              GLenum format, GLenum type, GLsizei bufSize,
                              GLvoid *pixels);

extern void GLAPIENTRY
_mesa_ReadnPixelsARB( GLint x, GLint y, GLsizei width, GLsizei height,
                      GLenum format, GLenum type, GLsizei bufSize,
                      GLvoid *pixels );

#endif
