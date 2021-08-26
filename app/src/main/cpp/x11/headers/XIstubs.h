#ifndef XI_STUBS_H
#define XI_STUBS_H 1

#include "xlocale.h"
#include "xdefs.h"
#include "input.h"
#include "extensions/XIproto.h"

extern _X_EXPORT int
SetDeviceMode(ClientPtr /* client */ ,
              DeviceIntPtr /* dev */ ,
              int /* mode */ );

extern _X_EXPORT int
SetDeviceValuators(ClientPtr /* client */ ,
                   DeviceIntPtr /* dev */ ,
                   int * /* valuators */ ,
                   int /* first_valuator */ ,
                   int /* num_valuators */ );

extern _X_EXPORT int
ChangeDeviceControl(ClientPtr /* client */ ,
                    DeviceIntPtr /* dev */ ,
                    xDeviceCtl * /* control */ );

#endif                          /* XI_STUBS_H */
