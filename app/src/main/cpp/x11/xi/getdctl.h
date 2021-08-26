#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GETDCTL_H
#define GETDCTL_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXGetDeviceControl(ClientPtr    /* client */
);

int ProcXGetDeviceControl(ClientPtr     /* client */
);

void SRepXGetDeviceControl(ClientPtr /* client */ ,
                           int /* size */ ,
                           xGetDeviceControlReply *     /* rep */
);

#endif                          /* GETDCTL_H */
