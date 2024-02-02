#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"
#include "StringDefs.h"

static void ConstraintPartInitialize(WidgetClass wc);

externaldef(constraintclassrec)
ConstraintClassRec constraintClassRec = {
          {
                  /******* CorePart *******/
                  /* superclass           */ (WidgetClass) &compositeClassRec,
                  /* class_name           */ "Constraint",
                  /* widget_size          */ sizeof(ConstraintRec),
                  /* class_initialize     */ NULL,
                  /* class_part_initialize */ ConstraintPartInitialize,
                  /* class_inited         */ FALSE,
                  /* initialize           */ NULL,
                  /* initialize_hook      */ NULL,
                  /* realize              */ XtInheritRealize,
                  /* actions              */ NULL,
                  /* num_actions          */ 0,
                  /* resources            */ NULL,
                  /* num_resources        */ 0,
                  /* xrm_class            */ NULLQUARK,
                  /* compress_motion      */ FALSE,
                  /* compress_exposure    */ TRUE,
                  /* compress_enterleave  */ FALSE,
                  /* visible_interest     */ FALSE,
                  /* destroy              */ NULL,
                  /* resize               */ NULL,
                  /* expose               */ NULL,
                  /* set_values           */ NULL,
                  /* set_values_hook      */ NULL,
                  /* set_values_almost    */ XtInheritSetValuesAlmost,
                  /* get_values_hook      */ NULL,
                  /* accept_focus         */ NULL,
                  /* version              */ XtVersion,
                  /* callback_offsets     */ NULL,
                  /* tm_table             */ NULL,
                  /* query_geometry       */ NULL,
                  /* display_accelerator  */ NULL,
                  /* extension            */ NULL
          }
        , {
                  /**** CompositePart *****/
                  /* geometry_handler     */ NULL,
                  /* change_managed       */ NULL,
                  /* insert_child         */ XtInheritInsertChild,
                  /* delete_child         */ XtInheritDeleteChild,
                  /* extension            */ NULL
          }
        , {
                  /**** ConstraintPart ****/
                  /* resources            */ NULL,
                  /* num_resources        */ 0,
                  /* constraint_size      */ 0,
                  /* initialize           */ NULL,
                  /* destroy              */ NULL,
                  /* set_values           */ NULL,
                  /* extension            */ NULL
          }
};

externaldef(constraintwidgetclass)
WidgetClass constraintWidgetClass = (WidgetClass) &constraintClassRec;

static void
ConstraintPartInitialize(WidgetClass wc)
{
    ConstraintWidgetClass cwc = (ConstraintWidgetClass) wc;

    if (cwc->constraint_class.resources)
        _XtCompileResourceList(cwc->constraint_class.resources,
                               cwc->constraint_class.num_resources);

    _XtConstraintResDependencies((ConstraintWidgetClass) wc);
}
