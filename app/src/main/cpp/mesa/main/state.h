#ifndef STATE_H
#define STATE_H

#include "mtypes.h"

extern void
_mesa_update_allow_draw_out_of_order(struct gl_context *ctx);

extern void
_mesa_update_primitive_id_is_unused(struct gl_context *ctx);

extern void
_mesa_update_state(struct gl_context *ctx);

/* As above but can only be called between _mesa_lock_context_textures() and
 * _mesa_unlock_context_textures().
 */
extern void
_mesa_update_state_locked(struct gl_context *ctx);


extern void
_mesa_set_vp_override(struct gl_context *ctx, GLboolean flag);


/**
 * Update ctx->VertexProgram._VPMode.
 */
extern void
_mesa_update_vertex_processing_mode(struct gl_context *ctx);

extern void
_mesa_reset_vertex_processing_mode(struct gl_context *ctx);


static inline bool
_mesa_ati_fragment_shader_enabled(const struct gl_context *ctx)
{
    return ctx->ATIFragmentShader.Enabled &&
           ctx->ATIFragmentShader.Current->Instructions[0];
}

/**
 * Is the secondary color needed?
 */
static inline GLboolean
_mesa_need_secondary_color(const struct gl_context *ctx)
{
    if (ctx->Light.Enabled &&
        ctx->Light.Model.ColorControl == GL_SEPARATE_SPECULAR_COLOR)
        return GL_TRUE;

    if (ctx->Fog.ColorSumEnabled)
        return GL_TRUE;

    if (ctx->VertexProgram._Current &&
        (ctx->VertexProgram._Current != ctx->VertexProgram._TnlProgram) &&
        (ctx->VertexProgram._Current->info.inputs_read & VERT_BIT_COLOR1))
        return GL_TRUE;

    if (ctx->FragmentProgram._Current &&
        (ctx->FragmentProgram._Current != ctx->FragmentProgram._TexEnvProgram) &&
        (ctx->FragmentProgram._Current->info.inputs_read & VARYING_BIT_COL1))
        return GL_TRUE;

    if (_mesa_ati_fragment_shader_enabled(ctx))
        return GL_TRUE;

    return GL_FALSE;
}

static inline bool
_mesa_arb_vertex_program_enabled(const struct gl_context *ctx)
{
    return ctx->VertexProgram.Enabled &&
           ctx->VertexProgram.Current->arb.Instructions;
}

/** Compute two sided lighting state for fixed function or programs. */
static inline bool
_mesa_vertex_program_two_side_enabled(const struct gl_context *ctx)
{
    if (ctx->_Shader->CurrentProgram[MESA_SHADER_VERTEX] ||
        _mesa_arb_vertex_program_enabled(ctx))
        return ctx->VertexProgram.TwoSideEnabled;

    return ctx->Light.Enabled && ctx->Light.Model.TwoSide;
}

/** Return 0=GL_CCW or 1=GL_CW */
static inline bool
_mesa_polygon_get_front_bit(const struct gl_context *ctx)
{
    if (ctx->Transform.ClipOrigin == GL_LOWER_LEFT)
        return ctx->Polygon.FrontFace == GL_CW;

    return ctx->Polygon.FrontFace == GL_CCW;
}

static inline bool
_mesa_arb_fragment_program_enabled(const struct gl_context *ctx)
{
    return ctx->FragmentProgram.Enabled &&
           ctx->FragmentProgram.Current->arb.Instructions;
}

#endif
