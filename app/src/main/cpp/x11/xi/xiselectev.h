#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef XISELECTEVENTS_H
#define XISELECTEVENTS_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XI2proto.h"

int SProcXISelectEvents(ClientPtr client);
int ProcXISelectEvents(ClientPtr client);
int SProcXIGetSelectedEvents(ClientPtr client);
int ProcXIGetSelectedEvents(ClientPtr client);
void SRepXIGetSelectedEvents(ClientPtr client,
                             int len, xXIGetSelectedEventsReply * rep);

#endif                          /* _XISELECTEVENTS_H_ */
