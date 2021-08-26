#ifndef S_POINTS_H
#define S_POINTS_H

#include "swrast.h"

extern void
_swrast_choose_point( struct gl_context *ctx );

extern void
_swrast_add_spec_terms_point( struct gl_context *ctx,
                              const SWvertex *v0 );

#endif
