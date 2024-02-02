#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include "../headers/xlibint.h"
#include "../headers/extensions/XI2proto.h"
#include "../headers/extensions/XInput2.h"
#include "../headers/extensions/extutil.h"
#include <limits.h>
#include "XIint.h"


Status
XIGrabDevice(Display* dpy, int deviceid, Window grab_window, Time time,
             Cursor cursor, int grab_mode, int paired_device_mode,
             Bool owner_events, XIEventMask *mask)
{
    xXIGrabDeviceReq *req;
    xXIGrabDeviceReply reply;
    char* buff;
    int len;

    XExtDisplayInfo *extinfo = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_2_0, extinfo) == -1)
        return (NoSuchExtension);

    if (mask->mask_len > INT_MAX - 3 ||
        (mask->mask_len + 3)/4 >= 0xffff)
    {
        reply.status = BadValue;
        goto out;
    }

    /* mask->mask_len is in bytes, but we need 4-byte units on the wire,
     * and they need to be padded with 0 */
    len = (mask->mask_len + 3)/4;
    buff = calloc(4, len);
    if (!buff)
    {
        reply.status =  BadAlloc;
        goto out;
    }

    GetReq(XIGrabDevice, req);
    req->reqType  = extinfo->codes->major_opcode;
    req->ReqType  = X_XIGrabDevice;
    req->deviceid = deviceid;
    req->grab_window = grab_window;
    req->time = time;
    req->grab_mode = grab_mode;
    req->paired_device_mode = paired_device_mode;
    req->owner_events = owner_events;
    req->mask_len = len;
    req->cursor = cursor;

    memcpy(buff, mask->mask, mask->mask_len);

    SetReqLen(req, len, len);
    Data(dpy, buff, len * 4);
    free(buff);

    if (_XReply(dpy, (xReply *)&reply, 0, xTrue) == 0)
        reply.status = GrabSuccess;

    out:
    UnlockDisplay(dpy);
    SyncHandle();

    return reply.status;
}

Status
XIUngrabDevice(Display* dpy, int deviceid, Time time)
{
    xXIUngrabDeviceReq *req;

    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_2_0, info) == -1)
        return (NoSuchExtension);

    GetReq(XIUngrabDevice, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_XIUngrabDevice;

    req->deviceid = deviceid;
    req->time = time;

    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}


