#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _COMPINT_H_
#define _COMPINT_H_

#include "../headers/misc.h"
#include "../headers/scrnintstr.h"
#include "../headers/os.h"
#include "../headers/regionstr.h"
#include "../headers/validate.h"
#include "../headers/windowstr.h"
#include "../headers/input.h"
#include "../headers/resource.h"
#include "../headers/colormapst.h"
#include "../headers/cursorstr.h"
#include "../headers/dixstruct.h"
#include "../headers/gcstruct.h"
#include "../headers/servermd.h"
#include "../headers/dixevents.h"
#include "../headers/globals.h"
#include "../headers/picturestr.h"
#include "../headers/extnsionst.h"
#include "../headers/privates.h"
#include "../mi/mi.h"
#include "../miext/damage/damage.h"
#include "../damageext/damageextint.h"
#include "../xfixes/xfixes.h"
#include "../headers/extensions/compositeproto.h"
#include "compositeext.h"
#include <assert.h>

/*
 *  enable this for debugging

    #define COMPOSITE_DEBUG
 */

typedef struct _CompClientWindow {
    struct _CompClientWindow *next;
    XID id;
    int update;
} CompClientWindowRec, *CompClientWindowPtr;

typedef struct _CompWindow {
    RegionRec borderClip;
    DamagePtr damage;           /* for automatic update mode */
    Bool damageRegistered;
    Bool damaged;
    int update;
    CompClientWindowPtr clients;
    int oldx;
    int oldy;
    PixmapPtr pOldPixmap;
    int borderClipX, borderClipY;
} CompWindowRec, *CompWindowPtr;

#define COMP_ORIGIN_INVALID	    0x80000000

typedef struct _CompSubwindows {
    int update;
    CompClientWindowPtr clients;
} CompSubwindowsRec, *CompSubwindowsPtr;

#ifndef COMP_INCLUDE_RGB24_VISUAL
#define COMP_INCLUDE_RGB24_VISUAL 0
#endif

typedef struct _CompOverlayClientRec *CompOverlayClientPtr;

typedef struct _CompOverlayClientRec {
    CompOverlayClientPtr pNext;
    ClientPtr pClient;
    ScreenPtr pScreen;
    XID resource;
} CompOverlayClientRec;

typedef struct _CompImplicitRedirectException {
    XID parentVisual;
    XID winVisual;
} CompImplicitRedirectException;

typedef struct _CompScreen {
    PositionWindowProcPtr PositionWindow;
    CopyWindowProcPtr CopyWindow;
    CreateWindowProcPtr CreateWindow;
    DestroyWindowProcPtr DestroyWindow;
    RealizeWindowProcPtr RealizeWindow;
    UnrealizeWindowProcPtr UnrealizeWindow;
    ClipNotifyProcPtr ClipNotify;
    /*
     * Called from ConfigureWindow, these
     * three track changes to the offscreen storage
     * geometry
     */
    ConfigNotifyProcPtr ConfigNotify;
    MoveWindowProcPtr MoveWindow;
    ResizeWindowProcPtr ResizeWindow;
    ChangeBorderWidthProcPtr ChangeBorderWidth;
    /*
     * Reparenting has an effect on Subwindows redirect
     */
    ReparentWindowProcPtr ReparentWindow;

    /*
     * Colormaps for new visuals better not get installed
     */
    InstallColormapProcPtr InstallColormap;

    /*
     * Fake backing store via automatic redirection
     */
    ChangeWindowAttributesProcPtr ChangeWindowAttributes;

    Bool pendingScreenUpdate;

    CloseScreenProcPtr CloseScreen;
    int numAlternateVisuals;
    VisualID *alternateVisuals;
    int numImplicitRedirectExceptions;
    CompImplicitRedirectException *implicitRedirectExceptions;

    WindowPtr pOverlayWin;
    Window overlayWid;
    CompOverlayClientPtr pOverlayClients;

    SourceValidateProcPtr SourceValidate;
} CompScreenRec, *CompScreenPtr;

extern DevPrivateKeyRec CompScreenPrivateKeyRec;

#define CompScreenPrivateKey (&CompScreenPrivateKeyRec)

extern DevPrivateKeyRec CompWindowPrivateKeyRec;

#define CompWindowPrivateKey (&CompWindowPrivateKeyRec)

extern DevPrivateKeyRec CompSubwindowsPrivateKeyRec;

#define CompSubwindowsPrivateKey (&CompSubwindowsPrivateKeyRec)

#define GetCompScreen(s) ((CompScreenPtr) \
    dixLookupPrivate(&(s)->devPrivates, CompScreenPrivateKey))
#define GetCompWindow(w) ((CompWindowPtr) \
    dixLookupPrivate(&(w)->devPrivates, CompWindowPrivateKey))
#define GetCompSubwindows(w) ((CompSubwindowsPtr) \
    dixLookupPrivate(&(w)->devPrivates, CompSubwindowsPrivateKey))

extern RESTYPE CompositeClientSubwindowsType;
extern RESTYPE CompositeClientOverlayType;

/*
 * compalloc.c
 */

Bool
compRedirectWindow(ClientPtr pClient, WindowPtr pWin, int update);

void
compFreeClientWindow(WindowPtr pWin, XID id);

int
compUnredirectWindow(ClientPtr pClient, WindowPtr pWin, int update);

int
compRedirectSubwindows(ClientPtr pClient, WindowPtr pWin, int update);

void
compFreeClientSubwindows(WindowPtr pWin, XID id);

int
compUnredirectSubwindows(ClientPtr pClient, WindowPtr pWin, int update);

int
compRedirectOneSubwindow(WindowPtr pParent, WindowPtr pWin);

int
compUnredirectOneSubwindow(WindowPtr pParent, WindowPtr pWin);

Bool
compAllocPixmap(WindowPtr pWin);

void
compSetParentPixmap(WindowPtr pWin);

void
compRestoreWindow(WindowPtr pWin, PixmapPtr pPixmap);

Bool

compReallocPixmap(WindowPtr pWin, int x, int y,
                  unsigned int w, unsigned int h, int bw);

void compMarkAncestors(WindowPtr pWin);

/*
 * compinit.c
 */

Bool
compScreenInit(ScreenPtr pScreen);

/*
 * compoverlay.c
 */

void
compFreeOverlayClient(CompOverlayClientPtr pOcToDel);

CompOverlayClientPtr
compFindOverlayClient(ScreenPtr pScreen, ClientPtr pClient);

CompOverlayClientPtr
compCreateOverlayClient(ScreenPtr pScreen, ClientPtr pClient);

Bool
compCreateOverlayWindow(ScreenPtr pScreen);

void
compDestroyOverlayWindow(ScreenPtr pScreen);

/*
 * compwindow.c
 */

#ifdef COMPOSITE_DEBUG
void
 compCheckTree(ScreenPtr pScreen);
#else
#define compCheckTree(s)
#endif

void
compSetPixmap(WindowPtr pWin, PixmapPtr pPixmap, int bw);

Bool
compCheckRedirect(WindowPtr pWin);

Bool
compPositionWindow(WindowPtr pWin, int x, int y);

Bool
compRealizeWindow(WindowPtr pWin);

Bool
compUnrealizeWindow(WindowPtr pWin);

void
compClipNotify(WindowPtr pWin, int dx, int dy);

void
compMoveWindow(WindowPtr pWin, int x, int y, WindowPtr pSib, VTKind kind);

void

compResizeWindow(WindowPtr pWin, int x, int y,
                 unsigned int w, unsigned int h, WindowPtr pSib);

void
compChangeBorderWidth(WindowPtr pWin, unsigned int border_width);

void
compReparentWindow(WindowPtr pWin, WindowPtr pPriorParent);

Bool
compCreateWindow(WindowPtr pWin);

Bool
compDestroyWindow(WindowPtr pWin);

void
compSetRedirectBorderClip(WindowPtr pWin, RegionPtr pRegion);

RegionPtr
compGetRedirectBorderClip(WindowPtr pWin);

void
compCopyWindow(WindowPtr pWin, DDXPointRec ptOldOrg, RegionPtr prgnSrc);

void
compPaintChildrenToWindow(WindowPtr pWin);

WindowPtr
CompositeRealChildHead(WindowPtr pWin);

int
DeleteWindowNoInputDevices(void *value, XID wid);

int

compConfigNotify(WindowPtr pWin, int x, int y, int w, int h,
                 int bw, WindowPtr pSib);

void PanoramiXCompositeInit(void);
void PanoramiXCompositeReset(void);

#endif                          /* _COMPINT_H_ */
