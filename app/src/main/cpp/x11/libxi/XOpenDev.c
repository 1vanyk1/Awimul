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

XDevice *
XOpenDevice(
        register Display	*dpy,
        register XID	 id)
{
    register long rlen;	/* raw length */
    xOpenDeviceReq *req;
    xOpenDeviceReply rep;
    XDevice *dev;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return NULL;

    GetReq(OpenDevice, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_OpenDevice;
    req->deviceid = id;

    if (!_XReply(dpy, (xReply *) & rep, 0, xFalse)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return (XDevice *) NULL;
    }

    if (rep.length < INT_MAX >> 2 &&
        (rep.length << 2) >= rep.num_classes * sizeof(xInputClassInfo)) {
        rlen = rep.length << 2;
        dev = (XDevice *) Xmalloc(sizeof(XDevice) + rep.num_classes *
                                                    sizeof(XInputClassInfo));
    } else {
        rlen = 0;
        dev = NULL;
    }
    if (dev) {
        int dlen;	/* data length */

        dev->device_id = req->deviceid;
        dev->num_classes = rep.num_classes;
        dev->classes = (XInputClassInfo *) ((char *)dev + sizeof(XDevice));
        dlen = rep.num_classes * sizeof(xInputClassInfo);
        _XRead(dpy, (char *)dev->classes, dlen);
        /* could be padding that we still need to eat (yummy!) */
        if (rlen - dlen > 0)
            _XEatData(dpy, (unsigned long)rlen - dlen);
    } else
        _XEatDataWords(dpy, rep.length);

    UnlockDisplay(dpy);
    SyncHandle();
    return (dev);
}
