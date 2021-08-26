#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef LISTDEV_H
#define LISTDEV_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

#define VPC	20              /* Max # valuators per chunk */

int SProcXListInputDevices(ClientPtr    /* client */
);

int ProcXListInputDevices(ClientPtr     /* client */
);

void SRepXListInputDevices(ClientPtr /* client */ ,
                           int /* size */ ,
                           xListInputDevicesReply *     /* rep */
);

#endif                          /* LISTDEV_H */
