#ifndef _XtComposite_h
#define _XtComposite_h

#include "Intrinsic.h"

typedef struct _CompositeClassRec *CompositeWidgetClass;

typedef Cardinal (*XtOrderProc)(
        Widget 	/* child */
);

_XFUNCPROTOBEGIN

extern void XtManageChildren(
        WidgetList 		/* children */,
        Cardinal 		/* num_children */
);

extern void XtManageChild(
        Widget 		/* child */
);

extern void XtUnmanageChildren(
        WidgetList 		/* children */,
        Cardinal 		/* num_children */
);

extern void XtUnmanageChild(
        Widget 		/* child */
);

typedef void (*XtDoChangeProc)(
        Widget		/* composite_parent */,
        WidgetList		/* unmanage_children */,
        Cardinal *		/* num_unmanage_children */,
        WidgetList		/* manage_children */,
        Cardinal *		/* num_manage_children */,
        XtPointer		/* client_data */
);

extern void XtChangeManagedSet(
        WidgetList		/* unmanage_children */,
        Cardinal		/* num_unmanage_children */,
        XtDoChangeProc	/* do_change_proc */,
        XtPointer		/* client_data */,
        WidgetList		/* manage_children */,
        Cardinal		/* num_manage_children */
);

_XFUNCPROTOEND

#ifndef VMS
        externalref WidgetClass compositeWidgetClass;
#endif

#endif /* _XtComposite_h */
