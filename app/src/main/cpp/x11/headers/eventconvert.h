#ifndef _EVENTCONVERT_H_
#include "x.h"
#include "extensions/XIproto.h"
#include "input.h"
#include "events.h"
#include "eventstr.h"

_X_EXPORT int EventToCore(InternalEvent *event, xEvent **core, int *count);
_X_EXPORT int EventToXI(InternalEvent *ev, xEvent **xi, int *count);
_X_EXPORT int EventToXI2(InternalEvent *ev, xEvent **xi);
_X_INTERNAL int GetCoreType(enum EventType type);
_X_INTERNAL int GetXIType(enum EventType type);
_X_INTERNAL int GetXI2Type(enum EventType type);

_X_INTERNAL enum EventType GestureTypeToBegin(enum EventType type);
_X_INTERNAL enum EventType GestureTypeToEnd(enum EventType type);

#endif                          /* _EVENTCONVERT_H_ */
