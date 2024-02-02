#ifndef ST_SCISSOR_H
#define ST_SCISSOR_H

struct gl_context;
struct pipe_blit_info;

void
st_window_rectangles_to_blit(const struct gl_context *ctx,
                             struct pipe_blit_info *blit);

#endif /* ST_SCISSOR_H */
