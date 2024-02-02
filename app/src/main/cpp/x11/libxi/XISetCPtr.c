#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include "../headers/extensions/XI2proto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput2.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

Status
XISetClientPointer(Display* dpy, Window win, int deviceid)
{
    xXISetClientPointerReq* req;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_2_0, info) == -1)
        return (NoSuchExtension);

    GetReq(XISetClientPointer, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_XISetClientPointer;
    req->win = win;
    req->deviceid = deviceid;

    UnlockDisplay(dpy);
    SyncHandle();
    return Success;
}
