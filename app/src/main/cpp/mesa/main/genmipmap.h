#ifndef GENMIPMAP_H
#define GENMIPMAP_H

#include "glheader.h"

bool
_mesa_is_valid_generate_texture_mipmap_target(struct gl_context *ctx,
                                              GLenum target);
bool
_mesa_is_valid_generate_texture_mipmap_internalformat(struct gl_context *ctx,
                                                      GLenum internalformat);

void GLAPIENTRY
_mesa_GenerateMipmap_no_error(GLenum target);

extern void GLAPIENTRY
_mesa_GenerateMipmap(GLenum target);

void GLAPIENTRY
_mesa_GenerateTextureMipmap_no_error(GLuint texture);

extern void GLAPIENTRY
_mesa_GenerateTextureMipmap(GLuint texture);

extern void GLAPIENTRY
_mesa_GenerateTextureMipmapEXT(GLuint texture, GLenum target);

extern void GLAPIENTRY
_mesa_GenerateMultiTexMipmapEXT(GLenum texunit, GLenum target);

#endif /* GENMIPMAP_H */
