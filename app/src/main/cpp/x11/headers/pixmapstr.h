#ifndef PIXMAPSTRUCT_H
#define PIXMAPSTRUCT_H
#include "pixmap.h"
#include "screenint.h"
#include "regionstr.h"
#include "privates.h"
#include "../miext/damage/damage.h"
#include "extensions/randr.h"
#include "picturestr.h"

typedef struct _Drawable {
    unsigned char type;         /* DRAWABLE_<type> */
    unsigned char _class;        /* specific to type */
    unsigned char depth;
    unsigned char bitsPerPixel;
    XID id;                     /* resource id */
    short x;                    /* window: screen absolute, pixmap: 0 */
    short y;                    /* window: screen absolute, pixmap: 0 */
    unsigned short width;
    unsigned short height;
    ScreenPtr pScreen;
    unsigned long serialNumber;
} DrawableRec;

/*
 * PIXMAP -- device dependent
 */

typedef struct _Pixmap {
    DrawableRec drawable;
    PrivateRec *devPrivates;
    int refcnt;
    int devKind;                /* This is the pitch of the pixmap, typically width*bpp/8. */
    DevUnion devPrivate;        /* When !NULL, devPrivate.ptr points to the raw pixel data. */
    short screen_x;
    short screen_y;
    unsigned usage_hint;        /* see CREATE_PIXMAP_USAGE_* */

    PixmapPtr primary_pixmap;    /* pointer to primary copy of pixmap for pixmap sharing */
} PixmapRec;

typedef struct _PixmapDirtyUpdate {
    DrawablePtr src;            /* Root window / shared pixmap */
    PixmapPtr secondary_dst;    /* Shared / scanout pixmap */
    int x, y;
    DamagePtr damage;
    struct xorg_list ent;
    int dst_x, dst_y;
    Rotation rotation;
    PictTransform transform;
    struct pixman_f_transform f_transform, f_inverse;
} PixmapDirtyUpdateRec;

static inline void
PixmapBox(BoxPtr box, PixmapPtr pixmap)
{
    box->x1 = 0;
    box->x2 = pixmap->drawable.width;

    box->y1 = 0;
    box->y2 = pixmap->drawable.height;
}


static inline void
PixmapRegionInit(RegionPtr region, PixmapPtr pixmap)
{
    BoxRec box;

    PixmapBox(&box, pixmap);
    RegionInit(region, &box, 1);
}

#endif                          /* PIXMAPSTRUCT_H */
