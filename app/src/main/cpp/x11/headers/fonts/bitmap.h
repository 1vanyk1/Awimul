#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "fntfilio.h"
#include <stdio.h>  /* just for NULL */

/*
 * Internal format used to store bitmap fonts
 */

/* number of encoding entries in one segment */
#define BITMAP_FONT_SEGMENT_SIZE 128

typedef struct _BitmapExtra {
    Atom       *glyphNames;
    int        *sWidths;
    CARD32      bitmapsSizes[GLYPHPADOPTIONS];
    FontInfoRec info;
}           BitmapExtraRec, *BitmapExtraPtr;

typedef struct _BitmapFont {
    unsigned    version_num;
    int         num_chars;
    int         num_tables;
    CharInfoPtr metrics;	/* font metrics, including glyph pointers */
    xCharInfo  *ink_metrics;	/* ink metrics */
    char       *bitmaps;	/* base of bitmaps, useful only to free */
    CharInfoPtr **encoding;	/* array of arrays of char info pointers */
    CharInfoPtr pDefault;	/* default character */
    BitmapExtraPtr bitmapExtra;	/* stuff not used by X server */
}           BitmapFontRec, *BitmapFontPtr;

#define ACCESSENCODING(enc,i) \
(enc[(i)/BITMAP_FONT_SEGMENT_SIZE]?\
(enc[(i)/BITMAP_FONT_SEGMENT_SIZE][(i)%BITMAP_FONT_SEGMENT_SIZE]):\
0)
#define ACCESSENCODINGL(enc,i) \
(enc[(i)/BITMAP_FONT_SEGMENT_SIZE][(i)%BITMAP_FONT_SEGMENT_SIZE])

#define SEGMENT_MAJOR(n) ((n)/BITMAP_FONT_SEGMENT_SIZE)
#define SEGMENT_MINOR(n) ((n)%BITMAP_FONT_SEGMENT_SIZE)
#define NUM_SEGMENTS(n) \
  (((n)+BITMAP_FONT_SEGMENT_SIZE-1)/BITMAP_FONT_SEGMENT_SIZE)

extern int bitmapGetGlyphs ( FontPtr pFont, unsigned long count,
                             unsigned char *chars, FontEncoding charEncoding,
                             unsigned long *glyphCount, CharInfoPtr *glyphs );
extern int bitmapGetMetrics ( FontPtr pFont, unsigned long count,
                              unsigned char *chars, FontEncoding charEncoding,
                              unsigned long *glyphCount, xCharInfo **glyphs );

extern void bitmapComputeFontBounds ( FontPtr pFont );
extern void bitmapComputeFontInkBounds ( FontPtr pFont );
extern Bool bitmapAddInkMetrics ( FontPtr pFont );
extern int bitmapComputeWeight ( FontPtr pFont );

extern void BitmapRegisterFontFileFunctions ( void );

extern int BitmapOpenScalable ( FontPathElementPtr fpe, FontPtr *pFont,
                                int flags, FontEntryPtr entry, char *fileName,
                                FontScalablePtr vals, fsBitmapFormat format,
                                fsBitmapFormatMask fmask,
                                FontPtr non_cachable_font );
extern int BitmapGetInfoScalable ( FontPathElementPtr fpe,
                                   FontInfoPtr pFontInfo, FontEntryPtr entry,
                                   FontNamePtr fontName, char *fileName,
                                   FontScalablePtr vals );

#endif				/* _BITMAP_H_ */
