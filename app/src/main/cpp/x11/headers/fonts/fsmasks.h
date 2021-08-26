#ifndef _FSMASKS_H_
#define _FSMASKS_H_

#include "../xmd.h"

/* font format macros */
#define BitmapFormatByteOrderMask       (1L << 0)
#define BitmapFormatBitOrderMask        (1L << 1)
#define BitmapFormatImageRectMask       (3L << 2)
#define BitmapFormatScanlinePadMask     (3L << 8)
#define BitmapFormatScanlineUnitMask    (3L << 12)

#define BitmapFormatByteOrderLSB        (0)
#define BitmapFormatByteOrderMSB        (1L << 0)
#define BitmapFormatBitOrderLSB         (0)
#define BitmapFormatBitOrderMSB         (1L << 1)

#define BitmapFormatImageRectMin        (0L << 2)
#define BitmapFormatImageRectMaxWidth   (1L << 2)
#define BitmapFormatImageRectMax        (2L << 2)

#define BitmapFormatScanlinePad8        (0L << 8)
#define BitmapFormatScanlinePad16       (1L << 8)
#define BitmapFormatScanlinePad32       (2L << 8)
#define BitmapFormatScanlinePad64       (3L << 8)

#define BitmapFormatScanlineUnit8       (0L << 12)
#define BitmapFormatScanlineUnit16      (1L << 12)
#define BitmapFormatScanlineUnit32      (2L << 12)
#define BitmapFormatScanlineUnit64      (3L << 12)

#define BitmapFormatMaskByte            (1L << 0)
#define BitmapFormatMaskBit             (1L << 1)
#define BitmapFormatMaskImageRectangle  (1L << 2)
#define BitmapFormatMaskScanLinePad     (1L << 3)
#define BitmapFormatMaskScanLineUnit    (1L << 4)

typedef CARD32 fsBitmapFormat;
typedef CARD32 fsBitmapFormatMask;

#endif	/* _FSMASKS_H_ */