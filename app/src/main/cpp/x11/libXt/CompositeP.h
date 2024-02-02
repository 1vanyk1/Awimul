#ifndef _XtCompositeP_h
#define _XtCompositeP_h

#include "Composite.h"
#include "IntrinsicP.h"

_XFUNCPROTOBEGIN

/************************************************************************
 *
 * Additional instance fields for widgets of (sub)class 'Composite'
 *
 ************************************************************************/

typedef struct _CompositePart {
    WidgetList  children;	     /* array of ALL widget children	     */
    Cardinal    num_children;	     /* total number of widget children	     */
    Cardinal    num_slots;           /* number of slots in children array    */
    XtOrderProc insert_position;     /* compute position of new child	     */
} CompositePart,*CompositePtr;

typedef struct _CompositeRec {
    CorePart      core;
    CompositePart composite;
} CompositeRec;

/*********************************************************************
 *
 *  Additional class fields for widgets of (sub)class 'Composite'
 *
 ********************************************************************/

typedef struct _CompositeClassPart {
    XtGeometryHandler geometry_manager;	  /* geometry manager for children   */
    XtWidgetProc      change_managed;	  /* change managed state of child   */
    XtWidgetProc      insert_child;	  /* physically add child to parent  */
    XtWidgetProc      delete_child;	  /* physically remove child	     */
    XtPointer	      extension;	  /* pointer to extension record     */
} CompositeClassPart,*CompositePartPtr;

typedef struct {
    XtPointer next_extension;	/* 1st 4 mandated for all extension records */
    XrmQuark record_type;	/* NULLQUARK; on CompositeClassPart */
    long version;		/* must be XtCompositeExtensionVersion */
    Cardinal record_size;	/* sizeof(CompositeClassExtensionRec) */
    Boolean accepts_objects;
    Boolean allows_change_managed_set;
} CompositeClassExtensionRec, *CompositeClassExtension;


typedef struct _CompositeClassRec {
    CoreClassPart      core_class;
    CompositeClassPart composite_class;
} CompositeClassRec;

externalref CompositeClassRec compositeClassRec;

_XFUNCPROTOEND

#define XtCompositeExtensionVersion 2L
#define XtInheritGeometryManager ((XtGeometryHandler) _XtInherit)
#define XtInheritChangeManaged ((XtWidgetProc) _XtInherit)
#define XtInheritInsertChild ((XtWidgetProc) _XtInherit)
#define XtInheritDeleteChild ((XtWidgetProc) _XtInherit)

#endif /* _XtCompositeP_h */
/* DON'T ADD STUFF AFTER THIS #endif */
