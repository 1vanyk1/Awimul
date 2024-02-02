#include "xcompositeint.h"

XCompositeExtInfo XCompositeExtensionInfo;

const char XCompositeExtensionName[] = COMPOSITE_NAME;

/*
 * XCompositeExtRemoveDisplay - remove the indicated display from the
 * extension object. (Replaces XextRemoveDisplay.)
 */
static int
XCompositeExtRemoveDisplay (XCompositeExtInfo *extinfo, Display *dpy)
{
    XCompositeExtDisplayInfo *info, *prev;

    /*
     * locate this display and its back link so that it can be removed
     */
    _XLockMutex(_Xglobal_lock);
    prev = NULL;
    for (info = extinfo->head; info; info = info->next) {
        if (info->display == dpy) break;
        prev = info;
    }
    if (!info) {
        _XUnlockMutex(_Xglobal_lock);
        return 0;		/* hmm, actually an error */
    }

    /*
     * remove the display from the list; handles going to zero
     */
    if (prev)
        prev->next = info->next;
    else
        extinfo->head = info->next;

    extinfo->ndisplays--;
    if (info == extinfo->cur) extinfo->cur = NULL;  /* flush cache */
    _XUnlockMutex(_Xglobal_lock);

    Xfree ((char *) info);
    return 1;
}

static int
XCompositeCloseDisplay (Display *dpy, XExtCodes *codes)
{
    return XCompositeExtRemoveDisplay (&XCompositeExtensionInfo, dpy);
}

/*
 * XCompositeExtAddDisplay - add a display to this extension. (Replaces
 * XextAddDisplay)
 */
static XCompositeExtDisplayInfo *
XCompositeExtAddDisplay (XCompositeExtInfo	*extinfo,
                         Display		*dpy,
                         const char		*ext_name)
{
    XCompositeExtDisplayInfo    *info;

    info = (XCompositeExtDisplayInfo *) Xmalloc (sizeof (XCompositeExtDisplayInfo));
    if (!info) return NULL;
    info->display = dpy;

    info->codes = XInitExtension (dpy, ext_name);

    /*
     * if the server has the extension, then we can initialize the
     * appropriate function vectors
     */
    if (info->codes) {
        xCompositeQueryVersionReply	rep;
        xCompositeQueryVersionReq	*req;
        XESetCloseDisplay (dpy, info->codes->extension,
                           XCompositeCloseDisplay);
        /*
         * Get the version info
         */
        LockDisplay (dpy);
        GetReq (CompositeQueryVersion, req);
        req->reqType = info->codes->major_opcode;
        req->compositeReqType = X_CompositeQueryVersion;
        req->majorVersion = COMPOSITE_MAJOR;
        req->minorVersion = COMPOSITE_MINOR;
        if (!_XReply (dpy, (xReply *) &rep, 0, xTrue))
        {
            UnlockDisplay (dpy);
            SyncHandle ();
            Xfree(info);
            return NULL;
        }
        info->major_version = rep.majorVersion;
        info->minor_version = rep.minorVersion;
        UnlockDisplay (dpy);
        SyncHandle ();
    } else {
        /* The server doesn't have this extension.
         * Use a private Xlib-internal extension to hang the close_display
         * hook on so that the "cache" (extinfo->cur) is properly cleaned.
         * (XBUG 7955)
         */
        XExtCodes *codes = XAddExtension(dpy);
        if (!codes) {
            Xfree(info);
            return NULL;
        }
        XESetCloseDisplay (dpy, codes->extension, XCompositeCloseDisplay);
    }

    /*
     * now, chain it onto the list
     */
    _XLockMutex(_Xglobal_lock);
    info->next = extinfo->head;
    extinfo->head = info;
    extinfo->cur = info;
    extinfo->ndisplays++;
    _XUnlockMutex(_Xglobal_lock);
    return info;
}

/*
 * XCompositeExtFindDisplay - look for a display in this extension; keeps a
 * cache of the most-recently used for efficiency. (Replaces
 * XextFindDisplay.)
 */
static XCompositeExtDisplayInfo *
XCompositeExtFindDisplay (XCompositeExtInfo *extinfo,
                          Display	    *dpy)
{
    XCompositeExtDisplayInfo *info;

    /*
     * see if this was the most recently accessed display
     */
    if ((info = extinfo->cur) && info->display == dpy)
        return info;

    /*
     * look for display in list
     */
    _XLockMutex(_Xglobal_lock);
    for (info = extinfo->head; info; info = info->next) {
        if (info->display == dpy) {
            extinfo->cur = info;     /* cache most recently used */
            _XUnlockMutex(_Xglobal_lock);
            return info;
        }
    }
    _XUnlockMutex(_Xglobal_lock);

    return NULL;
}

XCompositeExtDisplayInfo *
XCompositeFindDisplay (Display *dpy)
{
    XCompositeExtDisplayInfo *info;

    info = XCompositeExtFindDisplay (&XCompositeExtensionInfo, dpy);
    if (!info)
        info = XCompositeExtAddDisplay (&XCompositeExtensionInfo, dpy,
                                        XCompositeExtensionName);
    return info;
}


Bool
XCompositeQueryExtension (Display *dpy,
                          int *event_base_return,
                          int *error_base_return)
{
    XCompositeExtDisplayInfo *info = XCompositeFindDisplay (dpy);

    if (XCompositeHasExtension(info))
    {
        *event_base_return = info->codes->first_event;
        *error_base_return = info->codes->first_error;
        return True;
    }
    else
        return False;
}

Status
XCompositeQueryVersion (Display *dpy,
                        int	    *major_version_return,
                        int	    *minor_version_return)
{
    XCompositeExtDisplayInfo	*info = XCompositeFindDisplay (dpy);

    XCompositeCheckExtension (dpy, info, 0);
    *major_version_return = info->major_version;
    *minor_version_return = info->minor_version;
    return 1;
}

int
XCompositeVersion (void)
{
    return XCOMPOSITE_VERSION;
}

void
XCompositeRedirectWindow (Display *dpy, Window window, int update)
{
    XCompositeExtDisplayInfo	    *info = XCompositeFindDisplay (dpy);
    xCompositeRedirectWindowReq	    *req;

    XCompositeSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (CompositeRedirectWindow, req);
    req->reqType = info->codes->major_opcode;
    req->compositeReqType = X_CompositeRedirectWindow;
    req->window = window;
    req->update = update;
    UnlockDisplay (dpy);
    SyncHandle ();
}

void
XCompositeRedirectSubwindows (Display *dpy, Window window, int update)
{
    XCompositeExtDisplayInfo	    *info = XCompositeFindDisplay (dpy);
    xCompositeRedirectSubwindowsReq *req;

    XCompositeSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (CompositeRedirectSubwindows, req);
    req->reqType = info->codes->major_opcode;
    req->compositeReqType = X_CompositeRedirectSubwindows;
    req->window = window;
    req->update = update;
    UnlockDisplay (dpy);
    SyncHandle ();
}

void
XCompositeUnredirectWindow (Display *dpy, Window window, int update)
{
    XCompositeExtDisplayInfo	    *info = XCompositeFindDisplay (dpy);
    xCompositeUnredirectWindowReq   *req;

    XCompositeSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (CompositeUnredirectWindow, req);
    req->reqType = info->codes->major_opcode;
    req->compositeReqType = X_CompositeUnredirectWindow;
    req->window = window;
    req->update = update;
    UnlockDisplay (dpy);
    SyncHandle ();
}

void
XCompositeUnredirectSubwindows (Display *dpy, Window window, int update)
{
    XCompositeExtDisplayInfo		*info = XCompositeFindDisplay (dpy);
    xCompositeUnredirectSubwindowsReq	*req;

    XCompositeSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (CompositeUnredirectSubwindows, req);
    req->reqType = info->codes->major_opcode;
    req->compositeReqType = X_CompositeUnredirectSubwindows;
    req->window = window;
    req->update = update;
    UnlockDisplay (dpy);
    SyncHandle ();
}

XserverRegion
XCompositeCreateRegionFromBorderClip (Display *dpy, Window window)
{
    XCompositeExtDisplayInfo		    *info = XCompositeFindDisplay (dpy);
    xCompositeCreateRegionFromBorderClipReq *req;
    XserverRegion			    region;

    XCompositeCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq (CompositeCreateRegionFromBorderClip, req);
    req->reqType = info->codes->major_opcode;
    req->compositeReqType = X_CompositeCreateRegionFromBorderClip;
    req->window = window;
    region = req->region = XAllocID (dpy);
    UnlockDisplay (dpy);
    SyncHandle ();
    return region;
}

Pixmap
XCompositeNameWindowPixmap (Display *dpy, Window window)
{
    XCompositeExtDisplayInfo	    *info = XCompositeFindDisplay (dpy);
    xCompositeNameWindowPixmapReq   *req;
    Pixmap			    pixmap;

    XCompositeCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq (CompositeNameWindowPixmap, req);
    req->reqType = info->codes->major_opcode;
    req->compositeReqType = X_CompositeNameWindowPixmap;
    req->window = window;
    pixmap = req->pixmap = XAllocID (dpy);
    UnlockDisplay (dpy);
    SyncHandle ();
    return pixmap;
}

Window
XCompositeGetOverlayWindow (Display *dpy, Window window)
{
    XCompositeExtDisplayInfo	    *info = XCompositeFindDisplay (dpy);
    xCompositeGetOverlayWindowReq   *req;
    xCompositeGetOverlayWindowReply rep;

    XCompositeCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq (CompositeGetOverlayWindow, req);
    req->reqType = info->codes->major_opcode;
    req->compositeReqType = X_CompositeGetOverlayWindow;
    req->window = window;
    if (!_XReply (dpy, (xReply *) &rep, 0, xFalse))
    {
        UnlockDisplay (dpy);
        SyncHandle ();
        return 0;
    }

    UnlockDisplay (dpy);
    SyncHandle ();

    return rep.overlayWin;
}

void
XCompositeReleaseOverlayWindow (Display *dpy, Window window)
{
    XCompositeExtDisplayInfo	    *info = XCompositeFindDisplay (dpy);
    xCompositeReleaseOverlayWindowReq   *req;

    XCompositeSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (CompositeReleaseOverlayWindow, req);
    req->reqType = info->codes->major_opcode;
    req->compositeReqType = X_CompositeReleaseOverlayWindow;
    req->window = window;
    UnlockDisplay (dpy);
    SyncHandle ();
}
