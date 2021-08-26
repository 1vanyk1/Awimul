#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GETSELEV_H
#define GETSELEV_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXGetSelectedExtensionEvents(ClientPtr  /* client */
);

int ProcXGetSelectedExtensionEvents(ClientPtr   /* client */
);

void SRepXGetSelectedExtensionEvents(ClientPtr /* client */ ,
                                     int /* size */ ,
                                     xGetSelectedExtensionEventsReply * /* rep */
);

#endif                          /* GETSELEV_H */
