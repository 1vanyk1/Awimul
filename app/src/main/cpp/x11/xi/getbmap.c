#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exglobals.h"

#include "getbmap.h"

/***********************************************************************
 *
 * This procedure gets the button mapping for the specified device.
 *
 */

int _X_COLD
SProcXGetDeviceButtonMapping(ClientPtr client)
{
REQUEST(xGetDeviceButtonMappingReq);
swaps(&stuff->length);
return (ProcXGetDeviceButtonMapping(client));
}

/***********************************************************************
 *
 * This procedure gets the button mapping for the specified device.
 *
 */

int
ProcXGetDeviceButtonMapping(ClientPtr client)
{
    DeviceIntPtr dev;
    xGetDeviceButtonMappingReply rep;
    ButtonClassPtr b;
    int rc;

    REQUEST(xGetDeviceButtonMappingReq);
    REQUEST_SIZE_MATCH(xGetDeviceButtonMappingReq);

    rep = (xGetDeviceButtonMappingReply) {
            .repType = X_Reply,
            .RepType = X_GetDeviceButtonMapping,
            .sequenceNumber = client->sequence,
            .nElts = 0,
            .length = 0
    };

    rc = dixLookupDevice(&dev, stuff->deviceid, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    b = dev->button;
    if (b == NULL)
        return BadMatch;

    rep.nElts = b->numButtons;
    rep.length = bytes_to_int32(rep.nElts);
    WriteReplyToClient(client, sizeof(xGetDeviceButtonMappingReply), &rep);
    WriteToClient(client, rep.nElts, &b->map[1]);
    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XGetDeviceButtonMapping function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXGetDeviceButtonMapping(ClientPtr client, int size,
        xGetDeviceButtonMappingReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
WriteToClient(client, size, rep);
}
