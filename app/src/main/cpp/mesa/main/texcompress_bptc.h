#ifndef TEXCOMPRESS_BPTC_H
#define TEXCOMPRESS_BPTC_H

#include <inttypes.h>
#include "glheader.h"
#include "texcompress.h"
#include "texstore.h"

GLboolean
_mesa_texstore_bptc_rgba_unorm(TEXSTORE_PARAMS);

GLboolean
_mesa_texstore_bptc_rgb_signed_float(TEXSTORE_PARAMS);

GLboolean
_mesa_texstore_bptc_rgb_unsigned_float(TEXSTORE_PARAMS);

compressed_fetch_func
_mesa_get_bptc_fetch_func(mesa_format format);

#endif
