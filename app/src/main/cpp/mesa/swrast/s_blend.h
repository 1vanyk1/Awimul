#ifndef S_BLEND_H
#define S_BLEND_H


#include "../main/glheader.h"
#include "s_span.h"

struct gl_context;
struct gl_renderbuffer;


extern void
_swrast_blend_span(struct gl_context *ctx, struct gl_renderbuffer *rb, SWspan *span);


extern void
_swrast_choose_blend_func(struct gl_context *ctx, GLenum chanType);


#endif
