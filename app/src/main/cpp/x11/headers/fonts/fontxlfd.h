#ifndef _FONTXLFD_H_
#define _FONTXLFD_H_

#include "FSproto.h"

/* Constants for values_supplied bitmap */

#define SIZE_SPECIFY_MASK		0xf

#define PIXELSIZE_MASK			0x3
#define PIXELSIZE_UNDEFINED		0
#define PIXELSIZE_SCALAR		0x1
#define PIXELSIZE_ARRAY			0x2
#define PIXELSIZE_SCALAR_NORMALIZED	0x3	/* Adjusted for resolution */

#define POINTSIZE_MASK			0xc
#define POINTSIZE_UNDEFINED		0
#define POINTSIZE_SCALAR		0x4
#define POINTSIZE_ARRAY			0x8

#define PIXELSIZE_WILDCARD		0x10
#define POINTSIZE_WILDCARD		0x20

#define ENHANCEMENT_SPECIFY_MASK	0x40

#define CHARSUBSET_SPECIFIED		0x40

#define EPS		1.0e-20
#define XLFD_NDIGITS	3		/* Round numbers in pixel and
					   point arrays to this many
					   digits for repeatability */

typedef struct _FontScalable {
    int		values_supplied;	/* Bitmap identifying what advanced
					   capabilities or enhancements
					   were specified in the font name */
    double	pixel_matrix[4];
    double	point_matrix[4];

    /* Pixel and point fields are deprecated in favor of the
       transformation matrices.  They are provided and filled in for the
       benefit of rasterizers that do not handle the matrices.  */

    int		pixel,
            point;

    int         x,
            y,
            width;
    char	*xlfdName;
    int		nranges;
    fsRange	*ranges;
}           FontScalableRec, *FontScalablePtr;


extern double xlfd_round_double ( double x );
extern Bool FontParseXLFDName ( char *fname, FontScalablePtr vals, int subst );
extern fsRange *FontParseRanges ( char *name, int *nranges );

#define FONT_XLFD_REPLACE_NONE	0
#define FONT_XLFD_REPLACE_STAR	1
#define FONT_XLFD_REPLACE_ZERO	2
#define FONT_XLFD_REPLACE_VALUE	3

#endif				/* _FONTXLFD_H_ */
