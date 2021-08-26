#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/x.h"
#include "../headers/xproto.h"
#include "../headers/windowstr.h"
#include "../headers/extnsionst.h"
#include "../headers/dixstruct.h"
#include "../headers/dixevents.h"
#include "../headers/dispatch.h"
#include "../headers/swaprep.h"
#include "../headers/swapreq.h"

int (*InitialVector[3]) (ClientPtr /* client */) = {
        0,
        ProcInitialConnection,
        ProcEstablishConnection
};

int (*ProcVector[256]) (ClientPtr /* client */) = {
        ProcBadRequest,
        ProcCreateWindow,
        ProcChangeWindowAttributes,
        ProcGetWindowAttributes,
        ProcDestroyWindow,
        ProcDestroySubwindows,              /* 5 */
        ProcChangeSaveSet,
        ProcReparentWindow,
        ProcMapWindow,
        ProcMapSubwindows,
        ProcUnmapWindow,                    /* 10 */
        ProcUnmapSubwindows,
        ProcConfigureWindow,
        ProcCirculateWindow,
        ProcGetGeometry,
        ProcQueryTree,                      /* 15 */
        ProcInternAtom,
        ProcGetAtomName,
        ProcChangeProperty,
        ProcDeleteProperty,
        ProcGetProperty,                    /* 20 */
        ProcListProperties,
        ProcSetSelectionOwner,
        ProcGetSelectionOwner,
        ProcConvertSelection,
        ProcSendEvent,                      /* 25 */
        ProcGrabPointer,
        ProcUngrabPointer,
        ProcGrabButton,
        ProcUngrabButton,
        ProcChangeActivePointerGrab,        /* 30 */
        ProcGrabKeyboard,
        ProcUngrabKeyboard,
        ProcGrabKey,
        ProcUngrabKey,
        ProcAllowEvents,                    /* 35 */
        ProcGrabServer,
        ProcUngrabServer,
        ProcQueryPointer,
        ProcGetMotionEvents,
        ProcTranslateCoords,                /* 40 */
        ProcWarpPointer,
        ProcSetInputFocus,
        ProcGetInputFocus,
        ProcQueryKeymap,
        ProcOpenFont,                       /* 45 */
        ProcCloseFont,
        ProcQueryFont,
        ProcQueryTextExtents,
        ProcListFonts,
        ProcListFontsWithInfo,              /* 50 */
        ProcSetFontPath,
        ProcGetFontPath,
        ProcCreatePixmap,
        ProcFreePixmap,
        ProcCreateGC,                       /* 55 */
        ProcChangeGC,
        ProcCopyGC,
        ProcSetDashes,
        ProcSetClipRectangles,
        ProcFreeGC,                         /* 60 */
        ProcClearToBackground,
        ProcCopyArea,
        ProcCopyPlane,
        ProcPolyPoint,
        ProcPolyLine,                       /* 65 */
        ProcPolySegment,
        ProcPolyRectangle,
        ProcPolyArc,
        ProcFillPoly,
        ProcPolyFillRectangle,              /* 70 */
        ProcPolyFillArc,
        ProcPutImage,
        ProcGetImage,
        ProcPolyText,
        ProcPolyText,                       /* 75 */
        ProcImageText8,
        ProcImageText16,
        ProcCreateColormap,
        ProcFreeColormap,
        ProcCopyColormapAndFree,            /* 80 */
        ProcInstallColormap,
        ProcUninstallColormap,
        ProcListInstalledColormaps,
        ProcAllocColor,
        ProcAllocNamedColor,                /* 85 */
        ProcAllocColorCells,
        ProcAllocColorPlanes,
        ProcFreeColors,
        ProcStoreColors,
        ProcStoreNamedColor,                /* 90 */
        ProcQueryColors,
        ProcLookupColor,
        ProcCreateCursor,
        ProcCreateGlyphCursor,
        ProcFreeCursor,                     /* 95 */
        ProcRecolorCursor,
        ProcQueryBestSize,
        ProcQueryExtension,
        ProcListExtensions,
        ProcChangeKeyboardMapping,          /* 100 */
        ProcGetKeyboardMapping,
        ProcChangeKeyboardControl,
        ProcGetKeyboardControl,
        ProcBell,
        ProcChangePointerControl,           /* 105 */
        ProcGetPointerControl,
        ProcSetScreenSaver,
        ProcGetScreenSaver,
        ProcChangeHosts,
        ProcListHosts,                      /* 110 */
        ProcChangeAccessControl,
        ProcChangeCloseDownMode,
        ProcKillClient,
        ProcRotateProperties,
        ProcForceScreenSaver,               /* 115 */
        ProcSetPointerMapping,
        ProcGetPointerMapping,
        ProcSetModifierMapping,
        ProcGetModifierMapping,
        ProcBadRequest,                     /* 120 */
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,                     /* 125 */
        ProcBadRequest,
        ProcNoOperation,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest
};

int (*SwappedProcVector[256]) (ClientPtr /* client */) = {
        ProcBadRequest,
        SProcCreateWindow,
        SProcChangeWindowAttributes,
        SProcResourceReq,                   /* GetWindowAttributes */
        SProcResourceReq,                   /* DestroyWindow */
        SProcResourceReq,                   /* 5 DestroySubwindows */
        SProcResourceReq,                   /* SProcChangeSaveSet, */
        SProcReparentWindow,
        SProcResourceReq,                   /* MapWindow */
        SProcResourceReq,                   /* MapSubwindows */
        SProcResourceReq,                   /* 10 UnmapWindow */
        SProcResourceReq,                   /* UnmapSubwindows */
        SProcConfigureWindow,
        SProcResourceReq,                   /* SProcCirculateWindow, */
        SProcResourceReq,                   /* GetGeometry */
        SProcResourceReq,                   /* 15 QueryTree */
        SProcInternAtom,
        SProcResourceReq,                   /* SProcGetAtomName, */
        SProcChangeProperty,
        SProcDeleteProperty,
        SProcGetProperty,                   /* 20 */
        SProcResourceReq,                   /* SProcListProperties, */
        SProcSetSelectionOwner,
        SProcResourceReq,                   /* SProcGetSelectionOwner, */
        SProcConvertSelection,
        SProcSendEvent,                     /* 25 */
        SProcGrabPointer,
        SProcResourceReq,                   /* SProcUngrabPointer, */
        SProcGrabButton,
        SProcUngrabButton,
        SProcChangeActivePointerGrab,       /* 30 */
        SProcGrabKeyboard,
        SProcResourceReq,                   /* SProcUngrabKeyboard, */
        SProcGrabKey,
        SProcUngrabKey,
        SProcResourceReq,                   /* 35 SProcAllowEvents, */
        SProcSimpleReq,                     /* SProcGrabServer, */
        SProcSimpleReq,                     /* SProcUngrabServer, */
        SProcResourceReq,                   /* SProcQueryPointer, */
        SProcGetMotionEvents,
        SProcTranslateCoords,               /*40 */
        SProcWarpPointer,
        SProcSetInputFocus,
        SProcSimpleReq,                     /* SProcGetInputFocus, */
        SProcSimpleReq,                     /* QueryKeymap, */
        SProcOpenFont,                      /* 45 */
        SProcResourceReq,                   /* SProcCloseFont, */
        SProcResourceReq,                   /* SProcQueryFont, */
        SProcResourceReq,                   /* SProcQueryTextExtents,  */
        SProcListFonts,
        SProcListFontsWithInfo,             /* 50 */
        SProcSetFontPath,
        SProcSimpleReq,                     /* GetFontPath, */
        SProcCreatePixmap,
        SProcResourceReq,                   /* SProcFreePixmap, */
        SProcCreateGC,                      /* 55 */
        SProcChangeGC,
        SProcCopyGC,
        SProcSetDashes,
        SProcSetClipRectangles,
        SProcResourceReq,                   /* 60 SProcFreeGC, */
        SProcClearToBackground,
        SProcCopyArea,
        SProcCopyPlane,
        SProcPoly,                          /* PolyPoint, */
        SProcPoly,                          /* 65 PolyLine */
        SProcPoly,                          /* PolySegment, */
        SProcPoly,                          /* PolyRectangle, */
        SProcPoly,                          /* PolyArc, */
        SProcFillPoly,
        SProcPoly,                          /* 70 PolyFillRectangle */
        SProcPoly,                          /* PolyFillArc, */
        SProcPutImage,
        SProcGetImage,
        SProcPolyText,
        SProcPolyText,                      /* 75 */
        SProcImageText,
        SProcImageText,
        SProcCreateColormap,
        SProcResourceReq,                   /* SProcFreeColormap, */
        SProcCopyColormapAndFree,           /* 80 */
        SProcResourceReq,                   /* SProcInstallColormap, */
        SProcResourceReq,                   /* SProcUninstallColormap, */
        SProcResourceReq,                   /* SProcListInstalledColormaps, */
        SProcAllocColor,
        SProcAllocNamedColor,               /* 85 */
        SProcAllocColorCells,
        SProcAllocColorPlanes,
        SProcFreeColors,
        SProcStoreColors,
        SProcStoreNamedColor,               /* 90 */
        SProcQueryColors,
        SProcLookupColor,
        SProcCreateCursor,
        SProcCreateGlyphCursor,
        SProcResourceReq,                   /* 95 SProcFreeCursor, */
        SProcRecolorCursor,
        SProcQueryBestSize,
        SProcQueryExtension,
        SProcSimpleReq,                     /* ListExtensions, */
        SProcChangeKeyboardMapping,         /* 100 */
        SProcSimpleReq,                     /* GetKeyboardMapping, */
        SProcChangeKeyboardControl,
        SProcSimpleReq,                     /* GetKeyboardControl, */
        SProcSimpleReq,                     /* Bell, */
        SProcChangePointerControl,          /* 105 */
        SProcSimpleReq,                     /* GetPointerControl, */
        SProcSetScreenSaver,
        SProcSimpleReq,                     /* GetScreenSaver, */
        SProcChangeHosts,
        SProcSimpleReq,                     /* 110 ListHosts, */
        SProcSimpleReq,                     /* SProcChangeAccessControl, */
        SProcSimpleReq,                     /* SProcChangeCloseDownMode, */
        SProcResourceReq,                   /* SProcKillClient, */
        SProcRotateProperties,
        SProcSimpleReq,                     /* 115 ForceScreenSaver */
        SProcSimpleReq,                     /* SetPointerMapping, */
        SProcSimpleReq,                     /* GetPointerMapping, */
        SProcSimpleReq,                     /* SetModifierMapping, */
        SProcSimpleReq,                     /* GetModifierMapping, */
        ProcBadRequest,                     /* 120 */
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,                     /* 125 */
        ProcBadRequest,
        SProcNoOperation,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest,
        ProcBadRequest
};

EventSwapPtr EventSwapVector[MAXEVENTS] = {
        (EventSwapPtr) SErrorEvent,
        NotImplemented,
        SKeyButtonPtrEvent,
        SKeyButtonPtrEvent,
        SKeyButtonPtrEvent,
        SKeyButtonPtrEvent,                         /* 5 */
        SKeyButtonPtrEvent,
        SEnterLeaveEvent,
        SEnterLeaveEvent,
        SFocusEvent,
        SFocusEvent,                                /* 10 */
        SKeymapNotifyEvent,
        SExposeEvent,
        SGraphicsExposureEvent,
        SNoExposureEvent,
        SVisibilityEvent,                           /* 15 */
        SCreateNotifyEvent,
        SDestroyNotifyEvent,
        SUnmapNotifyEvent,
        SMapNotifyEvent,
        SMapRequestEvent,                           /* 20 */
        SReparentEvent,
        SConfigureNotifyEvent,
        SConfigureRequestEvent,
        SGravityEvent,
        SResizeRequestEvent,                        /* 25 */
        SCirculateEvent,
        SCirculateEvent,
        SPropertyEvent,
        SSelectionClearEvent,
        SSelectionRequestEvent,                     /* 30 */
        SSelectionNotifyEvent,
        SColormapEvent,
        SClientMessageEvent,
        SMappingEvent,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented,
        NotImplemented
};

ReplySwapPtr ReplySwapVector[256] = {
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        (ReplySwapPtr) SGetWindowAttributesReply,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 5 */
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 10 */
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        (ReplySwapPtr) SGetGeometryReply,
        (ReplySwapPtr) SQueryTreeReply,             /* 15 */
        (ReplySwapPtr) SInternAtomReply,
        (ReplySwapPtr) SGetAtomNameReply,
        ReplyNotSwappd,
        ReplyNotSwappd,
        (ReplySwapPtr) SGetPropertyReply,           /* 20 */
        (ReplySwapPtr) SListPropertiesReply,
        ReplyNotSwappd,
        (ReplySwapPtr) SGetSelectionOwnerReply,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 25 */
        (ReplySwapPtr) SGenericReply,               /* SGrabPointerReply, */
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 30 */
        (ReplySwapPtr) SGenericReply,               /* SGrabKeyboardReply, */
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 35 */
        ReplyNotSwappd,
        ReplyNotSwappd,
        (ReplySwapPtr) SQueryPointerReply,
        (ReplySwapPtr) SGetMotionEventsReply,
        (ReplySwapPtr) STranslateCoordsReply,       /* 40 */
        ReplyNotSwappd,
        ReplyNotSwappd,
        (ReplySwapPtr) SGetInputFocusReply,
        (ReplySwapPtr) SQueryKeymapReply,
        ReplyNotSwappd,                             /* 45 */
        ReplyNotSwappd,
        (ReplySwapPtr) SQueryFontReply,
        (ReplySwapPtr) SQueryTextExtentsReply,
        (ReplySwapPtr) SListFontsReply,
        (ReplySwapPtr) SListFontsWithInfoReply,     /* 50 */
        ReplyNotSwappd,
        (ReplySwapPtr) SGetFontPathReply,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 55 */
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 60 */
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 65 */
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 70 */
        ReplyNotSwappd,
        ReplyNotSwappd,
        (ReplySwapPtr) SGetImageReply,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 75 */
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 80 */
        ReplyNotSwappd,
        ReplyNotSwappd,
        (ReplySwapPtr) SListInstalledColormapsReply,
        (ReplySwapPtr) SAllocColorReply,
        (ReplySwapPtr) SAllocNamedColorReply,       /* 85 */
        (ReplySwapPtr) SAllocColorCellsReply,
        (ReplySwapPtr) SAllocColorPlanesReply,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 90 */
        (ReplySwapPtr) SQueryColorsReply,
        (ReplySwapPtr) SLookupColorReply,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 95 */
        ReplyNotSwappd,
        (ReplySwapPtr) SQueryBestSizeReply,
        (ReplySwapPtr) SGenericReply,               /* SQueryExtensionReply, */
        (ReplySwapPtr) SListExtensionsReply,
        ReplyNotSwappd,                             /* 100 */
        (ReplySwapPtr) SGetKeyboardMappingReply,
        ReplyNotSwappd,
        (ReplySwapPtr) SGetKeyboardControlReply,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 105 */
        (ReplySwapPtr) SGetPointerControlReply,
        ReplyNotSwappd,
        (ReplySwapPtr) SGetScreenSaverReply,
        ReplyNotSwappd,
        (ReplySwapPtr) SListHostsReply,             /* 110 */
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,                             /* 115 */
        (ReplySwapPtr) SGenericReply,               /* SetPointerMapping */
        (ReplySwapPtr) SGetPointerMappingReply,
        (ReplySwapPtr) SGenericReply,               /* SetModifierMapping */
        (ReplySwapPtr) SGetModifierMappingReply,    /* 119 */
        ReplyNotSwappd,                             /* 120 */
        ReplyNotSwappd,                             /* 121 */
        ReplyNotSwappd,                             /* 122 */
        ReplyNotSwappd,                             /* 123 */
        ReplyNotSwappd,                             /* 124 */
        ReplyNotSwappd,                             /* 125 */
        ReplyNotSwappd,                             /* 126 */
        ReplyNotSwappd,                             /* NoOperation */
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd,
        ReplyNotSwappd
};
