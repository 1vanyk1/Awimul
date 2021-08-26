#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GETFOCUS_H
#define GETFOCUS_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXGetDeviceFocus(ClientPtr      /* client */
);

int ProcXGetDeviceFocus(ClientPtr       /* client */
);

void SRepXGetDeviceFocus(ClientPtr /* client */ ,
                         int /* size */ ,
                         xGetDeviceFocusReply * /* rep */
);

#endif                          /* GETFOCUS_H */
