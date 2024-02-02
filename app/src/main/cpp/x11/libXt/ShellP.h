#ifndef _XtShellPrivate_h
#define _XtShellPrivate_h

#include "Shell.h"
#include "IntrinsicP.h"

/* *****
 * ***** VendorP.h is included later on; it needs fields defined in the first
 * ***** part of this header file
 * *****
 */

_XFUNCPROTOBEGIN

/***********************************************************************
 *
 * Shell Widget Private Data
 *
 ***********************************************************************/

/* New fields for the Shell widget class record */

typedef struct {
    XtPointer       extension;          /* pointer to extension record      */
} ShellClassPart;

typedef struct {
    XtPointer next_extension;	/* 1st 4 mandated for all extension records */
    XrmQuark record_type;	/* NULLQUARK; on ShellClassPart */
    long version;		/* must be XtShellExtensionVersion */
    Cardinal record_size;	/* sizeof(ShellClassExtensionRec) */
    XtGeometryHandler root_geometry_manager;
} ShellClassExtensionRec, *ShellClassExtension;

#define XtShellExtensionVersion 1L
#define XtInheritRootGeometryManager ((XtGeometryHandler)_XtInherit)

typedef struct _ShellClassRec {
    CoreClassPart      core_class;
    CompositeClassPart composite_class;
    ShellClassPart  shell_class;
} ShellClassRec;

externalref ShellClassRec shellClassRec;

/* New fields for the shell widget */

typedef struct {
    char       *geometry;
    XtCreatePopupChildProc	create_popup_child_proc;
    XtGrabKind	grab_kind;
    Boolean	    spring_loaded;
    Boolean	    popped_up;
    Boolean	    allow_shell_resize;
    Boolean     client_specified; /* re-using old name */
#define _XtShellPositionValid	((Boolean)(1<<0))
#define _XtShellNotReparented	((Boolean)(1<<1))
#define _XtShellPPositionOK	((Boolean)(1<<2))
#define _XtShellGeometryParsed	((Boolean)(1<<3))
    Boolean	    save_under;
    Boolean	    override_redirect;

    XtCallbackList popup_callback;
    XtCallbackList popdown_callback;
    Visual*     visual;
} ShellPart;

typedef  struct {
    CorePart 	core;
    CompositePart 	composite;
    ShellPart 	shell;
} ShellRec, *ShellWidget;

/***********************************************************************
 *
 * OverrideShell Widget Private Data
 *
 ***********************************************************************/

/* New fields for the OverrideShell widget class record */

typedef struct {
    XtPointer       extension;          /* pointer to extension record      */
} OverrideShellClassPart;

typedef struct _OverrideShellClassRec {
    CoreClassPart      core_class;
    CompositeClassPart composite_class;
    ShellClassPart  shell_class;
    OverrideShellClassPart  override_shell_class;
} OverrideShellClassRec;

externalref OverrideShellClassRec overrideShellClassRec;

/* No new fields for the override shell widget */

typedef struct {int frabjous;} OverrideShellPart;

typedef  struct {
    CorePart 	core;
    CompositePart 	composite;
    ShellPart 	shell;
    OverrideShellPart override;
} OverrideShellRec, *OverrideShellWidget;

/***********************************************************************
 *
 * WMShell Widget Private Data
 *
 ***********************************************************************/

/* New fields for the WMShell widget class record */

typedef struct {
    XtPointer       extension;          /* pointer to extension record      */
} WMShellClassPart;

typedef struct _WMShellClassRec {
    CoreClassPart      core_class;
    CompositeClassPart composite_class;
    ShellClassPart  shell_class;
    WMShellClassPart wm_shell_class;
} WMShellClassRec;

externalref WMShellClassRec wmShellClassRec;

/* New fields for the WM shell widget */

typedef struct {
    char	   *title;
    int 	    wm_timeout;
    Boolean	    wait_for_wm;
    Boolean	    transient;
    Boolean     urgency;
    Widget      client_leader;
    String      window_role;
    struct _OldXSizeHints {	/* pre-R4 Xlib structure */
        long flags;
        int x, y;
        int width, height;
        int min_width, min_height;
        int max_width, max_height;
        int width_inc, height_inc;
        struct {
            int x;
            int y;
        } min_aspect, max_aspect;
    } size_hints;
    XWMHints    wm_hints;
    int base_width, base_height;
    int win_gravity;
    Atom title_encoding;
} WMShellPart;

typedef  struct {
    CorePart 	core;
    CompositePart 	composite;
    ShellPart 	shell;
    WMShellPart	wm;
} WMShellRec, *WMShellWidget;

_XFUNCPROTOEND

#include "VendorP.h"

        _XFUNCPROTOBEGIN

/***********************************************************************
 *
 * TransientShell Widget Private Data
 *
 ***********************************************************************/

/* New fields for the TransientShell widget class record */

typedef struct {
    XtPointer       extension;          /* pointer to extension record      */
} TransientShellClassPart;

typedef struct _TransientShellClassRec {
    CoreClassPart      core_class;
    CompositeClassPart composite_class;
    ShellClassPart  shell_class;
    WMShellClassPart   wm_shell_class;
    VendorShellClassPart vendor_shell_class;
    TransientShellClassPart transient_shell_class;
} TransientShellClassRec;

externalref TransientShellClassRec transientShellClassRec;

/* New fields for the transient shell widget */

typedef struct {
    Widget transient_for;
} TransientShellPart;

typedef  struct {
    CorePart 	core;
    CompositePart 	composite;
    ShellPart 	shell;
    WMShellPart	wm;
    VendorShellPart	vendor;
    TransientShellPart transient;
} TransientShellRec, *TransientShellWidget;

/***********************************************************************
 *
 * TopLevelShell Widget Private Data
 *
 ***********************************************************************/

/* New fields for the TopLevelShell widget class record */

typedef struct {
    XtPointer       extension;          /* pointer to extension record      */
} TopLevelShellClassPart;

typedef struct _TopLevelShellClassRec {
    CoreClassPart      core_class;
    CompositeClassPart composite_class;
    ShellClassPart  shell_class;
    WMShellClassPart   wm_shell_class;
    VendorShellClassPart vendor_shell_class;
    TopLevelShellClassPart top_level_shell_class;
} TopLevelShellClassRec;

externalref TopLevelShellClassRec topLevelShellClassRec;

/* New fields for the top level shell widget */

typedef struct {
    char	   *icon_name;
    Boolean	    iconic;
    Atom	    icon_name_encoding;
} TopLevelShellPart;

typedef  struct {
    CorePart 	core;
    CompositePart 	composite;
    ShellPart 	shell;
    WMShellPart	wm;
    VendorShellPart	vendor;
    TopLevelShellPart topLevel;
} TopLevelShellRec, *TopLevelShellWidget;

/***********************************************************************
 *
 * ApplicationShell Widget Private Data
 *
 ***********************************************************************/

/* New fields for the ApplicationShell widget class record */

typedef struct {
    XtPointer       extension;          /* pointer to extension record      */
} ApplicationShellClassPart;

typedef struct _ApplicationShellClassRec {
    CoreClassPart      core_class;
    CompositeClassPart composite_class;
    ShellClassPart  shell_class;
    WMShellClassPart   wm_shell_class;
    VendorShellClassPart vendor_shell_class;
    TopLevelShellClassPart top_level_shell_class;
    ApplicationShellClassPart application_shell_class;
} ApplicationShellClassRec;

externalref ApplicationShellClassRec applicationShellClassRec;

/* New fields for the application shell widget */

typedef struct {
#if defined(__cplusplus) || defined(c_plusplus)
    char *c_class;
#else
    char *class;
#endif
    XrmClass xrm_class;
    int argc;
    _XtString *argv;
} ApplicationShellPart;

typedef  struct {
    CorePart 	core;
    CompositePart 	composite;
    ShellPart 	shell;
    WMShellPart	wm;
    VendorShellPart	vendor;
    TopLevelShellPart topLevel;
    ApplicationShellPart application;
} ApplicationShellRec, *ApplicationShellWidget;

/***********************************************************************
 *
 * SessionShell Widget Private Data
 *
 ***********************************************************************/

/* New fields for the SessionShell widget class record */

typedef struct {
    XtPointer       extension;          /* pointer to extension record */
} SessionShellClassPart;

typedef struct _SessionShellClassRec {
    CoreClassPart      core_class;
    CompositeClassPart composite_class;
    ShellClassPart  shell_class;
    WMShellClassPart   wm_shell_class;
    VendorShellClassPart vendor_shell_class;
    TopLevelShellClassPart top_level_shell_class;
    ApplicationShellClassPart application_shell_class;
    SessionShellClassPart session_shell_class;
} SessionShellClassRec;

externalref SessionShellClassRec sessionShellClassRec;

typedef struct _XtSaveYourselfRec *XtSaveYourself; /* implementation-private */

/* New fields for the session shell widget */

typedef struct {
    SmcConn         connection;
    _XtString       session_id;
    _XtString*      restart_command;
    _XtString*      clone_command;
    _XtString*      discard_command;
    _XtString*      resign_command;
    _XtString*      shutdown_command;
    _XtString*      environment;
    _XtString       current_dir;
    String          program_path;
    unsigned char   restart_style;
    unsigned char   checkpoint_state;
    Boolean         join_session;
    XtCallbackList  save_callbacks;
    XtCallbackList  interact_callbacks;
    XtCallbackList  cancel_callbacks;
    XtCallbackList  save_complete_callbacks;
    XtCallbackList  die_callbacks;
    XtCallbackList  error_callbacks;
    XtSaveYourself  save;
    XtInputId       input_id;
    XtPointer       ses20;
    XtPointer       ses19;
    XtPointer       ses18;
    XtPointer       ses17;
    XtPointer       ses16;
    XtPointer       ses15;
    XtPointer       ses14;
    XtPointer       ses13;
    XtPointer       ses12;
    XtPointer       ses11;
    XtPointer       ses10;
    XtPointer       ses9;
    XtPointer       ses8;
    XtPointer       ses7;
    XtPointer       ses6;
    XtPointer       ses5;
    XtPointer       ses4;
    XtPointer       ses3;
    XtPointer       ses2;
    XtPointer       ses1;
} SessionShellPart;

typedef  struct {
    CorePart 	core;
    CompositePart 	composite;
    ShellPart 	shell;
    WMShellPart	wm;
    VendorShellPart	vendor;
    TopLevelShellPart topLevel;
    ApplicationShellPart application;
    SessionShellPart session;
} SessionShellRec, *SessionShellWidget;

_XFUNCPROTOEND

#endif /* _XtShellPrivate_h */
