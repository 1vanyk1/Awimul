#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xfixesint.h"

void
XFixesChangeSaveSet (Display *dpy, Window win, int mode, int target, int map)
{
    XFixesExtDisplayInfo	*info = XFixesFindDisplay (dpy);
    xXFixesChangeSaveSetReq	*req;

    XFixesSimpleCheckExtension (dpy, info);

    LockDisplay (dpy);
    GetReq (XFixesChangeSaveSet, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesChangeSaveSet;
    req->mode = mode;
    req->target = target;
    req->map = map;
    req->window = win;
    UnlockDisplay (dpy);
    SyncHandle ();
}
