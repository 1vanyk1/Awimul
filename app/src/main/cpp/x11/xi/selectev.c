#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"          /* window structure  */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XI2.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exevents.h"
#include "../headers/exglobals.h"

#include "grabdev.h"
#include "selectev.h"

static int
HandleDevicePresenceMask(ClientPtr client, WindowPtr win,
                         XEventClass * cls, CARD16 *count)
{
    int i, j;
    Mask mask;

    /* We use the device ID 256 to select events that aren't bound to
     * any device.  For now we only handle the device presence event,
     * but this could be extended to other events that aren't bound to
     * a device.
     *
     * In order not to break in CreateMaskFromList() we remove the
     * entries with device ID 256 from the XEventClass array.
     */

    mask = 0;
    for (i = 0, j = 0; i < *count; i++) {
        if (cls[i] >> 8 != 256) {
            cls[j] = cls[i];
            j++;
            continue;
        }

        switch (cls[i] & 0xff) {
            case _devicePresence:
                mask |= DevicePresenceNotifyMask;
                break;
        }
    }

    *count = j;

    if (mask == 0)
        return Success;

    /* We always only use mksidx = AllDevices for events not bound to
     * devices */
    if (AddExtensionClient(win, client, mask, XIAllDevices) != Success)
        return BadAlloc;

    RecalculateDeviceDeliverableEvents(win);

    return Success;
}

/***********************************************************************
 *
 * Handle requests from clients with a different byte order.
 *
 */

int _X_COLD
SProcXSelectExtensionEvent(ClientPtr client)
{
    REQUEST(xSelectExtensionEventReq);
    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xSelectExtensionEventReq);
    swapl(&stuff->window);
    swaps(&stuff->count);
    REQUEST_FIXED_SIZE(xSelectExtensionEventReq, stuff->count * sizeof(CARD32));
    SwapLongs((CARD32 *) (&stuff[1]), stuff->count);

    return (ProcXSelectExtensionEvent(client));
}

/***********************************************************************
 *
 * This procedure selects input from an extension device.
 *
 */

int
ProcXSelectExtensionEvent(ClientPtr client)
{
    int ret;
    int i;
    WindowPtr pWin;
    struct tmask tmp[EMASKSIZE];

    REQUEST(xSelectExtensionEventReq);
    REQUEST_AT_LEAST_SIZE(xSelectExtensionEventReq);

    if (stuff->length !=
        bytes_to_int32(sizeof(xSelectExtensionEventReq)) + stuff->count)
        return BadLength;

    ret = dixLookupWindow(&pWin, stuff->window, client, DixReceiveAccess);
    if (ret != Success)
        return ret;

    if (HandleDevicePresenceMask(client, pWin, (XEventClass *) &stuff[1],
                                 &stuff->count) != Success)
        return BadAlloc;

    if ((ret = CreateMaskFromList(client, (XEventClass *) &stuff[1],
                                  stuff->count, tmp, NULL,
                                  X_SelectExtensionEvent)) != Success)
        return ret;

    for (i = 0; i < EMASKSIZE; i++)
        if (tmp[i].dev != NULL) {
            if (tmp[i].mask & ~XIAllMasks) {
                client->errorValue = tmp[i].mask;
                return BadValue;
            }
            if ((ret =
                         SelectForWindow((DeviceIntPtr) tmp[i].dev, pWin, client,
                                         tmp[i].mask, DeviceButtonGrabMask)) != Success)
                return ret;
        }

    return Success;
}
