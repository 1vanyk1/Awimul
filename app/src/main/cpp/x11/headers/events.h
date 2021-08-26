#ifndef EVENTS_H
#define EVENTS_H
typedef struct _DeviceEvent DeviceEvent;
typedef struct _DeviceChangedEvent DeviceChangedEvent;
typedef struct _TouchOwnershipEvent TouchOwnershipEvent;
typedef struct _BarrierEvent BarrierEvent;
typedef struct _GestureEvent GestureEvent;

typedef struct _DGAEvent DGAEvent;
typedef struct _RawDeviceEvent RawDeviceEvent;

#ifdef XQUARTZ
typedef struct _XQuartzEvent XQuartzEvent;
#endif
typedef union _InternalEvent InternalEvent;

#endif
