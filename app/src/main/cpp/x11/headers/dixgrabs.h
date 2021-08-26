#ifndef DIXGRABS_H
#define DIXGRABS_H 1

#include "input.h"
#include "cursor.h"

struct _GrabParameters;

extern void PrintDeviceGrabInfo(DeviceIntPtr dev);
extern void UngrabAllDevices(Bool kill_client);

extern GrabPtr AllocGrab(const GrabPtr src);
extern void FreeGrab(GrabPtr grab);
extern Bool CopyGrab(GrabPtr dst, const GrabPtr src);

extern GrabPtr CreateGrab(int /* client */ ,
                          DeviceIntPtr /* device */ ,
                          DeviceIntPtr /* modDevice */ ,
                          WindowPtr /* window */ ,
                          enum InputLevel /* grabtype */ ,
                          GrabMask * /* mask */ ,
                          struct _GrabParameters * /* param */ ,
                          int /* type */ ,
                          KeyCode /* keybut */ ,
                          WindowPtr /* confineTo */ ,
                          CursorPtr /* cursor */ );

extern _X_EXPORT int DeletePassiveGrab(void *value,
                                       XID id);

extern _X_EXPORT Bool GrabMatchesSecond(GrabPtr /* pFirstGrab */ ,
        GrabPtr /* pSecondGrab */ ,
        Bool /*ignoreDevice */ );

extern _X_EXPORT int AddPassiveGrabToList(ClientPtr /* client */ ,
                                          GrabPtr /* pGrab */ );

extern _X_EXPORT Bool DeletePassiveGrabFromList(GrabPtr /* pMinuendGrab */ );

extern Bool GrabIsPointerGrab(GrabPtr grab);
extern Bool GrabIsKeyboardGrab(GrabPtr grab);
extern Bool GrabIsGestureGrab(GrabPtr grab);
#endif                          /* DIXGRABS_H */
