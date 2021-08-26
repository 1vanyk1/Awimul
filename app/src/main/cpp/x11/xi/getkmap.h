#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GETKMAP_H
#define GETKMAP_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXGetDeviceKeyMapping(ClientPtr /* client */
);

int ProcXGetDeviceKeyMapping(ClientPtr  /* client */
);

void SRepXGetDeviceKeyMapping(ClientPtr /* client */ ,
                              int /* size */ ,
                              xGetDeviceKeyMappingReply *       /* rep */
);

#endif                          /* GETKMAP_H */
