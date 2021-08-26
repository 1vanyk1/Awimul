#include "u_prim.h"


/** Return string name of given primitive type */
const char *
u_prim_name(enum pipe_prim_type prim)
{
    static const struct debug_named_value names[] = {
            DEBUG_NAMED_VALUE(PIPE_PRIM_POINTS),
            DEBUG_NAMED_VALUE(PIPE_PRIM_LINES),
            DEBUG_NAMED_VALUE(PIPE_PRIM_LINE_LOOP),
            DEBUG_NAMED_VALUE(PIPE_PRIM_LINE_STRIP),
            DEBUG_NAMED_VALUE(PIPE_PRIM_TRIANGLES),
            DEBUG_NAMED_VALUE(PIPE_PRIM_TRIANGLE_STRIP),
            DEBUG_NAMED_VALUE(PIPE_PRIM_TRIANGLE_FAN),
            DEBUG_NAMED_VALUE(PIPE_PRIM_QUADS),
            DEBUG_NAMED_VALUE(PIPE_PRIM_QUAD_STRIP),
            DEBUG_NAMED_VALUE(PIPE_PRIM_POLYGON),
            DEBUG_NAMED_VALUE(PIPE_PRIM_LINES_ADJACENCY),
            DEBUG_NAMED_VALUE(PIPE_PRIM_LINE_STRIP_ADJACENCY),
            DEBUG_NAMED_VALUE(PIPE_PRIM_TRIANGLES_ADJACENCY),
            DEBUG_NAMED_VALUE(PIPE_PRIM_TRIANGLE_STRIP_ADJACENCY),
            DEBUG_NAMED_VALUE_END
    };
    return debug_dump_enum(names, prim);
}
