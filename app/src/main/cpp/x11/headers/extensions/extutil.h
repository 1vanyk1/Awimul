#ifndef _EXTUTIL_H_
#define _EXTUTIL_H_

#include "Xext.h"

/*
 * We need to keep a list of open displays since the Xlib display list isn't
 * public.  We also have to per-display info in a separate block since it isn't
 * stored directly in the Display structure.
 */
typedef struct _XExtDisplayInfo {
    struct _XExtDisplayInfo *next;	/* keep a linked list */
    Display *display;			/* which display this is */
    XExtCodes *codes;			/* the extension protocol codes */
    XPointer data;			/* extra data for extension to use */
} XExtDisplayInfo;

typedef struct _XExtensionInfo {
    XExtDisplayInfo *head;		/* start of list */
    XExtDisplayInfo *cur;		/* most recently used */
    int ndisplays;			/* number of displays */
} XExtensionInfo;

typedef struct _XExtensionHooks {
    int (*create_gc)(
            Display*			/* display */,
            GC			/* gc */,
            XExtCodes*		/* codes */
    );
    int (*copy_gc)(
            Display*			/* display */,
            GC			/* gc */,
            XExtCodes*		/* codes */
    );
    int (*flush_gc)(
            Display*			/* display */,
            GC			/* gc */,
            XExtCodes*		/* codes */
    );
    int (*free_gc)(
            Display*			/* display */,
            GC			/* gc */,
            XExtCodes*		/* codes */
    );
    int (*create_font)(
            Display*			/* display */,
            XFontStruct*		/* fs */,
            XExtCodes*		/* codes */
    );
    int (*free_font)(
            Display*			/* display */,
            XFontStruct*		/* fs */,
            XExtCodes*		/* codes */
    );
    int (*close_display)(
            Display*			/* display */,
            XExtCodes*		/* codes */
    );
    Bool (*wire_to_event)(
            Display*			/* display */,
            XEvent*			/* re */,
            xEvent*			/* event */
    );
    Status (*event_to_wire)(
            Display*			/* display */,
            XEvent*			/* re */,
            xEvent*			/* event */
    );
    int (*error)(
            Display*			/* display */,
            xError*			/* err */,
            XExtCodes*		/* codes */,
            int*			/* ret_code */
    );
    char *(*error_string)(
            Display*		/* display */,
            int			/* code */,
            XExtCodes*		/* codes */,
            char*			/* buffer */,
            int			/* nbytes */
    );
} XExtensionHooks;

extern XExtensionInfo *XextCreateExtension(
        void
);
extern void XextDestroyExtension(
        XExtensionInfo*	/* info */
);
extern XExtDisplayInfo *XextAddDisplay(
        XExtensionInfo*	/* extinfo */,
        Display*		/* dpy */,
        _Xconst char*	/* ext_name */,
        XExtensionHooks*	/* hooks */,
        int			/* nevents */,
        XPointer		/* data */
);
extern int XextRemoveDisplay(
        XExtensionInfo*	/* extinfo */,
        Display*		/* dpy */
);
extern XExtDisplayInfo *XextFindDisplay(
        XExtensionInfo*	/* extinfo */,
        Display*		/* dpy */
);

#define XextHasExtension(i) ((i) && ((i)->codes))
#define XextCheckExtension(dpy,i,name,val) \
  if (!XextHasExtension(i)) { XMissingExtension (dpy, name); return val; }
#define XextSimpleCheckExtension(dpy,i,name) \
  if (!XextHasExtension(i)) { XMissingExtension (dpy, name); return; }


/*
 * helper macros to generate code that is common to all extensions; caller
 * should prefix it with static if extension source is in one file; this
 * could be a utility function, but have to stack 6 unused arguments for
 * something that is called many, many times would be bad.
 */
#define XEXT_GENERATE_FIND_DISPLAY(proc,extinfo,extname,hooks,nev,data) \
XExtDisplayInfo *proc (Display *dpy) \
{ \
    XExtDisplayInfo *dpyinfo; \
    if (!extinfo) { if (!(extinfo = XextCreateExtension())) return NULL; } \
    if (!(dpyinfo = XextFindDisplay (extinfo, dpy))) \
      dpyinfo = XextAddDisplay (extinfo,dpy,extname,hooks,nev,data); \
    return dpyinfo; \
}

#define XEXT_FIND_DISPLAY_PROTO(proc) \
	XExtDisplayInfo *proc(Display *dpy)

#define XEXT_GENERATE_CLOSE_DISPLAY(proc,extinfo) \
int proc (Display *dpy, XExtCodes *codes) \
{ \
    return XextRemoveDisplay (extinfo, dpy); \
}

#define XEXT_CLOSE_DISPLAY_PROTO(proc) \
	int proc(Display *dpy, XExtCodes *codes)

#define XEXT_GENERATE_ERROR_STRING(proc,extname,nerr,errl) \
char *proc (Display *dpy, int code, XExtCodes *codes, char *buf, int n) \
{  \
    code -= codes->first_error;  \
    if (code >= 0 && code < nerr) { \
	char tmp[256]; \
	snprintf (tmp, sizeof(tmp), "%s.%d", extname, code);            \
	XGetErrorDatabaseText (dpy, "XProtoError", tmp, errl[code], buf, n); \
	return buf; \
    } \
    return (char *)0; \
}

#define XEXT_ERROR_STRING_PROTO(proc) \
	char *proc(Display *dpy, int code, XExtCodes *codes, char *buf, int n)
#endif