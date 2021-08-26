#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"          /* window structure  */
#include "../headers/extensions/XI2.h"
#include "../headers/extensions/XI2proto.h"

#include "../headers/exglobals.h"          /* BadDevice */
#include "xisetdevfocus.h"

int _X_COLD
SProcXISetFocus(ClientPtr client)
{
REQUEST(xXISetFocusReq);
REQUEST_AT_LEAST_SIZE(xXISetFocusReq);

swaps(&stuff->length);
swaps(&stuff->deviceid);
swapl(&stuff->focus);
swapl(&stuff->time);

return ProcXISetFocus(client);
}

int _X_COLD
SProcXIGetFocus(ClientPtr client)
{
REQUEST(xXIGetFocusReq);
REQUEST_AT_LEAST_SIZE(xXIGetFocusReq);

swaps(&stuff->length);
swaps(&stuff->deviceid);

return ProcXIGetFocus(client);
}

int
ProcXISetFocus(ClientPtr client)
{
    DeviceIntPtr dev;
    int ret;

    REQUEST(xXISetFocusReq);
    REQUEST_AT_LEAST_SIZE(xXISetFocusReq);

    ret = dixLookupDevice(&dev, stuff->deviceid, client, DixSetFocusAccess);
    if (ret != Success)
        return ret;
    if (!dev->focus)
        return BadDevice;

    return SetInputFocus(client, dev, stuff->focus, RevertToParent,
                         stuff->time, TRUE);
}

int
ProcXIGetFocus(ClientPtr client)
{
    xXIGetFocusReply rep;
    DeviceIntPtr dev;
    int ret;

    REQUEST(xXIGetFocusReq);
    REQUEST_AT_LEAST_SIZE(xXIGetFocusReq);

    ret = dixLookupDevice(&dev, stuff->deviceid, client, DixGetFocusAccess);
    if (ret != Success)
        return ret;
    if (!dev->focus)
        return BadDevice;

    rep = (xXIGetFocusReply) {
            .repType = X_Reply,
            .RepType = X_XIGetFocus,
            .sequenceNumber = client->sequence,
            .length = 0
    };

    if (dev->focus->win == NoneWin)
        rep.focus = None;
    else if (dev->focus->win == PointerRootWin)
        rep.focus = PointerRoot;
    else if (dev->focus->win == FollowKeyboardWin)
        rep.focus = FollowKeyboard;
    else
        rep.focus = dev->focus->win->drawable.id;

    WriteReplyToClient(client, sizeof(xXIGetFocusReply), &rep);
    return Success;
}

void
SRepXIGetFocus(ClientPtr client, int len, xXIGetFocusReply * rep)
{
    swaps(&rep->sequenceNumber);
    swapl(&rep->length);
    swapl(&rep->focus);
    WriteToClient(client, len, rep);
}
