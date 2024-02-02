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
XGrabDeviceKey(
        register Display	*dpy,
        XDevice		*dev,
        unsigned int	 key,		/* CARD8 */
        unsigned int	 modifiers,	/* CARD16 */
        XDevice		*modifier_device,
        Window		 grab_window,
        Bool		 owner_events,
        unsigned int	 event_count,
        XEventClass		*event_list,
        int			 this_device_mode,
        int			 other_devices_mode)
{
    register xGrabDeviceKeyReq *req;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);

    GetReq(GrabDeviceKey, req);

    req->reqType = info->codes->major_opcode;
    req->ReqType = X_GrabDeviceKey;
    req->grabbed_device = dev->device_id;
    req->key = key;
    req->modifiers = modifiers;
    if (modifier_device)
        req->modifier_device = modifier_device->device_id;
    else
        req->modifier_device = UseXKeyboard;
    req->grabWindow = grab_window;
    req->ownerEvents = owner_events;
    req->event_count = event_count;
    req->this_device_mode = this_device_mode;
    req->other_devices_mode = other_devices_mode;
    req->length += event_count;

    /* note: Data is a macro that uses its arguments multiple
     * times, so "nvalues" is changed in a separate assignment
     * statement */

    if (event_count) {
        event_count <<= 2;
        Data32(dpy, (long *)event_list, event_count);
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}
