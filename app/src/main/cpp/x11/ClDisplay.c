#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xxcbint.h"
#include "headers/xlib.h"
#include "headers/xlibint.h"
#include "Xintconn.h"

/*
 * XCloseDisplay - XSync the connection to the X Server, close the connection,
 * and free all associated storage.  Extension close procs should only free
 * memory and must be careful about the types of requests they generate.
 */

int
XCloseDisplay (
        register Display *dpy)
{
    register _XExtension *ext;
    register int i;

    if (!(dpy->flags & XlibDisplayClosing))
    {
        dpy->flags |= XlibDisplayClosing;
        for (i = 0; i < dpy->nscreens; i++) {
            register Screen *sp = &dpy->screens[i];
            XFreeGC (dpy, sp->default_gc);
        }
        if (dpy->cursor_font != None) {
            XUnloadFont (dpy, dpy->cursor_font);
        }
        XSync(dpy, 1);  /* throw away pending events, catch errors */
        /* call out to any extensions interested */
        for (ext = dpy->ext_procs; ext; ext = ext->next) {
            if (ext->close_display)
                (*ext->close_display)(dpy, &ext->codes);
        }
        /* if the closes generated more protocol, sync them up */
        if (X_DPY_GET_REQUEST(dpy) != X_DPY_GET_LAST_REQUEST_READ(dpy))
            XSync(dpy, 1);
    }
    xcb_disconnect(dpy->xcb->connection);
    _XFreeDisplayStructure (dpy);
    return 0;
}