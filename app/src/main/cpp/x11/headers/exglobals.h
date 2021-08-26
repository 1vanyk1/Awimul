#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif
#include "privates.h"

#ifndef EXGLOBALS_H
#define EXGLOBALS_H 1

extern int IReqCode;
extern int IEventBase;
extern int BadDevice;
extern int BadMode;
extern int DeviceBusy;
extern int BadClass;

/* Note: only the ones needed in files other than extinit.c are declared */
extern const Mask DevicePointerMotionHintMask;
extern const Mask DeviceFocusChangeMask;
extern const Mask DeviceStateNotifyMask;
extern const Mask DeviceMappingNotifyMask;
extern const Mask DeviceOwnerGrabButtonMask;
extern const Mask DeviceButtonGrabMask;
extern const Mask DeviceButtonMotionMask;
extern const Mask DevicePresenceNotifyMask;
extern const Mask DevicePropertyNotifyMask;
extern const Mask XIAllMasks;

extern int DeviceValuator;
extern int DeviceKeyPress;
extern int DeviceKeyRelease;
extern int DeviceButtonPress;
extern int DeviceButtonRelease;
extern int DeviceMotionNotify;
extern int DeviceFocusIn;
extern int DeviceFocusOut;
extern int ProximityIn;
extern int ProximityOut;
extern int DeviceStateNotify;
extern int DeviceKeyStateNotify;
extern int DeviceButtonStateNotify;
extern int DeviceMappingNotify;
extern int ChangeDeviceNotify;
extern int DevicePresenceNotify;
extern int DevicePropertyNotify;

extern RESTYPE RT_INPUTCLIENT;

extern DevPrivateKeyRec XIClientPrivateKeyRec;

#define XIClientPrivateKey (&XIClientPrivateKeyRec)

#endif                          /* EXGLOBALS_H */