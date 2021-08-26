#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef SETMODE_H
#define SETMODE_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXSetDeviceMode(ClientPtr       /* client */
);

int ProcXSetDeviceMode(ClientPtr        /* client */
);

void SRepXSetDeviceMode(ClientPtr /* client */ ,
                        int /* size */ ,
                        xSetDeviceModeReply *   /* rep */
);

#endif                          /* SETMODE_H */
