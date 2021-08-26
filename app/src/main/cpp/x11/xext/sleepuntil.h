#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _SLEEPUNTIL_H_
#define _SLEEPUNTIL_H_ 1

#include "../headers/dix.h"

extern int ClientSleepUntil(ClientPtr client,
                            TimeStamp *revive,
                            void (*notifyFunc) (ClientPtr /* client */ ,
                                                void *    /* closure */
                            ), void *Closure);

#endif
