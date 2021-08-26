#ifndef _XGE_H_
#define _XGE_H_

#include "../xlib.h"
#include "../xfuncproto.h"

_XFUNCPROTOBEGIN

/**
 * Generic Event mask.
 * To be used whenever a list of masks per extension has to be provided.
 *
 * But, don't actually use the CARD{8,16,32} types.  We can't get them them
 * defined here without polluting the namespace.
 */
typedef struct {
    unsigned char       extension;
    unsigned char       pad0;
    unsigned short      pad1;
    unsigned int      evmask;
} XGenericEventMask;

Bool XGEQueryExtension(Display* dpy, int *event_basep, int *err_basep);
Bool XGEQueryVersion(Display* dpy, int *major, int* minor);

_XFUNCPROTOEND

#endif /* _XGE_H_ */
