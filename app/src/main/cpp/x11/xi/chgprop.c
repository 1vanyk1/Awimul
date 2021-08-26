#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"

#include "../headers/exevents.h"
#include "../headers/exglobals.h"

#include "chgprop.h"
#include "grabdev.h"

/***********************************************************************
 *
 * This procedure returns the extension version.
 *
 */

int _X_COLD
SProcXChangeDeviceDontPropagateList(ClientPtr client)
{
REQUEST(xChangeDeviceDontPropagateListReq);
swaps(&stuff->length);
REQUEST_AT_LEAST_SIZE(xChangeDeviceDontPropagateListReq);
swapl(&stuff->window);
swaps(&stuff->count);
REQUEST_FIXED_SIZE(xChangeDeviceDontPropagateListReq,
        stuff->count * sizeof(CARD32));
SwapLongs((CARD32 *) (&stuff[1]), stuff->count);
return (ProcXChangeDeviceDontPropagateList(client));
}

/***********************************************************************
 *
 * This procedure changes the dont-propagate list for the specified window.
 *
 */

int
ProcXChangeDeviceDontPropagateList(ClientPtr client)
{
    int i, rc;
    WindowPtr pWin;
    struct tmask tmp[EMASKSIZE];
    OtherInputMasks *others;

    REQUEST(xChangeDeviceDontPropagateListReq);
    REQUEST_AT_LEAST_SIZE(xChangeDeviceDontPropagateListReq);

    if (stuff->length !=
        bytes_to_int32(sizeof(xChangeDeviceDontPropagateListReq)) +
        stuff->count)
        return BadLength;

    rc = dixLookupWindow(&pWin, stuff->window, client, DixSetAttrAccess);
    if (rc != Success)
        return rc;

    if (stuff->mode != AddToList && stuff->mode != DeleteFromList) {
        client->errorValue = stuff->window;
        return BadMode;
    }

    if ((rc = CreateMaskFromList(client, (XEventClass *) &stuff[1],
                                 stuff->count, tmp, NULL,
                                 X_ChangeDeviceDontPropagateList)) != Success)
        return rc;

    others = wOtherInputMasks(pWin);
    if (!others && stuff->mode == DeleteFromList)
        return Success;
    for (i = 0; i < EMASKSIZE; i++) {
        if (tmp[i].mask == 0)
            continue;

        if (stuff->mode == DeleteFromList)
            tmp[i].mask = (others->dontPropagateMask[i] & ~tmp[i].mask);
        else if (others)
            tmp[i].mask |= others->dontPropagateMask[i];

        if (DeviceEventSuppressForWindow(pWin, client, tmp[i].mask, i) !=
            Success)
            return BadClass;
    }

    return Success;
}
