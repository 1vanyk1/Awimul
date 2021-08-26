#ifndef SERVERMD_H
#define SERVERMD_H 1

#include "xorg-server.h"

#if !defined(_DIX_CONFIG_H_) && !defined(_XORG_SERVER_H_)
#error Drivers must include xorg-server.h before any other xserver headers
#error xserver code must include dix-config.h before any other headers
#endif

#include "xarch.h"		/* for X_LITTLE_ENDIAN/X_BIG_ENDIAN */

#if X_BYTE_ORDER == X_LITTLE_ENDIAN
#define IMAGE_BYTE_ORDER        LSBFirst
#define BITMAP_BIT_ORDER        LSBFirst
#elif X_BYTE_ORDER == X_BIG_ENDIAN
#define IMAGE_BYTE_ORDER        MSBFirst
#define BITMAP_BIT_ORDER        MSBFirst
#else
#error "Too weird to live."
#endif

#ifndef GLYPHPADBYTES
#define GLYPHPADBYTES           4
#endif

/* size of buffer to use with GetImage, measured in bytes. There's obviously
 * a trade-off between the amount of heap used and the number of times the
 * ddx routine has to be called.
 */
#ifndef IMAGE_BUFSIZE
#define IMAGE_BUFSIZE		(64*1024)
#endif

/* pad scanline to a longword */
#ifndef BITMAP_SCANLINE_UNIT
#define BITMAP_SCANLINE_UNIT	32
#endif

#ifndef BITMAP_SCANLINE_PAD
#define BITMAP_SCANLINE_PAD  32
#define LOG2_BITMAP_PAD		5
#define LOG2_BYTES_PER_SCANLINE_PAD	2
#endif

#include "xfuncproto.h"
/*
 *   This returns the number of padding units, for depth d and width w.
 * For bitmaps this can be calculated with the macros above.
 * Other depths require either grovelling over the formats field of the
 * screenInfo or hardwired constants.
 */

typedef struct _PaddingInfo {
    int padRoundUp;             /* pixels per pad unit - 1 */
    int padPixelsLog2;          /* log 2 (pixels per pad unit) */
    int padBytesLog2;           /* log 2 (bytes per pad unit) */
    int notPower2;              /* bitsPerPixel not a power of 2 */
    int bytesPerPixel;          /* only set when notPower2 is TRUE */
    int bitsPerPixel;           /* bits per pixel */
} PaddingInfo;
extern _X_EXPORT PaddingInfo PixmapWidthPaddingInfo[];

/* The only portable way to get the bpp from the depth is to look it up */
#define BitsPerPixel(d) (PixmapWidthPaddingInfo[d].bitsPerPixel)

#define PixmapWidthInPadUnits(w, d) \
    (PixmapWidthPaddingInfo[d].notPower2 ? \
    (((int)(w) * PixmapWidthPaddingInfo[d].bytesPerPixel +  \
	         PixmapWidthPaddingInfo[d].bytesPerPixel) >> \
	PixmapWidthPaddingInfo[d].padBytesLog2) : \
    ((int)((w) + PixmapWidthPaddingInfo[d].padRoundUp) >> \
	PixmapWidthPaddingInfo[d].padPixelsLog2))

/*
 *	Return the number of bytes to which a scanline of the given
 * depth and width will be padded.
 */
#define PixmapBytePad(w, d) \
    (PixmapWidthInPadUnits(w, d) << PixmapWidthPaddingInfo[d].padBytesLog2)

#define BitmapBytePad(w) \
    (((int)((w) + BITMAP_SCANLINE_PAD - 1) >> LOG2_BITMAP_PAD) << LOG2_BYTES_PER_SCANLINE_PAD)

#endif                          /* SERVERMD_H */