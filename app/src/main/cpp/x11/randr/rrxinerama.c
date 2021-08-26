#include "randrstr.h"
#include "../headers/swaprep.h"
#include "../headers/extensions/panoramiXproto.h"
#include "../headers/protocol-versions.h"

/* Xinerama is not multi-screen capable; just report about screen 0 */
#define RR_XINERAMA_SCREEN  0

static int ProcRRXineramaQueryVersion(ClientPtr client);
static int ProcRRXineramaGetState(ClientPtr client);
static int ProcRRXineramaGetScreenCount(ClientPtr client);
static int ProcRRXineramaGetScreenSize(ClientPtr client);
static int ProcRRXineramaIsActive(ClientPtr client);
static int ProcRRXineramaQueryScreens(ClientPtr client);
static int _X_COLD SProcRRXineramaDispatch(ClientPtr client);

Bool noRRXineramaExtension = FALSE;

/* Proc */

int
ProcRRXineramaQueryVersion(ClientPtr client)
{
    xPanoramiXQueryVersionReply rep = {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = 0,
            .majorVersion = SERVER_RRXINERAMA_MAJOR_VERSION,
            .minorVersion = SERVER_RRXINERAMA_MINOR_VERSION
    };

    REQUEST_SIZE_MATCH(xPanoramiXQueryVersionReq);
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swaps(&rep.majorVersion);
        swaps(&rep.minorVersion);
    }
    WriteToClient(client, sizeof(xPanoramiXQueryVersionReply), &rep);
    return Success;
}

int
ProcRRXineramaGetState(ClientPtr client)
{
    REQUEST(xPanoramiXGetStateReq);
    WindowPtr pWin;
    xPanoramiXGetStateReply rep;
    register int rc;
    ScreenPtr pScreen;
    rrScrPrivPtr pScrPriv;
    Bool active = FALSE;

    REQUEST_SIZE_MATCH(xPanoramiXGetStateReq);
    rc = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    pScreen = pWin->drawable.pScreen;
    pScrPriv = rrGetScrPriv(pScreen);
    if (pScrPriv) {
        /* XXX do we need more than this? */
        active = TRUE;
    }

    rep = (xPanoramiXGetStateReply) {
            .type = X_Reply,
            .state = active,
            .sequenceNumber = client->sequence,
            .length = 0,
            .window = stuff->window
    };
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.window);
    }
    WriteToClient(client, sizeof(xPanoramiXGetStateReply), &rep);
    return Success;
}

static int
RRXineramaScreenCount(ScreenPtr pScreen)
{
    return RRMonitorCountList(pScreen);
}

static Bool
RRXineramaScreenActive(ScreenPtr pScreen)
{
    return RRXineramaScreenCount(pScreen) > 0;
}

int
ProcRRXineramaGetScreenCount(ClientPtr client)
{
    REQUEST(xPanoramiXGetScreenCountReq);
    WindowPtr pWin;
    xPanoramiXGetScreenCountReply rep;
    register int rc;

    REQUEST_SIZE_MATCH(xPanoramiXGetScreenCountReq);
    rc = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    rep = (xPanoramiXGetScreenCountReply)  {
            .type = X_Reply,
            .ScreenCount = RRXineramaScreenCount(pWin->drawable.pScreen),
            .sequenceNumber = client->sequence,
            .length = 0,
            .window = stuff->window
    };
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.window);
    }
    WriteToClient(client, sizeof(xPanoramiXGetScreenCountReply), &rep);
    return Success;
}

int
ProcRRXineramaGetScreenSize(ClientPtr client)
{
    REQUEST(xPanoramiXGetScreenSizeReq);
    WindowPtr pWin, pRoot;
    ScreenPtr pScreen;
    xPanoramiXGetScreenSizeReply rep;
    register int rc;

    REQUEST_SIZE_MATCH(xPanoramiXGetScreenSizeReq);
    rc = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    pScreen = pWin->drawable.pScreen;
    pRoot = pScreen->root;

    rep = (xPanoramiXGetScreenSizeReply) {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = 0,
            .width = pRoot->drawable.width,
            .height = pRoot->drawable.height,
            .window = stuff->window,
            .screen = stuff->screen
    };
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.width);
        swapl(&rep.height);
        swapl(&rep.window);
        swapl(&rep.screen);
    }
    WriteToClient(client, sizeof(xPanoramiXGetScreenSizeReply), &rep);
    return Success;
}

int
ProcRRXineramaIsActive(ClientPtr client)
{
    xXineramaIsActiveReply rep;

    REQUEST_SIZE_MATCH(xXineramaIsActiveReq);

    rep = (xXineramaIsActiveReply) {
            .type = X_Reply,
            .length = 0,
            .sequenceNumber = client->sequence,
            .state = RRXineramaScreenActive(screenInfo.screens[RR_XINERAMA_SCREEN])
    };
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.state);
    }
    WriteToClient(client, sizeof(xXineramaIsActiveReply), &rep);
    return Success;
}

static void
RRXineramaWriteMonitor(ClientPtr client, RRMonitorPtr monitor)
{
    xXineramaScreenInfo scratch;

    scratch.x_org = monitor->geometry.box.x1;
    scratch.y_org = monitor->geometry.box.y1;
    scratch.width = monitor->geometry.box.x2 - monitor->geometry.box.x1;
    scratch.height = monitor->geometry.box.y2 - monitor->geometry.box.y1;

    if (client->swapped) {
        swaps(&scratch.x_org);
        swaps(&scratch.y_org);
        swaps(&scratch.width);
        swaps(&scratch.height);
    }

    WriteToClient(client, sz_XineramaScreenInfo, &scratch);
}

int
ProcRRXineramaQueryScreens(ClientPtr client)
{
    xXineramaQueryScreensReply rep;
    ScreenPtr pScreen = screenInfo.screens[RR_XINERAMA_SCREEN];
    int m;
    RRMonitorPtr monitors = NULL;
    int nmonitors = 0;

    REQUEST_SIZE_MATCH(xXineramaQueryScreensReq);

    if (RRXineramaScreenActive(pScreen)) {
        RRGetInfo(pScreen, FALSE);
        if (!RRMonitorMakeList(pScreen, TRUE, &monitors, &nmonitors))
            return BadAlloc;
    }

    rep = (xXineramaQueryScreensReply) {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = bytes_to_int32(nmonitors * sz_XineramaScreenInfo),
            .number = nmonitors
    };
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.number);
    }
    WriteToClient(client, sizeof(xXineramaQueryScreensReply), &rep);

    for (m = 0; m < nmonitors; m++)
        RRXineramaWriteMonitor(client, &monitors[m]);

    if (monitors)
        RRMonitorFreeList(monitors, nmonitors);

    return Success;
}

static int
ProcRRXineramaDispatch(ClientPtr client)
{
    REQUEST(xReq);
    switch (stuff->data) {
        case X_PanoramiXQueryVersion:
            return ProcRRXineramaQueryVersion(client);
        case X_PanoramiXGetState:
            return ProcRRXineramaGetState(client);
        case X_PanoramiXGetScreenCount:
            return ProcRRXineramaGetScreenCount(client);
        case X_PanoramiXGetScreenSize:
            return ProcRRXineramaGetScreenSize(client);
        case X_XineramaIsActive:
            return ProcRRXineramaIsActive(client);
        case X_XineramaQueryScreens:
            return ProcRRXineramaQueryScreens(client);
    }
    return BadRequest;
}

/* SProc */

static int _X_COLD
SProcRRXineramaQueryVersion(ClientPtr client)
{
    REQUEST(xPanoramiXQueryVersionReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xPanoramiXQueryVersionReq);
    return ProcRRXineramaQueryVersion(client);
}

static int _X_COLD
SProcRRXineramaGetState(ClientPtr client)
{
    REQUEST(xPanoramiXGetStateReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xPanoramiXGetStateReq);
    swapl(&stuff->window);
    return ProcRRXineramaGetState(client);
}

static int _X_COLD
SProcRRXineramaGetScreenCount(ClientPtr client)
{
    REQUEST(xPanoramiXGetScreenCountReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xPanoramiXGetScreenCountReq);
    swapl(&stuff->window);
    return ProcRRXineramaGetScreenCount(client);
}

static int _X_COLD
SProcRRXineramaGetScreenSize(ClientPtr client)
{
    REQUEST(xPanoramiXGetScreenSizeReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xPanoramiXGetScreenSizeReq);
    swapl(&stuff->window);
    swapl(&stuff->screen);
    return ProcRRXineramaGetScreenSize(client);
}

static int _X_COLD
SProcRRXineramaIsActive(ClientPtr client)
{
    REQUEST(xXineramaIsActiveReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xXineramaIsActiveReq);
    return ProcRRXineramaIsActive(client);
}

static int _X_COLD
SProcRRXineramaQueryScreens(ClientPtr client)
{
    REQUEST(xXineramaQueryScreensReq);
    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xXineramaQueryScreensReq);
    return ProcRRXineramaQueryScreens(client);
}

int
SProcRRXineramaDispatch(ClientPtr client)
{
    REQUEST(xReq);
    switch (stuff->data) {
        case X_PanoramiXQueryVersion:
            return SProcRRXineramaQueryVersion(client);
        case X_PanoramiXGetState:
            return SProcRRXineramaGetState(client);
        case X_PanoramiXGetScreenCount:
            return SProcRRXineramaGetScreenCount(client);
        case X_PanoramiXGetScreenSize:
            return SProcRRXineramaGetScreenSize(client);
        case X_XineramaIsActive:
            return SProcRRXineramaIsActive(client);
        case X_XineramaQueryScreens:
            return SProcRRXineramaQueryScreens(client);
    }
    return BadRequest;
}

void
RRXineramaExtensionInit(void)
{
#ifdef PANORAMIX
    if (!noPanoramiXExtension)
        return;
#endif

    if (noRRXineramaExtension)
        return;

    /*
     * Xinerama isn't capable enough to have multiple protocol screens each
     * with their own output geometry.  So if there's more than one protocol
     * screen, just don't even try.
     */
    if (screenInfo.numScreens > 1)
        return;

    (void) AddExtension(PANORAMIX_PROTOCOL_NAME, 0, 0,
                        ProcRRXineramaDispatch,
                        SProcRRXineramaDispatch, NULL, StandardMinorOpcode);
}
