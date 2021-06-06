#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

Bool
XGetEventData(Display *dpy, XGenericEventCookie *event)
{
    Bool rc;
    LockDisplay(dpy);

    rc = _XFetchEventCookie(dpy, event);

    UnlockDisplay(dpy);

    return rc;
}
