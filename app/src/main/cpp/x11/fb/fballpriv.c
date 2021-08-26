#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "fb.h"

static DevPrivateKeyRec fbScreenPrivateKeyRec;
DevPrivateKey
fbGetScreenPrivateKey(void)
{
    return &fbScreenPrivateKeyRec;
}

Bool
fbAllocatePrivates(ScreenPtr pScreen)
{
    FbScreenPrivPtr     pScrPriv;

    if (!dixRegisterPrivateKey
            (&fbScreenPrivateKeyRec, PRIVATE_SCREEN, sizeof(FbScreenPrivRec)))
        return FALSE;

    pScrPriv = fbGetScreenPrivate(pScreen);

    if (!dixRegisterScreenSpecificPrivateKey (pScreen, &pScrPriv->gcPrivateKeyRec, PRIVATE_GC, sizeof(FbGCPrivRec)))
        return FALSE;
    if (!dixRegisterScreenSpecificPrivateKey (pScreen, &pScrPriv->winPrivateKeyRec, PRIVATE_WINDOW, 0))
        return FALSE;

    return TRUE;
}

#ifdef FB_ACCESS_WRAPPER
ReadMemoryProcPtr wfbReadMemory;
WriteMemoryProcPtr wfbWriteMemory;
#endif
