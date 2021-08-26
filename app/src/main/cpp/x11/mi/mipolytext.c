#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/x.h"
#include "../headers/xmd.h"
#include "../headers/xproto.h"
#include "../headers/misc.h"
#include "../headers/gcstruct.h"
#include "../headers/fonts/fontstruct.h"
#include "../headers/dixfontstr.h"
#include "mi.h"

int
miPolyText8(DrawablePtr pDraw, GCPtr pGC, int x, int y, int count, char *chars)
{
    unsigned long n, i;
    int w;
    CharInfoPtr charinfo[255];  /* encoding only has 1 byte for count */

    GetGlyphs(pGC->font, (unsigned long) count, (unsigned char *) chars,
              Linear8Bit, &n, charinfo);
    w = 0;
    for (i = 0; i < n; i++)
        w += charinfo[i]->metrics.characterWidth;
    if (n != 0)
        (*pGC->ops->PolyGlyphBlt) (pDraw, pGC, x, y, n, charinfo,
                                   FONTGLYPHS(pGC->font));
    return x + w;
}

int
miPolyText16(DrawablePtr pDraw, GCPtr pGC, int x, int y, int count,
             unsigned short *chars)
{
    unsigned long n, i;
    int w;
    CharInfoPtr charinfo[255];  /* encoding only has 1 byte for count */

    GetGlyphs(pGC->font, (unsigned long) count, (unsigned char *) chars,
              (FONTLASTROW(pGC->font) == 0) ? Linear16Bit : TwoD16Bit,
              &n, charinfo);
    w = 0;
    for (i = 0; i < n; i++)
        w += charinfo[i]->metrics.characterWidth;
    if (n != 0)
        (*pGC->ops->PolyGlyphBlt) (pDraw, pGC, x, y, n, charinfo,
                                   FONTGLYPHS(pGC->font));
    return x + w;
}

void
miImageText8(DrawablePtr pDraw, GCPtr pGC, int x, int y, int count, char *chars)
{
    unsigned long n;
    FontPtr font = pGC->font;
    CharInfoPtr charinfo[255];  /* encoding only has 1 byte for count */

    GetGlyphs(font, (unsigned long) count, (unsigned char *) chars,
              Linear8Bit, &n, charinfo);
    if (n != 0)
        (*pGC->ops->ImageGlyphBlt) (pDraw, pGC, x, y, n, charinfo,
                                    FONTGLYPHS(font));
}

void
miImageText16(DrawablePtr pDraw, GCPtr pGC, int x, int y,
              int count, unsigned short *chars)
{
    unsigned long n;
    FontPtr font = pGC->font;
    CharInfoPtr charinfo[255];  /* encoding only has 1 byte for count */

    GetGlyphs(font, (unsigned long) count, (unsigned char *) chars,
              (FONTLASTROW(pGC->font) == 0) ? Linear16Bit : TwoD16Bit,
              &n, charinfo);
    if (n != 0)
        (*pGC->ops->ImageGlyphBlt) (pDraw, pGC, x, y, n, charinfo,
                                    FONTGLYPHS(font));
}
