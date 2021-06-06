#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

Window XCreateSimpleWindow(
        register Display *dpy,
        Window parent,
        int x,
        int y,
        unsigned int width,
        unsigned int height,
        unsigned int borderWidth,
        unsigned long border,
        unsigned long background)
{
    Window wid;
    register xCreateWindowReq *req;

    LockDisplay(dpy);
    GetReqExtra(CreateWindow, 8, req);
    req->parent = parent;
    req->x = x;
    req->y = y;
    req->width = width;
    req->height = height;
    req->borderWidth = borderWidth;
    req->depth = 0;
    req->class = CopyFromParent;
    req->visual = CopyFromParent;
    wid = req->wid = XAllocID(dpy);
    req->mask = CWBackPixel | CWBorderPixel;

    {
        register CARD32 *valuePtr = (CARD32 *) NEXTPTR(req,xCreateWindowReq);
        *valuePtr++ = background;
        *valuePtr = border;
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return (wid);
}