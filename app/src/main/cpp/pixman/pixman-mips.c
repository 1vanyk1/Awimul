#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pixman-private.h"

#if defined(USE_MIPS_DSPR2) || defined(USE_LOONGSON_MMI)

#include <string.h>
#include <stdlib.h>

static pixman_bool_t
have_feature (const char *search_string)
{
#if defined (__linux__) /* linux ELF */
    /* Simple detection of MIPS features at runtime for Linux.
     * It is based on /proc/cpuinfo, which reveals hardware configuration
     * to user-space applications.  According to MIPS (early 2010), no similar
     * facility is universally available on the MIPS architectures, so it's up
     * to individual OSes to provide such.
     */
    const char *file_name = "/proc/cpuinfo";
    char cpuinfo_line[256];
    FILE *f = NULL;

    if ((f = fopen (file_name, "r")) == NULL)
        return FALSE;

    while (fgets (cpuinfo_line, sizeof (cpuinfo_line), f) != NULL)
    {
        if (strstr (cpuinfo_line, search_string) != NULL)
        {
            fclose (f);
            return TRUE;
        }
    }

    fclose (f);
#endif

    /* Did not find string in the proc file, or not Linux ELF. */
    return FALSE;
}

#endif

pixman_implementation_t *
_pixman_mips_get_implementations (pixman_implementation_t *imp)
{
#ifdef USE_LOONGSON_MMI
    /* I really don't know if some Loongson CPUs don't have MMI. */
    if (!_pixman_disabled ("loongson-mmi") && have_feature ("Loongson"))
	imp = _pixman_implementation_create_mmx (imp);
#endif

#ifdef USE_MIPS_DSPR2
    if (!_pixman_disabled ("mips-dspr2"))
    {
	int already_compiling_everything_for_dspr2 = 0;
#if defined(__mips_dsp) && (__mips_dsp_rev >= 2)
	already_compiling_everything_for_dspr2 = 1;
#endif
	if (already_compiling_everything_for_dspr2 ||
	    /* Only currently available MIPS core that supports DSPr2 is 74K. */
	    have_feature ("MIPS 74K"))
	{
	    imp = _pixman_implementation_create_mips_dspr2 (imp);
	}
    }
#endif

    return imp;
}
