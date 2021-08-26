#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/XIstubs.h"
#include "../headers/exglobals.h"

#include "setdval.h"

/***********************************************************************
 *
 * Handle a request from a client with a different byte order.
 *
 */

int _X_COLD
SProcXSetDeviceValuators(ClientPtr client)
{
REQUEST(xSetDeviceValuatorsReq);
swaps(&stuff->length);
return (ProcXSetDeviceValuators(client));
}

/***********************************************************************
 *
 * This procedure sets the value of valuators on an extension input device.
 *
 */

int
ProcXSetDeviceValuators(ClientPtr client)
{
    DeviceIntPtr dev;
    xSetDeviceValuatorsReply rep;
    int rc;

    REQUEST(xSetDeviceValuatorsReq);
    REQUEST_AT_LEAST_SIZE(xSetDeviceValuatorsReq);

    rep = (xSetDeviceValuatorsReply) {
            .repType = X_Reply,
            .RepType = X_SetDeviceValuators,
            .sequenceNumber = client->sequence,
            .length = 0,
            .status = Success
    };

    if (stuff->length != bytes_to_int32(sizeof(xSetDeviceValuatorsReq)) +
                         stuff->num_valuators)
        return BadLength;

    rc = dixLookupDevice(&dev, stuff->deviceid, client, DixSetAttrAccess);
    if (rc != Success)
        return rc;
    if (dev->valuator == NULL)
        return BadMatch;

    if (IsXTestDevice(dev, NULL))
        return BadMatch;

    if (stuff->first_valuator + stuff->num_valuators > dev->valuator->numAxes)
        return BadValue;

    if ((dev->deviceGrab.grab) && !SameClient(dev->deviceGrab.grab, client))
        rep.status = AlreadyGrabbed;
    else
        rep.status = SetDeviceValuators(client, dev, (int *) &stuff[1],
                                        stuff->first_valuator,
                                        stuff->num_valuators);

    if (rep.status != Success && rep.status != AlreadyGrabbed)
        return rep.status;

    WriteReplyToClient(client, sizeof(xSetDeviceValuatorsReply), &rep);
    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XSetDeviceValuators function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXSetDeviceValuators(ClientPtr client, int size,
        xSetDeviceValuatorsReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
WriteToClient(client, size, rep);
}
