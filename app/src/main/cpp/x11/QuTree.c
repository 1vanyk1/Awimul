#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "reallocarray.h"

Status XQueryTree (
        register Display *dpy,
        Window w,
        Window *root,	/* RETURN */
        Window *parent,	/* RETURN */
        Window **children,	/* RETURN */
        unsigned int *nchildren)  /* RETURN */
{
    unsigned long nbytes;
    xQueryTreeReply rep;
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(QueryTree, w, req);
    if (!_XReply(dpy, (xReply *)&rep, 0, xFalse)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return (0);
    }

    *children = (Window *) NULL;
    if (rep.nChildren != 0) {
        *children = Xmallocarray(rep.nChildren, sizeof(Window));
        if (! *children) {
            _XEatDataWords(dpy, rep.length);
            UnlockDisplay(dpy);
            SyncHandle();
            return (0);
        }
        nbytes = rep.nChildren << 2;
        _XRead32 (dpy, (long *) *children, nbytes);
    }
    *parent = rep.parent;
    *root = rep.root;
    *nchildren = rep.nChildren;
    UnlockDisplay(dpy);
    SyncHandle();
    return (1);
}
