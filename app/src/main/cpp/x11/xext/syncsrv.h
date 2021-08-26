#ifndef _SYNCSRV_H_
#define _SYNCSRV_H_

#include "../headers/list.h"
#include "../mi/misync.h"
#include "../mi/misyncstr.h"

/*
 * The System Counter interface
 */

typedef enum {
    XSyncCounterNeverChanges,
    XSyncCounterNeverIncreases,
    XSyncCounterNeverDecreases,
    XSyncCounterUnrestricted
} SyncCounterType;

typedef void (*SyncSystemCounterQueryValue)(void *counter,
                                            int64_t *value_return
);
typedef void (*SyncSystemCounterBracketValues)(void *counter,
                                               int64_t *pbracket_less,
                                               int64_t *pbracket_greater
);

typedef struct _SysCounterInfo {
    SyncCounter *pCounter;
    char *name;
    int64_t resolution;
    int64_t bracket_greater;
    int64_t bracket_less;
    SyncCounterType counterType;        /* how can this counter change */
    SyncSystemCounterQueryValue QueryValue;
    SyncSystemCounterBracketValues BracketValues;
    void *private;
    struct xorg_list entry;
} SysCounterInfo;

typedef struct _SyncAlarmClientList {
    ClientPtr client;
    XID delete_id;
    struct _SyncAlarmClientList *next;
} SyncAlarmClientList;

typedef struct _SyncAlarm {
    SyncTrigger trigger;
    ClientPtr client;
    XSyncAlarm alarm_id;
    int64_t delta;
    int events;
    int state;
    SyncAlarmClientList *pEventClients;
} SyncAlarm;

typedef struct {
    ClientPtr client;
    CARD32 delete_id;
    int num_waitconditions;
} SyncAwaitHeader;

typedef struct {
    SyncTrigger trigger;
    int64_t event_threshold;
    SyncAwaitHeader *pHeader;
} SyncAwait;

typedef union {
    SyncAwaitHeader header;
    SyncAwait await;
} SyncAwaitUnion;

extern SyncCounter* SyncCreateSystemCounter(const char *name,
                                            int64_t initial_value,
                                            int64_t resolution,
                                            SyncCounterType counterType,
                                            SyncSystemCounterQueryValue QueryValue,
                                            SyncSystemCounterBracketValues BracketValues
);

extern void SyncChangeCounter(SyncCounter *pCounter,
                              int64_t new_value);

extern void SyncDestroySystemCounter(void *pCounter);

extern SyncCounter *SyncInitDeviceIdleTime(DeviceIntPtr dev);
extern void SyncRemoveDeviceIdleTime(SyncCounter *counter);

int
SyncCreateFenceFromFD(ClientPtr client, DrawablePtr pDraw, XID id, int fd, BOOL initially_triggered);

int
SyncFDFromFence(ClientPtr client, DrawablePtr pDraw, SyncFence *fence);

void
SyncDeleteTriggerFromSyncObject(SyncTrigger * pTrigger);

int
SyncAddTriggerToSyncObject(SyncTrigger * pTrigger);

#endif                          /* _SYNCSRV_H_ */