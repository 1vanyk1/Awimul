#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef SENDEXEV_H
#define SENDEXEV_H 1

#include "../headers/xdefs.h"

int SProcXSendExtensionEvent(ClientPtr  /* client */
);

int ProcXSendExtensionEvent(ClientPtr   /* client */
);

#endif                          /* SENDEXEV_H */
