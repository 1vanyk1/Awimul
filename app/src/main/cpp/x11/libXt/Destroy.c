#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"

struct _DestroyRec {
    int dispatch_level;
    Widget widget;
};

static void
Recursive(Widget widget, XtWidgetProc proc)
{
    register Cardinal i;

    /* Recurse down normal children */
    if (XtIsComposite(widget)) {
        CompositePart *cwp = &(((CompositeWidget) widget)->composite);

        for (i = 0; i < cwp->num_children; i++) {
            Recursive(cwp->children[i], proc);
        }
    }

    /* Recurse down popup children */
    if (XtIsWidget(widget)) {
        for (i = 0; i < widget->core.num_popups; i++) {
            Recursive(widget->core.popup_list[i], proc);
        }
    }

    /* Finally, apply procedure to this widget */
    (*proc) (widget);
}                               /* Recursive */

static void
Phase1Destroy(Widget widget)
{
    Widget hookobj = XtHooksOfDisplay(XtDisplayOfObject(widget));

    widget->core.being_destroyed = TRUE;
    if (XtHasCallbacks(hookobj, XtNdestroyHook) == XtCallbackHasSome) {
        XtDestroyHookDataRec call_data;

        call_data.type = XtHdestroy;
        call_data.widget = widget;
        XtCallCallbackList(hookobj,
                           ((HookObject) hookobj)->hooks.destroyhook_callbacks,
                           (XtPointer) &call_data);
    }
}                               /* Phase1Destroy */

static void
Phase2Callbacks(Widget widget)
{
    if (widget->core.destroy_callbacks != NULL) {
        XtCallCallbackList(widget,
                           widget->core.destroy_callbacks, (XtPointer) NULL);
    }
}                               /* Phase2Callbacks */

static void
Phase2Destroy(register Widget widget)
{
    register WidgetClass class;
    register ConstraintWidgetClass cwClass;
    ObjectClassExtension ext;

    /* Call constraint destroy procedures */
    if (XtParent(widget) != NULL && !XtIsShell(widget) &&
        XtIsConstraint(XtParent(widget))) {
        LOCK_PROCESS;
        cwClass = (ConstraintWidgetClass) XtParent(widget)->core.widget_class;
        UNLOCK_PROCESS;
        for (;;) {
            XtWidgetProc destroy;

            LOCK_PROCESS;
            destroy = cwClass->constraint_class.destroy;
            UNLOCK_PROCESS;
            if (destroy)
                (*destroy) (widget);
            if (cwClass == (ConstraintWidgetClass) constraintWidgetClass)
                break;
            LOCK_PROCESS;
            cwClass = (ConstraintWidgetClass) cwClass->core_class.superclass;
            UNLOCK_PROCESS;
        }
    }

    /* Call widget destroy procedures */
    LOCK_PROCESS;
    for (class = widget->core.widget_class;
         class != NULL; class = class->core_class.superclass) {
        XtWidgetProc destroy;

        destroy = class->core_class.destroy;
        UNLOCK_PROCESS;
        if (destroy)
            (*destroy) (widget);
        LOCK_PROCESS;
    }

    /* Call widget deallocate procedure */
    ext = (ObjectClassExtension) XtGetClassExtension(widget->core.widget_class,
                                                     XtOffsetOf(CoreClassPart,
                                                                extension),
                                                     NULLQUARK,
                                                     XtObjectExtensionVersion,
                                                     sizeof
                                                             (ObjectClassExtensionRec));
    if (ext && ext->deallocate) {
        XtDeallocateProc deallocate;

        deallocate = ext->deallocate;
        UNLOCK_PROCESS;
        (*deallocate) (widget, NULL);
    }
    else {
        UNLOCK_PROCESS;
        XtFree((char *) widget);
    }
}                               /* Phase2Destroy */

static Boolean
IsDescendant(Widget widget, Widget root)
{
    while ((widget = XtParent(widget)) != root) {
        if (widget == NULL)
            return False;
    }
    return True;
}

static void
XtPhase2Destroy(Widget widget)
{
    Display *display = NULL;
    Window window;
    Widget parent;
    XtAppContext app = XtWidgetToApplicationContext(widget);
    Widget outerInPhase2Destroy = app->in_phase2_destroy;
    int starting_count = app->destroy_count;
    Boolean isPopup = False;

    /* invalidate focus trace cache for this display */
    _XtGetPerDisplay(XtDisplayOfObject(widget))->pdi.traceDepth = 0;

    parent = widget->core.parent;

    if (parent && XtIsWidget(parent) && parent->core.num_popups) {
        Cardinal i;

        for (i = 0; i < parent->core.num_popups; i++) {
            if (parent->core.popup_list[i] == widget) {
                isPopup = True;
                break;
            }
        }
    }

    if (!isPopup && parent && XtIsComposite(parent)) {
        XtWidgetProc delete_child;

        LOCK_PROCESS;
        delete_child =
                ((CompositeWidgetClass) parent->core.widget_class)->composite_class.
                        delete_child;
        UNLOCK_PROCESS;
        if (XtIsRectObj(widget)) {
            XtUnmanageChild(widget);
        }
        if (delete_child == NULL) {
            String param;
            Cardinal num_params = 1;

            LOCK_PROCESS;
            param = parent->core.widget_class->core_class.class_name;
            UNLOCK_PROCESS;
            XtAppWarningMsg(XtWidgetToApplicationContext(widget),
                            "invalidProcedure", "deleteChild",
                            XtCXtToolkitError,
                            "null delete_child procedure for class %s in XtDestroy",
                            &param, &num_params);
        }
        else {
            (*delete_child) (widget);
        }
    }

    /* widget is freed in Phase2Destroy, so retrieve window now.
     * Shells destroy their own windows, to prevent window leaks in
     * popups; this test is practical only when XtIsShell() is cheap.
     */
    if (XtIsShell(widget) || !XtIsWidget(widget)) {
        window = 0;
    }
    else {
        display = XtDisplay(widget);

        window = widget->core.window;
    }

    Recursive(widget, Phase2Callbacks);
    if (app->destroy_count > starting_count) {
        int i = starting_count;

        while (i < app->destroy_count) {

            DestroyRec *dr = app->destroy_list + i;

            if (IsDescendant(dr->widget, widget)) {
                Widget descendant = dr->widget;
                register int j;

                app->destroy_count--;
                for (j = app->destroy_count - i; --j >= 0; dr++)
                    *dr = *(dr + 1);
                XtPhase2Destroy(descendant);
            }
            else
                i++;
        }
    }

    app->in_phase2_destroy = widget;
    Recursive(widget, Phase2Destroy);
    app->in_phase2_destroy = outerInPhase2Destroy;

    if (isPopup) {
        Cardinal i;

        for (i = 0; i < parent->core.num_popups; i++)
            if (parent->core.popup_list[i] == widget) {
                parent->core.num_popups--;
                while (i < parent->core.num_popups) {
                    parent->core.popup_list[i] = parent->core.popup_list[i + 1];
                    i++;
                }
                break;
            }
    }

    /* %%% the following parent test hides a more serious problem,
       but it avoids breaking those who depended on the old bug
       until we have time to fix it properly. */

    if (window && (parent == NULL || !parent->core.being_destroyed))
        XDestroyWindow(display, window);
}                               /* XtPhase2Destroy */

void
_XtDoPhase2Destroy(XtAppContext app, int dispatch_level)
{
    /* Phase 2 must occur in fifo order.  List is not necessarily
     * contiguous in dispatch_level.
     */

    int i = 0;

    while (i < app->destroy_count) {

        /* XtPhase2Destroy can result in calls to XtDestroyWidget,
         * and these could cause app->destroy_list to be reallocated.
         */

        DestroyRec *dr = app->destroy_list + i;

        if (dr->dispatch_level >= dispatch_level) {
            Widget w = dr->widget;
            register int j;

            app->destroy_count--;
            for (j = app->destroy_count - i; --j >= 0; dr++)
                *dr = *(dr + 1);
            XtPhase2Destroy(w);
        }
        else
            i++;
    }
}

void
XtDestroyWidget(Widget widget)
{
    XtAppContext app;
    DestroyRec *dr;

    app = XtWidgetToApplicationContext(widget);
    LOCK_APP(app);
    if (widget->core.being_destroyed) {
        UNLOCK_APP(app);
        return;
    }
    Recursive(widget, Phase1Destroy);

    if (app->in_phase2_destroy && IsDescendant(widget, app->in_phase2_destroy)) {
        XtPhase2Destroy(widget);
        UNLOCK_APP(app);
        return;
    }

    if (app->destroy_count == app->destroy_list_size) {
        app->destroy_list_size += 10;
        app->destroy_list = (DestroyRec *)
                XtRealloc((char *) app->destroy_list,
                          (Cardinal) (sizeof(DestroyRec) *
                                      (size_t) app->destroy_list_size)
                );
    }
    dr = app->destroy_list + app->destroy_count++;
    dr->dispatch_level = app->dispatch_level;
    dr->widget = widget;

    if (app->dispatch_level > 1) {
        int i;

        for (i = app->destroy_count - 1; i;) {
            /* this handles only one case of nesting difficulties */
            dr = app->destroy_list + (--i);
            if (dr->dispatch_level < app->dispatch_level &&
                IsDescendant(dr->widget, widget)) {
                DestroyRec *dr2 = app->destroy_list + (app->destroy_count - 1);

                dr2->dispatch_level = dr->dispatch_level;
                break;
            }
        }
    }

    if (_XtSafeToDestroy(app)) {
        app->dispatch_level = 1;        /* avoid nested _XtDoPhase2Destroy */
        _XtDoPhase2Destroy(app, 0);
        app->dispatch_level = 0;
    }
    UNLOCK_APP(app);

}                               /* XtDestroyWidget */