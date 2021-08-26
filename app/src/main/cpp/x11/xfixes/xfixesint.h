#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _XFIXESINT_H_
#define _XFIXESINT_H_

#include "../headers/x.h"
#include "../headers/xproto.h"
#include "../headers/misc.h"
#include "../headers/os.h"
#include "../headers/dixstruct.h"
#include "../headers/extnsionst.h"
#include "../headers/extensions/xfixesproto.h"
#include "../headers/windowstr.h"
#include "../headers/selection.h"
#include "xfixes.h"

#define PANORAMIX

extern int XFixesEventBase;

typedef struct _XFixesClient {
    CARD32 major_version;
    CARD32 minor_version;
} XFixesClientRec, *XFixesClientPtr;

#define GetXFixesClient(pClient) ((XFixesClientPtr)dixLookupPrivate(&(pClient)->devPrivates, XFixesClientPrivateKey))

extern int (*ProcXFixesVector[XFixesNumberRequests]) (ClientPtr);

/* Save set */
int
ProcXFixesChangeSaveSet(ClientPtr client);

int
SProcXFixesChangeSaveSet(ClientPtr client);

/* Selection events */
int
ProcXFixesSelectSelectionInput(ClientPtr client);

int
SProcXFixesSelectSelectionInput(ClientPtr client);

void

SXFixesSelectionNotifyEvent(xXFixesSelectionNotifyEvent * from,
                            xXFixesSelectionNotifyEvent * to);
Bool
XFixesSelectionInit(void);

/* Cursor notification */
Bool
XFixesCursorInit(void);

int
ProcXFixesSelectCursorInput(ClientPtr client);

int
SProcXFixesSelectCursorInput(ClientPtr client);

void

SXFixesCursorNotifyEvent(xXFixesCursorNotifyEvent * from,
                         xXFixesCursorNotifyEvent * to);

int
ProcXFixesGetCursorImage(ClientPtr client);

int
SProcXFixesGetCursorImage(ClientPtr client);

/* Cursor names (Version 2) */

int
ProcXFixesSetCursorName(ClientPtr client);

int
SProcXFixesSetCursorName(ClientPtr client);

int
ProcXFixesGetCursorName(ClientPtr client);

int
SProcXFixesGetCursorName(ClientPtr client);

int
ProcXFixesGetCursorImageAndName(ClientPtr client);

int
SProcXFixesGetCursorImageAndName(ClientPtr client);

/* Cursor replacement (Version 2) */

int
ProcXFixesChangeCursor(ClientPtr client);

int
SProcXFixesChangeCursor(ClientPtr client);

int
ProcXFixesChangeCursorByName(ClientPtr client);

int
SProcXFixesChangeCursorByName(ClientPtr client);

/* Region objects (Version 2* */
Bool
XFixesRegionInit(void);

int
ProcXFixesCreateRegion(ClientPtr client);

int
SProcXFixesCreateRegion(ClientPtr client);

int
ProcXFixesCreateRegionFromBitmap(ClientPtr client);

int
SProcXFixesCreateRegionFromBitmap(ClientPtr client);

int
ProcXFixesCreateRegionFromWindow(ClientPtr client);

int
SProcXFixesCreateRegionFromWindow(ClientPtr client);

int
ProcXFixesCreateRegionFromGC(ClientPtr client);

int
SProcXFixesCreateRegionFromGC(ClientPtr client);

int
ProcXFixesCreateRegionFromPicture(ClientPtr client);

int
SProcXFixesCreateRegionFromPicture(ClientPtr client);

int
ProcXFixesDestroyRegion(ClientPtr client);

int
SProcXFixesDestroyRegion(ClientPtr client);

int
ProcXFixesSetRegion(ClientPtr client);

int
SProcXFixesSetRegion(ClientPtr client);

int
ProcXFixesCopyRegion(ClientPtr client);

int
SProcXFixesCopyRegion(ClientPtr client);

int
ProcXFixesCombineRegion(ClientPtr client);

int
SProcXFixesCombineRegion(ClientPtr client);

int
ProcXFixesInvertRegion(ClientPtr client);

int
SProcXFixesInvertRegion(ClientPtr client);

int
ProcXFixesTranslateRegion(ClientPtr client);

int
SProcXFixesTranslateRegion(ClientPtr client);

int
ProcXFixesRegionExtents(ClientPtr client);

int
SProcXFixesRegionExtents(ClientPtr client);

int
ProcXFixesFetchRegion(ClientPtr client);

int
SProcXFixesFetchRegion(ClientPtr client);

int
ProcXFixesSetGCClipRegion(ClientPtr client);

int
SProcXFixesSetGCClipRegion(ClientPtr client);

int
ProcXFixesSetWindowShapeRegion(ClientPtr client);

int
SProcXFixesSetWindowShapeRegion(ClientPtr client);

int
ProcXFixesSetPictureClipRegion(ClientPtr client);

int
SProcXFixesSetPictureClipRegion(ClientPtr client);

int
ProcXFixesExpandRegion(ClientPtr client);

int
SProcXFixesExpandRegion(ClientPtr client);

int
PanoramiXFixesSetGCClipRegion(ClientPtr client);

int
PanoramiXFixesSetWindowShapeRegion(ClientPtr client);

int
PanoramiXFixesSetPictureClipRegion(ClientPtr client);

/* Cursor Visibility (Version 4) */

int
ProcXFixesHideCursor(ClientPtr client);

int
SProcXFixesHideCursor(ClientPtr client);

int
ProcXFixesShowCursor(ClientPtr client);

int
SProcXFixesShowCursor(ClientPtr client);

/* Version 5 */

int
ProcXFixesCreatePointerBarrier(ClientPtr client);

int
SProcXFixesCreatePointerBarrier(ClientPtr client);

int
ProcXFixesDestroyPointerBarrier(ClientPtr client);

int
SProcXFixesDestroyPointerBarrier(ClientPtr client);

/* Version 6 */

Bool
XFixesClientDisconnectInit(void);

int
ProcXFixesSetClientDisconnectMode(ClientPtr client);

int
ProcXFixesGetClientDisconnectMode(ClientPtr client);

int
SProcXFixesSetClientDisconnectMode(ClientPtr client);

int
SProcXFixesGetClientDisconnectMode(ClientPtr client);

Bool
XFixesShouldDisconnectClient(ClientPtr client);

/* Xinerama */
#ifdef PANORAMIX
extern int (*PanoramiXSaveXFixesVector[XFixesNumberRequests]) (ClientPtr);
void PanoramiXFixesInit(void);
void PanoramiXFixesReset(void);
#endif

#endif                          /* _XFIXESINT_H_ */
