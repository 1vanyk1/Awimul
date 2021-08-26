#include "../main/glheader.h"
#include "../main/mtypes.h"
#include "../vbo/vbo.h"

#include "t_split.h"


/* True if a primitive can be split without copying of vertices, false
 * otherwise.
 */
GLboolean
_tnl_split_prim_inplace(GLenum mode, GLuint *first, GLuint *incr)
{
    switch (mode) {
        case GL_POINTS:
            *first = 1;
            *incr = 1;
            return GL_TRUE;
        case GL_LINES:
            *first = 2;
            *incr = 2;
            return GL_TRUE;
        case GL_LINE_STRIP:
            *first = 2;
            *incr = 1;
            return GL_TRUE;
        case GL_TRIANGLES:
            *first = 3;
            *incr = 3;
            return GL_TRUE;
        case GL_TRIANGLE_STRIP:
            *first = 3;
            *incr = 1;
            return GL_TRUE;
        case GL_QUADS:
            *first = 4;
            *incr = 4;
            return GL_TRUE;
        case GL_QUAD_STRIP:
            *first = 4;
            *incr = 2;
            return GL_TRUE;
        default:
            *first = 0;
            *incr = 1;		/* so that count % incr works */
            return GL_FALSE;
    }
}



void
_tnl_split_prims(struct gl_context *ctx,
                 const struct tnl_vertex_array arrays[],
                 const struct _mesa_prim *prim,
                 GLuint nr_prims,
                 const struct _mesa_index_buffer *ib,
                 GLuint min_index,
                 GLuint max_index,
                 GLuint num_instances,
                 GLuint base_instance,
                 tnl_draw_func draw,
                 const struct split_limits *limits)
{
    if (ib) {
        if (limits->max_indices == 0) {
            /* Could traverse the indices, re-emitting vertices in turn.
             * But it's hard to see why this case would be needed - for
             * software tnl, it is better to convert to non-indexed
             * rendering after transformation is complete.  Are there any devices
             * with hardware tnl that cannot do indexed rendering?
             *
             * For now, this path is disabled.
             */
            assert(0);
        }
        else if (max_index - min_index >= limits->max_verts) {
            /* The vertex buffers are too large for hardware (or the
             * swtnl module).  Traverse the indices, re-emitting vertices
             * in turn.  Use a vertex cache to preserve some of the
             * sharing from the original index list.
             */
            _tnl_split_copy(ctx, arrays, prim, nr_prims, ib, draw, limits);
        }
        else if (ib->count > limits->max_indices) {
            /* The index buffer is too large for hardware.  Try to split
             * on whole-primitive boundaries, otherwise try to split the
             * individual primitives.
             */
            _tnl_split_inplace(ctx, arrays, prim, nr_prims, ib,
                               num_instances, base_instance, draw, limits);
        }
        else {
            /* Why were we called? */
            assert(0);
        }
    }
    else {
        if (max_index - min_index >= limits->max_verts) {
            /* The vertex buffer is too large for hardware (or the swtnl
             * module).  Try to split on whole-primitive boundaries,
             * otherwise try to split the individual primitives.
             */
            _tnl_split_inplace(ctx, arrays, prim, nr_prims, ib,
                               num_instances, base_instance, draw, limits);
        }
        else {
            /* Why were we called? */
            assert(0);
        }
    }
}

