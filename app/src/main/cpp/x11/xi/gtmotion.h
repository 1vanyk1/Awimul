#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GTMOTION_H
#define GTMOTION_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXGetDeviceMotionEvents(ClientPtr       /* client */
);

int ProcXGetDeviceMotionEvents(ClientPtr        /* client */
);

void SRepXGetDeviceMotionEvents(ClientPtr /* client */ ,
                                int /* size */ ,
                                xGetDeviceMotionEventsReply *   /* rep */
);

#endif                          /* GTMOTION_H */
