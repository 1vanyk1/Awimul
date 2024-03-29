#ifndef STENCIL_H
#define STENCIL_H


#include "glheader.h"
#include "macros.h"

struct gl_context;

extern void GLAPIENTRY
_mesa_ClearStencil( GLint s );


void GLAPIENTRY
_mesa_StencilFunc_no_error(GLenum func, GLint ref, GLuint mask);

extern void GLAPIENTRY
_mesa_StencilFunc(GLenum func, GLint ref, GLuint mask);


extern void GLAPIENTRY
_mesa_StencilMask( GLuint mask );

void GLAPIENTRY
_mesa_StencilOp_no_error(GLenum fail, GLenum zfail, GLenum zpass);

extern void GLAPIENTRY
_mesa_StencilOp(GLenum fail, GLenum zfail, GLenum zpass);


extern void GLAPIENTRY
_mesa_ActiveStencilFaceEXT(GLenum face);

void GLAPIENTRY
_mesa_StencilOpSeparate_no_error(GLenum face, GLenum fail, GLenum zfail,
                                 GLenum zpass);

extern void GLAPIENTRY
_mesa_StencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);


void GLAPIENTRY
_mesa_StencilFuncSeparate_no_error(GLenum face, GLenum func, GLint ref,
                                   GLuint mask);

extern void GLAPIENTRY
_mesa_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);


extern void GLAPIENTRY
_mesa_StencilFuncSeparateATI(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);

void GLAPIENTRY
_mesa_StencilMaskSeparate_no_error(GLenum face, GLuint mask);

extern void GLAPIENTRY
_mesa_StencilMaskSeparate(GLenum face, GLuint mask);

extern void
_mesa_init_stencil( struct gl_context * ctx );

/* From the GL 4.3 spec, 17.3.5:
 *    "Stencil comparison operations and queries of <ref> clamp its value
 *    to the range [0, 2^s-1], where <s> is the number of bits in the
 *    stencil buffer attached to the draw framebuffer."
 */

static inline GLint
_mesa_get_stencil_ref(struct gl_context const *ctx, int face)
{
    GLint stencilMax = (1 << ctx->DrawBuffer->Visual.stencilBits) - 1;
    GLint ref = ctx->Stencil.Ref[face];
    return CLAMP(ref, 0, stencilMax);
}

static inline bool
_mesa_stencil_is_enabled(const struct gl_context *ctx)
{
    return ctx->Stencil.Enabled &&
           ctx->DrawBuffer->Visual.stencilBits > 0;
}

static inline bool
_mesa_stencil_is_two_sided(const struct gl_context *ctx)
{
    const int face = ctx->Stencil._BackFace;

    return _mesa_stencil_is_enabled(ctx) &&
           (ctx->Stencil.Function[0] != ctx->Stencil.Function[face] ||
            ctx->Stencil.FailFunc[0] != ctx->Stencil.FailFunc[face] ||
            ctx->Stencil.ZPassFunc[0] != ctx->Stencil.ZPassFunc[face] ||
            ctx->Stencil.ZFailFunc[0] != ctx->Stencil.ZFailFunc[face] ||
            ctx->Stencil.Ref[0] != ctx->Stencil.Ref[face] ||
            ctx->Stencil.ValueMask[0] != ctx->Stencil.ValueMask[face] ||
            ctx->Stencil.WriteMask[0] != ctx->Stencil.WriteMask[face]);
}

static inline bool
_mesa_stencil_is_write_enabled(const struct gl_context *ctx, bool is_two_sided)
{
    return _mesa_stencil_is_enabled(ctx) &&
           (ctx->Stencil.WriteMask[0] != 0 ||
            (is_two_sided &&
             ctx->Stencil.WriteMask[ctx->Stencil._BackFace] != 0));
}

#endif
