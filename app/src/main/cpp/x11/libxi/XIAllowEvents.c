#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include "../headers/xlibint.h"
#include "../headers/extensions/XI2proto.h"
#include "../headers/extensions/XInput2.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

/* for GetRequest() to work */
#define X_XI2_2AllowEvents X_XIAllowEvents

static Status
_XIAllowEvents(Display *dpy, int deviceid, int event_mode, Time time,
               unsigned int touchid, Window grab_window)
{
    Bool have_XI22 = False;
    xXIAllowEventsReq *req;
    xXI2_2AllowEventsReq *req_XI22;

    XExtDisplayInfo *extinfo = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_2_0, extinfo) == -1)
        return (NoSuchExtension);

    if (_XiCheckVersion(extinfo, XInput_2_2) >= 0)
        have_XI22 = True;

    if (have_XI22)
    {
        GetReq(XI2_2AllowEvents, req_XI22);
        req = (xXIAllowEventsReq*)req_XI22;
    } else
        GetReq(XIAllowEvents, req);

    req->reqType = extinfo->codes->major_opcode;
    req->ReqType = X_XIAllowEvents;
    req->deviceid = deviceid;
    req->mode = event_mode;
    req->time = time;

    if (have_XI22) {
        req_XI22->touchid = touchid;
        req_XI22->grab_window = grab_window;
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return Success;
}

Status
XIAllowEvents(Display *dpy, int deviceid, int event_mode, Time time)
{
    return _XIAllowEvents(dpy, deviceid, event_mode, time, 0, None);
}

Status
XIAllowTouchEvents(Display *dpy, int deviceid, unsigned int touchid,
                   Window grab_window, int event_mode)
{
    XExtDisplayInfo *extinfo = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_2_2, extinfo) == -1)
        return (NoSuchExtension);
    UnlockDisplay(dpy);

    return _XIAllowEvents(dpy, deviceid, event_mode, CurrentTime, touchid, grab_window);
}
