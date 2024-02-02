#ifndef PROG_TO_NIR_H
#define PROG_TO_NIR_H

#include "../main/mtypes.h"
#include "../compiler/nir/nir.h"

#ifdef __cplusplus
extern "C" {
#endif

struct nir_shader *prog_to_nir(const struct gl_program *prog,
                               const nir_shader_compiler_options *options);

enum glsl_sampler_dim _mesa_texture_index_to_sampler_dim(gl_texture_index index,
                                                         bool *is_array);

#ifdef __cplusplus
}
#endif
#endif
