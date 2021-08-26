#ifndef TEXCOMPRESS_RGTC_H
#define TEXCOMPRESS_RGTC_H

#include "glheader.h"
#include "texstore.h"


extern GLboolean
_mesa_texstore_red_rgtc1(TEXSTORE_PARAMS);

extern GLboolean
_mesa_texstore_signed_red_rgtc1(TEXSTORE_PARAMS);

extern GLboolean
_mesa_texstore_rg_rgtc2(TEXSTORE_PARAMS);

extern GLboolean
_mesa_texstore_signed_rg_rgtc2(TEXSTORE_PARAMS);

extern compressed_fetch_func
_mesa_get_compressed_rgtc_func(mesa_format format);


#endif
