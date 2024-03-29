#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _MISYNCSTR_H_
#define _MISYNCSTR_H_

#include <stdint.h>
#include "../headers/dix.h"
#include "misync.h"
#include "../headers/scrnintstr.h"
#include "../headers/extensions/syncconst.h"

/* Sync object types */
#define SYNC_COUNTER		0
#define SYNC_FENCE		1

struct _SyncObject {
    ClientPtr client;           /* Owning client. 0 for system counters */
    struct _SyncTriggerList *pTriglist; /* list of triggers */
    XID id;                     /* resource ID */
    unsigned char type;         /* SYNC_* */
    unsigned char initialized;  /* FALSE if created but not initialized */
    Bool beingDestroyed;        /* in process of going away */
};

typedef struct _SyncCounter {
    SyncObject sync;            /* Common sync object data */
    int64_t value;              /* counter value */
    struct _SysCounterInfo *pSysCounterInfo; /* NULL if not a system counter */
} SyncCounter;

struct _SyncFence {
    SyncObject sync;            /* Common sync object data */
    ScreenPtr pScreen;          /* Screen of this fence object */
    SyncFenceFuncsRec funcs;    /* Funcs for performing ops on fence */
    Bool triggered;             /* fence state */
    PrivateRec *devPrivates;    /* driver-specific per-fence data */
};

struct _SyncTrigger {
    SyncObject *pSync;
    int64_t wait_value;         /* wait value */
    unsigned int value_type;    /* Absolute or Relative */
    unsigned int test_type;     /* transition or Comparison type */
    int64_t test_value;         /* trigger event threshold value */
    Bool (*CheckTrigger)(struct _SyncTrigger *pTrigger,
                         int64_t newval);
    void (*TriggerFired)(struct _SyncTrigger *pTrigger);
    void (*CounterDestroyed)(struct _SyncTrigger *pTrigger);
};

typedef struct _SyncTriggerList {
    SyncTrigger *pTrigger;
    struct _SyncTriggerList *next;
} SyncTriggerList;

extern DevPrivateKeyRec miSyncScreenPrivateKey;

#define SYNC_SCREEN_PRIV(pScreen) 				\
    (SyncScreenPrivPtr) dixLookupPrivate(&pScreen->devPrivates,	\
					 &miSyncScreenPrivateKey)

typedef struct _syncScreenPriv {
    /* Wrappable sync-specific screen functions */
    SyncScreenFuncsRec funcs;

    /* Wrapped screen functions */
    CloseScreenProcPtr CloseScreen;
} SyncScreenPrivRec, *SyncScreenPrivPtr;

#endif                          /* _MISYNCSTR_H_ */