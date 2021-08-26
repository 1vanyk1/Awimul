#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef SETMMAP_H
#define SETMMAP_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXSetDeviceModifierMapping(ClientPtr    /* client */
);

int ProcXSetDeviceModifierMapping(ClientPtr     /* client */
);

void SRepXSetDeviceModifierMapping(ClientPtr /* client */ ,
                                   int /* size */ ,
                                   xSetDeviceModifierMappingReply *     /* rep */
);

#endif                          /* SETMMAP_H */
