#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef SETFOCUS_H
#define SETFOCUS_H 1

#include "../headers/xdefs.h"

int SProcXSetDeviceFocus(ClientPtr      /* client */
);

int ProcXSetDeviceFocus(ClientPtr       /* client */
);

#endif                          /* SETFOCUS_H */
