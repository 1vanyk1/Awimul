#ifndef TEXCOMPRESS_ETC1_H
#define TEXCOMPRESS_ETC1_H

#include <inttypes.h>
#include "glheader.h"
#include "texcompress.h"
#include "texstore.h"


GLboolean
_mesa_texstore_etc1_rgb8(TEXSTORE_PARAMS);

GLboolean
_mesa_texstore_etc2_rgb8(TEXSTORE_PARAMS);

GLboolean
_mesa_texstore_etc2_srgb8(TEXSTORE_PARAMS);

GLboolean
_mesa_texstore_etc2_rgba8_eac(TEXSTORE_PARAMS);

GLboolean
_mesa_texstore_etc2_srgb8_alpha8_eac(TEXSTORE_PARAMS);

GLboolean
_mesa_texstore_etc2_r11_eac(TEXSTORE_PARAMS);

GLboolean
_mesa_texstore_etc2_rg11_eac(TEXSTORE_PARAMS);

GLboolean
_mesa_texstore_etc2_signed_r11_eac(TEXSTORE_PARAMS);

GLboolean
_mesa_texstore_etc2_signed_rg11_eac(TEXSTORE_PARAMS);

GLboolean
_mesa_texstore_etc2_rgb8_punchthrough_alpha1(TEXSTORE_PARAMS);

GLboolean
_mesa_texstore_etc2_srgb8_punchthrough_alpha1(TEXSTORE_PARAMS);

void
_mesa_etc1_unpack_rgba8888(uint8_t *dst_row,
                           unsigned dst_stride,
                           const uint8_t *src_row,
                           unsigned src_stride,
                           unsigned src_width,
                           unsigned src_height);
void
_mesa_unpack_etc2_format(uint8_t *dst_row,
                         unsigned dst_stride,
                         const uint8_t *src_row,
                         unsigned src_stride,
                         unsigned src_width,
                         unsigned src_height,
                         mesa_format format,
                         bool bgra);

compressed_fetch_func
_mesa_get_etc_fetch_func(mesa_format format);

#endif
