#ifndef S_STENCIL_H
#define S_STENCIL_H


#include "../main/mtypes.h"
#include "s_span.h"



extern GLboolean
_swrast_stencil_and_ztest_span(struct gl_context *ctx, SWspan *span);


extern void
_swrast_read_stencil_span(struct gl_context *ctx, struct gl_renderbuffer *rb,
                          GLint n, GLint x, GLint y, GLubyte stencil[]);


extern void
_swrast_write_stencil_span( struct gl_context *ctx, GLint n, GLint x, GLint y,
                            const GLubyte stencil[] );


extern void
_swrast_clear_stencil_buffer(struct gl_context *ctx);


#endif
