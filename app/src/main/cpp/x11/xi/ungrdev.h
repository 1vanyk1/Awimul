#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef UNGRDEV_H
#define UNGRDEV_H 1

#include "../headers/xdefs.h"

int SProcXUngrabDevice(ClientPtr        /* client */
);

int ProcXUngrabDevice(ClientPtr /* client */
);

#endif                          /* UNGRDEV_H */
