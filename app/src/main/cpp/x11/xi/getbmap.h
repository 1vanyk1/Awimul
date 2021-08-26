#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GETBMAP_H
#define GETBMAP_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXGetDeviceButtonMapping(ClientPtr      /* client */
);

int ProcXGetDeviceButtonMapping(ClientPtr       /* client */
);

void SRepXGetDeviceButtonMapping(ClientPtr /* client */ ,
                                 int /* size */ ,
                                 xGetDeviceButtonMappingReply * /* rep */
);

#endif                          /* GETBMAP_H */
