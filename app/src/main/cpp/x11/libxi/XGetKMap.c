#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <limits.h>
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

KeySym *
XGetDeviceKeyMapping(register Display * dpy, XDevice * dev,
#if NeedWidePrototypes
        unsigned int first,
#else
                     KeyCode first,
#endif
                     int keycount, int *syms_per_code)
{
    long nbytes;
    register KeySym *mapping = NULL;
    xGetDeviceKeyMappingReq *req;
    xGetDeviceKeyMappingReply rep;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return NULL;

    GetReq(GetDeviceKeyMapping, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_GetDeviceKeyMapping;
    req->deviceid = dev->device_id;
    req->firstKeyCode = first;
    req->count = keycount;

    if (!_XReply(dpy, (xReply *) & rep, 0, xFalse)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return (KeySym *) NULL;
    }
    if (rep.length > 0) {
        if (rep.length < INT_MAX >> 2 &&
            rep.length == rep.keySymsPerKeyCode * keycount) {
            *syms_per_code = rep.keySymsPerKeyCode;
            nbytes = (long)rep.length << 2;
            mapping = (KeySym *) Xmalloc((unsigned)nbytes);
        } else {
            *syms_per_code = 0;
            nbytes = 0;
            mapping = NULL;
        }
        if (mapping)
            _XRead(dpy, (char *)mapping, nbytes);
        else
            _XEatDataWords(dpy, rep.length);
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return (mapping);
}
