#ifndef TEXFORMAT_H
#define TEXFORMAT_H


#include "formats.h"

struct gl_context;

extern mesa_format
_mesa_choose_tex_format(struct gl_context *ctx, GLenum target,
                        GLint internalFormat, GLenum format, GLenum type);

#endif
