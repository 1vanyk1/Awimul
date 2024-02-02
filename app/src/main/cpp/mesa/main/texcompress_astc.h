#ifndef TEXCOMPRESS_ASTC_H
#define TEXCOMPRESS_ASTC_H

#include <inttypes.h>
#include "texcompress.h"

#ifdef __cplusplus
extern "C" {
#endif

void
_mesa_unpack_astc_2d_ldr(uint8_t *dst_row,
                         unsigned dst_stride,
                         const uint8_t *src_row,
                         unsigned src_stride,
                         unsigned src_width,
                         unsigned src_height,
                         mesa_format format);

#ifdef __cplusplus
}
#endif

#endif
