#ifndef ST_SHADER_CACHE_H
#define ST_SHADER_CACHE_H

#include "st_context.h"
#include "../main/mtypes.h"
#include "../include/pipe/p_state.h"
#include "../util/blob.h"
#include "../util/disk_cache.h"
#include "../util/mesa-sha1.h"

#ifdef __cplusplus
extern "C" {
#endif

void
st_get_program_binary_driver_sha1(struct gl_context *ctx, uint8_t *sha1);

void
st_serialise_tgsi_program(struct gl_context *ctx, struct gl_program *prog);

void
st_serialise_tgsi_program_binary(struct gl_context *ctx,
                                 struct gl_shader_program *shProg,
                                 struct gl_program *prog);

void
st_deserialise_tgsi_program(struct gl_context *ctx,
                            struct gl_shader_program *shProg,
                            struct gl_program *prog);

void
st_serialise_nir_program(struct gl_context *ctx, struct gl_program *prog);

void
st_serialise_nir_program_binary(struct gl_context *ctx,
                                struct gl_shader_program *shProg,
                                struct gl_program *prog);

void
st_deserialise_nir_program(struct gl_context *ctx,
                           struct gl_shader_program *shProg,
                           struct gl_program *prog);

bool
st_load_ir_from_disk_cache(struct gl_context *ctx,
                           struct gl_shader_program *prog,
                           bool nir);

void
st_store_ir_in_disk_cache(struct st_context *st, struct gl_program *prog,
                          bool nir);

#ifdef __cplusplus
}
#endif

#endif
