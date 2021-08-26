#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GETMMAP_H
#define GETMMAP_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXGetDeviceModifierMapping(ClientPtr    /* client */
);

int ProcXGetDeviceModifierMapping(ClientPtr     /* client */
);

void SRepXGetDeviceModifierMapping(ClientPtr /* client */ ,
                                   int /* size */ ,
                                   xGetDeviceModifierMappingReply *     /* rep */
);

#endif                          /* GETMMAP_H */
