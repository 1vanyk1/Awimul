#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
static XColor _Xconst foreground = { 0,    0,     0,     0  };  /* black */
static XColor _Xconst background = { 0, 65535, 65535, 65535 };  /* white */

Cursor XCreateFontCursor(
        Display *dpy,
        unsigned int which)
{
    /*
     * the cursor font contains the shape glyph followed by the mask
     * glyph; so character position 0 contains a shape, 1 the mask for 0,
     * 2 a shape, etc.  <X11/cursorfont.h> contains hash define names
     * for all of these.
     */

    if (dpy->cursor_font == None) {
        dpy->cursor_font = XLoadFont (dpy, CURSORFONT);
        if (dpy->cursor_font == None) return None;
    }

    return XCreateGlyphCursor (dpy, dpy->cursor_font, dpy->cursor_font,
                               which, which + 1, &foreground, &background);
}
