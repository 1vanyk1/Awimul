#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <limits.h>
#include <stdio.h>
#include "headers/xlibint.h"
#include "xcms/Xcmsint.h"


Status
XParseColor (
        register Display *dpy,
        Colormap cmap,
        _Xconst char *spec,
        XColor *def)
{
    register size_t n, i;
    int r, g, b;
    char c;
    XcmsCCC ccc;
    XcmsColor cmsColor;

    if (!spec) return(0);
    n = strlen (spec);
    if (n >= USHRT_MAX)
        return(0);
    if (*spec == '#') {
        /*
         * RGB
         */
        spec++;
        n--;
        if (n != 3 && n != 6 && n != 9 && n != 12)
            return (0);
        n /= 3;
        g = b = 0;
        do {
            r = g;
            g = b;
            b = 0;
            for (i = 0; i < n; i++) {
                c = *spec++;
                b <<= 4;
                if (c >= '0' && c <= '9')
                    b |= c - '0';
                else if (c >= 'A' && c <= 'F')
                    b |= c - ('A' - 10);
                else if (c >= 'a' && c <= 'f')
                    b |= c - ('a' - 10);
                else return (0);
            }
        } while (*spec != '\0');
        n <<= 2;
        n = 16 - n;
        def->red = r << n;
        def->green = g << n;
        def->blue = b << n;
        def->flags = DoRed | DoGreen | DoBlue;
        return (1);
    }


#ifdef XCMS
    /*
	 * Let's Attempt to use Xcms and i18n approach to Parse Color
	 */
	if ((ccc = XcmsCCCOfColormap(dpy, cmap)) != (XcmsCCC)NULL) {
	    const char *tmpName = spec;

	    switch (_XcmsResolveColorString(ccc, &tmpName, &cmsColor,
					    XcmsRGBFormat)) {
	    case XcmsSuccess:
	    case XcmsSuccessWithCompression:
		cmsColor.pixel = def->pixel;
		_XcmsRGB_to_XColor(&cmsColor, def, 1);
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
    {
        xLookupColorReply reply;
        register xLookupColorReq *req;
        LockDisplay(dpy);
        GetReq (LookupColor, req);
        req->cmap = cmap;
        req->nbytes = (CARD16) (n = strlen(spec));
        req->length += (n + 3) >> 2;
        Data (dpy, spec, (long)n);
        if (!_XReply (dpy, (xReply *) &reply, 0, xTrue)) {
            UnlockDisplay(dpy);
            SyncHandle();
            return (0);
        }
        def->red = reply.exactRed;
        def->green = reply.exactGreen;
        def->blue = reply.exactBlue;
        def->flags = DoRed | DoGreen | DoBlue;
        UnlockDisplay(dpy);
        SyncHandle();
        return (1);
    }
}
