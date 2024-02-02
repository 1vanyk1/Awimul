#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include "../headers/extensions/XI2proto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput2.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

Bool
XIGetClientPointer(Display* dpy, Window win, int *deviceid)
{
    xXIGetClientPointerReq *req;
    xXIGetClientPointerReply rep;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, Dont_Check, info) == -1)
        return False;

    GetReq(XIGetClientPointer, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_XIGetClientPointer;
    req->win = win;

    if (!_XReply(dpy, (xReply*) &rep, 0, xFalse)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return False;
    }

    *deviceid = rep.deviceid;

    UnlockDisplay(dpy);
    SyncHandle();
    return rep.set;
}
