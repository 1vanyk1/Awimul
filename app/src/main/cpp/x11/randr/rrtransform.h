#ifndef _RRTRANSFORM_H_
#define _RRTRANSFORM_H_

#include "../headers/extensions/randr.h"
#include "../headers/picturestr.h"

typedef struct _rrTransform RRTransformRec, *RRTransformPtr;

struct _rrTransform {
    PictTransform transform;
    struct pict_f_transform f_transform;
    struct pict_f_transform f_inverse;
    PictFilterPtr filter;
    xFixed *params;
    int nparams;
    int width;
    int height;
};

extern _X_EXPORT void
RRTransformInit(RRTransformPtr transform);

extern _X_EXPORT void
RRTransformFini(RRTransformPtr transform);

extern _X_EXPORT Bool
RRTransformEqual(RRTransformPtr a, RRTransformPtr b);

extern _X_EXPORT Bool

RRTransformSetFilter(RRTransformPtr dst,
PictFilterPtr filter,
        xFixed * params, int nparams, int width, int height);

extern _X_EXPORT Bool
RRTransformCopy(RRTransformPtr dst, RRTransformPtr src);

/*
 * Compute the complete transformation matrix including
 * client-specified transform, rotation/reflection values and the crtc
 * offset.
 *
 * Return TRUE if the resulting transform is not a simple translation.
 */
extern _X_EXPORT Bool

RRTransformCompute(int x,
                   int y,
                   int width,
                   int height,
                   Rotation rotation,
                   RRTransformPtr rr_transform,
                   PictTransformPtr transform,
                   struct pict_f_transform *f_transform,
                   struct pict_f_transform *f_inverse);

#endif                          /* _RRTRANSFORM_H_ */
