#ifndef _XCOMPOSITEINT_H_
#define _XCOMPOSITEINT_H_

#include <stdio.h>
#include "../headers/xlib.h"
#include "../headers/xlibint.h"
#include "../headers/xutil.h"
#include "../headers/extensions/compositeproto.h"
#include "../headers/extensions/Xcomposite.h"

typedef struct _XCompositeExtDisplayInfo {
    struct _XCompositeExtDisplayInfo  *next;    /* keep a linked list */
    Display                 *display;	    /* which display this is */
    XExtCodes               *codes;	    /* the extension protocol codes */
    int			    major_version;  /* -1 means we don't know */
    int			    minor_version;  /* -1 means we don't know */
} XCompositeExtDisplayInfo;

/* replaces XExtensionInfo */
typedef struct _XCompositeExtInfo {
    XCompositeExtDisplayInfo    *head;          /* start of the list */
    XCompositeExtDisplayInfo    *cur;           /* most recently used */
    int                     ndisplays;      /* number of displays */
} XCompositeExtInfo;

extern XCompositeExtInfo XCompositeExtensionInfo;
extern const char XCompositeExtensionName[];

XCompositeExtDisplayInfo *
XCompositeFindDisplay (Display *dpy);

#define XCompositeHasExtension(i) ((i) && ((i)->codes))

#define XCompositeCheckExtension(dpy,i,val) \
  if (!XCompositeHasExtension(i)) { return val; }

#define XCompositeSimpleCheckExtension(dpy,i) \
  if (!XCompositeHasExtension(i)) { return; }

#endif /* _XCOMPOSITEINT_H_ */
