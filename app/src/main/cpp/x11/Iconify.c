#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xatom.h"
#include "headers/xos.h"
#include "headers/xutil.h"
#include <stdio.h>

/*
 * This function instructs the window manager to change this window from
 * NormalState to IconicState.
 */
Status XIconifyWindow (
        Display *dpy,
        Window w,
        int screen)
{
    Atom prop;

    prop = XInternAtom (dpy, "WM_CHANGE_STATE", False);
    if (prop == None)
        return False;
    else {
        XClientMessageEvent ev = {
                .type = ClientMessage,
                .window = w,
                .message_type = prop,
                .format = 32,
                .data.l[0] = IconicState
        };
        Window root = RootWindow (dpy, screen);

        return (XSendEvent (dpy, root, False,
                            SubstructureRedirectMask|SubstructureNotifyMask,
                            (XEvent *)&ev));
    }
}
