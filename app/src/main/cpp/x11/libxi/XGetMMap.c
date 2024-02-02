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

XModifierKeymap *
XGetDeviceModifierMapping(
        register Display	*dpy,
        XDevice		*dev)
{
    unsigned long nbytes;
    XModifierKeymap *res;
    xGetDeviceModifierMappingReq *req;
    xGetDeviceModifierMappingReply rep;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return NULL;

    GetReq(GetDeviceModifierMapping, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_GetDeviceModifierMapping;
    req->deviceid = dev->device_id;

    if (!_XReply(dpy, (xReply *) & rep, 0, xFalse)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return (XModifierKeymap *) NULL;
    }
    if (rep.length < (INT_MAX >> 2) &&
        rep.numKeyPerModifier == rep.length >> 1) {
        nbytes = (unsigned long)rep.length << 2;
        res = (XModifierKeymap *) Xmalloc(sizeof(XModifierKeymap));
    } else {
        nbytes = 0;
        res = NULL;
    }
    if (res) {
        res->modifiermap = (KeyCode *) Xmalloc(nbytes);
        if (res->modifiermap)
            _XReadPad(dpy, (char *)res->modifiermap, nbytes);
        else
            _XEatDataWords(dpy, rep.length);
        res->max_keypermod = rep.numKeyPerModifier;
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return (res);
}
