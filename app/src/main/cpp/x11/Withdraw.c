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
 * NormalState or IconicState to Withdrawn.
 */
Status XWithdrawWindow (
        Display *dpy,
        Window w,
        int screen)
{
    XUnmapWindow (dpy, w);

    {
        Window root = RootWindow (dpy, screen);
        XUnmapEvent ev = {
                .type = UnmapNotify,
                .event = root,
                .window = w,
                .from_configure = False
        };
        return (XSendEvent (dpy, root, False,
                            SubstructureRedirectMask|SubstructureNotifyMask,
                            (XEvent *)&ev));
    }
}
