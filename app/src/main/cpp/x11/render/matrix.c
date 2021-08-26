#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/misc.h"
#include "../headers/scrnintstr.h"
#include "../headers/os.h"
#include "../headers/regionstr.h"
#include "../headers/validate.h"
#include "../headers/windowstr.h"
#include "../headers/input.h"
#include "../headers/resource.h"
#include "../headers/colormapst.h"
#include "../headers/cursorstr.h"
#include "../headers/dixstruct.h"
#include "../headers/gcstruct.h"
#include "../headers/servermd.h"
#include "../headers/picturestr.h"

void
PictTransform_from_xRenderTransform(PictTransformPtr pict,
                                    xRenderTransform * render)
{
    pict->matrix[0][0] = render->matrix11;
    pict->matrix[0][1] = render->matrix12;
    pict->matrix[0][2] = render->matrix13;

    pict->matrix[1][0] = render->matrix21;
    pict->matrix[1][1] = render->matrix22;
    pict->matrix[1][2] = render->matrix23;

    pict->matrix[2][0] = render->matrix31;
    pict->matrix[2][1] = render->matrix32;
    pict->matrix[2][2] = render->matrix33;
}

void
xRenderTransform_from_PictTransform(xRenderTransform * render,
                                    PictTransformPtr pict)
{
    render->matrix11 = pict->matrix[0][0];
    render->matrix12 = pict->matrix[0][1];
    render->matrix13 = pict->matrix[0][2];

    render->matrix21 = pict->matrix[1][0];
    render->matrix22 = pict->matrix[1][1];
    render->matrix23 = pict->matrix[1][2];

    render->matrix31 = pict->matrix[2][0];
    render->matrix32 = pict->matrix[2][1];
    render->matrix33 = pict->matrix[2][2];
}

Bool
PictureTransformPoint(PictTransformPtr transform, PictVectorPtr vector)
{
    return pixman_transform_point(transform, vector);
}

Bool
PictureTransformPoint3d(PictTransformPtr transform, PictVectorPtr vector)
{
    return pixman_transform_point_3d(transform, vector);
}
