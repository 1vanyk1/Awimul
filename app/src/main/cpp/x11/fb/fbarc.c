#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "fb.h"
#include "../mi/mizerarc.h"
#include <limits.h>

typedef void (*FbArc) (FbBits * dst,
                       FbStride dstStride,
                       int dstBpp,
                       xArc * arc, int dx, int dy, FbBits and, FbBits xor);

void
fbPolyArc(DrawablePtr pDrawable, GCPtr pGC, int narcs, xArc * parcs)
{
    FbArc arc;

    if (pGC->lineWidth == 0) {
        arc = 0;
        if (pGC->lineStyle == LineSolid && pGC->fillStyle == FillSolid) {
            switch (pDrawable->bitsPerPixel) {
                case 8:
                    arc = fbArc8;
                    break;
                case 16:
                    arc = fbArc16;
                    break;
                case 32:
                    arc = fbArc32;
                    break;
            }
        }
        if (arc) {
            FbGCPrivPtr pPriv = fbGetGCPrivate(pGC);
            FbBits *dst;
            FbStride dstStride;
            int dstBpp;
            int dstXoff, dstYoff;
            BoxRec box;
            int x2, y2;
            RegionPtr cclip;

#ifdef FB_ACCESS_WRAPPER
            int wrapped = 1;
#endif

            cclip = fbGetCompositeClip(pGC);
            fbGetDrawable(pDrawable, dst, dstStride, dstBpp, dstXoff, dstYoff);
            while (narcs--) {
                if (miCanZeroArc(parcs)) {
                    box.x1 = parcs->x + pDrawable->x;
                    box.y1 = parcs->y + pDrawable->y;
                    /*
                     * Because box.x2 and box.y2 get truncated to 16 bits, and the
                     * RECT_IN_REGION test treats the resulting number as a signed
                     * integer, the RECT_IN_REGION test alone can go the wrong way.
                     * This can result in a server crash because the rendering
                     * routines in this file deal directly with cpu addresses
                     * of pixels to be stored, and do not clip or otherwise check
                     * that all such addresses are within their respective pixmaps.
                     * So we only allow the RECT_IN_REGION test to be used for
                     * values that can be expressed correctly in a signed short.
                     */
                    x2 = box.x1 + (int) parcs->width + 1;
                    box.x2 = x2;
                    y2 = box.y1 + (int) parcs->height + 1;
                    box.y2 = y2;
                    if ((x2 <= SHRT_MAX) && (y2 <= SHRT_MAX) &&
                        (RegionContainsRect(cclip, &box) == rgnIN)) {
#ifdef FB_ACCESS_WRAPPER
                        if (!wrapped) {
                            fbPrepareAccess(pDrawable);
                            wrapped = 1;
                        }
#endif
                        (*arc) (dst, dstStride, dstBpp,
                                parcs, pDrawable->x + dstXoff,
                                pDrawable->y + dstYoff, pPriv->and, pPriv->xor);
                    }
                    else {
#ifdef FB_ACCESS_WRAPPER
                        if (wrapped) {
                            fbFinishAccess(pDrawable);
                            wrapped = 0;
                        }
#endif
                        miZeroPolyArc(pDrawable, pGC, 1, parcs);
                    }
                }
                else {
#ifdef FB_ACCESS_WRAPPER
                    if (wrapped) {
                        fbFinishAccess(pDrawable);
                        wrapped = 0;
                    }
#endif
                    miPolyArc(pDrawable, pGC, 1, parcs);
                }
                parcs++;
            }
#ifdef FB_ACCESS_WRAPPER
            if (wrapped) {
                fbFinishAccess(pDrawable);
                wrapped = 0;
            }
#endif
        }
        else
            miZeroPolyArc(pDrawable, pGC, narcs, parcs);
    }
    else
        miPolyArc(pDrawable, pGC, narcs, parcs);
}
