#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stdio.h>
#include "../headers/x.h"
#include "../headers/xproto.h"
#include "../headers/misc.h"
#include "../headers/cursor.h"
#include "../headers/cursorstr.h"
#include "../headers/extnsionst.h"
#include "../headers/dixstruct.h"
#include "../headers/gc.h"
#include "../headers/gcstruct.h"
#include "../headers/scrnintstr.h"
#include "../headers/window.h"
#include "../headers/windowstr.h"
#include "../headers/pixmapstr.h"
#include "panoramiX.h"
#include "../headers/extensions/panoramiXproto.h"
#include "panoramiXsrv.h"
#include "../headers/globals.h"
#include "panoramiXh.h"

static int _X_COLD
SProcPanoramiXQueryVersion(ClientPtr client)
{
    REQUEST(xPanoramiXQueryVersionReq);

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xPanoramiXQueryVersionReq);
    return ProcPanoramiXQueryVersion(client);
}

static int _X_COLD
SProcPanoramiXGetState(ClientPtr client)
{
    REQUEST(xPanoramiXGetStateReq);

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xPanoramiXGetStateReq);
    swapl(&stuff->window);
    return ProcPanoramiXGetState(client);
}

static int _X_COLD
SProcPanoramiXGetScreenCount(ClientPtr client)
{
    REQUEST(xPanoramiXGetScreenCountReq);

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xPanoramiXGetScreenCountReq);
    swapl(&stuff->window);
    return ProcPanoramiXGetScreenCount(client);
}

static int _X_COLD
SProcPanoramiXGetScreenSize(ClientPtr client)
{
    REQUEST(xPanoramiXGetScreenSizeReq);

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xPanoramiXGetScreenSizeReq);
    swapl(&stuff->window);
    swapl(&stuff->screen);
    return ProcPanoramiXGetScreenSize(client);
}

static int _X_COLD
SProcXineramaIsActive(ClientPtr client)
{
    REQUEST(xXineramaIsActiveReq);

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xXineramaIsActiveReq);
    return ProcXineramaIsActive(client);
}

static int _X_COLD
SProcXineramaQueryScreens(ClientPtr client)
{
    REQUEST(xXineramaQueryScreensReq);

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xXineramaQueryScreensReq);
    return ProcXineramaQueryScreens(client);
}

int _X_COLD
SProcPanoramiXDispatch(ClientPtr client)
{
    REQUEST(xReq);
    switch (stuff->data) {
        case X_PanoramiXQueryVersion:
            return SProcPanoramiXQueryVersion(client);
        case X_PanoramiXGetState:
            return SProcPanoramiXGetState(client);
        case X_PanoramiXGetScreenCount:
            return SProcPanoramiXGetScreenCount(client);
        case X_PanoramiXGetScreenSize:
            return SProcPanoramiXGetScreenSize(client);
        case X_XineramaIsActive:
            return SProcXineramaIsActive(client);
        case X_XineramaQueryScreens:
            return SProcXineramaQueryScreens(client);
    }
    return BadRequest;
}
