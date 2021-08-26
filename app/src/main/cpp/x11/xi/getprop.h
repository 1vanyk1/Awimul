#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GETPROP_H
#define GETPROP_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/extensions/XI.h"

int SProcXGetDeviceDontPropagateList(ClientPtr  /* client */
);

int ProcXGetDeviceDontPropagateList(ClientPtr   /* client */
);

XEventClass *ClassFromMask(XEventClass * /* buf */ ,
                           Mask /* mask */ ,
                           int /* maskndx */ ,
                           CARD16 * /* count */ ,
                           int  /* mode */
);

void SRepXGetDeviceDontPropagateList(ClientPtr /* client */ ,
                                     int /* size */ ,
                                     xGetDeviceDontPropagateListReply * /* rep */
);

#endif                          /* GETPROP_H */