#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _MISYNC_H_
#define _MISYNC_H_

typedef struct _SyncObject SyncObject;
typedef struct _SyncFence SyncFence;
typedef struct _SyncTrigger SyncTrigger;

typedef void (*SyncScreenCreateFenceFunc) (ScreenPtr pScreen,
                                           SyncFence * pFence,
                                           Bool initially_triggered);
typedef void (*SyncScreenDestroyFenceFunc) (ScreenPtr pScreen,
                                            SyncFence * pFence);

typedef struct _syncScreenFuncs {
    SyncScreenCreateFenceFunc CreateFence;
    SyncScreenDestroyFenceFunc DestroyFence;
} SyncScreenFuncsRec, *SyncScreenFuncsPtr;


extern _X_EXPORT void
miSyncScreenCreateFence(ScreenPtr pScreen, SyncFence * pFence,
                        Bool initially_triggered);
extern _X_EXPORT void
miSyncScreenDestroyFence(ScreenPtr pScreen, SyncFence * pFence);

typedef void (*SyncFenceSetTriggeredFunc) (SyncFence * pFence);
typedef void (*SyncFenceResetFunc) (SyncFence * pFence);
typedef Bool (*SyncFenceCheckTriggeredFunc) (SyncFence * pFence);
typedef void (*SyncFenceAddTriggerFunc) (SyncTrigger * pTrigger);
typedef void (*SyncFenceDeleteTriggerFunc) (SyncTrigger * pTrigger);

typedef struct _syncFenceFuncs {
    SyncFenceSetTriggeredFunc SetTriggered;
    SyncFenceResetFunc Reset;
    SyncFenceCheckTriggeredFunc CheckTriggered;
    SyncFenceAddTriggerFunc AddTrigger;
    SyncFenceDeleteTriggerFunc DeleteTrigger;
} SyncFenceFuncsRec, *SyncFenceFuncsPtr;

extern _X_EXPORT void

miSyncInitFence(ScreenPtr pScreen, SyncFence * pFence,
                Bool initially_triggered);
extern _X_EXPORT void
miSyncDestroyFence(SyncFence * pFence);
extern _X_EXPORT void
miSyncTriggerFence(SyncFence * pFence);

extern _X_EXPORT SyncScreenFuncsPtr miSyncGetScreenFuncs(ScreenPtr pScreen);
extern _X_EXPORT Bool
miSyncSetup(ScreenPtr pScreen);

Bool
miSyncFenceCheckTriggered(SyncFence * pFence);

void
miSyncFenceSetTriggered(SyncFence * pFence);

void
miSyncFenceReset(SyncFence * pFence);

void
miSyncFenceAddTrigger(SyncTrigger * pTrigger);

void
miSyncFenceDeleteTrigger(SyncTrigger * pTrigger);

int
miSyncInitFenceFromFD(DrawablePtr pDraw, SyncFence *pFence, int fd, BOOL initially_triggered);

int
miSyncFDFromFence(DrawablePtr pDraw, SyncFence *pFence);

#include "../headers/scrnintstr.h"
//#include "misync.h"
#include "misyncstr.h"

#endif                          /* _MISYNC_H_ */