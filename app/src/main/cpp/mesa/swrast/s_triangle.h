#ifndef S_TRIANGLES_H
#define S_TRIANGLES_H


#include "swrast.h"


extern GLboolean
_swrast_culltriangle( struct gl_context *ctx,
                      const SWvertex *v0,
                      const SWvertex *v1,
                      const SWvertex *v2);

extern void
_swrast_choose_triangle( struct gl_context *ctx );

extern void
_swrast_add_spec_terms_triangle( struct gl_context *ctx,
                                 const SWvertex *v0,
                                 const SWvertex *v1,
                                 const SWvertex *v2 );


#endif
