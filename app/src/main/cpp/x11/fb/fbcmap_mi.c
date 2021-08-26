#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/x.h"
#include "fb.h"
#include "../mi/micmap.h"

int
fbListInstalledColormaps(ScreenPtr pScreen, Colormap * pmaps)
{
    return miListInstalledColormaps(pScreen, pmaps);
}

void
fbInstallColormap(ColormapPtr pmap)
{
    miInstallColormap(pmap);
}

void
fbUninstallColormap(ColormapPtr pmap)
{
    miUninstallColormap(pmap);
}

void
fbResolveColor(unsigned short *pred,
               unsigned short *pgreen, unsigned short *pblue, VisualPtr pVisual)
{
    miResolveColor(pred, pgreen, pblue, pVisual);
}

Bool
fbInitializeColormap(ColormapPtr pmap)
{
    return miInitializeColormap(pmap);
}

int
fbExpandDirectColors(ColormapPtr pmap,
                     int ndef, xColorItem * indefs, xColorItem * outdefs)
{
    return miExpandDirectColors(pmap, ndef, indefs, outdefs);
}

Bool
fbCreateDefColormap(ScreenPtr pScreen)
{
    return miCreateDefColormap(pScreen);
}

void
fbClearVisualTypes(void)
{
    miClearVisualTypes();
}

Bool
fbSetVisualTypes(int depth, int visuals, int bitsPerRGB)
{
    return miSetVisualTypes(depth, visuals, bitsPerRGB, -1);
}

Bool
fbSetVisualTypesAndMasks(int depth, int visuals, int bitsPerRGB,
                         Pixel redMask, Pixel greenMask, Pixel blueMask)
{
    return miSetVisualTypesAndMasks(depth, visuals, bitsPerRGB, -1,
                                    redMask, greenMask, blueMask);
}

/*
 * Given a list of formats for a screen, create a list
 * of visuals and depths for the screen which correspond to
 * the set which can be used with this version of fb.
 */
Bool
fbInitVisuals(VisualPtr * visualp,
              DepthPtr * depthp,
              int *nvisualp,
              int *ndepthp,
              int *rootDepthp,
              VisualID * defaultVisp, unsigned long sizes, int bitsPerRGB)
{
    return miInitVisuals(visualp, depthp, nvisualp, ndepthp, rootDepthp,
                         defaultVisp, sizes, bitsPerRGB, -1);
}
