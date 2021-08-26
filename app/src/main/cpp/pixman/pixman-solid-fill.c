#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "pixman-private.h"

static uint32_t
color_to_uint32 (const pixman_color_t *color)
{
    return
            ((unsigned int) color->alpha >> 8 << 24) |
            ((unsigned int) color->red >> 8 << 16) |
            ((unsigned int) color->green & 0xff00) |
            ((unsigned int) color->blue >> 8);
}

static argb_t
color_to_float (const pixman_color_t *color)
{
    argb_t result;

    result.a = pixman_unorm_to_float (color->alpha, 16);
    result.r = pixman_unorm_to_float (color->red, 16);
    result.g = pixman_unorm_to_float (color->green, 16);
    result.b = pixman_unorm_to_float (color->blue, 16);

    return result;
}

PIXMAN_EXPORT pixman_image_t *
pixman_image_create_solid_fill (const pixman_color_t *color)
{
    pixman_image_t *img = _pixman_image_allocate ();

    if (!img)
        return NULL;

    img->type = SOLID;
    img->solid.color = *color;
    img->solid.color_32 = color_to_uint32 (color);
    img->solid.color_float = color_to_float (color);

    return img;
}

