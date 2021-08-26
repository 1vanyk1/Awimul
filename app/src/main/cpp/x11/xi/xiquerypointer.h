#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef QUERYDP_H
#define QUERYDP_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XI2proto.h"

int SProcXIQueryPointer(ClientPtr /* client */ );
int ProcXIQueryPointer(ClientPtr /* client */ );
void SRepXIQueryPointer(ClientPtr /* client */ ,
                        int /* size */ ,
                        xXIQueryPointerReply * /* rep */ );

#endif                          /* QUERYDP_H */
