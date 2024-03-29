#ifndef IMAGE_H
#define IMAGE_H


#include "glheader.h"

struct gl_context;
struct gl_pixelstore_attrib;
struct gl_framebuffer;

extern void
_mesa_swap2(GLushort *p, GLuint n);

extern void
_mesa_swap4(GLuint *p, GLuint n);


extern GLintptr
_mesa_image_offset( GLuint dimensions,
                    const struct gl_pixelstore_attrib *packing,
                    GLsizei width, GLsizei height,
                    GLenum format, GLenum type,
                    GLint img, GLint row, GLint column );

extern GLvoid *
_mesa_image_address( GLuint dimensions,
                     const struct gl_pixelstore_attrib *packing,
                     const GLvoid *image,
                     GLsizei width, GLsizei height,
                     GLenum format, GLenum type,
                     GLint img, GLint row, GLint column );

extern GLvoid *
_mesa_image_address1d( const struct gl_pixelstore_attrib *packing,
                       const GLvoid *image,
                       GLsizei width,
                       GLenum format, GLenum type,
                       GLint column );

extern GLvoid *
_mesa_image_address2d( const struct gl_pixelstore_attrib *packing,
                       const GLvoid *image,
                       GLsizei width, GLsizei height,
                       GLenum format, GLenum type,
                       GLint row, GLint column );

extern GLvoid *
_mesa_image_address3d( const struct gl_pixelstore_attrib *packing,
                       const GLvoid *image,
                       GLsizei width, GLsizei height,
                       GLenum format, GLenum type,
                       GLint img, GLint row, GLint column );


extern GLint
_mesa_image_row_stride( const struct gl_pixelstore_attrib *packing,
                        GLint width, GLenum format, GLenum type );


extern GLint
_mesa_image_image_stride( const struct gl_pixelstore_attrib *packing,
                          GLint width, GLint height,
                          GLenum format, GLenum type );


extern void
_mesa_expand_bitmap(GLsizei width, GLsizei height,
                    const struct gl_pixelstore_attrib *unpack,
                    const GLubyte *bitmap,
                    GLubyte *destBuffer, GLint destStride,
                    GLubyte onValue);


extern void
_mesa_convert_colors(GLenum srcType, const GLvoid *src,
                     GLenum dstType, GLvoid *dst,
                     GLuint count, const GLubyte mask[]);


extern GLboolean
_mesa_clip_drawpixels(const struct gl_context *ctx,
                      GLint *destX, GLint *destY,
                      GLsizei *width, GLsizei *height,
                      struct gl_pixelstore_attrib *unpack);


extern GLboolean
_mesa_clip_readpixels(const struct gl_context *ctx,
                      GLint *srcX, GLint *srcY,
                      GLsizei *width, GLsizei *height,
                      struct gl_pixelstore_attrib *pack);

extern GLboolean
_mesa_clip_copytexsubimage(const struct gl_context *ctx,
                           GLint *destX, GLint *destY,
                           GLint *srcX, GLint *srcY,
                           GLsizei *width, GLsizei *height);

extern GLboolean
_mesa_clip_to_region(GLint xmin, GLint ymin,
                     GLint xmax, GLint ymax,
                     GLint *x, GLint *y,
                     GLsizei *width, GLsizei *height );

extern GLboolean
_mesa_clip_blit(struct gl_context *ctx,
                const struct gl_framebuffer *readFb,
                const struct gl_framebuffer *drawFb,
                GLint *srcX0, GLint *srcY0, GLint *srcX1, GLint *srcY1,
                GLint *dstX0, GLint *dstY0, GLint *dstX1, GLint *dstY1);

void
_mesa_swap_bytes_2d_image(GLenum format, GLenum type,
                          const struct gl_pixelstore_attrib *packing,
                          GLsizei width, GLsizei height,
                          GLvoid *dst, const GLvoid *src);

#endif
