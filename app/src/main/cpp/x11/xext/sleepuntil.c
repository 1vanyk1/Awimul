#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "sleepuntil.h"
#include "../headers/x.h"
#include "../headers/xmd.h"
#include "../headers/misc.h"
#include "../headers/windowstr.h"
#include "../headers/dixstruct.h"
#include "../headers/pixmapstr.h"
#include "../headers/scrnintstr.h"

typedef struct _Sertafied {
    struct _Sertafied *next;
    TimeStamp revive;
    ClientPtr pClient;
    XID id;
    void (*notifyFunc) (ClientPtr /* client */ ,
                        void *    /* closure */
    );

    void *closure;
} SertafiedRec, *SertafiedPtr;

static SertafiedPtr pPending;
static RESTYPE SertafiedResType;
static Bool BlockHandlerRegistered;
static int SertafiedGeneration;

static void ClientAwaken(ClientPtr /* client */ ,
                         void *    /* closure */
);
static int SertafiedDelete(void *  /* value */ ,
                           XID     /* id */
);
static void SertafiedBlockHandler(void *data,
                                  void *timeout);

static void SertafiedWakeupHandler(void *data,
                                   int i);

int
ClientSleepUntil(ClientPtr client,
                 TimeStamp *revive,
                 void (*notifyFunc) (ClientPtr, void *), void *closure)
{
    SertafiedPtr pRequest, pReq, pPrev;

    if (SertafiedGeneration != serverGeneration) {
        SertafiedResType = CreateNewResourceType(SertafiedDelete,
                                                 "ClientSleep");
        if (!SertafiedResType)
            return FALSE;
        SertafiedGeneration = serverGeneration;
        BlockHandlerRegistered = FALSE;
    }
    pRequest = malloc(sizeof(SertafiedRec));
    if (!pRequest)
        return FALSE;
    pRequest->pClient = client;
    pRequest->revive = *revive;
    pRequest->id = FakeClientID(client->index);
    pRequest->closure = closure;
    if (!BlockHandlerRegistered) {
        if (!RegisterBlockAndWakeupHandlers(SertafiedBlockHandler,
                                            SertafiedWakeupHandler,
                                            (void *) 0)) {
            free(pRequest);
            return FALSE;
        }
        BlockHandlerRegistered = TRUE;
    }
    pRequest->notifyFunc = 0;
    if (!AddResource(pRequest->id, SertafiedResType, (void *) pRequest))
        return FALSE;
    if (!notifyFunc)
        notifyFunc = ClientAwaken;
    pRequest->notifyFunc = notifyFunc;
    /* Insert into time-ordered queue, with earliest activation time coming first. */
    pPrev = 0;
    for (pReq = pPending; pReq; pReq = pReq->next) {
        if (CompareTimeStamps(pReq->revive, *revive) == LATER)
            break;
        pPrev = pReq;
    }
    if (pPrev)
        pPrev->next = pRequest;
    else
        pPending = pRequest;
    pRequest->next = pReq;
    IgnoreClient(client);
    return TRUE;
}

static void
ClientAwaken(ClientPtr client, void *closure)
{
    AttendClient(client);
}

static int
SertafiedDelete(void *value, XID id)
{
    SertafiedPtr pRequest = (SertafiedPtr) value;
    SertafiedPtr pReq, pPrev;

    pPrev = 0;
    for (pReq = pPending; pReq; pPrev = pReq, pReq = pReq->next)
        if (pReq == pRequest) {
            if (pPrev)
                pPrev->next = pReq->next;
            else
                pPending = pReq->next;
            break;
        }
    if (pRequest->notifyFunc)
        (*pRequest->notifyFunc) (pRequest->pClient, pRequest->closure);
    free(pRequest);
    return TRUE;
}

static void
SertafiedBlockHandler(void *data, void *wt)
{
    SertafiedPtr pReq, pNext;
    unsigned long delay;
    TimeStamp now;

    if (!pPending)
        return;
    now.milliseconds = GetTimeInMillis();
    now.months = currentTime.months;
    if ((int) (now.milliseconds - currentTime.milliseconds) < 0)
        now.months++;
    for (pReq = pPending; pReq; pReq = pNext) {
        pNext = pReq->next;
        if (CompareTimeStamps(pReq->revive, now) == LATER)
            break;
        FreeResource(pReq->id, RT_NONE);

        /* AttendClient() may have been called via the resource delete
         * function so a client may have input to be processed and so
         *  set delay to 0 to prevent blocking in WaitForSomething().
         */
        AdjustWaitForDelay(wt, 0);
    }
    pReq = pPending;
    if (!pReq)
        return;
    delay = pReq->revive.milliseconds - now.milliseconds;
    AdjustWaitForDelay(wt, delay);
}

static void
SertafiedWakeupHandler(void *data, int i)
{
    SertafiedPtr pReq, pNext;
    TimeStamp now;

    now.milliseconds = GetTimeInMillis();
    now.months = currentTime.months;
    if ((int) (now.milliseconds - currentTime.milliseconds) < 0)
        now.months++;
    for (pReq = pPending; pReq; pReq = pNext) {
        pNext = pReq->next;
        if (CompareTimeStamps(pReq->revive, now) == LATER)
            break;
        FreeResource(pReq->id, RT_NONE);
    }
    if (!pPending) {
        RemoveBlockAndWakeupHandlers(SertafiedBlockHandler,
                                     SertafiedWakeupHandler, (void *) 0);
        BlockHandlerRegistered = FALSE;
    }
}
