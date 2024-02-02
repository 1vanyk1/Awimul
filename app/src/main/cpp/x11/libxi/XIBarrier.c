#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include "../headers/xlibint.h"
#include "../headers/extensions/XI2proto.h"
#include "../headers/extensions/XInput2.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

void
XIBarrierReleasePointers(Display *dpy,
                         XIBarrierReleasePointerInfo *barriers,
                         int num_barriers)
{
    XExtDisplayInfo	        *info = XInput_find_display(dpy);
    xXIBarrierReleasePointerReq *req;
    int extra = 0;
    int i;
    xXIBarrierReleasePointerInfo *b;

    if (!num_barriers)
        return;

    extra = (num_barriers * sizeof(xXIBarrierReleasePointerInfo));

    LockDisplay (dpy);
    GetReqExtra (XIBarrierReleasePointer, extra, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_XIBarrierReleasePointer;
    req->num_barriers = num_barriers;

    b = (xXIBarrierReleasePointerInfo *) &req[1];
    for (i = 0; i < num_barriers; i++, b++) {
        b->deviceid = barriers[i].deviceid;
        b->eventid = barriers[i].eventid;
        b->barrier = barriers[i].barrier;
    }

    UnlockDisplay (dpy);
    SyncHandle ();
}

void
XIBarrierReleasePointer(Display *dpy,
                        int            deviceid,
                        PointerBarrier barrier,
                        BarrierEventID eventid)
{
    XIBarrierReleasePointerInfo info;
    info.deviceid = deviceid;
    info.barrier = barrier;
    info.eventid = eventid;

    XIBarrierReleasePointers(dpy, &info, 1);
}
