#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"          /* window struct      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exglobals.h"
#include "../headers/dixgrabs.h"
#include "../headers/xkbsrv.h"
#include "../headers/xkbstr.h"

#include "ungrdevk.h"

#define AllModifiersMask ( \
	ShiftMask | LockMask | ControlMask | Mod1Mask | Mod2Mask | \
	Mod3Mask | Mod4Mask | Mod5Mask )

/***********************************************************************
 *
 * Handle requests from a client with a different byte order.
 *
 */

int _X_COLD
SProcXUngrabDeviceKey(ClientPtr client)
{
REQUEST(xUngrabDeviceKeyReq);
swaps(&stuff->length);
REQUEST_SIZE_MATCH(xUngrabDeviceKeyReq);
swapl(&stuff->grabWindow);
swaps(&stuff->modifiers);
return (ProcXUngrabDeviceKey(client));
}

/***********************************************************************
 *
 * Release a grab of a key on an extension device.
 *
 */

int
ProcXUngrabDeviceKey(ClientPtr client)
{
    DeviceIntPtr dev;
    DeviceIntPtr mdev;
    WindowPtr pWin;
    GrabPtr temporaryGrab;
    int rc;

    REQUEST(xUngrabDeviceKeyReq);
    REQUEST_SIZE_MATCH(xUngrabDeviceKeyReq);

    rc = dixLookupDevice(&dev, stuff->grabbed_device, client, DixGrabAccess);
    if (rc != Success)
        return rc;
    if (dev->key == NULL)
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

    if (((stuff->key > dev->key->xkbInfo->desc->max_key_code) ||
         (stuff->key < dev->key->xkbInfo->desc->min_key_code))
        && (stuff->key != AnyKey))
        return BadValue;

    if ((stuff->modifiers != AnyModifier) &&
        (stuff->modifiers & ~AllModifiersMask))
        return BadValue;

    temporaryGrab = AllocGrab(NULL);
    if (!temporaryGrab)
        return BadAlloc;

    temporaryGrab->resource = client->clientAsMask;
    temporaryGrab->device = dev;
    temporaryGrab->window = pWin;
    temporaryGrab->type = DeviceKeyPress;
    temporaryGrab->grabtype = XI;
    temporaryGrab->modifierDevice = mdev;
    temporaryGrab->modifiersDetail.exact = stuff->modifiers;
    temporaryGrab->modifiersDetail.pMask = NULL;
    temporaryGrab->detail.exact = stuff->key;
    temporaryGrab->detail.pMask = NULL;

    DeletePassiveGrabFromList(temporaryGrab);
    FreeGrab(temporaryGrab);
    return Success;
}
