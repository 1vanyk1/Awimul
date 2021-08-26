#include "../main/glheader.h"
#include "../main/context.h"
#include "../main/macros.h"

#include "s_alpha.h"
#include "s_context.h"


#define ALPHA_TEST(ALPHA, LOOP_CODE)		\
do {						\
   switch (ctx->Color.AlphaFunc) {		\
      case GL_LESS:				\
         for (i = 0; i < n; i++) {		\
            mask[i] &= (ALPHA < ref);		\
            LOOP_CODE;				\
         }					\
         break;					\
      case GL_LEQUAL:				\
         for (i = 0; i < n; i++) {		\
            mask[i] &= (ALPHA <= ref);		\
            LOOP_CODE;				\
         }					\
         break;					\
      case GL_GEQUAL:				\
         for (i = 0; i < n; i++) {		\
            mask[i] &= (ALPHA >= ref);		\
            LOOP_CODE;				\
         }					\
         break;					\
      case GL_GREATER:				\
         for (i = 0; i < n; i++) {		\
            mask[i] &= (ALPHA > ref);		\
            LOOP_CODE;				\
         }					\
         break;					\
      case GL_NOTEQUAL:				\
         for (i = 0; i < n; i++) {		\
            mask[i] &= (ALPHA != ref);		\
            LOOP_CODE;				\
         }					\
         break;					\
      case GL_EQUAL:				\
         for (i = 0; i < n; i++) {		\
            mask[i] &= (ALPHA == ref);		\
            LOOP_CODE;				\
         }					\
         break;					\
      default:					\
         _mesa_problem(ctx, "Invalid alpha test in _swrast_alpha_test" ); \
         return 0;				\
   }						\
} while (0)



/**
 * Perform the alpha test for an array of pixels.
 * For pixels that fail the test, mask[i] will be set to 0.
 * \return  0 if all pixels in the span failed the alpha test,
 *          1 if one or more pixels passed the alpha test.
 */
GLint
_swrast_alpha_test(const struct gl_context *ctx, SWspan *span)
{
    const GLuint n = span->end;
    GLubyte *mask = span->array->mask;
    GLuint i;

    if (ctx->Color.AlphaFunc == GL_ALWAYS) {
        /* do nothing */
        return 1;
    }
    else if (ctx->Color.AlphaFunc == GL_NEVER) {
        /* All pixels failed - caller should check for this return value and
         * act accordingly.
         */
        span->writeAll = GL_FALSE;
        return 0;
    }

    if (span->arrayMask & SPAN_RGBA) {
        /* Use array's alpha values */
        if (span->array->ChanType == GL_UNSIGNED_BYTE) {
            GLubyte (*rgba)[4] = span->array->rgba8;
            GLubyte ref;
            CLAMPED_FLOAT_TO_UBYTE(ref, ctx->Color.AlphaRef);
            ALPHA_TEST(rgba[i][ACOMP], ;);
        }
        else if (span->array->ChanType == GL_UNSIGNED_SHORT) {
            GLushort (*rgba)[4] = span->array->rgba16;
            GLushort ref;
            CLAMPED_FLOAT_TO_USHORT(ref, ctx->Color.AlphaRef);
            ALPHA_TEST(rgba[i][ACOMP], ;);
        }
        else {
            GLfloat (*rgba)[4] = span->array->attribs[VARYING_SLOT_COL0];
            const GLfloat ref = ctx->Color.AlphaRef;
            ALPHA_TEST(rgba[i][ACOMP], ;);
        }
    }
    else {
        /* Interpolate alpha values */
        assert(span->interpMask & SPAN_RGBA);
        if (span->array->ChanType == GL_UNSIGNED_BYTE) {
            const GLfixed alphaStep = span->alphaStep;
            GLfixed alpha = span->alpha;
            GLubyte ref;
            CLAMPED_FLOAT_TO_UBYTE(ref, ctx->Color.AlphaRef);
            ALPHA_TEST(FixedToInt(alpha), alpha += alphaStep);
        }
        else if (span->array->ChanType == GL_UNSIGNED_SHORT) {
            const GLfixed alphaStep = span->alphaStep;
            GLfixed alpha = span->alpha;
            GLushort ref;
            CLAMPED_FLOAT_TO_USHORT(ref, ctx->Color.AlphaRef);
            ALPHA_TEST(FixedToInt(alpha), alpha += alphaStep);
        }
        else {
            const GLfloat alphaStep = FixedToFloat(span->alphaStep);
            GLfloat alpha = FixedToFloat(span->alpha);
            const GLfloat ref = ctx->Color.AlphaRef;
            ALPHA_TEST(alpha, alpha += alphaStep);
        }
    }

    span->writeAll = GL_FALSE;

    /* XXX examine mask[] values? */
    return 1;
}
