#ifndef S_LINES_H
#define S_LINES_H

#include "swrast.h"

void
_swrast_choose_line( struct gl_context *ctx );

void
_swrast_add_spec_terms_line( struct gl_context *ctx,
                             const SWvertex *v0,
                             const SWvertex *v1 );


#endif
