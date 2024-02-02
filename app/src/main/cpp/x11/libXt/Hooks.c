#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"
#include "CreateI.h"

static void
FreeBlockHookList(Widget widget _X_UNUSED,
                  XtPointer closure, /* ActionHook* */
                  XtPointer call_data _X_UNUSED)
{
    BlockHook list = *(BlockHook *) closure;

    while (list != NULL) {
        BlockHook next = list->next;

        XtFree((XtPointer) list);
        list = next;
    }
}

XtBlockHookId
XtAppAddBlockHook(XtAppContext app, XtBlockHookProc proc, XtPointer closure)
{
    BlockHook hook = XtNew(BlockHookRec);

    LOCK_APP(app);
    hook->next = app->block_hook_list;
    hook->app = app;
    hook->proc = proc;
    hook->closure = closure;
    if (app->block_hook_list == NULL) {
        _XtAddCallback(&app->destroy_callbacks,
                       FreeBlockHookList, (XtPointer) &app->block_hook_list);
    }
    app->block_hook_list = hook;
    UNLOCK_APP(app);
    return (XtBlockHookId) hook;
}

void
XtRemoveBlockHook(XtBlockHookId id)
{
    BlockHook *p, hook = (BlockHook) id;
    XtAppContext app = hook->app;

    LOCK_APP(app);
    for (p = &app->block_hook_list; p != NULL && *p != hook; p = &(*p)->next);
    if (p == NULL) {
#ifdef DEBUG
        XtAppWarningMsg(app, "badId", "xtRemoveBlockHook", XtCXtToolkitError,
                        "XtRemoveBlockHook called with bad or old hook id",
                        NULL, NULL);
#endif   /*DEBUG*/
        UNLOCK_APP(app);
        return;
    }
    *p = hook->next;
    XtFree((XtPointer) hook);
    UNLOCK_APP(app);
}

static void
DeleteShellFromHookObj(Widget shell,
                       XtPointer closure,
                       XtPointer call_data _X_UNUSED)
{
    /* app_con is locked when this function is called */
    Cardinal ii, jj;
    HookObject ho = (HookObject) closure;

    for (ii = 0; ii < ho->hooks.num_shells; ii++)
        if (ho->hooks.shells[ii] == shell) {
            /* collapse the list */
            for (jj = ii; jj < ho->hooks.num_shells; jj++) {
                if ((jj + 1) < ho->hooks.num_shells)
                    ho->hooks.shells[jj] = ho->hooks.shells[jj + 1];
            }
            break;
        }
    ho->hooks.num_shells--;
}

#define SHELL_INCR 4

void
_XtAddShellToHookObj(Widget shell)
{
    /* app_con is locked when this function is called */
    HookObject ho = (HookObject) XtHooksOfDisplay(XtDisplay(shell));

    if (ho->hooks.num_shells == ho->hooks.max_shells) {
        ho->hooks.max_shells += SHELL_INCR;
        ho->hooks.shells =
                (WidgetList) XtRealloc((char *) ho->hooks.shells,
                                       (Cardinal) (ho->hooks.max_shells *
                                                   sizeof(Widget)));
    }
    ho->hooks.shells[ho->hooks.num_shells++] = shell;

    XtAddCallback(shell, XtNdestroyCallback, DeleteShellFromHookObj,
                  (XtPointer) ho);
}

Boolean
_XtIsHookObject(Widget widget)
{
    return (widget->core.widget_class == hookObjectClass);
}

Widget
XtHooksOfDisplay(Display *dpy)
{
    Widget retval;
    XtPerDisplay pd;

    DPY_TO_APPCON(dpy);

    LOCK_APP(app);
    pd = _XtGetPerDisplay(dpy);
    if (pd->hook_object == NULL)
        pd->hook_object =
                _XtCreateHookObj((Screen *) DefaultScreenOfDisplay(dpy));
    retval = pd->hook_object;
    UNLOCK_APP(app);
    return retval;
}
