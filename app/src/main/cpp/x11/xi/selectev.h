#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef SELECTEV_H
#define SELECTEV_H 1

#include "../headers/xdefs.h"

int SProcXSelectExtensionEvent(ClientPtr        /* client */
);

int ProcXSelectExtensionEvent(ClientPtr /* client */
);

#endif                          /* SELECTEV_H */
