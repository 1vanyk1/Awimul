#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <limits.h>
#include <stdio.h>
#include "headers/xlibint.h"
#include "xcms/Xcmsint.h"

Status
XAllocNamedColor(
        register Display *dpy,
        Colormap cmap,
        _Xconst char *colorname, /* STRING8 */
        XColor *hard_def, /* RETURN */
        XColor *exact_def) /* RETURN */
{

    long nbytes;
    xAllocNamedColorReply rep;
    xAllocNamedColorReq *req;

    XcmsCCC ccc;
    XcmsColor cmsColor_exact;
    Status ret;

    if (colorname != NULL && strlen(colorname) >= USHRT_MAX)
        return (0);

#ifdef XCMS
    /*
     * Let's Attempt to use Xcms and i18n approach to Parse Color
     */
    if ((ccc = XcmsCCCOfColormap(dpy, cmap)) != (XcmsCCC)NULL) {
	const char *tmpName = colorname;

	switch (_XcmsResolveColorString(ccc, &tmpName, &cmsColor_exact,
					XcmsRGBFormat)) {
	case XcmsSuccess:
	case XcmsSuccessWithCompression:
	    _XcmsRGB_to_XColor(&cmsColor_exact, exact_def, 1);
	    memcpy((char *)hard_def, (char *)exact_def, sizeof(XColor));
	    ret = XAllocColor(dpy, cmap, hard_def);
	    exact_def->pixel = hard_def->pixel;
	    return(ret);
	case XcmsFailure:
	case _XCMS_NEWNAME:
	    /*
	     * if the result was _XCMS_NEWNAME tmpName points to
	     * a string in cmsColNm.c:pairs table, for example,
	     * gray70 would become tekhvc:0.0/70.0/0.0
	     */
	    break;
	}
    }
#endif

    /*
     * Xcms and i18n approach failed.
     */
    LockDisplay(dpy);
    GetReq(AllocNamedColor, req);

    req->cmap = cmap;
    nbytes = req->nbytes = (CARD16) strlen(colorname);
    req->length += (nbytes + 3) >> 2; /* round up to mult of 4 */

    _XSend(dpy, colorname, nbytes);
    /* _XSend is more efficient that Data, since _XReply follows */

    if (!_XReply (dpy, (xReply *) &rep, 0, xTrue)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return (0);
    }

    exact_def->red = rep.exactRed;
    exact_def->green = rep.exactGreen;
    exact_def->blue = rep.exactBlue;

    hard_def->red = rep.screenRed;
    hard_def->green = rep.screenGreen;
    hard_def->blue = rep.screenBlue;

    exact_def->pixel = hard_def->pixel = rep.pixel;

    UnlockDisplay(dpy);
    SyncHandle();
    return (1);
}
