#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef CHGPROP_H
#define CHGPROP_H 1

#include "../headers/xdefs.h"

int SProcXChangeDeviceDontPropagateList(ClientPtr       /* client */
);

int ProcXChangeDeviceDontPropagateList(ClientPtr        /* client */
);

#endif                          /* CHGPROP_H */
