#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../headers/xlibint.h"
#include "xlcint.h"

void
XmbDrawText(
        Display            *dpy,
        Drawable            d,
        GC                  gc,
        int                 x,
        int                 y,
        XmbTextItem        *text_items,
        int                 nitems)
{
    register XFontSet fs = NULL;
    register XmbTextItem *p = text_items;
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
        esc = (*fs->methods->mb_draw_string) (dpy, d, fs, gc, x, y,
                                              p->chars, p->nchars);
        if (!esc)
            esc = fs->methods->mb_escapement (fs, p->chars, p->nchars);
        x += esc;
    }
}

void
XmbDrawString(
        Display            *dpy,
        Drawable            d,
        XFontSet            font_set,
        GC                  gc,
        int                 x,
        int                 y,
        _Xconst char       *text,
        int                 text_len)
{
    (void)(*font_set->methods->mb_draw_string) (dpy, d, font_set, gc, x, y,
                                                text, text_len);
}


void
XmbDrawImageString(
        Display            *dpy,
        Drawable            d,
        XFontSet            font_set,
        GC                  gc,
        int                 x,
        int                 y,
        _Xconst char       *text,
        int                 text_len)
{
    (*font_set->methods->mb_draw_image_string) (dpy, d, font_set, gc, x, y,
                                                text, text_len);
}

int
XmbTextEscapement(
        XFontSet        font_set,
        _Xconst char   *text,
        int             text_len)
{
    return (*font_set->methods->mb_escapement) (font_set, text, text_len);
}

int
XmbTextExtents(
        XFontSet        font_set,
        _Xconst char   *text,
        int             text_len,
        XRectangle     *overall_ink_extents,
        XRectangle     *overall_logical_extents)
{
    return (*font_set->methods->mb_extents) (font_set, text, text_len,
                                             overall_ink_extents,
                                             overall_logical_extents);
}

Status
XmbTextPerCharExtents(
        XFontSet        font_set,
        _Xconst char   *text,
        int             text_len,
        XRectangle     *ink_extents_buffer,
        XRectangle     *logical_extents_buffer,
        int             buffer_size,
        int            *num_chars,
        XRectangle     *max_ink_extents,
        XRectangle     *max_logical_extents)
{
    return (*font_set->methods->mb_extents_per_char)
            (font_set, text, text_len,
             ink_extents_buffer, logical_extents_buffer,
             buffer_size, num_chars, max_ink_extents, max_logical_extents);
}
