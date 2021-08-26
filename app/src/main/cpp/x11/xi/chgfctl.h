#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef CHGFCTL_H
#define CHGFCTL_H 1

#include "../headers/xdefs.h"

int SProcXChangeFeedbackControl(ClientPtr       /* client */
);

int ProcXChangeFeedbackControl(ClientPtr        /* client */
);

#endif                          /* CHGFCTL_H */
