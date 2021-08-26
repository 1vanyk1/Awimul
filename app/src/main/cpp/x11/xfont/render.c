#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "libxfontint.h"
#include "../headers/fonts/fntfilst.h"
#include "../headers/fonts/fontutil.h"
#include "../headers/fonts/pcf.h"
#include "builtin.h"

static int
BuiltinOpenBitmap (FontPathElementPtr fpe, FontPtr *ppFont, int	flags,
                   FontEntryPtr entry, char *fileName, fsBitmapFormat format,
                   fsBitmapFormatMask fmask, FontPtr unused)
{
    FontFilePtr	file;
    FontPtr     pFont;
    int         ret;
    int         bit,
            byte,
            glyph,
            scan,
            image;

    file = BuiltinFileOpen (fileName);
    if (!file)
        return BadFontName;
    pFont = malloc(sizeof(FontRec));
    if (!pFont) {
        BuiltinFileClose (file, 0);
        return AllocError;
    }
    /* set up default values */
    FontDefaultFormat(&bit, &byte, &glyph, &scan);
    /* get any changes made from above */
    ret = CheckFSFormat(format, fmask, &bit, &byte, &scan, &glyph, &image);

    /* Fill in font record. Data format filled in by reader. */
    pFont->refcnt = 0;
    pFont->maxPrivate = -1;
    pFont->devPrivates = (pointer *) 0;

    ret = pcfReadFont (pFont, file, bit, byte, glyph, scan);

    BuiltinFileClose (file, 0);
    if (ret != Successful)
        free(pFont);
    else
        *ppFont = pFont;
    return ret;
}

static int
BuiltinGetInfoBitmap (FontPathElementPtr fpe, FontInfoPtr pFontInfo,
                      FontEntryPtr entry, char *fileName)
{
    FontFilePtr file;
    int		ret;

    file = BuiltinFileOpen (fileName);
    if (!file)
        return BadFontName;
    ret = pcfReadFontInfo (pFontInfo, file);
    BuiltinFileClose (file, 0);
    return ret;
}

static int
BuiltinOpenScalable (FontPathElementPtr fpe,
                     FontPtr *pFont,
                     int flags,
                     FontEntryPtr entry,
                     char *fileName,
                     FontScalablePtr vals,
                     fsBitmapFormat format,
                     fsBitmapFormatMask fmask,
                     FontPtr non_cachable_font)	/* We don't do licensing */
{
    return BadFontName;
}

static int
BuiltinGetInfoScalable (FontPathElementPtr fpe,
                        FontInfoPtr pFontInfo,
                        FontEntryPtr entry,
                        FontNamePtr fontName,
                        char *fileName,
                        FontScalablePtr vals)
{
    return BadFontName;
}

static FontRendererRec renderers[] = {
        { ".builtin", 8,
                BuiltinOpenBitmap,
                BuiltinOpenScalable,
                BuiltinGetInfoBitmap,
                BuiltinGetInfoScalable,
                0 }
};

#define numRenderers	(sizeof renderers / sizeof renderers[0])

void
BuiltinRegisterFontFileFunctions(void)
{
    int	i;
    for (i = 0; i < numRenderers; i++)
        FontFileRegisterRenderer ((FontRendererRec *) &renderers[i]);
}

