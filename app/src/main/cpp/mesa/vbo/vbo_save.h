#ifndef VBO_SAVE_H
#define VBO_SAVE_H

#include "vbo.h"
#include "vbo_attrib.h"

/* For display lists, this structure holds a run of vertices of the
 * same format, and a strictly well-formed set of begin/end pairs,
 * starting on the first vertex and ending at the last.  Vertex
 * copying on buffer breaks is precomputed according to these
 * primitives, though there are situations where the copying will need
 * correction at execute-time, perhaps by replaying the list as
 * immediate mode commands.
 *
 * On executing this list, the 'current' values may be updated with
 * the values of the final vertex, and often no fixup of the start of
 * the vertex list is required.
 *
 * Eval and other commands that don't fit into these vertex lists are
 * compiled using the fallback opcode mechanism provided by dlist.c.
 */
struct vbo_save_vertex_list {
    struct gl_vertex_array_object *VAO[VP_MODE_MAX];

    /* Copy of the final vertex from node->vertex_store->bufferobj.
     * Keep this in regular (non-VBO) memory to avoid repeated
     * map/unmap of the VBO when updating GL current data.
     */
    fi_type *current_data;

    GLuint vertex_count;         /**< number of vertices in this list */
    GLuint wrap_count;		/* number of copied vertices at start */

    struct _mesa_prim *prims;
    GLuint prim_count;
    GLuint min_index, max_index;

    struct {
        struct _mesa_index_buffer ib;
        struct pipe_draw_info info;
        unsigned char *mode;
        struct pipe_draw_start_count *start_count;
        unsigned num_draws;
    } merged;

    struct vbo_save_primitive_store *prim_store;
};


/**
 * Return the stride in bytes of the display list node.
 */
static inline GLsizei
_vbo_save_get_stride(const struct vbo_save_vertex_list *node)
{
    return node->VAO[0]->BufferBinding[0].Stride;
}


/**
 * Return the first referenced vertex index in the display list node.
 */
static inline GLuint
_vbo_save_get_min_index(const struct vbo_save_vertex_list *node)
{
    assert(node->prim_count > 0);
    return node->min_index;
}


/**
 * Return the last referenced vertex index in the display list node.
 */
static inline GLuint
_vbo_save_get_max_index(const struct vbo_save_vertex_list *node)
{
    assert(node->prim_count > 0);
    return node->max_index;
}


/**
 * Return the vertex count in the display list node.
 */
static inline GLuint
_vbo_save_get_vertex_count(const struct vbo_save_vertex_list *node)
{
    assert(node->prim_count > 0);
    const struct _mesa_prim *first_prim = &node->prims[0];
    const struct _mesa_prim *last_prim = &node->prims[node->prim_count - 1];
    return last_prim->start - first_prim->start + last_prim->count;
}


/* These buffers should be a reasonable size to support upload to
 * hardware.  Current vbo implementation will re-upload on any
 * changes, so don't make too big or apps which dynamically create
 * dlists and use only a few times will suffer.
 *
 * Consider stategy of uploading regions from the VBO on demand in the
 * case of dynamic vbos.  Then make the dlist code signal that
 * likelyhood as it occurs.  No reason we couldn't change usage
 * internally even though this probably isn't allowed for client VBOs?
 */
#define VBO_SAVE_BUFFER_SIZE (256*1024) /* dwords */
#define VBO_SAVE_PRIM_SIZE   128
#define VBO_SAVE_PRIM_MODE_MASK         0x3f
#define VBO_SAVE_INDEX_SIZE (32 * 1024)

struct vbo_save_vertex_store {
    struct gl_buffer_object *bufferobj;
    fi_type *buffer_map;
    GLuint used;           /**< Number of 4-byte words used in buffer */
};

/* Storage to be shared among several vertex_lists.
 */
struct vbo_save_primitive_store {
    struct _mesa_prim *prims;
    GLuint used;
    GLuint size;
    GLuint refcount;
};


void vbo_save_init(struct gl_context *ctx);
void vbo_save_destroy(struct gl_context *ctx);

/* save_loopback.c:
 */
void _vbo_loopback_vertex_list(struct gl_context *ctx,
                               const struct vbo_save_vertex_list* node);

/* Callbacks:
 */
void
vbo_save_playback_vertex_list(struct gl_context *ctx, void *data);

void
vbo_save_api_init(struct vbo_save_context *save);

fi_type *
vbo_save_map_vertex_store(struct gl_context *ctx,
                          struct vbo_save_vertex_store *vertex_store);

void
vbo_save_unmap_vertex_store(struct gl_context *ctx,
                            struct vbo_save_vertex_store *vertex_store);

#endif /* VBO_SAVE_H */
