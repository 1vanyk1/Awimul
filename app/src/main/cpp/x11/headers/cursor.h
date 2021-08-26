#ifndef CURSOR_H
#define CURSOR_H

#include "misc.h"
#include "screenint.h"
#include "window.h"
#include "privates.h"

#define NullCursor ((CursorPtr)NULL)

/* Provide support for alpha composited cursors */
#define ARGB_CURSOR

struct _DeviceIntRec;

typedef struct _Cursor *CursorPtr;
typedef struct _CursorMetric *CursorMetricPtr;

extern _X_EXPORT DevScreenPrivateKeyRec cursorScreenDevPriv;

#define CursorScreenKey (&cursorScreenDevPriv)

extern _X_EXPORT CursorPtr rootCursor;

extern _X_EXPORT int FreeCursor(void *pCurs,
                                XID cid);

extern _X_EXPORT CursorPtr RefCursor(CursorPtr /* cursor */);
extern _X_EXPORT CursorPtr UnrefCursor(CursorPtr /* cursor */);
extern _X_EXPORT int CursorRefCount(const CursorPtr /* cursor */);

extern _X_EXPORT int AllocARGBCursor(unsigned char * /*psrcbits */ ,
                                     unsigned char * /*pmaskbits */ ,
                                     CARD32 * /*argb */ ,
                                     CursorMetricPtr /*cm */ ,
                                     unsigned /*foreRed */ ,
                                     unsigned /*foreGreen */ ,
                                     unsigned /*foreBlue */ ,
                                     unsigned /*backRed */ ,
                                     unsigned /*backGreen */ ,
                                     unsigned /*backBlue */ ,
                                     CursorPtr * /*ppCurs */ ,
                                     ClientPtr /*client */ ,
                                     XID /*cid */ );

extern _X_EXPORT int AllocGlyphCursor(Font /*source */ ,
                                      unsigned int /*sourceChar */ ,
                                      Font /*mask */ ,
                                      unsigned int /*maskChar */ ,
                                      unsigned /*foreRed */ ,
                                      unsigned /*foreGreen */ ,
                                      unsigned /*foreBlue */ ,
                                      unsigned /*backRed */ ,
                                      unsigned /*backGreen */ ,
                                      unsigned /*backBlue */ ,
                                      CursorPtr * /*ppCurs */ ,
                                      ClientPtr /*client */ ,
                                      XID /*cid */ );

extern _X_EXPORT CursorPtr CreateRootCursor(char * /*pfilename */ ,
                                            unsigned int /*glyph */ );

extern _X_EXPORT int ServerBitsFromGlyph(FontPtr /*pfont */ ,
                                         unsigned int /*ch */ ,
                                         CursorMetricPtr /*cm */ ,
                                         unsigned char ** /*ppbits */ );

extern _X_EXPORT Bool CursorMetricsFromGlyph(FontPtr /*pfont */ ,
                                             unsigned /*ch */ ,
                                             CursorMetricPtr /*cm */ );

extern _X_EXPORT void CheckCursorConfinement(WindowPtr /*pWin */ );

extern _X_EXPORT void NewCurrentScreen(struct _DeviceIntRec * /*pDev */ ,
                                       ScreenPtr /*newScreen */ ,
                                       int /*x */ ,
                                       int /*y */ );

extern _X_EXPORT Bool PointerConfinedToScreen(struct _DeviceIntRec * /* pDev */
);

extern _X_EXPORT void GetSpritePosition(struct _DeviceIntRec * /* pDev */ ,
                                        int * /*px */ ,
                                        int * /*py */ );

#ifdef PANORAMIX
extern _X_EXPORT int XineramaGetCursorScreen(struct _DeviceIntRec *pDev);
#endif                          /* PANORAMIX */

#endif                          /* CURSOR_H */