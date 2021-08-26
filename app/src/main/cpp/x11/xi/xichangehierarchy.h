#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef CHDEVHIER_H
#define CHDEVHIER_H 1

#include "../headers/xdefs.h"

int SProcXIChangeHierarchy(ClientPtr /* client */ );
int ProcXIChangeHierarchy(ClientPtr /* client */ );

void XISendDeviceHierarchyEvent(int flags[]);

#endif                          /* CHDEVHIER_H */