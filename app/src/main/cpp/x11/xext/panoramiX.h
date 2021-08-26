#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _PANORAMIX_H_
#define _PANORAMIX_H_

#define _PANORAMIX_SERVER
#include "../headers/extensions/panoramiXproto.h"
#undef _PANORAMIX_SERVER
#include "../headers/gcstruct.h"
#include "../headers/dixstruct.h"

typedef struct _PanoramiXInfo {
    XID id;
} PanoramiXInfo;

typedef struct {
    PanoramiXInfo info[MAXSCREENS];
    RESTYPE type;
    union {
        struct {
            char visibility;
            char class;
            char root;
        } win;
        struct {
            Bool shared;
        } pix;
        struct {
            Bool root;
        } pict;
        char raw_data[4];
    } u;
} PanoramiXRes;

#define FOR_NSCREENS_FORWARD(j) for(j = 0; j < PanoramiXNumScreens; j++)
#define FOR_NSCREENS_FORWARD_SKIP(j) for(j = 1; j < PanoramiXNumScreens; j++)
#define FOR_NSCREENS_BACKWARD(j) for(j = PanoramiXNumScreens - 1; j >= 0; j--)
#define FOR_NSCREENS(j) FOR_NSCREENS_FORWARD(j)

#define IS_SHARED_PIXMAP(r) (((r)->type == XRT_PIXMAP) && (r)->u.pix.shared)

#define IS_ROOT_DRAWABLE(d) (((d)->type == XRT_WINDOW) && (d)->u.win.root)
#endif                          /* _PANORAMIX_H_ */