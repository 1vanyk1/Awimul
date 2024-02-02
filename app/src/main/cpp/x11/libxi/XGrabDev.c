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
XGrabDevice(
        register Display	*dpy,
        XDevice		*dev,
        Window		 grab_window,
        Bool		 ownerEvents,
        int			 event_count,
        XEventClass		*event_list,
        int			 this_device_mode,
        int			 other_devices_mode,
        Time		 time)
{
    xGrabDeviceReply rep;
    register xGrabDeviceReq *req;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);

    GetReq(GrabDevice, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_GrabDevice;

    req->deviceid = dev->device_id;
    req->grabWindow = grab_window;
    req->ownerEvents = ownerEvents;
    req->event_count = event_count;
    req->this_device_mode = this_device_mode;
    req->other_devices_mode = other_devices_mode;
    req->time = time;
    req->length += event_count;

    /* note: Data is a macro that uses its arguments multiple
     * times, so "nvalues" is changed in a separate assignment
     * statement */

    event_count <<= 2;
    Data32(dpy, (long *)event_list, event_count);

    if (_XReply(dpy, (xReply *) & rep, 0, xTrue) == 0)
        rep.status = GrabSuccess;
    UnlockDisplay(dpy);
    SyncHandle();
    return (rep.status);
}
