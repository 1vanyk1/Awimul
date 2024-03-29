#include "../include/pipe/p_defines.h"
#include "u_inlines.h"

#include "format/u_format.h"
#include "format/u_format_bptc.h"
#include "u_math.h"
#include "u_memory.h"
#include "u_surface.h"
#include "u_tile.h"


/**
 * Move raw block of pixels from transfer object to user memory.
 */
void
pipe_get_tile_raw(struct pipe_transfer *pt,
                  const void *src,
                  uint x, uint y, uint w, uint h,
                  void *dst, int dst_stride)
{
    if (dst_stride == 0)
        dst_stride = util_format_get_stride(pt->resource->format, w);

    if (u_clip_tile(x, y, &w, &h, &pt->box))
        return;

    util_copy_rect(dst, pt->resource->format, dst_stride, 0, 0, w, h, src, pt->stride, x, y);
}


/**
 * Move raw block of pixels from user memory to transfer object.
 */
void
pipe_put_tile_raw(struct pipe_transfer *pt,
                  void *dst,
                  uint x, uint y, uint w, uint h,
                  const void *src, int src_stride)
{
    enum pipe_format format = pt->resource->format;

    if (src_stride == 0)
        src_stride = util_format_get_stride(format, w);

    if (u_clip_tile(x, y, &w, &h, &pt->box))
        return;

    util_copy_rect(dst, format, pt->stride, x, y, w, h, src, src_stride, 0, 0);
}




/** Convert short in [-32768,32767] to GLfloat in [-1.0,1.0] */
#define SHORT_TO_FLOAT(S)   ((2.0F * (S) + 1.0F) * (1.0F/65535.0F))

#define UNCLAMPED_FLOAT_TO_SHORT(us, f)  \
   us = ( (short) ( CLAMP((f), -1.0, 1.0) * 32767.0F) )



/*** PIPE_FORMAT_Z16_UNORM ***/

/**
 * Return each Z value as four floats in [0,1].
 */
static void
z16_get_tile_rgba(const ushort *src,
                  unsigned w, unsigned h,
                  float *p,
                  unsigned dst_stride)
{
    const float scale = 1.0f / 65535.0f;
    unsigned i, j;

    for (i = 0; i < h; i++) {
        float *pRow = p;
        for (j = 0; j < w; j++, pRow += 4) {
            pRow[0] =
            pRow[1] =
            pRow[2] =
            pRow[3] = *src++ * scale;
        }
        p += dst_stride;
    }
}




/*** PIPE_FORMAT_Z32_UNORM ***/

/**
 * Return each Z value as four floats in [0,1].
 */
static void
z32_get_tile_rgba(const unsigned *src,
                  unsigned w, unsigned h,
                  float *p,
                  unsigned dst_stride)
{
    const double scale = 1.0 / (double) 0xffffffff;
    unsigned i, j;

    for (i = 0; i < h; i++) {
        float *pRow = p;
        for (j = 0; j < w; j++, pRow += 4) {
            pRow[0] =
            pRow[1] =
            pRow[2] =
            pRow[3] = (float) (*src++ * scale);
        }
        p += dst_stride;
    }
}


/*** PIPE_FORMAT_Z24_UNORM_S8_UINT ***/

/**
 * Return Z component as four float in [0,1].  Stencil part ignored.
 */
static void
s8z24_get_tile_rgba(const unsigned *src,
                    unsigned w, unsigned h,
                    float *p,
                    unsigned dst_stride)
{
    const double scale = 1.0 / ((1 << 24) - 1);
    unsigned i, j;

    for (i = 0; i < h; i++) {
        float *pRow = p;
        for (j = 0; j < w; j++, pRow += 4) {
            pRow[0] =
            pRow[1] =
            pRow[2] =
            pRow[3] = (float) (scale * (*src++ & 0xffffff));
        }
        p += dst_stride;
    }
}


/*** PIPE_FORMAT_S8_UINT_Z24_UNORM ***/

/**
 * Return Z component as four float in [0,1].  Stencil part ignored.
 */
static void
z24s8_get_tile_rgba(const unsigned *src,
                    unsigned w, unsigned h,
                    float *p,
                    unsigned dst_stride)
{
    const double scale = 1.0 / ((1 << 24) - 1);
    unsigned i, j;

    for (i = 0; i < h; i++) {
        float *pRow = p;
        for (j = 0; j < w; j++, pRow += 4) {
            pRow[0] =
            pRow[1] =
            pRow[2] =
            pRow[3] = (float) (scale * (*src++ >> 8));
        }
        p += dst_stride;
    }
}

/*** PIPE_FORMAT_S8X24_UINT ***/

/**
 * Return S component as four uint32_t in [0..255].  Z part ignored.
 */
static void
s8x24_get_tile_rgba(const unsigned *src,
                    unsigned w, unsigned h,
                    float *p,
                    unsigned dst_stride)
{
    unsigned i, j;

    for (i = 0; i < h; i++) {
        uint32_t *pRow = (uint32_t *)p;

        for (j = 0; j < w; j++, pRow += 4) {
            pRow[0] =
            pRow[1] =
            pRow[2] =
            pRow[3] = ((*src++ >> 24) & 0xff);
        }

        p += dst_stride;
    }
}

/*** PIPE_FORMAT_X24S8_UINT ***/

/**
 * Return S component as four uint32_t in [0..255].  Z part ignored.
 */
static void
x24s8_get_tile_rgba(const unsigned *src,
                    unsigned w, unsigned h,
                    float *p,
                    unsigned dst_stride)
{
    unsigned i, j;

    for (i = 0; i < h; i++) {
        uint32_t *pRow = (uint32_t *)p;
        for (j = 0; j < w; j++, pRow += 4) {
            pRow[0] =
            pRow[1] =
            pRow[2] =
            pRow[3] = (*src++ & 0xff);
        }
        p += dst_stride;
    }
}


/**
 * Return S component as four uint32_t in [0..255].  Z part ignored.
 */
static void
s8_get_tile_rgba(const unsigned char *src,
                 unsigned w, unsigned h,
                 float *p,
                 unsigned dst_stride)
{
    unsigned i, j;

    for (i = 0; i < h; i++) {
        uint32_t *pRow = (uint32_t *)p;
        for (j = 0; j < w; j++, pRow += 4) {
            pRow[0] =
            pRow[1] =
            pRow[2] =
            pRow[3] = (*src++ & 0xff);
        }
        p += dst_stride;
    }
}

/*** PIPE_FORMAT_Z32_FLOAT ***/

/**
 * Return each Z value as four floats in [0,1].
 */
static void
z32f_get_tile_rgba(const float *src,
                   unsigned w, unsigned h,
                   float *p,
                   unsigned dst_stride)
{
    unsigned i, j;

    for (i = 0; i < h; i++) {
        float *pRow = p;
        for (j = 0; j < w; j++, pRow += 4) {
            pRow[0] =
            pRow[1] =
            pRow[2] =
            pRow[3] = *src++;
        }
        p += dst_stride;
    }
}

/*** PIPE_FORMAT_Z32_FLOAT_S8X24_UINT ***/

/**
 * Return each Z value as four floats in [0,1].
 */
static void
z32f_x24s8_get_tile_rgba(const float *src,
                         unsigned w, unsigned h,
                         float *p,
                         unsigned dst_stride)
{
    unsigned i, j;

    for (i = 0; i < h; i++) {
        float *pRow = p;
        for (j = 0; j < w; j++, pRow += 4) {
            pRow[0] =
            pRow[1] =
            pRow[2] =
            pRow[3] = *src;
            src += 2;
        }
        p += dst_stride;
    }
}

/*** PIPE_FORMAT_X32_S8X24_UINT ***/

/**
 * Return S component as four uint32_t in [0..255].  Z part ignored.
 */
static void
x32_s8_get_tile_rgba(const unsigned *src,
                     unsigned w, unsigned h,
                     float *p,
                     unsigned dst_stride)
{
    unsigned i, j;

    for (i = 0; i < h; i++) {
        uint32_t *pRow = (uint32_t *)p;
        for (j = 0; j < w; j++, pRow += 4) {
            src++;
            pRow[0] =
            pRow[1] =
            pRow[2] =
            pRow[3] = (*src++ & 0xff);
        }
        p += dst_stride;
    }
}

void
pipe_put_tile_rgba(struct pipe_transfer *pt,
                   void *dst,
                   uint x, uint y, uint w, uint h,
                   enum pipe_format format, const void *p)
{
    unsigned src_stride = w * 4;

    if (u_clip_tile(x, y, &w, &h, &pt->box))
        return;

    /* While we do generate RGBA tiles for z/s for softpipe's texture fetch
     * path, we never have to store from "RGBA" to Z/S.
     */
    if (util_format_is_depth_or_stencil(format))
        return;

    util_format_write_4(format,
                        p, src_stride * sizeof(float),
                        dst, pt->stride,
                        x, y, w, h);
}

void
pipe_get_tile_rgba(struct pipe_transfer *pt,
                   const void *src,
                   uint x, uint y, uint w, uint h,
                   enum pipe_format format,
                   void *dst)
{
    unsigned dst_stride = w * 4;
    void *packed;

    if (u_clip_tile(x, y, &w, &h, &pt->box)) {
        return;
    }

    packed = MALLOC(util_format_get_nblocks(format, w, h) * util_format_get_blocksize(format));
    if (!packed) {
        return;
    }

    if (format == PIPE_FORMAT_UYVY || format == PIPE_FORMAT_YUYV) {
        assert((x & 1) == 0);
    }

    pipe_get_tile_raw(pt, src, x, y, w, h, packed, 0);

    switch (format) {
        case PIPE_FORMAT_Z16_UNORM:
            z16_get_tile_rgba((ushort *) packed, w, h, dst, dst_stride);
            break;
        case PIPE_FORMAT_Z32_UNORM:
            z32_get_tile_rgba((unsigned *) packed, w, h, dst, dst_stride);
            break;
        case PIPE_FORMAT_Z24_UNORM_S8_UINT:
        case PIPE_FORMAT_Z24X8_UNORM:
            s8z24_get_tile_rgba((unsigned *) packed, w, h, dst, dst_stride);
            break;
        case PIPE_FORMAT_S8_UINT:
            s8_get_tile_rgba((unsigned char *) packed, w, h, dst, dst_stride);
            break;
        case PIPE_FORMAT_X24S8_UINT:
            s8x24_get_tile_rgba((unsigned *) packed, w, h, dst, dst_stride);
            break;
        case PIPE_FORMAT_S8_UINT_Z24_UNORM:
        case PIPE_FORMAT_X8Z24_UNORM:
            z24s8_get_tile_rgba((unsigned *) packed, w, h, dst, dst_stride);
            break;
        case PIPE_FORMAT_S8X24_UINT:
            x24s8_get_tile_rgba((unsigned *) packed, w, h, dst, dst_stride);
            break;
        case PIPE_FORMAT_Z32_FLOAT:
            z32f_get_tile_rgba((float *) packed, w, h, dst, dst_stride);
            break;
        case PIPE_FORMAT_Z32_FLOAT_S8X24_UINT:
            z32f_x24s8_get_tile_rgba((float *) packed, w, h, dst, dst_stride);
            break;
        case PIPE_FORMAT_X32_S8X24_UINT:
            x32_s8_get_tile_rgba((unsigned *) packed, w, h, dst, dst_stride);
            break;
        default:
            util_format_read_4(format,
                               dst, dst_stride * sizeof(float),
                               packed, util_format_get_stride(format, w),
                               0, 0, w, h);
    }

    FREE(packed);
}
