#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

#define AllMaskBits (CWBackPixmap|CWBackPixel|CWBorderPixmap|\
		     CWBorderPixel|CWBitGravity|CWWinGravity|\
		     CWBackingStore|CWBackingPlanes|CWBackingPixel|\
		     CWOverrideRedirect|CWSaveUnder|CWEventMask|\
		     CWDontPropagate|CWColormap|CWCursor)

int
XChangeWindowAttributes (
        register Display *dpy,
        Window w,
        unsigned long valuemask,
        XSetWindowAttributes *attributes)
{
    register xChangeWindowAttributesReq *req;

    LockDisplay(dpy);
    GetReq(ChangeWindowAttributes,req);
    req->window = w;
    valuemask &= AllMaskBits;
    if ((req->valueMask = valuemask))
        _XProcessWindowAttributes (dpy, req, valuemask, attributes);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
