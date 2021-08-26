#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "fb.h"

static void
fbZeroLine(DrawablePtr pDrawable, GCPtr pGC, int mode, int npt, DDXPointPtr ppt)
{
    int x1, y1, x2, y2;
    int x, y;
    int dashOffset;

    x = pDrawable->x;
    y = pDrawable->y;
    x1 = ppt->x;
    y1 = ppt->y;
    dashOffset = pGC->dashOffset;
    while (--npt) {
        ++ppt;
        x2 = ppt->x;
        y2 = ppt->y;
        if (mode == CoordModePrevious) {
            x2 += x1;
            y2 += y1;
        }
        fbSegment(pDrawable, pGC, x1 + x, y1 + y,
                  x2 + x, y2 + y,
                  npt == 1 && pGC->capStyle != CapNotLast, &dashOffset);
        x1 = x2;
        y1 = y2;
    }
}

static void
fbZeroSegment(DrawablePtr pDrawable, GCPtr pGC, int nseg, xSegment * pSegs)
{
    int dashOffset;
    int x, y;
    Bool drawLast = pGC->capStyle != CapNotLast;

    x = pDrawable->x;
    y = pDrawable->y;
    while (nseg--) {
        dashOffset = pGC->dashOffset;
        fbSegment(pDrawable, pGC,
                  pSegs->x1 + x, pSegs->y1 + y,
                  pSegs->x2 + x, pSegs->y2 + y, drawLast, &dashOffset);
        pSegs++;
    }
}

void
fbFixCoordModePrevious(int npt, DDXPointPtr ppt)
{
    int x, y;

    x = ppt->x;
    y = ppt->y;
    npt--;
    while (npt--) {
        ppt++;
        x = (ppt->x += x);
        y = (ppt->y += y);
    }
}

void
fbPolyLine(DrawablePtr pDrawable, GCPtr pGC, int mode, int npt, DDXPointPtr ppt)
{
    void (*line) (DrawablePtr, GCPtr, int mode, int npt, DDXPointPtr ppt);

    if (pGC->lineWidth == 0) {
        line = fbZeroLine;
        if (pGC->fillStyle == FillSolid &&
            pGC->lineStyle == LineSolid &&
            RegionNumRects(fbGetCompositeClip(pGC)) == 1) {
            switch (pDrawable->bitsPerPixel) {
                case 8:
                    line = fbPolyline8;
                    break;
                case 16:
                    line = fbPolyline16;
                    break;
                case 32:
                    line = fbPolyline32;
                    break;
            }
        }
    }
    else {
        if (pGC->lineStyle != LineSolid)
            line = miWideDash;
        else
            line = miWideLine;
    }
    (*line) (pDrawable, pGC, mode, npt, ppt);
}

void
fbPolySegment(DrawablePtr pDrawable, GCPtr pGC, int nseg, xSegment * pseg)
{
    void (*seg) (DrawablePtr pDrawable, GCPtr pGC, int nseg, xSegment * pseg);

    if (pGC->lineWidth == 0) {
        seg = fbZeroSegment;
        if (pGC->fillStyle == FillSolid &&
            pGC->lineStyle == LineSolid &&
            RegionNumRects(fbGetCompositeClip(pGC)) == 1) {
            switch (pDrawable->bitsPerPixel) {
                case 8:
                    seg = fbPolySegment8;
                    break;
                case 16:
                    seg = fbPolySegment16;
                    break;
                case 32:
                    seg = fbPolySegment32;
                    break;
            }
        }
    }
    else {
        seg = miPolySegment;
    }
    (*seg) (pDrawable, pGC, nseg, pseg);
}
