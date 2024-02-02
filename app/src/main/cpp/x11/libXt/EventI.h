#ifndef _Event_h_
#define _Event_h_

typedef struct _XtGrabRec  *XtGrabList;

#include "PassivGraI.h"
#include "IntrinsicI.h"

_XFUNCPROTOBEGIN

extern void _XtEventInitialize(
        void
);

typedef struct _XtEventRec {
    XtEventTable	next;
    EventMask		mask;	/*  also select_data count for RecExt */
    XtEventHandler	proc;
    XtPointer		closure;
    unsigned int	select:1;
    unsigned int	has_type_specifier:1;
    unsigned int	async:1; /* not used, here for Digital extension? */
} XtEventRec;

typedef struct _XtGrabRec {
    XtGrabList next;
    Widget   widget;
    unsigned int exclusive:1;
    unsigned int spring_loaded:1;
}XtGrabRec;

typedef struct _BlockHookRec {
    struct _BlockHookRec* next;
    XtAppContext app;
    XtBlockHookProc proc;
    XtPointer closure;
} BlockHookRec, *BlockHook;

extern void _XtFreeEventTable(
        XtEventTable*	/* event_table */
);

extern Boolean _XtOnGrabList(
        Widget	/* widget */,
        XtGrabRec*	/* grabList */
);

extern void _XtRemoveAllInputs(
        XtAppContext /* app */
);

extern void _XtRefreshMapping(
        XEvent*	/* event */,
        _XtBoolean	/* dispatch */
);

extern void _XtSendFocusEvent(
        Widget	/* child */,
        int		/* type */);

extern EventMask _XtConvertTypeToMask(
        int		/* eventType */
);

/* EventUtil.c */
extern Widget _XtFindRemapWidget(XEvent *event, Widget widget,
                                 EventMask mask, XtPerDisplayInput pdi);
extern void _XtUngrabBadGrabs(XEvent *event, Widget widget,
                              EventMask mask, XtPerDisplayInput pdi);
extern void _XtFillAncestorList(Widget **listPtr, int *maxElemsPtr,
                                int *numElemsPtr, Widget start,
                                Widget breakWidget);

/* NextEvent.c */
extern Boolean XtAppPeekEvent_SkipTimer;

_XFUNCPROTOEND

#endif /* _Event_h_ */