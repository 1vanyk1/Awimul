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
#include "../headers/input.h"

#include "xichangecursor.h"

/***********************************************************************
 *
 * This procedure allows a client to set one pointer's cursor.
 *
 */

int _X_COLD
SProcXIChangeCursor(ClientPtr client)
{
REQUEST(xXIChangeCursorReq);
REQUEST_SIZE_MATCH(xXIChangeCursorReq);
swaps(&stuff->length);
swapl(&stuff->win);
swapl(&stuff->cursor);
swaps(&stuff->deviceid);
return (ProcXIChangeCursor(client));
}

int
ProcXIChangeCursor(ClientPtr client)
{
    int rc;
    WindowPtr pWin = NULL;
    DeviceIntPtr pDev = NULL;
    CursorPtr pCursor = NULL;

    REQUEST(xXIChangeCursorReq);
    REQUEST_SIZE_MATCH(xXIChangeCursorReq);

    rc = dixLookupDevice(&pDev, stuff->deviceid, client, DixSetAttrAccess);
    if (rc != Success)
        return rc;

    if (!IsMaster(pDev) || !IsPointerDevice(pDev))
        return BadDevice;

    if (stuff->win != None) {
        rc = dixLookupWindow(&pWin, stuff->win, client, DixSetAttrAccess);
        if (rc != Success)
            return rc;
    }

    if (stuff->cursor == None) {
        if (pWin == pWin->drawable.pScreen->root)
            pCursor = rootCursor;
        else
            pCursor = (CursorPtr) None;
    }
    else {
        rc = dixLookupResourceByType((void **) &pCursor, stuff->cursor,
                                     RT_CURSOR, client, DixUseAccess);
        if (rc != Success)
            return rc;
    }

    ChangeWindowDeviceCursor(pWin, pDev, pCursor);

    return Success;
}
