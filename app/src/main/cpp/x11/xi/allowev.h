#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef ALLOWEV_H
#define ALLOWEV_H 1

#include "../headers/xdefs.h"

int SProcXAllowDeviceEvents(ClientPtr   /* client */
);

int ProcXAllowDeviceEvents(ClientPtr    /* client */
);

#endif                          /* ALLOWEV_H */
