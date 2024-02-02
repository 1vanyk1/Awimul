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
XGetDeviceFocus(
        register Display	*dpy,
        XDevice		*dev,
        Window		*focus,
        int			*revert_to,
        Time		*time)
{
    xGetDeviceFocusReq *req;
    xGetDeviceFocusReply rep;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);

    GetReq(GetDeviceFocus, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_GetDeviceFocus;
    req->deviceid = dev->device_id;

    (void)_XReply(dpy, (xReply *) & rep, 0, xTrue);
    *focus = rep.focus;
    *revert_to = rep.revertTo;
    *time = rep.time;
    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}
