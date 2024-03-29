#include "u_debug.h"
#include "u_inlines.h"
#include "u_math.h"
#include "format/u_format.h"
#include "u_draw.h"


/**
 * Returns the largest legal index value plus one for the current set
 * of bound vertex buffers.  Regardless of any other consideration,
 * all vertex lookups need to be clamped to 0..max_index-1 to prevent
 * an out-of-bound access.
 *
 * Note that if zero is returned it means that one or more buffers is
 * too small to contain any valid vertex data.
 */
unsigned
util_draw_max_index(
        const struct pipe_vertex_buffer *vertex_buffers,
        const struct pipe_vertex_element *vertex_elements,
        unsigned nr_vertex_elements,
        const struct pipe_draw_info *info)
{
    unsigned max_index;
    unsigned i;

    max_index = ~0U - 1;
    for (i = 0; i < nr_vertex_elements; i++) {
        const struct pipe_vertex_element *element =
                &vertex_elements[i];
        const struct pipe_vertex_buffer *buffer =
                &vertex_buffers[element->vertex_buffer_index];
        unsigned buffer_size;
        const struct util_format_description *format_desc;
        unsigned format_size;

        if (buffer->is_user_buffer || !buffer->buffer.resource) {
            continue;
        }

        assert(buffer->buffer.resource->height0 == 1);
        assert(buffer->buffer.resource->depth0 == 1);
        buffer_size = buffer->buffer.resource->width0;

        format_desc = util_format_description(element->src_format);
        assert(format_desc->block.width == 1);
        assert(format_desc->block.height == 1);
        assert(format_desc->block.bits % 8 == 0);
        format_size = format_desc->block.bits/8;

        if (buffer->buffer_offset >= buffer_size) {
            /* buffer is too small */
            return 0;
        }

        buffer_size -= buffer->buffer_offset;

        if (element->src_offset >= buffer_size) {
            /* buffer is too small */
            return 0;
        }

        buffer_size -= element->src_offset;

        if (format_size > buffer_size) {
            /* buffer is too small */
            return 0;
        }

        buffer_size -= format_size;

        if (buffer->stride != 0) {
            unsigned buffer_max_index;

            buffer_max_index = buffer_size / buffer->stride;

            if (element->instance_divisor == 0) {
                /* Per-vertex data */
                max_index = MIN2(max_index, buffer_max_index);
            }
            else {
                /* Per-instance data. Simply make sure gallium frontends didn't
                 * request more instances than those that fit in the buffer */
                if ((info->start_instance + info->instance_count)/element->instance_divisor
                    > (buffer_max_index + 1)) {
                    /* FIXME: We really should stop thinking in terms of maximum
                     * indices/instances and simply start clamping against buffer
                     * size. */
                    debug_printf("%s: too many instances for vertex buffer\n",
                                 __FUNCTION__);
                    return 0;
                }
            }
        }
    }

    return max_index + 1;
}


/* This extracts the draw arguments from the indirect resource,
 * puts them into a new instance of pipe_draw_info, and calls draw_vbo on it.
 */
void
util_draw_indirect(struct pipe_context *pipe,
                   const struct pipe_draw_info *info_in,
                   const struct pipe_draw_indirect_info *indirect)
{
    struct pipe_draw_info info;
    struct pipe_transfer *transfer;
    uint32_t *params;
    unsigned num_params = info_in->index_size ? 5 : 4;

    assert(indirect);
    assert(!indirect->count_from_stream_output);

    memcpy(&info, info_in, sizeof(info));

    uint32_t draw_count = indirect->draw_count;

    if (indirect->indirect_draw_count) {
        struct pipe_transfer *dc_transfer;
        uint32_t *dc_param = pipe_buffer_map_range(pipe,
                                                   indirect->indirect_draw_count,
                                                   indirect->indirect_draw_count_offset,
                                                   4, PIPE_MAP_READ, &dc_transfer);
        if (!dc_transfer) {
            debug_printf("%s: failed to map indirect draw count buffer\n", __FUNCTION__);
            return;
        }
        if (dc_param[0] < draw_count)
            draw_count = dc_param[0];
        pipe_buffer_unmap(pipe, dc_transfer);
    }

    if (indirect->stride)
        num_params = MIN2(indirect->stride / 4, num_params);
    params = (uint32_t *)
            pipe_buffer_map_range(pipe,
                                  indirect->buffer,
                                  indirect->offset,
                                  (num_params * indirect->draw_count) * sizeof(uint32_t),
                                  PIPE_MAP_READ,
                                  &transfer);
    if (!transfer) {
        debug_printf("%s: failed to map indirect buffer\n", __FUNCTION__);
        return;
    }

    for (unsigned i = 0; i < draw_count; i++) {
        struct pipe_draw_start_count draw;

        draw.count = params[0];
        info.instance_count = params[1];
        draw.start = params[2];
        info.index_bias = info_in->index_size ? params[3] : 0;
        info.start_instance = info_in->index_size ? params[4] : params[3];
        info.drawid = i;

        pipe->draw_vbo(pipe, &info, NULL, &draw, 1);

        params += indirect->stride / 4;
    }
    pipe_buffer_unmap(pipe, transfer);
}

void
util_draw_multi(struct pipe_context *pctx, const struct pipe_draw_info *info,
                const struct pipe_draw_indirect_info *indirect,
                const struct pipe_draw_start_count *draws,
                unsigned num_draws)
{
    struct pipe_draw_info tmp_info = *info;

    /* If you call this with num_draws==1, that is probably going to be
     * an infinite loop
     */
    assert(num_draws > 1);

    for (unsigned i = 0; i < num_draws; i++) {
        if (indirect || (draws[i].count && info->instance_count))
            pctx->draw_vbo(pctx, &tmp_info, indirect, &draws[i], 1);
        if (tmp_info.increment_draw_id)
            tmp_info.drawid++;
    }
}
