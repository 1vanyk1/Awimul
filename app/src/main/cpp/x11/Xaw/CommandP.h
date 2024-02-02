#ifndef _XawCommandP_h
#define _XawCommandP_h

/*
 * Command Widget Private Data
 */
#include "Command.h"
#include "LabelP.h"

typedef enum {
    HighlightNone,		/* Do not highlight */
    HighlightWhenUnset,		/* Highlight only when unset, this is
				   to preserve current command widget
				   functionality */
    HighlightAlways		/* Always highlight, lets the toggle widget
				   and other subclasses do the right thing */
} XtCommandHighlight;

/* New fields for the Command widget class record */
typedef struct _CommandClass {
    XtPointer extension;
} CommandClassPart;

/* Full class record declaration */
typedef struct _CommandClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    LabelClassPart	label_class;
    CommandClassPart    command_class;
} CommandClassRec;

extern CommandClassRec commandClassRec;

/* New fields for the Command widget record */
typedef struct {
    /* resources */
    Dimension   highlight_thickness;
    XtCallbackList callbacks;

    /* private state */
    Pixmap	gray_pixmap;
    GC		normal_GC;
    GC		inverse_GC;
    Boolean	set;
    XtCommandHighlight	highlighted;

    /* more resources */
    int			shape_style;
    Dimension		corner_round;

#ifndef OLDXAW
    XtPointer pad[4];	/* for future use and keep binary compatability */
#endif
} CommandPart;

/* Full widget declaration */
typedef struct _CommandRec {
    CorePart         core;
    SimplePart	     simple;
    LabelPart	     label;
    CommandPart      command;
} CommandRec;

#endif /* _XawCommandP_h */
