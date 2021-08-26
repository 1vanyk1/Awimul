#include "vndserver.h"

#include <string.h>
#include "../headers/scrnintstr.h"
#include "../headers/windowstr.h"
#include "../headers/dixstruct.h"
#include "../headers/extnsionst.h"
#include "../headers/glx_extinit.h"

#include "../../GL/glxproto.h"
#include "vndservervendor.h"

ExtensionEntry *GlxExtensionEntry;
int GlxErrorBase = 0;
static CallbackListRec vndInitCallbackList;
static CallbackListPtr vndInitCallbackListPtr = &vndInitCallbackList;
static DevPrivateKeyRec glvXGLVScreenPrivKey;
static DevPrivateKeyRec glvXGLVClientPrivKey;

// The resource type used to keep track of the vendor library for XID's.
RESTYPE idResource;

static int
idResourceDeleteCallback(void *value, XID id)
{
    return 0;
}

static GlxScreenPriv *
xglvGetScreenPrivate(ScreenPtr pScreen)
{
    return dixLookupPrivate(&pScreen->devPrivates, &glvXGLVScreenPrivKey);
}

static void
xglvSetScreenPrivate(ScreenPtr pScreen, void *priv)
{
    dixSetPrivate(&pScreen->devPrivates, &glvXGLVScreenPrivKey, priv);
}

GlxScreenPriv *
GlxGetScreen(ScreenPtr pScreen)
{
    if (pScreen != NULL) {
        GlxScreenPriv *priv = xglvGetScreenPrivate(pScreen);
        if (priv == NULL) {
            priv = calloc(1, sizeof(GlxScreenPriv));
            if (priv == NULL) {
                return NULL;
            }

            xglvSetScreenPrivate(pScreen, priv);
        }
        return priv;
    } else {
        return NULL;
    }
}

static void
GlxMappingReset(void)
{
    int i;

    for (i=0; i<screenInfo.numScreens; i++) {
        GlxScreenPriv *priv = xglvGetScreenPrivate(screenInfo.screens[i]);
        if (priv != NULL) {
            xglvSetScreenPrivate(screenInfo.screens[i], NULL);
            free(priv);
        }
    }
}

static Bool
GlxMappingInit(void)
{
    int i;

    for (i=0; i<screenInfo.numScreens; i++) {
        if (GlxGetScreen(screenInfo.screens[i]) == NULL) {
            GlxMappingReset();
            return FALSE;
        }
    }

    idResource = CreateNewResourceType(idResourceDeleteCallback,
                                       "GLXServerIDRes");
    if (idResource == RT_NONE)
    {
        GlxMappingReset();
        return FALSE;
    }
    return TRUE;
}

static GlxClientPriv *
xglvGetClientPrivate(ClientPtr pClient)
{
    return dixLookupPrivate(&pClient->devPrivates, &glvXGLVClientPrivKey);
}

static void
xglvSetClientPrivate(ClientPtr pClient, void *priv)
{
    dixSetPrivate(&pClient->devPrivates, &glvXGLVClientPrivKey, priv);
}

GlxClientPriv *
GlxGetClientData(ClientPtr client)
{
    GlxClientPriv *cl = xglvGetClientPrivate(client);
    if (cl == NULL) {
        cl = calloc(1, sizeof(GlxClientPriv)
                       + screenInfo.numScreens * sizeof(GlxServerVendor *));
        if (cl != NULL) {
            int i;

            cl->vendors = (GlxServerVendor **) (cl + 1);
            for (i=0; i<screenInfo.numScreens; i++)
            {
                cl->vendors[i] = GlxGetVendorForScreen(NULL, screenInfo.screens[i]);
            }

            xglvSetClientPrivate(client, cl);
        }
    }
    return cl;
}

void
GlxFreeClientData(ClientPtr client)
{
    GlxClientPriv *cl = xglvGetClientPrivate(client);
    if (cl != NULL) {
        unsigned int i;
        for (i = 0; i < cl->contextTagCount; i++) {
            GlxContextTagInfo *tag = &cl->contextTags[i];
            if (tag->vendor != NULL) {
                tag->vendor->glxvc.makeCurrent(client, tag->tag,
                                               None, None, None, 0);
            }
        }
        xglvSetClientPrivate(client, NULL);
        free(cl->contextTags);
        free(cl);
    }
}

static void
GLXClientCallback(CallbackListPtr *list, void *closure, void *data)
{
    NewClientInfoRec *clientinfo = (NewClientInfoRec *) data;
    ClientPtr client = clientinfo->client;

    switch (client->clientState)
    {
        case ClientStateRetained:
        case ClientStateGone:
            GlxFreeClientData(client);
            break;
    }
}

static void
GLXReset(ExtensionEntry *extEntry)
{
    // xf86Msg(X_INFO, "GLX: GLXReset\n");

    GlxVendorExtensionReset(extEntry);
    GlxDispatchReset();
    GlxMappingReset();

    if ((dispatchException & DE_TERMINATE) == DE_TERMINATE) {
        while (vndInitCallbackList.list != NULL) {
            CallbackPtr next = vndInitCallbackList.list->next;
            free(vndInitCallbackList.list);
            vndInitCallbackList.list = next;
        }
    }
}

void
GlxExtensionInit(void)
{
    ExtensionEntry *extEntry;
    GlxExtensionEntry = NULL;

    // Init private keys, per-screen data
    if (!dixRegisterPrivateKey(&glvXGLVScreenPrivKey, PRIVATE_SCREEN, 0))
        return;
    if (!dixRegisterPrivateKey(&glvXGLVClientPrivKey, PRIVATE_CLIENT, 0))
        return;

    if (!GlxMappingInit()) {
        return;
    }

    if (!GlxDispatchInit()) {
        return;
    }

    if (!AddCallback(&ClientStateCallback, GLXClientCallback, NULL)) {
        return;
    }

    extEntry = AddExtension(GLX_EXTENSION_NAME, __GLX_NUMBER_EVENTS,
                            __GLX_NUMBER_ERRORS, GlxDispatchRequest,
                            GlxDispatchRequest, GLXReset, StandardMinorOpcode);
    if (!extEntry) {
        return;
    }

    GlxExtensionEntry = extEntry;
    GlxErrorBase = extEntry->errorBase;
    CallCallbacks(&vndInitCallbackListPtr, extEntry);

    /* We'd better have found at least one vendor */
    for (int i = 0; i < screenInfo.numScreens; i++)
        if (GlxGetVendorForScreen(serverClient, screenInfo.screens[i]))
            return;
    extEntry->base = 0;
}

static int
GlxForwardRequest(GlxServerVendor *vendor, ClientPtr client)
{
    return vendor->glxvc.handleRequest(client);
}

static GlxServerVendor *
GlxGetContextTag(ClientPtr client, GLXContextTag tag)
{
    GlxContextTagInfo *tagInfo = GlxLookupContextTag(client, tag);

    if (tagInfo != NULL) {
        return tagInfo->vendor;
    } else {
        return NULL;
    }
}

static Bool
GlxSetContextTagPrivate(ClientPtr client, GLXContextTag tag, void *data)
{
    GlxContextTagInfo *tagInfo = GlxLookupContextTag(client, tag);
    if (tagInfo != NULL) {
        tagInfo->data = data;
        return TRUE;
    } else {
        return FALSE;
    }
}

static void *
GlxGetContextTagPrivate(ClientPtr client, GLXContextTag tag)
{
    GlxContextTagInfo *tagInfo = GlxLookupContextTag(client, tag);
    if (tagInfo != NULL) {
        return tagInfo->data;
    } else {
        return NULL;
    }
}

static GlxServerImports *
GlxAllocateServerImports(void)
{
    return calloc(1, sizeof(GlxServerImports));
}

static void
GlxFreeServerImports(GlxServerImports *imports)
{
    free(imports);
}

_X_EXPORT const GlxServerExports glxServer = {
        .majorVersion = GLXSERVER_VENDOR_ABI_MAJOR_VERSION,
        .minorVersion = GLXSERVER_VENDOR_ABI_MINOR_VERSION,

        .extensionInitCallback = &vndInitCallbackListPtr,

        .allocateServerImports = GlxAllocateServerImports,
        .freeServerImports = GlxFreeServerImports,

        .createVendor = GlxCreateVendor,
        .destroyVendor = GlxDestroyVendor,
        .setScreenVendor = GlxSetScreenVendor,

        .addXIDMap = GlxAddXIDMap,
        .getXIDMap = GlxGetXIDMap,
        .removeXIDMap = GlxRemoveXIDMap,
        .getContextTag = GlxGetContextTag,
        .setContextTagPrivate = GlxSetContextTagPrivate,
        .getContextTagPrivate = GlxGetContextTagPrivate,
        .getVendorForScreen = GlxGetVendorForScreen,
        .forwardRequest =  GlxForwardRequest,
        .setClientScreenVendor = GlxSetClientScreenVendor,
};

const GlxServerExports *
glvndGetExports(void)
{
    return &glxServer;
}
