#ifndef _NIR_SERIALIZE_H
#define _NIR_SERIALIZE_H

#include "nir.h"
#include "../../util/blob.h"

#ifdef __cplusplus
extern "C" {
#endif

void nir_serialize(struct blob *blob, const nir_shader *nir, bool strip);
nir_shader *nir_deserialize(void *mem_ctx,
                            const struct nir_shader_compiler_options *options,
                            struct blob_reader *blob);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
