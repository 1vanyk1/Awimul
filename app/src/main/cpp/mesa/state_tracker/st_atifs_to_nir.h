#ifndef ST_ATIFS_TO_TGSI_H
#define ST_ATIFS_TO_TGSI_H

#include "../main/glheader.h"
#include "../include/pipe/p_defines.h"
#include "../compiler/nir/nir.h"

#if defined __cplusplus
extern "C" {
#endif

struct gl_context;
struct gl_program;
struct ati_fragment_shader;
struct st_fp_variant_key;

nir_shader *
st_translate_atifs_program(struct ati_fragment_shader *atifs,
                           const struct st_fp_variant_key *key,
                           struct gl_program *program,
                           const nir_shader_compiler_options *options);


void
st_init_atifs_prog(struct gl_context *ctx, struct gl_program *prog);

#if defined __cplusplus
} /* extern "C" */
#endif

#endif /* ST_ATIFS_TO_TGSI_H */
