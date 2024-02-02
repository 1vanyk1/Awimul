#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "XpmI.h"

#ifdef NEED_STRDUP
/*
 * in case strdup is not provided by the system here is one
 * which does the trick
 */
char *
xpmstrdup(char *s1)
{
    char *s2;
    size_t l = strlen(s1) + 1;

    if (s2 = (char *) XpmMalloc(l))
	strcpy(s2, s1);
    return s2;
}

#endif

unsigned int
xpmatoui(
        register char	*p,
        unsigned int	 l,
        unsigned int	*ui_return)
{
    register unsigned int n, i;

    n = 0;
    for (i = 0; i < l; i++)
        if (*p >= '0' && *p <= '9')
            n = n * 10 + *p++ - '0';
        else
            break;

    if (i != 0 && i == l) {
        *ui_return = n;
        return 1;
    } else
        return 0;
}

/*
 * Function returning a character string related to an error code.
 */
char *
XpmGetErrorString(int errcode)
{
    switch (errcode) {
        case XpmColorError:
            return ("XpmColorError");
        case XpmSuccess:
            return ("XpmSuccess");
        case XpmOpenFailed:
            return ("XpmOpenFailed");
        case XpmFileInvalid:
            return ("XpmFileInvalid");
        case XpmNoMemory:
            return ("XpmNoMemory");
        case XpmColorFailed:
            return ("XpmColorFailed");
        default:
            return ("Invalid XpmError");
    }
}

/*
 * The following function provides a way to figure out if the linked library is
 * newer or older than the one with which a program has been first compiled.
 */
int
XpmLibraryVersion(void)
{
    return XpmIncludeVersion;
}


/* The following should help people wanting to use their own functions */
#ifdef XpmFree
#undef XpmFree
#endif

void
XpmFree(void *ptr)
{
    free(ptr);
}
