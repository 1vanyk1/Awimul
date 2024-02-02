#ifndef _XtresourceI_h
#define _XtresourceI_h

#include "IntrinsicI.h"

#define StringToQuark(string) XrmStringToQuark(string)
#define StringToName(string) XrmStringToName(string)
#define StringToClass(string) XrmStringToClass(string)

_XFUNCPROTOBEGIN

extern void _XtDependencies(
        XtResourceList  * /* class_resp */,
        Cardinal	    * /* class_num_resp */,
        XrmResourceList * /* super_res */,
        Cardinal	     /* super_num_res */,
        Cardinal	     /* super_widget_size */);

extern void _XtResourceDependencies(
        WidgetClass  /* wc */
);

extern void _XtConstraintResDependencies(
        ConstraintWidgetClass  /* wc */
);

extern XtCacheRef* _XtGetResources(
        Widget	    /* w */,
        ArgList	    /* args */,
        Cardinal	    /* num_args */,
        XtTypedArgList  /* typed_args */,
        Cardinal*	    /* num_typed_args */
);

extern void _XtCopyFromParent(
        Widget		/* widget */,
        int			/* offset */,
        XrmValue*		/* value */
);

extern void _XtCopyToArg(char *src, XtArgVal *dst, unsigned int size);
extern void _XtCopyFromArg(XtArgVal src, char *dst, unsigned int size);
extern XrmResourceList* _XtCreateIndirectionTable(XtResourceList resources,
                                                  Cardinal num_resources);
extern void _XtResourceListInitialize(void);

_XFUNCPROTOEND

#endif /* _XtresourceI_h */