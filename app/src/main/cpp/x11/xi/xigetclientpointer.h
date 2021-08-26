#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GETCPTR_H
#define GETCPTR_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XI2proto.h"

int SProcXIGetClientPointer(ClientPtr /* client */ );
int ProcXIGetClientPointer(ClientPtr /* client */ );
void SRepXIGetClientPointer(ClientPtr /* client */ ,
                            int /* size */ ,
                            xXIGetClientPointerReply * /* rep */ );

#endif                          /* GETCPTR_H */
