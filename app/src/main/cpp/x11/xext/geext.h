#ifdef HAVE_DIX_CONFIG_H
#include "../headers/dix-config.h"
#endif

#ifndef _GEEXT_H_
#define _GEEXT_H_
#include "../headers/extensions/geproto.h"
#include "../headers/xlocale.h"
#include "../headers/misc.h"
#include "../headers/xproto.h"

/** Struct to keep information about registered extensions */
typedef struct _GEExtension {
    /** Event swapping routine */
    void (*evswap) (xGenericEvent *from, xGenericEvent *to);
} GEExtension, *GEExtensionPtr;

/* All registered extensions and their handling functions. */
extern _X_EXPORT GEExtension GEExtensions[MAXEXTENSIONS];

/* Typecast to generic event */
#define GEV(ev) ((xGenericEvent*)(ev))
/* Returns the extension offset from the event */
#define GEEXT(ev) (GEV(ev)->extension)

/* Return zero-based extension offset (offset - 128). Only for use in arrays */
#define GEEXTIDX(ev) (GEEXT(ev) & 0x7F)
/* True if mask is set for extension on window */
#define GEMaskIsSet(pWin, extension, mask) \
    ((pWin)->optional && \
     (pWin)->optional->geMasks && \
     ((pWin)->optional->geMasks->eventMasks[(extension) & 0x7F] & (mask)))

/* Returns first client */
#define GECLIENT(pWin) \
    (((pWin)->optional) ? (pWin)->optional->geMasks->geClients : NULL)

/* Returns the event_fill for the given event */
#define GEEventFill(ev) \
    GEExtensions[GEEXTIDX(ev)].evfill

#define GEIsType(ev, ext, ev_type) \
        ((GEV(ev)->type == GenericEvent) &&  \
         GEEXT(ev) == (ext) && \
         GEV(ev)->evtype == (ev_type))

/* Interface for other extensions */
extern _X_EXPORT void GERegisterExtension(int extension,
                                          void (*ev_dispatch) (xGenericEvent
                                                               *from,
                                                               xGenericEvent
                                                               *to));

extern _X_EXPORT void GEInitEvent(xGenericEvent *ev, int extension);

#endif                          /* _GEEXT_H_ */