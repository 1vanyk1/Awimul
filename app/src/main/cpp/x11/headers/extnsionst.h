#ifndef EXTENSIONSTRUCT_H
#define EXTENSIONSTRUCT_H

#include "dix.h"
#include "misc.h"
#include "screenint.h"
#include "extension.h"
#include "gc.h"
#include "privates.h"

typedef struct _ExtensionEntry {
    int index;
    void (*CloseDown) (         /* called at server shutdown */
            struct _ExtensionEntry * /* extension */ );
    const char *name;           /* extension name */
    int base;                   /* base request number */
    int eventBase;
    int eventLast;
    int errorBase;
    int errorLast;
    int num_aliases;
    const char **aliases;
    void *extPrivate;
    unsigned short (*MinorOpcode) (     /* called for errors */
            ClientPtr /* client */ );
    PrivateRec *devPrivates;
} ExtensionEntry;

/*
 * The arguments may be different for extension event swapping functions.
 * Deal with this by casting when initializing the event's EventSwapVector[]
 * entries.
 */
typedef void (*EventSwapPtr) (xEvent *, xEvent *);

extern _X_EXPORT EventSwapPtr EventSwapVector[128];

extern _X_EXPORT void
NotImplemented(                 /* FIXME: this may move to another file... */
        xEvent *, xEvent *) _X_NORETURN;

#define    SetGCVector(pGC, VectorElement, NewRoutineAddress, Atom)    \
    pGC->VectorElement = NewRoutineAddress;

#define    GetGCValue(pGC, GCElement)    (pGC->GCElement)

extern _X_EXPORT ExtensionEntry *
AddExtension(const char * /*name */ ,
             int /*NumEvents */ ,
             int /*NumErrors */ ,
             int (* /*MainProc */ )(ClientPtr /*client */ ),
             int (* /*SwappedMainProc */ )(ClientPtr /*client */ ),
             void (* /*CloseDownProc */ )(ExtensionEntry * /*extension */ ),
             unsigned short (* /*MinorOpcodeProc */ )(ClientPtr /*client */ )
);

extern _X_EXPORT Bool
AddExtensionAlias(const char * /*alias */ ,
                  ExtensionEntry * /*extension */ );

extern _X_EXPORT ExtensionEntry *
CheckExtension(const char *extname);
extern _X_EXPORT ExtensionEntry *
GetExtensionEntry(int major);

#endif                          /* EXTENSIONSTRUCT_H */