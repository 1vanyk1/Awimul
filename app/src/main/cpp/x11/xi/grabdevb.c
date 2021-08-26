#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"          /* window structure  */
#include "../headers/extensions/XIproto.h"
#include "../headers/extensions/XI.h"
#include "../headers/exevents.h"
#include "../headers/exglobals.h"
#include "../xext/xace.h"

#include "grabdev.h"
#include "grabdevb.h"

/***********************************************************************
 *
 * Handle requests from clients with a different byte order.
 *
 */

int _X_COLD
SProcXGrabDeviceButton(ClientPtr client)
{
    REQUEST(xGrabDeviceButtonReq);
    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xGrabDeviceButtonReq);
    swapl(&stuff->grabWindow);
    swaps(&stuff->modifiers);
    swaps(&stuff->event_count);
    REQUEST_FIXED_SIZE(xGrabDeviceButtonReq,
                       stuff->event_count * sizeof(CARD32));
    SwapLongs((CARD32 *) (&stuff[1]), stuff->event_count);

    return (ProcXGrabDeviceButton(client));
}

/***********************************************************************
 *
 * Grab a button on an extension device.
 *
 */

int
ProcXGrabDeviceButton(ClientPtr client)
{
    int ret;
    DeviceIntPtr dev;
    DeviceIntPtr mdev;
    XEventClass *class;
    struct tmask tmp[EMASKSIZE];
    GrabParameters param;
    GrabMask mask;

    REQUEST(xGrabDeviceButtonReq);
    REQUEST_AT_LEAST_SIZE(xGrabDeviceButtonReq);

    if (stuff->length !=
        bytes_to_int32(sizeof(xGrabDeviceButtonReq)) + stuff->event_count)
        return BadLength;

    ret = dixLookupDevice(&dev, stuff->grabbed_device, client, DixGrabAccess);
    if (ret != Success)
        return ret;

    if (stuff->modifier_device != UseXKeyboard) {
        ret = dixLookupDevice(&mdev, stuff->modifier_device, client,
                              DixUseAccess);
        if (ret != Success)
            return ret;
        if (mdev->key == NULL)
            return BadMatch;
    }
    else {
        mdev = PickKeyboard(client);
        ret = XaceHook(XACE_DEVICE_ACCESS, client, mdev, DixUseAccess);
        if (ret != Success)
            return ret;
    }

    class = (XEventClass *) (&stuff[1]);        /* first word of values */

    if ((ret = CreateMaskFromList(client, class,
                                  stuff->event_count, tmp, dev,
                                  X_GrabDeviceButton)) != Success)
        return ret;

    param = (GrabParameters) {
            .grabtype = XI,
            .ownerEvents = stuff->ownerEvents,
            .this_device_mode = stuff->this_device_mode,
            .other_devices_mode = stuff->other_devices_mode,
            .grabWindow = stuff->grabWindow,
            .modifiers = stuff->modifiers
    };
    mask.xi = tmp[stuff->grabbed_device].mask;

    ret = GrabButton(client, dev, mdev, stuff->button, &param, XI, &mask);

    return ret;
}
