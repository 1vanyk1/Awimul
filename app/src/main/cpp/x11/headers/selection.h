#ifndef SELECTION_H
#define SELECTION_H 1

#include "dixstruct.h"
#include "privates.h"

/*
 *  Selection data structures
 */

typedef struct _Selection {
    Atom selection;
    TimeStamp lastTimeChanged;
    Window window;
    WindowPtr pWin;
    ClientPtr client;
    struct _Selection *next;
    PrivateRec *devPrivates;
} Selection;

/*
 *  Selection API
 */

extern _X_EXPORT int dixLookupSelection(Selection ** result, Atom name,
                                        ClientPtr client, Mask access_mode);

extern _X_EXPORT Selection *CurrentSelections;

extern _X_EXPORT CallbackListPtr SelectionCallback;

typedef enum {
    SelectionSetOwner,
    SelectionWindowDestroy,
    SelectionClientClose
} SelectionCallbackKind;

typedef struct {
    struct _Selection *selection;
    ClientPtr client;
    SelectionCallbackKind kind;
} SelectionInfoRec;

/*
 *  Selection server internals
 */

extern _X_EXPORT void InitSelections(void);

extern _X_EXPORT void DeleteWindowFromAnySelections(WindowPtr pWin);

extern _X_EXPORT void DeleteClientFromAnySelections(ClientPtr client);

#endif                          /* SELECTION_H */