#define FbSelectPart(xor,o,t)    xor

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "fb.h"

void
fbSolid(FbBits * dst,
        FbStride dstStride,
        int dstX, int bpp, int width, int height, FbBits and, FbBits xor)
{
    FbBits startmask, endmask;
    int n, nmiddle;
    int startbyte, endbyte;

    dst += dstX >> FB_SHIFT;
    dstX &= FB_MASK;
    FbMaskBitsBytes(dstX, width, and == 0, startmask, startbyte,
                    nmiddle, endmask, endbyte);
    if (startmask)
        dstStride--;
    dstStride -= nmiddle;
    while (height--) {
        if (startmask) {
            FbDoLeftMaskByteRRop(dst, startbyte, startmask, and, xor);
            dst++;
        }
        n = nmiddle;
        if (!and)
            while (n--)
                WRITE(dst++, xor);
        else
            while (n--) {
                WRITE(dst, FbDoRRop(READ(dst), and, xor));
                dst++;
            }
        if (endmask)
        FbDoRightMaskByteRRop(dst, endbyte, endmask, and, xor);
        dst += dstStride;
    }
}
