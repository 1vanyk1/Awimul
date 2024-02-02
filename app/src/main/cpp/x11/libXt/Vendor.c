#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"
#include "StringDefs.h"
#include "Shell.h"
#include "ShellP.h"
#include "Vendor.h"
#include "VendorP.h"
#include <stdio.h>

/***************************************************************************
 *
 * Vendor shell class record
 *
 ***************************************************************************/

#if defined(__CYGWIN__) || defined(__MINGW32__)
/* to fix the EditRes problem because of wrong linker semantics */
extern WidgetClass vendorShellWidgetClass;

int __stdcall
DllMain(unsigned long mod_handle, unsigned long flag, void *routine)
{
    switch (flag) {
    case 1:                    /* DLL_PROCESS_ATTACH - process attach */
        vendorShellWidgetClass = (WidgetClass) (&vendorShellClassRec);
        break;
    case 0:                    /* DLL_PROCESS_DETACH - process detach */
        break;
    }
    return 1;
}
#endif

/* *INDENT-OFF* */
externaldef(vendorshellclassrec)
VendorShellClassRec vendorShellClassRec = {
        {
                /* superclass            */ (WidgetClass) &wmShellClassRec,
                /* class_name            */ "VendorShell",
                /* size                  */ sizeof(VendorShellRec),
                /* Class Initializer     */ NULL,
                /* class_part_initialize */ NULL,
                /* Class init'ed ?       */ FALSE,
                /* initialize            */ NULL,
                /* initialize_notify     */ NULL,
                /* realize               */ XtInheritRealize,
                /* actions               */ NULL,
                /* num_actions           */ 0,
                /* resources             */ NULL,
                /* resource_count        */ 0,
                /* xrm_class             */ NULLQUARK,
                /* compress_motion       */ FALSE,
                /* compress_exposure     */ TRUE,
                /* compress_enterleave   */ FALSE,
                /* visible_interest      */ FALSE,
                /* destroy               */ NULL,
                /* resize                */ XtInheritResize,
                /* expose                */ NULL,
                /* set_values            */ NULL,
                /* set_values_hook       */ NULL,
                /* set_values_almost     */ XtInheritSetValuesAlmost,
                /* get_values_hook       */ NULL,
                /* accept_focus          */ NULL,
                /* intrinsics version    */ XtVersion,
                /* callback offsets      */ NULL,
                /* tm_table              */ NULL,
                /* query_geometry        */ NULL,
                /* display_accelerator   */ NULL,
                /* extension             */ NULL
        },
        {
                /* geometry_manager    */ XtInheritGeometryManager,
                /* change_managed      */ XtInheritChangeManaged,
                /* insert_child        */ XtInheritInsertChild,
                /* delete_child        */ XtInheritDeleteChild,
                /* extension           */ NULL
        },
        {
                /* extension           */ NULL
        },
        {
                /* extension           */ NULL
        },
        {
                /* extension           */ NULL
        }
};
/* *INDENT-ON* */

//#if !defined(AIXSHLIB) || !defined(SHAREDCODE)
//externaldef(vendorshellwidgetclass)
//WidgetClass vendorShellWidgetClass = (WidgetClass) (&vendorShellClassRec);
//#endif
