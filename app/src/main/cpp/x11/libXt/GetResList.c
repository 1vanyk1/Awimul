#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"
#include "Intrinsic.h"

/*
 * XtGetResourceList(), XtGetConstraintResourceList()
 */

#define TOXRMQUARK(p) ((XrmQuark)(long)(p))     /* avoid LP64 warnings */

void
XtGetResourceList(WidgetClass widget_class,
                  XtResourceList *resources,
                  Cardinal *num_resources)
{
    int size;
    register Cardinal i, dest = 0;
    register XtResourceList *list, dlist;

    LOCK_PROCESS;
    size = (int) (widget_class->core_class.num_resources * sizeof(XtResource));
    *resources = (XtResourceList) __XtMalloc((unsigned) size);

    if (!widget_class->core_class.class_inited) {
        /* Easy case */

        (void) memmove((char *) *resources,
                       (char *) widget_class->core_class.resources,
                       (size_t) size);
        *num_resources = widget_class->core_class.num_resources;
        UNLOCK_PROCESS;
        return;
    }

    /* Nope, it's the hard case */

    list = (XtResourceList *) widget_class->core_class.resources;
    dlist = *resources;
    for (i = 0; i < widget_class->core_class.num_resources; i++) {
        if (list[i] != NULL) {
            dlist[dest].resource_name = (String)
                    XrmQuarkToString(TOXRMQUARK(list[i]->resource_name));
            dlist[dest].resource_class = (String)
                    XrmQuarkToString(TOXRMQUARK(list[i]->resource_class));
            dlist[dest].resource_type = (String)
                    XrmQuarkToString(TOXRMQUARK(list[i]->resource_type));
            dlist[dest].resource_size = list[i]->resource_size;
            /* trust that resource_offset isn't that big */
            dlist[dest].resource_offset = (Cardinal)
                    -((int) (list[i]->resource_offset + 1));
            dlist[dest].default_type = (String)
                    XrmQuarkToString(TOXRMQUARK(list[i]->default_type));
            dlist[dest].default_addr = list[i]->default_addr;
            dest++;
        }
    }
    *num_resources = dest;
    UNLOCK_PROCESS;
}

static Boolean
ClassIsSubclassOf(WidgetClass class, WidgetClass superclass)
{
    for (; class != NULL; class = class->core_class.superclass) {
        if (class == superclass)
            return True;
    }
    return False;
}

void
XtGetConstraintResourceList(WidgetClass widget_class,
                            XtResourceList *resources,
                            Cardinal *num_resources)
{
    int size;
    register Cardinal i, dest = 0;
    register XtResourceList *list, dlist;
    ConstraintWidgetClass class = (ConstraintWidgetClass) widget_class;

    LOCK_PROCESS;
    if ((class->core_class.class_inited &&
         !(class->core_class.class_inited & ConstraintClassFlag))
        || (!class->core_class.class_inited &&
            !ClassIsSubclassOf(widget_class, constraintWidgetClass))
        || class->constraint_class.num_resources == 0) {

        *resources = NULL;
        *num_resources = 0;
        UNLOCK_PROCESS;
        return;
    }

    size = (int) (class->constraint_class.num_resources * sizeof(XtResource));
    *resources = (XtResourceList) __XtMalloc((unsigned) size);

    if (!class->core_class.class_inited) {
        /* Easy case */

        (void) memmove((char *) *resources,
                       (char *) class->constraint_class.resources,
                       (size_t) size);
        *num_resources = class->constraint_class.num_resources;
        UNLOCK_PROCESS;
        return;
    }

    /* Nope, it's the hard case */

    list = (XtResourceList *) class->constraint_class.resources;
    dlist = *resources;
    for (i = 0; i < class->constraint_class.num_resources; i++) {
        if (list[i] != NULL) {
            dlist[dest].resource_name = (String)
                    XrmQuarkToString(TOXRMQUARK(list[i]->resource_name));
            dlist[dest].resource_class = (String)
                    XrmQuarkToString(TOXRMQUARK(list[i]->resource_class));
            dlist[dest].resource_type = (String)
                    XrmQuarkToString(TOXRMQUARK(list[i]->resource_type));
            dlist[dest].resource_size = list[i]->resource_size;
            /* trust that resource_offset isn't that big */
            dlist[dest].resource_offset = (Cardinal)
                    -((int) (list[i]->resource_offset + 1));
            dlist[dest].default_type = (String)
                    XrmQuarkToString(TOXRMQUARK(list[i]->default_type));
            dlist[dest].default_addr = list[i]->default_addr;
            dest++;
        }
    }
    *num_resources = dest;
    UNLOCK_PROCESS;
}
