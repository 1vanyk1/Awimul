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

#include "xisetclientpointer.h"

int _X_COLD
SProcXISetClientPointer(ClientPtr client)
{
REQUEST(xXISetClientPointerReq);
REQUEST_SIZE_MATCH(xXISetClientPointerReq);

swaps(&stuff->length);
swapl(&stuff->win);
swaps(&stuff->deviceid);
return (ProcXISetClientPointer(client));
}

int
ProcXISetClientPointer(ClientPtr client)
{
    DeviceIntPtr pDev;
    ClientPtr targetClient;
    int rc;

    REQUEST(xXISetClientPointerReq);
    REQUEST_SIZE_MATCH(xXISetClientPointerReq);

    rc = dixLookupDevice(&pDev, stuff->deviceid, client, DixManageAccess);
    if (rc != Success) {
        client->errorValue = stuff->deviceid;
        return rc;
    }

    if (!IsMaster(pDev)) {
        client->errorValue = stuff->deviceid;
        return BadDevice;
    }

    pDev = GetMaster(pDev, MASTER_POINTER);

    if (stuff->win != None) {
        rc = dixLookupClient(&targetClient, stuff->win, client,
                             DixManageAccess);

        if (rc != Success)
            return BadWindow;

    }
    else
        targetClient = client;

    rc = SetClientPointer(targetClient, pDev);
    if (rc != Success) {
        client->errorValue = stuff->deviceid;
        return rc;
    }

    return Success;
}
