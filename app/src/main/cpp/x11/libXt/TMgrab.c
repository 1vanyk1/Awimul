#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"

typedef struct _GrabActionRec {
    struct _GrabActionRec *next;
    XtActionProc action_proc;
    Boolean owner_events;
    unsigned int event_mask;
    int pointer_mode, keyboard_mode;
} GrabActionRec;

static GrabActionRec *grabActionList = NULL;

static void
GrabAllCorrectKeys(Widget widget,
                   TMTypeMatch typeMatch,
                   TMModifierMatch modMatch, GrabActionRec * grabP)
{
    Display *dpy = XtDisplay(widget);
    KeyCode *keycodes, *keycodeP;
    Cardinal keycount;
    Modifiers careOn = 0;
    Modifiers careMask = 0;

    if (modMatch->lateModifiers) {
        Boolean resolved;

        resolved = _XtComputeLateBindings(dpy, modMatch->lateModifiers,
                                          &careOn, &careMask);
        if (!resolved)
            return;
    }
    careOn = (careOn | (Modifiers) modMatch->modifiers);
    careMask = (careMask | (Modifiers) modMatch->modifierMask);

    keycodes = NULL;
    XtKeysymToKeycodeList(dpy,
                          (KeySym) typeMatch->eventCode, &keycodes, &keycount);
    if (keycount == 0) {
        XtFree((char *) keycodes);
        return;
    }
    for (keycodeP = keycodes; keycount--; keycodeP++) {
        if (modMatch->standard) {
            /* find standard modifiers that produce this keysym */
            KeySym keysym;
            int std_mods, least_mod;
            Modifiers modifiers_return;

            XtTranslateKeycode(dpy, *keycodeP, (Modifiers) 0,
                               &modifiers_return, &keysym);
            if (careOn & modifiers_return) {
                XtFree((char *) keycodes);
                return;
            }
            if (keysym == typeMatch->eventCode) {
                XtGrabKey(widget, *keycodeP, careOn,
                          grabP->owner_events,
                          grabP->pointer_mode, grabP->keyboard_mode);
                /* continue; *//* grab all modifier combinations */
            }
            least_mod = (int) (modifiers_return & (~modifiers_return + 1));
            for (std_mods = (int) modifiers_return;
                 std_mods >= least_mod; std_mods--) {
                Modifiers dummy;

                /* check all useful combinations of modifier bits */
                if ((modifiers_return & (Modifiers) std_mods) &&
                    !(~modifiers_return & (Modifiers) std_mods)) {
                    XtTranslateKeycode(dpy, *keycodeP,
                                       (Modifiers) std_mods, &dummy, &keysym);
                    if (keysym == typeMatch->eventCode) {
                        XtGrabKey(widget, *keycodeP,
                                  careOn | (Modifiers) std_mods,
                                  grabP->owner_events,
                                  grabP->pointer_mode, grabP->keyboard_mode);
                        /* break; *//* grab all modifier combinations */
                    }
                }
            }
        }
        else {                  /* !event->standard */

            XtGrabKey(widget, *keycodeP, careOn,
                      grabP->owner_events,
                      grabP->pointer_mode, grabP->keyboard_mode);
        }
    }
    XtFree((char *) keycodes);
}

typedef struct {
    TMShortCard count;
    Widget widget;
    GrabActionRec *grabP;
} DoGrabRec;

static Boolean
DoGrab(StatePtr state, XtPointer data)
{
    /* *INDENT-EQLS* */
    DoGrabRec *doGrabP     = (DoGrabRec *) data;
    GrabActionRec *grabP   = doGrabP->grabP;
    Widget widget          = doGrabP->widget;
    TMShortCard count      = doGrabP->count;
    TMShortCard typeIndex  = state->typeIndex;
    TMShortCard modIndex   = state->modIndex;
    ActionRec *action;
    TMTypeMatch typeMatch;
    TMModifierMatch modMatch;
    Modifiers careOn = 0;
    Modifiers careMask = 0;

    LOCK_PROCESS;
    typeMatch = TMGetTypeMatch(typeIndex);
    modMatch = TMGetModifierMatch(modIndex);

    for (action = state->actions; action; action = action->next)
        if (count == action->idx)
            break;
    if (!action) {
        UNLOCK_PROCESS;
        return False;
    }

    switch (typeMatch->eventType) {
        case ButtonPress:
        case ButtonRelease:
            if (modMatch->lateModifiers) {
                Boolean resolved = _XtComputeLateBindings(XtDisplay(widget),
                                                          modMatch->lateModifiers,
                                                          &careOn, &careMask);

                if (!resolved)
                    break;
            }
            careOn = (careOn | (Modifiers) modMatch->modifiers);
            XtGrabButton(widget,
                         (int) typeMatch->eventCode,
                         careOn,
                         grabP->owner_events,
                         grabP->event_mask,
                         grabP->pointer_mode,
                         grabP->keyboard_mode,
                         None,
                         None);
            break;

        case KeyPress:
        case KeyRelease:
            GrabAllCorrectKeys(widget, typeMatch, modMatch, grabP);
            break;

        case EnterNotify:
            break;

        default:
            XtAppWarningMsg(XtWidgetToApplicationContext(widget),
                            "invalidPopup", "unsupportedOperation",
                            XtCXtToolkitError,
                            "Pop-up menu creation is only supported on Button, Key or EnterNotify events.",
                            NULL, NULL);
            break;
    }
    UNLOCK_PROCESS;
    return False;
}

void
_XtRegisterGrabs(Widget widget)
{
    XtTranslations xlations = widget->core.tm.translations;
    TMComplexStateTree *stateTreePtr;
    unsigned int count;
    TMShortCard i;
    TMBindData bindData = (TMBindData) widget->core.tm.proc_table;
    XtActionProc *procs;

    if (!XtIsRealized(widget) || widget->core.being_destroyed)
        return;

    /* walk the widget instance action bindings table looking for */
    /* actions registered as grab actions. */
    /* when you find one, do a grab on the triggering event */

    if (xlations == NULL)
        return;
    stateTreePtr = (TMComplexStateTree *) xlations->stateTreeTbl;
    if (*stateTreePtr == NULL)
        return;
    for (i = 0; i < xlations->numStateTrees; i++, stateTreePtr++) {
        if (bindData->simple.isComplex)
            procs = TMGetComplexBindEntry(bindData, i)->procs;
        else
            procs = TMGetSimpleBindEntry(bindData, i)->procs;
        for (count = 0; count < (*stateTreePtr)->numQuarks; count++) {
            GrabActionRec *grabP;
            DoGrabRec doGrab;

            LOCK_PROCESS;
            for (grabP = grabActionList; grabP != NULL; grabP = grabP->next) {
                if (grabP->action_proc == procs[count]) {
                    /* we've found a "grabber" in the action table. Find the
                     * states that call this action.  Note that if there is
                     * more than one "grabber" in the action table, we end
                     * up searching all of the states multiple times.
                     */
                    doGrab.widget = widget;
                    doGrab.grabP = grabP;
                    doGrab.count = (TMShortCard) count;
                    _XtTraverseStateTree((TMStateTree) *stateTreePtr,
                                         DoGrab, (XtPointer) &doGrab);
                }
            }
            UNLOCK_PROCESS;
        }
    }
}

void
XtRegisterGrabAction(XtActionProc action_proc,
                     _XtBoolean owner_events,
                     unsigned int event_mask,
                     int pointer_mode,
                     int keyboard_mode)
{
    GrabActionRec *actionP;

    LOCK_PROCESS;
    for (actionP = grabActionList; actionP != NULL; actionP = actionP->next) {
        if (actionP->action_proc == action_proc)
            break;
    }
    if (actionP == NULL) {
        actionP = XtNew(GrabActionRec);
        actionP->action_proc = action_proc;
        actionP->next = grabActionList;
        grabActionList = actionP;
    }
#ifdef DEBUG
    else if (actionP->owner_events != owner_events
             || actionP->event_mask != event_mask
             || actionP->pointer_mode != pointer_mode
             || actionP->keyboard_mode != keyboard_mode) {
        Cardinal n = 0;

        XtWarningMsg("argsReplaced", "xtRegisterGrabAction", XtCXtToolkitError,
                     "XtRegisterGrabAction called on same proc with different args",
                     NULL, &n);
    }
#endif /*DEBUG*/
    actionP->owner_events = (Boolean) owner_events;
    actionP->event_mask = event_mask;
    actionP->pointer_mode = pointer_mode;
    actionP->keyboard_mode = keyboard_mode;
    UNLOCK_PROCESS;
}

void
_XtGrabInitialize(XtAppContext app _X_UNUSED)
{
    LOCK_PROCESS;
    if (grabActionList == NULL)
        XtRegisterGrabAction(XtMenuPopupAction, True,
                             (unsigned) (ButtonPressMask | ButtonReleaseMask),
                             GrabModeAsync, GrabModeAsync);
    UNLOCK_PROCESS;

}
