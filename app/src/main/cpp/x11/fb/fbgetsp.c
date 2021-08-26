#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "fb.h"

void
fbGetSpans(DrawablePtr pDrawable,
           int wMax,
           DDXPointPtr ppt, int *pwidth, int nspans, char *pchardstStart)
{
    FbBits *src, *dst;
    FbStride srcStride;
    int srcBpp;
    int srcXoff, srcYoff;
    int xoff;

    /*
     * XFree86 DDX empties the root borderClip when the VT is
     * switched away; this checks for that case
     */
    if (!fbDrawableEnabled(pDrawable))
        return;

    fbGetDrawable(pDrawable, src, srcStride, srcBpp, srcXoff, srcYoff);

    while (nspans--) {
        xoff = (int) (((long) pchardstStart) & (FB_MASK >> 3));
        dst = (FbBits *) (pchardstStart - xoff);
        xoff <<= 3;
        fbBlt(src + (ppt->y + srcYoff) * srcStride, srcStride,
              (ppt->x + srcXoff) * srcBpp,
              dst,
              1,
              xoff,
              *pwidth * srcBpp, 1, GXcopy, FB_ALLONES, srcBpp, FALSE, FALSE);
        pchardstStart += PixmapBytePad(*pwidth, pDrawable->depth);
        ppt++;
        pwidth++;
    }

    fbFinishAccess(pDrawable);
}
