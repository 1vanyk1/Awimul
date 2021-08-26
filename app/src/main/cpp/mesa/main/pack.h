#ifndef PACK_H
#define PACK_H


#include "glheader.h"

struct gl_context;
struct gl_pixelstore_attrib;

extern void
_mesa_unpack_polygon_stipple(const GLubyte *pattern, GLuint dest[32],
                             const struct gl_pixelstore_attrib *unpacking);


extern void
_mesa_pack_polygon_stipple(const GLuint pattern[32], GLubyte *dest,
                           const struct gl_pixelstore_attrib *packing);


extern void
_mesa_pack_bitmap(GLint width, GLint height, const GLubyte *source,
                  GLubyte *dest, const struct gl_pixelstore_attrib *packing);


extern void
_mesa_unpack_stencil_span(struct gl_context *ctx, GLuint n,
                          GLenum dstType, GLvoid *dest,
                          GLenum srcType, const GLvoid *source,
                          const struct gl_pixelstore_attrib *srcPacking,
                          GLbitfield transferOps);

extern void
_mesa_pack_stencil_span(struct gl_context *ctx, GLuint n,
                        GLenum dstType, GLvoid *dest, const GLubyte *source,
                        const struct gl_pixelstore_attrib *dstPacking);


extern void
_mesa_unpack_depth_span(struct gl_context *ctx, GLuint n,
                        GLenum dstType, GLvoid *dest, GLuint depthMax,
                        GLenum srcType, const GLvoid *source,
                        const struct gl_pixelstore_attrib *srcPacking);

extern void
_mesa_pack_depth_span(struct gl_context *ctx, GLuint n, GLvoid *dest,
                      GLenum dstType, const GLfloat *depthSpan,
                      const struct gl_pixelstore_attrib *dstPacking);


extern void
_mesa_pack_depth_stencil_span(struct gl_context *ctx,GLuint n,
                              GLenum dstType, GLuint *dest,
                              const GLfloat *depthVals,
                              const GLubyte *stencilVals,
                              const struct gl_pixelstore_attrib *dstPacking);


extern void *
_mesa_unpack_image(GLuint dimensions,
                   GLsizei width, GLsizei height, GLsizei depth,
                   GLenum format, GLenum type, const GLvoid *pixels,
                   const struct gl_pixelstore_attrib *unpack);

extern void
_mesa_pack_luminance_from_rgba_float(GLuint n, GLfloat rgba[][4],
                                     GLvoid *dstAddr, GLenum dst_format,
                                     GLbitfield transferOps);

extern void
_mesa_pack_luminance_from_rgba_integer(GLuint n, GLuint rgba[][4], bool rgba_is_signed,
                                       GLvoid *dstAddr, GLenum dst_format,
                                       GLenum dst_type);

extern GLfloat *
_mesa_unpack_color_index_to_rgba_float(struct gl_context *ctx, GLuint dims,
                                       const void *src, GLenum srcFormat, GLenum srcType,
                                       int srcWidth, int srcHeight, int srcDepth,
                                       const struct gl_pixelstore_attrib *srcPacking,
                                       GLbitfield transferOps);

extern GLubyte *
_mesa_unpack_color_index_to_rgba_ubyte(struct gl_context *ctx, GLuint dims,
                                       const void *src, GLenum srcFormat, GLenum srcType,
                                       int srcWidth, int srcHeight, int srcDepth,
                                       const struct gl_pixelstore_attrib *srcPacking,
                                       GLbitfield transferOps);

#endif
