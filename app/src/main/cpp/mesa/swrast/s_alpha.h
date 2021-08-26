#ifndef S_ALPHA_H
#define S_ALPHA_H


#include "../main/glheader.h"
#include "s_span.h"

struct gl_context;

extern GLint
_swrast_alpha_test( const struct gl_context *ctx, SWspan *span );


#endif
