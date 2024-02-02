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
XUngrabDevice(
        register Display	*dpy,
        XDevice		*dev,
        Time		 time)
{
    register xUngrabDeviceReq *req;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);

    GetReq(UngrabDevice, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_UngrabDevice;

    req->deviceid = dev->device_id;
    req->time = time;

    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}
