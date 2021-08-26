#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef SETDVAL_H
#define SETDVAL_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXSetDeviceValuators(ClientPtr  /* client */
);

int ProcXSetDeviceValuators(ClientPtr   /* client */
);

void SRepXSetDeviceValuators(ClientPtr /* client */ ,
                             int /* size */ ,
                             xSetDeviceValuatorsReply * /* rep */
);

#endif                          /* SETDVAL_H */
