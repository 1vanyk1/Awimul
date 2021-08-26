#ifndef GLSL_PROGRAM_H
#define GLSL_PROGRAM_H

#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;
struct gl_shader;
struct gl_shader_program;

extern void
_mesa_glsl_compile_shader(struct gl_context *ctx, struct gl_shader *shader,
                          bool dump_ast, bool dump_hir, bool force_recompile);

#ifdef __cplusplus
} /* extern "C" */
#endif

extern void
link_shaders(struct gl_context *ctx, struct gl_shader_program *prog);

extern void
build_program_resource_list(struct gl_context *ctx,
                            struct gl_shader_program *shProg,
                            bool add_packed_varyings_only);

extern long
parse_program_resource_name(const GLchar *name, const size_t len,
                            const GLchar **out_base_name_end);

#endif /* GLSL_PROGRAM_H */
