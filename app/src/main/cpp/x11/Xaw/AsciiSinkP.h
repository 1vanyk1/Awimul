#ifndef _XawAsciiSinkP_h
#define _XawAsciiSinkP_h

/*
 * AsciiSink Object Private Data
 */
#include "TextSinkP.h"
#include "AsciiSink.h"

/* New fields for the AsciiSink object class record */
typedef struct _AsciiSinkClassPart {
    XtPointer extension;
} AsciiSinkClassPart;

/* Full class record declaration */
typedef struct _AsciiSinkClassRec {
    ObjectClassPart     object_class;
    TextSinkClassPart	text_sink_class;
    AsciiSinkClassPart	ascii_sink_class;
} AsciiSinkClassRec;

extern AsciiSinkClassRec asciiSinkClassRec;

/* New fields for the AsciiSink object record */
typedef struct {
    /* resources */
    XFontStruct *font;          /* Font to draw in. */
    Boolean echo;
    Boolean display_nonprinting;

    /* private */
    GC normgc, invgc, xorgc;
    XawTextPosition cursor_position;
    XawTextInsertState laststate;
    short cursor_x, cursor_y;	/* Cursor Location. */
#ifndef OLDXAW
    XtPointer pad[4];	/* for future use and keep binary compatability */
#endif
} AsciiSinkPart;

/* Full instance record declaration */
typedef struct _AsciiSinkRec {
    ObjectPart          object;
    TextSinkPart	text_sink;
    AsciiSinkPart	ascii_sink;
} AsciiSinkRec;

#endif /* _XawAsciiSinkP_h */
