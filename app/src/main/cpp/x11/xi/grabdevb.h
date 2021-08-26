#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GRABDEVB_H
#define GRABDEVB_H 1

#include "../headers/xdefs.h"

int SProcXGrabDeviceButton(ClientPtr    /* client */
);

int ProcXGrabDeviceButton(ClientPtr     /* client */
);

#endif                          /* GRABDEVB_H */
