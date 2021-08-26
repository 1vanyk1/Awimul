#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GRABDEVK_H
#define GRABDEVK_H 1

#include "../headers/xdefs.h"

int SProcXGrabDeviceKey(ClientPtr       /* client */
);

int ProcXGrabDeviceKey(ClientPtr        /* client */
);

#endif                          /* GRABDEVK_H */
