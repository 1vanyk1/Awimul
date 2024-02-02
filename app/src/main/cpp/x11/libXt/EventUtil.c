#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"
#include "PassivGraI.h"
#include "StringDefs.h"
#include "EventI.h"

static XContext perWidgetInputContext = 0;

void
_XtFreePerWidgetInput(Widget w, XtPerWidgetInput pwi)
{
    LOCK_PROCESS;
    XDeleteContext(XtDisplay(w), (Window) w, perWidgetInputContext);

    XtFree((char *) pwi);
    UNLOCK_PROCESS;
}

/*
 * This routine gets the passive list associated with the widget
 * from the context manager.
 */
XtPerWidgetInput
_XtGetPerWidgetInput(Widget widget, _XtBoolean create)
{
    XtPerWidgetInput pwi = NULL;
    Display *dpy = widget->core.screen->display;

    LOCK_PROCESS;
    if (!perWidgetInputContext)
        perWidgetInputContext = XUniqueContext();

    if (XFindContext(dpy,
                     (Window) widget,
                     perWidgetInputContext, (XPointer *) &pwi) && create) {
        pwi = (XtPerWidgetInput)
                __XtMalloc((unsigned) sizeof(XtPerWidgetInputRec));

        pwi->focusKid = NULL;
        pwi->queryEventDescendant = NULL;
        pwi->focalPoint = XtUnrelated;
        pwi->keyList = pwi->ptrList = NULL;

        pwi->haveFocus =
        pwi->map_handler_added =
        pwi->realize_handler_added = pwi->active_handler_added = FALSE;

        XtAddCallback(widget, XtNdestroyCallback,
                      _XtDestroyServerGrabs, (XtPointer) pwi);

        (void) XSaveContext(dpy,
                            (Window) widget,
                            perWidgetInputContext, (char *) pwi);
    }
    UNLOCK_PROCESS;
    return pwi;
}

void
_XtFillAncestorList(Widget **listPtr,
                    int *maxElemsPtr,
                    int *numElemsPtr,
                    Widget start,
                    Widget breakWidget)
{
#define CACHESIZE 16
    Cardinal i;
    Widget w;
    Widget *trace = *listPtr;

    /* First time in, allocate the ancestor list */
    if (trace == NULL) {
        trace = (Widget *) __XtMalloc(CACHESIZE * sizeof(Widget));
        *maxElemsPtr = CACHESIZE;
    }
    /* First fill in the ancestor list */

    trace[0] = start;

    for (i = 1, w = XtParent(start);
         w != NULL && !XtIsShell(trace[i - 1]) && trace[i - 1] != breakWidget;
         w = XtParent(w), i++) {
        if (i == (Cardinal) *maxElemsPtr) {
            /* This should rarely happen, but if it does it'll probably
               happen again, so grow the ancestor list */
            *maxElemsPtr += CACHESIZE;
            trace = (Widget *) XtRealloc((char *) trace,
                                         (Cardinal) (sizeof(Widget) *
                                                     (size_t) (*maxElemsPtr)));
        }
        trace[i] = w;
    }
    *listPtr = trace;
    *numElemsPtr = (int) i;
#undef CACHESIZE
}

Widget
_XtFindRemapWidget(XEvent *event,
                   Widget widget,
                   EventMask mask,
                   XtPerDisplayInput pdi)
{
    Widget dspWidget = widget;

    if (!pdi->traceDepth || !(widget == pdi->trace[0])) {
        _XtFillAncestorList(&pdi->trace, &pdi->traceMax,
                            &pdi->traceDepth, widget, NULL);
        pdi->focusWidget = NULL;        /* invalidate the focus
                                           cache */
    }
    if (mask & (KeyPressMask | KeyReleaseMask))
        dspWidget = _XtProcessKeyboardEvent((XKeyEvent *) event, widget, pdi);
    else if (mask & (ButtonPressMask | ButtonReleaseMask))
        dspWidget = _XtProcessPointerEvent((XButtonEvent *) event, widget, pdi);

    return dspWidget;
}

void
_XtUngrabBadGrabs(XEvent *event,
                  Widget widget,
                  EventMask mask,
                  XtPerDisplayInput pdi)
{
    XKeyEvent *ke = (XKeyEvent *) event;

    if (mask & (KeyPressMask | KeyReleaseMask)) {
        if (IsServerGrab(pdi->keyboard.grabType) &&
            !_XtOnGrabList(pdi->keyboard.grab.widget, pdi->grabList))
            XtUngrabKeyboard(widget, ke->time);

    }
    else {
        if (IsServerGrab(pdi->pointer.grabType) &&
            !_XtOnGrabList(pdi->pointer.grab.widget, pdi->grabList))
            XtUngrabPointer(widget, ke->time);
    }
}
