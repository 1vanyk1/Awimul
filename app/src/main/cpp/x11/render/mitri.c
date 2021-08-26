#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/scrnintstr.h"
#include "../headers/gcstruct.h"
#include "../headers/pixmapstr.h"
#include "../headers/windowstr.h"
#include "../mi/mi.h"
#include "../headers/picturestr.h"
#include "mipict.h"

void
miPointFixedBounds(int npoint, xPointFixed * points, BoxPtr bounds)
{
    bounds->x1 = xFixedToInt(points->x);
    bounds->x2 = xFixedToInt(xFixedCeil(points->x));
    bounds->y1 = xFixedToInt(points->y);
    bounds->y2 = xFixedToInt(xFixedCeil(points->y));
    points++;
    npoint--;
    while (npoint-- > 0) {
        INT16 x1 = xFixedToInt(points->x);
        INT16 x2 = xFixedToInt(xFixedCeil(points->x));
        INT16 y1 = xFixedToInt(points->y);
        INT16 y2 = xFixedToInt(xFixedCeil(points->y));

        if (x1 < bounds->x1)
            bounds->x1 = x1;
        else if (x2 > bounds->x2)
            bounds->x2 = x2;
        if (y1 < bounds->y1)
            bounds->y1 = y1;
        else if (y2 > bounds->y2)
            bounds->y2 = y2;
        points++;
    }
}

void
miTriangleBounds(int ntri, xTriangle * tris, BoxPtr bounds)
{
    miPointFixedBounds(ntri * 3, (xPointFixed *) tris, bounds);
}
