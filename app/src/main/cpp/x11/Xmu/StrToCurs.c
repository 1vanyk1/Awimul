#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include	"../libXt/Intrinsic.h"
#include	"../libXt/StringDefs.h"
#include	"Converters.h"
#include	"Drawing.h"
#include	"CurUtil.h"
#include	"CharSet.h"

#include <stdlib.h>

#ifdef _POSIX_SOURCE
#include <limits.h>
#else
#define _POSIX_SOURCE
#include <limits.h>
#undef _POSIX_SOURCE
#endif

#ifndef PATH_MAX
#ifdef WIN32
#define PATH_MAX 512
#else
#include <sys/param.h>
#endif
#ifndef PATH_MAX
#ifdef MAXPATHLEN
#define PATH_MAX MAXPATHLEN
#else
#define PATH_MAX 1024
#endif
#endif
#endif /* PATH_MAX */

/* Kludge source to avoid encountering broken shared library linkers
   which insist on resolving references unused by the application,
   and broken object file formats that don't correctly distinguish
   references to procedures from references to data.
 */
#if defined(SVR4)
#define XMU_KLUDGE
#endif

/*
 * XmuConvertStringToCursor:
 *
 * allows String to specify a standard cursor name (from cursorfont.h), a
 * font name and glyph index of the form "FONT fontname index [[font] index]",
 * or a bitmap file name (absolute, or relative to the global resource
 * bitmapFilePath, class BitmapFilePath).  If the resource is not
 * defined, the default value is the build symbol BITMAPDIR.
 *
 * shares lots of code with XmuCvtStringToPixmap, but unfortunately
 * can't use it as the hotspot info is lost.
 *
 * To use, include the following in your ClassInitialize procedure:

static XtConvertArgRec screenConvertArg[] = {
    {XtBaseOffset, (XtPointer) XtOffsetOf(WidgetRec, core.screen),
     sizeof(Screen *)}
};

    XtAddConverter(XtRString, XtRCursor, XmuCvtStringToCursor,
		   screenConvertArg, XtNumber(screenConvertArg));
 *
 */

#define	done(address, type) \
	{ (*toVal).size = sizeof(type); (*toVal).addr = (XPointer) address; }

#define FONTSPECIFIER		"FONT "

/*ARGSUSED*/
void
XmuCvtStringToCursor(XrmValuePtr args, Cardinal *num_args,
                     XrmValuePtr fromVal, XrmValuePtr toVal)
{
    static Cursor cursor;		/* static for cvt magic */
    char *name = (char *)fromVal->addr;
    Screen *screen;
    register int i;
    char maskname[PATH_MAX];
    Pixmap source, mask = 0;
    /* XXX - make fg/bg resources */
    static XColor bgColor = {0, 0xffff, 0xffff, 0xffff};
    static XColor fgColor = {0, 0, 0, 0};
    int xhot, yhot;
    int len;


    if (*num_args != 1)
        XtErrorMsg("wrongParameters","cvtStringToCursor","XtToolkitError",
                   "String to cursor conversion needs screen argument",
                   (String *)NULL, (Cardinal *)NULL);

    if (XmuCompareISOLatin1(name, "None") == 0)
    {
        cursor = None;
        done(&cursor, Cursor);
        return;
    }

    screen = *((Screen **) args[0].addr);

    if (0 == strncmp(FONTSPECIFIER, name, strlen(FONTSPECIFIER))) {
        char source_name[PATH_MAX], mask_name[PATH_MAX];
        int source_char, mask_char, fields = 0;
        Font source_font, mask_font;
        XrmValue fromString, toFont;
        XrmValue cvtArg;
        Boolean success;
        Display *dpy = DisplayOfScreen(screen);
        char *strspec = NULL;
        int strspeclen;
#ifdef XMU_KLUDGE
        Cardinal num;
#endif

        strspeclen = strlen("FONT %s %d %s %d") + 21;
        strspec = XtMalloc(strspeclen);
        if (strspec != NULL) {
            snprintf(strspec, strspeclen, "FONT %%%lds %%d %%%lds %%d",
                     (unsigned long)sizeof(source_name) - 1,
                     (unsigned long)sizeof(mask_name) - 1);
            fields = sscanf(name, strspec,
                            source_name, &source_char,
                            mask_name, &mask_char);
            XtFree(strspec);
        }
        if (fields < 2) {
            XtStringConversionWarning(name, XtRCursor);
            return;
        }

        fromString.addr = source_name;
        fromString.size = strlen(source_name) + 1;
        toFont.addr = (XPointer) &source_font;
        toFont.size = sizeof(Font);
        cvtArg.addr = (XPointer) &dpy;
        cvtArg.size = sizeof(Display *);
        /* XXX using display of screen argument as message display */
#ifdef XMU_KLUDGE
        /* XXX Sacrifice caching */
	num = 1;
	success = XtCvtStringToFont(dpy, &cvtArg, &num, &fromString, &toFont,
				    NULL);
#else
        success = XtCallConverter(dpy, XtCvtStringToFont, &cvtArg,
                                  (Cardinal)1, &fromString, &toFont, NULL);
#endif
        if (!success) {
            XtStringConversionWarning(name, XtRCursor);
            return;
        }

        switch (fields) {
            case 2:		/* defaulted mask font & char */
                mask_font = source_font;
                mask_char = source_char;
                break;

            case 3:		/* defaulted mask font */
                mask_font = source_font;
                mask_char = atoi(mask_name);
                break;

            case 4:		/* specified mask font & char */
                fromString.addr = mask_name;
                fromString.size = strlen(mask_name) + 1;
                toFont.addr = (XPointer) &mask_font;
                toFont.size = sizeof(Font);
                /* XXX using display of screen argument as message display */
#ifdef XMU_KLUDGE
                /* XXX Sacrifice caching */
	    num = 1;
	    success = XtCvtStringToFont(dpy, &cvtArg, &num, &fromString,
					&toFont, NULL);
#else
                success = XtCallConverter(dpy, XtCvtStringToFont, &cvtArg,
                                          (Cardinal)1, &fromString, &toFont, NULL);
#endif
                if (!success) {
                    XtStringConversionWarning(name, XtRCursor);
                    return;
                }
        }

        cursor = XCreateGlyphCursor( DisplayOfScreen(screen), source_font,
                                     mask_font, source_char, mask_char,
                                     &fgColor, &bgColor );
        done(&cursor, Cursor);
        return;
    }

    i = XmuCursorNameToIndex (name);
    if (i != -1) {
        cursor = XCreateFontCursor (DisplayOfScreen(screen), i);
        done(&cursor, Cursor);
        return;
    }

    if ((source = XmuLocateBitmapFile (screen, name,
                                       maskname, (sizeof maskname) - 4,
                                       NULL, NULL, &xhot, &yhot)) == None) {
        XtStringConversionWarning (name, XtRCursor);
        cursor = None;
        done(&cursor, Cursor);
        return;
    }
    len = strlen (maskname);
    for (i = 0; i < 2; i++) {
        strcpy (maskname + len, i == 0 ? "Mask" : "msk");
        if ((mask = XmuLocateBitmapFile (screen, maskname, NULL, 0,
                                         NULL, NULL, NULL, NULL)) != None)
            break;
    }

    cursor = XCreatePixmapCursor( DisplayOfScreen(screen), source, mask,
                                  &fgColor, &bgColor, xhot, yhot );
    XFreePixmap( DisplayOfScreen(screen), source );
    if (mask != None) XFreePixmap( DisplayOfScreen(screen), mask );

    done(&cursor, Cursor);
}

#define	new_done(type, value) \
	{							\
	    if (toVal->addr != NULL) {				\
		if (toVal->size < sizeof(type)) {		\
		    toVal->size = sizeof(type);			\
		    return False;				\
		}						\
		*(type*)(toVal->addr) = (value);		\
	    }							\
	    else {						\
		static type static_val;				\
		static_val = (value);				\
		toVal->addr = (XPointer)&static_val;		\
	    }							\
	    toVal->size = sizeof(type);				\
	    return True;					\
	}

/*	Function Name: XmuCvtStringToColorCursor
 *	Description: Converts a string into a colored cursor.
 *	Arguments: dpy
 *		   args - an argument list (see below).
 *                 num_args - number of elements in the argument list.
 *                 fromVal - value to convert from.
 *                 toVal - value to convert to.
 *		   data
 *	Returns:   True or False
 */

/*ARGSUSED*/
Boolean
XmuCvtStringToColorCursor(Display *dpy, XrmValuePtr args, Cardinal *num_args,
                          XrmValuePtr fromVal, XrmValuePtr toVal,
                          XtPointer *converter_data)
{
    Cursor cursor;
    Screen *screen;
    Pixel fg, bg;
    Colormap c_map;
    XColor colors[2];
    Cardinal number;
    XrmValue ret_val;

    if (*num_args != 4) {
        XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
                        "wrongParameters","cvtStringToColorCursor","XmuError",
                        "String to color cursor conversion needs four arguments",
                        (String *)NULL, (Cardinal *)NULL);
        return False;
    }

    screen = *((Screen **) args[0].addr);
    fg = *((Pixel *) args[1].addr);
    bg = *((Pixel *) args[2].addr);
    c_map = *((Colormap *) args[3].addr);

    number = 1;
    XmuCvtStringToCursor(args, &number, fromVal, &ret_val);

    cursor = *((Cursor *) ret_val.addr);

    if (cursor == None || (fg == BlackPixelOfScreen(screen)
                           && bg == WhitePixelOfScreen(screen)))
    new_done(Cursor, cursor);

    colors[0].pixel = fg;
    colors[1].pixel = bg;

    XQueryColors (dpy, c_map, colors, 2);
    XRecolorCursor(dpy, cursor, colors, colors + 1);
    new_done(Cursor, cursor);
}