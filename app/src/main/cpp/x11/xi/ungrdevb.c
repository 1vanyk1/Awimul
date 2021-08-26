#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"          /* focus struct      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exglobals.h"
#include "../headers/dixgrabs.h"

#include "ungrdevb.h"

#define AllModifiersMask ( \
	ShiftMask | LockMask | ControlMask | Mod1Mask | Mod2Mask | \
	Mod3Mask | Mod4Mask | Mod5Mask )

/***********************************************************************
 *
 * Handle requests from a client with a different byte order.
 *
 */

int _X_COLD
SProcXUngrabDeviceButton(ClientPtr client)
{
REQUEST(xUngrabDeviceButtonReq);
swaps(&stuff->length);
REQUEST_SIZE_MATCH(xUngrabDeviceButtonReq);
swapl(&stuff->grabWindow);
swaps(&stuff->modifiers);
return (ProcXUngrabDeviceButton(client));
}

/***********************************************************************
 *
 * Release a grab of a button on an extension device.
 *
 */

int
ProcXUngrabDeviceButton(ClientPtr client)
{
    DeviceIntPtr dev;
    DeviceIntPtr mdev;
    WindowPtr pWin;
    GrabPtr temporaryGrab;
    int rc;

    REQUEST(xUngrabDeviceButtonReq);
    REQUEST_SIZE_MATCH(xUngrabDeviceButtonReq);

    rc = dixLookupDevice(&dev, stuff->grabbed_device, client, DixGrabAccess);
    if (rc != Success)
        return rc;
    if (dev->button == NULL)
        return BadMatch;

    if (stuff->modifier_device != UseXKeyboard) {
        rc = dixLookupDevice(&mdev, stuff->modifier_device, client,
                             DixReadAccess);
        if (rc != Success)
            return BadDevice;
        if (mdev->key == NULL)
            return BadMatch;
    }
    else
        mdev = PickKeyboard(client);

    rc = dixLookupWindow(&pWin, stuff->grabWindow, client, DixSetAttrAccess);
    if (rc != Success)
        return rc;

    if ((stuff->modifiers != AnyModifier) &&
        (stuff->modifiers & ~AllModifiersMask))
        return BadValue;

    temporaryGrab = AllocGrab(NULL);
    if (!temporaryGrab)
        return BadAlloc;

    temporaryGrab->resource = client->clientAsMask;
    temporaryGrab->device = dev;
    temporaryGrab->window = pWin;
    temporaryGrab->type = DeviceButtonPress;
    temporaryGrab->grabtype = XI;
    temporaryGrab->modifierDevice = mdev;
    temporaryGrab->modifiersDetail.exact = stuff->modifiers;
    temporaryGrab->modifiersDetail.pMask = NULL;
    temporaryGrab->detail.exact = stuff->button;
    temporaryGrab->detail.pMask = NULL;

    DeletePassiveGrabFromList(temporaryGrab);

    FreeGrab(temporaryGrab);
    return Success;
}
