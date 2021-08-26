#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GRABDEV_H
#define GRABDEV_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/extensions/XI.h"
#include "../headers/input.h"

int SProcXGrabDevice(ClientPtr  /* client */
);

int ProcXGrabDevice(ClientPtr   /* client */
);

int CreateMaskFromList(ClientPtr /* client */ ,
                       XEventClass * /* list */ ,
                       int /* count */ ,
                       struct tmask /* mask */ [],
                       DeviceIntPtr /* dev */ ,
                       int      /* req */
);

void SRepXGrabDevice(ClientPtr /* client */ ,
                     int /* size */ ,
                     xGrabDeviceReply * /* rep */
);

#endif                          /* GRABDEV_H */
