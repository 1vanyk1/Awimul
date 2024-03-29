#include "../main/errors.h"
#include "../main/glheader.h"
#include "../main/macros.h"

#include "../main/format_pack.h"

#include "s_context.h"
#include "s_span.h"
#include "s_stencil.h"
#include "s_zoom.h"


/**
 * Compute the bounds of the region resulting from zooming a pixel span.
 * The resulting region will be entirely inside the window/scissor bounds
 * so no additional clipping is needed.
 * \param imageX, imageY  position of the mage being drawn (gl WindowPos)
 * \param spanX, spanY  position of span being drawing
 * \param width  number of pixels in span
 * \param x0, x1  returned X bounds of zoomed region [x0, x1)
 * \param y0, y1  returned Y bounds of zoomed region [y0, y1)
 * \return GL_TRUE if any zoomed pixels visible, GL_FALSE if totally clipped
 */
static GLboolean
compute_zoomed_bounds(struct gl_context *ctx, GLint imageX, GLint imageY,
                      GLint spanX, GLint spanY, GLint width,
                      GLint *x0, GLint *x1, GLint *y0, GLint *y1)
{
    const struct gl_framebuffer *fb = ctx->DrawBuffer;
    GLint c0, c1, r0, r1;

    assert(spanX >= imageX);
    assert(spanY >= imageY);

    /*
     * Compute destination columns: [c0, c1)
     */
    c0 = imageX + (GLint) ((spanX - imageX) * ctx->Pixel.ZoomX);
    c1 = imageX + (GLint) ((spanX + width - imageX) * ctx->Pixel.ZoomX);
    if (c1 < c0) {
        /* swap */
        GLint tmp = c1;
        c1 = c0;
        c0 = tmp;
    }
    c0 = CLAMP(c0, fb->_Xmin, fb->_Xmax);
    c1 = CLAMP(c1, fb->_Xmin, fb->_Xmax);
    if (c0 == c1) {
        return GL_FALSE; /* no width */
    }

    /*
     * Compute destination rows: [r0, r1)
     */
    r0 = imageY + (GLint) ((spanY - imageY) * ctx->Pixel.ZoomY);
    r1 = imageY + (GLint) ((spanY + 1 - imageY) * ctx->Pixel.ZoomY);
    if (r1 < r0) {
        /* swap */
        GLint tmp = r1;
        r1 = r0;
        r0 = tmp;
    }
    r0 = CLAMP(r0, fb->_Ymin, fb->_Ymax);
    r1 = CLAMP(r1, fb->_Ymin, fb->_Ymax);
    if (r0 == r1) {
        return GL_FALSE; /* no height */
    }

    *x0 = c0;
    *x1 = c1;
    *y0 = r0;
    *y1 = r1;

    return GL_TRUE;
}


/**
 * Convert a zoomed x image coordinate back to an unzoomed x coord.
 * 'zx' is screen position of a pixel in the zoomed image, who's left edge
 * is at 'imageX'.
 * return corresponding x coord in the original, unzoomed image.
 * This can use this for unzooming X or Y values.
 */
static inline GLint
unzoom_x(GLfloat zoomX, GLint imageX, GLint zx)
{
    /*
    zx = imageX + (x - imageX) * zoomX;
    zx - imageX = (x - imageX) * zoomX;
    (zx - imageX) / zoomX = x - imageX;
    */
    GLint x;
    if (zoomX < 0.0F)
        zx++;
    x = imageX + (GLint) ((zx - imageX) / zoomX);
    return x;
}



/**
 * Helper function called from _swrast_write_zoomed_rgba/rgb/
 * index/depth_span().
 */
static void
zoom_span( struct gl_context *ctx, GLint imgX, GLint imgY, const SWspan *span,
           const GLvoid *src, GLenum format )
{
    SWcontext *swrast = SWRAST_CONTEXT(ctx);
    SWspan zoomed;
    GLint x0, x1, y0, y1;
    GLint zoomedWidth;

    if (!compute_zoomed_bounds(ctx, imgX, imgY, span->x, span->y, span->end,
                               &x0, &x1, &y0, &y1)) {
        return;  /* totally clipped */
    }

    if (!swrast->ZoomedArrays) {
        /* allocate on demand */
        swrast->ZoomedArrays = (SWspanarrays *) calloc(1, sizeof(SWspanarrays));
        if (!swrast->ZoomedArrays)
            return;
    }

    zoomedWidth = x1 - x0;
    assert(zoomedWidth > 0);
    assert(zoomedWidth <= SWRAST_MAX_WIDTH);

    /* no pixel arrays! must be horizontal spans. */
    assert((span->arrayMask & SPAN_XY) == 0);
    assert(span->primitive == GL_BITMAP);

    INIT_SPAN(zoomed, GL_BITMAP);
    zoomed.x = x0;
    zoomed.end = zoomedWidth;
    zoomed.array = swrast->ZoomedArrays;
    zoomed.array->ChanType = span->array->ChanType;
    if (zoomed.array->ChanType == GL_UNSIGNED_BYTE)
        zoomed.array->rgba = (GLchan (*)[4]) zoomed.array->rgba8;
    else if (zoomed.array->ChanType == GL_UNSIGNED_SHORT)
        zoomed.array->rgba = (GLchan (*)[4]) zoomed.array->rgba16;
    else
        zoomed.array->rgba = (GLchan (*)[4]) zoomed.array->attribs[VARYING_SLOT_COL0];

    COPY_4V(zoomed.attrStart[VARYING_SLOT_POS], span->attrStart[VARYING_SLOT_POS]);
    COPY_4V(zoomed.attrStepX[VARYING_SLOT_POS], span->attrStepX[VARYING_SLOT_POS]);
    COPY_4V(zoomed.attrStepY[VARYING_SLOT_POS], span->attrStepY[VARYING_SLOT_POS]);

    zoomed.attrStart[VARYING_SLOT_FOGC][0] = span->attrStart[VARYING_SLOT_FOGC][0];
    zoomed.attrStepX[VARYING_SLOT_FOGC][0] = span->attrStepX[VARYING_SLOT_FOGC][0];
    zoomed.attrStepY[VARYING_SLOT_FOGC][0] = span->attrStepY[VARYING_SLOT_FOGC][0];

    if (format == GL_RGBA || format == GL_RGB) {
        /* copy Z info */
        zoomed.z = span->z;
        zoomed.zStep = span->zStep;
        /* we'll generate an array of colorss */
        zoomed.interpMask = span->interpMask & ~SPAN_RGBA;
        zoomed.arrayMask |= SPAN_RGBA;
        zoomed.arrayAttribs |= VARYING_BIT_COL0;  /* we'll produce these values */
        assert(span->arrayMask & SPAN_RGBA);
    }
    else if (format == GL_DEPTH_COMPONENT) {
        /* Copy color info */
        zoomed.red = span->red;
        zoomed.green = span->green;
        zoomed.blue = span->blue;
        zoomed.alpha = span->alpha;
        zoomed.redStep = span->redStep;
        zoomed.greenStep = span->greenStep;
        zoomed.blueStep = span->blueStep;
        zoomed.alphaStep = span->alphaStep;
        /* we'll generate an array of depth values */
        zoomed.interpMask = span->interpMask & ~SPAN_Z;
        zoomed.arrayMask |= SPAN_Z;
        assert(span->arrayMask & SPAN_Z);
    }
    else {
        _mesa_problem(ctx, "Bad format in zoom_span");
        return;
    }

    /* zoom the span horizontally */
    if (format == GL_RGBA) {
        if (zoomed.array->ChanType == GL_UNSIGNED_BYTE) {
            const GLubyte (*rgba)[4] = (const GLubyte (*)[4]) src;
            GLint i;
            for (i = 0; i < zoomedWidth; i++) {
                GLint j = unzoom_x(ctx->Pixel.ZoomX, imgX, x0 + i) - span->x;
                assert(j >= 0);
                assert(j < (GLint) span->end);
                COPY_4UBV(zoomed.array->rgba8[i], rgba[j]);
            }
        }
        else if (zoomed.array->ChanType == GL_UNSIGNED_SHORT) {
            const GLushort (*rgba)[4] = (const GLushort (*)[4]) src;
            GLint i;
            for (i = 0; i < zoomedWidth; i++) {
                GLint j = unzoom_x(ctx->Pixel.ZoomX, imgX, x0 + i) - span->x;
                assert(j >= 0);
                assert(j < (GLint) span->end);
                COPY_4V(zoomed.array->rgba16[i], rgba[j]);
            }
        }
        else {
            const GLfloat (*rgba)[4] = (const GLfloat (*)[4]) src;
            GLint i;
            for (i = 0; i < zoomedWidth; i++) {
                GLint j = unzoom_x(ctx->Pixel.ZoomX, imgX, x0 + i) - span->x;
                assert(j >= 0);
                assert(j < (GLint) span->end);
                COPY_4V(zoomed.array->attribs[VARYING_SLOT_COL0][i], rgba[j]);
            }
        }
    }
    else if (format == GL_RGB) {
        if (zoomed.array->ChanType == GL_UNSIGNED_BYTE) {
            const GLubyte (*rgb)[3] = (const GLubyte (*)[3]) src;
            GLint i;
            for (i = 0; i < zoomedWidth; i++) {
                GLint j = unzoom_x(ctx->Pixel.ZoomX, imgX, x0 + i) - span->x;
                assert(j >= 0);
                assert(j < (GLint) span->end);
                zoomed.array->rgba8[i][0] = rgb[j][0];
                zoomed.array->rgba8[i][1] = rgb[j][1];
                zoomed.array->rgba8[i][2] = rgb[j][2];
                zoomed.array->rgba8[i][3] = 0xff;
            }
        }
        else if (zoomed.array->ChanType == GL_UNSIGNED_SHORT) {
            const GLushort (*rgb)[3] = (const GLushort (*)[3]) src;
            GLint i;
            for (i = 0; i < zoomedWidth; i++) {
                GLint j = unzoom_x(ctx->Pixel.ZoomX, imgX, x0 + i) - span->x;
                assert(j >= 0);
                assert(j < (GLint) span->end);
                zoomed.array->rgba16[i][0] = rgb[j][0];
                zoomed.array->rgba16[i][1] = rgb[j][1];
                zoomed.array->rgba16[i][2] = rgb[j][2];
                zoomed.array->rgba16[i][3] = 0xffff;
            }
        }
        else {
            const GLfloat (*rgb)[3] = (const GLfloat (*)[3]) src;
            GLint i;
            for (i = 0; i < zoomedWidth; i++) {
                GLint j = unzoom_x(ctx->Pixel.ZoomX, imgX, x0 + i) - span->x;
                assert(j >= 0);
                assert(j < (GLint) span->end);
                zoomed.array->attribs[VARYING_SLOT_COL0][i][0] = rgb[j][0];
                zoomed.array->attribs[VARYING_SLOT_COL0][i][1] = rgb[j][1];
                zoomed.array->attribs[VARYING_SLOT_COL0][i][2] = rgb[j][2];
                zoomed.array->attribs[VARYING_SLOT_COL0][i][3] = 1.0F;
            }
        }
    }
    else if (format == GL_DEPTH_COMPONENT) {
        const GLuint *zValues = (const GLuint *) src;
        GLint i;
        for (i = 0; i < zoomedWidth; i++) {
            GLint j = unzoom_x(ctx->Pixel.ZoomX, imgX, x0 + i) - span->x;
            assert(j >= 0);
            assert(j < (GLint) span->end);
            zoomed.array->z[i] = zValues[j];
        }
        /* Now, fall into the RGB path below */
        format = GL_RGBA;
    }

    /* write the span in rows [r0, r1) */
    if (format == GL_RGBA || format == GL_RGB) {
        /* Writing the span may modify the colors, so make a backup now if we're
         * going to call _swrast_write_zoomed_span() more than once.
         * Also, clipping may change the span end value, so store it as well.
         */
        const GLint end = zoomed.end; /* save */
        void *rgbaSave;
        const GLint pixelSize =
                (zoomed.array->ChanType == GL_UNSIGNED_BYTE) ? 4 * sizeof(GLubyte) :
                ((zoomed.array->ChanType == GL_UNSIGNED_SHORT) ? 4 * sizeof(GLushort)
                                                               : 4 * sizeof(GLfloat));

        rgbaSave = malloc(zoomed.end * pixelSize);
        if (!rgbaSave) {
            return;
        }

        if (y1 - y0 > 1) {
            memcpy(rgbaSave, zoomed.array->rgba, zoomed.end * pixelSize);
        }
        for (zoomed.y = y0; zoomed.y < y1; zoomed.y++) {
            _swrast_write_rgba_span(ctx, &zoomed);
            zoomed.end = end;  /* restore */
            if (y1 - y0 > 1) {
                /* restore the colors */
                memcpy(zoomed.array->rgba, rgbaSave, zoomed.end * pixelSize);
            }
        }

        free(rgbaSave);
    }
}


void
_swrast_write_zoomed_rgba_span(struct gl_context *ctx, GLint imgX, GLint imgY,
                               const SWspan *span, const GLvoid *rgba)
{
    zoom_span(ctx, imgX, imgY, span, rgba, GL_RGBA);
}


void
_swrast_write_zoomed_rgb_span(struct gl_context *ctx, GLint imgX, GLint imgY,
                              const SWspan *span, const GLvoid *rgb)
{
    zoom_span(ctx, imgX, imgY, span, rgb, GL_RGB);
}


void
_swrast_write_zoomed_depth_span(struct gl_context *ctx, GLint imgX, GLint imgY,
                                const SWspan *span)
{
    zoom_span(ctx, imgX, imgY, span,
              (const GLvoid *) span->array->z, GL_DEPTH_COMPONENT);
}


/**
 * Zoom/write stencil values.
 * No per-fragment operations are applied.
 */
void
_swrast_write_zoomed_stencil_span(struct gl_context *ctx, GLint imgX, GLint imgY,
                                  GLint width, GLint spanX, GLint spanY,
                                  const GLubyte stencil[])
{
    GLubyte *zoomedVals;
    GLint x0, x1, y0, y1, y;
    GLint i, zoomedWidth;

    if (!compute_zoomed_bounds(ctx, imgX, imgY, spanX, spanY, width,
                               &x0, &x1, &y0, &y1)) {
        return;  /* totally clipped */
    }

    zoomedWidth = x1 - x0;
    assert(zoomedWidth > 0);
    assert(zoomedWidth <= SWRAST_MAX_WIDTH);

    zoomedVals = malloc(zoomedWidth * sizeof(GLubyte));
    if (!zoomedVals)
        return;

    /* zoom the span horizontally */
    for (i = 0; i < zoomedWidth; i++) {
        GLint j = unzoom_x(ctx->Pixel.ZoomX, imgX, x0 + i) - spanX;
        assert(j >= 0);
        assert(j < width);
        zoomedVals[i] = stencil[j];
    }

    /* write the zoomed spans */
    for (y = y0; y < y1; y++) {
        _swrast_write_stencil_span(ctx, zoomedWidth, x0, y, zoomedVals);
    }

    free(zoomedVals);
}


/**
 * Zoom/write 32-bit Z values.
 * No per-fragment operations are applied.
 */
void
_swrast_write_zoomed_z_span(struct gl_context *ctx, GLint imgX, GLint imgY,
                            GLint width, GLint spanX, GLint spanY,
                            const GLuint *zVals)
{
    struct gl_renderbuffer *rb =
            ctx->DrawBuffer->Attachment[BUFFER_DEPTH].Renderbuffer;
    GLuint *zoomedVals;
    GLint x0, x1, y0, y1, y;
    GLint i, zoomedWidth;

    if (!compute_zoomed_bounds(ctx, imgX, imgY, spanX, spanY, width,
                               &x0, &x1, &y0, &y1)) {
        return;  /* totally clipped */
    }

    zoomedWidth = x1 - x0;
    assert(zoomedWidth > 0);
    assert(zoomedWidth <= SWRAST_MAX_WIDTH);

    zoomedVals = malloc(zoomedWidth * sizeof(GLuint));
    if (!zoomedVals)
        return;

    /* zoom the span horizontally */
    for (i = 0; i < zoomedWidth; i++) {
        GLint j = unzoom_x(ctx->Pixel.ZoomX, imgX, x0 + i) - spanX;
        assert(j >= 0);
        assert(j < width);
        zoomedVals[i] = zVals[j];
    }

    /* write the zoomed spans */
    for (y = y0; y < y1; y++) {
        GLubyte *dst = _swrast_pixel_address(rb, x0, y);
        _mesa_pack_uint_z_row(rb->Format, zoomedWidth, zoomedVals, dst);
    }

    free(zoomedVals);
}
