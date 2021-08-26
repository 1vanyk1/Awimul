#ifndef U_SAMPLER_H
#define U_SAMPLER_H


#include "../include/pipe/p_defines.h"
#include "../include/pipe/p_format.h"
#include "../include/pipe/p_state.h"

#ifdef __cplusplus
extern "C" {
#endif


void
u_sampler_view_default_template(struct pipe_sampler_view *view,
                                const struct pipe_resource *texture,
                                enum pipe_format format);

void
u_sampler_view_default_dx9_template(struct pipe_sampler_view *view,
                                    const struct pipe_resource *texture,
                                    enum pipe_format format);


#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* U_SAMPLER_H */
