#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include "../headers/xlibint.h"
#include "../headers/extensions/XI2proto.h"
#include "../headers/extensions/XInput2.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

Status
XIGetFocus(Display *dpy, int deviceid, Window *focus_return)
{
    xXIGetFocusReq   *req;
    xXIGetFocusReply reply;

    XExtDisplayInfo *extinfo = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_2_0, extinfo) == -1)
        return (NoSuchExtension);

    GetReq(XIGetFocus, req);
    req->reqType  = extinfo->codes->major_opcode;
    req->ReqType  = X_XIGetFocus;
    req->deviceid = deviceid;

    if (!_XReply(dpy, (xReply*) &reply, 0, xFalse)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return False;
    }

    *focus_return = reply.focus;

    UnlockDisplay(dpy);
    SyncHandle();
    return Success;

}

