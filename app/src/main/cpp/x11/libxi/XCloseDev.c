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
XCloseDevice(
        register Display	*dpy,
        register XDevice	*dev)
{
    xCloseDeviceReq *req;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);

    GetReq(CloseDevice, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_CloseDevice;
    req->deviceid = dev->device_id;

    XFree((char *)dev);
    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}
