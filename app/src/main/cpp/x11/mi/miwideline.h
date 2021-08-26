#include "mifpoly.h"            /* for ICEIL */

typedef struct _PolyEdge {
    int height;                 /* number of scanlines to process */
    int x;                      /* starting x coordinate */
    int stepx;                  /* fixed integral dx */
    int signdx;                 /* variable dx sign */
    int e;                      /* initial error term */
    int dy;
    int dx;
} PolyEdgeRec, *PolyEdgePtr;

#define SQSECANT 108.856472512142       /* 1/sin^2(11/2) - miter limit constant */

/*
 * types for general polygon routines
 */

typedef struct _PolyVertex {
    double x, y;
} PolyVertexRec, *PolyVertexPtr;

typedef struct _PolySlope {
    int dx, dy;
    double k;                   /* x0 * dy - y0 * dx */
} PolySlopeRec, *PolySlopePtr;

/*
 * Line face description for caps/joins
 */

typedef struct _LineFace {
    double xa, ya;
    int dx, dy;
    int x, y;
    double k;
} LineFaceRec, *LineFacePtr;

/*
 * macros for polygon fillers
 */

#define MILINESETPIXEL(pDrawable, pGC, pixel, oldPixel) { \
    oldPixel = pGC->fgPixel; \
    if (pixel != oldPixel) { \
	ChangeGCVal gcval; \
	gcval.val = pixel; \
	ChangeGC (NullClient, pGC, GCForeground, &gcval); \
	ValidateGC (pDrawable, pGC); \
    } \
}
#define MILINERESETPIXEL(pDrawable, pGC, pixel, oldPixel) { \
    if (pixel != oldPixel) { \
	ChangeGCVal gcval; \
	gcval.val = oldPixel; \
	ChangeGC (NullClient, pGC, GCForeground, &gcval); \
	ValidateGC (pDrawable, pGC); \
    } \
}
