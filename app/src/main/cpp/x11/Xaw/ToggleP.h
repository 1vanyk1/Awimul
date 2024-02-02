#ifndef _XawToggleP_h
#define _XawToggleP_h

#include "Toggle.h"
#include "CommandP.h"

/*
 * Toggle Widget Private Data
 */
#define streq(a, b)	(strcmp((a), (b)) == 0)

typedef struct _RadioGroup {
    struct _RadioGroup *prev, *next; /* Pointers to other elements in group  */
    Widget widget;		     /* Widget corrosponding to this element */
} RadioGroup;

/* New fields for the Toggle widget class */
typedef struct _ToggleClass  {
    XtActionProc Set;
    XtActionProc Unset;
    XtPointer extension;
} ToggleClassPart;

/* class record declaration */
typedef struct _ToggleClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    LabelClassPart	label_class;
    CommandClassPart	command_class;
    ToggleClassPart	toggle_class;
} ToggleClassRec;

extern ToggleClassRec toggleClassRec;

/* New fields for the Toggle widget */
typedef struct {
    /* resources */
    Widget widget;
    XtPointer radio_data;

    /* private */
    RadioGroup *radio_group;
#ifndef OLDXAW
    XtPointer pad[4];	/* for future use and keep binary compatability */
#endif
} TogglePart;

/* Full widget declaration */
typedef struct _ToggleRec {
    CorePart	core;
    SimplePart	simple;
    LabelPart	label;
    CommandPart	command;
    TogglePart	toggle;
} ToggleRec;

#endif /* _XawToggleP_h */
