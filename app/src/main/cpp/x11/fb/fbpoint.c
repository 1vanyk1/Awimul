#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "fb.h"

typedef void (*FbDots) (FbBits * dst,
                        FbStride dstStride,
                        int dstBpp,
                        BoxPtr pBox,
                        xPoint * pts,
                        int npt,
                        int xorg,
                        int yorg, int xoff, int yoff, FbBits and, FbBits xor);

static void
fbDots(FbBits * dstOrig,
       FbStride dstStride,
       int dstBpp,
       BoxPtr pBox,
       xPoint * pts,
       int npt,
       int xorg, int yorg, int xoff, int yoff, FbBits andOrig, FbBits xorOrig)
{
    FbStip *dst = (FbStip *) dstOrig;
    int x1, y1, x2, y2;
    int x, y;
    FbStip *d;
    FbStip and = andOrig;
    FbStip xor = xorOrig;

    dstStride = FbBitsStrideToStipStride(dstStride);
    x1 = pBox->x1;
    y1 = pBox->y1;
    x2 = pBox->x2;
    y2 = pBox->y2;
    while (npt--) {
        x = pts->x + xorg;
        y = pts->y + yorg;
        pts++;
        if (x1 <= x && x < x2 && y1 <= y && y < y2) {
            FbStip mask;
            x = (x + xoff) * dstBpp;
            d = dst + ((y + yoff) * dstStride) + (x >> FB_STIP_SHIFT);
            x &= FB_STIP_MASK;

            mask = FbStipMask(x, dstBpp);
            WRITE(d, FbDoMaskRRop(READ(d), and, xor, mask));
        }
    }
}

void
fbPolyPoint(DrawablePtr pDrawable,
            GCPtr pGC, int mode, int nptInit, xPoint * pptInit)
{
    FbGCPrivPtr pPriv = fbGetGCPrivate(pGC);
    RegionPtr pClip = fbGetCompositeClip(pGC);
    FbBits *dst;
    FbStride dstStride;
    int dstBpp;
    int dstXoff, dstYoff;
    FbDots dots;
    FbBits and, xor;
    xPoint *ppt;
    int npt;
    BoxPtr pBox;
    int nBox;

    /* make pointlist origin relative */
    ppt = pptInit;
    npt = nptInit;
    if (mode == CoordModePrevious) {
        npt--;
        while (npt--) {
            ppt++;
            ppt->x += (ppt - 1)->x;
            ppt->y += (ppt - 1)->y;
        }
    }
    fbGetDrawable(pDrawable, dst, dstStride, dstBpp, dstXoff, dstYoff);
    and = pPriv->and;
    xor = pPriv->xor;
    dots = fbDots;
    switch (dstBpp) {
        case 8:
            dots = fbDots8;
            break;
        case 16:
            dots = fbDots16;
            break;
        case 32:
            dots = fbDots32;
            break;
    }
    for (nBox = RegionNumRects(pClip), pBox = RegionRects(pClip);
         nBox--; pBox++)
        (*dots) (dst, dstStride, dstBpp, pBox, pptInit, nptInit,
                 pDrawable->x, pDrawable->y, dstXoff, dstYoff, and, xor);
    fbFinishAccess(pDrawable);
}