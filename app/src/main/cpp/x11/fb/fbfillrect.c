#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "fb.h"

void
fbPolyFillRect(DrawablePtr pDrawable, GCPtr pGC, int nrect, xRectangle *prect)
{
    RegionPtr pClip = fbGetCompositeClip(pGC);
    register BoxPtr pbox;
    BoxPtr pextent;
    int extentX1, extentX2, extentY1, extentY2;
    int fullX1, fullX2, fullY1, fullY2;
    int partX1, partX2, partY1, partY2;
    int xorg, yorg;
    int n;

    xorg = pDrawable->x;
    yorg = pDrawable->y;

    pextent = RegionExtents(pClip);
    extentX1 = pextent->x1;
    extentY1 = pextent->y1;
    extentX2 = pextent->x2;
    extentY2 = pextent->y2;
    while (nrect--) {
        fullX1 = prect->x + xorg;
        fullY1 = prect->y + yorg;
        fullX2 = fullX1 + (int) prect->width;
        fullY2 = fullY1 + (int) prect->height;
        prect++;

        if (fullX1 < extentX1)
            fullX1 = extentX1;

        if (fullY1 < extentY1)
            fullY1 = extentY1;

        if (fullX2 > extentX2)
            fullX2 = extentX2;

        if (fullY2 > extentY2)
            fullY2 = extentY2;

        if ((fullX1 >= fullX2) || (fullY1 >= fullY2))
            continue;
        n = RegionNumRects(pClip);
        if (n == 1) {
            fbFill(pDrawable,
                   pGC, fullX1, fullY1, fullX2 - fullX1, fullY2 - fullY1);
        }
        else {
            pbox = RegionRects(pClip);
            /*
             * clip the rectangle to each box in the clip region
             * this is logically equivalent to calling Intersect()
             */
            while (n--) {
                partX1 = pbox->x1;
                if (partX1 < fullX1)
                    partX1 = fullX1;
                partY1 = pbox->y1;
                if (partY1 < fullY1)
                    partY1 = fullY1;
                partX2 = pbox->x2;
                if (partX2 > fullX2)
                    partX2 = fullX2;
                partY2 = pbox->y2;
                if (partY2 > fullY2)
                    partY2 = fullY2;

                pbox++;

                if (partX1 < partX2 && partY1 < partY2)
                    fbFill(pDrawable, pGC,
                           partX1, partY1, partX2 - partX1, partY2 - partY1);
            }
        }
    }
}
