#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _COMPOSITEEXT_H_
#define _COMPOSITEEXT_H_

#include "../headers/misc.h"
#include "../headers/scrnintstr.h"

extern _X_EXPORT Bool CompositeRegisterAlternateVisuals(ScreenPtr pScreen,
VisualID * vids,
int nVisuals);

extern _X_EXPORT Bool CompositeRegisterImplicitRedirectionException(ScreenPtr pScreen,
VisualID parentVisual,
        VisualID winVisual);


extern _X_EXPORT Bool compIsAlternateVisual(ScreenPtr pScreen, XID visual);
extern _X_EXPORT RESTYPE CompositeClientWindowType;

#endif                          /* _COMPOSITEEXT_H_ */
