#ifndef GLX_EXT_INIT_H
#define GLX_EXT_INIT_H

/* this is separate due to sdksyms pulling in extinit.h */
/* XXX this comment no longer makes sense i think */
#include "xdefs.h"

typedef struct __GLXprovider __GLXprovider;
typedef struct __GLXscreen __GLXscreen;
struct __GLXprovider {
    __GLXscreen *(*screenProbe) (ScreenPtr pScreen);
    const char *name;
    __GLXprovider *next;
};
extern __GLXprovider __glXDRISWRastProvider;

void GlxPushProvider(__GLXprovider * provider);
Bool xorgGlxCreateVendor(void);
//#else
//static inline Bool xorgGlxCreateVendor(void) { return TRUE; }
//#endif


#endif
