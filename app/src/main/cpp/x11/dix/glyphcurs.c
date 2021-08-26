#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/misc.h"
#include "../headers/fonts/fontstruct.h"
#include "../headers/dixfontstr.h"
#include "../headers/scrnintstr.h"
#include "../headers/gcstruct.h"
#include "../headers/resource.h"
#include "../headers/dix.h"
#include "../headers/cursorstr.h"
#include "../headers/opaque.h"
#include "../headers/servermd.h"

/*
    get the bits out of the font in a portable way.  to avoid
dealing with padding and such-like, we draw the glyph into
a bitmap, then read the bits out with GetImage, which
uses server-natural format.
    since all screens return the same bitmap format, we'll just use
the first one we find.
    the character origin lines up with the hotspot in the
cursor metrics.
*/

int
ServerBitsFromGlyph(FontPtr pfont, unsigned ch, CursorMetricPtr cm,
                    unsigned char **ppbits)
{
    ScreenPtr pScreen;
    GCPtr pGC;
    xRectangle rect;
    PixmapPtr ppix;
    char *pbits;
    ChangeGCVal gcval[3];
    unsigned char char2b[2];

    /* turn glyph index into a protocol-format char2b */
    char2b[0] = (unsigned char) (ch >> 8);
    char2b[1] = (unsigned char) (ch & 0xff);

    pScreen = screenInfo.screens[0];
    pbits = calloc(BitmapBytePad(cm->width), cm->height);
    if (!pbits)
        return BadAlloc;

    ppix = (PixmapPtr) (*pScreen->CreatePixmap) (pScreen, cm->width,
                                                 cm->height, 1,
                                                 CREATE_PIXMAP_USAGE_SCRATCH);
    pGC = GetScratchGC(1, pScreen);
    if (!ppix || !pGC) {
        if (ppix)
            (*pScreen->DestroyPixmap) (ppix);
        if (pGC)
            FreeScratchGC(pGC);
        free(pbits);
        return BadAlloc;
    }

    rect.x = 0;
    rect.y = 0;
    rect.width = cm->width;
    rect.height = cm->height;

    /* fill the pixmap with 0 */
    gcval[0].val = GXcopy;
    gcval[1].val = 0;
    gcval[2].ptr = (void *) pfont;
    ChangeGC(NullClient, pGC, GCFunction | GCForeground | GCFont, gcval);
    ValidateGC((DrawablePtr) ppix, pGC);
    (*pGC->ops->PolyFillRect) ((DrawablePtr) ppix, pGC, 1, &rect);

    /* draw the glyph */
    gcval[0].val = 1;
    ChangeGC(NullClient, pGC, GCForeground, gcval);
    ValidateGC((DrawablePtr) ppix, pGC);
    (*pGC->ops->PolyText16) ((DrawablePtr) ppix, pGC, cm->xhot, cm->yhot,
                             1, (unsigned short *) char2b);
    (*pScreen->GetImage) ((DrawablePtr) ppix, 0, 0, cm->width, cm->height,
                          XYPixmap, 1, pbits);
    *ppbits = (unsigned char *) pbits;
    FreeScratchGC(pGC);
    (*pScreen->DestroyPixmap) (ppix);
    return Success;
}

Bool
CursorMetricsFromGlyph(FontPtr pfont, unsigned ch, CursorMetricPtr cm)
{
    CharInfoPtr pci;
    unsigned long nglyphs;
    CARD8 chs[2];
    FontEncoding encoding;

    chs[0] = ch >> 8;
    chs[1] = ch;
    encoding = (FONTLASTROW(pfont) == 0) ? Linear16Bit : TwoD16Bit;
    if (encoding == Linear16Bit) {
        if (ch < pfont->info.firstCol || pfont->info.lastCol < ch)
            return FALSE;
    }
    else {
        if (chs[0] < pfont->info.firstRow || pfont->info.lastRow < chs[0])
            return FALSE;
        if (chs[1] < pfont->info.firstCol || pfont->info.lastCol < chs[1])
            return FALSE;
    }
    (*pfont->get_glyphs) (pfont, 1, chs, encoding, &nglyphs, &pci);
    if (nglyphs == 0)
        return FALSE;
    cm->width = pci->metrics.rightSideBearing - pci->metrics.leftSideBearing;
    cm->height = pci->metrics.descent + pci->metrics.ascent;
    if (pci->metrics.leftSideBearing > 0) {
        cm->width += pci->metrics.leftSideBearing;
        cm->xhot = 0;
    }
    else {
        cm->xhot = -pci->metrics.leftSideBearing;
        if (pci->metrics.rightSideBearing < 0)
            cm->width -= pci->metrics.rightSideBearing;
    }
    if (pci->metrics.ascent < 0) {
        cm->height -= pci->metrics.ascent;
        cm->yhot = 0;
    }
    else {
        cm->yhot = pci->metrics.ascent;
        if (pci->metrics.descent < 0)
            cm->height -= pci->metrics.descent;
    }
    return TRUE;
}
