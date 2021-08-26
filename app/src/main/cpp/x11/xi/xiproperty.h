#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef XIPROPERTY_H
#define XIPROPERTY_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/extensions/XI2proto.h"

int ProcXListDeviceProperties(ClientPtr client);
int ProcXChangeDeviceProperty(ClientPtr client);
int ProcXDeleteDeviceProperty(ClientPtr client);
int ProcXGetDeviceProperty(ClientPtr client);

/* request swapping */
int SProcXListDeviceProperties(ClientPtr client);
int SProcXChangeDeviceProperty(ClientPtr client);
int SProcXDeleteDeviceProperty(ClientPtr client);
int SProcXGetDeviceProperty(ClientPtr client);

/* reply swapping */
void SRepXListDeviceProperties(ClientPtr client, int size,
                               xListDevicePropertiesReply * rep);
void SRepXGetDeviceProperty(ClientPtr client, int size,
                            xGetDevicePropertyReply * rep);

/* XI2 request/reply handling  */
int ProcXIListProperties(ClientPtr client);
int ProcXIChangeProperty(ClientPtr client);
int ProcXIDeleteProperty(ClientPtr client);
int ProcXIGetProperty(ClientPtr client);

int SProcXIListProperties(ClientPtr client);
int SProcXIChangeProperty(ClientPtr client);
int SProcXIDeleteProperty(ClientPtr client);
int SProcXIGetProperty(ClientPtr client);

void SRepXIListProperties(ClientPtr client, int size,
                          xXIListPropertiesReply * rep);
void SRepXIGetProperty(ClientPtr client, int size, xXIGetPropertyReply * rep);

void XIResetProperties(void);

#endif                          /* XIPROPERTY_H */