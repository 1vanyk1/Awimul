#ifndef IR_TO_MESA_H
#define IR_TO_MESA_H

#include "../main/glheader.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;
struct gl_program;
struct gl_shader;
struct gl_shader_program;
struct gl_linked_shader;
struct gl_program_parameter_list;

void _mesa_glsl_link_shader(struct gl_context *ctx, struct gl_shader_program *prog);
GLboolean _mesa_ir_link_shader(struct gl_context *ctx, struct gl_shader_program *prog);

void
_mesa_generate_parameters_list_for_uniforms(struct gl_context *ctx,
                                            struct gl_shader_program
                                            *shader_program,
                                            struct gl_linked_shader *sh,
                                            struct gl_program_parameter_list
                                            *params);
void
_mesa_associate_uniform_storage(struct gl_context *ctx,
                                struct gl_shader_program *shader_program,
                                struct gl_program *prog);

void
_mesa_ensure_and_associate_uniform_storage(struct gl_context *ctx,
                                           struct gl_shader_program *shader_program,
                                           struct gl_program *prog, unsigned required_space);

#ifdef __cplusplus
}
#endif

#endif /* IR_TO_MESA_H */
