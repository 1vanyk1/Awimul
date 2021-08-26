#include "../include/pipe/p_context.h"
#include "../include/pipe/p_defines.h"
#include "u_inlines.h"
#include "u_draw_quad.h"
#include "u_memory.h"
#include "../gallium/cso_cache/cso_context.h"


/**
 * Draw a simple vertex buffer / primitive.
 * Limited to float[4] vertex attribs, tightly packed.
 */
void
util_draw_vertex_buffer(struct pipe_context *pipe,
                        struct cso_context *cso,
                        struct pipe_resource *vbuf,
                        uint vbuf_slot,
                        uint offset,
                        uint prim_type,
                        uint num_verts,
                        uint num_attribs)
{
    struct pipe_vertex_buffer vbuffer;

    assert(num_attribs <= PIPE_MAX_ATTRIBS);

    /* tell pipe about the vertex buffer */
    memset(&vbuffer, 0, sizeof(vbuffer));
    vbuffer.buffer.resource = vbuf;
    vbuffer.stride = num_attribs * 4 * sizeof(float);  /* vertex size */
    vbuffer.buffer_offset = offset;

    /* note: vertex elements already set by caller */

    if (cso) {
        cso_set_vertex_buffers(cso, vbuf_slot, 1, &vbuffer);
        cso_draw_arrays(cso, prim_type, 0, num_verts);
    } else {
        pipe->set_vertex_buffers(pipe, vbuf_slot, 1, 0, false, &vbuffer);
        util_draw_arrays(pipe, prim_type, 0, num_verts);
    }
}


/**
 * Draw a simple vertex buffer / primitive.
 * Limited to float[4] vertex attribs, tightly packed.
 */
void
util_draw_user_vertex_buffer(struct cso_context *cso, void *buffer,
                             uint prim_type, uint num_verts, uint num_attribs)
{
    struct pipe_vertex_buffer vbuffer = {0};

    assert(num_attribs <= PIPE_MAX_ATTRIBS);

    vbuffer.is_user_buffer = true;
    vbuffer.buffer.user = buffer;
    vbuffer.stride = num_attribs * 4 * sizeof(float);  /* vertex size */

    /* note: vertex elements already set by caller */

    cso_set_vertex_buffers(cso, 0, 1, &vbuffer);
    cso_draw_arrays(cso, prim_type, 0, num_verts);
}
