#ifndef PIXMAP_H
#define PIXMAP_H

#include "misc.h"
#include "screenint.h"
#include "regionstr.h"
#include "extensions/randr.h"
/* types for Drawable */
#define DRAWABLE_WINDOW 0
#define DRAWABLE_PIXMAP 1
#define UNDRAWABLE_WINDOW 2

/* corresponding type masks for dixLookupDrawable() */
#define M_DRAWABLE_WINDOW	(1<<0)
#define M_DRAWABLE_PIXMAP	(1<<1)
#define M_UNDRAWABLE_WINDOW	(1<<2)
#define M_ANY			(-1)
#define M_WINDOW	(M_DRAWABLE_WINDOW|M_UNDRAWABLE_WINDOW)
#define M_DRAWABLE	(M_DRAWABLE_WINDOW|M_DRAWABLE_PIXMAP)
#define M_UNDRAWABLE	(M_UNDRAWABLE_WINDOW)

/* flags to PaintWindow() */
#define PW_BACKGROUND 0
#define PW_BORDER 1

#define NullPixmap ((PixmapPtr)0)

typedef struct _Drawable *DrawablePtr;
typedef struct _Pixmap *PixmapPtr;

typedef struct _PixmapDirtyUpdate *PixmapDirtyUpdatePtr;

typedef union _PixUnion {
    PixmapPtr pixmap;
    unsigned long pixel;
} PixUnion;

#define SamePixUnion(a,b,isPixel)\
    ((isPixel) ? (a).pixel == (b).pixel : (a).pixmap == (b).pixmap)

#define EqualPixUnion(as, a, bs, b)				\
    ((as) == (bs) && (SamePixUnion (a, b, as)))

#define OnScreenDrawable(type) \
	(type == DRAWABLE_WINDOW)

#define WindowDrawable(type) \
	((type == DRAWABLE_WINDOW) || (type == UNDRAWABLE_WINDOW))

extern _X_EXPORT PixmapPtr GetScratchPixmapHeader(ScreenPtr pScreen,
                                                  int width,
                                                  int height,
                                                  int depth,
                                                  int bitsPerPixel,
                                                  int devKind,
                                                  void *pPixData);

extern _X_EXPORT void FreeScratchPixmapHeader(PixmapPtr /*pPixmap */ );

extern _X_EXPORT Bool CreateScratchPixmapsForScreen(ScreenPtr /*pScreen */ );

extern _X_EXPORT void FreeScratchPixmapsForScreen(ScreenPtr /*pScreen */ );

extern _X_EXPORT PixmapPtr AllocatePixmap(ScreenPtr /*pScreen */ ,
                                          int /*pixDataSize */ );

extern _X_EXPORT void FreePixmap(PixmapPtr /*pPixmap */ );

extern _X_EXPORT PixmapPtr
PixmapShareToSecondary(PixmapPtr pixmap, ScreenPtr secondary);

extern _X_EXPORT void
PixmapUnshareSecondaryPixmap(PixmapPtr secondary_pixmap);

#define HAS_DIRTYTRACKING_ROTATION 1
#define HAS_DIRTYTRACKING_DRAWABLE_SRC 1
extern _X_EXPORT Bool
PixmapStartDirtyTracking(DrawablePtr src,
                         PixmapPtr slave_dst,
                         int x, int y, int dst_x, int dst_y,
                         Rotation rotation);

extern _X_EXPORT Bool
PixmapStopDirtyTracking(DrawablePtr src, PixmapPtr slave_dst);

/* helper function, drivers can do this themselves if they can do it more
   efficiently */
extern _X_EXPORT Bool
PixmapSyncDirtyHelper(PixmapDirtyUpdatePtr dirty);

#endif                          /* PIXMAP_H */
