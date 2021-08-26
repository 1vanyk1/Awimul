#ifndef _T_REBASE_H_
#define _T_REBASE_H_

#include "tnl.h"

void t_rebase_prims( struct gl_context *ctx,
                     const struct tnl_vertex_array *arrays,
                     const struct _mesa_prim *prim,
                     GLuint nr_prims,
                     const struct _mesa_index_buffer *ib,
                     GLuint min_index,
                     GLuint max_index,
                     GLuint num_instances,
                     GLuint base_instance,
                     tnl_draw_func draw );

#endif
