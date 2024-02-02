#ifndef _XMU_CVTCACHE_H_
#define _XMU_CVTCACHE_H_

#include "DisplayQue.h"
#include "../headers/xfuncproto.h"

typedef struct _XmuCvtCache {
    struct {
        char **bitmapFilePath;
    } string_to_bitmap;
    /* add other per-display data that needs to be cached */
} XmuCvtCache;

_XFUNCPROTOBEGIN

        XmuCvtCache *_XmuCCLookupDisplay
(
        Display	*dpy
);

extern void _XmuStringToBitmapInitCache(XmuCvtCache *c);
extern void _XmuStringToBitmapFreeCache(XmuCvtCache *c);

_XFUNCPROTOEND

#endif /* _XMU_CVTCACHE_H_ */
