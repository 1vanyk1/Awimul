#ifndef U_VBUF_H
#define U_VBUF_H

/* This module takes care of user buffer uploads and vertex format fallbacks.
 * It's designed for the drivers which don't want to use the Draw module.
 * There is a more detailed description at the beginning of the .c file.
 */

#include "../include/pipe/p_context.h"
#include "../include/pipe/p_state.h"
#include "../include/pipe/p_format.h"

struct cso_context;
struct cso_velems_state;
struct u_vbuf;

/* Hardware vertex fetcher limitations can be described by this structure. */
struct u_vbuf_caps {
    enum pipe_format format_translation[PIPE_FORMAT_COUNT];

    /* Whether vertex fetches don't have to be 4-byte-aligned. */
    /* TRUE if hardware supports it. */
    unsigned buffer_offset_unaligned:1;
    unsigned buffer_stride_unaligned:1;
    unsigned velem_src_offset_unaligned:1;

    /* Whether the driver supports user vertex buffers. */
    unsigned user_vertex_buffers:1;

    /* Maximum number of vertex buffers */
    unsigned max_vertex_buffers:6;

    bool fallback_always;
    bool fallback_only_for_user_vbuffers;
};


void u_vbuf_get_caps(struct pipe_screen *screen, struct u_vbuf_caps *caps,
                     bool needs64b);

struct u_vbuf *
u_vbuf_create(struct pipe_context *pipe, struct u_vbuf_caps *caps);

void u_vbuf_destroy(struct u_vbuf *mgr);

/* State and draw functions. */
void u_vbuf_set_vertex_elements(struct u_vbuf *mgr,
                                const struct cso_velems_state *velems);
void u_vbuf_unset_vertex_elements(struct u_vbuf *mgr);
void u_vbuf_set_vertex_buffers(struct u_vbuf *mgr,
                               unsigned start_slot, unsigned count,
                               unsigned unbind_num_trailing_slots,
                               bool take_ownership,
                               const struct pipe_vertex_buffer *bufs);
void u_vbuf_draw_vbo(struct u_vbuf *mgr, const struct pipe_draw_info *info,
                     const struct pipe_draw_indirect_info *indirect,
                     const struct pipe_draw_start_count draw);
void u_vbuf_get_minmax_index(struct pipe_context *pipe,
                             const struct pipe_draw_info *info,
                             const struct pipe_draw_start_count *draw,
                             unsigned *out_min_index, unsigned *out_max_index);

/* Save/restore functionality. */
void u_vbuf_save_vertex_elements(struct u_vbuf *mgr);
void u_vbuf_restore_vertex_elements(struct u_vbuf *mgr);

#endif
