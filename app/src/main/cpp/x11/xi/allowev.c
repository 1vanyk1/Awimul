#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"

#include "../headers/exglobals.h"

#include "allowev.h"
#include "../headers/dixevents.h"

/***********************************************************************
 *
 * This procedure allows frozen events to be routed.
 *
 */

int _X_COLD
SProcXAllowDeviceEvents(ClientPtr client)
{
REQUEST(xAllowDeviceEventsReq);
swaps(&stuff->length);
REQUEST_SIZE_MATCH(xAllowDeviceEventsReq);
swapl(&stuff->time);
return (ProcXAllowDeviceEvents(client));
}

/***********************************************************************
 *
 * This procedure allows frozen events to be routed.
 *
 */

int
ProcXAllowDeviceEvents(ClientPtr client)
{
    TimeStamp time;
    DeviceIntPtr thisdev;
    int rc;

    REQUEST(xAllowDeviceEventsReq);
    REQUEST_SIZE_MATCH(xAllowDeviceEventsReq);

    rc = dixLookupDevice(&thisdev, stuff->deviceid, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;
    time = ClientTimeToServerTime(stuff->time);

    switch (stuff->mode) {
        case ReplayThisDevice:
            AllowSome(client, time, thisdev, NOT_GRABBED);
            break;
        case SyncThisDevice:
            AllowSome(client, time, thisdev, FREEZE_NEXT_EVENT);
            break;
        case AsyncThisDevice:
            AllowSome(client, time, thisdev, THAWED);
            break;
        case AsyncOtherDevices:
            AllowSome(client, time, thisdev, THAW_OTHERS);
            break;
        case SyncAll:
            AllowSome(client, time, thisdev, FREEZE_BOTH_NEXT_EVENT);
            break;
        case AsyncAll:
            AllowSome(client, time, thisdev, THAWED_BOTH);
            break;
        default:
            client->errorValue = stuff->mode;
            return BadValue;
    }
    return Success;
}
