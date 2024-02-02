#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xrenderint.h"

Status
XRenderParseColor(Display *dpy, char *spec, XRenderColor *def)
{

    if (!strncmp (spec, "rgba:", 5))
    {
        unsigned short	elements[4];
        unsigned short	*pShort;
        int		i, n;
        char		c;

        spec += 5;
        /*
         * Attempt to parse the value portion.
         */
        pShort = elements;
        for (i = 0; i < 4; i++, pShort++, spec++) {
            n = 0;
            *pShort = 0;
            while (*spec != '/' && *spec != '\0') {
                if (++n > 4) {
                    return 0;
                }
                c = *spec++;
                *pShort <<= 4;
                if (c >= '0' && c <= '9')
                    *pShort |= c - '0';
                    /* assume string in lowercase
                    else if (c >= 'A' && c <= 'F')
                        *pShort |= c - ('A' - 10);
                    */
                else if (c >= 'a' && c <= 'f')
                    *pShort |= c - ('a' - 10);
                else return 0;
            }
            if (n == 0)
                return 0;
            if (n < 4) {
                *pShort = ((unsigned long)*pShort * 0xFFFF) / ((1 << n*4) - 1);
            }
        }
        def->red = elements[0];
        def->green = elements[1];
        def->blue = elements[2];
        def->alpha = elements[3];
    }
    else
    {
        XColor	    coreColor;
        Colormap    colormap;

        colormap = DefaultColormap (dpy, DefaultScreen (dpy));
        if (!XParseColor (dpy, colormap, spec, &coreColor))
            return 0;
        def->red = coreColor.red;
        def->green = coreColor.green;
        def->blue = coreColor.blue;
        def->alpha = 0xffff;
    }
    def->red = (def->red * def->alpha) / 0xffffU;
    def->green = (def->green * def->alpha) / 0xffffU;
    def->blue = (def->blue * def->alpha) / 0xffffU;
    return 1;
}
