#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"
#include "StringDefs.h"

/*
 *      XtSetSensitive()
 */

static void
SetAncestorSensitive(register Widget widget, Boolean ancestor_sensitive)
{
    Arg args[1];

    if (widget->core.ancestor_sensitive == ancestor_sensitive)
        return;

    XtSetArg(args[0], XtNancestorSensitive, ancestor_sensitive);
    XtSetValues(widget, args, XtNumber(args));

    /* If widget's sensitive is TRUE, propagate new ancestor_sensitive to
       children's ancestor_sensitive; else do nothing as children's
       ancestor_sensitive is already FALSE */

    if (widget->core.sensitive && XtIsComposite(widget)) {
        Cardinal i;
        WidgetList children;

        children = ((CompositeWidget) widget)->composite.children;
        for (i = 0; i < ((CompositeWidget) widget)->composite.num_children; i++) {
            SetAncestorSensitive(children[i], ancestor_sensitive);
        }
    }
}                               /* SetAncestorSensitive */

void
XtSetSensitive(register Widget widget, _XtBoolean sensitive)
{
    Arg args[1];

    WIDGET_TO_APPCON(widget);

    LOCK_APP(app);
    if (widget->core.sensitive == sensitive) {
        UNLOCK_APP(app);
        return;
    }

    XtSetArg(args[0], XtNsensitive, sensitive);
    XtSetValues(widget, args, XtNumber(args));

    /* If widget's ancestor_sensitive is TRUE, propagate new sensitive to
       children's ancestor_sensitive; else do nothing as children's
       ancestor_sensitive is already FALSE */

    if (widget->core.ancestor_sensitive && XtIsComposite(widget)) {
        Cardinal i;
        WidgetList children;

        children = ((CompositeWidget) widget)->composite.children;
        for (i = 0; i < ((CompositeWidget) widget)->composite.num_children; i++) {
            SetAncestorSensitive(children[i], (Boolean) sensitive);
        }
    }
    UNLOCK_APP(app);
}                               /* XtSetSensitive */
