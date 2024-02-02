#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include "../headers/xlibint.h"
#include "../headers/extensions/XI2proto.h"
#include "../headers/extensions/XInput2.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

Status
XISetFocus(Display *dpy, int deviceid, Window focus, Time time)
{
    xXISetFocusReq   *req;

    XExtDisplayInfo *extinfo = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_2_0, extinfo) == -1)
        return (NoSuchExtension);

    GetReq(XISetFocus, req);
    req->reqType  = extinfo->codes->major_opcode;
    req->ReqType  = X_XISetFocus;
    req->deviceid = deviceid;
    req->focus    = focus;
    req->time     = time;

    UnlockDisplay(dpy);
    SyncHandle();
    return Success;

}

