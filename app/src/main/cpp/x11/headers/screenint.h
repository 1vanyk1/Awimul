#ifndef SCREENINT_H
#define SCREENINT_H

#include "misc.h"
#include "xlocale.h"

typedef struct _PixmapFormat *PixmapFormatPtr;
typedef struct _Visual *VisualPtr;
typedef struct _Depth *DepthPtr;
typedef struct _Screen *ScreenPtr;

extern _X_EXPORT int AddScreen(Bool (* /*pfnInit */ )(
        ScreenPtr /*pScreen */
        ,
        int /*argc */ ,
        char ** /*argv */ ),
                               int /*argc */ ,
                               char ** /*argv */ );


extern _X_EXPORT int AddGPUScreen(Bool (*pfnInit) (ScreenPtr /*pScreen */ ,
                                                   int /*argc */ ,
                                                   char **      /*argv */
),
                                  int argc, char **argv);

extern _X_EXPORT void RemoveGPUScreen(ScreenPtr pScreen);

extern _X_EXPORT void
AttachUnboundGPU(ScreenPtr pScreen, ScreenPtr newScreen);
extern _X_EXPORT void
DetachUnboundGPU(ScreenPtr unbound);

extern _X_EXPORT void
AttachOutputGPU(ScreenPtr pScreen, ScreenPtr newScreen);

extern _X_EXPORT void
DetachOutputGPU(ScreenPtr output);

extern _X_EXPORT void
AttachOffloadGPU(ScreenPtr pScreen, ScreenPtr newScreen);

extern _X_EXPORT void
DetachOffloadGPU(ScreenPtr slave);

typedef struct _ColormapRec *ColormapPtr;

#endif                          /* SCREENINT_H */