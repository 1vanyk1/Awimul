#ifndef S_LOGIC_H
#define S_LOGIC_H


#include "s_span.h"

struct gl_context;
struct gl_renderbuffer;

extern void
_swrast_logicop_rgba_span(struct gl_context *ctx, struct gl_renderbuffer *rb,
                          SWspan *span);


#endif
