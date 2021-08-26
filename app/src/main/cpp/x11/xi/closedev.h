#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef CLOSEDEV_H
#define CLOSEDEV_H 1

#include "../headers/xdefs.h"

int SProcXCloseDevice(ClientPtr /* client */
);

int ProcXCloseDevice(ClientPtr  /* client */
);

#endif                          /* CLOSEDEV_H */
