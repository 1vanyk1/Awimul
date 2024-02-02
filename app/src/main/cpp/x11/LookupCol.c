#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <limits.h>
#include <stdio.h>
#include "headers/xlibint.h"
#include "xcms/Xcmsint.h"


Status
XLookupColor (
        register Display *dpy,
        Colormap cmap,
        _Xconst char *spec,
        XColor *def,
        XColor *scr)
{
    register size_t n;
    xLookupColorReply reply;
    register xLookupColorReq *req;
    XcmsCCC ccc;
    XcmsColor cmsColor_exact;

    if (spec == NULL)
        return 0;
    n = strlen (spec);
    if (n >= USHRT_MAX)
        return 0;
#ifdef XCMS
    /*
	 * Let's Attempt to use Xcms and i18n approach to Parse Color
	 */
	if ((ccc = XcmsCCCOfColormap(dpy, cmap)) != (XcmsCCC)NULL) {
	    const char *tmpName = spec;

	    switch (_XcmsResolveColorString(ccc, &tmpName, &cmsColor_exact,
					    XcmsRGBFormat)) {
	    case XcmsSuccess:
	    case XcmsSuccessWithCompression:
		_XcmsRGB_to_XColor(&cmsColor_exact, def, 1);
		memcpy((char *)scr, (char *)def, sizeof(XColor));
		_XUnresolveColor(ccc, scr);
		return(1);
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
     * Xcms and i18n methods failed, so lets pass it to the server
     * for parsing.
     */
    LockDisplay(dpy);
    GetReq (LookupColor, req);
    req->cmap = cmap;
    req->nbytes = n;
    req->length += (n + 3) >> 2;
    Data (dpy, spec, (long)n);
    if (!_XReply (dpy, (xReply *) &reply, 0, xTrue)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return (0);
    }
    def->red   = reply.exactRed;
    def->green = reply.exactGreen;
    def->blue  = reply.exactBlue;

    scr->red   = reply.screenRed;
    scr->green = reply.screenGreen;
    scr->blue  = reply.screenBlue;

    UnlockDisplay(dpy);
    SyncHandle();
    return (1);
}
