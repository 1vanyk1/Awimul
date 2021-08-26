#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/windowstr.h"          /* focus struct      */
#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"

#include "../headers/dixevents.h"

#include "../headers/exglobals.h"

#include "setfocus.h"

/***********************************************************************
 *
 * This procedure sets the focus for a device.
 *
 */

int _X_COLD
SProcXSetDeviceFocus(ClientPtr client)
{
REQUEST(xSetDeviceFocusReq);
swaps(&stuff->length);
REQUEST_SIZE_MATCH(xSetDeviceFocusReq);
swapl(&stuff->focus);
swapl(&stuff->time);
return (ProcXSetDeviceFocus(client));
}

/***********************************************************************
 *
 * This procedure sets the focus for a device.
 *
 */

int
ProcXSetDeviceFocus(ClientPtr client)
{
    int ret;
    DeviceIntPtr dev;

    REQUEST(xSetDeviceFocusReq);
    REQUEST_SIZE_MATCH(xSetDeviceFocusReq);

    ret = dixLookupDevice(&dev, stuff->device, client, DixSetFocusAccess);
    if (ret != Success)
        return ret;
    if (!dev->focus)
        return BadDevice;

    ret = SetInputFocus(client, dev, stuff->focus, stuff->revertTo,
                        stuff->time, TRUE);

    return ret;
}
