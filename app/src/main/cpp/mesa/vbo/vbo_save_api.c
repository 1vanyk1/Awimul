#include "../main/glheader.h"
#include "../main/arrayobj.h"
#include "../main/bufferobj.h"
#include "../main/context.h"
#include "../main/dlist.h"
#include "../main/enums.h"
#include "../main/eval.h"
#include "../main/macros.h"
#include "../main/draw_validate.h"
#include "../main/api_arrayelt.h"
#include "../main/vtxfmt.h"
#include "../main/dispatch.h"
#include "../main/state.h"
#include "../main/varray.h"
#include "../util/bitscan.h"
#include "../util/u_memory.h"

#include "../include/pipe/p_state.h"

#include "vbo_noop.h"
#include "vbo_private.h"


#ifdef ERROR
#undef ERROR
#endif

/**
 * Display list flag only used by this VBO code.
 */
#define DLIST_DANGLING_REFS     0x1


/* An interesting VBO number/name to help with debugging */
#define VBO_BUF_ID  12345

static void GLAPIENTRY
_save_Materialfv(GLenum face, GLenum pname, const GLfloat *params);

static void GLAPIENTRY
_save_EvalCoord1f(GLfloat u);

static void GLAPIENTRY
_save_EvalCoord2f(GLfloat u, GLfloat v);

/*
 * NOTE: Old 'parity' issue is gone, but copying can still be
 * wrong-footed on replay.
 */
static GLuint
copy_vertices(struct gl_context *ctx,
              const struct vbo_save_vertex_list *node,
              const fi_type * src_buffer)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    struct _mesa_prim *prim = &node->prims[node->prim_count - 1];
    GLuint sz = save->vertex_size;
    const fi_type *src = src_buffer + prim->start * sz;
    fi_type *dst = save->copied.buffer;

    if (prim->end)
        return 0;

    return vbo_copy_vertices(ctx, prim->mode, prim->start, &prim->count,
                             prim->begin, sz, true, dst, src);
}


static struct vbo_save_vertex_store *
alloc_vertex_store(struct gl_context *ctx, int vertex_count)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    struct vbo_save_vertex_store *vertex_store =
            CALLOC_STRUCT(vbo_save_vertex_store);

    int size = MAX2(vertex_count * save->vertex_size, VBO_SAVE_BUFFER_SIZE);

    /* obj->Name needs to be non-zero, but won't ever be examined more
     * closely than that.  In particular these buffers won't be entered
     * into the hash and can never be confused with ones visible to the
     * user.  Perhaps there could be a special number for internal
     * buffers:
     */
    vertex_store->bufferobj = ctx->Driver.NewBufferObject(ctx, VBO_BUF_ID);
    if (vertex_store->bufferobj) {
        save->out_of_memory =
                !ctx->Driver.BufferData(ctx,
                                        GL_ARRAY_BUFFER_ARB,
                                        size * sizeof(GLfloat),
                                        NULL, GL_STATIC_DRAW_ARB,
                                        GL_MAP_WRITE_BIT |
                                        GL_DYNAMIC_STORAGE_BIT,
                                        vertex_store->bufferobj);
    }
    else {
        save->out_of_memory = GL_TRUE;
    }

    if (save->out_of_memory) {
        _mesa_error(ctx, GL_OUT_OF_MEMORY, "internal VBO allocation");
        _mesa_install_save_vtxfmt(ctx, &save->vtxfmt_noop);
    }

    vertex_store->buffer_map = NULL;
    vertex_store->used = 0;

    return vertex_store;
}


static void
free_vertex_store(struct gl_context *ctx,
                  struct vbo_save_vertex_store *vertex_store)
{
    assert(!vertex_store->buffer_map);

    if (vertex_store->bufferobj) {
        _mesa_reference_buffer_object(ctx, &vertex_store->bufferobj, NULL);
    }

    free(vertex_store);
}


fi_type *
vbo_save_map_vertex_store(struct gl_context *ctx,
                          struct vbo_save_vertex_store *vertex_store)
{
    const GLbitfield access = (GL_MAP_WRITE_BIT |
                               GL_MAP_INVALIDATE_RANGE_BIT |
                               GL_MAP_UNSYNCHRONIZED_BIT |
                               GL_MAP_FLUSH_EXPLICIT_BIT |
                               MESA_MAP_ONCE);

    assert(vertex_store->bufferobj);
    assert(!vertex_store->buffer_map);  /* the buffer should not be mapped */

    if (vertex_store->bufferobj->Size > 0) {
        /* Map the remaining free space in the VBO */
        GLintptr offset = vertex_store->used * sizeof(GLfloat);
        GLsizeiptr size = vertex_store->bufferobj->Size - offset;
        fi_type *range = (fi_type *)
                ctx->Driver.MapBufferRange(ctx, offset, size, access,
                                           vertex_store->bufferobj,
                                           MAP_INTERNAL);
        if (range) {
            /* compute address of start of whole buffer (needed elsewhere) */
            vertex_store->buffer_map = range - vertex_store->used;
            assert(vertex_store->buffer_map);
            return range;
        }
        else {
            vertex_store->buffer_map = NULL;
            return NULL;
        }
    }
    else {
        /* probably ran out of memory for buffers */
        return NULL;
    }
}


void
vbo_save_unmap_vertex_store(struct gl_context *ctx,
                            struct vbo_save_vertex_store *vertex_store)
{
    if (vertex_store->bufferobj->Size > 0) {
        GLintptr offset = 0;
        GLsizeiptr length = vertex_store->used * sizeof(GLfloat)
                            - vertex_store->bufferobj->Mappings[MAP_INTERNAL].Offset;

        /* Explicitly flush the region we wrote to */
        ctx->Driver.FlushMappedBufferRange(ctx, offset, length,
                                           vertex_store->bufferobj,
                                           MAP_INTERNAL);

        ctx->Driver.UnmapBuffer(ctx, vertex_store->bufferobj, MAP_INTERNAL);
    }
    vertex_store->buffer_map = NULL;
}


static struct vbo_save_primitive_store *
alloc_prim_store(int prim_count)
{
    struct vbo_save_primitive_store *store =
            CALLOC_STRUCT(vbo_save_primitive_store);
    store->size = MAX2(prim_count, VBO_SAVE_PRIM_SIZE);
    store->prims = calloc(store->size, sizeof(struct _mesa_prim));
    store->used = 0;
    store->refcount = 1;
    return store;
}


static void
reset_counters(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;

    save->prims = save->prim_store->prims + save->prim_store->used;
    save->buffer_map = save->vertex_store->buffer_map + save->vertex_store->used;

    assert(save->buffer_map == save->buffer_ptr);

    if (save->vertex_size)
        save->max_vert = (save->vertex_store->bufferobj->Size / sizeof(float) - save->vertex_store->used) /
                         save->vertex_size;
    else
        save->max_vert = 0;

    save->vert_count = 0;
    save->prim_count = 0;
    save->prim_max = save->prim_store->size - save->prim_store->used;
    save->dangling_attr_ref = GL_FALSE;
}

/**
 * For a list of prims, try merging prims that can just be extensions of the
 * previous prim.
 */
static void
merge_prims(struct gl_context *ctx, struct _mesa_prim *prim_list,
            GLuint *prim_count)
{
    GLuint i;
    struct _mesa_prim *prev_prim = prim_list;

    for (i = 1; i < *prim_count; i++) {
        struct _mesa_prim *this_prim = prim_list + i;

        vbo_try_prim_conversion(&this_prim->mode, &this_prim->count);

        if (vbo_merge_draws(ctx, true,
                            prev_prim->mode, this_prim->mode,
                            prev_prim->start, this_prim->start,
                            &prev_prim->count, this_prim->count,
                            prev_prim->basevertex, this_prim->basevertex,
                            &prev_prim->end,
                            this_prim->begin, this_prim->end)) {
            /* We've found a prim that just extend the previous one.  Tack it
             * onto the previous one, and let this primitive struct get dropped.
             */
            continue;
        }

        /* If any previous primitives have been dropped, then we need to copy
         * this later one into the next available slot.
         */
        prev_prim++;
        if (prev_prim != this_prim)
            *prev_prim = *this_prim;
    }

    *prim_count = prev_prim - prim_list + 1;
}


/**
 * Convert GL_LINE_LOOP primitive into GL_LINE_STRIP so that drivers
 * don't have to worry about handling the _mesa_prim::begin/end flags.
 * See https://bugs.freedesktop.org/show_bug.cgi?id=81174
 */
static void
convert_line_loop_to_strip(struct vbo_save_context *save,
                           struct vbo_save_vertex_list *node)
{
    struct _mesa_prim *prim = &node->prims[node->prim_count - 1];

    assert(prim->mode == GL_LINE_LOOP);

    if (prim->end) {
        /* Copy the 0th vertex to end of the buffer and extend the
         * vertex count by one to finish the line loop.
         */
        const GLuint sz = save->vertex_size;
        /* 0th vertex: */
        const fi_type *src = save->buffer_map + prim->start * sz;
        /* end of buffer: */
        fi_type *dst = save->buffer_map + (prim->start + prim->count) * sz;

        memcpy(dst, src, sz * sizeof(float));

        prim->count++;
        node->vertex_count++;
        save->vert_count++;
        save->buffer_ptr += sz;
        save->vertex_store->used += sz;
    }

    if (!prim->begin) {
        /* Drawing the second or later section of a long line loop.
         * Skip the 0th vertex.
         */
        prim->start++;
        prim->count--;
    }

    prim->mode = GL_LINE_STRIP;
}


/* Compare the present vao if it has the same setup. */
static bool
compare_vao(gl_vertex_processing_mode mode,
            const struct gl_vertex_array_object *vao,
            const struct gl_buffer_object *bo, GLintptr buffer_offset,
            GLuint stride, GLbitfield64 vao_enabled,
            const GLubyte size[VBO_ATTRIB_MAX],
            const GLenum16 type[VBO_ATTRIB_MAX],
            const GLuint offset[VBO_ATTRIB_MAX])
{
    if (!vao)
        return false;

    /* If the enabled arrays are not the same we are not equal. */
    if (vao_enabled != vao->Enabled)
        return false;

    /* Check the buffer binding at 0 */
    if (vao->BufferBinding[0].BufferObj != bo)
        return false;
    /* BufferBinding[0].Offset != buffer_offset is checked per attribute */
    if (vao->BufferBinding[0].Stride != stride)
        return false;
    assert(vao->BufferBinding[0].InstanceDivisor == 0);

    /* Retrieve the mapping from VBO_ATTRIB to VERT_ATTRIB space */
    const GLubyte *const vao_to_vbo_map = _vbo_attribute_alias_map[mode];

    /* Now check the enabled arrays */
    GLbitfield mask = vao_enabled;
    while (mask) {
        const int attr = u_bit_scan(&mask);
        const unsigned char vbo_attr = vao_to_vbo_map[attr];
        const GLenum16 tp = type[vbo_attr];
        const GLintptr off = offset[vbo_attr] + buffer_offset;
        const struct gl_array_attributes *attrib = &vao->VertexAttrib[attr];
        if (attrib->RelativeOffset + vao->BufferBinding[0].Offset != off)
            return false;
        if (attrib->Format.Type != tp)
            return false;
        if (attrib->Format.Size != size[vbo_attr])
            return false;
        assert(attrib->Format.Format == GL_RGBA);
        assert(attrib->Format.Normalized == GL_FALSE);
        assert(attrib->Format.Integer == vbo_attrtype_to_integer_flag(tp));
        assert(attrib->Format.Doubles == vbo_attrtype_to_double_flag(tp));
        assert(attrib->BufferBindingIndex == 0);
    }

    return true;
}


/* Create or reuse the vao for the vertex processing mode. */
static void
update_vao(struct gl_context *ctx,
           gl_vertex_processing_mode mode,
           struct gl_vertex_array_object **vao,
           struct gl_buffer_object *bo, GLintptr buffer_offset,
           GLuint stride, GLbitfield64 vbo_enabled,
           const GLubyte size[VBO_ATTRIB_MAX],
           const GLenum16 type[VBO_ATTRIB_MAX],
           const GLuint offset[VBO_ATTRIB_MAX])
{
    /* Compute the bitmasks of vao_enabled arrays */
    GLbitfield vao_enabled = _vbo_get_vao_enabled_from_vbo(mode, vbo_enabled);

    /*
     * Check if we can possibly reuse the exisiting one.
     * In the long term we should reset them when something changes.
     */
    if (compare_vao(mode, *vao, bo, buffer_offset, stride,
                    vao_enabled, size, type, offset))
        return;

    /* The initial refcount is 1 */
    _mesa_reference_vao(ctx, vao, NULL);
    *vao = _mesa_new_vao(ctx, ~((GLuint)0));

    /*
     * assert(stride <= ctx->Const.MaxVertexAttribStride);
     * MaxVertexAttribStride is not set for drivers that does not
     * expose GL 44 or GLES 31.
     */

    /* Bind the buffer object at binding point 0 */
    _mesa_bind_vertex_buffer(ctx, *vao, 0, bo, buffer_offset, stride, false,
                             false);

    /* Retrieve the mapping from VBO_ATTRIB to VERT_ATTRIB space
     * Note that the position/generic0 aliasing is done in the VAO.
     */
    const GLubyte *const vao_to_vbo_map = _vbo_attribute_alias_map[mode];
    /* Now set the enable arrays */
    GLbitfield mask = vao_enabled;
    while (mask) {
        const int vao_attr = u_bit_scan(&mask);
        const GLubyte vbo_attr = vao_to_vbo_map[vao_attr];
        assert(offset[vbo_attr] <= ctx->Const.MaxVertexAttribRelativeOffset);

        _vbo_set_attrib_format(ctx, *vao, vao_attr, buffer_offset,
                               size[vbo_attr], type[vbo_attr], offset[vbo_attr]);
        _mesa_vertex_attrib_binding(ctx, *vao, vao_attr, 0);
    }
    _mesa_enable_vertex_array_attribs(ctx, *vao, vao_enabled);
    assert(vao_enabled == (*vao)->Enabled);
    assert((vao_enabled & ~(*vao)->VertexAttribBufferMask) == 0);

    /* Finalize and freeze the VAO */
    _mesa_set_vao_immutable(ctx, *vao);
}


static void
realloc_storage(struct gl_context *ctx, int prim_count, int vertex_count)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    if (vertex_count >= 0) {
        /* Unmap old store:
         */
        vbo_save_unmap_vertex_store(ctx, save->vertex_store);

        /* Release old reference:
         */
        free_vertex_store(ctx, save->vertex_store);
        save->vertex_store = NULL;
        /* When we have a new vbo, we will for sure need a new vao */
        for (gl_vertex_processing_mode vpm = 0; vpm < VP_MODE_MAX; ++vpm)
            _mesa_reference_vao(ctx, &save->VAO[vpm], NULL);

        /* Allocate and map new store:
         */
        save->vertex_store = alloc_vertex_store(ctx, vertex_count);
        save->buffer_ptr = vbo_save_map_vertex_store(ctx, save->vertex_store);
        save->out_of_memory = save->buffer_ptr == NULL;
    }

    if (prim_count >= 0) {
        if (--save->prim_store->refcount == 0) {
            free(save->prim_store->prims);
            free(save->prim_store);
        }
        save->prim_store = alloc_prim_store(prim_count);
    }
}


/**
 * Insert the active immediate struct onto the display list currently
 * being built.
 */
static void
compile_vertex_list(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    struct vbo_save_vertex_list *node;

    /* Allocate space for this structure in the display list currently
     * being compiled.
     */
    node = (struct vbo_save_vertex_list *)
            _mesa_dlist_alloc_vertex_list(ctx);

    if (!node)
        return;

    memset(node, 0, sizeof(struct vbo_save_vertex_list));

    /* Make sure the pointer is aligned to the size of a pointer */
    assert((GLintptr) node % sizeof(void *) == 0);

    /* Duplicate our template, increment refcounts to the storage structs:
     */
    GLintptr old_offset = 0;
    if (save->VAO[0]) {
        old_offset = save->VAO[0]->BufferBinding[0].Offset
                     + save->VAO[0]->VertexAttrib[VERT_ATTRIB_POS].RelativeOffset;
    }
    const GLsizei stride = save->vertex_size*sizeof(GLfloat);
    GLintptr buffer_offset =
            (save->buffer_map - save->vertex_store->buffer_map) * sizeof(GLfloat);
    assert(old_offset <= buffer_offset);
    const GLintptr offset_diff = buffer_offset - old_offset;
    GLuint start_offset = 0;
    if (offset_diff > 0 && stride > 0 && offset_diff % stride == 0) {
        /* The vertex size is an exact multiple of the buffer offset.
         * This means that we can use zero-based vertex attribute pointers
         * and specify the start of the primitive with the _mesa_prim::start
         * field.  This results in issuing several draw calls with identical
         * vertex attribute information.  This can result in fewer state
         * changes in drivers.  In particular, the Gallium CSO module will
         * filter out redundant vertex buffer changes.
         */
        /* We cannot immediately update the primitives as some methods below
         * still need the uncorrected start vertices
         */
        start_offset = offset_diff/stride;
        assert(old_offset == buffer_offset - offset_diff);
        buffer_offset = old_offset;
    }
    GLuint offsets[VBO_ATTRIB_MAX];
    for (unsigned i = 0, offset = 0; i < VBO_ATTRIB_MAX; ++i) {
        offsets[i] = offset;
        offset += save->attrsz[i] * sizeof(GLfloat);
    }
    node->vertex_count = save->vert_count;
    node->wrap_count = save->copied.nr;
    node->prims = save->prims;
    node->merged.ib.obj = NULL;
    node->prim_count = save->prim_count;
    node->prim_store = save->prim_store;

    /* Create a pair of VAOs for the possible VERTEX_PROCESSING_MODEs
     * Note that this may reuse the previous one of possible.
     */
    for (gl_vertex_processing_mode vpm = VP_MODE_FF; vpm < VP_MODE_MAX; ++vpm) {
        /* create or reuse the vao */
        update_vao(ctx, vpm, &save->VAO[vpm],
                   save->vertex_store->bufferobj, buffer_offset, stride,
                   save->enabled, save->attrsz, save->attrtype, offsets);
        /* Reference the vao in the dlist */
        node->VAO[vpm] = NULL;
        _mesa_reference_vao(ctx, &node->VAO[vpm], save->VAO[vpm]);
    }

    node->prim_store->refcount++;

    if (save->no_current_update) {
        node->current_data = NULL;
    }
    else {
        GLuint current_size = save->vertex_size - save->attrsz[0];
        node->current_data = NULL;

        if (current_size) {
            node->current_data = malloc(current_size * sizeof(GLfloat));
            if (node->current_data) {
                const char *buffer = (const char *)save->buffer_map;
                unsigned attr_offset = save->attrsz[0] * sizeof(GLfloat);
                unsigned vertex_offset = 0;

                if (node->vertex_count)
                    vertex_offset = (node->vertex_count - 1) * stride;

                memcpy(node->current_data, buffer + vertex_offset + attr_offset,
                       current_size * sizeof(GLfloat));
            } else {
                _mesa_error(ctx, GL_OUT_OF_MEMORY, "Current value allocation");
            }
        }
    }

    assert(save->attrsz[VBO_ATTRIB_POS] != 0 || node->vertex_count == 0);

    if (save->dangling_attr_ref)
        ctx->ListState.CurrentList->Flags |= DLIST_DANGLING_REFS;

    save->vertex_store->used += save->vertex_size * node->vertex_count;
    save->prim_store->used += node->prim_count;

    /* Copy duplicated vertices
     */
    save->copied.nr = copy_vertices(ctx, node, save->buffer_map);

    if (node->prims[node->prim_count - 1].mode == GL_LINE_LOOP) {
        convert_line_loop_to_strip(save, node);
    }

    merge_prims(ctx, node->prims, &node->prim_count);

    /* Correct the primitive starts, we can only do this here as copy_vertices
     * and convert_line_loop_to_strip above consume the uncorrected starts.
     * On the other hand the _vbo_loopback_vertex_list call below needs the
     * primitves to be corrected already.
     */
    for (unsigned i = 0; i < node->prim_count; i++) {
        node->prims[i].start += start_offset;
    }

    /* Create an index buffer. */
    node->min_index = node->max_index = 0;
    if (save->vert_count && node->prim_count) {
        /* We won't modify node->prims, so use a const alias to avoid unintended
         * writes to it. */
        const struct _mesa_prim *original_prims = node->prims;

        int end = original_prims[node->prim_count - 1].start +
                  original_prims[node->prim_count - 1].count;
        int total_vert_count = end - original_prims[0].start;

        node->min_index = node->prims[0].start;
        node->max_index = end - 1;

        /* Estimate for the worst case: all prims are line strips (the +1 is because
         * wrap_buffers may call use but the last primitive may not be complete) */
        int max_indices_count = MAX2(total_vert_count * 2 - (node->prim_count * 2) + 1,
                                     total_vert_count);

        int indices_offset = 0;
        int available = save->previous_ib ? (save->previous_ib->Size / 4 - save->ib_first_free_index) : 0;
        if (available >= max_indices_count) {
            indices_offset = save->ib_first_free_index;
        }
        int size = max_indices_count * sizeof(uint32_t);
        uint32_t* indices = (uint32_t*) malloc(size);
        struct _mesa_prim *merged_prims = NULL;

        int idx = 0;

        int last_valid_prim = -1;
        /* Construct indices array. */
        for (unsigned i = 0; i < node->prim_count; i++) {
            assert(original_prims[i].basevertex == 0);
            GLubyte mode = original_prims[i].mode;

            int vertex_count = original_prims[i].count;
            if (!vertex_count) {
                continue;
            }

            /* Line strips may get converted to lines */
            if (mode == GL_LINE_STRIP)
                mode = GL_LINES;

            /* If 2 consecutive prims use the same mode => merge them. */
            bool merge_prims = last_valid_prim >= 0 &&
                               mode == merged_prims[last_valid_prim].mode &&
                               mode != GL_LINE_LOOP && mode != GL_TRIANGLE_FAN &&
                               mode != GL_QUAD_STRIP && mode != GL_POLYGON &&
                               mode != GL_PATCHES;

            /* To be able to merge consecutive triangle strips we need to insert
             * a degenerate triangle.
             */
            if (merge_prims &&
                mode == GL_TRIANGLE_STRIP) {
                /* Insert a degenerate triangle */
                assert(merged_prims[last_valid_prim].mode == GL_TRIANGLE_STRIP);
                unsigned tri_count = merged_prims[last_valid_prim].count - 2;

                indices[idx] = indices[idx - 1];
                indices[idx + 1] = original_prims[i].start;
                idx += 2;
                merged_prims[last_valid_prim].count += 2;

                if (tri_count % 2) {
                    /* Add another index to preserve winding order */
                    indices[idx++] = original_prims[i].start;
                    merged_prims[last_valid_prim].count++;
                }
            }

            int start = idx;

            /* Convert line strips to lines if it'll allow if the previous
             * prim mode is GL_LINES (so merge_prims is true) or if the next
             * primitive mode is GL_LINES or GL_LINE_LOOP.
             */
            if (original_prims[i].mode == GL_LINE_STRIP &&
                (merge_prims ||
                 (i < node->prim_count - 1 &&
                  (original_prims[i + 1].mode == GL_LINE_STRIP ||
                   original_prims[i + 1].mode == GL_LINES)))) {
                for (unsigned j = 0; j < vertex_count; j++) {
                    indices[idx++] = original_prims[i].start + j;
                    /* Repeat all but the first/last indices. */
                    if (j && j != vertex_count - 1) {
                        indices[idx++] = original_prims[i].start + j;
                    }
                }
            } else {
                /* We didn't convert to LINES, so restore the original mode */
                mode = original_prims[i].mode;

                for (unsigned j = 0; j < vertex_count; j++) {
                    indices[idx++] = original_prims[i].start + j;
                }
            }

            if (merge_prims) {
                /* Update vertex count. */
                merged_prims[last_valid_prim].count += idx - start;
            } else {
                /* Keep this primitive */
                last_valid_prim += 1;
                assert(last_valid_prim <= i);
                merged_prims = realloc(merged_prims, (1 + last_valid_prim) * sizeof(struct _mesa_prim));
                merged_prims[last_valid_prim] = original_prims[i];
                merged_prims[last_valid_prim].start = indices_offset + start;
                merged_prims[last_valid_prim].count = idx - start;
            }
            merged_prims[last_valid_prim].mode = mode;
        }

        assert(idx > 0 && idx <= max_indices_count);

        unsigned merged_prim_count = last_valid_prim + 1;
        node->merged.ib.ptr = NULL;
        node->merged.ib.count = idx;
        node->merged.ib.index_size_shift = (GL_UNSIGNED_INT - GL_UNSIGNED_BYTE) >> 1;

        if (!indices_offset) {
            /* Allocate a new index buffer */
            _mesa_reference_buffer_object(ctx, &save->previous_ib, NULL);
            save->previous_ib = ctx->Driver.NewBufferObject(ctx, VBO_BUF_ID + 1);
            bool success = ctx->Driver.BufferData(ctx,
                                                  GL_ELEMENT_ARRAY_BUFFER_ARB,
                                                  MAX2(VBO_SAVE_INDEX_SIZE, idx) * sizeof(uint32_t),
                                                  NULL,
                                                  GL_STATIC_DRAW_ARB, GL_MAP_WRITE_BIT,
                                                  save->previous_ib);
            if (!success) {
                _mesa_reference_buffer_object(ctx, &save->previous_ib, NULL);
                _mesa_error(ctx, GL_OUT_OF_MEMORY, "IB allocation");
            }
        }

        _mesa_reference_buffer_object(ctx, &node->merged.ib.obj, save->previous_ib);

        if (node->merged.ib.obj) {
            ctx->Driver.BufferSubData(ctx,
                                      indices_offset * sizeof(uint32_t),
                                      idx * sizeof(uint32_t),
                                      indices,
                                      node->merged.ib.obj);
            save->ib_first_free_index = indices_offset + idx;
        } else {
            node->vertex_count = 0;
            node->prim_count = 0;
        }

        /* Prepare for DrawGallium */
        memset(&node->merged.info, 0, sizeof(struct pipe_draw_info));
        /* The other info fields will be updated in vbo_save_playback_vertex_list */
        node->merged.info.index_size = 4;
        node->merged.info.instance_count = 1;
        node->merged.info.index.gl_bo = node->merged.ib.obj;
        node->merged.start_count = malloc(merged_prim_count * sizeof(struct pipe_draw_start_count));
        if (merged_prim_count == 1) {
            node->merged.info.mode = merged_prims[0].mode;
            node->merged.mode = NULL;
        } else {
            node->merged.mode = malloc(merged_prim_count * sizeof(unsigned char));
        }

        for (unsigned i = 0; i < merged_prim_count; i++) {
            node->merged.start_count[i].start = merged_prims[i].start;
            node->merged.start_count[i].count = merged_prims[i].count;
            if (merged_prim_count > 1)
                node->merged.mode[i] = merged_prims[i].mode;
        }
        node->merged.num_draws = merged_prim_count;
        if (node->merged.num_draws > 1) {
            bool same_mode = true;
            for (unsigned i = 1; i < node->merged.num_draws && same_mode; i++) {
                same_mode = node->merged.mode[i] == node->merged.mode[0];
            }
            if (same_mode) {
                /* All primitives use the same mode, so we can simplify a bit */
                node->merged.info.mode = node->merged.mode[0];
                free(node->merged.mode);
                node->merged.mode = NULL;
            }
        }

        free(indices);
        free(merged_prims);
    }

    /* Deal with GL_COMPILE_AND_EXECUTE:
     */
    if (ctx->ExecuteFlag) {
        struct _glapi_table *dispatch = GET_DISPATCH();

        _glapi_set_dispatch(ctx->Exec);

        /* Note that the range of referenced vertices must be mapped already */
        _vbo_loopback_vertex_list(ctx, node);

        _glapi_set_dispatch(dispatch);
    }

    /* Decide whether the storage structs are full, or can be used for
     * the next vertex lists as well.
     */
    if (save->vertex_store->used >
        save->vertex_store->bufferobj->Size / sizeof(float) - 16 * (save->vertex_size + 4)) {
        realloc_storage(ctx, -1, 0);
    }
    else {
        /* update buffer_ptr for next vertex */
        save->buffer_ptr = save->vertex_store->buffer_map
                           + save->vertex_store->used;
    }

    if (save->prim_store->used > save->prim_store->size - 6) {
        realloc_storage(ctx, 0, -1);
    }

    /* Reset our structures for the next run of vertices:
     */
    reset_counters(ctx);
}


/**
 * This is called when we fill a vertex buffer before we hit a glEnd().
 * We
 * TODO -- If no new vertices have been stored, don't bother saving it.
 */
static void
wrap_buffers(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    GLint i = save->prim_count - 1;
    GLenum mode;

    assert(i < (GLint) save->prim_max);
    assert(i >= 0);

    /* Close off in-progress primitive.
     */
    save->prims[i].count = (save->vert_count - save->prims[i].start);
    mode = save->prims[i].mode;

    /* store the copied vertices, and allocate a new list.
     */
    compile_vertex_list(ctx);

    /* Restart interrupted primitive
     */
    save->prims[0].mode = mode;
    save->prims[0].begin = 0;
    save->prims[0].end = 0;
    save->prims[0].start = 0;
    save->prims[0].count = 0;
    save->prim_count = 1;
}


/**
 * Called only when buffers are wrapped as the result of filling the
 * vertex_store struct.
 */
static void
wrap_filled_vertex(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    unsigned numComponents;

    /* Emit a glEnd to close off the last vertex list.
     */
    wrap_buffers(ctx);

    /* Copy stored stored vertices to start of new list.
     */
    assert(save->max_vert - save->vert_count > save->copied.nr);

    numComponents = save->copied.nr * save->vertex_size;
    memcpy(save->buffer_ptr,
           save->copied.buffer,
           numComponents * sizeof(fi_type));
    save->buffer_ptr += numComponents;
    save->vert_count += save->copied.nr;
}


static void
copy_to_current(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    GLbitfield64 enabled = save->enabled & (~BITFIELD64_BIT(VBO_ATTRIB_POS));

    while (enabled) {
        const int i = u_bit_scan64(&enabled);
        assert(save->attrsz[i]);

        if (save->attrtype[i] == GL_DOUBLE ||
            save->attrtype[i] == GL_UNSIGNED_INT64_ARB)
            memcpy(save->current[i], save->attrptr[i], save->attrsz[i] * sizeof(GLfloat));
        else
            COPY_CLEAN_4V_TYPE_AS_UNION(save->current[i], save->attrsz[i],
                                        save->attrptr[i], save->attrtype[i]);
    }
}


static void
copy_from_current(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    GLbitfield64 enabled = save->enabled & (~BITFIELD64_BIT(VBO_ATTRIB_POS));

    while (enabled) {
        const int i = u_bit_scan64(&enabled);

        switch (save->attrsz[i]) {
            case 4:
                save->attrptr[i][3] = save->current[i][3];
                FALLTHROUGH;
            case 3:
                save->attrptr[i][2] = save->current[i][2];
                FALLTHROUGH;
            case 2:
                save->attrptr[i][1] = save->current[i][1];
                FALLTHROUGH;
            case 1:
                save->attrptr[i][0] = save->current[i][0];
                break;
            case 0:
                unreachable("Unexpected vertex attribute size");
        }
    }
}


/**
 * Called when we increase the size of a vertex attribute.  For example,
 * if we've seen one or more glTexCoord2f() calls and now we get a
 * glTexCoord3f() call.
 * Flush existing data, set new attrib size, replay copied vertices.
 */
static void
upgrade_vertex(struct gl_context *ctx, GLuint attr, GLuint newsz)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    GLuint oldsz;
    GLuint i;
    fi_type *tmp;

    /* Store the current run of vertices, and emit a GL_END.  Emit a
     * BEGIN in the new buffer.
     */
    if (save->vert_count)
        wrap_buffers(ctx);
    else
        assert(save->copied.nr == 0);

    /* Do a COPY_TO_CURRENT to ensure back-copying works for the case
     * when the attribute already exists in the vertex and is having
     * its size increased.
     */
    copy_to_current(ctx);

    /* Fix up sizes:
     */
    oldsz = save->attrsz[attr];
    save->attrsz[attr] = newsz;
    save->enabled |= BITFIELD64_BIT(attr);

    save->vertex_size += newsz - oldsz;
    save->max_vert = ((save->vertex_store->bufferobj->Size / sizeof(float) -
                       save->vertex_store->used) /
                      save->vertex_size);
    save->vert_count = 0;

    /* Recalculate all the attrptr[] values:
     */
    tmp = save->vertex;
    for (i = 0; i < VBO_ATTRIB_MAX; i++) {
        if (save->attrsz[i]) {
            save->attrptr[i] = tmp;
            tmp += save->attrsz[i];
        }
        else {
            save->attrptr[i] = NULL;       /* will not be dereferenced. */
        }
    }

    /* Copy from current to repopulate the vertex with correct values.
     */
    copy_from_current(ctx);

    /* Replay stored vertices to translate them to new format here.
     *
     * If there are copied vertices and the new (upgraded) attribute
     * has not been defined before, this list is somewhat degenerate,
     * and will need fixup at runtime.
     */
    if (save->copied.nr) {
        const fi_type *data = save->copied.buffer;
        fi_type *dest = save->buffer_map;

        /* Need to note this and fix up at runtime (or loopback):
         */
        if (attr != VBO_ATTRIB_POS && save->currentsz[attr][0] == 0) {
            assert(oldsz == 0);
            save->dangling_attr_ref = GL_TRUE;
        }

        for (i = 0; i < save->copied.nr; i++) {
            GLbitfield64 enabled = save->enabled;
            while (enabled) {
                const int j = u_bit_scan64(&enabled);
                assert(save->attrsz[j]);
                if (j == attr) {
                    if (oldsz) {
                        COPY_CLEAN_4V_TYPE_AS_UNION(dest, oldsz, data,
                                                    save->attrtype[j]);
                        data += oldsz;
                        dest += newsz;
                    }
                    else {
                        COPY_SZ_4V(dest, newsz, save->current[attr]);
                        dest += newsz;
                    }
                }
                else {
                    GLint sz = save->attrsz[j];
                    COPY_SZ_4V(dest, sz, data);
                    data += sz;
                    dest += sz;
                }
            }
        }

        save->buffer_ptr = dest;
        save->vert_count += save->copied.nr;
    }
}


/**
 * This is called when the size of a vertex attribute changes.
 * For example, after seeing one or more glTexCoord2f() calls we
 * get a glTexCoord4f() or glTexCoord1f() call.
 */
static void
fixup_vertex(struct gl_context *ctx, GLuint attr,
             GLuint sz, GLenum newType)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;

    if (sz > save->attrsz[attr] ||
        newType != save->attrtype[attr]) {
        /* New size is larger.  Need to flush existing vertices and get
         * an enlarged vertex format.
         */
        upgrade_vertex(ctx, attr, sz);
    }
    else if (sz < save->active_sz[attr]) {
        GLuint i;
        const fi_type *id = vbo_get_default_vals_as_union(save->attrtype[attr]);

        /* New size is equal or smaller - just need to fill in some
         * zeros.
         */
        for (i = sz; i <= save->attrsz[attr]; i++)
            save->attrptr[attr][i - 1] = id[i - 1];
    }

    save->active_sz[attr] = sz;
}


/**
 * Reset the current size of all vertex attributes to the default
 * value of 0.  This signals that we haven't yet seen any per-vertex
 * commands such as glNormal3f() or glTexCoord2f().
 */
static void
reset_vertex(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;

    while (save->enabled) {
        const int i = u_bit_scan64(&save->enabled);
        assert(save->attrsz[i]);
        save->attrsz[i] = 0;
        save->active_sz[i] = 0;
    }

    save->vertex_size = 0;
}


/**
 * If index=0, does glVertexAttrib*() alias glVertex() to emit a vertex?
 * It depends on a few things, including whether we're inside or outside
 * of glBegin/glEnd.
 */
static inline bool
is_vertex_position(const struct gl_context *ctx, GLuint index)
{
    return (index == 0 &&
            _mesa_attr_zero_aliases_vertex(ctx) &&
            _mesa_inside_dlist_begin_end(ctx));
}



#define ERROR(err)   _mesa_compile_error(ctx, err, __func__);


/* Only one size for each attribute may be active at once.  Eg. if
 * Color3f is installed/active, then Color4f may not be, even if the
 * vertex actually contains 4 color coordinates.  This is because the
 * 3f version won't otherwise set color[3] to 1.0 -- this is the job
 * of the chooser function when switching between Color4f and Color3f.
 */
#define ATTR_UNION(A, N, T, C, V0, V1, V2, V3)			\
do {								\
   struct vbo_save_context *save = &vbo_context(ctx)->save;	\
   int sz = (sizeof(C) / sizeof(GLfloat));			\
								\
   if (save->active_sz[A] != N)					\
      fixup_vertex(ctx, A, N * sz, T);				\
								\
   {								\
      C *dest = (C *)save->attrptr[A];                          \
      if (N>0) dest[0] = V0;					\
      if (N>1) dest[1] = V1;					\
      if (N>2) dest[2] = V2;					\
      if (N>3) dest[3] = V3;					\
      save->attrtype[A] = T;					\
   }								\
								\
   if ((A) == 0) {						\
      GLuint i;							\
								\
      for (i = 0; i < save->vertex_size; i++)			\
	 save->buffer_ptr[i] = save->vertex[i];			\
								\
      save->buffer_ptr += save->vertex_size;			\
								\
      if (++save->vert_count >= save->max_vert)			\
	 wrap_filled_vertex(ctx);				\
   }								\
} while (0)

#define TAG(x) _save_##x

#include "vbo_attrib_tmp.h"



#define MAT( ATTR, N, face, params )			\
do {							\
   if (face != GL_BACK)					\
      MAT_ATTR( ATTR, N, params ); /* front */		\
   if (face != GL_FRONT)				\
      MAT_ATTR( ATTR + 1, N, params ); /* back */	\
} while (0)


/**
 * Save a glMaterial call found between glBegin/End.
 * glMaterial calls outside Begin/End are handled in dlist.c.
 */
static void GLAPIENTRY
_save_Materialfv(GLenum face, GLenum pname, const GLfloat *params)
{
    GET_CURRENT_CONTEXT(ctx);

    if (face != GL_FRONT && face != GL_BACK && face != GL_FRONT_AND_BACK) {
        _mesa_compile_error(ctx, GL_INVALID_ENUM, "glMaterial(face)");
        return;
    }

    switch (pname) {
        case GL_EMISSION:
            MAT(VBO_ATTRIB_MAT_FRONT_EMISSION, 4, face, params);
            break;
        case GL_AMBIENT:
            MAT(VBO_ATTRIB_MAT_FRONT_AMBIENT, 4, face, params);
            break;
        case GL_DIFFUSE:
            MAT(VBO_ATTRIB_MAT_FRONT_DIFFUSE, 4, face, params);
            break;
        case GL_SPECULAR:
            MAT(VBO_ATTRIB_MAT_FRONT_SPECULAR, 4, face, params);
            break;
        case GL_SHININESS:
            if (*params < 0 || *params > ctx->Const.MaxShininess) {
                _mesa_compile_error(ctx, GL_INVALID_VALUE, "glMaterial(shininess)");
            }
            else {
                MAT(VBO_ATTRIB_MAT_FRONT_SHININESS, 1, face, params);
            }
            break;
        case GL_COLOR_INDEXES:
            MAT(VBO_ATTRIB_MAT_FRONT_INDEXES, 3, face, params);
            break;
        case GL_AMBIENT_AND_DIFFUSE:
            MAT(VBO_ATTRIB_MAT_FRONT_AMBIENT, 4, face, params);
            MAT(VBO_ATTRIB_MAT_FRONT_DIFFUSE, 4, face, params);
            break;
        default:
            _mesa_compile_error(ctx, GL_INVALID_ENUM, "glMaterial(pname)");
            return;
    }
}


/* Cope with EvalCoord/CallList called within a begin/end object:
 *     -- Flush current buffer
 *     -- Fallback to opcodes for the rest of the begin/end object.
 */
static void
dlist_fallback(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;

    if (save->vert_count || save->prim_count) {
        if (save->prim_count > 0) {
            /* Close off in-progress primitive. */
            GLint i = save->prim_count - 1;
            save->prims[i].count = save->vert_count - save->prims[i].start;
        }

        /* Need to replay this display list with loopback,
         * unfortunately, otherwise this primitive won't be handled
         * properly:
         */
        save->dangling_attr_ref = GL_TRUE;

        compile_vertex_list(ctx);
    }

    copy_to_current(ctx);
    reset_vertex(ctx);
    reset_counters(ctx);
    if (save->out_of_memory) {
        _mesa_install_save_vtxfmt(ctx, &save->vtxfmt_noop);
    }
    else {
        _mesa_install_save_vtxfmt(ctx, &ctx->ListState.ListVtxfmt);
    }
    ctx->Driver.SaveNeedFlush = GL_FALSE;
}


static void GLAPIENTRY
_save_EvalCoord1f(GLfloat u)
{
    GET_CURRENT_CONTEXT(ctx);
    dlist_fallback(ctx);
    CALL_EvalCoord1f(ctx->Save, (u));
}

static void GLAPIENTRY
_save_EvalCoord1fv(const GLfloat * v)
{
    GET_CURRENT_CONTEXT(ctx);
    dlist_fallback(ctx);
    CALL_EvalCoord1fv(ctx->Save, (v));
}

static void GLAPIENTRY
_save_EvalCoord2f(GLfloat u, GLfloat v)
{
    GET_CURRENT_CONTEXT(ctx);
    dlist_fallback(ctx);
    CALL_EvalCoord2f(ctx->Save, (u, v));
}

static void GLAPIENTRY
_save_EvalCoord2fv(const GLfloat * v)
{
    GET_CURRENT_CONTEXT(ctx);
    dlist_fallback(ctx);
    CALL_EvalCoord2fv(ctx->Save, (v));
}

static void GLAPIENTRY
_save_EvalPoint1(GLint i)
{
    GET_CURRENT_CONTEXT(ctx);
    dlist_fallback(ctx);
    CALL_EvalPoint1(ctx->Save, (i));
}

static void GLAPIENTRY
_save_EvalPoint2(GLint i, GLint j)
{
    GET_CURRENT_CONTEXT(ctx);
    dlist_fallback(ctx);
    CALL_EvalPoint2(ctx->Save, (i, j));
}

static void GLAPIENTRY
_save_CallList(GLuint l)
{
    GET_CURRENT_CONTEXT(ctx);
    dlist_fallback(ctx);
    CALL_CallList(ctx->Save, (l));
}

static void GLAPIENTRY
_save_CallLists(GLsizei n, GLenum type, const GLvoid * v)
{
    GET_CURRENT_CONTEXT(ctx);
    dlist_fallback(ctx);
    CALL_CallLists(ctx->Save, (n, type, v));
}



/**
 * Called when a glBegin is getting compiled into a display list.
 * Updating of ctx->Driver.CurrentSavePrimitive is already taken care of.
 */
void
vbo_save_NotifyBegin(struct gl_context *ctx, GLenum mode,
                     bool no_current_update)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    const GLuint i = save->prim_count++;

    ctx->Driver.CurrentSavePrimitive = mode;

    assert(i < save->prim_max);
    save->prims[i].mode = mode & VBO_SAVE_PRIM_MODE_MASK;
    save->prims[i].begin = 1;
    save->prims[i].end = 0;
    save->prims[i].start = save->vert_count;
    save->prims[i].count = 0;

    save->no_current_update = no_current_update;

    if (save->out_of_memory) {
        _mesa_install_save_vtxfmt(ctx, &save->vtxfmt_noop);
    }
    else {
        _mesa_install_save_vtxfmt(ctx, &save->vtxfmt);
    }

    /* We need to call vbo_save_SaveFlushVertices() if there's state change */
    ctx->Driver.SaveNeedFlush = GL_TRUE;
}


static void GLAPIENTRY
_save_End(void)
{
    GET_CURRENT_CONTEXT(ctx);
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    const GLint i = save->prim_count - 1;

    ctx->Driver.CurrentSavePrimitive = PRIM_OUTSIDE_BEGIN_END;
    save->prims[i].end = 1;
    save->prims[i].count = (save->vert_count - save->prims[i].start);

    if (i == (GLint) save->prim_max - 1) {
        compile_vertex_list(ctx);
        assert(save->copied.nr == 0);
    }

    /* Swap out this vertex format while outside begin/end.  Any color,
     * etc. received between here and the next begin will be compiled
     * as opcodes.
     */
    if (save->out_of_memory) {
        _mesa_install_save_vtxfmt(ctx, &save->vtxfmt_noop);
    }
    else {
        _mesa_install_save_vtxfmt(ctx, &ctx->ListState.ListVtxfmt);
    }
}


static void GLAPIENTRY
_save_Begin(GLenum mode)
{
    GET_CURRENT_CONTEXT(ctx);
    (void) mode;
    _mesa_compile_error(ctx, GL_INVALID_OPERATION, "Recursive glBegin");
}


static void GLAPIENTRY
_save_PrimitiveRestartNV(void)
{
    GET_CURRENT_CONTEXT(ctx);
    struct vbo_save_context *save = &vbo_context(ctx)->save;

    if (save->prim_count == 0) {
        /* We're not inside a glBegin/End pair, so calling glPrimitiverRestartNV
         * is an error.
         */
        _mesa_compile_error(ctx, GL_INVALID_OPERATION,
                            "glPrimitiveRestartNV called outside glBegin/End");
    } else {
        /* get current primitive mode */
        GLenum curPrim = save->prims[save->prim_count - 1].mode;
        bool no_current_update = save->no_current_update;

        /* restart primitive */
        CALL_End(ctx->CurrentServerDispatch, ());
        vbo_save_NotifyBegin(ctx, curPrim, no_current_update);
    }
}


/* Unlike the functions above, these are to be hooked into the vtxfmt
 * maintained in ctx->ListState, active when the list is known or
 * suspected to be outside any begin/end primitive.
 * Note: OBE = Outside Begin/End
 */
static void GLAPIENTRY
_save_OBE_Rectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    GET_CURRENT_CONTEXT(ctx);
    struct _glapi_table *dispatch = ctx->CurrentServerDispatch;

    vbo_save_NotifyBegin(ctx, GL_QUADS, false);
    CALL_Vertex2f(dispatch, (x1, y1));
    CALL_Vertex2f(dispatch, (x2, y1));
    CALL_Vertex2f(dispatch, (x2, y2));
    CALL_Vertex2f(dispatch, (x1, y2));
    CALL_End(dispatch, ());
}


static void GLAPIENTRY
_save_OBE_Rectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
    _save_OBE_Rectf((GLfloat) x1, (GLfloat) y1, (GLfloat) x2, (GLfloat) y2);
}

static void GLAPIENTRY
_save_OBE_Rectdv(const GLdouble *v1, const GLdouble *v2)
{
    _save_OBE_Rectf((GLfloat) v1[0], (GLfloat) v1[1], (GLfloat) v2[0], (GLfloat) v2[1]);
}

static void GLAPIENTRY
_save_OBE_Rectfv(const GLfloat *v1, const GLfloat *v2)
{
    _save_OBE_Rectf(v1[0], v1[1], v2[0], v2[1]);
}

static void GLAPIENTRY
_save_OBE_Recti(GLint x1, GLint y1, GLint x2, GLint y2)
{
    _save_OBE_Rectf((GLfloat) x1, (GLfloat) y1, (GLfloat) x2, (GLfloat) y2);
}

static void GLAPIENTRY
_save_OBE_Rectiv(const GLint *v1, const GLint *v2)
{
    _save_OBE_Rectf((GLfloat) v1[0], (GLfloat) v1[1], (GLfloat) v2[0], (GLfloat) v2[1]);
}

static void GLAPIENTRY
_save_OBE_Rects(GLshort x1, GLshort y1, GLshort x2, GLshort y2)
{
    _save_OBE_Rectf((GLfloat) x1, (GLfloat) y1, (GLfloat) x2, (GLfloat) y2);
}

static void GLAPIENTRY
_save_OBE_Rectsv(const GLshort *v1, const GLshort *v2)
{
    _save_OBE_Rectf((GLfloat) v1[0], (GLfloat) v1[1], (GLfloat) v2[0], (GLfloat) v2[1]);
}

static void
_ensure_draws_fits_in_storage(struct gl_context *ctx, int primcount, int vertcount)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;

    bool realloc_prim = save->prim_count + primcount > save->prim_max;
    bool realloc_vert = save->vertex_size && (save->vert_count + vertcount >= save->max_vert);

    if (realloc_prim || realloc_vert) {
        if (save->vert_count || save->prim_count)
            compile_vertex_list(ctx);
        realloc_storage(ctx, realloc_prim ? primcount : -1, realloc_vert ? vertcount : -1);
        reset_counters(ctx);
        assert(save->prim_max);
    }
}


static void GLAPIENTRY
_save_OBE_DrawArrays(GLenum mode, GLint start, GLsizei count)
{
    GET_CURRENT_CONTEXT(ctx);
    struct gl_vertex_array_object *vao = ctx->Array.VAO;
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    GLint i;

    if (!_mesa_is_valid_prim_mode(ctx, mode)) {
        _mesa_compile_error(ctx, GL_INVALID_ENUM, "glDrawArrays(mode)");
        return;
    }
    if (count < 0) {
        _mesa_compile_error(ctx, GL_INVALID_VALUE, "glDrawArrays(count<0)");
        return;
    }

    if (save->out_of_memory)
        return;

    _ensure_draws_fits_in_storage(ctx, 1, count);

    /* Make sure to process any VBO binding changes */
    _mesa_update_state(ctx);

    _mesa_vao_map_arrays(ctx, vao, GL_MAP_READ_BIT);

    vbo_save_NotifyBegin(ctx, mode, true);

    for (i = 0; i < count; i++)
        _mesa_array_element(ctx, start + i);
    CALL_End(ctx->CurrentServerDispatch, ());

    _mesa_vao_unmap_arrays(ctx, vao);
}


static void GLAPIENTRY
_save_OBE_MultiDrawArrays(GLenum mode, const GLint *first,
                          const GLsizei *count, GLsizei primcount)
{
    GET_CURRENT_CONTEXT(ctx);
    GLint i;

    if (!_mesa_is_valid_prim_mode(ctx, mode)) {
        _mesa_compile_error(ctx, GL_INVALID_ENUM, "glMultiDrawArrays(mode)");
        return;
    }

    if (primcount < 0) {
        _mesa_compile_error(ctx, GL_INVALID_VALUE,
                            "glMultiDrawArrays(primcount<0)");
        return;
    }

    unsigned vertcount = 0;
    for (i = 0; i < primcount; i++) {
        if (count[i] < 0) {
            _mesa_compile_error(ctx, GL_INVALID_VALUE,
                                "glMultiDrawArrays(count[i]<0)");
            return;
        }
        vertcount += count[i];
    }

    _ensure_draws_fits_in_storage(ctx, primcount, vertcount);

    for (i = 0; i < primcount; i++) {
        if (count[i] > 0) {
            _save_OBE_DrawArrays(mode, first[i], count[i]);
        }
    }
}


static void
array_element(struct gl_context *ctx,
              GLint basevertex, GLuint elt, unsigned index_size_shift)
{
    /* Section 10.3.5 Primitive Restart:
     * [...]
     *    When one of the *BaseVertex drawing commands specified in section 10.5
     * is used, the primitive restart comparison occurs before the basevertex
     * offset is added to the array index.
     */
    /* If PrimitiveRestart is enabled and the index is the RestartIndex
     * then we call PrimitiveRestartNV and return.
     */
    if (ctx->Array._PrimitiveRestart[index_size_shift] &&
        elt == ctx->Array._RestartIndex[index_size_shift]) {
        CALL_PrimitiveRestartNV(ctx->CurrentServerDispatch, ());
        return;
    }

    _mesa_array_element(ctx, basevertex + elt);
}


/* Could do better by copying the arrays and element list intact and
 * then emitting an indexed prim at runtime.
 */
static void GLAPIENTRY
_save_OBE_DrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type,
                                 const GLvoid * indices, GLint basevertex)
{
    GET_CURRENT_CONTEXT(ctx);
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    struct gl_vertex_array_object *vao = ctx->Array.VAO;
    struct gl_buffer_object *indexbuf = vao->IndexBufferObj;
    GLint i;

    if (!_mesa_is_valid_prim_mode(ctx, mode)) {
        _mesa_compile_error(ctx, GL_INVALID_ENUM, "glDrawElements(mode)");
        return;
    }
    if (count < 0) {
        _mesa_compile_error(ctx, GL_INVALID_VALUE, "glDrawElements(count<0)");
        return;
    }
    if (type != GL_UNSIGNED_BYTE &&
        type != GL_UNSIGNED_SHORT &&
        type != GL_UNSIGNED_INT) {
        _mesa_compile_error(ctx, GL_INVALID_VALUE, "glDrawElements(count<0)");
        return;
    }

    if (save->out_of_memory)
        return;

    _ensure_draws_fits_in_storage(ctx, 1, count);

    /* Make sure to process any VBO binding changes */
    _mesa_update_state(ctx);

    _mesa_vao_map(ctx, vao, GL_MAP_READ_BIT);

    if (indexbuf)
        indices =
                ADD_POINTERS(indexbuf->Mappings[MAP_INTERNAL].Pointer, indices);

    vbo_save_NotifyBegin(ctx, mode, true);

    switch (type) {
        case GL_UNSIGNED_BYTE:
            for (i = 0; i < count; i++)
                array_element(ctx, basevertex, ((GLubyte *) indices)[i], 0);
            break;
        case GL_UNSIGNED_SHORT:
            for (i = 0; i < count; i++)
                array_element(ctx, basevertex, ((GLushort *) indices)[i], 1);
            break;
        case GL_UNSIGNED_INT:
            for (i = 0; i < count; i++)
                array_element(ctx, basevertex, ((GLuint *) indices)[i], 2);
            break;
        default:
            _mesa_error(ctx, GL_INVALID_ENUM, "glDrawElements(type)");
            break;
    }

    CALL_End(ctx->CurrentServerDispatch, ());

    _mesa_vao_unmap(ctx, vao);
}

static void GLAPIENTRY
_save_OBE_DrawElements(GLenum mode, GLsizei count, GLenum type,
                       const GLvoid * indices)
{
    _save_OBE_DrawElementsBaseVertex(mode, count, type, indices, 0);
}


static void GLAPIENTRY
_save_OBE_DrawRangeElements(GLenum mode, GLuint start, GLuint end,
                            GLsizei count, GLenum type,
                            const GLvoid * indices)
{
    GET_CURRENT_CONTEXT(ctx);
    struct vbo_save_context *save = &vbo_context(ctx)->save;

    if (!_mesa_is_valid_prim_mode(ctx, mode)) {
        _mesa_compile_error(ctx, GL_INVALID_ENUM, "glDrawRangeElements(mode)");
        return;
    }
    if (count < 0) {
        _mesa_compile_error(ctx, GL_INVALID_VALUE,
                            "glDrawRangeElements(count<0)");
        return;
    }
    if (type != GL_UNSIGNED_BYTE &&
        type != GL_UNSIGNED_SHORT &&
        type != GL_UNSIGNED_INT) {
        _mesa_compile_error(ctx, GL_INVALID_ENUM, "glDrawRangeElements(type)");
        return;
    }
    if (end < start) {
        _mesa_compile_error(ctx, GL_INVALID_VALUE,
                            "glDrawRangeElements(end < start)");
        return;
    }

    if (save->out_of_memory)
        return;

    _save_OBE_DrawElements(mode, count, type, indices);
}


static void GLAPIENTRY
_save_OBE_MultiDrawElements(GLenum mode, const GLsizei *count, GLenum type,
                            const GLvoid * const *indices, GLsizei primcount)
{
    GET_CURRENT_CONTEXT(ctx);
    struct _glapi_table *dispatch = ctx->CurrentServerDispatch;
    GLsizei i;

    int vertcount = 0;
    for (i = 0; i < primcount; i++) {
        vertcount += count[i];
    }
    _ensure_draws_fits_in_storage(ctx, primcount, vertcount);

    for (i = 0; i < primcount; i++) {
        if (count[i] > 0) {
            CALL_DrawElements(dispatch, (mode, count[i], type, indices[i]));
        }
    }
}


static void GLAPIENTRY
_save_OBE_MultiDrawElementsBaseVertex(GLenum mode, const GLsizei *count,
                                      GLenum type,
                                      const GLvoid * const *indices,
                                      GLsizei primcount,
                                      const GLint *basevertex)
{
    GET_CURRENT_CONTEXT(ctx);
    struct _glapi_table *dispatch = ctx->CurrentServerDispatch;
    GLsizei i;

    int vertcount = 0;
    for (i = 0; i < primcount; i++) {
        vertcount += count[i];
    }
    _ensure_draws_fits_in_storage(ctx, primcount, vertcount);

    for (i = 0; i < primcount; i++) {
        if (count[i] > 0) {
            CALL_DrawElementsBaseVertex(dispatch, (mode, count[i], type,
                    indices[i],
                    basevertex[i]));
        }
    }
}


static void
vtxfmt_init(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    GLvertexformat *vfmt = &save->vtxfmt;

#define NAME_AE(x) _ae_##x
#define NAME_CALLLIST(x) _save_##x
#define NAME(x) _save_##x
#define NAME_ES(x) _save_##x##ARB

#include "vbo_init_tmp.h"
}


/**
 * Initialize the dispatch table with the VBO functions for display
 * list compilation.
 */
void
vbo_initialize_save_dispatch(const struct gl_context *ctx,
                             struct _glapi_table *exec)
{
    SET_DrawArrays(exec, _save_OBE_DrawArrays);
    SET_MultiDrawArrays(exec, _save_OBE_MultiDrawArrays);
    SET_DrawElements(exec, _save_OBE_DrawElements);
    SET_DrawElementsBaseVertex(exec, _save_OBE_DrawElementsBaseVertex);
    SET_DrawRangeElements(exec, _save_OBE_DrawRangeElements);
    SET_MultiDrawElementsEXT(exec, _save_OBE_MultiDrawElements);
    SET_MultiDrawElementsBaseVertex(exec, _save_OBE_MultiDrawElementsBaseVertex);
    SET_Rectf(exec, _save_OBE_Rectf);
    SET_Rectd(exec, _save_OBE_Rectd);
    SET_Rectdv(exec, _save_OBE_Rectdv);
    SET_Rectfv(exec, _save_OBE_Rectfv);
    SET_Recti(exec, _save_OBE_Recti);
    SET_Rectiv(exec, _save_OBE_Rectiv);
    SET_Rects(exec, _save_OBE_Rects);
    SET_Rectsv(exec, _save_OBE_Rectsv);

    /* Note: other glDraw functins aren't compiled into display lists */
}



void
vbo_save_SaveFlushVertices(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;

    /* Noop when we are actually active:
     */
    if (ctx->Driver.CurrentSavePrimitive <= PRIM_MAX)
        return;

    if (save->vert_count || save->prim_count)
        compile_vertex_list(ctx);

    copy_to_current(ctx);
    reset_vertex(ctx);
    reset_counters(ctx);
    ctx->Driver.SaveNeedFlush = GL_FALSE;
}


/**
 * Called from glNewList when we're starting to compile a display list.
 */
void
vbo_save_NewList(struct gl_context *ctx, GLuint list, GLenum mode)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;

    (void) list;
    (void) mode;

    if (!save->prim_store)
        save->prim_store = alloc_prim_store(0);

    if (!save->vertex_store)
        save->vertex_store = alloc_vertex_store(ctx, 0);

    save->buffer_ptr = vbo_save_map_vertex_store(ctx, save->vertex_store);

    reset_vertex(ctx);
    reset_counters(ctx);
    ctx->Driver.SaveNeedFlush = GL_FALSE;
}


/**
 * Called from glEndList when we're finished compiling a display list.
 */
void
vbo_save_EndList(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;

    /* EndList called inside a (saved) Begin/End pair?
     */
    if (_mesa_inside_dlist_begin_end(ctx)) {
        if (save->prim_count > 0) {
            GLint i = save->prim_count - 1;
            ctx->Driver.CurrentSavePrimitive = PRIM_OUTSIDE_BEGIN_END;
            save->prims[i].end = 0;
            save->prims[i].count = save->vert_count - save->prims[i].start;
        }

        /* Make sure this vertex list gets replayed by the "loopback"
         * mechanism:
         */
        save->dangling_attr_ref = GL_TRUE;
        vbo_save_SaveFlushVertices(ctx);

        /* Swap out this vertex format while outside begin/end.  Any color,
         * etc. received between here and the next begin will be compiled
         * as opcodes.
         */
        _mesa_install_save_vtxfmt(ctx, &ctx->ListState.ListVtxfmt);
    }

    vbo_save_unmap_vertex_store(ctx, save->vertex_store);

    assert(save->vertex_size == 0);
}


/**
 * Called from the display list code when we're about to execute a
 * display list.
 */
void
vbo_save_BeginCallList(struct gl_context *ctx, struct gl_display_list *dlist)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    save->replay_flags |= dlist->Flags;
}


/**
 * Called from the display list code when we're finished executing a
 * display list.
 */
void
vbo_save_EndCallList(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;

    if (ctx->ListState.CallDepth == 1)
        save->replay_flags = 0;
}


/**
 * Called during context creation/init.
 */
static void
current_init(struct gl_context *ctx)
{
    struct vbo_save_context *save = &vbo_context(ctx)->save;
    GLint i;

    for (i = VBO_ATTRIB_POS; i <= VBO_ATTRIB_GENERIC15; i++) {
        const GLuint j = i - VBO_ATTRIB_POS;
        assert(j < VERT_ATTRIB_MAX);
        save->currentsz[i] = &ctx->ListState.ActiveAttribSize[j];
        save->current[i] = (fi_type *) ctx->ListState.CurrentAttrib[j];
    }

    for (i = VBO_ATTRIB_FIRST_MATERIAL; i <= VBO_ATTRIB_LAST_MATERIAL; i++) {
        const GLuint j = i - VBO_ATTRIB_FIRST_MATERIAL;
        assert(j < MAT_ATTRIB_MAX);
        save->currentsz[i] = &ctx->ListState.ActiveMaterialSize[j];
        save->current[i] = (fi_type *) ctx->ListState.CurrentMaterial[j];
    }
}


/**
 * Initialize the display list compiler.  Called during context creation.
 */
void
vbo_save_api_init(struct vbo_save_context *save)
{
    struct gl_context *ctx = gl_context_from_vbo_save(save);

    vtxfmt_init(ctx);
    current_init(ctx);
    _mesa_noop_vtxfmt_init(ctx, &save->vtxfmt_noop);
}
