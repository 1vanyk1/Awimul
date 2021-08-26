#ifndef CURSORSTRUCT_H
#define CURSORSTRUCT_H

#include "cursor.h"
#include "privates.h"
/*
 * device-independent cursor storage
 */

/*
 * source and mask point directly to the bits, which are in the server-defined
 * bitmap format.
 */
typedef struct _CursorBits {
    unsigned char *source;      /* points to bits */
    unsigned char *mask;        /* points to bits */
    Bool emptyMask;             /* all zeros mask */
    unsigned short width, height, xhot, yhot;   /* metrics */
    int refcnt;                 /* can be shared */
    PrivateRec *devPrivates;    /* set by pScr->RealizeCursor */
    CARD32 *argb;               /* full-color alpha blended */
} CursorBits, *CursorBitsPtr;

#define CURSOR_BITS_SIZE (sizeof(CursorBits) + dixPrivatesSize(PRIVATE_CURSOR_BITS))

typedef struct _Cursor {
    CursorBitsPtr bits;
    unsigned short foreRed, foreGreen, foreBlue;        /* device-independent color */
    unsigned short backRed, backGreen, backBlue;        /* device-independent color */
    int refcnt;
    PrivateRec *devPrivates;    /* set by pScr->RealizeCursor */
    XID id;
    CARD32 serialNumber;
    Atom name;
} CursorRec;

#define CURSOR_REC_SIZE (sizeof(CursorRec) + dixPrivatesSize(PRIVATE_CURSOR))

typedef struct _CursorMetric {
    unsigned short width, height, xhot, yhot;
} CursorMetricRec;

typedef struct {
    int x, y;
    ScreenPtr pScreen;
} HotSpot;

#endif                          /* CURSORSTRUCT_H */