#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/xlibint.h"
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

void
XDeleteDeviceProperty(Display* dpy, XDevice* dev, Atom property)
{
    xDeleteDevicePropertyReq   *req;

    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return;

    GetReq(DeleteDeviceProperty, req);
    req->reqType    = info->codes->major_opcode;
    req->ReqType    = X_DeleteDeviceProperty;
    req->deviceid   = dev->device_id;
    req->property   = property;

    UnlockDisplay(dpy);
    SyncHandle();
    return;
}

