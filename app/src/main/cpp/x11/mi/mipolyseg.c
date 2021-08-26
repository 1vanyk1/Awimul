#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/x.h"
#include "../headers/xprotostr.h"
#include "../headers/regionstr.h"
#include "../headers/gcstruct.h"
#include "../headers/pixmap.h"
#include "mi.h"

/*****************************************************************
 * miPolySegment
 *
 *    For each segment, draws a line between (x1, y1) and (x2, y2).  The
 *    lines are drawn in the order listed.
 *
 *    Walks the segments, compressing them into format for PolyLines.
 *
 *****************************************************************/

void
miPolySegment(DrawablePtr pDraw, GCPtr pGC, int nseg, xSegment * pSegs)
{
    int i;

    for (i = 0; i < nseg; i++) {
        (*pGC->ops->Polylines) (pDraw, pGC, CoordModeOrigin, 2,
                                (DDXPointPtr) pSegs);
        pSegs++;
    }
}
