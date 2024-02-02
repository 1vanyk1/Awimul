#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"
#include "StringDefs.h"
/******************************************************************
 *
 * Hook Object Resources
 *
 ******************************************************************/

/* *INDENT-OFF* */
static XtResource resources[] = {
        { XtNcreateHook, XtCCallback, XtRCallback, sizeof(XtPointer),
                XtOffsetOf(HookObjRec, hooks.createhook_callbacks),
                                                          XtRCallback, (XtPointer)NULL},
        { XtNchangeHook, XtCCallback, XtRCallback, sizeof(XtPointer),
                XtOffsetOf(HookObjRec, hooks.changehook_callbacks),
                                                          XtRCallback, (XtPointer)NULL},
        { XtNconfigureHook, XtCCallback, XtRCallback, sizeof(XtPointer),
                XtOffsetOf(HookObjRec, hooks.confighook_callbacks),
                                                          XtRCallback, (XtPointer)NULL},
        { XtNgeometryHook, XtCCallback, XtRCallback, sizeof(XtPointer),
                XtOffsetOf(HookObjRec, hooks.geometryhook_callbacks),
                                                          XtRCallback, (XtPointer)NULL},
        { XtNdestroyHook, XtCCallback, XtRCallback, sizeof(XtPointer),
                XtOffsetOf(HookObjRec, hooks.destroyhook_callbacks),
                                                          XtRCallback, (XtPointer)NULL},
        { XtNshells, XtCReadOnly, XtRWidgetList, sizeof(WidgetList),
                XtOffsetOf(HookObjRec, hooks.shells), XtRImmediate, (XtPointer) NULL },
        { XtNnumShells, XtCReadOnly, XtRCardinal, sizeof(Cardinal),
                XtOffsetOf(HookObjRec, hooks.num_shells), XtRImmediate, (XtPointer) 0 }
};
/* *INDENT-ON* */

static void GetValuesHook(Widget widget, ArgList args, Cardinal *num_args);
static void Initialize(Widget req, Widget new, ArgList args,
                       Cardinal *num_args);

/* *INDENT-OFF* */
externaldef(hookobjclassrec) HookObjClassRec hookObjClassRec = {
        { /* Object Class Part */
                /* superclass              */ (WidgetClass)&objectClassRec,
                /* class_name              */ "Hook",
                /* widget_size             */ sizeof(HookObjRec),
                /* class_initialize        */ NULL,
                /* class_part_initialize   */ NULL,
                /* class_inited            */ FALSE,
                /* initialize              */ Initialize,
                /* initialize_hook         */ NULL,
                /* realize                 */ NULL,
                /* actions                 */ NULL,
                /* num_actions             */ 0,
                /* resources               */ resources,
                /* num_resources           */ XtNumber(resources),
                /* xrm_class               */ NULLQUARK,
                /* compress_motion         */ FALSE,
                /* compress_exposure       */ TRUE,
                /* compress_enterleave     */ FALSE,
                /* visible_interest        */ FALSE,
                /* destroy                 */ NULL,
                /* resize                  */ NULL,
                /* expose                  */ NULL,
                /* set_values              */ NULL,
                /* set_values_hook         */ NULL,
                /* set_values_almost       */ NULL,
                /* get_values_hook         */ GetValuesHook,
                /* accept_focus            */ NULL,
                /* version                 */ XtVersion,
                /* callback_offsets        */ NULL,
                /* tm_table                */ NULL,
                /* query_geometry          */ NULL,
                /* display_accelerator     */ NULL,
                /* extension               */ NULL
        },
        { /* HookObj Class Part */
                /* unused               */  0
        }
};
/* *INDENT-ON* */

externaldef(hookObjectClass)
WidgetClass hookObjectClass = (WidgetClass) &hookObjClassRec;

static void
FreeShellList(Widget w,
              XtPointer closure _X_UNUSED,
              XtPointer call_data _X_UNUSED)
{
    HookObject h = (HookObject) w;

    if (h->hooks.shells != NULL)
        XtFree((char *) h->hooks.shells);
}

static void
Initialize(Widget req _X_UNUSED,
           Widget new,
           ArgList args _X_UNUSED,
           Cardinal *num_args _X_UNUSED)
{
    HookObject w = (HookObject) new;

    w->hooks.max_shells = 0;
    XtAddCallback(new, XtNdestroyCallback, FreeShellList, (XtPointer) NULL);
}

static void
GetValuesHook(Widget widget _X_UNUSED,
              ArgList args _X_UNUSED,
              Cardinal *num_args _X_UNUSED)
{
    /* get the XtNshells and XtNnumShells pseudo-resources */
}
