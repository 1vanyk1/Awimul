#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../headers/xlibint.h"
#include "xlcPubI.h"
#include "../headers/xutil.h"
#include "../headers/xatom.h"
#include <stdio.h>

static int
get_buf_size(
        Bool is_wide_char,
        XPointer list,
        int count)
{
    int length = 0;
    char **mb_list;
    wchar_t **wc_list;

    if (list == NULL)
        return 0;

    if (is_wide_char) {
        wc_list = (wchar_t **) list;
        for ( ; count-- > 0; wc_list++) {
            if (*wc_list)
                length += _Xwcslen(*wc_list) + 1;
        }
        length *= 5;	/* XXX */
    } else {
        mb_list = (char **) list;
        for ( ; count-- > 0; mb_list++) {
            if (*mb_list)
                length = (int) ((size_t) length + (strlen(*mb_list) + 1));
        }
        length *= 3;	/* XXX */
    }
    length = (length / BUFSIZ + 1) * BUFSIZ;	/* XXX */

    return length;
}

static int
_XTextListToTextProperty(
        XLCd lcd,
        Display *dpy,
        const char *from_type,
        XPointer list,
        int count,
        XICCEncodingStyle style,
        XTextProperty *text_prop)
{
    Atom encoding;
    XlcConv conv;
    const char *to_type;
    char **mb_list = NULL;
    wchar_t **wc_list = NULL;
    XPointer from;
    char *to, *buf, *value;
    int from_left, to_left, buf_len, nitems, unconv_num = 0, ret, i;
    Bool is_wide_char = False;

    if (strcmp(XlcNWideChar, from_type) == 0)
        is_wide_char = True;

    buf_len = get_buf_size(is_wide_char, list, count);
    if ((buf = Xmalloc(buf_len)) == NULL)
        return XNoMemory;

    switch (style) {
        case XStringStyle:
        case XStdICCTextStyle:
            encoding = XA_STRING;
            to_type = XlcNString;
            break;
        case XUTF8StringStyle:
            encoding = XInternAtom(dpy, "UTF8_STRING", False);
            to_type = XlcNUtf8String;
            break;
        case XCompoundTextStyle:
            encoding = XInternAtom(dpy, "COMPOUND_TEXT", False);
            to_type = XlcNCompoundText;
            break;
        case XTextStyle:
            encoding = XInternAtom(dpy, XLC_PUBLIC(lcd, encoding_name), False);
            to_type = XlcNMultiByte;
            if (is_wide_char == False) {
                nitems = 0;
                mb_list = (char **) list;
                to = buf;
                for (i = 0; i < count && buf_len > 0; i++) {
                    if (*mb_list)
                        strcpy(to, *mb_list);
                    else
                        *to = '\0';
                    from_left = (int) (*mb_list ? strlen(*mb_list) : 0) + 1;
                    nitems += from_left;
                    to += from_left;
                    mb_list++;
                }
                unconv_num = 0;
                goto done;
            }
            break;
        default:
            Xfree(buf);
            return XConverterNotFound;
    }

    if (count < 1) {
        nitems = 0;
        goto done;
    }

    retry:
    conv = _XlcOpenConverter(lcd, from_type, lcd, to_type);
    if (conv == NULL) {
        Xfree(buf);
        return XConverterNotFound;
    }

    if (is_wide_char)
        wc_list = (wchar_t **) list;
    else
        mb_list = (char **) list;

    to = buf;
    to_left = buf_len;

    unconv_num = 0;

    for (i = 1; to_left > 0; i++) {
        if (is_wide_char) {
            from = (XPointer) *wc_list;
            from_left = _Xwcslen(*wc_list);
            wc_list++;
        } else {
            from = (XPointer) *mb_list;
            from_left = (int) (*mb_list ? strlen(*mb_list) : 0);
            mb_list++;
        }

        ret = _XlcConvert(conv, &from, &from_left, (XPointer *) &to, &to_left,
                          NULL, 0);

        if (ret < 0)
            continue;

        if (ret > 0 && style == XStdICCTextStyle && encoding == XA_STRING) {
            _XlcCloseConverter(conv);
            encoding = XInternAtom(dpy, "COMPOUND_TEXT", False);
            to_type = XlcNCompoundText;
            goto retry;
        }

        unconv_num += ret;
        *to++ = '\0';
        to_left--;

        if (i >= count)
            break;

        _XlcResetConverter(conv);
    }

    _XlcCloseConverter(conv);

    nitems = (int) (to - buf);
    done:
    if (nitems <= 0)
        nitems = 1;
    value = Xmalloc(nitems);
    if (value == NULL) {
        Xfree(buf);
        return XNoMemory;
    }
    if (nitems == 1)
        *value = 0;
    else
        memcpy(value, buf, (size_t) nitems);
    nitems--;
    Xfree(buf);

    text_prop->value = (unsigned char *) value;
    text_prop->encoding = encoding;
    text_prop->format = 8;
    text_prop->nitems = (unsigned long) nitems;

    return unconv_num;
}

int
_XmbTextListToTextProperty(
        XLCd lcd,
        Display *dpy,
        char **list,
        int count,
        XICCEncodingStyle style,
        XTextProperty *text_prop)
{
    return _XTextListToTextProperty(lcd, dpy, XlcNMultiByte, (XPointer) list,
                                    count, style, text_prop);
}

int
_XwcTextListToTextProperty(
        XLCd lcd,
        Display *dpy,
        wchar_t **list,
        int count,
        XICCEncodingStyle style,
        XTextProperty *text_prop)
{
    return _XTextListToTextProperty(lcd, dpy, XlcNWideChar, (XPointer) list,
                                    count, style, text_prop);
}

int
_Xutf8TextListToTextProperty(
        XLCd lcd,
        Display *dpy,
        char **list,
        int count,
        XICCEncodingStyle style,
        XTextProperty *text_prop)
{
    return _XTextListToTextProperty(lcd, dpy, XlcNUtf8String, (XPointer) list,
                                    count, style, text_prop);
}