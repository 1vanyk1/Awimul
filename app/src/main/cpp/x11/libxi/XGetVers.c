#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

XExtensionVersion *
XGetExtensionVersion(register Display * dpy, _Xconst char *name)
{
    XExtensionVersion *ext;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);

    if (_XiCheckExtInit(dpy, Dont_Check, info) == -1)
        return NULL;

    ext = _XiGetExtensionVersionRequest(dpy, name, info->codes->major_opcode);

    UnlockDisplay(dpy);
    SyncHandle();

    return ext;
}

_X_HIDDEN XExtensionVersion*
_XiGetExtensionVersionRequest(Display *dpy, _Xconst char *name, int xi_opcode)
{
xGetExtensionVersionReq *req;
xGetExtensionVersionReply rep;
XExtensionVersion *ext;

GetReq(GetExtensionVersion, req);
req->reqType = xi_opcode;
req->ReqType = X_GetExtensionVersion;
req->nbytes = strlen(name);
req->length += (unsigned)(req->nbytes + 3) >> 2;
_XSend(dpy, name, (long)req->nbytes);

if (!_XReply(dpy, (xReply *) & rep, 0, xTrue)) {
return NULL;
}

ext = (XExtensionVersion *) Xmalloc(sizeof(XExtensionVersion));
if (ext) {
ext->present = rep.present;
if (ext->present) {
ext->major_version = rep.major_version;
ext->minor_version = rep.minor_version;
}
}

return ext;
}
