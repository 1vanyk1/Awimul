#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef SETBMAP_H
#define SETBMAP_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXSetDeviceButtonMapping(ClientPtr      /* client */
);

int ProcXSetDeviceButtonMapping(ClientPtr       /* client */
);

void SRepXSetDeviceButtonMapping(ClientPtr /* client */ ,
                                 int /* size */ ,
                                 xSetDeviceButtonMappingReply * /* rep */
);

#endif                          /* SETBMAP_H */
