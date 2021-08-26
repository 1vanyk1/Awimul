#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef UNGRDEVK_H
#define UNGRDEVK_H 1

#include "../headers/xdefs.h"

int SProcXUngrabDeviceKey(ClientPtr     /* client */
);

int ProcXUngrabDeviceKey(ClientPtr      /* client */
);

#endif                          /* UNGRDEVK_H */
