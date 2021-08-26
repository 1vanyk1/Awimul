#ifndef MIPOINTRST_H
#define MIPOINTRST_H

#include "mipointer.h"
#include "../headers/scrnintstr.h"

typedef struct {
    miPointerSpriteFuncPtr spriteFuncs; /* sprite-specific methods */
    miPointerScreenFuncPtr screenFuncs; /* screen-specific methods */
    CloseScreenProcPtr CloseScreen;
    Bool waitForUpdate;         /* don't move cursor from input thread */
    Bool showTransparent;       /* show empty cursors */
} miPointerScreenRec, *miPointerScreenPtr;
#endif                          /* MIPOINTRST_H */
