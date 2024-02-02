#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

/* Xlib's wire_vec is defined for a single event only, libXi may return
 * multiple events.
 */
typedef Status (*ext_event_to_wire)(Display*, XEvent*, xEvent**, int*);

Status
XSendExtensionEvent(
        register Display	*dpy,
        XDevice		*dev,
        Window		 dest,
        Bool		 prop,
        int			 count,
        XEventClass		*list,
        XEvent		*event)
{
    int num_events;
    int ev_size;
    xSendExtensionEventReq *req;
    xEvent *ev;
    ext_event_to_wire *fp;
    Status status;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);

    /* call through display to find proper conversion routine */

    fp = (ext_event_to_wire*)&dpy->wire_vec[event->type & 0177];
    if (*fp == NULL)
        *fp = _XiEventToWire;
    status = (*fp) (dpy, event, &ev, &num_events);

    if (status) {
        GetReq(SendExtensionEvent, req);
        req->reqType = info->codes->major_opcode;
        req->ReqType = X_SendExtensionEvent;
        req->deviceid = dev->device_id;
        req->destination = dest;
        req->propagate = prop;
        req->count = count;
        req->num_events = num_events;
        ev_size = num_events * sizeof(xEvent);
        req->length += (count + (ev_size >> 2));

        /* note: Data is a macro that uses its arguments multiple
         * times, so "count" is changed in a separate assignment
         * statement.  Any extra events must be sent before the event
         * list, in order to ensure quad alignment. */

        Data(dpy, (char *)ev, ev_size);

        count <<= 2;
        Data32(dpy, (long *)list, count);
        XFree((char *)ev);
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return (status);
}
