#ifndef GL_NIR_H
#define GL_NIR_H

#ifdef __cplusplus
extern "C" {
#endif

struct nir_shader;
struct gl_shader_program;

bool gl_nir_lower_atomics(nir_shader *shader,
                          const struct gl_shader_program *shader_program,
                          bool use_binding_as_idx);

bool gl_nir_lower_images(nir_shader *shader, bool bindless_only);
bool gl_nir_lower_samplers(nir_shader *shader,
                           const struct gl_shader_program *shader_program);
bool gl_nir_lower_samplers_as_deref(nir_shader *shader,
                                    const struct gl_shader_program *shader_program);

bool gl_nir_lower_buffers(nir_shader *shader,
                          const struct gl_shader_program *shader_program);

#ifdef __cplusplus
}
#endif

#endif /* GL_NIR_H */
