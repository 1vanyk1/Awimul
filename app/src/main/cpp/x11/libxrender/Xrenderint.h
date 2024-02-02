#ifndef _XRENDERINT_H_
#define _XRENDERINT_H_

//#include "config.h"
#include "../headers/xlibint.h"
#include "../headers/xutil.h"
#include "../headers/extensions/renderproto.h"
#include "../headers/extensions/Xrender.h"

typedef struct {
    Visual		*visual;
    XRenderPictFormat	*format;
} XRenderVisual;

typedef struct {
    int			depth;
    int			nvisuals;
    XRenderVisual	*visuals;
} XRenderDepth;

typedef struct {
    XRenderDepth	*depths;
    int			ndepths;
    XRenderPictFormat	*fallback;
    int			subpixel;
} XRenderScreen;

typedef struct _XRenderInfo {
    int			major_version;
    int			minor_version;
    XRenderPictFormat	*format;
    int			nformat;
    XRenderScreen	*screen;
    int			nscreen;
    XRenderDepth	*depth;
    int			ndepth;
    XRenderVisual	*visual;
    int			nvisual;
    int			*subpixel;
    int			nsubpixel;
    char		**filter;
    int			nfilter;
    short    		*filter_alias;
    int			nfilter_alias;
} XRenderInfo;

/* replaces XRenderExtDisplayInfo */
typedef struct _XRenderExtDisplayInfo {
    struct _XRenderExtDisplayInfo *next;    /* keep a linked list */
    Display                       *display; /* which display this is */
    XExtCodes                     *codes;   /* the extension protocol codes */
    XRenderInfo                   *info;    /* extra data for the extension to use */
} XRenderExtDisplayInfo;

/* replaces XExtensionInfo */
typedef struct _XRenderExtInfo {
    XRenderExtDisplayInfo  *head;           /* start of the list */
    XRenderExtDisplayInfo  *cur;            /* most recently used */
    int                     ndisplays;      /* number of displays */
} XRenderExtInfo;

extern XRenderExtInfo XRenderExtensionInfo;
extern char XRenderExtensionName[];

XRenderExtDisplayInfo *
XRenderFindDisplay (Display *dpy);

#define RenderHasExtension(i) ((i) && ((i)->codes))

#define RenderCheckExtension(dpy,i,val) \
  if (!RenderHasExtension(i)) { return val; }

#define RenderSimpleCheckExtension(dpy,i) \
  if (!RenderHasExtension(i)) { return; }

/*
 * Xlib uses long for 32-bit values.  Xrender uses int.  This
 * matters on alpha.  Note that this macro assumes that int is 32 bits
 * except on WORD64 machines where it is 64 bits.
 */

#ifdef WORD64
#define DataInt32(dpy,d,len)	Data32(dpy,(long *) (d),len)
#else
#define DataInt32(dpy,d,len)	Data(dpy,(char *) (d),len)
#endif

#endif /* _XRENDERINT_H_ */