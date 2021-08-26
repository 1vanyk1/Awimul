#ifndef FORMATQUERY_H
#define FORMATQUERY_H

#include "glheader.h"

size_t
_mesa_query_samples_for_format(struct gl_context *ctx, GLenum target,
                               GLenum internalFormat, int samples[16]);

void
_mesa_query_internal_format_default(struct gl_context *ctx, GLenum target,
                                    GLenum internalFormat, GLenum pname,
                                    GLint *params);

extern void GLAPIENTRY
_mesa_GetInternalformativ(GLenum target, GLenum internalformat,
                          GLenum pname, GLsizei bufSize, GLint *params);

extern void GLAPIENTRY
_mesa_GetInternalformati64v(GLenum target, GLenum internalformat,
                            GLenum pname, GLsizei bufSize, GLint64 *params);

#endif /* FORMATQUERY_H */
