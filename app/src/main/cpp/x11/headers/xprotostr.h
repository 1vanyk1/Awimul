#ifndef XPROTOSTRUCTS_H
#define XPROTOSTRUCTS_H

#include "xmd.h"
/* Used by PolySegment */
typedef struct _xSegment {
    INT16 x1 B16, y1 B16, x2 B16, y2 B16;
} xSegment;
/* POINT */
typedef struct _xPoint {
    INT16		x B16, y B16;
} xPoint;
typedef struct _xRectangle {
    INT16 x B16, y B16;
    CARD16  width B16, height B16;
} xRectangle;
/*  ARC  */
typedef struct _xArc {
    INT16 x B16, y B16;
    CARD16   width B16, height B16;
    INT16   angle1 B16, angle2 B16;
} xArc;
#endif /* XPROTOSTRUCTS_H */
