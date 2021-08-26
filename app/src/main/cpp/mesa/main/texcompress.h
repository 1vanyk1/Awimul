#ifndef TEXCOMPRESS_H
#define TEXCOMPRESS_H

#include "formats.h"
#include "glheader.h"

struct gl_context;

extern GLenum
_mesa_gl_compressed_format_base_format(GLenum format);

extern GLuint
_mesa_get_compressed_formats(struct gl_context *ctx, GLint *formats);

extern mesa_format
_mesa_glenum_to_compressed_format(GLenum format);

extern GLenum
_mesa_compressed_format_to_glenum(struct gl_context *ctx, mesa_format mesaFormat);

extern GLubyte *
_mesa_compressed_image_address(GLint col, GLint row, GLint img,
                               mesa_format mesaFormat,
                               GLsizei width, const GLubyte *image);


/** A function to fetch one texel from a compressed texture */
typedef void (*compressed_fetch_func)(const GLubyte *map,
                                      GLint rowStride,
                                      GLint i, GLint j,
                                      GLfloat *texel);

extern compressed_fetch_func
_mesa_get_compressed_fetch_func(mesa_format format);


extern void
_mesa_decompress_image(mesa_format format, GLuint width, GLuint height,
                       const GLubyte *src, GLint srcRowStride,
                       GLfloat *dest);

#endif /* TEXCOMPRESS_H */
