#ifndef SS_CONTEXT_H
#define SS_CONTEXT_H

#include "../main/glheader.h"
#include "../swrast/swrast.h"
#include "../tnl/t_context.h"

typedef struct {
    GLuint NewState;
    GLenum render_prim;
    GLbitfield64 last_index_bitset;
    SWvertex *verts;
    GLboolean intColors;
} SScontext;

#define SWSETUP_CONTEXT(ctx) ((SScontext *)ctx->swsetup_context)

#endif
