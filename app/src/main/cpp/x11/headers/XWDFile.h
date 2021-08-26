#ifndef XWDFILE_H
#define XWDFILE_H

#include "xmd.h"

#define XWD_FILE_VERSION 7
#define sz_XWDheader 100
#define sz_XWDColor 12

typedef CARD32 xwdval;		/* for old broken programs */

/* Values in the file are most significant byte first. */

typedef struct _xwd_file_header {
    /* header_size = SIZEOF(XWDheader) + length of null-terminated
     * window name. */
    CARD32 header_size B32;

    CARD32 file_version B32;	/* = XWD_FILE_VERSION above */
    CARD32 pixmap_format B32;	/* ZPixmap or XYPixmap */
    CARD32 pixmap_depth B32;	/* Pixmap depth */
    CARD32 pixmap_width B32;	/* Pixmap width */
    CARD32 pixmap_height B32;	/* Pixmap height */
    CARD32 xoffset B32;		/* Bitmap x offset, normally 0 */
    CARD32 byte_order B32;		/* of image data: MSBFirst, LSBFirst */

    /* bitmap_unit applies to bitmaps (depth 1 format XY) only.
     * It is the number of bits that each scanline is padded to. */
    CARD32 bitmap_unit B32;

    CARD32 bitmap_bit_order B32;	/* bitmaps only: MSBFirst, LSBFirst */

    /* bitmap_pad applies to pixmaps (non-bitmaps) only.
     * It is the number of bits that each scanline is padded to. */
    CARD32 bitmap_pad B32;

    CARD32 bits_per_pixel B32;	/* Bits per pixel */

    /* bytes_per_line is pixmap_width padded to bitmap_unit (bitmaps)
     * or bitmap_pad (pixmaps).  It is the delta (in bytes) to get
     * to the same x position on an adjacent row. */
    CARD32 bytes_per_line B32;
    CARD32 visual_class B32;	/* Class of colormap */
    CARD32 red_mask B32;		/* Z red mask */
    CARD32 green_mask B32;		/* Z green mask */
    CARD32 blue_mask B32;		/* Z blue mask */
    CARD32 bits_per_rgb B32;	/* Log2 of distinct color values */
    CARD32 colormap_entries B32;	/* Number of entries in colormap; not used? */
    CARD32 ncolors B32;		/* Number of XWDColor structures */
    CARD32 window_width B32;	/* Window width */
    CARD32 window_height B32;	/* Window height */
    CARD32 window_x B32;		/* Window upper left X coordinate */
    CARD32 window_y B32;		/* Window upper left Y coordinate */
    CARD32 window_bdrwidth B32;	/* Window border width */
} XWDFileHeader;

/* Null-terminated window name follows the above structure. */

/* Next comes XWDColor structures, at offset XWDFileHeader.header_size in
 * the file.  XWDFileHeader.ncolors tells how many XWDColor structures
 * there are.
 */

typedef struct {
    CARD32	pixel B32;
    CARD16	red B16;
    CARD16	green B16;
    CARD16	blue B16;
    CARD8	flags;
    CARD8	pad;
} XWDColor;

/* Last comes the image data in the format described by XWDFileHeader. */

#endif /* XWDFILE_H */

