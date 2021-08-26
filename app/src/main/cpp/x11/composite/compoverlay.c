#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "compint.h"
#include "../xext/xace.h"

#ifdef PANORAMIX
#include "../xext/panoramiXsrv.h"
#endif

/*
 * Delete the given overlay client list element from its screen list.
 */
void
compFreeOverlayClient(CompOverlayClientPtr pOcToDel)
{
    ScreenPtr pScreen = pOcToDel->pScreen;
    CompScreenPtr cs = GetCompScreen(pScreen);
    CompOverlayClientPtr *pPrev, pOc;

    for (pPrev = &cs->pOverlayClients; (pOc = *pPrev); pPrev = &pOc->pNext) {
        if (pOc == pOcToDel) {
            *pPrev = pOc->pNext;
            free(pOc);
            break;
        }
    }

    /* Destroy overlay window when there are no more clients using it */
    if (cs->pOverlayClients == NULL)
        compDestroyOverlayWindow(pScreen);
}

/*
 * Return the client's first overlay client rec from the given screen
 */
CompOverlayClientPtr
compFindOverlayClient(ScreenPtr pScreen, ClientPtr pClient)
{
    CompScreenPtr cs = GetCompScreen(pScreen);
    CompOverlayClientPtr pOc;

    for (pOc = cs->pOverlayClients; pOc != NULL; pOc = pOc->pNext)
        if (pOc->pClient == pClient)
            return pOc;

    return NULL;
}

/*
 * Create an overlay client object for the given client
 */
CompOverlayClientPtr
compCreateOverlayClient(ScreenPtr pScreen, ClientPtr pClient)
{
    CompScreenPtr cs = GetCompScreen(pScreen);
    CompOverlayClientPtr pOc;

    pOc = (CompOverlayClientPtr) malloc(sizeof(CompOverlayClientRec));
    if (pOc == NULL)
        return NULL;

    pOc->pClient = pClient;
    pOc->pScreen = pScreen;
    pOc->resource = FakeClientID(pClient->index);
    pOc->pNext = cs->pOverlayClients;
    cs->pOverlayClients = pOc;

    /*
     * Create a resource for this element so it can be deleted
     * when the client goes away.
     */
    if (!AddResource(pOc->resource, CompositeClientOverlayType, (void *) pOc))
        return NULL;

    return pOc;
}

/*
 * Create the overlay window and map it
 */
Bool
compCreateOverlayWindow(ScreenPtr pScreen)
{
    CompScreenPtr cs = GetCompScreen(pScreen);
    WindowPtr pRoot = pScreen->root;
    WindowPtr pWin;
    XID attrs[] = { None, TRUE };       /* backPixmap, overrideRedirect */
    int result;
    int w = pScreen->width;
    int h = pScreen->height;
    int x = 0, y = 0;

#ifdef PANORAMIX
    if (!noPanoramiXExtension) {
        x = -pScreen->x;
        y = -pScreen->y;
        w = PanoramiXPixWidth;
        h = PanoramiXPixHeight;
    }
#endif

    pWin = cs->pOverlayWin =
            CreateWindow(cs->overlayWid, pRoot, x, y, w, h, 0,
                         InputOutput, CWBackPixmap | CWOverrideRedirect, &attrs[0],
                         pRoot->drawable.depth,
                         serverClient, pScreen->rootVisual, &result);
    if (pWin == NULL)
        return FALSE;

    if (!AddResource(pWin->drawable.id, RT_WINDOW, (void *) pWin))
        return FALSE;

    MapWindow(pWin, serverClient);

    return TRUE;
}

/*
 * Destroy the overlay window
 */
void
compDestroyOverlayWindow(ScreenPtr pScreen)
{
    CompScreenPtr cs = GetCompScreen(pScreen);

    cs->pOverlayWin = NullWindow;
    FreeResource(cs->overlayWid, RT_NONE);
}
