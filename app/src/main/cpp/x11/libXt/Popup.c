#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"
#include "ShellP.h"

void
_XtPopup(Widget widget, XtGrabKind grab_kind, _XtBoolean spring_loaded)
{
    register ShellWidget shell_widget = (ShellWidget) widget;

    if (!XtIsShell(widget)) {
        XtAppErrorMsg(XtWidgetToApplicationContext(widget),
                      "invalidClass", "xtPopup", XtCXtToolkitError,
                      "XtPopup requires a subclass of shellWidgetClass",
                      NULL, NULL);
    }

    if (!shell_widget->shell.popped_up) {
        XtGrabKind call_data = grab_kind;

        XtCallCallbacks(widget, XtNpopupCallback, (XtPointer) &call_data);
        shell_widget->shell.popped_up = TRUE;
        shell_widget->shell.grab_kind = grab_kind;
        shell_widget->shell.spring_loaded = (Boolean) spring_loaded;
        if (shell_widget->shell.create_popup_child_proc != NULL) {
            (*(shell_widget->shell.create_popup_child_proc)) (widget);
        }
        if (grab_kind == XtGrabExclusive) {
            XtAddGrab(widget, TRUE, spring_loaded);
        }
        else if (grab_kind == XtGrabNonexclusive) {
            XtAddGrab(widget, FALSE, spring_loaded);
        }
        XtRealizeWidget(widget);
        XMapRaised(XtDisplay(widget), XtWindow(widget));
    }
    else
        XRaiseWindow(XtDisplay(widget), XtWindow(widget));

}                               /* _XtPopup */

void
XtPopup(Widget widget, XtGrabKind grab_kind)
{
    Widget hookobj;

    switch (grab_kind) {

        case XtGrabNone:
        case XtGrabExclusive:
        case XtGrabNonexclusive:
            break;

        default:
            XtAppWarningMsg(XtWidgetToApplicationContext(widget),
                            "invalidGrabKind", "xtPopup", XtCXtToolkitError,
                            "grab kind argument has invalid value; XtGrabNone assumed",
                            NULL, NULL);
            grab_kind = XtGrabNone;
    }

    _XtPopup(widget, grab_kind, FALSE);

    hookobj = XtHooksOfDisplay(XtDisplay(widget));
    if (XtHasCallbacks(hookobj, XtNchangeHook) == XtCallbackHasSome) {
        XtChangeHookDataRec call_data;

        call_data.type = XtHpopup;
        call_data.widget = widget;
        call_data.event_data = (XtPointer) (XtIntPtr) grab_kind;
        XtCallCallbackList(hookobj,
                           ((HookObject) hookobj)->hooks.changehook_callbacks,
                           (XtPointer) &call_data);
    }
}                               /* XtPopup */

void
XtPopupSpringLoaded(Widget widget)
{
    Widget hookobj;

    _XtPopup(widget, XtGrabExclusive, True);

    hookobj = XtHooksOfDisplay(XtDisplay(widget));
    if (XtHasCallbacks(hookobj, XtNchangeHook) == XtCallbackHasSome) {
        XtChangeHookDataRec call_data;

        call_data.type = XtHpopupSpringLoaded;
        call_data.widget = widget;
        XtCallCallbackList(hookobj,
                           ((HookObject) hookobj)->hooks.changehook_callbacks,
                           (XtPointer) &call_data);
    }
}

void
XtPopdown(Widget widget)
{
    /* Unmap a shell widget if it is mapped, and remove from grab list */
    Widget hookobj;
    ShellWidget shell_widget = (ShellWidget) widget;
    XtGrabKind grab_kind;

    if (!XtIsShell(widget)) {
        XtAppErrorMsg(XtWidgetToApplicationContext(widget),
                      "invalidClass", "xtPopdown", XtCXtToolkitError,
                      "XtPopdown requires a subclass of shellWidgetClass",
                      NULL, NULL);
    }

#ifndef X_NO_XT_POPDOWN_CONFORMANCE
    if (!shell_widget->shell.popped_up)
        return;
#endif

    grab_kind = shell_widget->shell.grab_kind;
    XWithdrawWindow(XtDisplay(widget), XtWindow(widget),
                    XScreenNumberOfScreen(XtScreen(widget)));
    if (grab_kind != XtGrabNone)
        XtRemoveGrab(widget);
    shell_widget->shell.popped_up = FALSE;
    XtCallCallbacks(widget, XtNpopdownCallback, (XtPointer) &grab_kind);

    hookobj = XtHooksOfDisplay(XtDisplay(widget));
    if (XtHasCallbacks(hookobj, XtNchangeHook) == XtCallbackHasSome) {
        XtChangeHookDataRec call_data;

        call_data.type = XtHpopdown;
        call_data.widget = widget;
        XtCallCallbackList(hookobj,
                           ((HookObject) hookobj)->hooks.changehook_callbacks,
                           (XtPointer) &call_data);
    }
}                               /* XtPopdown */

void
XtCallbackPopdown(Widget widget _X_UNUSED,
                  XtPointer closure,
                  XtPointer call_data _X_UNUSED)
{
    register XtPopdownID id = (XtPopdownID) closure;

    XtPopdown(id->shell_widget);
    if (id->enable_widget != NULL) {
        XtSetSensitive(id->enable_widget, TRUE);
    }
}                               /* XtCallbackPopdown */
