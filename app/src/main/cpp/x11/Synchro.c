#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"


static int _XSyncFunction(register Display *dpy)
{
    XSync(dpy,0);
    return 0;
}

int (*XSynchronize(Display *dpy, int onoff))(Display *)
{
    int (*temp)(Display *);
    int (*func)(Display *) = NULL;

    if (onoff)
        func = _XSyncFunction;

    LockDisplay(dpy);
    if (dpy->flags & XlibDisplayPrivSync) {
        temp = dpy->savedsynchandler;
        dpy->savedsynchandler = func;
    } else {
        temp = dpy->synchandler;
        dpy->synchandler = func;
    }
    UnlockDisplay(dpy);
    return (temp);
}

int (*XSetAfterFunction(
        Display *dpy,
        int (*func)(
                Display*
        )
))(Display *)
{
    int (*temp)(Display *);

    LockDisplay(dpy);
    if (dpy->flags & XlibDisplayPrivSync) {
        temp = dpy->savedsynchandler;
        dpy->savedsynchandler = func;
    } else {
        temp = dpy->synchandler;
        dpy->synchandler = func;
    }
    UnlockDisplay(dpy);
    return (temp);
}
