#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <ctype.h>
#include "../x11/headers/dix.h"

#ifndef HAVE_STRCASECMP
int
xstrcasecmp(const char *str1, const char *str2)
{
    const u_char *us1 = (const u_char *) str1, *us2 = (const u_char *) str2;

    while (tolower(*us1) == tolower(*us2)) {
        if (*us1++ == '\0')
            return 0;
        us2++;
    }

    return (tolower(*us1) - tolower(*us2));
}
#endif

#ifndef HAVE_STRNCASECMP
int
xstrncasecmp(const char *s1, const char *s2, size_t n)
{
    if (n != 0) {
        const u_char *us1 = (const u_char *) s1, *us2 = (const u_char *) s2;

        do {
            if (tolower(*us1) != tolower(*us2++))
                return (tolower(*us1) - tolower(*--us2));
            if (*us1++ == '\0')
                break;
        } while (--n != 0);
    }

    return 0;
}
#endif
