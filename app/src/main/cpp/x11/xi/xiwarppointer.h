#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef WARPDEVP_H
#define WARPDEVP_H 1

#include "../headers/xdefs.h"

int SProcXIWarpPointer(ClientPtr /* client */ );
int ProcXIWarpPointer(ClientPtr /* client */ );

#endif                          /* WARPDEVP_H */
