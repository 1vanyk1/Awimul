#ifndef VNDSERVER_H
#define VNDSERVER_H

//#include <dix-config.h>
#include "../headers/glxvndabi.h"

#define GLXContextID CARD32
#define GLXDrawable CARD32

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct GlxScreenPrivRec {
    GlxServerVendor *vendor;
} GlxScreenPriv;

typedef struct GlxContextTagInfoRec {
    GLXContextTag tag;
    ClientPtr client;
    GlxServerVendor *vendor;
    void *data;
    GLXContextID context;
    GLXDrawable drawable;
    GLXDrawable readdrawable;
} GlxContextTagInfo;

typedef struct GlxClientPrivRec {
    GlxContextTagInfo *contextTags;
    unsigned int contextTagCount;

    /**
     * The vendor handles for each screen.
     */
    GlxServerVendor **vendors;
} GlxClientPriv;

extern int GlxErrorBase;
extern RESTYPE idResource;

extern ExtensionEntry *GlxExtensionEntry;
Bool GlxDispatchInit(void);
void GlxDispatchReset(void);

/**
 * Handles a request from the client.
 *
 * This function will look up the correct handler function and forward the
 * request to it.
 */
int GlxDispatchRequest(ClientPtr client);

/**
 * Looks up the GlxClientPriv struct for a client. If we don't have a
 * GlxClientPriv struct yet, then allocate one.
 */
GlxClientPriv *GlxGetClientData(ClientPtr client);

/**
 * Frees any data that's specific to a client. This should be called when a
 * client disconnects.
 */
void GlxFreeClientData(ClientPtr client);

Bool GlxAddXIDMap(XID id, GlxServerVendor *vendor);
GlxServerVendor * GlxGetXIDMap(XID id);
void GlxRemoveXIDMap(XID id);

/**
 * Records the client that sent the current request. This is needed in
 * GlxGetXIDMap to know which client's (screen -> vendor) mapping to use for a
 * regular X window.
 */
void GlxSetRequestClient(ClientPtr client);

GlxContextTagInfo *GlxAllocContextTag(ClientPtr client, GlxServerVendor *vendor);
GlxContextTagInfo *GlxLookupContextTag(ClientPtr client, GLXContextTag tag);
void GlxFreeContextTag(GlxContextTagInfo *tagInfo);

Bool GlxSetScreenVendor(ScreenPtr screen, GlxServerVendor *vendor);
Bool GlxSetClientScreenVendor(ClientPtr client, ScreenPtr screen, GlxServerVendor *vendor);
GlxScreenPriv *GlxGetScreen(ScreenPtr pScreen);
GlxServerVendor *GlxGetVendorForScreen(ClientPtr client, ScreenPtr screen);

static inline CARD32 GlxCheckSwap(ClientPtr client, CARD32 value)
{
    if (client->swapped)
    {
        value = ((value & 0XFF000000) >> 24) | ((value & 0X00FF0000) >>  8)
                | ((value & 0X0000FF00) <<  8) | ((value & 0X000000FF) << 24);
    }
    return value;
}

#if defined(__cplusplus)
}
#endif

_X_EXPORT const GlxServerExports *glvndGetExports(void);

#endif // VNDSERVER_H
