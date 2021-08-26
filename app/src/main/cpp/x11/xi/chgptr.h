#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef CHGPTR_H
#define CHGPTR_H 1

#include "../headers/xdefs.h"
#include "../headers/input.h"

int SProcXChangePointerDevice(ClientPtr /* client */
);

int ProcXChangePointerDevice(ClientPtr  /* client */
);

void DeleteFocusClassDeviceStruct(DeviceIntPtr  /* dev */
);

#endif                          /* CHGPTR_H */
