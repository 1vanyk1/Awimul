#ifndef _XawTipP_h
#define _XawTipP_h

#include "Tip.h"
#include "../libXt/CoreP.h"
#include "XawInit.h"

typedef struct {
    XtPointer extension;
} TipClassPart;

typedef struct _TipClassRec {
    CoreClassPart core_class;
    TipClassPart tip_class;
} TipClassRec;

extern TipClassRec tipClassRec;

typedef struct _TipPart {
    /* resources */
    Pixel foreground;
    XFontStruct	*font;
    XFontSet fontset;
    Dimension top_margin;
    Dimension bottom_margin;
    Dimension left_margin;
    Dimension right_margin;
    int backing_store;
    int timeout;
    XawDisplayList *display_list;

    /* private */
    GC gc;
    XtIntervalId timer;
    String label;
    Boolean international;
    unsigned char encoding;
    XtPointer pad[4];
} TipPart;

typedef struct _TipRec {
    CorePart core;
    TipPart tip;
} TipRec;

#endif /* _XawTipP_h */
