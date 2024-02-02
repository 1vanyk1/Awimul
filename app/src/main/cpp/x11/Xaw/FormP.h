#ifndef _XawFormP_h
#define _XawFormP_h

#include "../headers/xfuncproto.h"

#include "Form.h"
#include "XawInit.h"

_XFUNCPROTOBEGIN

#define XtREdgeType "EdgeType"

typedef enum {
    LayoutPending,
    LayoutInProgress,
    LayoutDone
} LayoutState;

#define XtInheritLayout							\
((Boolean (*)(FormWidget, unsigned int, unsigned int, Bool))_XtInherit)

typedef struct {
    Boolean(*layout)(FormWidget, unsigned int, unsigned int, Bool);
#ifndef OLDXAW
    XtPointer extension;
#endif
} FormClassPart;

typedef struct _FormClassRec {
    CoreClassPart	core_class;
    CompositeClassPart	composite_class;
    ConstraintClassPart	constraint_class;
    FormClassPart	form_class;
} FormClassRec;

extern FormClassRec formClassRec;

typedef struct _FormPart {
    /* resources */
    int		default_spacing;    /* default distance between children */

    /* private */
    Dimension	old_width, old_height; /* reference value for *_virtual  */
    int		no_refigure;	    /* no re-layout while > 0		 */
    Boolean	needs_relayout;	    /* next time no_refigure == 0	 */
    Boolean	resize_in_layout;   /* should layout() do geom request?  */
    Dimension	preferred_width, preferred_height; /* cached from layout */
    Boolean	resize_is_no_op;    /* Causes resize to take not action  */
#ifndef OLDXAW
    XawDisplayList *display_list;
    XtPointer pad[4];	/* for future use and keep binary compatability */
#endif
} FormPart;

typedef struct _FormRec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    FormPart		form;
} FormRec;

typedef struct _FormConstraintsPart {
    /* resources */
    XtEdgeType top, bottom, left, right;/* where to drag edge on resize */
    int		dx;		/* desired horiz offset			*/
    int		dy;		/* desired vertical offset		*/
    Widget	horiz_base;	/* measure dx from here if non-null	*/
    Widget	vert_base;	/* measure dy from here if non-null	*/
    Boolean	allow_resize;	/* True if child may request resize	*/

    /* private */
    short	virtual_width, virtual_height;
    Position	new_x, new_y;
    LayoutState	layout_state;	/* temporary layout state		*/
    Boolean	deferred_resize;/* was resized while no_refigure is set */
#ifndef OLDXAW
    short	virtual_x, virtual_y;
    XtPointer	pad[2];		/* leave some space for further optimizations
				 * in the form widget geometry
				 */
#endif
} FormConstraintsPart;

typedef struct _FormConstraintsRec {
    FormConstraintsPart	form;
} FormConstraintsRec, *FormConstraints;

_XFUNCPROTOEND

#endif /* _XawFormP_h */
