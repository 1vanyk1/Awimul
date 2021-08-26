#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exglobals.h"

#include "getmmap.h"

/***********************************************************************
 *
 * This procedure gets the modifier mapping for an extension device,
 * for clients on machines with a different byte ordering than the server.
 *
 */

int _X_COLD
SProcXGetDeviceModifierMapping(ClientPtr client)
{
REQUEST(xGetDeviceModifierMappingReq);
swaps(&stuff->length);
return (ProcXGetDeviceModifierMapping(client));
}

/***********************************************************************
 *
 * Get the device Modifier mapping.
 *
 */

int
ProcXGetDeviceModifierMapping(ClientPtr client)
{
    DeviceIntPtr dev;
    xGetDeviceModifierMappingReply rep;
    KeyCode *modkeymap = NULL;
    int ret, max_keys_per_mod;

    REQUEST(xGetDeviceModifierMappingReq);
    REQUEST_SIZE_MATCH(xGetDeviceModifierMappingReq);

    ret = dixLookupDevice(&dev, stuff->deviceid, client, DixGetAttrAccess);
    if (ret != Success)
        return ret;

    ret = generate_modkeymap(client, dev, &modkeymap, &max_keys_per_mod);
    if (ret != Success)
        return ret;

    rep = (xGetDeviceModifierMappingReply) {
            .repType = X_Reply,
            .RepType = X_GetDeviceModifierMapping,
            .sequenceNumber = client->sequence,
            .numKeyPerModifier = max_keys_per_mod,
            /* length counts 4 byte quantities - there are 8 modifiers 1 byte big */
            .length = max_keys_per_mod << 1
    };

    WriteReplyToClient(client, sizeof(xGetDeviceModifierMappingReply), &rep);
    WriteToClient(client, max_keys_per_mod * 8, modkeymap);

    free(modkeymap);

    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XGetDeviceModifierMapping function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXGetDeviceModifierMapping(ClientPtr client, int size,
        xGetDeviceModifierMappingReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
WriteToClient(client, size, rep);
}
