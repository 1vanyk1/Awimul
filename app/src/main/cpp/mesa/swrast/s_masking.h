#ifndef S_MASKING_H
#define S_MASKING_H


#include "../main/glheader.h"
#include "s_span.h"

struct gl_context;
struct gl_renderbuffer;


extern void
_swrast_mask_rgba_span(struct gl_context *ctx, struct gl_renderbuffer *rb,
                       SWspan *span, GLuint buf);

#endif
