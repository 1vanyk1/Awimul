#ifndef S_ATIFRAGSHADER_H
#define S_ATIFRAGSHADER_H


#include "s_span.h"

struct gl_context;

extern void
_swrast_exec_fragment_shader( struct gl_context *ctx, SWspan *span );


#endif
