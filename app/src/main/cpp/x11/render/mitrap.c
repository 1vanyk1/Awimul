#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/scrnintstr.h"
#include "../headers/gcstruct.h"
#include "../headers/pixmapstr.h"
#include "../headers/windowstr.h"
#include "../headers/servermd.h"
#include "../mi/mi.h"
#include "../headers/picturestr.h"
#include "mipict.h"

static xFixed
miLineFixedX(xLineFixed * l, xFixed y, Bool ceil)
{
    xFixed dx = l->p2.x - l->p1.x;
    xFixed_32_32 ex = (xFixed_32_32) (y - l->p1.y) * dx;
    xFixed dy = l->p2.y - l->p1.y;

    if (ceil)
        ex += (dy - 1);
    return l->p1.x + (xFixed) (ex / dy);
}

void
miTrapezoidBounds(int ntrap, xTrapezoid * traps, BoxPtr box)
{
    box->y1 = MAXSHORT;
    box->y2 = MINSHORT;
    box->x1 = MAXSHORT;
    box->x2 = MINSHORT;
    for (; ntrap; ntrap--, traps++) {
        INT16 x1, y1, x2, y2;

        if (!xTrapezoidValid(traps))
            continue;
        y1 = xFixedToInt(traps->top);
        if (y1 < box->y1)
            box->y1 = y1;

        y2 = xFixedToInt(xFixedCeil(traps->bottom));
        if (y2 > box->y2)
            box->y2 = y2;

        x1 = xFixedToInt(min(miLineFixedX(&traps->left, traps->top, FALSE),
                             miLineFixedX(&traps->left, traps->bottom, FALSE)));
        if (x1 < box->x1)
            box->x1 = x1;

        x2 = xFixedToInt(xFixedCeil
                                 (max
                                          (miLineFixedX(&traps->right, traps->top, TRUE),
                                           miLineFixedX(&traps->right, traps->bottom, TRUE))));
        if (x2 > box->x2)
            box->x2 = x2;
    }
}
