#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#define XFONT_BITMAP 1
#endif
#include "libxfontint.h"

#include "../headers/fonts/fontmisc.h"
#include "../headers/fonts/fntfilst.h"
#include "../headers/fonts/bitmap.h"

/*
 * Translate monolithic build symbols to modular build symbols.
 * I chose to make the modular symbols 'canonical' because they
 * are prefixed with XFONT_, neatly avoiding name collisions
 * with other packages.
 */

#ifdef BUILD_FREETYPE
# define XFONT_FREETYPE 1
#endif

void
FontFileRegisterFpeFunctions(void)
{
#ifdef XFONT_BITMAP
    /* bitmap is always builtin to libXfont now */
    BitmapRegisterFontFileFunctions ();
#endif
#ifdef XFONT_FREETYPE
    FreeTypeRegisterFontFileFunctions();
#endif

    FontFileRegisterLocalFpeFunctions ();
#ifdef HAVE_READLINK
    CatalogueRegisterLocalFpeFunctions ();
#endif
}

