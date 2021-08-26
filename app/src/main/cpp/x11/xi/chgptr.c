#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/XIstubs.h"
#include "../headers/windowstr.h"          /* window structure  */
#include "../headers/scrnintstr.h"         /* screen structure  */

#include "../headers/dixevents.h"
#include "../headers/exevents.h"
#include "../headers/exglobals.h"

#include "chgptr.h"

/***********************************************************************
 *
 * This procedure is invoked to swap the request bytes if the server and
 * client have a different byte order.
 *
 */

int _X_COLD
SProcXChangePointerDevice(ClientPtr client)
{
    REQUEST(xChangePointerDeviceReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xChangePointerDeviceReq);
    return (ProcXChangePointerDevice(client));
}

/***********************************************************************
 *
 * This procedure changes the device used as the X pointer.
 *
 */

int
ProcXChangePointerDevice(ClientPtr client)
{
    /* REQUEST(xChangePointerDeviceReq); */
    REQUEST_SIZE_MATCH(xChangePointerDeviceReq);

    return BadDevice;
}
