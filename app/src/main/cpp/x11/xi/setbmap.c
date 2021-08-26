#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exevents.h"
#include "../headers/exglobals.h"

#include "setbmap.h"

/***********************************************************************
 *
 * This procedure changes the button mapping.
 *
 */

int _X_COLD
SProcXSetDeviceButtonMapping(ClientPtr client)
{
REQUEST(xSetDeviceButtonMappingReq);
swaps(&stuff->length);
return (ProcXSetDeviceButtonMapping(client));
}

/***********************************************************************
 *
 * This procedure lists the input devices available to the server.
 *
 */

int
ProcXSetDeviceButtonMapping(ClientPtr client)
{
    int ret;
    xSetDeviceButtonMappingReply rep;
    DeviceIntPtr dev;

    REQUEST(xSetDeviceButtonMappingReq);
    REQUEST_AT_LEAST_SIZE(xSetDeviceButtonMappingReq);

    if (stuff->length !=
        bytes_to_int32(sizeof(xSetDeviceButtonMappingReq) + stuff->map_length))
        return BadLength;

    ret = dixLookupDevice(&dev, stuff->deviceid, client, DixManageAccess);
    if (ret != Success)
        return ret;

    rep = (xSetDeviceButtonMappingReply) {
            .repType = X_Reply,
            .RepType = X_SetDeviceButtonMapping,
            .sequenceNumber = client->sequence,
            .length = 0,
            .status = MappingSuccess
    };

    ret =
            ApplyPointerMapping(dev, (CARD8 *) &stuff[1], stuff->map_length,
                                client);
    if (ret == -1)
        return BadValue;
    else if (ret == MappingBusy)
        rep.status = ret;
    else if (ret != Success)
        return ret;

    WriteReplyToClient(client, sizeof(xSetDeviceButtonMappingReply), &rep);

    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XSetDeviceButtonMapping function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXSetDeviceButtonMapping(ClientPtr client, int size,
        xSetDeviceButtonMappingReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
WriteToClient(client, size, rep);
}
