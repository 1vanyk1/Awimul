#ifndef PIXMAN_REGION16
#define PIXMAN_REGION16

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#undef PIXMAN_DISABLE_DEPRECATED

#include "pixman-private.h"

#include <stdlib.h>

typedef pixman_box16_t		box_type_t;
typedef pixman_region16_data_t	region_data_type_t;
typedef pixman_region16_t	region_type_t;
typedef int32_t                 overflow_int_t;

typedef struct {
    int x, y;
} point_type_t;

#define PREFIX(x) pixman_region##x

#define PIXMAN_REGION_MAX INT16_MAX
#define PIXMAN_REGION_MIN INT16_MIN

#include "pixman-region.c"

/* This function exists only to make it possible to preserve the X ABI -
 * it should go away at first opportunity.
 *
 * The problem is that the X ABI exports the three structs and has used
 * them through macros. So the X server calls this function with
 * the addresses of those structs which makes the existing code continue to
 * work.
 */
PIXMAN_EXPORT void
pixman_region_set_static_pointers (pixman_box16_t *empty_box,
                                   pixman_region16_data_t *empty_data,
                                   pixman_region16_data_t *broken_data)
{
    pixman_region_empty_box = empty_box;
    pixman_region_empty_data = empty_data;
    pixman_broken_data = broken_data;
}

#endif