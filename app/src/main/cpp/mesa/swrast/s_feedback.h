#ifndef S_FEEDBACK_H
#define S_FEEDBACK_H


#include "swrast.h"


extern void _swrast_feedback_point( struct gl_context *ctx, const SWvertex *v );

extern void _swrast_feedback_line( struct gl_context *ctx,
                                   const SWvertex *v1, const SWvertex *v2 );

extern void _swrast_feedback_triangle( struct gl_context *ctx, const SWvertex *v0,
                                       const SWvertex *v1, const SWvertex *v2 );

extern void _swrast_select_point( struct gl_context *ctx, const SWvertex *v );

extern void _swrast_select_line( struct gl_context *ctx,
                                 const SWvertex *v1, const SWvertex *v2 );

extern void _swrast_select_triangle( struct gl_context *ctx, const SWvertex *v0,
                                     const SWvertex *v1, const SWvertex *v2 );

#endif
