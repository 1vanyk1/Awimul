#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef DEVBELL_H
#define DEVBELL_H 1

#include "../headers/xdefs.h"

int SProcXDeviceBell(ClientPtr  /* client */
);

int ProcXDeviceBell(ClientPtr   /* client */
);

#endif                          /* DEVBELL_H */
