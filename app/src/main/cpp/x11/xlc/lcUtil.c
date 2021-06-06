#include "../headers/xlib.h"
#include "xlcPublic.h"

/* Don't use <ctype.h> here because it is locale dependent. */

#define set_toupper(ch) \
  if (ch >= 'a' && ch <= 'z') \
    ch = ch - 'a' + 'A';

/* Compares two ISO 8859-1 strings, ignoring case of ASCII letters.
   Like strcasecmp in an ASCII locale. */
int
_XlcCompareISOLatin1(
        const char *str1,
        const char *str2)
{
    unsigned char ch1, ch2;

    for ( ; ; str1++, str2++) {
        ch1 = *str1;
        ch2 = *str2;
        if (ch1 == '\0' || ch2 == '\0')
            break;
        set_toupper(ch1);
        set_toupper(ch2);
        if (ch1 != ch2)
            break;
    }

    return ch1 - ch2;
}

/* Compares two ISO 8859-1 strings, at most len bytes of each, ignoring
   case of ASCII letters. Like strncasecmp in an ASCII locale. */
int
_XlcNCompareISOLatin1(
        const char *str1,
        const char *str2,
        int len)
{
    unsigned char ch1, ch2;

    for ( ; ; str1++, str2++, len--) {
        if (len == 0)
            return 0;
        ch1 = *str1;
        ch2 = *str2;
        if (ch1 == '\0' || ch2 == '\0')
            break;
        set_toupper(ch1);
        set_toupper(ch2);
        if (ch1 != ch2)
            break;
    }

    return ch1 - ch2;
}