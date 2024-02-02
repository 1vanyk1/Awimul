#ifndef U_GENMIPMAP_H
#define U_GENMIPMAP_H

#include "../include/pipe/p_state.h"


#ifdef __cplusplus
extern "C" {
#endif


struct pipe_context;

extern boolean
util_gen_mipmap(struct pipe_context *pipe, struct pipe_resource *pt,
                enum pipe_format format, uint base_level, uint last_level,
                uint first_layer, uint last_layer, uint filter);


#ifdef __cplusplus
}
#endif

#endif
