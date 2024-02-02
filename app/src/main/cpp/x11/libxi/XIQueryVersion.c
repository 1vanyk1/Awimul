#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include "../headers/xlibint.h"
#include "../headers/extensions/XI2proto.h"
#include "../headers/extensions/XInput2.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

Status
XIQueryVersion(Display *dpy, int *major_inout, int *minor_inout)
{
    int rc = Success;

    XExtDisplayInfo *info = XInput_find_display(dpy);

    rc = _xiQueryVersion(dpy, major_inout, minor_inout, info);

    SyncHandle();
    return rc;
}

_X_HIDDEN Status
_xiQueryVersion(Display * dpy, int *major, int *minor, XExtDisplayInfo *info)
{
xXIQueryVersionReq *req;
xXIQueryVersionReply rep;

LockDisplay(dpy);

/* This could mean either a malloc problem, or supported
    version < XInput_2_0 */
if (_XiCheckExtInit(dpy, XInput_2_0, info) == -1)
{
XExtensionVersion *ext;
XExtDisplayInfo *extinfo = XInput_find_display(dpy);

if (!extinfo || !extinfo->data) {
*major = 0;
*minor = 0;
return BadRequest;
}

ext = ((XInputData*)extinfo->data)->vers;

*major = ext->major_version;
*minor = ext->minor_version;
return BadRequest;
}

GetReq(XIQueryVersion, req);
req->reqType = info->codes->major_opcode;
req->ReqType = X_XIQueryVersion;
req->major_version = *major;
req->minor_version = *minor;

if (!_XReply(dpy, (xReply*)&rep, 0, xTrue)) {
UnlockDisplay(dpy);
return BadImplementation;
}

*major = rep.major_version;
*minor = rep.minor_version;

UnlockDisplay(dpy);
return Success;
}
