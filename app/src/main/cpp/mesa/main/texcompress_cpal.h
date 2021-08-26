#ifndef TEXCOMPRESS_CPAL_H
#define TEXCOMPRESS_CPAL_H


#include "glheader.h"
extern void
_mesa_cpal_compressed_teximage2d(GLenum target, GLint level,
                                 GLenum internalFormat,
                                 GLsizei width, GLsizei height,
                                 GLsizei imageSize, const void *palette);

extern unsigned
_mesa_cpal_compressed_size(int level, GLenum internalFormat,
                           unsigned width, unsigned height);


#endif /* TEXCOMPRESS_CPAL_H */
