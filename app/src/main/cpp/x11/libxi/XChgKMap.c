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
XChangeDeviceKeyMapping(
        register Display	*dpy,
        XDevice		*dev,
        int			 first,
        int			 syms_per_code,
        KeySym		*keysyms,
        int			 count)
{
    register long nbytes;
    xChangeDeviceKeyMappingReq *req;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);

    GetReq(ChangeDeviceKeyMapping, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_ChangeDeviceKeyMapping;
    req->deviceid = dev->device_id;
    req->firstKeyCode = first;
    req->keyCodes = count;
    req->keySymsPerKeyCode = syms_per_code;
    req->length += count * syms_per_code;
    nbytes = syms_per_code * count * sizeof(CARD32);
    Data(dpy, (char *)keysyms, nbytes);

    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}
