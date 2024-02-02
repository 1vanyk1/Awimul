#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"
#include <limits.h>

XDeviceTimeCoord *
XGetDeviceMotionEvents(
        register Display	*dpy,
        XDevice		*dev,
        Time		 start,
        Time		 stop,
        int			*nEvents,
        int			*mode,
        int			*axis_count)
{
    xGetDeviceMotionEventsReq *req;
    xGetDeviceMotionEventsReply rep;
    XDeviceTimeCoord *tc;
    int *data, *bufp, *readp, *savp;
    unsigned long size;
    int i, j;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return NULL;

    GetReq(GetDeviceMotionEvents, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_GetDeviceMotionEvents;
    req->start = start;
    req->stop = stop;
    req->deviceid = dev->device_id;

    if (!_XReply(dpy, (xReply *) & rep, 0, xFalse)) {
        UnlockDisplay(dpy);
        SyncHandle();
        *nEvents = 0;
        return (NULL);
    }

    *mode = rep.mode;
    *axis_count = rep.axes;
    *nEvents = rep.nEvents;
    if (!rep.nEvents) {
        UnlockDisplay(dpy);
        SyncHandle();
        return (NULL);
    }
    if (rep.length < (INT_MAX >> 2)) {
        size = rep.length << 2;
        savp = readp = Xmalloc(size);
    } else {
        size = 0;
        savp = readp = NULL;
    }
    /* rep.axes is a CARD8, so assume max number of axes for bounds check */
    if (rep.nEvents <
        (INT_MAX / (sizeof(XDeviceTimeCoord) + (UCHAR_MAX * sizeof(int)))) &&
        rep.nEvents * (rep.axes + 1) <= rep.length) {
        size_t bsize = rep.nEvents *
                       (sizeof(XDeviceTimeCoord) + (rep.axes * sizeof(int)));
        bufp = Xmalloc(bsize);
    } else
        bufp = NULL;
    if (!bufp || !savp) {
        Xfree(bufp);
        Xfree(savp);
        *nEvents = 0;
        _XEatDataWords(dpy, rep.length);
        UnlockDisplay(dpy);
        SyncHandle();
        return (NULL);
    }
    _XRead(dpy, (char *)readp, size);

    tc = (XDeviceTimeCoord *) bufp;
    data = (int *)(tc + rep.nEvents);
    for (i = 0; i < *nEvents; i++, tc++) {
        tc->time = *readp++;
        tc->data = data;
        for (j = 0; j < *axis_count; j++)
            *data++ = *readp++;
    }
    XFree((char *)savp);
    UnlockDisplay(dpy);
    SyncHandle();
    return ((XDeviceTimeCoord *) bufp);
}

void
XFreeDeviceMotionEvents(XDeviceTimeCoord *events)
{
    XFree((char *)events);
}
