#ifndef U_FRAMEBUFFER_H
#define U_FRAMEBUFFER_H


#include "../include/pipe/p_compiler.h"
#include "../include/pipe/p_state.h"


#ifdef __cplusplus
extern "C" {
#endif

extern boolean
util_framebuffer_state_equal(const struct pipe_framebuffer_state *dst,
                             const struct pipe_framebuffer_state *src);

extern void
util_copy_framebuffer_state(struct pipe_framebuffer_state *dst,
                            const struct pipe_framebuffer_state *src);


extern void
util_unreference_framebuffer_state(struct pipe_framebuffer_state *fb);


extern boolean
util_framebuffer_min_size(const struct pipe_framebuffer_state *fb,
                          unsigned *width,
                          unsigned *height);


extern unsigned
util_framebuffer_get_num_layers(const struct pipe_framebuffer_state *fb);


extern unsigned
util_framebuffer_get_num_samples(const struct pipe_framebuffer_state *fb);


extern void
util_sample_locations_flip_y(struct pipe_screen *screen, unsigned fb_height,
                             unsigned samples, uint8_t *locations);


#ifdef __cplusplus
}
#endif

#endif /* U_FRAMEBUFFER_H */
