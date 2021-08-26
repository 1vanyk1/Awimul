#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/XIstubs.h"
#include "../headers/exglobals.h"

#include "setmode.h"

/***********************************************************************
 *
 * Handle a request from a client with a different byte order.
 *
 */

int _X_COLD
SProcXSetDeviceMode(ClientPtr client)
{
REQUEST(xSetDeviceModeReq);
swaps(&stuff->length);
return (ProcXSetDeviceMode(client));
}

/***********************************************************************
 *
 * This procedure sets the mode of a device.
 *
 */

int
ProcXSetDeviceMode(ClientPtr client)
{
    DeviceIntPtr dev;
    xSetDeviceModeReply rep;
    int rc;

    REQUEST(xSetDeviceModeReq);
    REQUEST_SIZE_MATCH(xSetDeviceModeReq);

    rep = (xSetDeviceModeReply) {
            .repType = X_Reply,
            .RepType = X_SetDeviceMode,
            .sequenceNumber = client->sequence,
            .length = 0
    };

    rc = dixLookupDevice(&dev, stuff->deviceid, client, DixSetAttrAccess);
    if (rc != Success)
        return rc;
    if (dev->valuator == NULL)
        return BadMatch;

    if (IsXTestDevice(dev, NULL))
        return BadMatch;

    if ((dev->deviceGrab.grab) && !SameClient(dev->deviceGrab.grab, client))
        rep.status = AlreadyGrabbed;
    else
        rep.status = SetDeviceMode(client, dev, stuff->mode);

    if (rep.status == Success)
        valuator_set_mode(dev, VALUATOR_MODE_ALL_AXES, stuff->mode);
    else if (rep.status != AlreadyGrabbed) {
        switch (rep.status) {
            case BadMatch:
            case BadImplementation:
            case BadAlloc:
                break;
            default:
                rep.status = BadMode;
        }
        return rep.status;
    }

    WriteReplyToClient(client, sizeof(xSetDeviceModeReply), &rep);
    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XSetDeviceMode function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXSetDeviceMode(ClientPtr client, int size, xSetDeviceModeReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
WriteToClient(client, size, rep);
}
