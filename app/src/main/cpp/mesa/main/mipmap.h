#ifndef MIPMAP_H
#define MIPMAP_H

#include "glheader.h"

struct gl_context;
struct gl_texture_object;

unsigned
_mesa_compute_num_levels(struct gl_context *ctx,
                         struct gl_texture_object *texObj,
                         GLenum target);

extern void
_mesa_generate_mipmap_level(GLenum target,
                            GLenum datatype, GLuint comps,
                            GLint border,
                            GLint srcWidth, GLint srcHeight, GLint srcDepth,
                            const GLubyte **srcData,
                            GLint srcRowStride,
                            GLint dstWidth, GLint dstHeight, GLint dstDepth,
                            GLubyte **dstData,
                            GLint dstRowStride);

void
_mesa_prepare_mipmap_levels(struct gl_context *ctx,
                            struct gl_texture_object *texObj,
                            unsigned baseLevel, unsigned maxLevel);

extern void
_mesa_generate_mipmap(struct gl_context *ctx, GLenum target,
                      struct gl_texture_object *texObj);

extern GLboolean
_mesa_next_mipmap_level_size(GLenum target, GLint border,
                             GLint srcWidth, GLint srcHeight, GLint srcDepth,
                             GLint *dstWidth, GLint *dstHeight, GLint *dstDepth);

#endif /* MIPMAP_H */
