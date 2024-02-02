#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XRecolorCursor(
        register Display *dpy,
        Cursor cursor,
        XColor *foreground,
        XColor *background)
{
    register xRecolorCursorReq *req;

    LockDisplay(dpy);
    GetReq(RecolorCursor, req);
    req->cursor = cursor;
    req->foreRed = foreground->red;
    req->foreGreen = foreground->green;
    req->foreBlue = foreground->blue;
    req->backRed = background->red;
    req->backGreen = background->green;
    req->backBlue = background->blue;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

