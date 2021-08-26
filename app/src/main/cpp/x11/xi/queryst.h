#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef QUERYST_H
#define QUERYST_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXQueryDeviceState(ClientPtr    /* client */
);

int ProcXQueryDeviceState(ClientPtr     /* client */
);

void SRepXQueryDeviceState(ClientPtr /* client */ ,
                           int /* size */ ,
                           xQueryDeviceStateReply *     /* rep */
);

#endif                          /* QUERYST_H */
