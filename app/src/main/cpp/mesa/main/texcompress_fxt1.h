#ifndef TEXCOMPRESS_FXT1_H
#define TEXCOMPRESS_FXT1_H

#include "glheader.h"
#include "texstore.h"


extern GLboolean
        _mesa_texstore_fxt1(TEXSTORE_PARAMS);


compressed_fetch_func
_mesa_get_fxt_fetch_func(mesa_format format);

#endif /* TEXCOMPRESS_FXT1_H */
