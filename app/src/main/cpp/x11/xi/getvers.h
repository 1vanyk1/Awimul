#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GETVERS_H
#define GETVERS_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXGetExtensionVersion(ClientPtr /* client */
);

int ProcXGetExtensionVersion(ClientPtr  /* client */
);

void SRepXGetExtensionVersion(ClientPtr /* client */ ,
                              int /* size */ ,
                              xGetExtensionVersionReply *       /* rep */
);

#endif                          /* GETVERS_H */
