#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"
#include "StringDefs.h"
#include "CreateI.h"

/******************************************************************
 *
 * Rectangle Object Resources
 *
 ******************************************************************/

static void XtCopyAncestorSensitive(Widget, int, XrmValue *);

/* *INDENT-OFF* */
static XtResource resources[] = {

        {XtNancestorSensitive, XtCSensitive, XtRBoolean, sizeof(Boolean),
                XtOffsetOf(RectObjRec,rectangle.ancestor_sensitive),XtRCallProc,
                                                                       (XtPointer)XtCopyAncestorSensitive},
        {XtNx, XtCPosition, XtRPosition, sizeof(Position),
                XtOffsetOf(RectObjRec,rectangle.x), XtRImmediate, (XtPointer)0},
        {XtNy, XtCPosition, XtRPosition, sizeof(Position),
                XtOffsetOf(RectObjRec,rectangle.y), XtRImmediate, (XtPointer)0},
        {XtNwidth, XtCWidth, XtRDimension, sizeof(Dimension),
                XtOffsetOf(RectObjRec,rectangle.width), XtRImmediate, (XtPointer)0},
        {XtNheight, XtCHeight, XtRDimension, sizeof(Dimension),
                XtOffsetOf(RectObjRec,rectangle.height), XtRImmediate, (XtPointer)0},
        {XtNborderWidth, XtCBorderWidth, XtRDimension, sizeof(Dimension),
                XtOffsetOf(RectObjRec,rectangle.border_width), XtRImmediate,
                                                                       (XtPointer)1},
        {XtNsensitive, XtCSensitive, XtRBoolean, sizeof(Boolean),
                XtOffsetOf(RectObjRec,rectangle.sensitive), XtRImmediate,
                                                                       (XtPointer)True}
};
/* *INDENT-ON* */

static void RectClassPartInitialize(WidgetClass);
static void RectSetValuesAlmost(Widget, Widget, XtWidgetGeometry *,
                                XtWidgetGeometry *);

/* *INDENT-OFF* */
externaldef(rectobjclassrec) RectObjClassRec rectObjClassRec = {
        {
                /* superclass            */ (WidgetClass)&objectClassRec,
                /* class_name            */ "Rect",
                /* widget_size           */ sizeof(RectObjRec),
                /* class_initialize      */ NULL,
                /* class_part_initialize */ RectClassPartInitialize,
                /* class_inited          */ FALSE,
                /* initialize            */ NULL,
                /* initialize_hook       */ NULL,
                /* realize               */ NULL,
                /* actions               */ NULL,
                /* num_actions           */ 0,
                /* resources             */ resources,
                /* num_resources         */ XtNumber(resources),
                /* xrm_class             */ NULLQUARK,
                /* compress_motion       */ FALSE,
                /* compress_exposure     */ TRUE,
                /* compress_enterleave   */ FALSE,
                /* visible_interest      */ FALSE,
                /* destroy               */ NULL,
                /* resize                */ NULL,
                /* expose                */ NULL,
                /* set_values            */ NULL,
                /* set_values_hook       */ NULL,
                /* set_values_almost     */ RectSetValuesAlmost,
                /* get_values_hook       */ NULL,
                /* accept_focus          */ NULL,
                /* version               */ XtVersion,
                /* callback_offsets      */ NULL,
                /* tm_table              */ NULL,
                /* query_geometry        */ NULL,
                /* display_accelerator   */ NULL,
                /* extension             */ NULL
        }
};
/* *INDENT-ON* */

externaldef(rectObjClass)
WidgetClass rectObjClass = (WidgetClass) &rectObjClassRec;

static void
XtCopyAncestorSensitive(Widget widget, int offset _X_UNUSED, XrmValue *value)
{
    static Boolean sensitive;
    Widget parent = widget->core.parent;

    sensitive = (parent->core.ancestor_sensitive & parent->core.sensitive);
    value->addr = (XPointer) (&sensitive);
}

/*
 * Start of rectangle object methods
 */

static void
RectClassPartInitialize(register WidgetClass wc)
{
    register RectObjClass roc = (RectObjClass) wc;
    register RectObjClass super = ((RectObjClass) roc->rect_class.superclass);

    /* We don't need to check for null super since we'll get to object
       eventually, and it had better define them!  */

    if (roc->rect_class.resize == XtInheritResize) {
        roc->rect_class.resize = super->rect_class.resize;
    }

    if (roc->rect_class.expose == XtInheritExpose) {
        roc->rect_class.expose = super->rect_class.expose;
    }

    if (roc->rect_class.set_values_almost == XtInheritSetValuesAlmost) {
        roc->rect_class.set_values_almost = super->rect_class.set_values_almost;
    }

    if (roc->rect_class.query_geometry == XtInheritQueryGeometry) {
        roc->rect_class.query_geometry = super->rect_class.query_geometry;
    }
}

/*
 * Why there isn't an Initialize Method:
 *
 * Initialization of the RectObj non-Resource field is done by the
 * intrinsics in _XtCreateWidget in order that the field is initialized
 * for use by converters during instance resource resolution.
 */

static void
RectSetValuesAlmost(Widget old _X_UNUSED,
                    Widget new _X_UNUSED,
                    XtWidgetGeometry *request,
                    XtWidgetGeometry *reply)
{
    *request = *reply;
}
