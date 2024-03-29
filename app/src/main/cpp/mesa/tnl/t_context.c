#include "../main/glheader.h"

#include "../main/context.h"
#include "../main/macros.h"
#include "../main/mtypes.h"
#include "../main/light.h"
#include "../math/m_translate.h"
#include "../math/m_xform.h"
#include "../main/state.h"
#include "../main/viewport.h"
#include "../util/simple_list.h"
#include "../util/u_memory.h"

#include "tnl.h"
#include "t_context.h"
#include "t_pipeline.h"

#include "../vbo/vbo.h"

GLboolean
_tnl_CreateContext( struct gl_context *ctx )
{
    TNLcontext *tnl;
    GLuint i;

    /* Create the TNLcontext structure
     */
    ctx->swtnl_context = tnl = calloc(1, sizeof(TNLcontext));

    if (!tnl) {
        return GL_FALSE;
    }

    /* Initialize the VB.
     */
    tnl->vb.Size = ctx->Const.MaxArrayLockSize + MAX_CLIPPED_VERTICES;


    /* Initialize tnl state.
     */
    if (ctx->VertexProgram._MaintainTnlProgram) {
        _tnl_install_pipeline( ctx, _tnl_vp_pipeline );
    } else {
        _tnl_install_pipeline( ctx, _tnl_default_pipeline );
    }

    _math_matrix_ctr(&tnl->_WindowMap);

    tnl->NeedNdcCoords = GL_TRUE;
    tnl->AllowVertexFog = GL_TRUE;
    tnl->AllowPixelFog = GL_TRUE;

    /* Set a few default values in the driver struct.
     */
    tnl->Driver.Render.PrimTabElts = _tnl_render_tab_elts;
    tnl->Driver.Render.PrimTabVerts = _tnl_render_tab_verts;
    tnl->Driver.NotifyMaterialChange = _tnl_validate_shine_tables;

    tnl->nr_blocks = 0;

    /* Lighting miscellaneous */
    tnl->_ShineTabList = MALLOC_STRUCT( tnl_shine_tab );
    make_empty_list( tnl->_ShineTabList );
    /* Allocate 10 (arbitrary) shininess lookup tables */
    for (i = 0 ; i < 10 ; i++) {
        struct tnl_shine_tab *s = MALLOC_STRUCT( tnl_shine_tab );
        s->shininess = -1;
        s->refcount = 0;
        insert_at_tail( tnl->_ShineTabList, s );
    }

    _math_init_transformation();
    _math_init_translate();

    /* Keep our list of tnl_vertex_array inputs */
    _tnl_init_inputs(&tnl->draw_arrays);

    return GL_TRUE;
}


void
_tnl_DestroyContext( struct gl_context *ctx )
{
    struct tnl_shine_tab *s, *tmps;
    TNLcontext *tnl = TNL_CONTEXT(ctx);

    /* Free lighting shininess exponentiation table */
    foreach_s( s, tmps, tnl->_ShineTabList ) {
        free( s );
    }
    free( tnl->_ShineTabList );

    _tnl_destroy_pipeline( ctx );

    free(tnl);
    ctx->swtnl_context = NULL;
}


void
_tnl_InvalidateState( struct gl_context *ctx, GLuint new_state )
{
    TNLcontext *tnl = TNL_CONTEXT(ctx);
    const struct gl_program *vp = ctx->VertexProgram._Current;
    const struct gl_program *fp = ctx->FragmentProgram._Current;
    GLuint i;

    if (new_state & (_NEW_LIGHT_CONSTANTS | _NEW_MATERIAL))
        _mesa_update_light_materials(ctx);

    if (new_state & (_NEW_HINT | _NEW_PROGRAM)) {
        assert(tnl->AllowVertexFog || tnl->AllowPixelFog);
        tnl->_DoVertexFog = ((tnl->AllowVertexFog && (ctx->Hint.Fog != GL_NICEST))
                             || !tnl->AllowPixelFog) && !fp;
    }

    tnl->pipeline.new_state |= new_state;

    /* Calculate tnl->render_inputs.  This bitmask indicates which vertex
     * attributes need to be emitted to the rasterizer.
     */
    tnl->render_inputs_bitset = BITFIELD64_BIT(_TNL_ATTRIB_POS);

    if (!fp || (fp->info.inputs_read & VARYING_BIT_COL0)) {
        tnl->render_inputs_bitset |= BITFIELD64_BIT(_TNL_ATTRIB_COLOR0);
    }

    if (_mesa_need_secondary_color(ctx))
        tnl->render_inputs_bitset |= BITFIELD64_BIT(_TNL_ATTRIB_COLOR1);

    for (i = 0; i < ctx->Const.MaxTextureCoordUnits; i++) {
        if (ctx->Texture._EnabledCoordUnits & (1 << i) ||
            (fp && fp->info.inputs_read & VARYING_BIT_TEX(i)) ||
            _mesa_ati_fragment_shader_enabled(ctx)) {
            tnl->render_inputs_bitset |= BITFIELD64_BIT(_TNL_ATTRIB_TEX(i));
        }
    }

    if (ctx->Fog.Enabled
        || (fp != NULL && (fp->info.inputs_read & VARYING_BIT_FOGC) != 0)) {
        /* Either fixed-function fog or a fragment program needs fog coord.
         */
        tnl->render_inputs_bitset |= BITFIELD64_BIT(_TNL_ATTRIB_FOG);
    }

    if (ctx->Polygon.FrontMode != GL_FILL ||
        ctx->Polygon.BackMode != GL_FILL)
        tnl->render_inputs_bitset |= BITFIELD64_BIT(_TNL_ATTRIB_EDGEFLAG);

    if (ctx->RenderMode == GL_FEEDBACK)
        tnl->render_inputs_bitset |= BITFIELD64_BIT(_TNL_ATTRIB_TEX0);

    if (ctx->Point._Attenuated || ctx->VertexProgram.PointSizeEnabled)
        tnl->render_inputs_bitset |= BITFIELD64_BIT(_TNL_ATTRIB_POINTSIZE);

    /* check for varying vars which are written by the vertex program */
    if (vp) {
        GLuint i;
        for (i = 0; i < MAX_VARYING; i++) {
            if (vp->info.outputs_written &
                BITFIELD64_BIT(VARYING_SLOT_VAR0 + i)) {
                tnl->render_inputs_bitset |= BITFIELD64_BIT(_TNL_ATTRIB_GENERIC(i));
            }
        }
    }

    if (new_state & (_NEW_VIEWPORT | _NEW_BUFFERS)) {
        float scale[3], translate[3];
        _mesa_get_viewport_xform(ctx, 0, scale, translate);
        _math_matrix_viewport(&tnl->_WindowMap, scale, translate,
                              ctx->DrawBuffer->_DepthMaxF);
    }
}


void
_tnl_wakeup( struct gl_context *ctx )
{
    /* Assume we haven't been getting state updates either:
     */
    _tnl_InvalidateState( ctx, ~0 );

#if 0
    if (ctx->Light.ColorMaterialEnabled) {
      _mesa_update_color_material( ctx,
				   ctx->Current.Attrib[VERT_ATTRIB_COLOR0] );
   }
#endif
}




/**
 * Drivers call this function to tell the TCL module whether or not
 * it wants Normalized Device Coords (NDC) computed.  I.e. whether
 * we should "Divide-by-W".  Software renders will want that.
 */
void
_tnl_need_projected_coords( struct gl_context *ctx, GLboolean mode )
{
    TNLcontext *tnl = TNL_CONTEXT(ctx);
    tnl->NeedNdcCoords = mode;
}

void
_tnl_allow_vertex_fog( struct gl_context *ctx, GLboolean value )
{
    TNLcontext *tnl = TNL_CONTEXT(ctx);
    tnl->AllowVertexFog = value;
    tnl->_DoVertexFog = ((tnl->AllowVertexFog && (ctx->Hint.Fog != GL_NICEST))
                         || !tnl->AllowPixelFog) && !ctx->FragmentProgram._Current;

}

void
_tnl_allow_pixel_fog( struct gl_context *ctx, GLboolean value )
{
    TNLcontext *tnl = TNL_CONTEXT(ctx);
    tnl->AllowPixelFog = value;
    tnl->_DoVertexFog = ((tnl->AllowVertexFog && (ctx->Hint.Fog != GL_NICEST))
                         || !tnl->AllowPixelFog) && !ctx->FragmentProgram._Current;
}

