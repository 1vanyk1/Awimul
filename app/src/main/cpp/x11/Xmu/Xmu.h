#ifndef _XMU_H_
#define _XMU_H_

#include "../headers/xfuncproto.h"
#include "../libXt/Intrinsic.h"
#include "Atoms.h"		/* _XA_... */
#include "CharSet.h"		/* CopyISOLatin1Lowered */
#include "Converters.h"		/* CvtStringTo... */
#include "Drawing.h"		/* DrawRoundedRect, DrawLogo */
#include "Error.h"		/* PrintDefaultError */
#include "StdSel.h"		/* ConvertStandardSelection */

/*
 * clip lists
 */
typedef struct _XmuSegment {
    int x1, x2;
    struct _XmuSegment *next;
} XmuSegment;

typedef struct _XmuScanline {
    int y;
    XmuSegment *segment;
    struct _XmuScanline *next;
} XmuScanline;

typedef struct _XmuArea {
    XmuScanline *scanline;
} XmuArea;

#define XmuCreateArea()		XmuNewArea(0, 0, 0, 0)
#define XmuAreaOr(dst, src)	XmuAreaOrXor((dst), (src), True)
#define XmuAreaXor(dst, src)	XmuAreaOrXor((dst), (src), False)

#define XmuDestroyArea(a)					\
		  do {						\
		    XmuDestroyScanlineList((a)->scanline);	\
		    XtFree((char *)(a));			\
		  } while (0)

#define FreeArea(a)						\
		  do {						\
		    XmuDestroyScanlineList((a)->scanline);	\
		    a->scanline = (Scanline *)0;		\
		  } while (0)

#define XmuValidSegment(s)	((s)->x1 < (s)->x2)
#define XmuSegmentEqu(s1, s2)	((s1)->x1 == (s2)->x1 && (s1)->x2 == (s2)->x2)
#define XmuDestroySegment(s)	XtFree((char *)(s))

#define XmuDestroyScanline(s)					\
		  do {						\
		    XmuDestroySegmentList((s)->segment);	\
		    XtFree((char*)(s));				\
		  } while (0)

XmuArea *XmuNewArea(int, int, int, int);
XmuArea *XmuAreaDup(XmuArea*);
XmuArea *XmuAreaCopy(XmuArea*, XmuArea*);
XmuArea *XmuAreaNot(XmuArea*, int, int, int, int);
XmuArea *XmuAreaOrXor(XmuArea*, XmuArea*, Bool);
XmuArea *XmuAreaAnd(XmuArea*, XmuArea*);
Bool XmuValidArea(XmuArea*);
Bool XmuValidScanline(XmuScanline*);
Bool XmuScanlineEqu(XmuScanline*, XmuScanline*);
XmuSegment *XmuNewSegment(int, int);
void XmuDestroySegmentList(XmuSegment*);
XmuScanline *XmuScanlineCopy(XmuScanline*, XmuScanline*);
Bool XmuAppendSegment(XmuSegment*, XmuSegment*);
XmuScanline *XmuOptimizeScanline(XmuScanline*);
XmuScanline *XmuScanlineNot(XmuScanline *scanline, int, int);
XmuScanline *XmuScanlineOr(XmuScanline*, XmuScanline*);
XmuScanline *XmuScanlineAnd(XmuScanline*, XmuScanline*);
XmuScanline *XmuScanlineXor(XmuScanline*, XmuScanline*);
XmuScanline *XmuNewScanline(int, int, int);
void XmuDestroyScanlineList(XmuScanline*);
XmuArea *XmuOptimizeArea(XmuArea *area);

#ifndef notdef
XmuScanline *XmuScanlineOrSegment(XmuScanline*, XmuSegment*);
XmuScanline *XmuScanlineAndSegment(XmuScanline*, XmuSegment*);
XmuScanline *XmuScanlineXorSegment(XmuScanline*, XmuSegment*);
#endif /* notdef */

#ifndef _SYSUTIL_H_
int XmuSnprintf(char *str, int size, _Xconst char *fmt, ...)
_X_ATTRIBUTE_PRINTF(3,4);
#endif

#endif /* _XMU_H_ */

