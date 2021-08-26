#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"          /* window structure  */
#include "../headers/scrnintstr.h"         /* screen structure  */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/XIstubs.h"
#include "../headers/exglobals.h"

#include "closedev.h"

/***********************************************************************
 *
 * This procedure closes an input device.
 *
 */

int _X_COLD
SProcXCloseDevice(ClientPtr client)
{
REQUEST(xCloseDeviceReq);
swaps(&stuff->length);
REQUEST_SIZE_MATCH(xCloseDeviceReq);
return (ProcXCloseDevice(client));
}

/***********************************************************************
 *
 * Clear out event selections and passive grabs from a window for the
 * specified device.
 *
 */

static void
DeleteDeviceEvents(DeviceIntPtr dev, WindowPtr pWin, ClientPtr client)
{
    InputClientsPtr others;
    OtherInputMasks *pOthers;
    GrabPtr grab, next;

    if ((pOthers = wOtherInputMasks(pWin)) != 0)
        for (others = pOthers->inputClients; others; others = others->next)
            if (SameClient(others, client))
                others->mask[dev->id] = NoEventMask;

    for (grab = wPassiveGrabs(pWin); grab; grab = next) {
        next = grab->next;
        if ((grab->device == dev) &&
            (client->clientAsMask == CLIENT_BITS(grab->resource)))
            FreeResource(grab->resource, RT_NONE);
    }
}

/***********************************************************************
 *
 * Walk through the window tree, deleting event selections for this client
 * from this device from all windows.
 *
 */

static void
DeleteEventsFromChildren(DeviceIntPtr dev, WindowPtr p1, ClientPtr client)
{
    WindowPtr p2;

    while (p1) {
        p2 = p1->firstChild;
        DeleteDeviceEvents(dev, p1, client);
        DeleteEventsFromChildren(dev, p2, client);
        p1 = p1->nextSib;
    }
}

/***********************************************************************
 *
 * This procedure closes an input device.
 *
 */

int
ProcXCloseDevice(ClientPtr client)
{
    int rc, i;
    WindowPtr pWin, p1;
    DeviceIntPtr d;

    REQUEST(xCloseDeviceReq);
    REQUEST_SIZE_MATCH(xCloseDeviceReq);

    rc = dixLookupDevice(&d, stuff->deviceid, client, DixUseAccess);
    if (rc != Success)
        return rc;

    if (d->deviceGrab.grab && SameClient(d->deviceGrab.grab, client))
        (*d->deviceGrab.DeactivateGrab) (d);    /* release active grab */

    /* Remove event selections from all windows for events from this device
     * and selected by this client.
     * Delete passive grabs from all windows for this device.      */

    for (i = 0; i < screenInfo.numScreens; i++) {
        pWin = screenInfo.screens[i]->root;
        DeleteDeviceEvents(d, pWin, client);
        p1 = pWin->firstChild;
        DeleteEventsFromChildren(d, p1, client);
    }

    return Success;
}
