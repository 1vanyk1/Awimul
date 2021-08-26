#ifndef PROGRAM_BINARY_H
#define PROGRAM_BINARY_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void
_mesa_get_program_binary_length(struct gl_context *ctx,
                                struct gl_shader_program *sh_prog,
                                GLint *length);

void
_mesa_get_program_binary(struct gl_context *ctx,
                         struct gl_shader_program *sh_prog,
                         GLsizei buf_size, GLsizei *length,
                         GLenum *binary_format, GLvoid *binary);

void
_mesa_program_binary(struct gl_context *ctx, struct gl_shader_program *sh_prog,
                     GLenum binary_format, const GLvoid *binary,
                     GLsizei length);

#ifdef __cplusplus
}
#endif

#endif /* PROGRAM_BINARY_H */
