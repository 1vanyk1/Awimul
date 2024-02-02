#ifndef P_TILE_H
#define P_TILE_H

#include "../include/pipe/p_compiler.h"
#include "../include/pipe/p_format.h"
#include "../include/pipe/p_state.h"

struct pipe_context;
struct pipe_transfer;

/**
 * Clip tile against transfer dims.
 *
 * XXX: this only clips width and height!
 *
 * \return TRUE if tile is totally clipped, FALSE otherwise
 */
static inline boolean
u_clip_tile(uint x, uint y, uint *w, uint *h, const struct pipe_box *box)
{
    if ((int) x >= box->width)
        return TRUE;
    if ((int) y >= box->height)
        return TRUE;
    if ((int) (x + *w) > box->width)
        *w = box->width - x;
    if ((int) (y + *h) > box->height)
        *h = box->height - y;
    return FALSE;
}

#ifdef __cplusplus
extern "C" {
#endif

void
pipe_get_tile_raw(struct pipe_transfer *pt,
                  const void *src,
                  uint x, uint y, uint w, uint h,
                  void *p, int dst_stride);

void
pipe_put_tile_raw(struct pipe_transfer *pt,
                  void *dst,
                  uint x, uint y, uint w, uint h,
                  const void *p, int src_stride);


void
pipe_get_tile_rgba(struct pipe_transfer *pt,
                   const void *src,
                   uint x, uint y, uint w, uint h,
                   enum pipe_format format,
                   void *dst);

void
pipe_put_tile_rgba(struct pipe_transfer *pt,
                   void *dst,
                   uint x, uint y, uint w, uint h,
                   enum pipe_format format,
                   const void *src);

#ifdef __cplusplus
}
#endif

#endif
