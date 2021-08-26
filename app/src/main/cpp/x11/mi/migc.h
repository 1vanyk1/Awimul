#ifndef _MIGC_H
#define _MIGC_H

#include "../headers/xlocale.h"
#include "../headers/gc.h"

extern _X_EXPORT void miChangeGC(GCPtr          pGC,
                                 unsigned long  mask);

extern _X_EXPORT void miDestroyGC(GCPtr         pGC);

extern _X_EXPORT void miDestroyClip(GCPtr       pGC);

extern _X_EXPORT void miChangeClip(GCPtr        pGC,
                                   int          type,
                                   void         *pvalue,
                                   int          nrects);

extern _X_EXPORT void miCopyClip(GCPtr          pgcDst,
                                 GCPtr          pgcSrc);

extern _X_EXPORT void miCopyGC(GCPtr            pGCSrc,
                               unsigned long    changes,
                               GCPtr            pGCDst);

extern _X_EXPORT void miComputeCompositeClip(GCPtr              pGC,
                                             DrawablePtr        pDrawable);

#endif
