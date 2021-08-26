#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef CHGDCTL_H
#define CHGDCTL_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXChangeDeviceControl(ClientPtr /* client */
);

int ProcXChangeDeviceControl(ClientPtr  /* client */
);

void SRepXChangeDeviceControl(ClientPtr /* client */ ,
                              int /* size */ ,
                              xChangeDeviceControlReply *       /* rep */
);

#endif                          /* CHGDCTL_H */
