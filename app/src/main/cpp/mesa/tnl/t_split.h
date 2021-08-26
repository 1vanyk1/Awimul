#ifndef _TNL_SPLIT_H
#define _TNL_SPLIT_H

#include "tnl.h"


/* True if a primitive can be split without copying of vertices, false
 * otherwise.
 */
GLboolean
_tnl_split_prim_inplace(GLenum mode, GLuint *first, GLuint *incr);

void
_tnl_split_inplace(struct gl_context *ctx,
                   const struct tnl_vertex_array arrays[],
                   const struct _mesa_prim *prim,
                   GLuint nr_prims,
                   const struct _mesa_index_buffer *ib,
                   GLuint num_instances,
                   GLuint base_instance,
                   tnl_draw_func draw,
                   const struct split_limits *limits);

/* Requires ib != NULL:
 */
void
_tnl_split_copy(struct gl_context *ctx,
                const struct tnl_vertex_array arrays[],
                const struct _mesa_prim *prim,
                GLuint nr_prims,
                const struct _mesa_index_buffer *ib,
                tnl_draw_func draw,
                const struct split_limits *limits);

#endif
