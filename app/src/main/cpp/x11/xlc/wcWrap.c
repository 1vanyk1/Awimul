#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../headers/xlibint.h"
#include "xlcint.h"

void
XwcDrawText(
        Display            *dpy,
        Drawable            d,
        GC                  gc,
        int                 x,
        int                 y,
        XwcTextItem        *text_items,
        int                 nitems)
{
    register XFontSet fs = NULL;
    register XwcTextItem *p = text_items;
    register int i = nitems;
    register int esc;

    /* ignore leading items with no fontset */
    while (i && !p->font_set) {
        i--;
        p++;
    }

    for (; --i >= 0; p++) {
        if (p->font_set)
            fs = p->font_set;
        x += p->delta;
        esc = (*fs->methods->wc_draw_string) (dpy, d, fs, gc, x, y,
                                              p->chars, p->nchars);
        if (!esc)
            esc = fs->methods->wc_escapement (fs, p->chars, p->nchars);
        x += esc;
    }
}

void
XwcDrawString(
        Display            *dpy,
        Drawable            d,
        XFontSet            font_set,
        GC                  gc,
        int                 x,
        int                 y,
        _Xconst wchar_t    *text,
        int                 text_len)
{
    (void)(*font_set->methods->wc_draw_string) (dpy, d, font_set, gc, x, y,
                                                text, text_len);
}

void
XwcDrawImageString(
        Display            *dpy,
        Drawable            d,
        XFontSet            font_set,
        GC                  gc,
        int                 x,
        int                 y,
        _Xconst wchar_t    *text,
        int                 text_len)
{
    (*font_set->methods->wc_draw_image_string) (dpy, d, font_set, gc, x, y,
                                                text, text_len);
}

int
XwcTextEscapement(
        XFontSet            font_set,
        _Xconst wchar_t    *text,
        int                 text_len)
{
    return (*font_set->methods->wc_escapement) (font_set, text, text_len);
}

int
XwcTextExtents(
        XFontSet            font_set,
        _Xconst wchar_t    *text,
        int                 text_len,
        XRectangle         *overall_ink_extents,
        XRectangle         *overall_logical_extents)
{
    return (*font_set->methods->wc_extents) (font_set, text, text_len,
                                             overall_ink_extents,
                                             overall_logical_extents);
}

Status
XwcTextPerCharExtents(
        XFontSet            font_set,
        _Xconst wchar_t    *text,
        int                 text_len,
        XRectangle         *ink_extents_buffer,
        XRectangle         *logical_extents_buffer,
        int                 buffer_size,
        int                *num_chars,
        XRectangle         *max_ink_extents,
        XRectangle         *max_logical_extents)
{
    return (*font_set->methods->wc_extents_per_char)
            (font_set, text, text_len,
             ink_extents_buffer, logical_extents_buffer,
             buffer_size, num_chars, max_ink_extents, max_logical_extents);
}
