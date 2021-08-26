#ifndef S_TEXFILTER_H
#define S_TEXFILTER_H


#include "s_context.h"

struct gl_context;
struct gl_texture_object;


extern texture_sample_func
_swrast_choose_texture_sample_func( struct gl_context *ctx,
                                    const struct gl_texture_object *tObj,
                                    const struct gl_sampler_object *sampler);


#endif
