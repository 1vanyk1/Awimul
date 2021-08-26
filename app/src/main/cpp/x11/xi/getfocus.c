#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/windowstr.h"          /* focus struct      */
#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exglobals.h"

#include "getfocus.h"

/***********************************************************************
 *
 * This procedure gets the focus for a device.
 *
 */

int _X_COLD
SProcXGetDeviceFocus(ClientPtr client)
{
REQUEST(xGetDeviceFocusReq);
swaps(&stuff->length);
return (ProcXGetDeviceFocus(client));
}

/***********************************************************************
 *
 * This procedure gets the focus for a device.
 *
 */

int
ProcXGetDeviceFocus(ClientPtr client)
{
    DeviceIntPtr dev;
    FocusClassPtr focus;
    xGetDeviceFocusReply rep;
    int rc;

    REQUEST(xGetDeviceFocusReq);
    REQUEST_SIZE_MATCH(xGetDeviceFocusReq);

    rc = dixLookupDevice(&dev, stuff->deviceid, client, DixGetFocusAccess);
    if (rc != Success)
        return rc;
    if (!dev->focus)
        return BadDevice;

    rep = (xGetDeviceFocusReply) {
            .repType = X_Reply,
            .RepType = X_GetDeviceFocus,
            .sequenceNumber = client->sequence,
            .length = 0
    };

    focus = dev->focus;

    if (focus->win == NoneWin)
        rep.focus = None;
    else if (focus->win == PointerRootWin)
        rep.focus = PointerRoot;
    else if (focus->win == FollowKeyboardWin)
        rep.focus = FollowKeyboard;
    else
        rep.focus = focus->win->drawable.id;

    rep.time = focus->time.milliseconds;
    rep.revertTo = focus->revert;
    WriteReplyToClient(client, sizeof(xGetDeviceFocusReply), &rep);
    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the GetDeviceFocus function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXGetDeviceFocus(ClientPtr client, int size, xGetDeviceFocusReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
swapl(&rep->focus);
swapl(&rep->time);
WriteToClient(client, size, rep);
}
