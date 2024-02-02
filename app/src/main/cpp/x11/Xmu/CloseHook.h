#ifndef _XMU_CLOSEHOOK_H_
#define _XMU_CLOSEHOOK_H_

#include "../headers/xlib.h"
#include "../headers/xfuncproto.h"
#include "../headers/xlibint.h"

typedef XPointer CloseHook;

typedef int (*XmuCloseHookProc)(Display *dpy, XPointer data);

_XFUNCPROTOBEGIN

        CloseHook XmuAddCloseDisplayHook
(
        Display		*dpy,
XmuCloseHookProc	proc,
        XPointer		arg
);

Bool XmuLookupCloseDisplayHook
        (
                Display		*dpy,
                CloseHook		handle,
XmuCloseHookProc	proc,
        XPointer		arg
);

Bool XmuRemoveCloseDisplayHook
        (
                Display		*dpy,
                CloseHook		handle,
XmuCloseHookProc	proc,
        XPointer		arg
);

_XFUNCPROTOEND

#endif /* _XMU_CLOSEHOOK_H_ */
