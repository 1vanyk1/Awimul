#ifndef _XtConstraintP_h
#define _XtConstraintP_h

#include "Constraint.h"
#include "IntrinsicP.h"

_XFUNCPROTOBEGIN

typedef struct _ConstraintPart {
    XtPointer   mumble;		/* No new fields, keep C compiler happy */
} ConstraintPart;

typedef struct _ConstraintRec {
    CorePart	    core;
    CompositePart   composite;
    ConstraintPart  constraint;
} ConstraintRec, *ConstraintWidget;

typedef struct _ConstraintClassPart {
    XtResourceList resources;	      /* constraint resource list	     */
    Cardinal   num_resources;         /* number of constraints in list       */
    Cardinal   constraint_size;       /* size of constraint record           */
    XtInitProc initialize;            /* constraint initialization           */
    XtWidgetProc destroy;             /* constraint destroy proc             */
    XtSetValuesFunc set_values;       /* constraint set_values proc          */
    XtPointer	    extension;		/* pointer to extension record      */
} ConstraintClassPart;

typedef struct {
    XtPointer next_extension;	/* 1st 4 mandated for all extension records */
    XrmQuark record_type;	/* NULLQUARK; on ConstraintClassPart */
    long version;		/* must be XtConstraintExtensionVersion */
    Cardinal record_size;	/* sizeof(ConstraintClassExtensionRec) */
    XtArgsProc get_values_hook;
} ConstraintClassExtensionRec, *ConstraintClassExtension;

typedef struct _ConstraintClassRec {
    CoreClassPart       core_class;
    CompositeClassPart  composite_class;
    ConstraintClassPart constraint_class;
} ConstraintClassRec;

externalref ConstraintClassRec constraintClassRec;

_XFUNCPROTOEND

#define XtConstraintExtensionVersion 1L

#endif /* _XtConstraintP_h */
