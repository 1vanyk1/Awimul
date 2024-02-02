#include "IntrinsicI.h"

typedef XrmResource **CallbackTable;

#define _XtCBCalling 1
#define _XtCBFreeAfterCalling 2

_XFUNCPROTOBEGIN

typedef struct internalCallbackRec {
    unsigned short count;
    char	   is_padded;	/* contains NULL padding for external form */
    char	   call_state;  /* combination of _XtCB{FreeAfter}Calling */
#if __STDC_VERSION__ >= 199901L
    /* When compiling with C99 or newer, use a flexible member to ensure
     * padding and alignment for sizeof(). */
    XtCallbackRec callbacks[];
#elif defined(LONG64)
    unsigned int   align_pad;	/* padding to align callback list */
#endif
    /* XtCallbackList */
} InternalCallbackRec, *InternalCallbackList;

typedef Boolean (*_XtConditionProc)(
        XtPointer	/* data */
);

extern void _XtAddCallback(
        InternalCallbackList*	/* callbacks */,
        XtCallbackProc		/* callback */,
        XtPointer 			/* closure */
);

extern void _XtAddCallbackOnce(
        InternalCallbackList*	/* callbacks */,
        XtCallbackProc		/* callback */,
        XtPointer 			/* closure */
);

extern InternalCallbackList _XtCompileCallbackList(
        XtCallbackList		/* xtcallbacks */
);

extern XtCallbackList _XtGetCallbackList(
        InternalCallbackList*	/* callbacks */
);

extern void _XtRemoveAllCallbacks(
        InternalCallbackList*	/* callbacks */
);

extern void _XtRemoveCallback(
        InternalCallbackList*	/* callbacks */,
        XtCallbackProc		/* callback */,
        XtPointer			/* closure */
);

extern void _XtPeekCallback(
        Widget			/* widget */,
        XtCallbackList		/* callbacks */,
        XtCallbackProc *		/* callback */,
        XtPointer *			/* closure */
);

extern void _XtCallConditionalCallbackList(
        Widget			/* widget */,
        XtCallbackList		/* callbacks */,
        XtPointer			/* call_data */,
        _XtConditionProc		/* cond_proc */
);

_XFUNCPROTOEND