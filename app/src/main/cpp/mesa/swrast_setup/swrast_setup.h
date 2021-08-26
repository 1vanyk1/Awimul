#ifndef SWRAST_SETUP_H
#define SWRAST_SETUP_H

#include "../swrast/swrast.h"

extern GLboolean
_swsetup_CreateContext( struct gl_context *ctx );

extern void
_swsetup_DestroyContext( struct gl_context *ctx );

extern void
_swsetup_InvalidateState( struct gl_context *ctx, GLuint new_state );

extern void
_swsetup_Wakeup( struct gl_context *ctx );

/* Helper function to translate a hardware vertex (as understood by
 * the tnl/t_vertex.c code) to a swrast vertex.
 */
extern void
_swsetup_Translate( struct gl_context *ctx, const void *vertex, SWvertex *dest );

#endif
