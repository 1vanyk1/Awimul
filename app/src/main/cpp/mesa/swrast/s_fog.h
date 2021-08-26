#ifndef S_FOG_H
#define S_FOG_H


#include "../main/glheader.h"
#include "s_span.h"

struct gl_context;

extern GLfloat
_swrast_z_to_fogfactor(struct gl_context *ctx, GLfloat z);

extern void
_swrast_fog_rgba_span( const struct gl_context *ctx, SWspan *span );

#endif
