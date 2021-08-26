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
#include "grabdevk.h"

/***********************************************************************
 *
 * Handle requests from clients with a different byte order.
 *
 */

int _X_COLD
SProcXGrabDeviceKey(ClientPtr client)
{
    REQUEST(xGrabDeviceKeyReq);
    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xGrabDeviceKeyReq);
    swapl(&stuff->grabWindow);
    swaps(&stuff->modifiers);
    swaps(&stuff->event_count);
    REQUEST_FIXED_SIZE(xGrabDeviceKeyReq, stuff->event_count * sizeof(CARD32));
    SwapLongs((CARD32 *) (&stuff[1]), stuff->event_count);
    return (ProcXGrabDeviceKey(client));
}

/***********************************************************************
 *
 * Grab a key on an extension device.
 *
 */

int
ProcXGrabDeviceKey(ClientPtr client)
{
    int ret;
    DeviceIntPtr dev;
    DeviceIntPtr mdev;
    XEventClass *class;
    struct tmask tmp[EMASKSIZE];
    GrabParameters param;
    GrabMask mask;

    REQUEST(xGrabDeviceKeyReq);
    REQUEST_AT_LEAST_SIZE(xGrabDeviceKeyReq);

    if (stuff->length !=
        bytes_to_int32(sizeof(xGrabDeviceKeyReq)) + stuff->event_count)
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
                                  X_GrabDeviceKey)) != Success)
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

    ret = GrabKey(client, dev, mdev, stuff->key, &param, XI, &mask);

    return ret;
}
