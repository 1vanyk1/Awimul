#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../libXt/Intrinsic.h"
#include "Converters.h"
#include "CharSet.h"

/*
 * Prototypes
 */
static void InitializeQuarks(void);

/*
 * Initialization
 */
static XrmQuark QnotUseful, QwhenMapped, Qalways, Qdefault;
static Boolean haveQuarks;

/*
 * Implementation
 */
static void
InitializeQuarks(void)
{
    if (!haveQuarks)
    {
        char name[11];

        XmuNCopyISOLatin1Lowered(name, XtEnotUseful, sizeof(name));
        QnotUseful = XrmStringToQuark(name);
        XmuNCopyISOLatin1Lowered(name, XtEwhenMapped, sizeof(name));
        QwhenMapped = XrmStringToQuark(name);
        XmuNCopyISOLatin1Lowered(name, XtEalways, sizeof(name));
        Qalways = XrmStringToQuark(name);
        XmuNCopyISOLatin1Lowered(name, XtEdefault, sizeof(name));
        Qdefault = XrmStringToQuark(name);
        haveQuarks = True;
    }
}

/*ARGSUSED*/
void
XmuCvtStringToBackingStore(XrmValue *args, Cardinal *num_args,
                           XrmValuePtr fromVal, XrmValuePtr toVal)
{
    XrmQuark	q;
    char name[11];
    static int	backingStoreType;

    if (*num_args != 0)
        XtWarning("String to BackingStore conversion needs no extra arguments");

    InitializeQuarks();
    XmuNCopyISOLatin1Lowered(name, (char *)fromVal->addr, sizeof(name));

    q = XrmStringToQuark (name);
    if (q == QnotUseful)
        backingStoreType = NotUseful;
    else if (q == QwhenMapped)
        backingStoreType = WhenMapped;
    else if (q == Qalways)
        backingStoreType = Always;
    else if (q == Qdefault)
        backingStoreType = Always + WhenMapped + NotUseful;
    else
    {
        XtStringConversionWarning((char *)fromVal->addr, XtRBackingStore);
        return;
    }
    toVal->size = sizeof(int);
    toVal->addr = (XPointer)&backingStoreType;
}

/*ARGSUSED*/
Boolean
XmuCvtBackingStoreToString(Display *dpy, XrmValuePtr args, Cardinal *num_args,
                           XrmValuePtr fromVal, XrmValuePtr toVal,
                           XtPointer *data)
{
    static String buffer;
    Cardinal size;

    switch (*(int *)fromVal->addr)
    {
        case NotUseful:
            buffer = XtEnotUseful;
            break;
        case WhenMapped:
            buffer = XtEwhenMapped;
            break;
        case Always:
            buffer = XtEalways;
            break;
        case (Always + WhenMapped + NotUseful):
            buffer = XtEdefault;
            break;
        default:
            XtWarning("Cannot convert BackingStore to String");
            toVal->addr = NULL;
            toVal->size = 0;
            return (False);
    }

    size = strlen(buffer) + 1;
    if (toVal->addr != NULL)
    {
        if (toVal->size < size)
        {
            toVal->size = size;
            return (False);
        }
        strcpy((char *)toVal->addr, buffer);
    }
    else
        toVal->addr = (XPointer)buffer;
    toVal->size = sizeof(String);

    return (True);
}
