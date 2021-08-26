#ifndef PROGRAM_H
#define PROGRAM_H

#include "prog_parameter.h"


#ifdef __cplusplus
extern "C" {
#endif

extern struct gl_program _mesa_DummyProgram;


extern void
_mesa_init_program(struct gl_context *ctx);

extern void
_mesa_free_program_data(struct gl_context *ctx);

extern void
_mesa_update_default_objects_program(struct gl_context *ctx);

extern void
_mesa_set_program_error(struct gl_context *ctx, GLint pos, const char *string);

extern struct gl_program *
_mesa_init_gl_program(struct gl_program *prog, gl_shader_stage stage,
                      GLuint id, bool is_arb_asm);

extern struct gl_program *
_mesa_new_program(struct gl_context *ctx, gl_shader_stage stage, GLuint id,
                  bool is_arb_asm);

extern void
_mesa_delete_program(struct gl_context *ctx, struct gl_program *prog);

extern struct gl_program *
_mesa_lookup_program(struct gl_context *ctx, GLuint id);

extern void
_mesa_reference_program_(struct gl_context *ctx,
                         struct gl_program **ptr,
                         struct gl_program *prog);

static inline void
_mesa_reference_program(struct gl_context *ctx,
                        struct gl_program **ptr,
                        struct gl_program *prog)
{
    if (*ptr != prog)
        _mesa_reference_program_(ctx, ptr, prog);
}

extern  GLboolean
_mesa_insert_instructions(struct gl_program *prog, GLuint start, GLuint count);

extern  GLboolean
_mesa_delete_instructions(struct gl_program *prog, GLuint start, GLuint count,
                          void *mem_ctx);

extern void
_mesa_find_used_registers(const struct gl_program *prog,
                          gl_register_file file,
                          GLboolean used[], GLuint usedSize);

extern GLint
_mesa_find_free_register(const GLboolean used[],
                         GLuint maxRegs, GLuint firstReg);

extern GLint
_mesa_get_min_invocations_per_fragment(struct gl_context *ctx,
                                       const struct gl_program *prog);

static inline GLuint
_mesa_program_enum_to_shader_stage(GLenum v)
{
    switch (v) {
        case GL_VERTEX_PROGRAM_ARB:
            return MESA_SHADER_VERTEX;
        case GL_FRAGMENT_PROGRAM_ARB:
            return MESA_SHADER_FRAGMENT;
        case GL_FRAGMENT_SHADER_ATI:
            return MESA_SHADER_FRAGMENT;
        case GL_GEOMETRY_PROGRAM_NV:
            return MESA_SHADER_GEOMETRY;
        case GL_TESS_CONTROL_PROGRAM_NV:
            return MESA_SHADER_TESS_CTRL;
        case GL_TESS_EVALUATION_PROGRAM_NV:
            return MESA_SHADER_TESS_EVAL;
        case GL_COMPUTE_PROGRAM_NV:
            return MESA_SHADER_COMPUTE;
        default:
            assert(0);
            return ~0;
    }
}


static inline GLenum
_mesa_shader_stage_to_program(unsigned stage)
{
    switch (stage) {
        case MESA_SHADER_VERTEX:
            return GL_VERTEX_PROGRAM_ARB;
        case MESA_SHADER_FRAGMENT:
            return GL_FRAGMENT_PROGRAM_ARB;
        case MESA_SHADER_GEOMETRY:
            return GL_GEOMETRY_PROGRAM_NV;
        case MESA_SHADER_TESS_CTRL:
            return GL_TESS_CONTROL_PROGRAM_NV;
        case MESA_SHADER_TESS_EVAL:
            return GL_TESS_EVALUATION_PROGRAM_NV;
        case MESA_SHADER_COMPUTE:
            return GL_COMPUTE_PROGRAM_NV;
    }

    assert(!"Unexpected shader stage in _mesa_shader_stage_to_program");
    return GL_VERTEX_PROGRAM_ARB;
}


GLbitfield
gl_external_samplers(const struct gl_program *prog);

void
_mesa_add_separate_state_parameters(struct gl_program *prog,
                                    struct gl_program_parameter_list *state_params);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PROGRAM_H */
