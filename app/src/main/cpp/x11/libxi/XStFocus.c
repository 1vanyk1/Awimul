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
XSetDeviceFocus(
        register Display	*dpy,
        XDevice		*dev,
        Window		 focus,
        int			 revert_to,
        Time		 time)
{
    xSetDeviceFocusReq *req;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);

    GetReq(SetDeviceFocus, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_SetDeviceFocus;
    req->device = dev->device_id;
    req->focus = focus;
    req->revertTo = revert_to;
    req->time = time;

    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}
