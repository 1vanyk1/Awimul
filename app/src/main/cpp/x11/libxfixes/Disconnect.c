#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xfixesint.h"
#include <limits.h>

void
XFixesSetClientDisconnectMode(Display *dpy, int disconnect_mode)
{
    XFixesExtDisplayInfo *info = XFixesFindDisplay(dpy);
    xXFixesSetClientDisconnectModeReq *req;

    XFixesSimpleCheckExtension(dpy, info);
    if (info->major_version < 6)
        return;

    LockDisplay(dpy);
    GetReq(XFixesSetClientDisconnectMode, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesSetClientDisconnectMode;
    req->disconnect_mode = disconnect_mode;
    UnlockDisplay(dpy);
    SyncHandle();
}

int
XFixesGetClientDisconnectMode(Display *dpy)
{
    XFixesExtDisplayInfo *info = XFixesFindDisplay(dpy);
    xXFixesGetClientDisconnectModeReq *req;
    xXFixesGetClientDisconnectModeReply rep;
    int disconnect_mode;

    XFixesCheckExtension(dpy, info, 0);
    if (info->major_version < 6)
        return XFixesClientDisconnectFlagDefault;

    LockDisplay(dpy);
    GetReq(XFixesGetClientDisconnectMode, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesGetClientDisconnectMode;

    if (!_XReply(dpy, (xReply *) &rep, 0, xFalse))
    {
        UnlockDisplay(dpy);
        SyncHandle();
        return XFixesClientDisconnectFlagDefault;
    }

    disconnect_mode = rep.disconnect_mode;
    UnlockDisplay(dpy);
    SyncHandle();

    return disconnect_mode;
}
