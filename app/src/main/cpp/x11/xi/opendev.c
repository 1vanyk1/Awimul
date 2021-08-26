#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/XIstubs.h"
#include "../headers/windowstr.h"          /* window structure  */
#include "../headers/exglobals.h"
#include "../headers/exevents.h"

#include "opendev.h"

extern CARD8 event_base[];

/***********************************************************************
 *
 * This procedure swaps the request if the server and client have different
 * byte orderings.
 *
 */

int _X_COLD
SProcXOpenDevice(ClientPtr client)
{
REQUEST(xOpenDeviceReq);
swaps(&stuff->length);
return (ProcXOpenDevice(client));
}

/***********************************************************************
 *
 * This procedure causes the server to open an input device.
 *
 */

int
ProcXOpenDevice(ClientPtr client)
{
    xInputClassInfo evbase[numInputClasses];
    int j = 0;
    int status = Success;
    xOpenDeviceReply rep;
    DeviceIntPtr dev;

    REQUEST(xOpenDeviceReq);
    REQUEST_SIZE_MATCH(xOpenDeviceReq);

    status = dixLookupDevice(&dev, stuff->deviceid, client, DixUseAccess);

    if (status == BadDevice) {  /* not open */
        for (dev = inputInfo.off_devices; dev; dev = dev->next)
            if (dev->id == stuff->deviceid)
                break;
        if (dev == NULL)
            return BadDevice;
    }
    else if (status != Success)
        return status;

    if (IsMaster(dev))
        return BadDevice;

    if (status != Success)
        return status;

    if (dev->key != NULL) {
        evbase[j].class = KeyClass;
        evbase[j++].event_type_base = event_base[KeyClass];
    }
    if (dev->button != NULL) {
        evbase[j].class = ButtonClass;
        evbase[j++].event_type_base = event_base[ButtonClass];
    }
    if (dev->valuator != NULL) {
        evbase[j].class = ValuatorClass;
        evbase[j++].event_type_base = event_base[ValuatorClass];
    }
    if (dev->kbdfeed != NULL || dev->ptrfeed != NULL || dev->leds != NULL ||
        dev->intfeed != NULL || dev->bell != NULL || dev->stringfeed != NULL) {
        evbase[j].class = FeedbackClass;
        evbase[j++].event_type_base = event_base[FeedbackClass];
    }
    if (dev->focus != NULL) {
        evbase[j].class = FocusClass;
        evbase[j++].event_type_base = event_base[FocusClass];
    }
    if (dev->proximity != NULL) {
        evbase[j].class = ProximityClass;
        evbase[j++].event_type_base = event_base[ProximityClass];
    }
    evbase[j].class = OtherClass;
    evbase[j++].event_type_base = event_base[OtherClass];
    rep = (xOpenDeviceReply) {
            .repType = X_Reply,
            .RepType = X_OpenDevice,
            .sequenceNumber = client->sequence,
            .length = bytes_to_int32(j * sizeof(xInputClassInfo)),
            .num_classes = j
    };
    WriteReplyToClient(client, sizeof(xOpenDeviceReply), &rep);
    WriteToClient(client, j * sizeof(xInputClassInfo), evbase);
    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XOpenDevice function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXOpenDevice(ClientPtr client, int size, xOpenDeviceReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
WriteToClient(client, size, rep);
}
