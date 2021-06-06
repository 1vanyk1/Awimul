#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "xlc/xlcint.h"
#include <ctype.h>
#include "headers/xos.h"
#include "reallocarray.h"


#define	XMAXLIST	256

char **
_XParseBaseFontNameList(
        char           *str,
        int            *num)
{
    char           *plist[XMAXLIST];
    char          **list;
    char           *ptr, *psave;

    *num = 0;
    if (!str || !*str) {
        return (char **)NULL;
    }
    while (*str && isspace(*str))
        str++;
    if (!*str)
        return (char **)NULL;

    if (!(ptr = strdup(str))) {
        return (char **)NULL;
    }

    psave = ptr;
    /* somebody who specifies more than XMAXLIST basefontnames will lose */
    while (*num < (sizeof plist / sizeof plist[0])) {
        char	*back;

        plist[*num] = ptr;
        if ((ptr = strchr(ptr, ','))) {
            back = ptr;
        } else {
            back = plist[*num] + strlen(plist[*num]);
        }
        while (isspace(*(back - 1)))
            back--;
        *back = '\0';
        (*num)++;
        if (!ptr)
            break;
        ptr++;
        while (*ptr && isspace(*ptr))
            ptr++;
        if (!*ptr)
            break;
    }
    if (!(list = Xmallocarray((*num + 1), sizeof(char *)))) {
        Xfree(psave);
        return (char **)NULL;
    }
    memcpy((char *)list, (char *)plist, sizeof(char *) * (size_t) (*num));
    *(list + *num) = NULL;

    return list;
}

static char **
copy_string_list(
        char **string_list,
        int list_count)
{
    char **string_list_ret, **list_src, **list_dst, *dst;
    int length, count;

    if (string_list == NULL || list_count <= 0)
        return (char **) NULL;

    string_list_ret = Xmallocarray(list_count, sizeof(char *));
    if (string_list_ret == NULL)
        return (char **) NULL;

    list_src = string_list;
    count = list_count;
    for (length = 0; count-- > 0; list_src++)
        length = length + (int) strlen(*list_src) + 1;

    dst = Xmalloc(length);
    if (dst == NULL) {
        Xfree(string_list_ret);
        return (char **) NULL;
    }

    list_src = string_list;
    count = list_count;
    list_dst = string_list_ret;
    for ( ;  count-- > 0; list_src++) {
        strcpy(dst, *list_src);
        *list_dst++ = dst;
        dst += strlen(dst) + 1;
    }

    return string_list_ret;
}

XFontSet
XCreateFontSet (
        Display        *dpy,
        _Xconst char   *base_font_name_list,
        char         ***missing_charset_list,
        int            *missing_charset_count,
        char          **def_string)
{
    XOM om;
    XOC oc;
    XOMCharSetList *list;

    *missing_charset_list = NULL;
    *missing_charset_count = 0;

    om = XOpenOM(dpy, NULL, NULL, NULL);
    if (om == NULL)
        return (XFontSet) NULL;

    if ((oc = XCreateOC(om, XNBaseFontName, base_font_name_list, NULL))) {
        list = &oc->core.missing_list;
        oc->core.om_automatic = True;
    } else
        list = &om->core.required_charset;

    *missing_charset_list = copy_string_list(list->charset_list,
                                             list->charset_count);
    *missing_charset_count = list->charset_count;

    if (list->charset_list && *missing_charset_list == NULL)
        oc = NULL;

    if (oc && def_string) {
        *def_string = oc->core.default_string;
        if (!*def_string)
            *def_string = (char *)"";
    }

    if (oc == NULL)
        XCloseOM(om);

    return (XFontSet) oc;
}

int
XFontsOfFontSet(
        XFontSet        font_set,
        XFontStruct  ***font_struct_list,
        char         ***font_name_list)
{
    *font_name_list   = font_set->core.font_info.font_name_list;
    *font_struct_list = font_set->core.font_info.font_struct_list;
    return font_set->core.font_info.num_font;
}

char *
XBaseFontNameListOfFontSet(XFontSet font_set)
{
    return font_set->core.base_name_list;
}

char *
XLocaleOfFontSet(XFontSet font_set)
{
    return font_set->core.om->core.lcd->core->name;
}

Bool
XContextDependentDrawing(XFontSet font_set)
{
    return font_set->core.om->core.context_dependent;
}

Bool
XDirectionalDependentDrawing(XFontSet font_set)
{
    return font_set->core.om->core.directional_dependent;
}

Bool
XContextualDrawing(XFontSet font_set)
{
    return font_set->core.om->core.contextual_drawing;
}

XFontSetExtents *
XExtentsOfFontSet(XFontSet font_set)
{
    if (!font_set)
        return NULL;
    return &font_set->core.font_set_extents;
}

void
XFreeFontSet(
        Display        *dpy,
        XFontSet        font_set)
{
    XCloseOM(font_set->core.om);
}