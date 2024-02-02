#ifndef _VendorEP_h
#define _VendorEP_h

#include "XawImP.h"

typedef struct {
    XtPointer	extension;
} XawVendorShellExtClassPart;

typedef	struct _VendorShellExtClassRec {
    ObjectClassPart	object_class;
    XawVendorShellExtClassPart	vendor_shell_ext_class;
} XawVendorShellExtClassRec;

typedef struct {
    Widget	parent;
    XawImPart	im;
    XawIcPart	ic;
#ifndef OLDXAW
    XtPointer pad[4];	/* for future use and keep binary compatability */
#endif
} XawVendorShellExtPart;

typedef	struct XawVendorShellExtRec {
    ObjectPart	object;
    XawVendorShellExtPart	vendor_ext;
} XawVendorShellExtRec, *XawVendorShellExtWidget;

#endif	/* _VendorEP_h */
