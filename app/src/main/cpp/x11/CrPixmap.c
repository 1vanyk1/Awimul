#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

#ifdef USE_DYNAMIC_XCURSOR
void
_XNoticeCreateBitmap (Display	    *dpy,
		      Pixmap	    pid,
		      unsigned int  width,
		      unsigned int  height);
#endif

Pixmap XCreatePixmap (
        register Display *dpy,
        Drawable d,
        unsigned int width,
        unsigned int height,
        unsigned int depth)
{
    Pixmap pid;
    register xCreatePixmapReq *req;

    LockDisplay(dpy);
    GetReq(CreatePixmap, req);
    req->drawable = d;
    req->width = width;
    req->height = height;
    req->depth = depth;
    pid = req->pid = XAllocID(dpy);
    UnlockDisplay(dpy);
    SyncHandle();
#ifdef USE_DYNAMIC_XCURSOR
    if (depth == 1)
	_XNoticeCreateBitmap (dpy, pid, width, height);
#endif
    return (pid);
}

