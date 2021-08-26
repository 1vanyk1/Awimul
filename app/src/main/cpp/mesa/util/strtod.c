#include <stdlib.h>

#if defined(_GNU_SOURCE) && defined(HAVE_STRTOD_L)
#include <locale.h>
#ifdef HAVE_XLOCALE_H
#include <xlocale.h>
#endif
static locale_t loc;
#endif

#include "strtod.h"


void
_mesa_locale_init(void)
{
#if defined(_GNU_SOURCE) && defined(HAVE_STRTOD_L)
    loc = newlocale(LC_CTYPE_MASK, "C", NULL);
#endif
}

void
_mesa_locale_fini(void)
{
#if defined(_GNU_SOURCE) && defined(HAVE_STRTOD_L)
    freelocale(loc);
#endif
}

/**
 * Wrapper around strtod which uses the "C" locale so the decimal
 * point is always '.'
 */
double
_mesa_strtod(const char *s, char **end)
{
#if defined(_GNU_SOURCE) && defined(HAVE_STRTOD_L)
    return strtod_l(s, end, loc);
#else
    return strtod(s, end);
#endif
}


/**
 * Wrapper around strtof which uses the "C" locale so the decimal
 * point is always '.'
 */
float
_mesa_strtof(const char *s, char **end)
{
#if defined(_GNU_SOURCE) && defined(HAVE_STRTOD_L)
    return strtof_l(s, end, loc);
#elif defined(HAVE_STRTOF)
    return strtof(s, end);
#else
    return (float) strtod(s, end);
#endif
}
