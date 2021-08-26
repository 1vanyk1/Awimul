#include "../main/glheader.h"
#include "../main/ffvertex_prog.h"
#include "../main/mtypes.h"
#include "t_vp_build.h"


/**
 * XXX This should go away someday, but still referenced by some drivers...
 */
void _tnl_UpdateFixedFunctionProgram( struct gl_context *ctx )
{
    if (!ctx->VertexProgram._Current ||
        ctx->VertexProgram._Current == ctx->VertexProgram._TnlProgram) {
        ctx->VertexProgram._Current
                = ctx->VertexProgram._TnlProgram
                = _mesa_get_fixed_func_vertex_program(ctx);
    }
}
