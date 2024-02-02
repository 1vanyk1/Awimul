#include "../main/macros.h"
#include "../main/mtypes.h"
#include "../include/pipe/p_state.h"

#include "st_scissor.h"

void
st_window_rectangles_to_blit(const struct gl_context *ctx,
                             struct pipe_blit_info *blit)
{
    unsigned i;

    blit->num_window_rectangles = ctx->Scissor.NumWindowRects;
    blit->window_rectangle_include =
            ctx->Scissor.WindowRectMode == GL_INCLUSIVE_EXT;
    for (i = 0; i < blit->num_window_rectangles; i++) {
        const struct gl_scissor_rect *src_rect = &ctx->Scissor.WindowRects[i];
        struct pipe_scissor_state *dst_rect = &blit->window_rectangles[i];
        dst_rect->minx = MAX2(src_rect->X, 0);
        dst_rect->miny = MAX2(src_rect->Y, 0);
        dst_rect->maxx = MAX2(src_rect->X + src_rect->Width, 0);
        dst_rect->maxy = MAX2(src_rect->Y + src_rect->Height, 0);
    }
}
