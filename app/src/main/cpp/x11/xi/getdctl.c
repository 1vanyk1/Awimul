#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exglobals.h"

#include "getdctl.h"

/***********************************************************************
 *
 * This procedure gets the control attributes for an extension device,
 * for clients on machines with a different byte ordering than the server.
 *
 */

int _X_COLD
SProcXGetDeviceControl(ClientPtr client)
{
REQUEST(xGetDeviceControlReq);
swaps(&stuff->length);
REQUEST_SIZE_MATCH(xGetDeviceControlReq);
swaps(&stuff->control);
return (ProcXGetDeviceControl(client));
}

/***********************************************************************
 *
 * This procedure copies DeviceResolution data, swapping if necessary.
 *
 */

static void
CopySwapDeviceResolution(ClientPtr client, ValuatorClassPtr v, char *buf,
                         int length)
{
    AxisInfoPtr a;
    xDeviceResolutionState *r;
    int i, *iptr;

    r = (xDeviceResolutionState *) buf;
    r->control = DEVICE_RESOLUTION;
    r->length = length;
    r->num_valuators = v->numAxes;
    buf += sizeof(xDeviceResolutionState);
    iptr = (int *) buf;
    for (i = 0, a = v->axes; i < v->numAxes; i++, a++)
        *iptr++ = a->resolution;
    for (i = 0, a = v->axes; i < v->numAxes; i++, a++)
        *iptr++ = a->min_resolution;
    for (i = 0, a = v->axes; i < v->numAxes; i++, a++)
        *iptr++ = a->max_resolution;
    if (client->swapped) {
        swaps(&r->control);
        swaps(&r->length);
        swapl(&r->num_valuators);
        iptr = (int *) buf;
        for (i = 0; i < (3 * v->numAxes); i++, iptr++) {
            swapl(iptr);
        }
    }
}

static void
CopySwapDeviceCore(ClientPtr client, DeviceIntPtr dev, char *buf)
{
    xDeviceCoreState *c = (xDeviceCoreState *) buf;

    c->control = DEVICE_CORE;
    c->length = sizeof(xDeviceCoreState);
    c->status = dev->coreEvents;
    c->iscore = (dev == inputInfo.keyboard || dev == inputInfo.pointer);

    if (client->swapped) {
        swaps(&c->control);
        swaps(&c->length);
    }
}

static void
CopySwapDeviceEnable(ClientPtr client, DeviceIntPtr dev, char *buf)
{
    xDeviceEnableState *e = (xDeviceEnableState *) buf;

    e->control = DEVICE_ENABLE;
    e->length = sizeof(xDeviceEnableState);
    e->enable = dev->enabled;

    if (client->swapped) {
        swaps(&e->control);
        swaps(&e->length);
    }
}

/***********************************************************************
 *
 * This procedure writes the reply for the xGetDeviceControl function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXGetDeviceControl(ClientPtr client, int size, xGetDeviceControlReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
WriteToClient(client, size, rep);
}

/***********************************************************************
 *
 * Get the state of the specified device control.
 *
 */

int
ProcXGetDeviceControl(ClientPtr client)
{
    int rc, total_length = 0;
    char *buf, *savbuf;
    DeviceIntPtr dev;
    xGetDeviceControlReply rep;

    REQUEST(xGetDeviceControlReq);
    REQUEST_SIZE_MATCH(xGetDeviceControlReq);

    rc = dixLookupDevice(&dev, stuff->deviceid, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    rep = (xGetDeviceControlReply) {
            .repType = X_Reply,
            .RepType = X_GetDeviceControl,
            .sequenceNumber = client->sequence,
            .length = 0
    };

    switch (stuff->control) {
        case DEVICE_RESOLUTION:
            if (!dev->valuator)
                return BadMatch;
            total_length = sizeof(xDeviceResolutionState) +
                           (3 * sizeof(int) * dev->valuator->numAxes);
            break;
        case DEVICE_ABS_CALIB:
        case DEVICE_ABS_AREA:
            return BadMatch;
        case DEVICE_CORE:
            total_length = sizeof(xDeviceCoreState);
            break;
        case DEVICE_ENABLE:
            total_length = sizeof(xDeviceEnableState);
            break;
        default:
            return BadValue;
    }

    buf = (char *) malloc(total_length);
    if (!buf)
        return BadAlloc;
    savbuf = buf;

    switch (stuff->control) {
        case DEVICE_RESOLUTION:
            CopySwapDeviceResolution(client, dev->valuator, buf, total_length);
            break;
        case DEVICE_CORE:
            CopySwapDeviceCore(client, dev, buf);
            break;
        case DEVICE_ENABLE:
            CopySwapDeviceEnable(client, dev, buf);
            break;
        default:
            break;
    }

    rep.length = bytes_to_int32(total_length);
    WriteReplyToClient(client, sizeof(xGetDeviceControlReply), &rep);
    WriteToClient(client, total_length, savbuf);
    free(savbuf);
    return Success;
}
