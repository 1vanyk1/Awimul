#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "fb.h"

void
fbFillSpans(DrawablePtr pDrawable,
            GCPtr pGC, int n, DDXPointPtr ppt, int *pwidth, int fSorted)
{
    RegionPtr pClip = fbGetCompositeClip(pGC);
    BoxPtr pextent, pbox;
    int nbox;
    int extentX1, extentX2, extentY1, extentY2;
    int fullX1, fullX2, fullY1;
    int partX1, partX2;

    pextent = RegionExtents(pClip);
    extentX1 = pextent->x1;
    extentY1 = pextent->y1;
    extentX2 = pextent->x2;
    extentY2 = pextent->y2;
    while (n--) {
        fullX1 = ppt->x;
        fullY1 = ppt->y;
        fullX2 = fullX1 + (int) *pwidth;
        ppt++;
        pwidth++;

        if (fullY1 < extentY1 || extentY2 <= fullY1)
            continue;

        if (fullX1 < extentX1)
            fullX1 = extentX1;

        if (fullX2 > extentX2)
            fullX2 = extentX2;

        if (fullX1 >= fullX2)
            continue;

        nbox = RegionNumRects(pClip);
        if (nbox == 1) {
            fbFill(pDrawable, pGC, fullX1, fullY1, fullX2 - fullX1, 1);
        }
        else {
            pbox = RegionRects(pClip);
            while (nbox--) {
                if (pbox->y1 <= fullY1 && fullY1 < pbox->y2) {
                    partX1 = pbox->x1;
                    if (partX1 < fullX1)
                        partX1 = fullX1;
                    partX2 = pbox->x2;
                    if (partX2 > fullX2)
                        partX2 = fullX2;
                    if (partX2 > partX1) {
                        fbFill(pDrawable, pGC,
                               partX1, fullY1, partX2 - partX1, 1);
                    }
                }
                pbox++;
            }
        }
    }
}
