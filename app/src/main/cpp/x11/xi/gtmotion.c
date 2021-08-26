#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exevents.h"
#include "../headers/exglobals.h"

#include "gtmotion.h"

/***********************************************************************
 *
 * Swap the request if server and client have different byte ordering.
 *
 */

int _X_COLD
SProcXGetDeviceMotionEvents(ClientPtr client)
{
REQUEST(xGetDeviceMotionEventsReq);
swaps(&stuff->length);
REQUEST_SIZE_MATCH(xGetDeviceMotionEventsReq);
swapl(&stuff->start);
swapl(&stuff->stop);
return (ProcXGetDeviceMotionEvents(client));
}

/****************************************************************************
 *
 * Get the motion history for an extension pointer devices.
 *
 */

int
ProcXGetDeviceMotionEvents(ClientPtr client)
{
    INT32 *coords = NULL, *bufptr;
    xGetDeviceMotionEventsReply rep;
    unsigned long i;
    int rc, num_events, axes, size = 0;
    unsigned long nEvents;
    DeviceIntPtr dev;
    TimeStamp start, stop;
    int length = 0;
    ValuatorClassPtr v;

    REQUEST(xGetDeviceMotionEventsReq);

    REQUEST_SIZE_MATCH(xGetDeviceMotionEventsReq);
    rc = dixLookupDevice(&dev, stuff->deviceid, client, DixReadAccess);
    if (rc != Success)
        return rc;
    v = dev->valuator;
    if (v == NULL || v->numAxes == 0)
        return BadMatch;
    if (dev->valuator->motionHintWindow)
        MaybeStopDeviceHint(dev, client);
    axes = v->numAxes;
    rep = (xGetDeviceMotionEventsReply) {
            .repType = X_Reply,
            .RepType = X_GetDeviceMotionEvents,
            .sequenceNumber = client->sequence,
            .length = 0,
            .nEvents = 0,
            .axes = axes,
            .mode = Absolute        /* XXX we don't do relative at the moment */
    };
    start = ClientTimeToServerTime(stuff->start);
    stop = ClientTimeToServerTime(stuff->stop);
    if (CompareTimeStamps(start, stop) == LATER ||
        CompareTimeStamps(start, currentTime) == LATER) {
        WriteReplyToClient(client, sizeof(xGetDeviceMotionEventsReply), &rep);
        return Success;
    }
    if (CompareTimeStamps(stop, currentTime) == LATER)
        stop = currentTime;
    num_events = v->numMotionEvents;
    if (num_events) {
        size = sizeof(Time) + (axes * sizeof(INT32));
        rep.nEvents = GetMotionHistory(dev, (xTimecoord **) &coords,   /* XXX */
                                       start.milliseconds, stop.milliseconds,
                                       (ScreenPtr) NULL, FALSE);
    }
    if (rep.nEvents > 0) {
        length = bytes_to_int32(rep.nEvents * size);
        rep.length = length;
    }
    nEvents = rep.nEvents;
    WriteReplyToClient(client, sizeof(xGetDeviceMotionEventsReply), &rep);
    if (nEvents) {
        if (client->swapped) {
            bufptr = coords;
            for (i = 0; i < nEvents * (axes + 1); i++) {
                swapl(bufptr);
                bufptr++;
            }
        }
        WriteToClient(client, length * 4, coords);
    }
    free(coords);
    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XGetDeviceMotionEvents function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXGetDeviceMotionEvents(ClientPtr client, int size,
        xGetDeviceMotionEventsReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
swapl(&rep->nEvents);
WriteToClient(client, size, rep);
}
