#ifndef _Xt_ObjectP_h_
#define _Xt_ObjectP_h_

#include "Object.h"
#include "IntrinsicP.h"

_XFUNCPROTOBEGIN

/**********************************************************
 * Object Instance Data Structures
 *
 **********************************************************/
/* these fields match CorePart and can not be changed */

typedef struct _ObjectPart {
    Widget          self;               /* pointer to widget itself          */
    WidgetClass     widget_class;       /* pointer to Widget's ClassRec      */
    Widget          parent;             /* parent widget                     */
    XrmName         xrm_name;           /* widget resource name quarkified   */
    Boolean         being_destroyed;    /* marked for destroy                */
    XtCallbackList  destroy_callbacks;  /* who to call when widget destroyed */
    XtPointer       constraints;        /* constraint record                 */
} ObjectPart;

typedef struct _ObjectRec {
    ObjectPart  object;
} ObjectRec;

/********************************************************
 * Object Class Data Structures
 *
 ********************************************************/
/* these fields match CoreClassPart and can not be changed */
/* ideally these structures would only contain the fields required;
   but because the CoreClassPart cannot be changed at this late date
   extraneous fields are necessary to make the field offsets match */

typedef struct _ObjectClassPart {

    WidgetClass     superclass;         /* pointer to superclass ClassRec   */
    String          class_name;         /* widget resource class name       */
    Cardinal        widget_size;        /* size in bytes of widget record   */
    XtProc          class_initialize;   /* class initialization proc        */
    XtWidgetClassProc class_part_initialize; /* dynamic initialization      */
    XtEnum          class_inited;       /* has class been initialized?      */
    XtInitProc      initialize;         /* initialize subclass fields       */
    XtArgsProc      initialize_hook;    /* notify that initialize called    */
    XtProc          obj1;		/* NULL                             */
    XtPointer       obj2;               /* NULL                             */
    Cardinal        obj3;               /* NULL                             */
    XtResourceList  resources;          /* resources for subclass fields    */
    Cardinal        num_resources;      /* number of entries in resources   */
    XrmClass        xrm_class;          /* resource class quarkified        */
    Boolean         obj4;               /* NULL                             */
    XtEnum          obj5;               /* NULL                             */
    Boolean         obj6;               /* NULL				    */
    Boolean         obj7;               /* NULL                             */
    XtWidgetProc    destroy;            /* free data for subclass pointers  */
    XtProc          obj8;               /* NULL                             */
    XtProc          obj9;               /* NULL			            */
    XtSetValuesFunc set_values;         /* set subclass resource values     */
    XtArgsFunc      set_values_hook;    /* notify that set_values called    */
    XtProc          obj10;              /* NULL                             */
    XtArgsProc      get_values_hook;    /* notify that get_values called    */
    XtProc          obj11;              /* NULL                             */
    XtVersionType   version;            /* version of intrinsics used       */
    XtPointer       callback_private;   /* list of callback offsets         */
    String          obj12;              /* NULL                             */
    XtProc          obj13;              /* NULL                             */
    XtProc          obj14;              /* NULL                             */
    XtPointer       extension;          /* pointer to extension record      */
}ObjectClassPart;

typedef struct {
    XtPointer next_extension;	/* 1st 4 required for all extension records */
    XrmQuark record_type;	/* NULLQUARK; when on ObjectClassPart */
    long version;		/* must be XtObjectExtensionVersion */
    Cardinal record_size;	/* sizeof(ObjectClassExtensionRec) */
    XtAllocateProc allocate;
    XtDeallocateProc deallocate;
} ObjectClassExtensionRec, *ObjectClassExtension;

typedef struct _ObjectClassRec {
    ObjectClassPart object_class;
} ObjectClassRec;

externalref ObjectClassRec objectClassRec;

_XFUNCPROTOEND

#define XtObjectExtensionVersion 1L
#define XtInheritAllocate ((XtAllocateProc) _XtInherit)
#define XtInheritDeallocate ((XtDeallocateProc) _XtInherit)

#endif /*_Xt_ObjectP_h_*/
