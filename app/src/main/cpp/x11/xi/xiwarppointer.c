#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/x.h"              /* for inputstr.h    */
#include "../headers/xproto.h"         /* Request macro     */
#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"          /* window structure  */
#include "../headers/scrnintstr.h"         /* screen structure  */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XI2proto.h"
#include "../headers/extnsionst.h"
#include "../headers/exevents.h"
#include "../headers/exglobals.h"
#include "../mi/mipointer.h"          /* for miPointerUpdateSprite */

#include "xiwarppointer.h"
/***********************************************************************
 *
 * This procedure allows a client to warp the pointer of a device.
 *
 */

int _X_COLD
SProcXIWarpPointer(ClientPtr client)
{
REQUEST(xXIWarpPointerReq);
REQUEST_SIZE_MATCH(xXIWarpPointerReq);

swaps(&stuff->length);
swapl(&stuff->src_win);
swapl(&stuff->dst_win);
swapl(&stuff->src_x);
swapl(&stuff->src_y);
swaps(&stuff->src_width);
swaps(&stuff->src_height);
swapl(&stuff->dst_x);
swapl(&stuff->dst_y);
swaps(&stuff->deviceid);
return (ProcXIWarpPointer(client));
}

int
ProcXIWarpPointer(ClientPtr client)
{
    int rc;
    int x, y;
    WindowPtr dest = NULL;
    DeviceIntPtr pDev;
    SpritePtr pSprite;
    ScreenPtr newScreen;
    int src_x, src_y;
    int dst_x, dst_y;

    REQUEST(xXIWarpPointerReq);
    REQUEST_SIZE_MATCH(xXIWarpPointerReq);

    /* FIXME: panoramix stuff is missing, look at ProcWarpPointer */

    rc = dixLookupDevice(&pDev, stuff->deviceid, client, DixWriteAccess);

    if (rc != Success) {
        client->errorValue = stuff->deviceid;
        return rc;
    }

    if ((!IsMaster(pDev) && !IsFloating(pDev)) ||
        (IsMaster(pDev) && !IsPointerDevice(pDev))) {
        client->errorValue = stuff->deviceid;
        return BadDevice;
    }

    if (stuff->dst_win != None) {
        rc = dixLookupWindow(&dest, stuff->dst_win, client, DixGetAttrAccess);
        if (rc != Success) {
            client->errorValue = stuff->dst_win;
            return rc;
        }
    }

    pSprite = pDev->spriteInfo->sprite;
    x = pSprite->hotPhys.x;
    y = pSprite->hotPhys.y;

    src_x = stuff->src_x / (double) (1 << 16);
    src_y = stuff->src_y / (double) (1 << 16);
    dst_x = stuff->dst_x / (double) (1 << 16);
    dst_y = stuff->dst_y / (double) (1 << 16);

    if (stuff->src_win != None) {
        int winX, winY;
        WindowPtr src;

        rc = dixLookupWindow(&src, stuff->src_win, client, DixGetAttrAccess);
        if (rc != Success) {
            client->errorValue = stuff->src_win;
            return rc;
        }

        winX = src->drawable.x;
        winY = src->drawable.y;
        if (src->drawable.pScreen != pSprite->hotPhys.pScreen ||
            x < winX + src_x ||
            y < winY + src_y ||
            (stuff->src_width != 0 &&
             winX + src_x + (int) stuff->src_width < 0) ||
            (stuff->src_height != 0 &&
             winY + src_y + (int) stuff->src_height < y) ||
            !PointInWindowIsVisible(src, x, y))
            return Success;
    }

    if (dest) {
        x = dest->drawable.x;
        y = dest->drawable.y;
        newScreen = dest->drawable.pScreen;
    }
    else
        newScreen = pSprite->hotPhys.pScreen;

    x += dst_x;
    y += dst_y;

    if (x < 0)
        x = 0;
    else if (x > newScreen->width)
        x = newScreen->width - 1;

    if (y < 0)
        y = 0;
    else if (y > newScreen->height)
        y = newScreen->height - 1;

    if (newScreen == pSprite->hotPhys.pScreen) {
        if (x < pSprite->physLimits.x1)
            x = pSprite->physLimits.x1;
        else if (x >= pSprite->physLimits.x2)
            x = pSprite->physLimits.x2 - 1;

        if (y < pSprite->physLimits.y1)
            y = pSprite->physLimits.y1;
        else if (y >= pSprite->physLimits.y2)
            y = pSprite->physLimits.y2 - 1;

        if (pSprite->hotShape)
            ConfineToShape(pDev, pSprite->hotShape, &x, &y);
        (*newScreen->SetCursorPosition) (pDev, newScreen, x, y, TRUE);
    }
    else if (!PointerConfinedToScreen(pDev)) {
        NewCurrentScreen(pDev, newScreen, x, y);
    }

    /* if we don't update the device, we get a jump next time it moves */
    pDev->last.valuators[0] = x;
    pDev->last.valuators[1] = y;
    miPointerUpdateSprite(pDev);

    if (*newScreen->CursorWarpedTo)
        (*newScreen->CursorWarpedTo) (pDev, newScreen, client,
                                      dest, pSprite, x, y);

    /* FIXME: XWarpPointer is supposed to generate an event. It doesn't do it
       here though. */
    return Success;
}
