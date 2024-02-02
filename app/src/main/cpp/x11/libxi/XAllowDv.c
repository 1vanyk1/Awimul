#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

int
XAllowDeviceEvents(
        register Display	*dpy,
        XDevice		 *dev,
        int			 event_mode,
        Time		 time)
{
    xAllowDeviceEventsReq *req;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);

    GetReq(AllowDeviceEvents, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_AllowDeviceEvents;
    req->deviceid = dev->device_id;
    req->mode = event_mode;
    req->time = time;

    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}
