#ifndef GLSPIRV_H
#define GLSPIRV_H

#include "../compiler/nir/nir.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gl_shader_program;
struct gl_context;
struct gl_shader;

/**
 * A SPIR-V module contains the raw SPIR-V binary as set by ShaderBinary.
 *
 * It is reference-counted, because the same module can be attached to multiple
 * shader objects simultaneously.
 */
struct gl_spirv_module {
    unsigned RefCount;
    GLint Length;
    char Binary[0];
};

/**
 * SPIR-V data needed to compile and link a SPIR-V shader.
 *
 * It includes a SPIR-V binary that is potentially shared among different
 * shaders; and shader-specific specialization constants and entry point.
 *
 * It is reference-counted because it is shared between gl_shader and its
 * corresponding gl_linked_shader.
 */
struct gl_shader_spirv_data {
    GLint RefCount;

    struct gl_spirv_module *SpirVModule;

    GLchar *SpirVEntryPoint;

    GLuint NumSpecializationConstants;
    GLuint *SpecializationConstantsIndex;
    GLuint *SpecializationConstantsValue;
};

void
_mesa_spirv_module_reference(struct gl_spirv_module **dest,
                             struct gl_spirv_module *src);

void
_mesa_shader_spirv_data_reference(struct gl_shader_spirv_data **dest,
                                  struct gl_shader_spirv_data *src);

void
_mesa_spirv_shader_binary(struct gl_context *ctx,
                          unsigned n, struct gl_shader **shaders,
                          const void* binary, size_t length);

void
_mesa_spirv_link_shaders(struct gl_context *ctx,
                         struct gl_shader_program *prog);

nir_shader *
_mesa_spirv_to_nir(struct gl_context *ctx,
                   const struct gl_shader_program *prog,
                   gl_shader_stage stage,
                   const nir_shader_compiler_options *options);

/**
 * \name API functions
 */
/*@{*/

void GLAPIENTRY
_mesa_SpecializeShaderARB(GLuint shader,
const GLchar *pEntryPoint,
        GLuint numSpecializationConstants,
const GLuint *pConstantIndex,
const GLuint *pConstantValue);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif /* GLSPIRV_H */
