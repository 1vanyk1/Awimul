#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XFreeGC (
        register Display *dpy,
        GC gc)
{
    register xResourceReq *req;
    register _XExtension *ext;
    LockDisplay(dpy);
    /* call out to any extensions interested */
    for (ext = dpy->ext_procs; ext; ext = ext->next)
        if (ext->free_GC) (*ext->free_GC)(dpy, gc, &ext->codes);
    GetResReq (FreeGC, gc->gid, req);
    UnlockDisplay(dpy);
    SyncHandle();
    _XFreeExtData(gc->ext_data);
    Xfree(gc);
    return 1;
}
