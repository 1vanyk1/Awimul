#ifndef ARBPROGPARSE_H
#define ARBPROGPARSE_H

#include "../main/glheader.h"

struct gl_context;
struct gl_program;

extern void
_mesa_parse_arb_vertex_program(struct gl_context *ctx, GLenum target,
                               const GLvoid *str, GLsizei len,
                               struct gl_program *program);

extern void
_mesa_parse_arb_fragment_program(struct gl_context *ctx, GLenum target,
                                 const GLvoid *str, GLsizei len,
                                 struct gl_program *program);

#endif
