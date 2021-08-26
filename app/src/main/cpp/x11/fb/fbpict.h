#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _FBPICT_H_
#define _FBPICT_H_

#include "../headers/xlocale.h"
#include "../headers/xmd.h"
#include "../headers/picture.h"
#include "../headers/extensions/renderproto.h"
#include "../headers/glyphstr.h"

/* fbpict.c */
extern _X_EXPORT void
fbComposite(CARD8 op,
            PicturePtr pSrc,
            PicturePtr pMask,
            PicturePtr pDst,
            INT16 xSrc,
            INT16 ySrc,
            INT16 xMask,
            INT16 yMask, INT16 xDst, INT16 yDst, CARD16 width, CARD16 height);

/* fbtrap.c */

extern _X_EXPORT void
fbAddTraps(PicturePtr pPicture,
           INT16 xOff, INT16 yOff, int ntrap, xTrap * traps);

extern _X_EXPORT void
fbRasterizeTrapezoid(PicturePtr alpha, xTrapezoid * trap, int x_off, int y_off);

extern _X_EXPORT void
fbAddTriangles(PicturePtr pPicture,
               INT16 xOff, INT16 yOff, int ntri, xTriangle * tris);

extern _X_EXPORT void
fbTrapezoids(CARD8 op,
             PicturePtr pSrc,
             PicturePtr pDst,
             PictFormatPtr maskFormat,
             INT16 xSrc, INT16 ySrc, int ntrap, xTrapezoid * traps);

extern _X_EXPORT void
fbTriangles(CARD8 op,
            PicturePtr pSrc,
            PicturePtr pDst,
            PictFormatPtr maskFormat,
            INT16 xSrc, INT16 ySrc, int ntris, xTriangle * tris);

extern _X_EXPORT void
fbGlyphs(CARD8 op,
         PicturePtr pSrc,
         PicturePtr pDst,
         PictFormatPtr maskFormat,
         INT16 xSrc,
         INT16 ySrc, int nlist,
         GlyphListPtr list,
         GlyphPtr *glyphs);

#endif                          /* _FBPICT_H_ */
