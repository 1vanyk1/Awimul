#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XI2.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exevents.h"
#include "../headers/exglobals.h"

#include "setmmap.h"

/***********************************************************************
 *
 * This procedure sets the modifier mapping for an extension device,
 * for clients on machines with a different byte ordering than the server.
 *
 */

int _X_COLD
SProcXSetDeviceModifierMapping(ClientPtr client)
{
REQUEST(xSetDeviceModifierMappingReq);
swaps(&stuff->length);
return (ProcXSetDeviceModifierMapping(client));
}

/***********************************************************************
 *
 * Set the device Modifier mapping.
 *
 */

int
ProcXSetDeviceModifierMapping(ClientPtr client)
{
    int ret;
    xSetDeviceModifierMappingReply rep;
    DeviceIntPtr dev;

    REQUEST(xSetDeviceModifierMappingReq);
    REQUEST_AT_LEAST_SIZE(xSetDeviceModifierMappingReq);

    if (stuff->length != bytes_to_int32(sizeof(xSetDeviceModifierMappingReq)) +
                         (stuff->numKeyPerModifier << 1))
        return BadLength;

    rep = (xSetDeviceModifierMappingReply) {
            .repType = X_Reply,
            .RepType = X_SetDeviceModifierMapping,
            .sequenceNumber = client->sequence,
            .length = 0
    };

    ret = dixLookupDevice(&dev, stuff->deviceid, client, DixManageAccess);
    if (ret != Success)
        return ret;

    ret = change_modmap(client, dev, (KeyCode *) &stuff[1],
                        stuff->numKeyPerModifier);
    if (ret == Success)
        ret = MappingSuccess;

    if (ret == MappingSuccess || ret == MappingBusy || ret == MappingFailed) {
        rep.success = ret;
        WriteReplyToClient(client, sizeof(xSetDeviceModifierMappingReply),
                           &rep);
    }
    else if (ret == -1) {
        return BadValue;
    }
    else {
        return ret;
    }

    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XSetDeviceModifierMapping function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXSetDeviceModifierMapping(ClientPtr client, int size,
        xSetDeviceModifierMappingReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
WriteToClient(client, size, rep);
}
