#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef CHGKMAP_H
#define CHGKMAP_H 1

#include "../headers/xdefs.h"

int SProcXChangeDeviceKeyMapping(ClientPtr      /* client */
);

int ProcXChangeDeviceKeyMapping(ClientPtr       /* client */
);

#endif                          /* CHGKMAP_H */
