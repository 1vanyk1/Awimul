#ifndef ST_CB_PROGRAM_H
#define ST_CB_PROGRAM_H

#ifdef __cplusplus
extern "C" {
#endif

struct dd_function_table;

extern void
st_init_program_functions(struct dd_function_table *functions);
void st_delete_program(struct gl_context *ctx, struct gl_program *prog);
GLboolean st_program_string_notify(struct gl_context *ctx,
                                   GLenum target,
                                   struct gl_program *prog);
struct gl_program *st_new_ati_fs(struct gl_context *ctx, struct ati_fragment_shader *curProg);

void st_max_shader_compiler_threads(struct gl_context *ctx, unsigned count);
bool st_get_shader_program_completion_status(struct gl_context *ctx,
                                             struct gl_shader_program *shprog);

#ifdef __cplusplus
}
#endif
#endif
