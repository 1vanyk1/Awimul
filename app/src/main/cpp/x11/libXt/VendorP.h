#ifndef  _XtVendorPrivate_h
#define _XtVendorPrivate_h

#include "Vendor.h"

/* New fields for the VendorShell widget class record */

_XFUNCPROTOBEGIN

typedef struct {
    XtPointer       extension;          /* pointer to extension record      */
} VendorShellClassPart;

typedef struct _VendorShellClassRec {
    CoreClassPart      core_class;
    CompositeClassPart composite_class;
    ShellClassPart  shell_class;
    WMShellClassPart   wm_shell_class;
    VendorShellClassPart vendor_shell_class;
} VendorShellClassRec;

externalref VendorShellClassRec vendorShellClassRec;

/* New fields for the vendor shell widget. */

typedef struct {
    int		vendor_specific;
} VendorShellPart;

typedef  struct {
    CorePart 	core;
    CompositePart 	composite;
    ShellPart 	shell;
    WMShellPart	wm;
    VendorShellPart	vendor;
} VendorShellRec, *VendorShellWidget;

_XFUNCPROTOEND

#endif  /* _XtVendorPrivate_h */