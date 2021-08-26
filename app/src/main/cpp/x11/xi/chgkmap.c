#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exevents.h"
#include "../headers/exglobals.h"

#include "chgkmap.h"

/***********************************************************************
 *
 * This procedure swaps the request when the client and
 * server have different byte orderings.
 *
 */

int _X_COLD
SProcXChangeDeviceKeyMapping(ClientPtr client)
{
unsigned int count;

REQUEST(xChangeDeviceKeyMappingReq);
swaps(&stuff->length);
REQUEST_AT_LEAST_SIZE(xChangeDeviceKeyMappingReq);
count = stuff->keyCodes * stuff->keySymsPerKeyCode;
REQUEST_FIXED_SIZE(xChangeDeviceKeyMappingReq, count * sizeof(CARD32));
SwapLongs((CARD32 *) (&stuff[1]), count);
return (ProcXChangeDeviceKeyMapping(client));
}

/***********************************************************************
 *
 * Change the device key mapping.
 *
 */

int
ProcXChangeDeviceKeyMapping(ClientPtr client)
{
    int ret;
    unsigned len;
    DeviceIntPtr dev;
    unsigned int count;

    REQUEST(xChangeDeviceKeyMappingReq);
    REQUEST_AT_LEAST_SIZE(xChangeDeviceKeyMappingReq);

    count = stuff->keyCodes * stuff->keySymsPerKeyCode;
    REQUEST_FIXED_SIZE(xChangeDeviceKeyMappingReq, count * sizeof(CARD32));

    ret = dixLookupDevice(&dev, stuff->deviceid, client, DixManageAccess);
    if (ret != Success)
        return ret;
    len = stuff->length - bytes_to_int32(sizeof(xChangeDeviceKeyMappingReq));

    ret = ChangeKeyMapping(client, dev, len, DeviceMappingNotify,
                           stuff->firstKeyCode, stuff->keyCodes,
                           stuff->keySymsPerKeyCode, (KeySym *) &stuff[1]);

    return ret;
}
