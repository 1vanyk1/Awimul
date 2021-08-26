#include "../../headers/region.h"
#include "../../headers/pixmap.h"

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _DAMAGE_H_
#define _DAMAGE_H_

typedef struct _damage *DamagePtr;

typedef enum _damageReportLevel {
    DamageReportRawRegion,
    DamageReportDeltaRegion,
    DamageReportBoundingBox,
    DamageReportNonEmpty,
    DamageReportNone
} DamageReportLevel;

typedef void (*DamageReportFunc) (DamagePtr pDamage, RegionPtr pRegion,
                                  void *closure);
typedef void (*DamageDestroyFunc) (DamagePtr pDamage, void *closure);

typedef void (*DamageScreenCreateFunc) (DamagePtr);
typedef void (*DamageScreenRegisterFunc) (DrawablePtr, DamagePtr);
typedef void (*DamageScreenUnregisterFunc) (DrawablePtr, DamagePtr);
typedef void (*DamageScreenDestroyFunc) (DamagePtr);

typedef struct _damageScreenFuncs {
    DamageScreenCreateFunc Create;
    DamageScreenRegisterFunc Register;
    DamageScreenUnregisterFunc Unregister;
    DamageScreenDestroyFunc Destroy;
} DamageScreenFuncsRec, *DamageScreenFuncsPtr;

extern _X_EXPORT void miDamageCreate(DamagePtr);
extern _X_EXPORT void miDamageRegister(DrawablePtr, DamagePtr);
extern _X_EXPORT void miDamageUnregister(DrawablePtr, DamagePtr);
extern _X_EXPORT void miDamageDestroy(DamagePtr);

extern _X_EXPORT Bool
DamageSetup(ScreenPtr pScreen);

extern _X_EXPORT DamagePtr
DamageCreate(DamageReportFunc damageReport,
DamageDestroyFunc damageDestroy,
        DamageReportLevel damageLevel,
Bool isInternal, ScreenPtr pScreen, void *closure);

extern _X_EXPORT void
        DamageDrawInternal(ScreenPtr pScreen, Bool enable);

extern _X_EXPORT void
DamageRegister(DrawablePtr pDrawable, DamagePtr pDamage);

extern _X_EXPORT void
DamageUnregister(DamagePtr pDamage);

extern _X_EXPORT void
DamageDestroy(DamagePtr pDamage);

extern _X_EXPORT Bool
DamageSubtract(DamagePtr pDamage, const RegionPtr pRegion);

extern _X_EXPORT void
DamageEmpty(DamagePtr pDamage);

extern _X_EXPORT RegionPtr
DamageRegion(DamagePtr pDamage);

extern _X_EXPORT RegionPtr
DamagePendingRegion(DamagePtr pDamage);

/* In case of rendering, call this before the submitting the commands. */
extern _X_EXPORT void
DamageRegionAppend(DrawablePtr pDrawable, RegionPtr pRegion);

/* Call this directly after the rendering operation has been submitted. */
extern _X_EXPORT void
DamageRegionProcessPending(DrawablePtr pDrawable);

/* Call this when you create a new Damage and you wish to send an initial damage message (to it). */
extern _X_EXPORT void
DamageReportDamage(DamagePtr pDamage, RegionPtr pDamageRegion);

/* Avoid using this call, it only exists for API compatibility. */
extern _X_EXPORT void
DamageDamageRegion(DrawablePtr pDrawable, const RegionPtr pRegion);

extern _X_EXPORT void
DamageSetReportAfterOp(DamagePtr pDamage, Bool reportAfter);

extern _X_EXPORT DamageScreenFuncsPtr DamageGetScreenFuncs(ScreenPtr);

#endif                          /* _DAMAGE_H_ */