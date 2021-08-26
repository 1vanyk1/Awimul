#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/XIstubs.h"
#include "../headers/globals.h"

#include "../headers/exevents.h"
#include "../headers/exglobals.h"

#include "chgkbd.h"
#include "chgptr.h"

/***********************************************************************
 *
 * This procedure changes the keyboard device.
 *
 */

int _X_COLD
SProcXChangeKeyboardDevice(ClientPtr client)
{
REQUEST(xChangeKeyboardDeviceReq);
swaps(&stuff->length);
REQUEST_SIZE_MATCH(xChangeKeyboardDeviceReq);
return (ProcXChangeKeyboardDevice(client));
}

/***********************************************************************
 *
 * This procedure is invoked to swap the request bytes if the server and
 * client have a different byte order.
 *
 */

int
ProcXChangeKeyboardDevice(ClientPtr client)
{
    /* REQUEST(xChangeKeyboardDeviceReq); */
    REQUEST_SIZE_MATCH(xChangeKeyboardDeviceReq);

    return BadDevice;
}
