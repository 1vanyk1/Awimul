#ifndef _XtHookObjI_h
#define _XtHookObjI_h

#include "IntrinsicI.h"

_XFUNCPROTOBEGIN

/* This object is implementation-dependent and private to the library. */

typedef struct _HookObjRec *HookObject;
typedef struct _HookObjClassRec *HookObjectClass;

externalref WidgetClass hookObjectClass;

typedef struct _HookObjPart {
    /* resources */
    XtCallbackList createhook_callbacks;
    XtCallbackList changehook_callbacks;
    XtCallbackList confighook_callbacks;
    XtCallbackList geometryhook_callbacks;
    XtCallbackList destroyhook_callbacks;
    WidgetList shells;
    Cardinal num_shells;
    /* private data */
    Cardinal max_shells;
    Screen* screen;
}HookObjPart;

typedef struct _HookObjRec {
    ObjectPart object;
    HookObjPart hooks;
} HookObjRec;

typedef struct _HookObjClassPart {
    int unused;
} HookObjClassPart;

typedef struct _HookObjClassRec {
    ObjectClassPart object_class;
    HookObjClassPart hook_class;
} HookObjClassRec;

externalref HookObjClassRec hookObjClassRec;

_XFUNCPROTOEND

#endif /* ifndef _Xt_HookObjI_h */