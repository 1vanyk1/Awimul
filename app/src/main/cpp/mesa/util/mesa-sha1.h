#ifndef MESA_SHA1_H
#define MESA_SHA1_H

#include <stdlib.h>
#include "../include/c99_compat.h"
#include "sha1/sha1.h"

#ifdef __cplusplus
extern "C" {
#endif

#define mesa_sha1 _SHA1_CTX

static inline void
_mesa_sha1_init(struct mesa_sha1 *ctx)
{
    SHA1Init(ctx);
}

static inline void
_mesa_sha1_update(struct mesa_sha1 *ctx, const void *data, size_t size)
{
    SHA1Update(ctx, (const unsigned char *)data, size);
}

static inline void
_mesa_sha1_final(struct mesa_sha1 *ctx, unsigned char result[20])
{
    SHA1Final(result, ctx);
}

void
_mesa_sha1_format(char *buf, const unsigned char *sha1);

void
_mesa_sha1_hex_to_sha1(unsigned char *buf, const char *hex);

void
_mesa_sha1_compute(const void *data, size_t size, unsigned char result[20]);

#ifdef __cplusplus
} /* extern C */
#endif

#endif
