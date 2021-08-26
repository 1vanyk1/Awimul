#ifndef S_DEPTH_H
#define S_DEPTH_H


#include "../main/glheader.h"
#include "s_span.h"

struct gl_context;
struct gl_renderbuffer;


extern GLuint
_swrast_depth_test_span( struct gl_context *ctx, SWspan *span);

extern void
_swrast_depth_clamp_span( struct gl_context *ctx, SWspan *span );

extern GLboolean
_swrast_depth_bounds_test( struct gl_context *ctx, SWspan *span );


extern void
_swrast_read_depth_span_float( struct gl_context *ctx, struct gl_renderbuffer *rb,
                               GLint n, GLint x, GLint y, GLfloat depth[] );

extern void
_swrast_clear_depth_buffer(struct gl_context *ctx);

extern void
_swrast_clear_depth_stencil_buffer(struct gl_context *ctx);


#endif
