#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"          /* window structure  */
#include "../mi/mi.h"
#include "../headers/eventstr.h"
#include "../headers/extensions/XI2.h"
#include "../headers/extensions/XI2proto.h"

#include "../headers/exglobals.h"          /* BadDevice */
#include "../headers/exevents.h"
#include "xiallowev.h"

int _X_COLD
SProcXIAllowEvents(ClientPtr client)
{
REQUEST(xXIAllowEventsReq);
REQUEST_AT_LEAST_SIZE(xXIAllowEventsReq);

swaps(&stuff->length);
swaps(&stuff->deviceid);
swapl(&stuff->time);
if (stuff->length > 3) {
xXI2_2AllowEventsReq *req_xi22 = (xXI2_2AllowEventsReq *) stuff;

REQUEST_AT_LEAST_SIZE(xXI2_2AllowEventsReq);
swapl(&req_xi22->touchid);
swapl(&req_xi22->grab_window);
}

return ProcXIAllowEvents(client);
}

int
ProcXIAllowEvents(ClientPtr client)
{
    TimeStamp time;
    DeviceIntPtr dev;
    int ret = Success;
    XIClientPtr xi_client;
    Bool have_xi22 = FALSE;

    REQUEST(xXI2_2AllowEventsReq);

    xi_client = dixLookupPrivate(&client->devPrivates, XIClientPrivateKey);

    if (version_compare(xi_client->major_version,
                        xi_client->minor_version, 2, 2) >= 0) {
        REQUEST_AT_LEAST_SIZE(xXI2_2AllowEventsReq);
        have_xi22 = TRUE;
    }
    else {
        REQUEST_AT_LEAST_SIZE(xXIAllowEventsReq);
    }

    ret = dixLookupDevice(&dev, stuff->deviceid, client, DixGetAttrAccess);
    if (ret != Success)
        return ret;

    time = ClientTimeToServerTime(stuff->time);

    switch (stuff->mode) {
        case XIReplayDevice:
            AllowSome(client, time, dev, NOT_GRABBED);
            break;
        case XISyncDevice:
            AllowSome(client, time, dev, FREEZE_NEXT_EVENT);
            break;
        case XIAsyncDevice:
            AllowSome(client, time, dev, THAWED);
            break;
        case XIAsyncPairedDevice:
            if (IsMaster(dev))
                AllowSome(client, time, dev, THAW_OTHERS);
            break;
        case XISyncPair:
            if (IsMaster(dev))
                AllowSome(client, time, dev, FREEZE_BOTH_NEXT_EVENT);
            break;
        case XIAsyncPair:
            if (IsMaster(dev))
                AllowSome(client, time, dev, THAWED_BOTH);
            break;
        case XIRejectTouch:
        case XIAcceptTouch:
        {
            int rc;
            WindowPtr win;

            if (!have_xi22)
                return BadValue;

            rc = dixLookupWindow(&win, stuff->grab_window, client, DixReadAccess);
            if (rc != Success)
                return rc;

            ret = TouchAcceptReject(client, dev, stuff->mode, stuff->touchid,
                                    stuff->grab_window, &client->errorValue);
        }
            break;
        default:
            client->errorValue = stuff->mode;
            ret = BadValue;
    }

    return ret;
}
