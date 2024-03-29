#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xrenderint.h"
#include <limits.h>

XFilters *
XRenderQueryFilters (Display *dpy, Drawable drawable)
{
    XRenderExtDisplayInfo		*info = XRenderFindDisplay (dpy);
    XRenderInfo			*xri;
    xRenderQueryFiltersReq	*req;
    xRenderQueryFiltersReply	rep;
    XFilters			*filters;
    char			*name;
    char			len;
    int				i;
    unsigned long		nbytes, nbytesAlias, nbytesName, reply_left;

    if (!RenderHasExtension (info))
        return NULL;

    if (!XRenderQueryFormats (dpy))
        return NULL;

    xri = info->info;
    if (xri->minor_version < 6)
        return NULL;

    LockDisplay (dpy);
    GetReq (RenderQueryFilters, req);
    req->reqType = info->codes->major_opcode;
    req->renderReqType = X_RenderQueryFilters;
    req->drawable = drawable;
    if (!_XReply (dpy, (xReply *) &rep, 0, xFalse))
    {
        UnlockDisplay (dpy);
        SyncHandle ();
        return NULL;
    }

    /*
     * Limit each component of combined size to 1/4 the max, which is far
     * more than they should ever possibly need.
     */
    if ((rep.length < (INT_MAX >> 2)) &&
        (rep.numFilters < ((INT_MAX / 4) / sizeof (char *))) &&
        (rep.numAliases < ((INT_MAX / 4) / sizeof (short)))) {
        /*
         * Compute total number of bytes for filter names
         */
        nbytes = (unsigned long)rep.length << 2;
        nbytesAlias = rep.numAliases * 2;
        if (rep.numAliases & 1)
            nbytesAlias += 2;
        nbytesName = nbytes - nbytesAlias;

        /*
         * Allocate one giant block for the whole data structure
         */
        filters = Xmalloc (sizeof (XFilters) +
                           (rep.numFilters * sizeof (char *)) +
                           (rep.numAliases * sizeof (short)) +
                           nbytesName);
    } else
        filters = NULL;

    if (!filters)
    {
        _XEatDataWords(dpy, rep.length);
        UnlockDisplay (dpy);
        SyncHandle ();
        return NULL;
    }

    /*
     * Layout:
     *	XFilters
     *	numFilters  char * pointers to filter names
     *	numAliases  short alias values
     *	nbytesName  char strings
     */

    filters->nfilter = rep.numFilters;
    filters->nalias = rep.numAliases;
    filters->filter = (char **) (filters + 1);
    filters->alias = (short *) (filters->filter + rep.numFilters);
    name = (char *) (filters->alias + rep.numAliases);

    /*
     * Read the filter aliases
     */
    _XRead16Pad (dpy, filters->alias, 2 * rep.numAliases);
    reply_left = 8 + rep.length - 2 * rep.numAliases;;

    /*
     * Read the filter names
     */
    for (i = 0; i < rep.numFilters; i++)
    {
        int	l;
        _XRead (dpy, &len, 1);
        reply_left--;
        l = len & 0xff;
        if ((unsigned long)l + 1 > nbytesName) {
            _XEatDataWords(dpy, reply_left);
            Xfree(filters);
            UnlockDisplay (dpy);
            SyncHandle ();
            return NULL;
        }
        nbytesName -= l + 1;
        filters->filter[i] = name;
        _XRead (dpy, name, l);
        reply_left -= l;
        name[l] = '\0';
        name += l + 1;
    }
    i = name - (char *) (filters->alias + rep.numAliases);

    if (i & 3)
        _XEatData (dpy, 4 - (i & 3));

    UnlockDisplay (dpy);
    SyncHandle ();
    return filters;
}

void
XRenderSetPictureFilter  (Display   *dpy,
                          Picture   picture,
                          const char *filter,
                          XFixed    *params,
                          int	    nparams)
{
    XRenderExtDisplayInfo		*info = XRenderFindDisplay (dpy);
    xRenderSetPictureFilterReq	*req;
    int				nbytes = strlen (filter);

    RenderSimpleCheckExtension (dpy, info);
    LockDisplay(dpy);
    GetReq(RenderSetPictureFilter, req);
    req->reqType = info->codes->major_opcode;
    req->renderReqType = X_RenderSetPictureFilter;
    req->picture = picture;
    req->nbytes = nbytes;
    req->length += ((nbytes + 3) >> 2) + nparams;
    Data (dpy, filter, nbytes);
    Data (dpy, (_Xconst char *)params, nparams << 2);
    UnlockDisplay(dpy);
    SyncHandle();
}