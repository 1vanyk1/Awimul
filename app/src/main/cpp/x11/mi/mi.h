#ifndef MI_H
#define MI_H
#include "../headers/x.h"
#include "../headers/region.h"
#include "../headers/validate.h"
#include "../headers/window.h"
#include "../headers/gc.h"
#include "../headers/fonts/font.h"
#include "../headers/input.h"
#include "../headers/cursor.h"
#include "../headers/privates.h"
#include "../headers/colormap.h"
#include "../headers/events.h"

#define MiBits	CARD32

typedef struct _miDash *miDashPtr;

#define EVEN_DASH	0
#define ODD_DASH	~0

/* miarc.c */

extern _X_EXPORT void miWideArc(DrawablePtr pDraw,
                                GCPtr pGC,
                                int narcs,
                                xArc * parcs);

extern _X_EXPORT void miPolyArc(DrawablePtr /*pDraw */ ,
                                GCPtr /*pGC */ ,
                                int /*narcs */ ,
                                xArc *  /*parcs */
);

/* mibitblt.c */

extern _X_EXPORT RegionPtr miCopyArea(DrawablePtr /*pSrcDrawable */ ,
                                      DrawablePtr /*pDstDrawable */ ,
                                      GCPtr /*pGC */ ,
                                      int /*xIn */ ,
                                      int /*yIn */ ,
                                      int /*widthSrc */ ,
                                      int /*heightSrc */ ,
                                      int /*xOut */ ,
                                      int       /*yOut */
);

extern _X_EXPORT RegionPtr miCopyPlane(DrawablePtr /*pSrcDrawable */ ,
                                       DrawablePtr /*pDstDrawable */ ,
                                       GCPtr /*pGC */ ,
                                       int /*srcx */ ,
                                       int /*srcy */ ,
                                       int /*width */ ,
                                       int /*height */ ,
                                       int /*dstx */ ,
                                       int /*dsty */ ,
                                       unsigned long    /*bitPlane */
);

extern _X_EXPORT void miGetImage(DrawablePtr /*pDraw */ ,
                                 int /*sx */ ,
                                 int /*sy */ ,
                                 int /*w */ ,
                                 int /*h */ ,
                                 unsigned int /*format */ ,
                                 unsigned long /*planeMask */ ,
                                 char * /*pdstLine */
);

extern _X_EXPORT void miPutImage(DrawablePtr /*pDraw */ ,
                                 GCPtr /*pGC */ ,
                                 int /*depth */ ,
                                 int /*x */ ,
                                 int /*y */ ,
                                 int /*w */ ,
                                 int /*h */ ,
                                 int /*leftPad */ ,
                                 int /*format */ ,
                                 char * /*pImage */
);

/* micopy.c  */

#define miGetCompositeClip(pGC) ((pGC)->pCompositeClip)

typedef void (*miCopyProc) (DrawablePtr pSrcDrawable,
                            DrawablePtr pDstDrawable,
                            GCPtr pGC,
                            BoxPtr pDstBox,
                            int nbox,
                            int dx,
                            int dy,
                            Bool reverse,
                            Bool upsidedown, Pixel bitplane, void *closure);

extern _X_EXPORT void

miCopyRegion(DrawablePtr pSrcDrawable,
             DrawablePtr pDstDrawable,
             GCPtr pGC,
             RegionPtr pDstRegion,
             int dx,
             int dy, miCopyProc copyProc, Pixel bitPlane, void *closure);

extern _X_EXPORT RegionPtr

miDoCopy(DrawablePtr pSrcDrawable,
         DrawablePtr pDstDrawable,
         GCPtr pGC,
         int xIn,
         int yIn,
         int widthSrc,
         int heightSrc,
         int xOut,
         int yOut, miCopyProc copyProc, Pixel bitplane, void *closure);

/* micursor.c */

extern _X_EXPORT void miRecolorCursor(DeviceIntPtr /* pDev */ ,
                                      ScreenPtr /*pScr */ ,
                                      CursorPtr /*pCurs */ ,
                                      Bool      /*displayed */
);

/* midash.c */

extern _X_EXPORT void miStepDash(int /*dist */ ,
                                 int * /*pDashIndex */ ,
                                 unsigned char * /*pDash */ ,
                                 int /*numInDashList */ ,
                                 int *  /*pDashOffset */
);

/* mieq.c */

#ifndef INPUT_H
typedef struct _DeviceRec *DevicePtr;
#endif

extern _X_EXPORT Bool mieqInit(void
);

extern _X_EXPORT void mieqFini(void);

extern _X_EXPORT void mieqEnqueue(DeviceIntPtr /*pDev */ ,
                                  InternalEvent *       /*e */
);

extern _X_EXPORT void mieqSwitchScreen(DeviceIntPtr /* pDev */ ,
                                       ScreenPtr /*pScreen */ ,
                                       Bool     /*set_dequeue_screen */
);

extern _X_EXPORT void mieqProcessDeviceEvent(DeviceIntPtr /* dev */ ,
                                             InternalEvent * /* event */ ,
                                             ScreenPtr  /* screen */
);

extern _X_EXPORT void mieqProcessInputEvents(void
);

extern _X_EXPORT void mieqAddCallbackOnDrained(CallbackProcPtr callback,
                                               void *param);

extern _X_EXPORT void mieqRemoveCallbackOnDrained(CallbackProcPtr callback,
                                                  void *param);

extern DeviceIntPtr CopyGetMasterEvent(DeviceIntPtr /* sdev */ ,
                                       InternalEvent * /* original */ ,
                                       InternalEvent *  /* copy */
);

/**
 * Custom input event handler. If you need to process input events in some
 * other way than the default path, register an input event handler for the
 * given internal event type.
 */
typedef void (*mieqHandler) (int screen, InternalEvent *event,
                             DeviceIntPtr dev);
void _X_EXPORT mieqSetHandler(int event, mieqHandler handler);

/* miexpose.c */

extern _X_EXPORT RegionPtr miHandleExposures(DrawablePtr /*pSrcDrawable */ ,
                                             DrawablePtr /*pDstDrawable */ ,
                                             GCPtr /*pGC */ ,
                                             int /*srcx */ ,
                                             int /*srcy */ ,
                                             int /*width */ ,
                                             int /*height */ ,
                                             int /*dstx */ ,
                                             int /*dsty */
);

extern _X_EXPORT void miSendExposures(WindowPtr /*pWin */ ,
                                      RegionPtr /*pRgn */ ,
                                      int /*dx */ ,
                                      int       /*dy */
);

extern _X_EXPORT void miWindowExposures(WindowPtr /*pWin */ ,
                                        RegionPtr /*prgn */);

extern _X_EXPORT void miPaintWindow(WindowPtr /*pWin */ ,
                                    RegionPtr /*prgn */ ,
                                    int /*what */
);

extern _X_EXPORT void miClearDrawable(DrawablePtr /*pDraw */ ,
                                      GCPtr     /*pGC */
);

/* mifillrct.c */

extern _X_EXPORT void miPolyFillRect(DrawablePtr /*pDrawable */ ,
                                     GCPtr /*pGC */ ,
                                     int /*nrectFill */ ,
                                     xRectangle *       /*prectInit */
);

/* miglblt.c */

extern _X_EXPORT void miPolyGlyphBlt(DrawablePtr pDrawable,
                                     GCPtr pGC,
                                     int x,
                                     int y,
                                     unsigned int nglyph,
                                     CharInfoPtr *ppci,
                                     void *pglyphBase
);

extern _X_EXPORT void miImageGlyphBlt(DrawablePtr pDrawable,
                                      GCPtr pGC,
                                      int x,
                                      int y,
                                      unsigned int nglyph,
                                      CharInfoPtr *ppci,
                                      void *pglyphBase
);

/* mipoly.c */

extern _X_EXPORT void miFillPolygon(DrawablePtr /*dst */ ,
                                    GCPtr /*pgc */ ,
                                    int /*shape */ ,
                                    int /*mode */ ,
                                    int /*count */ ,
                                    DDXPointPtr /*pPts */
);

/* mipolypnt.c */

extern _X_EXPORT void miPolyPoint(DrawablePtr /*pDrawable */ ,
                                  GCPtr /*pGC */ ,
                                  int /*mode */ ,
                                  int /*npt */ ,
                                  xPoint *      /*pptInit */
);

/* mipolyrect.c */

extern _X_EXPORT void miPolyRectangle(DrawablePtr /*pDraw */ ,
                                      GCPtr /*pGC */ ,
                                      int /*nrects */ ,
                                      xRectangle *      /*pRects */
);

/* mipolyseg.c */

extern _X_EXPORT void miPolySegment(DrawablePtr /*pDraw */ ,
                                    GCPtr /*pGC */ ,
                                    int /*nseg */ ,
                                    xSegment *  /*pSegs */
);

/* mipolytext.c */

extern _X_EXPORT int miPolyText8(DrawablePtr /*pDraw */ ,
                                 GCPtr /*pGC */ ,
                                 int /*x */ ,
                                 int /*y */ ,
                                 int /*count */ ,
                                 char * /*chars */
);

extern _X_EXPORT int miPolyText16(DrawablePtr /*pDraw */ ,
                                  GCPtr /*pGC */ ,
                                  int /*x */ ,
                                  int /*y */ ,
                                  int /*count */ ,
                                  unsigned short *      /*chars */
);

extern _X_EXPORT void miImageText8(DrawablePtr /*pDraw */ ,
                                   GCPtr /*pGC */ ,
                                   int /*x */ ,
                                   int /*y */ ,
                                   int /*count */ ,
                                   char *       /*chars */
);

extern _X_EXPORT void miImageText16(DrawablePtr /*pDraw */ ,
                                    GCPtr /*pGC */ ,
                                    int /*x */ ,
                                    int /*y */ ,
                                    int /*count */ ,
                                    unsigned short *    /*chars */
);

/* mipushpxl.c */

extern _X_EXPORT void miPushPixels(GCPtr /*pGC */ ,
                                   PixmapPtr /*pBitMap */ ,
                                   DrawablePtr /*pDrawable */ ,
                                   int /*dx */ ,
                                   int /*dy */ ,
                                   int /*xOrg */ ,
                                   int  /*yOrg */
);

/* miscrinit.c */

extern _X_EXPORT void
miSourceValidate(DrawablePtr pDrawable, int x, int y, int w, int h,
                 unsigned int subWindowMode);

extern _X_EXPORT Bool miModifyPixmapHeader(PixmapPtr pPixmap,
                                           int width,
                                           int height,
                                           int depth,
                                           int bitsPerPixel,
                                           int devKind,
                                           void *pPixData
);

extern _X_EXPORT Bool miCreateScreenResources(ScreenPtr /*pScreen */
);

extern _X_EXPORT Bool miScreenDevPrivateInit(ScreenPtr pScreen,
                                             int width,
                                             void *pbits
);

extern _X_EXPORT Bool miScreenInit(ScreenPtr pScreen,
                                   void *pbits,
                                   int xsize,
                                   int ysize,
                                   int dpix,
                                   int dpiy,
                                   int width,
                                   int rootDepth,
                                   int numDepths,
                                   DepthPtr depths,
                                   VisualID rootVisual,
                                   int numVisuals,
                                   VisualPtr visuals
);

/* mivaltree.c */

extern _X_EXPORT int miShapedWindowIn(RegionPtr /*universe */ ,
                                      RegionPtr /*bounding */ ,
                                      BoxPtr /*rect */ ,
                                      int /*x */ ,
                                      int       /*y */
);

extern _X_EXPORT int miValidateTree(WindowPtr /*pParent */ ,
                                    WindowPtr /*pChild */ ,
                                    VTKind      /*kind */
);

extern _X_EXPORT void miWideLine(DrawablePtr /*pDrawable */ ,
                                 GCPtr /*pGC */ ,
                                 int /*mode */ ,
                                 int /*npt */ ,
                                 DDXPointPtr    /*pPts */
);

extern _X_EXPORT void miWideDash(DrawablePtr /*pDrawable */ ,
                                 GCPtr /*pGC */ ,
                                 int /*mode */ ,
                                 int /*npt */ ,
                                 DDXPointPtr    /*pPts */
);

extern _X_EXPORT void miPolylines(DrawablePtr pDrawable,
                                  GCPtr pGC,
                                  int mode,
                                  int npt,
                                  DDXPointPtr pPts);

/* miwindow.c */

extern _X_EXPORT void miClearToBackground(WindowPtr /*pWin */ ,
                                          int /*x */ ,
                                          int /*y */ ,
                                          int /*w */ ,
                                          int /*h */ ,
                                          Bool  /*generateExposures */
);

extern _X_EXPORT void miMarkWindow(WindowPtr    /*pWin */
);

extern _X_EXPORT Bool miMarkOverlappedWindows(WindowPtr /*pWin */ ,
                                              WindowPtr /*pFirst */ ,
                                              WindowPtr *       /*ppLayerWin */
);

extern _X_EXPORT void miHandleValidateExposures(WindowPtr       /*pWin */
);

extern _X_EXPORT void miMoveWindow(WindowPtr /*pWin */ ,
                                   int /*x */ ,
                                   int /*y */ ,
                                   WindowPtr /*pNextSib */ ,
                                   VTKind       /*kind */
);

extern _X_EXPORT void miResizeWindow(WindowPtr /*pWin */ ,
                                     int /*x */ ,
                                     int /*y */ ,
                                     unsigned int /*w */ ,
                                     unsigned int /*h */ ,
                                     WindowPtr    /*pSib */
);

extern _X_EXPORT WindowPtr miGetLayerWindow(WindowPtr   /*pWin */
);

extern _X_EXPORT void miSetShape(WindowPtr /*pWin */ ,
                                 int    /*kind */
);

extern _X_EXPORT void miChangeBorderWidth(WindowPtr /*pWin */ ,
                                          unsigned int  /*width */
);

extern _X_EXPORT void miMarkUnrealizedWindow(WindowPtr /*pChild */ ,
                                             WindowPtr /*pWin */ ,
                                             Bool       /*fromConfigure */
);

extern _X_EXPORT WindowPtr miSpriteTrace(SpritePtr pSprite, int x, int y);

extern _X_EXPORT WindowPtr miXYToWindow(ScreenPtr pScreen, SpritePtr pSprite, int x, int y);

/* mizerarc.c */

extern _X_EXPORT void miZeroPolyArc(DrawablePtr /*pDraw */ ,
                                    GCPtr /*pGC */ ,
                                    int /*narcs */ ,
                                    xArc *      /*parcs */
);

/* mizerline.c */

extern _X_EXPORT void miZeroLine(DrawablePtr /*dst */ ,
                                 GCPtr /*pgc */ ,
                                 int /*mode */ ,
                                 int /*nptInit */ ,
                                 DDXPointRec *  /*pptInit */
);

extern _X_EXPORT void miZeroDashLine(DrawablePtr /*dst */ ,
                                     GCPtr /*pgc */ ,
                                     int /*mode */ ,
                                     int /*nptInit */ ,
                                     DDXPointRec *      /*pptInit */
);

extern _X_EXPORT void miPolyFillArc(DrawablePtr /*pDraw */ ,
                                    GCPtr /*pGC */ ,
                                    int /*narcs */ ,
                                    xArc *      /*parcs */
);

#endif                          /* MI_H */