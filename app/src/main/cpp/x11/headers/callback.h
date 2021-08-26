#ifndef CALLBACK_H
#define CALLBACK_H

#include "x.h"              /* for GContext, Mask */
#include "xdefs.h"          /* for Bool */
#include "xproto.h"
#include "xfuncproto.h"

/*
 *  callback manager stuff
 */

#ifndef _XTYPEDEF_CALLBACKLISTPTR
typedef struct _CallbackList *CallbackListPtr;  /* also in misc.h */

#define _XTYPEDEF_CALLBACKLISTPTR
#endif

typedef void (*CallbackProcPtr) (CallbackListPtr *, void *, void *);

extern _X_EXPORT Bool AddCallback(CallbackListPtr *pcbl,
CallbackProcPtr callback,
void *data);

extern _X_EXPORT Bool DeleteCallback(CallbackListPtr *pcbl,
CallbackProcPtr callback,
void *data);

extern _X_EXPORT void _CallCallbacks(CallbackListPtr *pcbl,
                                     void *call_data);

static inline void
CallCallbacks(CallbackListPtr *pcbl, void *call_data)
{
    if (!pcbl || !*pcbl)
        return;
    _CallCallbacks(pcbl, call_data);
}

extern _X_EXPORT void DeleteCallbackList(CallbackListPtr *pcbl);

extern _X_EXPORT void InitCallbackManager(void);
extern _X_EXPORT void DeleteCallbackManager(void);

#endif                          /* CALLBACK_H */