#ifndef TEXCOMPRESS_S3TC_H
#define TEXCOMPRESS_S3TC_H

#include "glheader.h"
#include "texstore.h"
#include "texcompress.h"

struct gl_context;

extern GLboolean
_mesa_texstore_rgb_dxt1(TEXSTORE_PARAMS);

extern GLboolean
_mesa_texstore_rgba_dxt1(TEXSTORE_PARAMS);

extern GLboolean
_mesa_texstore_rgba_dxt3(TEXSTORE_PARAMS);

extern GLboolean
_mesa_texstore_rgba_dxt5(TEXSTORE_PARAMS);


extern compressed_fetch_func
_mesa_get_dxt_fetch_func(mesa_format format);


#endif /* TEXCOMPRESS_S3TC_H */
