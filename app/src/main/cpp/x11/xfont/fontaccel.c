#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "libxfontint.h"
#include "../headers/fonts/fontmisc.h"
#include "../headers/fonts/fontstruct.h"
#include "../headers/fonts/fontutil.h"

void
FontComputeInfoAccelerators(FontInfoPtr pFontInfo)
{
    pFontInfo->noOverlap = FALSE;
    if (pFontInfo->maxOverlap <= pFontInfo->minbounds.leftSideBearing)
        pFontInfo->noOverlap = TRUE;

    if ((pFontInfo->minbounds.ascent == pFontInfo->maxbounds.ascent) &&
        (pFontInfo->minbounds.descent == pFontInfo->maxbounds.descent) &&
        (pFontInfo->minbounds.leftSideBearing ==
         pFontInfo->maxbounds.leftSideBearing) &&
        (pFontInfo->minbounds.rightSideBearing ==
         pFontInfo->maxbounds.rightSideBearing) &&
        (pFontInfo->minbounds.characterWidth ==
         pFontInfo->maxbounds.characterWidth) &&
        (pFontInfo->minbounds.attributes == pFontInfo->maxbounds.attributes)) {
        pFontInfo->constantMetrics = TRUE;
        if ((pFontInfo->maxbounds.leftSideBearing == 0) &&
            (pFontInfo->maxbounds.rightSideBearing ==
             pFontInfo->maxbounds.characterWidth) &&
            (pFontInfo->maxbounds.ascent == pFontInfo->fontAscent) &&
            (pFontInfo->maxbounds.descent == pFontInfo->fontDescent))
            pFontInfo->terminalFont = TRUE;
        else
            pFontInfo->terminalFont = FALSE;
    } else {
        pFontInfo->constantMetrics = FALSE;
        pFontInfo->terminalFont = FALSE;
    }
    if (pFontInfo->minbounds.characterWidth == pFontInfo->maxbounds.characterWidth)
        pFontInfo->constantWidth = TRUE;
    else
        pFontInfo->constantWidth = FALSE;

    if ((pFontInfo->minbounds.leftSideBearing >= 0) &&
        (pFontInfo->maxOverlap <= 0) &&
        (pFontInfo->minbounds.ascent >= -pFontInfo->fontDescent) &&
        (pFontInfo->maxbounds.ascent <= pFontInfo->fontAscent) &&
        (-pFontInfo->minbounds.descent <= pFontInfo->fontAscent) &&
        (pFontInfo->maxbounds.descent <= pFontInfo->fontDescent))
        pFontInfo->inkInside = TRUE;
    else
        pFontInfo->inkInside = FALSE;
}

int
FontCouldBeTerminal(FontInfoPtr pFontInfo)
{
    if ((pFontInfo->minbounds.leftSideBearing >= 0) &&
        (pFontInfo->maxbounds.rightSideBearing <= pFontInfo->maxbounds.characterWidth) &&
        (pFontInfo->minbounds.characterWidth == pFontInfo->maxbounds.characterWidth) &&
        (pFontInfo->maxbounds.ascent <= pFontInfo->fontAscent) &&
        (pFontInfo->maxbounds.descent <= pFontInfo->fontDescent) &&
        (pFontInfo->maxbounds.leftSideBearing != 0 ||
         pFontInfo->minbounds.rightSideBearing != pFontInfo->minbounds.characterWidth ||
         pFontInfo->minbounds.ascent != pFontInfo->fontAscent ||
         pFontInfo->minbounds.descent != pFontInfo->fontDescent)) {
        /* blow off font with nothing but a SPACE */
        if (pFontInfo->maxbounds.ascent == 0 &&
            pFontInfo->maxbounds.descent == 0)
            return FALSE;
        return TRUE;
    }
    return FALSE;
}
