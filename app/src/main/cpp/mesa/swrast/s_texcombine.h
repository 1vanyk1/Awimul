#ifndef S_TEXCOMBINE_H
#define S_TEXCOMBINE_H


#include "s_span.h"

struct gl_context;

extern void
_swrast_texture_span( struct gl_context *ctx, SWspan *span );

#endif
