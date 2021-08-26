#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef OPENDEV_H
#define OPENDEV_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXOpenDevice(ClientPtr  /* client */
);

int ProcXOpenDevice(ClientPtr   /* client */
);

void SRepXOpenDevice(ClientPtr /* client */ ,
                     int /* size */ ,
                     xOpenDeviceReply * /* rep */
);

#endif                          /* OPENDEV_H */
