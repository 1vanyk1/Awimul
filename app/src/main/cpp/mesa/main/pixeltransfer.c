#include "glheader.h"
#include "macros.h"
#include "pixeltransfer.h"

#include "mtypes.h"
#include "../util/rounding.h"


/*
 * Apply scale and bias factors to an array of RGBA pixels.
 */
void
_mesa_scale_and_bias_rgba(GLuint n, GLfloat rgba[][4],
                          GLfloat rScale, GLfloat gScale,
                          GLfloat bScale, GLfloat aScale,
                          GLfloat rBias, GLfloat gBias,
                          GLfloat bBias, GLfloat aBias)
{
    if (rScale != 1.0F || rBias != 0.0F) {
        GLuint i;
        for (i = 0; i < n; i++) {
            rgba[i][RCOMP] = rgba[i][RCOMP] * rScale + rBias;
        }
    }
    if (gScale != 1.0F || gBias != 0.0F) {
        GLuint i;
        for (i = 0; i < n; i++) {
            rgba[i][GCOMP] = rgba[i][GCOMP] * gScale + gBias;
        }
    }
    if (bScale != 1.0F || bBias != 0.0F) {
        GLuint i;
        for (i = 0; i < n; i++) {
            rgba[i][BCOMP] = rgba[i][BCOMP] * bScale + bBias;
        }
    }
    if (aScale != 1.0F || aBias != 0.0F) {
        GLuint i;
        for (i = 0; i < n; i++) {
            rgba[i][ACOMP] = rgba[i][ACOMP] * aScale + aBias;
        }
    }
}


/*
 * Apply pixel mapping to an array of floating point RGBA pixels.
 */
void
_mesa_map_rgba( const struct gl_context *ctx, GLuint n, GLfloat rgba[][4] )
{
    const GLfloat rscale = (GLfloat) (ctx->PixelMaps.RtoR.Size - 1);
    const GLfloat gscale = (GLfloat) (ctx->PixelMaps.GtoG.Size - 1);
    const GLfloat bscale = (GLfloat) (ctx->PixelMaps.BtoB.Size - 1);
    const GLfloat ascale = (GLfloat) (ctx->PixelMaps.AtoA.Size - 1);
    const GLfloat *rMap = ctx->PixelMaps.RtoR.Map;
    const GLfloat *gMap = ctx->PixelMaps.GtoG.Map;
    const GLfloat *bMap = ctx->PixelMaps.BtoB.Map;
    const GLfloat *aMap = ctx->PixelMaps.AtoA.Map;
    GLuint i;
    for (i=0;i<n;i++) {
        GLfloat r = CLAMP(rgba[i][RCOMP], 0.0F, 1.0F);
        GLfloat g = CLAMP(rgba[i][GCOMP], 0.0F, 1.0F);
        GLfloat b = CLAMP(rgba[i][BCOMP], 0.0F, 1.0F);
        GLfloat a = CLAMP(rgba[i][ACOMP], 0.0F, 1.0F);
        rgba[i][RCOMP] = rMap[(int)_mesa_lroundevenf(r * rscale)];
        rgba[i][GCOMP] = gMap[(int)_mesa_lroundevenf(g * gscale)];
        rgba[i][BCOMP] = bMap[(int)_mesa_lroundevenf(b * bscale)];
        rgba[i][ACOMP] = aMap[(int)_mesa_lroundevenf(a * ascale)];
    }
}

/*
 * Map color indexes to float rgba values.
 */
void
_mesa_map_ci_to_rgba( const struct gl_context *ctx, GLuint n,
                      const GLuint index[], GLfloat rgba[][4] )
{
    GLuint rmask = ctx->PixelMaps.ItoR.Size - 1;
    GLuint gmask = ctx->PixelMaps.ItoG.Size - 1;
    GLuint bmask = ctx->PixelMaps.ItoB.Size - 1;
    GLuint amask = ctx->PixelMaps.ItoA.Size - 1;
    const GLfloat *rMap = ctx->PixelMaps.ItoR.Map;
    const GLfloat *gMap = ctx->PixelMaps.ItoG.Map;
    const GLfloat *bMap = ctx->PixelMaps.ItoB.Map;
    const GLfloat *aMap = ctx->PixelMaps.ItoA.Map;
    GLuint i;
    for (i=0;i<n;i++) {
        rgba[i][RCOMP] = rMap[index[i] & rmask];
        rgba[i][GCOMP] = gMap[index[i] & gmask];
        rgba[i][BCOMP] = bMap[index[i] & bmask];
        rgba[i][ACOMP] = aMap[index[i] & amask];
    }
}


void
_mesa_scale_and_bias_depth(const struct gl_context *ctx, GLuint n,
                           GLfloat depthValues[])
{
    const GLfloat scale = ctx->Pixel.DepthScale;
    const GLfloat bias = ctx->Pixel.DepthBias;
    GLuint i;
    for (i = 0; i < n; i++) {
        GLfloat d = depthValues[i] * scale + bias;
        depthValues[i] = CLAMP(d, 0.0F, 1.0F);
    }
}


void
_mesa_scale_and_bias_depth_uint(const struct gl_context *ctx, GLuint n,
                                GLuint depthValues[])
{
    const GLdouble max = (double) 0xffffffff;
    const GLdouble scale = ctx->Pixel.DepthScale;
    const GLdouble bias = ctx->Pixel.DepthBias * max;
    GLuint i;
    for (i = 0; i < n; i++) {
        GLdouble d = (GLdouble) depthValues[i] * scale + bias;
        d = CLAMP(d, 0.0, max);
        depthValues[i] = (GLuint) d;
    }
}

/**
 * Apply various pixel transfer operations to an array of RGBA pixels
 * as indicated by the transferOps bitmask
 */
void
_mesa_apply_rgba_transfer_ops(struct gl_context *ctx, GLbitfield transferOps,
                              GLuint n, GLfloat rgba[][4])
{
    /* scale & bias */
    if (transferOps & IMAGE_SCALE_BIAS_BIT) {
        _mesa_scale_and_bias_rgba(n, rgba,
                                  ctx->Pixel.RedScale, ctx->Pixel.GreenScale,
                                  ctx->Pixel.BlueScale, ctx->Pixel.AlphaScale,
                                  ctx->Pixel.RedBias, ctx->Pixel.GreenBias,
                                  ctx->Pixel.BlueBias, ctx->Pixel.AlphaBias);
    }
    /* color map lookup */
    if (transferOps & IMAGE_MAP_COLOR_BIT) {
        _mesa_map_rgba( ctx, n, rgba );
    }

    /* clamping to [0,1] */
    if (transferOps & IMAGE_CLAMP_BIT) {
        GLuint i;
        for (i = 0; i < n; i++) {
            rgba[i][RCOMP] = CLAMP(rgba[i][RCOMP], 0.0F, 1.0F);
            rgba[i][GCOMP] = CLAMP(rgba[i][GCOMP], 0.0F, 1.0F);
            rgba[i][BCOMP] = CLAMP(rgba[i][BCOMP], 0.0F, 1.0F);
            rgba[i][ACOMP] = CLAMP(rgba[i][ACOMP], 0.0F, 1.0F);
        }
    }
}


/*
 * Apply color index shift and offset to an array of pixels.
 */
void
_mesa_shift_and_offset_ci(const struct gl_context *ctx,
                          GLuint n, GLuint indexes[])
{
    GLint shift = ctx->Pixel.IndexShift;
    GLint offset = ctx->Pixel.IndexOffset;
    GLuint i;
    if (shift > 0) {
        for (i=0;i<n;i++) {
            indexes[i] = (indexes[i] << shift) + offset;
        }
    }
    else if (shift < 0) {
        shift = -shift;
        for (i=0;i<n;i++) {
            indexes[i] = (indexes[i] >> shift) + offset;
        }
    }
    else {
        for (i=0;i<n;i++) {
            indexes[i] = indexes[i] + offset;
        }
    }
}



/**
 * Apply color index shift, offset and table lookup to an array
 * of color indexes;
 */
void
_mesa_apply_ci_transfer_ops(const struct gl_context *ctx,
                            GLbitfield transferOps,
                            GLuint n, GLuint indexes[])
{
    if (transferOps & IMAGE_SHIFT_OFFSET_BIT) {
        _mesa_shift_and_offset_ci(ctx, n, indexes);
    }
    if (transferOps & IMAGE_MAP_COLOR_BIT) {
        const GLuint mask = ctx->PixelMaps.ItoI.Size - 1;
        GLuint i;
        for (i = 0; i < n; i++) {
            const GLuint j = indexes[i] & mask;
            indexes[i] = _mesa_lroundevenf(ctx->PixelMaps.ItoI.Map[j]);
        }
    }
}


/**
 * Apply stencil index shift, offset and table lookup to an array
 * of stencil values.
 */
void
_mesa_apply_stencil_transfer_ops(const struct gl_context *ctx, GLuint n,
                                 GLubyte stencil[])
{
    if (ctx->Pixel.IndexShift != 0 || ctx->Pixel.IndexOffset != 0) {
        const GLint offset = ctx->Pixel.IndexOffset;
        GLint shift = ctx->Pixel.IndexShift;
        GLuint i;
        if (shift > 0) {
            for (i = 0; i < n; i++) {
                stencil[i] = (stencil[i] << shift) + offset;
            }
        }
        else if (shift < 0) {
            shift = -shift;
            for (i = 0; i < n; i++) {
                stencil[i] = (stencil[i] >> shift) + offset;
            }
        }
        else {
            for (i = 0; i < n; i++) {
                stencil[i] = stencil[i] + offset;
            }
        }
    }
    if (ctx->Pixel.MapStencilFlag) {
        GLuint mask = ctx->PixelMaps.StoS.Size - 1;
        GLuint i;
        for (i = 0; i < n; i++) {
            stencil[i] = (GLubyte) ctx->PixelMaps.StoS.Map[ stencil[i] & mask ];
        }
    }
}
