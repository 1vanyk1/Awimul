#ifndef _XFIXESINT_H_
#define _XFIXESINT_H_

#include <stdio.h>
#include "../headers/xlib.h"
#include "../headers/xlibint.h"
#include "../headers/xutil.h"
#include "../headers/extensions/Xfixes.h"
#include "../headers/extensions/xfixesproto.h"

typedef struct _XFixesExtDisplayInfo {
    struct _XFixesExtDisplayInfo  *next;    /* keep a linked list */
    Display                 *display;	    /* which display this is */
    XExtCodes               *codes;	    /* the extension protocol codes */
    int			    major_version;  /* -1 means we don't know */
    int			    minor_version;  /* -1 means we don't know */
} XFixesExtDisplayInfo;

/* replaces XExtensionInfo */
typedef struct _XFixesExtInfo {
    XFixesExtDisplayInfo    *head;          /* start of the list */
    XFixesExtDisplayInfo    *cur;           /* most recently used */
    int                     ndisplays;      /* number of displays */
} XFixesExtInfo;

extern XFixesExtInfo XFixesExtensionInfo;
extern char XFixesExtensionName[];

XFixesExtDisplayInfo *
XFixesFindDisplay (Display *dpy);

#define XFixesHasExtension(i) ((i) && ((i)->codes))

#define XFixesCheckExtension(dpy,i,val) \
  if (!XFixesHasExtension(i)) { return val; }

#define XFixesSimpleCheckExtension(dpy,i) \
  if (!XFixesHasExtension(i)) { return; }

#endif /* _XFIXESINT_H_ */
