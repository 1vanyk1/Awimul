#include "../include/c99_math.h"
#include "../main/errors.h"
#include "../main/glheader.h"
#include "../main/macros.h"

#include "s_context.h"
#include "s_fog.h"


/**
 * Used to convert current raster distance to a fog factor in [0,1].
 */
GLfloat
_swrast_z_to_fogfactor(struct gl_context *ctx, GLfloat z)
{
    GLfloat d, f;

    switch (ctx->Fog.Mode) {
        case GL_LINEAR:
            if (ctx->Fog.Start == ctx->Fog.End)
                d = 1.0F;
            else
                d = 1.0F / (ctx->Fog.End - ctx->Fog.Start);
            f = (ctx->Fog.End - z) * d;
            return CLAMP(f, 0.0F, 1.0F);
        case GL_EXP:
            d = ctx->Fog.Density;
            f = expf(-d * z);
            f = CLAMP(f, 0.0F, 1.0F);
            return f;
        case GL_EXP2:
            d = ctx->Fog.Density;
            f = expf(-(d * d * z * z));
            f = CLAMP(f, 0.0F, 1.0F);
            return f;
        default:
            _mesa_problem(ctx, "Bad fog mode in _swrast_z_to_fogfactor");
            return 0.0;
    }
}


#define LINEAR_FOG(f, coord)  f = (fogEnd - coord) * fogScale

#define EXP_FOG(f, coord)  f = expf(density * coord)

#define EXP2_FOG(f, coord)				\
do {							\
   GLfloat tmp = negDensitySquared * coord * coord;	\
   if (tmp < FLT_MIN_10_EXP)				\
      tmp = FLT_MIN_10_EXP;				\
   f = expf(tmp);					\
 } while(0)


#define BLEND_FOG(f, coord)  f = coord



/**
 * Template code for computing fog blend factor and applying it to colors.
 * \param TYPE  either GLubyte, GLushort or GLfloat.
 * \param COMPUTE_F  code to compute the fog blend factor, f.
 */
#define FOG_LOOP(TYPE, FOG_FUNC)						\
if (span->arrayAttribs & VARYING_BIT_FOGC) {					\
   GLuint i;									\
   for (i = 0; i < span->end; i++) {						\
      const GLfloat fogCoord = span->array->attribs[VARYING_SLOT_FOGC][i][0];	\
      const GLfloat c = fabsf(fogCoord);					\
      GLfloat f, oneMinusF;							\
      FOG_FUNC(f, c);								\
      f = CLAMP(f, 0.0F, 1.0F);							\
      oneMinusF = 1.0F - f;							\
      rgba[i][RCOMP] = (TYPE) (f * rgba[i][RCOMP] + oneMinusF * rFog);		\
      rgba[i][GCOMP] = (TYPE) (f * rgba[i][GCOMP] + oneMinusF * gFog);		\
      rgba[i][BCOMP] = (TYPE) (f * rgba[i][BCOMP] + oneMinusF * bFog);		\
   }										\
}										\
else {										\
   const GLfloat fogStep = span->attrStepX[VARYING_SLOT_FOGC][0];		\
   GLfloat fogCoord = span->attrStart[VARYING_SLOT_FOGC][0];			\
   const GLfloat wStep = span->attrStepX[VARYING_SLOT_POS][3];			\
   GLfloat w = span->attrStart[VARYING_SLOT_POS][3];				\
   GLuint i;									\
   for (i = 0; i < span->end; i++) {						\
      const GLfloat c = fabsf(fogCoord) / w;					\
      GLfloat f, oneMinusF;							\
      FOG_FUNC(f, c);								\
      f = CLAMP(f, 0.0F, 1.0F);							\
      oneMinusF = 1.0F - f;							\
      rgba[i][RCOMP] = (TYPE) (f * rgba[i][RCOMP] + oneMinusF * rFog);		\
      rgba[i][GCOMP] = (TYPE) (f * rgba[i][GCOMP] + oneMinusF * gFog);		\
      rgba[i][BCOMP] = (TYPE) (f * rgba[i][BCOMP] + oneMinusF * bFog);		\
      fogCoord += fogStep;							\
      w += wStep;								\
   }										\
}

/**
 * Apply fog to a span of RGBA pixels.
 * The fog value are either in the span->array->fog array or interpolated from
 * the fog/fogStep values.
 * They fog values are either fog coordinates (Z) or fog blend factors.
 * _PreferPixelFog should be in sync with that state!
 */
void
_swrast_fog_rgba_span( const struct gl_context *ctx, SWspan *span )
{
    const SWcontext *swrast = CONST_SWRAST_CONTEXT(ctx);
    GLfloat rFog, gFog, bFog;

    assert(swrast->_FogEnabled);
    assert(span->arrayMask & SPAN_RGBA);

    /* compute (scaled) fog color */
    if (span->array->ChanType == GL_UNSIGNED_BYTE) {
        rFog = ctx->Fog.Color[RCOMP] * 255.0F;
        gFog = ctx->Fog.Color[GCOMP] * 255.0F;
        bFog = ctx->Fog.Color[BCOMP] * 255.0F;
    }
    else if (span->array->ChanType == GL_UNSIGNED_SHORT) {
        rFog = ctx->Fog.Color[RCOMP] * 65535.0F;
        gFog = ctx->Fog.Color[GCOMP] * 65535.0F;
        bFog = ctx->Fog.Color[BCOMP] * 65535.0F;
    }
    else {
        rFog = ctx->Fog.Color[RCOMP];
        gFog = ctx->Fog.Color[GCOMP];
        bFog = ctx->Fog.Color[BCOMP];
    }

    if (swrast->_PreferPixelFog) {
        /* The span's fog values are fog coordinates, now compute blend factors
         * and blend the fragment colors with the fog color.
         */
        switch (ctx->Fog.Mode) {
            case GL_LINEAR:
            {
                const GLfloat fogEnd = ctx->Fog.End;
                const GLfloat fogScale = (ctx->Fog.Start == ctx->Fog.End)
                                         ? 1.0F : 1.0F / (ctx->Fog.End - ctx->Fog.Start);
                if (span->array->ChanType == GL_UNSIGNED_BYTE) {
                    GLubyte (*rgba)[4] = span->array->rgba8;
                    FOG_LOOP(GLubyte, LINEAR_FOG);
                }
                else if (span->array->ChanType == GL_UNSIGNED_SHORT) {
                    GLushort (*rgba)[4] = span->array->rgba16;
                    FOG_LOOP(GLushort, LINEAR_FOG);
                }
                else {
                    GLfloat (*rgba)[4] = span->array->attribs[VARYING_SLOT_COL0];
                    assert(span->array->ChanType == GL_FLOAT);
                    FOG_LOOP(GLfloat, LINEAR_FOG);
                }
            }
                break;

            case GL_EXP:
            {
                const GLfloat density = -ctx->Fog.Density;
                if (span->array->ChanType == GL_UNSIGNED_BYTE) {
                    GLubyte (*rgba)[4] = span->array->rgba8;
                    FOG_LOOP(GLubyte, EXP_FOG);
                }
                else if (span->array->ChanType == GL_UNSIGNED_SHORT) {
                    GLushort (*rgba)[4] = span->array->rgba16;
                    FOG_LOOP(GLushort, EXP_FOG);
                }
                else {
                    GLfloat (*rgba)[4] = span->array->attribs[VARYING_SLOT_COL0];
                    assert(span->array->ChanType == GL_FLOAT);
                    FOG_LOOP(GLfloat, EXP_FOG);
                }
            }
                break;

            case GL_EXP2:
            {
                const GLfloat negDensitySquared = -ctx->Fog.Density * ctx->Fog.Density;
                if (span->array->ChanType == GL_UNSIGNED_BYTE) {
                    GLubyte (*rgba)[4] = span->array->rgba8;
                    FOG_LOOP(GLubyte, EXP2_FOG);
                }
                else if (span->array->ChanType == GL_UNSIGNED_SHORT) {
                    GLushort (*rgba)[4] = span->array->rgba16;
                    FOG_LOOP(GLushort, EXP2_FOG);
                }
                else {
                    GLfloat (*rgba)[4] = span->array->attribs[VARYING_SLOT_COL0];
                    assert(span->array->ChanType == GL_FLOAT);
                    FOG_LOOP(GLfloat, EXP2_FOG);
                }
            }
                break;

            default:
                _mesa_problem(ctx, "Bad fog mode in _swrast_fog_rgba_span");
                return;
        }
    }
    else {
        /* The span's fog start/step/array values are blend factors in [0,1].
         * They were previously computed per-vertex.
         */
        if (span->array->ChanType == GL_UNSIGNED_BYTE) {
            GLubyte (*rgba)[4] = span->array->rgba8;
            FOG_LOOP(GLubyte, BLEND_FOG);
        }
        else if (span->array->ChanType == GL_UNSIGNED_SHORT) {
            GLushort (*rgba)[4] = span->array->rgba16;
            FOG_LOOP(GLushort, BLEND_FOG);
        }
        else {
            GLfloat (*rgba)[4] = span->array->attribs[VARYING_SLOT_COL0];
            assert(span->array->ChanType == GL_FLOAT);
            FOG_LOOP(GLfloat, BLEND_FOG);
        }
    }
}
