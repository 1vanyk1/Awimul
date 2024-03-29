#ifndef PP_EXTERNAL_FILTERS_H
#define PP_EXTERNAL_FILTERS_H

#include "postprocess.h"

#define PP_FILTERS 6            /* Increment this if you add filters */
#define PP_MAX_PASSES 6


typedef bool (*pp_init_func) (struct pp_queue_t *, unsigned int,
                              unsigned int);
typedef void (*pp_free_func) (struct pp_queue_t *, unsigned int);

struct pp_filter_t
{
    const char *name;            /* Config name */
    unsigned int inner_tmps;     /* Request how many inner temps */
    unsigned int shaders;        /* Request how many shaders */
    unsigned int verts;          /* How many are vertex shaders */
    pp_init_func init;           /* Init function */
    pp_func main;                /* Run function */
    pp_free_func free;           /* Free function */
};

/*	Order matters. Put new filters in a suitable place. */

static const struct pp_filter_t pp_filters[PP_FILTERS] = {
/*    name			inner	shaders	verts	init			run                       free   */
        { "pp_noblue",		0,	2,	1,	pp_noblue_init,		pp_nocolor,               pp_nocolor_free },
        { "pp_nogreen",		0,	2,	1,	pp_nogreen_init,	pp_nocolor,               pp_nocolor_free },
        { "pp_nored",		0,	2,	1,	pp_nored_init,		pp_nocolor,               pp_nocolor_free },
        { "pp_celshade",		0,	2,	1,	pp_celshade_init,	pp_nocolor,               pp_celshade_free },
        { "pp_jimenezmlaa",		2,	5,	2,	pp_jimenezmlaa_init,	pp_jimenezmlaa,           pp_jimenezmlaa_free },
        { "pp_jimenezmlaa_color",	2,	5,	2,	pp_jimenezmlaa_init_color, pp_jimenezmlaa_color,  pp_jimenezmlaa_free },
};

#endif
