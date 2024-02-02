#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

int
XDeviceBell(
        register Display	*dpy,
        XDevice		*dev,
        XID			 feedbackclass,
        XID			 feedbackid,
        int			 percent)
{
    xDeviceBellReq *req;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Add_XDeviceBell, info) == -1)
        return (NoSuchExtension);

    GetReq(DeviceBell, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_DeviceBell;
    req->deviceid = dev->device_id;
    req->feedbackclass = feedbackclass;
    req->feedbackid = feedbackid;
    req->percent = percent;

    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}
