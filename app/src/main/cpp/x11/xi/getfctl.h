#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef GETFCTL_H
#define GETFCTL_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XIproto.h"

int SProcXGetFeedbackControl(ClientPtr  /* client */
);

int ProcXGetFeedbackControl(ClientPtr   /* client */
);

void SRepXGetFeedbackControl(ClientPtr /* client */ ,
                             int /* size */ ,
                             xGetFeedbackControlReply * /* rep */
);

#endif                          /* GETFCTL_H */
