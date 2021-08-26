#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "../../headers/scrnintstr.h"

#include "../../headers/picturestr.h"

#include "../damage/damage.h"
#include "../damage/damagestr.h"
typedef struct _shadowBuf *shadowBufPtr;

typedef void (*ShadowUpdateProc) (ScreenPtr pScreen, shadowBufPtr pBuf);

#define SHADOW_WINDOW_RELOCATE 1
#define SHADOW_WINDOW_READ 2
#define SHADOW_WINDOW_WRITE 4

typedef void *(*ShadowWindowProc) (ScreenPtr pScreen,
                                   CARD32 row,
                                   CARD32 offset,
                                   int mode, CARD32 *size, void *closure);

typedef struct _shadowBuf {
    DamagePtr pDamage;
    ShadowUpdateProc update;
    ShadowWindowProc window;
    PixmapPtr pPixmap;
    void *closure;
    int randr;

    /* screen wrappers */
    GetImageProcPtr GetImage;
    CloseScreenProcPtr CloseScreen;
    ScreenBlockHandlerProcPtr BlockHandler;
} shadowBufRec;

/* Match defines from randr extension */
#define SHADOW_ROTATE_0	    1
#define SHADOW_ROTATE_90    2
#define SHADOW_ROTATE_180   4
#define SHADOW_ROTATE_270   8
#define SHADOW_ROTATE_ALL   (SHADOW_ROTATE_0|SHADOW_ROTATE_90|\
			     SHADOW_ROTATE_180|SHADOW_ROTATE_270)
#define SHADOW_REFLECT_X    16
#define SHADOW_REFLECT_Y    32
#define SHADOW_REFLECT_ALL  (SHADOW_REFLECT_X|SHADOW_REFLECT_Y)

extern _X_EXPORT Bool
shadowSetup(ScreenPtr pScreen);

extern _X_EXPORT Bool

shadowAdd(ScreenPtr pScreen,
PixmapPtr pPixmap,
        ShadowUpdateProc update,
ShadowWindowProc window, int randr, void *closure);

extern _X_EXPORT void
        shadowRemove(ScreenPtr pScreen, PixmapPtr pPixmap);

extern _X_EXPORT void
        shadowUpdateAfb4(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateAfb8(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateIplan2p4(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateIplan2p8(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdatePacked(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdatePlanar4(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdatePlanar4x8(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotatePacked(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate8_90(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate16_90(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate16_90YX(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate32_90(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate8_180(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate16_180(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate32_180(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate8_270(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate16_270(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate16_270YX(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate32_270(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate8(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate16(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdateRotate32(ScreenPtr pScreen, shadowBufPtr pBuf);

extern _X_EXPORT void
        shadowUpdate32to24(ScreenPtr pScreen, shadowBufPtr pBuf);

typedef void (*shadowUpdateProc) (ScreenPtr, shadowBufPtr);

#endif                          /* _SHADOW_H_ */
