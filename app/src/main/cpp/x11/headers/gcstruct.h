#ifndef GCSTRUCT_H
#define GCSTRUCT_H

#include "gc.h"

#include "regionstr.h"
#include "region.h"
#include "pixmap.h"
#include "screenint.h"
#include "privates.h"
#include "xprotostr.h"

#define GCAllBits ((1 << (GCLastBit + 1)) - 1)

/*
 * functions which modify the state of the GC
 */

typedef struct _GCFuncs {
    void (*ValidateGC) (GCPtr /*pGC */ ,
                        unsigned long /*stateChanges */ ,
                        DrawablePtr /*pDrawable */ );

    void (*ChangeGC) (GCPtr /*pGC */ ,
                      unsigned long /*mask */ );

    void (*CopyGC) (GCPtr /*pGCSrc */ ,
                    unsigned long /*mask */ ,
                    GCPtr /*pGCDst */ );

    void (*DestroyGC) (GCPtr /*pGC */ );

    void (*ChangeClip) (GCPtr pGC,
                        int type,
                        void *pvalue,
                        int nrects);

    void (*DestroyClip) (GCPtr /*pGC */ );

    void (*CopyClip) (GCPtr /*pgcDst */ ,
                      GCPtr /*pgcSrc */ );
} GCFuncs;

/*
 * graphics operations invoked through a GC
 */

typedef struct _GCOps {
    void (*FillSpans) (DrawablePtr /*pDrawable */ ,
                       GCPtr /*pGC */ ,
                       int /*nInit */ ,
                       DDXPointPtr /*pptInit */ ,
                       int * /*pwidthInit */ ,
                       int /*fSorted */ );

    void (*SetSpans) (DrawablePtr /*pDrawable */ ,
                      GCPtr /*pGC */ ,
                      char * /*psrc */ ,
                      DDXPointPtr /*ppt */ ,
                      int * /*pwidth */ ,
                      int /*nspans */ ,
                      int /*fSorted */ );

    void (*PutImage) (DrawablePtr /*pDrawable */ ,
                      GCPtr /*pGC */ ,
                      int /*depth */ ,
                      int /*x */ ,
                      int /*y */ ,
                      int /*w */ ,
                      int /*h */ ,
                      int /*leftPad */ ,
                      int /*format */ ,
                      char * /*pBits */ );

    RegionPtr (*CopyArea) (DrawablePtr /*pSrc */ ,
                           DrawablePtr /*pDst */ ,
                           GCPtr /*pGC */ ,
                           int /*srcx */ ,
                           int /*srcy */ ,
                           int /*w */ ,
                           int /*h */ ,
                           int /*dstx */ ,
                           int /*dsty */ );

    RegionPtr (*CopyPlane) (DrawablePtr /*pSrcDrawable */ ,
                            DrawablePtr /*pDstDrawable */ ,
                            GCPtr /*pGC */ ,
                            int /*srcx */ ,
                            int /*srcy */ ,
                            int /*width */ ,
                            int /*height */ ,
                            int /*dstx */ ,
                            int /*dsty */ ,
                            unsigned long /*bitPlane */ );
    void (*PolyPoint) (DrawablePtr /*pDrawable */ ,
                       GCPtr /*pGC */ ,
                       int /*mode */ ,
                       int /*npt */ ,
                       DDXPointPtr /*pptInit */ );

    void (*Polylines) (DrawablePtr /*pDrawable */ ,
                       GCPtr /*pGC */ ,
                       int /*mode */ ,
                       int /*npt */ ,
                       DDXPointPtr /*pptInit */ );

    void (*PolySegment) (DrawablePtr /*pDrawable */ ,
                         GCPtr /*pGC */ ,
                         int /*nseg */ ,
                         xSegment * /*pSegs */ );

    void (*PolyRectangle) (DrawablePtr /*pDrawable */ ,
                           GCPtr /*pGC */ ,
                           int /*nrects */ ,
                           xRectangle * /*pRects */ );

    void (*PolyArc) (DrawablePtr /*pDrawable */ ,
                     GCPtr /*pGC */ ,
                     int /*narcs */ ,
                     xArc * /*parcs */ );

    void (*FillPolygon) (DrawablePtr /*pDrawable */ ,
                         GCPtr /*pGC */ ,
                         int /*shape */ ,
                         int /*mode */ ,
                         int /*count */ ,
                         DDXPointPtr /*pPts */ );

    void (*PolyFillRect) (DrawablePtr /*pDrawable */ ,
                          GCPtr /*pGC */ ,
                          int /*nrectFill */ ,
                          xRectangle * /*prectInit */ );

    void (*PolyFillArc) (DrawablePtr /*pDrawable */ ,
                         GCPtr /*pGC */ ,
                         int /*narcs */ ,
                         xArc * /*parcs */ );

    int (*PolyText8) (DrawablePtr /*pDrawable */ ,
                      GCPtr /*pGC */ ,
                      int /*x */ ,
                      int /*y */ ,
                      int /*count */ ,
                      char * /*chars */ );

    int (*PolyText16) (DrawablePtr /*pDrawable */ ,
                       GCPtr /*pGC */ ,
                       int /*x */ ,
                       int /*y */ ,
                       int /*count */ ,
                       unsigned short * /*chars */ );

    void (*ImageText8) (DrawablePtr /*pDrawable */ ,
                        GCPtr /*pGC */ ,
                        int /*x */ ,
                        int /*y */ ,
                        int /*count */ ,
                        char * /*chars */ );

    void (*ImageText16) (DrawablePtr /*pDrawable */ ,
                         GCPtr /*pGC */ ,
                         int /*x */ ,
                         int /*y */ ,
                         int /*count */ ,
                         unsigned short * /*chars */ );

    void (*ImageGlyphBlt) (DrawablePtr pDrawable,
                           GCPtr pGC,
                           int x,
                           int y,
                           unsigned int nglyph,
                           CharInfoPtr *ppci,
                           void *pglyphBase);

    void (*PolyGlyphBlt) (DrawablePtr pDrawable,
                          GCPtr pGC,
                          int x,
                          int y,
                          unsigned int nglyph,
                          CharInfoPtr *ppci,
                          void *pglyphBase);

    void (*PushPixels) (GCPtr /*pGC */ ,
                        PixmapPtr /*pBitMap */ ,
                        DrawablePtr /*pDst */ ,
                        int /*w */ ,
                        int /*h */ ,
                        int /*x */ ,
                        int /*y */ );
} GCOps;

/* there is padding in the bit fields because the Sun compiler doesn't
 * force alignment to 32-bit boundaries.  losers.
 */
typedef struct _GC {
    ScreenPtr pScreen;
    unsigned char depth;
    unsigned char alu;
    unsigned short lineWidth;
    unsigned short dashOffset;
    unsigned short numInDashList;
    unsigned char *dash;
    unsigned int lineStyle:2;
    unsigned int capStyle:2;
    unsigned int joinStyle:2;
    unsigned int fillStyle:2;
    unsigned int fillRule:1;
    unsigned int arcMode:1;
    unsigned int subWindowMode:1;
    unsigned int graphicsExposures:1;
    unsigned int miTranslate:1; /* should mi things translate? */
    unsigned int tileIsPixel:1; /* tile is solid pixel */
    unsigned int fExpose:1;     /* Call exposure handling */
    unsigned int freeCompClip:1;        /* Free composite clip */
    unsigned int scratch_inuse:1;       /* is this GC in a pool for reuse? */
    unsigned int unused:15;     /* see comment above */
    unsigned int planemask;
    unsigned int fgPixel;
    unsigned int bgPixel;
    /*
     * alas -- both tile and stipple must be here as they
     * are independently specifiable
     */
    PixUnion tile;
    PixmapPtr stipple;
    DDXPointRec patOrg;         /* origin for (tile, stipple) */
    DDXPointRec clipOrg;
    struct _Font *font;
    RegionPtr clientClip;
    unsigned int stateChanges; /* masked with GC_<kind> */
    unsigned int serialNumber;
    const GCFuncs *funcs;
    const GCOps *ops;
    PrivateRec *devPrivates;
    /*
     * The following were moved here from private storage to allow device-
     * independent access to them from screen wrappers.
     * --- 1997.11.03  Marc Aurele La France (tsi@xfree86.org)
     */
    PixmapPtr pRotatedPixmap;   /* tile/stipple rotated for alignment */
    RegionPtr pCompositeClip;
    /* fExpose & freeCompClip defined above */
} GC;

#endif                          /* GCSTRUCT_H */