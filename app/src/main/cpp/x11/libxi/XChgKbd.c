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
XChangeKeyboardDevice(
        register Display	*dpy,
        XDevice		*dev)
{
    xChangeKeyboardDeviceReq *req;
    xChangeKeyboardDeviceReply rep;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);

    GetReq(ChangeKeyboardDevice, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_ChangeKeyboardDevice;
    req->deviceid = dev->device_id;
    rep.status = Success;

    (void)_XReply(dpy, (xReply *) & rep, 0, xTrue);

    UnlockDisplay(dpy);
    SyncHandle();
    return (rep.status);
}
