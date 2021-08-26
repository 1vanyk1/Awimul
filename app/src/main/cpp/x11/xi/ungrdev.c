#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"          /* window structure  */
#include "../headers/extensions/XIproto.h"
#include "../headers/exglobals.h"

#include "ungrdev.h"

/***********************************************************************
 *
 * Handle requests from a client with a different byte order.
 *
 */

int _X_COLD
SProcXUngrabDevice(ClientPtr client)
{
REQUEST(xUngrabDeviceReq);
swaps(&stuff->length);
REQUEST_SIZE_MATCH(xUngrabDeviceReq);
swapl(&stuff->time);
return (ProcXUngrabDevice(client));
}

/***********************************************************************
 *
 * Release a grab of an extension device.
 *
 */

int
ProcXUngrabDevice(ClientPtr client)
{
    DeviceIntPtr dev;
    GrabPtr grab;
    TimeStamp time;
    int rc;

    REQUEST(xUngrabDeviceReq);
    REQUEST_SIZE_MATCH(xUngrabDeviceReq);

    rc = dixLookupDevice(&dev, stuff->deviceid, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;
    grab = dev->deviceGrab.grab;

    time = ClientTimeToServerTime(stuff->time);
    if ((CompareTimeStamps(time, currentTime) != LATER) &&
        (CompareTimeStamps(time, dev->deviceGrab.grabTime) != EARLIER) &&
        (grab) && SameClient(grab, client) && grab->grabtype == XI)
        (*dev->deviceGrab.DeactivateGrab) (dev);
    return Success;
}
