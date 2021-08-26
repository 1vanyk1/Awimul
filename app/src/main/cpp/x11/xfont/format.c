#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "libxfontint.h"
#include "../headers/fonts/FSproto.h"
#include "../headers/fonts/font.h"
#include "../headers/fonts/fontstruct.h"
#include "../headers/fonts/fontutil.h"

int
CheckFSFormat(fsBitmapFormat format,
              fsBitmapFormatMask fmask,
              int *bit_order,
              int *byte_order,
              int *scan,
              int *glyph,
              int *image)
{
    /* convert format to what the low levels want */
    if (fmask & BitmapFormatMaskBit) {
        *bit_order = format & BitmapFormatBitOrderMask;
        *bit_order = (*bit_order == BitmapFormatBitOrderMSB)
                     ? MSBFirst : LSBFirst;
    }
    if (fmask & BitmapFormatMaskByte) {
        *byte_order = format & BitmapFormatByteOrderMask;
        *byte_order = (*byte_order == BitmapFormatByteOrderMSB)
                      ? MSBFirst : LSBFirst;
    }
    if (fmask & BitmapFormatMaskScanLineUnit) {
        *scan = format & BitmapFormatScanlineUnitMask;
        /* convert byte paddings into byte counts */
        switch (*scan) {
            case BitmapFormatScanlineUnit8:
                *scan = 1;
                break;
            case BitmapFormatScanlineUnit16:
                *scan = 2;
                break;
            case BitmapFormatScanlineUnit32:
                *scan = 4;
                break;
            default:
                return BadFontFormat;
        }
    }
    if (fmask & BitmapFormatMaskScanLinePad) {
        *glyph = format & BitmapFormatScanlinePadMask;
        /* convert byte paddings into byte counts */
        switch (*glyph) {
            case BitmapFormatScanlinePad8:
                *glyph = 1;
                break;
            case BitmapFormatScanlinePad16:
                *glyph = 2;
                break;
            case BitmapFormatScanlinePad32:
                *glyph = 4;
                break;
            default:
                return BadFontFormat;
        }
    }
    if (fmask & BitmapFormatMaskImageRectangle) {
        *image = format & BitmapFormatImageRectMask;

        if (*image != BitmapFormatImageRectMin &&
            *image != BitmapFormatImageRectMaxWidth &&
            *image != BitmapFormatImageRectMax)
            return BadFontFormat;
    }
    return Successful;
}
