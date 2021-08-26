#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "xcms/Xcmsint.h"


Colormap XCreateColormap(
        register Display *dpy,
        Window w,
        Visual *visual,
        int alloc)
{
    register xCreateColormapReq *req;
    Colormap mid;

    LockDisplay(dpy);
    GetReq(CreateColormap, req);
    req->window = w;
    mid = req->mid = XAllocID(dpy);
    req->alloc = alloc;
    if (visual == CopyFromParent) req->visual = CopyFromParent;
    else req->visual = visual->visualid;

#ifdef XCMS
    _XcmsAddCmapRec(dpy, mid, w, visual);
#endif

    UnlockDisplay(dpy);
    SyncHandle();

    return(mid);
}
