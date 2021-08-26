#include "../main/glheader.h"
#include "../main/macros.h"

#include "s_context.h"
#include "s_masking.h"
#include "s_span.h"


/**
 * Apply the color mask to a span of rgba values.
 */
void
_swrast_mask_rgba_span(struct gl_context *ctx, struct gl_renderbuffer *rb,
                       SWspan *span, GLuint buf)
{
    const GLuint n = span->end;
    void *rbPixels;

    assert(n < SWRAST_MAX_WIDTH);
    assert(span->arrayMask & SPAN_RGBA);

    rbPixels = _swrast_get_dest_rgba(ctx, rb, span);

    /*
     * Do component masking.
     * Note that we're not using span->array->mask[] here.  We could...
     */
    if (span->array->ChanType == GL_UNSIGNED_BYTE) {
        const GLubyte colormask[4] = {
                GET_COLORMASK_BIT(ctx->Color.ColorMask, buf, 0) ? 0xff : 0,
                GET_COLORMASK_BIT(ctx->Color.ColorMask, buf, 1) ? 0xff : 0,
                GET_COLORMASK_BIT(ctx->Color.ColorMask, buf, 2) ? 0xff : 0,
                GET_COLORMASK_BIT(ctx->Color.ColorMask, buf, 3) ? 0xff : 0,
        };
        GLuint srcMask;
        memcpy(&srcMask, colormask, sizeof(srcMask));
        const GLuint dstMask = ~srcMask;
        const GLuint *dst = (const GLuint *) rbPixels;
        GLuint *src = (GLuint *) span->array->rgba8;
        GLuint i;
        for (i = 0; i < n; i++) {
            src[i] = (src[i] & srcMask) | (dst[i] & dstMask);
        }
    }
    else if (span->array->ChanType == GL_UNSIGNED_SHORT) {
        /* 2-byte components */
        /* XXX try to use 64-bit arithmetic someday */
        const GLushort rMask = GET_COLORMASK_BIT(ctx->Color.ColorMask, buf, 0) ? 0xffff : 0x0;
        const GLushort gMask = GET_COLORMASK_BIT(ctx->Color.ColorMask, buf, 1) ? 0xffff : 0x0;
        const GLushort bMask = GET_COLORMASK_BIT(ctx->Color.ColorMask, buf, 2) ? 0xffff : 0x0;
        const GLushort aMask = GET_COLORMASK_BIT(ctx->Color.ColorMask, buf, 3) ? 0xffff : 0x0;
        const GLushort (*dst)[4] = (const GLushort (*)[4]) rbPixels;
        GLushort (*src)[4] = span->array->rgba16;
        GLuint i;
        for (i = 0; i < n; i++) {
            src[i][RCOMP] = (src[i][RCOMP] & rMask) | (dst[i][RCOMP] & ~rMask);
            src[i][GCOMP] = (src[i][GCOMP] & gMask) | (dst[i][GCOMP] & ~gMask);
            src[i][BCOMP] = (src[i][BCOMP] & bMask) | (dst[i][BCOMP] & ~bMask);
            src[i][ACOMP] = (src[i][ACOMP] & aMask) | (dst[i][ACOMP] & ~aMask);
        }
    }
    else {
        /* 4-byte components */
        const GLuint rMask = GET_COLORMASK_BIT(ctx->Color.ColorMask, buf, 0) ? ~0x0 : 0x0;
        const GLuint gMask = GET_COLORMASK_BIT(ctx->Color.ColorMask, buf, 1) ? ~0x0 : 0x0;
        const GLuint bMask = GET_COLORMASK_BIT(ctx->Color.ColorMask, buf, 2) ? ~0x0 : 0x0;
        const GLuint aMask = GET_COLORMASK_BIT(ctx->Color.ColorMask, buf, 3) ? ~0x0 : 0x0;
        const GLuint (*dst)[4] = (const GLuint (*)[4]) rbPixels;
        GLuint (*src)[4] = (GLuint (*)[4]) span->array->attribs[VARYING_SLOT_COL0];
        GLuint i;
        for (i = 0; i < n; i++) {
            src[i][RCOMP] = (src[i][RCOMP] & rMask) | (dst[i][RCOMP] & ~rMask);
            src[i][GCOMP] = (src[i][GCOMP] & gMask) | (dst[i][GCOMP] & ~gMask);
            src[i][BCOMP] = (src[i][BCOMP] & bMask) | (dst[i][BCOMP] & ~bMask);
            src[i][ACOMP] = (src[i][ACOMP] & aMask) | (dst[i][ACOMP] & ~aMask);
        }
    }
}
