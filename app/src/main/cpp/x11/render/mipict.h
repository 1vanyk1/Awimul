#ifndef _MIPICT_H_
#define _MIPICT_H_

#include "../headers/picturestr.h"

#define MI_MAX_INDEXED	256     /* XXX depth must be <= 8 */

#if MI_MAX_INDEXED <= 256
typedef CARD8 miIndexType;
#endif

typedef struct _miIndexed {
    Bool color;
    CARD32 rgba[MI_MAX_INDEXED];
    miIndexType ent[32768];
} miIndexedRec, *miIndexedPtr;

#define miCvtR8G8B8to15(s) ((((s) >> 3) & 0x001f) | \
			     (((s) >> 6) & 0x03e0) | \
			     (((s) >> 9) & 0x7c00))
#define miIndexToEnt15(mif,rgb15) ((mif)->ent[rgb15])
#define miIndexToEnt24(mif,rgb24) miIndexToEnt15(mif,miCvtR8G8B8to15(rgb24))

#define miIndexToEntY24(mif,rgb24) ((mif)->ent[CvtR8G8B8toY15(rgb24)])

extern _X_EXPORT int
miCreatePicture(PicturePtr pPicture);

extern _X_EXPORT void
miDestroyPicture(PicturePtr pPicture);

extern _X_EXPORT void
miCompositeSourceValidate(PicturePtr pPicture);

extern _X_EXPORT Bool

miComputeCompositeRegion(RegionPtr pRegion,
PicturePtr pSrc,
        PicturePtr pMask,
PicturePtr pDst,
        INT16 xSrc,
INT16 ySrc,
        INT16 xMask,
INT16 yMask,
        INT16 xDst, INT16 yDst, CARD16 width, CARD16 height);

extern _X_EXPORT Bool
miPictureInit(ScreenPtr pScreen, PictFormatPtr formats, int nformats);

extern _X_EXPORT Bool
miRealizeGlyph(ScreenPtr pScreen, GlyphPtr glyph);

extern _X_EXPORT void
        miUnrealizeGlyph(ScreenPtr pScreen, GlyphPtr glyph);

extern _X_EXPORT void

miGlyphs(CARD8 op,
         PicturePtr pSrc,
         PicturePtr pDst,
         PictFormatPtr maskFormat,
         INT16 xSrc,
         INT16 ySrc, int nlist, GlyphListPtr list, GlyphPtr * glyphs);

extern _X_EXPORT void
miRenderColorToPixel(PictFormatPtr pPict, xRenderColor * color, CARD32 *pixel);

extern _X_EXPORT void
miRenderPixelToColor(PictFormatPtr pPict, CARD32 pixel, xRenderColor * color);

extern _X_EXPORT Bool
miIsSolidAlpha(PicturePtr pSrc);

extern _X_EXPORT void

miCompositeRects(CARD8 op,
                 PicturePtr pDst,
                 xRenderColor * color, int nRect, xRectangle *rects);

extern _X_EXPORT void
miTrapezoidBounds(int ntrap, xTrapezoid * traps, BoxPtr box);

extern _X_EXPORT void
miPointFixedBounds(int npoint, xPointFixed * points, BoxPtr bounds);

extern _X_EXPORT void
miTriangleBounds(int ntri, xTriangle * tris, BoxPtr bounds);

extern _X_EXPORT Bool
miInitIndexed(ScreenPtr pScreen, PictFormatPtr pFormat);

extern _X_EXPORT void
        miCloseIndexed(ScreenPtr pScreen, PictFormatPtr pFormat);

extern _X_EXPORT void

        miUpdateIndexed(ScreenPtr pScreen,
PictFormatPtr pFormat, int ndef, xColorItem * pdef);

#endif                          /* _MIPICT_H_ */
