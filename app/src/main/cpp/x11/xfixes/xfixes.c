#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "xfixesint.h"
#include "../headers/protocol-versions.h"
#include "../headers/extinit.h"

#define PANORAMIX

static unsigned char XFixesReqCode;
int XFixesEventBase;
int XFixesErrorBase;

static DevPrivateKeyRec XFixesClientPrivateKeyRec;

#define XFixesClientPrivateKey (&XFixesClientPrivateKeyRec)

static int
ProcXFixesQueryVersion(ClientPtr client)
{
    XFixesClientPtr pXFixesClient = GetXFixesClient(client);
    xXFixesQueryVersionReply rep = {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = 0
    };

    REQUEST(xXFixesQueryVersionReq);

    REQUEST_SIZE_MATCH(xXFixesQueryVersionReq);

    if (version_compare(stuff->majorVersion, stuff->minorVersion,
                        SERVER_XFIXES_MAJOR_VERSION,
                        SERVER_XFIXES_MINOR_VERSION) < 0) {
        rep.majorVersion = stuff->majorVersion;
        rep.minorVersion = stuff->minorVersion;
    }
    else {
        rep.majorVersion = SERVER_XFIXES_MAJOR_VERSION;
        rep.minorVersion = SERVER_XFIXES_MINOR_VERSION;
    }

    pXFixesClient->major_version = rep.majorVersion;
    pXFixesClient->minor_version = rep.minorVersion;
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.majorVersion);
        swapl(&rep.minorVersion);
    }
    WriteToClient(client, sizeof(xXFixesQueryVersionReply), &rep);
    return Success;
}

/* Major version controls available requests */
static const int version_requests[] = {
        X_XFixesQueryVersion,       /* before client sends QueryVersion */
        X_XFixesGetCursorImage,     /* Version 1 */
        X_XFixesChangeCursorByName, /* Version 2 */
        X_XFixesExpandRegion,       /* Version 3 */
        X_XFixesShowCursor,         /* Version 4 */
        X_XFixesDestroyPointerBarrier,      /* Version 5 */
        X_XFixesGetClientDisconnectMode,    /* Version 6 */
};

int (*ProcXFixesVector[XFixesNumberRequests]) (ClientPtr) = {
/*************** Version 1 ******************/
        ProcXFixesQueryVersion,
        ProcXFixesChangeSaveSet,
        ProcXFixesSelectSelectionInput,
        ProcXFixesSelectCursorInput, ProcXFixesGetCursorImage,
/*************** Version 2 ******************/
        ProcXFixesCreateRegion,
        ProcXFixesCreateRegionFromBitmap,
        ProcXFixesCreateRegionFromWindow,
        ProcXFixesCreateRegionFromGC,
        ProcXFixesCreateRegionFromPicture,
        ProcXFixesDestroyRegion,
        ProcXFixesSetRegion,
        ProcXFixesCopyRegion,
        ProcXFixesCombineRegion,
        ProcXFixesCombineRegion,
        ProcXFixesCombineRegion,
        ProcXFixesInvertRegion,
        ProcXFixesTranslateRegion,
        ProcXFixesRegionExtents,
        ProcXFixesFetchRegion,
        ProcXFixesSetGCClipRegion,
        ProcXFixesSetWindowShapeRegion,
        ProcXFixesSetPictureClipRegion,
        ProcXFixesSetCursorName,
        ProcXFixesGetCursorName,
        ProcXFixesGetCursorImageAndName,
        ProcXFixesChangeCursor, ProcXFixesChangeCursorByName,
/*************** Version 3 ******************/
        ProcXFixesExpandRegion,
/*************** Version 4 ****************/
        ProcXFixesHideCursor, ProcXFixesShowCursor,
/*************** Version 5 ****************/
        ProcXFixesCreatePointerBarrier, ProcXFixesDestroyPointerBarrier,
/*************** Version 6 ****************/
        ProcXFixesSetClientDisconnectMode, ProcXFixesGetClientDisconnectMode,
};

static int
ProcXFixesDispatch(ClientPtr client)
{
    REQUEST(xXFixesReq);
    XFixesClientPtr pXFixesClient = GetXFixesClient(client);

    if (pXFixesClient->major_version >= ARRAY_SIZE(version_requests))
        return BadRequest;
    if (stuff->xfixesReqType > version_requests[pXFixesClient->major_version])
        return BadRequest;
    return (*ProcXFixesVector[stuff->xfixesReqType]) (client);
}

static _X_COLD int
SProcXFixesQueryVersion(ClientPtr client)
{
    REQUEST(xXFixesQueryVersionReq);
    REQUEST_SIZE_MATCH(xXFixesQueryVersionReq);

    swaps(&stuff->length);
    swapl(&stuff->majorVersion);
    swapl(&stuff->minorVersion);
    return (*ProcXFixesVector[stuff->xfixesReqType]) (client);
}

static int (*SProcXFixesVector[XFixesNumberRequests]) (ClientPtr) = {
/*************** Version 1 ******************/
        SProcXFixesQueryVersion,
        SProcXFixesChangeSaveSet,
        SProcXFixesSelectSelectionInput,
        SProcXFixesSelectCursorInput, SProcXFixesGetCursorImage,
/*************** Version 2 ******************/
        SProcXFixesCreateRegion,
        SProcXFixesCreateRegionFromBitmap,
        SProcXFixesCreateRegionFromWindow,
        SProcXFixesCreateRegionFromGC,
        SProcXFixesCreateRegionFromPicture,
        SProcXFixesDestroyRegion,
        SProcXFixesSetRegion,
        SProcXFixesCopyRegion,
        SProcXFixesCombineRegion,
        SProcXFixesCombineRegion,
        SProcXFixesCombineRegion,
        SProcXFixesInvertRegion,
        SProcXFixesTranslateRegion,
        SProcXFixesRegionExtents,
        SProcXFixesFetchRegion,
        SProcXFixesSetGCClipRegion,
        SProcXFixesSetWindowShapeRegion,
        SProcXFixesSetPictureClipRegion,
        SProcXFixesSetCursorName,
        SProcXFixesGetCursorName,
        SProcXFixesGetCursorImageAndName,
        SProcXFixesChangeCursor, SProcXFixesChangeCursorByName,
/*************** Version 3 ******************/
        SProcXFixesExpandRegion,
/*************** Version 4 ****************/
        SProcXFixesHideCursor, SProcXFixesShowCursor,
/*************** Version 5 ****************/
        SProcXFixesCreatePointerBarrier, SProcXFixesDestroyPointerBarrier,
/*************** Version 6 ****************/
        SProcXFixesSetClientDisconnectMode, SProcXFixesGetClientDisconnectMode,
};

static _X_COLD int
SProcXFixesDispatch(ClientPtr client)
{
    REQUEST(xXFixesReq);
    if (stuff->xfixesReqType >= XFixesNumberRequests)
        return BadRequest;
    return (*SProcXFixesVector[stuff->xfixesReqType]) (client);
}

void
XFixesExtensionInit(void)
{
    ExtensionEntry *extEntry;

    if (!dixRegisterPrivateKey
            (&XFixesClientPrivateKeyRec, PRIVATE_CLIENT, sizeof(XFixesClientRec)))
        return;

    if (XFixesSelectionInit() &&
        XFixesCursorInit() &&
        XFixesRegionInit() &&
        XFixesClientDisconnectInit() &&
        (extEntry = AddExtension(XFIXES_NAME, XFixesNumberEvents,
                                 XFixesNumberErrors,
                                 ProcXFixesDispatch, SProcXFixesDispatch,
                                 NULL, StandardMinorOpcode)) != 0) {
        XFixesReqCode = (unsigned char) extEntry->base;
        XFixesEventBase = extEntry->eventBase;
        XFixesErrorBase = extEntry->errorBase;
        EventSwapVector[XFixesEventBase + XFixesSelectionNotify] =
                (EventSwapPtr) SXFixesSelectionNotifyEvent;
        EventSwapVector[XFixesEventBase + XFixesCursorNotify] =
                (EventSwapPtr) SXFixesCursorNotifyEvent;
        SetResourceTypeErrorValue(RegionResType, XFixesErrorBase + BadRegion);
        SetResourceTypeErrorValue(PointerBarrierType,
                                  XFixesErrorBase + BadBarrier);
    }
}

#ifdef PANORAMIX

int (*PanoramiXSaveXFixesVector[XFixesNumberRequests]) (ClientPtr);

void
PanoramiXFixesInit(void)
{
    int i;

    for (i = 0; i < XFixesNumberRequests; i++)
        PanoramiXSaveXFixesVector[i] = ProcXFixesVector[i];
    /*
     * Stuff in Xinerama aware request processing hooks
     */
    ProcXFixesVector[X_XFixesSetGCClipRegion] = PanoramiXFixesSetGCClipRegion;
    ProcXFixesVector[X_XFixesSetWindowShapeRegion] =
        PanoramiXFixesSetWindowShapeRegion;
    ProcXFixesVector[X_XFixesSetPictureClipRegion] =
        PanoramiXFixesSetPictureClipRegion;
}

void
PanoramiXFixesReset(void)
{
    int i;

    for (i = 0; i < XFixesNumberRequests; i++)
        ProcXFixesVector[i] = PanoramiXSaveXFixesVector[i];
}

#endif
