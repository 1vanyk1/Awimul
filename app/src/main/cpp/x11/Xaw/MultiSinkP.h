#ifndef _XawMultiSinkP_h
#define _XawMultiSinkP_h

#include "../headers/xfuncproto.h"

/*
 * MultiSink Object Private Data
 */
#include "TextSinkP.h"
#include "MultiSink.h"

/* new fields for the MultiSink object class */
typedef struct _MultiSinkClassPart {
    XtPointer extension;
} MultiSinkClassPart;

/* Full class record declaration */
typedef struct _MultiSinkClassRec {
    ObjectClassPart     object_class;
    TextSinkClassPart	text_sink_class;
    MultiSinkClassPart	multi_sink_class;
} MultiSinkClassRec;

extern MultiSinkClassRec multiSinkClassRec;

/* New fields for the MultiSink object record */
typedef struct {
    /* resources */
    Boolean echo;
    Boolean display_nonprinting;

    /* private */
    GC normgc, invgc, xorgc;
    XawTextPosition cursor_position;
    XawTextInsertState laststate;
    short cursor_x, cursor_y;		/* Cursor Location */
    XFontSet fontset;			/* font set to draw */
#ifndef OLDXAW
    XtPointer pad[4];	/* for future use and keep binary compatability */
#endif
} MultiSinkPart;

/* Full instance record declaration */
typedef struct _MultiSinkRec {
    ObjectPart          object;
    TextSinkPart	text_sink;
    MultiSinkPart	multi_sink;
} MultiSinkRec;

/*
 * Semi-private functions
 * for use by other Xaw modules only
 */
_XFUNCPROTOBEGIN

void _XawMultiSinkPosToXY
        (
                Widget			w,
                XawTextPosition	pos,
                Position		*x,
                Position		*y
        );

_XFUNCPROTOEND

#endif /* _XawMultiSinkP_h */
