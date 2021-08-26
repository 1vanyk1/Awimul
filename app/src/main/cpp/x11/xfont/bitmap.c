#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "libxfontint.h"

#include "../headers/fonts/fntfilst.h"
#include "../headers/fonts/bitmap.h"

int
bitmapGetGlyphs(FontPtr pFont, unsigned long count, unsigned char *chars,
                FontEncoding charEncoding,
                unsigned long *glyphCount, 	/* RETURN */
                CharInfoPtr *glyphs) 		/* RETURN */
{
    BitmapFontPtr  bitmapFont;
    unsigned int firstCol;
    register unsigned int numCols;
    unsigned int firstRow;
    unsigned int numRows;
    CharInfoPtr *glyphsBase;
    register unsigned int c;
    register CharInfoPtr pci;
    unsigned int r;
    CharInfoPtr **encoding;
    CharInfoPtr pDefault;

    bitmapFont = (BitmapFontPtr) pFont->fontPrivate;
    encoding = bitmapFont->encoding;
    pDefault = bitmapFont->pDefault;
    firstCol = pFont->info.firstCol;
    numCols = pFont->info.lastCol - firstCol + 1;
    glyphsBase = glyphs;
    switch (charEncoding) {

        case Linear8Bit:
        case TwoD8Bit:
            if (pFont->info.firstRow > 0) {
                if (pDefault)
                    while (count--)
                        *glyphs++ = pDefault;
                break;
            }
            if (pFont->info.allExist && pDefault) {
                while (count--) {
                    c = (*chars++) - firstCol;
                    if (c < numCols)
                        *glyphs++ = ACCESSENCODING(encoding,c);
                    else
                        *glyphs++ = pDefault;
                }
            } else {
                while (count--) {
                    c = (*chars++) - firstCol;
                    if (c < numCols && (pci = ACCESSENCODING(encoding,c)))
                        *glyphs++ = pci;
                    else if (pDefault)
                        *glyphs++ = pDefault;
                }
            }
            break;
        case Linear16Bit:
            if (pFont->info.allExist && pDefault) {
                while (count--) {
                    c = *chars++ << 8;
                    c = (c | *chars++) - firstCol;
                    if (c < numCols)
                        *glyphs++ = ACCESSENCODING(encoding,c);
                    else
                        *glyphs++ = pDefault;
                }
            } else {
                while (count--) {
                    c = *chars++ << 8;
                    c = (c | *chars++) - firstCol;
                    if (c < numCols && (pci = ACCESSENCODING(encoding,c)))
                        *glyphs++ = pci;
                    else if (pDefault)
                        *glyphs++ = pDefault;
                }
            }
            break;

        case TwoD16Bit:
            firstRow = pFont->info.firstRow;
            numRows = pFont->info.lastRow - firstRow + 1;
            while (count--) {
                r = (*chars++) - firstRow;
                c = (*chars++) - firstCol;
                if (r < numRows && c < numCols &&
                    (pci = ACCESSENCODING(encoding, r * numCols + c)))
                    *glyphs++ = pci;
                else if (pDefault)
                    *glyphs++ = pDefault;
            }
            break;
    }
    *glyphCount = glyphs - glyphsBase;
    return Successful;
}

static CharInfoRec nonExistantChar;

int
bitmapGetMetrics(FontPtr pFont, unsigned long count, unsigned char *chars,
                 FontEncoding charEncoding,
                 unsigned long *glyphCount,	/* RETURN */
                 xCharInfo **glyphs)		/* RETURN */
{
    int         ret;
    xCharInfo  *ink_metrics;
    CharInfoPtr metrics;
    BitmapFontPtr  bitmapFont;
    CharInfoPtr	oldDefault;
    int         i;

    bitmapFont = (BitmapFontPtr) pFont->fontPrivate;
    oldDefault = bitmapFont->pDefault;
    bitmapFont->pDefault = &nonExistantChar;
    ret = bitmapGetGlyphs(pFont, count, chars, charEncoding, glyphCount, (CharInfoPtr *) glyphs);
    if (ret == Successful) {
        if (bitmapFont->ink_metrics) {
            metrics = bitmapFont->metrics;
            ink_metrics = bitmapFont->ink_metrics;
            for (i = 0; i < *glyphCount; i++) {
                if (glyphs[i] != (xCharInfo *) & nonExistantChar)
                    glyphs[i] = ink_metrics + (((CharInfoPtr) glyphs[i]) - metrics);
            }
        }
    }
    bitmapFont->pDefault = oldDefault;
    return ret;
}
