#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef XISETDEVFOCUS_H
#define XISETDEVFOCUS_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XI2proto.h"

int SProcXISetFocus(ClientPtr client);
int ProcXISetFocus(ClientPtr client);

int SProcXIGetFocus(ClientPtr client);
int ProcXIGetFocus(ClientPtr client);

void SRepXIGetFocus(ClientPtr client, int len, xXIGetFocusReply * rep);
#endif                          /* XISETDEVFOCUS_H */
