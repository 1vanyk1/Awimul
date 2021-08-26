#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "fb.h"

static void
fbPushPattern(DrawablePtr pDrawable,
              GCPtr pGC,
              FbStip * src,
              FbStride srcStride, int srcX, int x, int y, int width, int height)
{
    FbStip *s, bitsMask, bitsMask0, bits;
    int xspan;
    int w;
    int lenspan;

    src += srcX >> FB_STIP_SHIFT;
    srcX &= FB_STIP_MASK;

    bitsMask0 = FbStipMask(srcX, 1);

    while (height--) {
        bitsMask = bitsMask0;
        w = width;
        s = src;
        src += srcStride;
        bits = READ(s++);
        xspan = x;
        while (w) {
            if (bits & bitsMask) {
                lenspan = 0;
                do {
                    lenspan++;
                    if (lenspan == w)
                        break;
                    bitsMask = FbStipRight(bitsMask, 1);
                    if (!bitsMask) {
                        bits = READ(s++);
                        bitsMask = FbBitsMask(0, 1);
                    }
                } while (bits & bitsMask);
                fbFill(pDrawable, pGC, xspan, y, lenspan, 1);
                xspan += lenspan;
                w -= lenspan;
            }
            else {
                do {
                    w--;
                    xspan++;
                    if (!w)
                        break;
                    bitsMask = FbStipRight(bitsMask, 1);
                    if (!bitsMask) {
                        bits = READ(s++);
                        bitsMask = FbBitsMask(0, 1);
                    }
                } while (!(bits & bitsMask));
            }
        }
        y++;
    }
}

static void
fbPushFill(DrawablePtr pDrawable,
           GCPtr pGC,
           FbStip * src,
           FbStride srcStride, int srcX, int x, int y, int width, int height)
{
    FbGCPrivPtr pPriv = fbGetGCPrivate(pGC);

    if (pGC->fillStyle == FillSolid) {
        FbBits *dst;
        FbStride dstStride;
        int dstBpp;
        int dstXoff, dstYoff;
        int dstX;
        int dstWidth;

        fbGetDrawable(pDrawable, dst, dstStride, dstBpp, dstXoff, dstYoff);
        dst = dst + (y + dstYoff) * dstStride;
        dstX = (x + dstXoff) * dstBpp;
        dstWidth = width * dstBpp;
        if (dstBpp == 1) {
            fbBltStip(src,
                      srcStride,
                      srcX,
                      (FbStip *) dst,
                      FbBitsStrideToStipStride(dstStride),
                      dstX,
                      dstWidth,
                      height,
                      FbStipple1Rop(pGC->alu, pGC->fgPixel), pPriv->pm, dstBpp);
        }
        else {
            fbBltOne(src,
                     srcStride,
                     srcX,
                     dst,
                     dstStride,
                     dstX,
                     dstBpp,
                     dstWidth,
                     height,
                     pPriv->and, pPriv->xor,
                     fbAnd(GXnoop, (FbBits) 0, FB_ALLONES),
                     fbXor(GXnoop, (FbBits) 0, FB_ALLONES));
        }
        fbFinishAccess(pDrawable);
    }
    else {
        fbPushPattern(pDrawable, pGC, src, srcStride, srcX,
                      x, y, width, height);
    }
}

void
fbPushImage(DrawablePtr pDrawable,
            GCPtr pGC,
            FbStip * src,
            FbStride srcStride, int srcX, int x, int y, int width, int height)
{
    RegionPtr pClip = fbGetCompositeClip(pGC);
    int nbox;
    BoxPtr pbox;
    int x1, y1, x2, y2;

    for (nbox = RegionNumRects(pClip),
                 pbox = RegionRects(pClip); nbox--; pbox++) {
        x1 = x;
        y1 = y;
        x2 = x + width;
        y2 = y + height;
        if (x1 < pbox->x1)
            x1 = pbox->x1;
        if (y1 < pbox->y1)
            y1 = pbox->y1;
        if (x2 > pbox->x2)
            x2 = pbox->x2;
        if (y2 > pbox->y2)
            y2 = pbox->y2;
        if (x1 >= x2 || y1 >= y2)
            continue;
        fbPushFill(pDrawable,
                   pGC,
                   src + (y1 - y) * srcStride,
                   srcStride, srcX + (x1 - x), x1, y1, x2 - x1, y2 - y1);
    }
}

void
fbPushPixels(GCPtr pGC,
             PixmapPtr pBitmap,
             DrawablePtr pDrawable, int dx, int dy, int xOrg, int yOrg)
{
    FbStip *stip;
    FbStride stipStride;
    int stipBpp;
    _X_UNUSED int stipXoff, stipYoff;

    fbGetStipDrawable(&pBitmap->drawable, stip, stipStride, stipBpp, stipXoff,
                      stipYoff);

    fbPushImage(pDrawable, pGC, stip, stipStride, 0, xOrg, yOrg, dx, dy);
}
