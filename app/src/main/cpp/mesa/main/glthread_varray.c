#include "glthread.h"
#include "glformats.h"
#include "mtypes.h"
#include "hash.h"
#include "dispatch.h"
#include "varray.h"


void
_mesa_glthread_reset_vao(struct glthread_vao *vao)
{
    static unsigned default_elem_size[VERT_ATTRIB_MAX] = {
            [VERT_ATTRIB_NORMAL] = 12,
            [VERT_ATTRIB_COLOR1] = 12,
            [VERT_ATTRIB_FOG] = 4,
            [VERT_ATTRIB_COLOR_INDEX] = 4,
            [VERT_ATTRIB_EDGEFLAG] = 1,
            [VERT_ATTRIB_POINT_SIZE] = 4,
    };

    vao->CurrentElementBufferName = 0;
    vao->UserEnabled = 0;
    vao->Enabled = 0;
    vao->BufferEnabled = 0;
    vao->UserPointerMask = 0;
    vao->NonZeroDivisorMask = 0;

    for (unsigned i = 0; i < ARRAY_SIZE(vao->Attrib); i++) {
        unsigned elem_size = default_elem_size[i];
        if (!elem_size)
            elem_size = 16;

        vao->Attrib[i].ElementSize = elem_size;
        vao->Attrib[i].RelativeOffset = 0;
        vao->Attrib[i].BufferIndex = i;
        vao->Attrib[i].Stride = elem_size;
        vao->Attrib[i].Divisor = 0;
        vao->Attrib[i].EnabledAttribCount = 0;
        vao->Attrib[i].Pointer = NULL;
    }
}

static struct glthread_vao *
lookup_vao(struct gl_context *ctx, GLuint id)
{
    struct glthread_state *glthread = &ctx->GLThread;
    struct glthread_vao *vao;

    assert(id != 0);

    if (glthread->LastLookedUpVAO &&
        glthread->LastLookedUpVAO->Name == id) {
        vao = glthread->LastLookedUpVAO;
    } else {
        vao = _mesa_HashLookupLocked(glthread->VAOs, id);
        if (!vao)
            return NULL;

        glthread->LastLookedUpVAO = vao;
    }

    return vao;
}

void
_mesa_glthread_BindVertexArray(struct gl_context *ctx, GLuint id)
{
    struct glthread_state *glthread = &ctx->GLThread;

    if (id == 0) {
        glthread->CurrentVAO = &glthread->DefaultVAO;
    } else {
        struct glthread_vao *vao = lookup_vao(ctx, id);

        if (vao)
            glthread->CurrentVAO = vao;
    }
}

void
_mesa_glthread_DeleteVertexArrays(struct gl_context *ctx,
                                  GLsizei n, const GLuint *ids)
{
    struct glthread_state *glthread = &ctx->GLThread;

    if (!ids)
        return;

    for (int i = 0; i < n; i++) {
        /* IDs equal to 0 should be silently ignored. */
        if (!ids[i])
            continue;

        struct glthread_vao *vao = lookup_vao(ctx, ids[i]);
        if (!vao)
            continue;

        /* If the array object is currently bound, the spec says "the binding
         * for that object reverts to zero and the default vertex array
         * becomes current."
         */
        if (glthread->CurrentVAO == vao)
            glthread->CurrentVAO = &glthread->DefaultVAO;

        if (glthread->LastLookedUpVAO == vao)
            glthread->LastLookedUpVAO = NULL;

        /* The ID is immediately freed for re-use */
        _mesa_HashRemoveLocked(glthread->VAOs, vao->Name);
        free(vao);
    }
}

void
_mesa_glthread_GenVertexArrays(struct gl_context *ctx,
                               GLsizei n, GLuint *arrays)
{
    struct glthread_state *glthread = &ctx->GLThread;

    if (!arrays)
        return;

    /* The IDs have been generated at this point. Create VAOs for glthread. */
    for (int i = 0; i < n; i++) {
        GLuint id = arrays[i];
        struct glthread_vao *vao;

        vao = calloc(1, sizeof(*vao));
        if (!vao)
            continue; /* Is that all we can do? */

        vao->Name = id;
        _mesa_glthread_reset_vao(vao);
        _mesa_HashInsertLocked(glthread->VAOs, id, vao, true);
    }
}

/* If vaobj is NULL, use the currently-bound VAO. */
static inline struct glthread_vao *
get_vao(struct gl_context *ctx, const GLuint *vaobj)
{
    if (vaobj)
        return lookup_vao(ctx, *vaobj);

    return ctx->GLThread.CurrentVAO;
}

static void
update_primitive_restart(struct gl_context *ctx)
{
    struct glthread_state *glthread = &ctx->GLThread;

    glthread->_PrimitiveRestart = glthread->PrimitiveRestart ||
                                  glthread->PrimitiveRestartFixedIndex;
    glthread->_RestartIndex[0] =
            _mesa_get_prim_restart_index(glthread->PrimitiveRestartFixedIndex,
                                         glthread->RestartIndex, 1);
    glthread->_RestartIndex[1] =
            _mesa_get_prim_restart_index(glthread->PrimitiveRestartFixedIndex,
                                         glthread->RestartIndex, 2);
    glthread->_RestartIndex[3] =
            _mesa_get_prim_restart_index(glthread->PrimitiveRestartFixedIndex,
                                         glthread->RestartIndex, 4);
}

void
_mesa_glthread_set_prim_restart(struct gl_context *ctx, GLenum cap, bool value)
{
    switch (cap) {
        case GL_PRIMITIVE_RESTART:
            ctx->GLThread.PrimitiveRestart = value;
            break;
        case GL_PRIMITIVE_RESTART_FIXED_INDEX:
            ctx->GLThread.PrimitiveRestartFixedIndex = value;
            break;
    }

    update_primitive_restart(ctx);
}

void
_mesa_glthread_PrimitiveRestartIndex(struct gl_context *ctx, GLuint index)
{
    ctx->GLThread.RestartIndex = index;
    update_primitive_restart(ctx);
}

static inline void
enable_buffer(struct glthread_vao *vao, unsigned binding_index)
{
    int attrib_count = ++vao->Attrib[binding_index].EnabledAttribCount;

    if (attrib_count == 1)
        vao->BufferEnabled |= 1 << binding_index;
    else if (attrib_count == 2)
        vao->BufferInterleaved |= 1 << binding_index;
}

static inline void
disable_buffer(struct glthread_vao *vao, unsigned binding_index)
{
    int attrib_count = --vao->Attrib[binding_index].EnabledAttribCount;

    if (attrib_count == 0)
        vao->BufferEnabled &= ~(1 << binding_index);
    else if (attrib_count == 1)
        vao->BufferInterleaved &= ~(1 << binding_index);
    else
        assert(attrib_count >= 0);
}

void
_mesa_glthread_ClientState(struct gl_context *ctx, GLuint *vaobj,
                           gl_vert_attrib attrib, bool enable)
{
    /* The primitive restart client state uses a special value. */
    if (attrib == VERT_ATTRIB_PRIMITIVE_RESTART_NV) {
        ctx->GLThread.PrimitiveRestart = enable;
        update_primitive_restart(ctx);
        return;
    }

    if (attrib >= VERT_ATTRIB_MAX)
        return;

    struct glthread_vao *vao = get_vao(ctx, vaobj);
    if (!vao)
        return;

    const unsigned attrib_bit = 1u << attrib;

    if (enable && !(vao->UserEnabled & attrib_bit)) {
        vao->UserEnabled |= attrib_bit;

        /* The generic0 attribute supersedes the position attribute. We need to
         * update BufferBindingEnabled accordingly.
         */
        if (attrib == VERT_ATTRIB_POS) {
            if (!(vao->UserEnabled & VERT_BIT_GENERIC0))
                enable_buffer(vao, vao->Attrib[VERT_ATTRIB_POS].BufferIndex);
        } else {
            enable_buffer(vao, vao->Attrib[attrib].BufferIndex);

            if (attrib == VERT_ATTRIB_GENERIC0 && vao->UserEnabled & VERT_BIT_POS)
                disable_buffer(vao, vao->Attrib[VERT_ATTRIB_POS].BufferIndex);
        }
    } else if (!enable && (vao->UserEnabled & attrib_bit)) {
        vao->UserEnabled &= ~attrib_bit;

        /* The generic0 attribute supersedes the position attribute. We need to
         * update BufferBindingEnabled accordingly.
         */
        if (attrib == VERT_ATTRIB_POS) {
            if (!(vao->UserEnabled & VERT_BIT_GENERIC0))
                disable_buffer(vao, vao->Attrib[VERT_ATTRIB_POS].BufferIndex);
        } else {
            disable_buffer(vao, vao->Attrib[attrib].BufferIndex);

            if (attrib == VERT_ATTRIB_GENERIC0 && vao->UserEnabled & VERT_BIT_POS)
                enable_buffer(vao, vao->Attrib[VERT_ATTRIB_POS].BufferIndex);
        }
    }

    /* The generic0 attribute supersedes the position attribute. */
    vao->Enabled = vao->UserEnabled;
    if (vao->Enabled & VERT_BIT_GENERIC0)
        vao->Enabled &= ~VERT_BIT_POS;
}

static void
set_attrib_binding(struct glthread_state *glthread, struct glthread_vao *vao,
                   gl_vert_attrib attrib, unsigned new_binding_index)
{
    unsigned old_binding_index = vao->Attrib[attrib].BufferIndex;

    if (old_binding_index != new_binding_index) {
        vao->Attrib[attrib].BufferIndex = new_binding_index;

        if (vao->Enabled & (1u << attrib)) {
            /* Update BufferBindingEnabled. */
            enable_buffer(vao, new_binding_index);
            disable_buffer(vao, old_binding_index);
        }
    }
}

void _mesa_glthread_AttribDivisor(struct gl_context *ctx, const GLuint *vaobj,
                                  gl_vert_attrib attrib, GLuint divisor)
{
    if (attrib >= VERT_ATTRIB_MAX)
        return;

    struct glthread_vao *vao = get_vao(ctx, vaobj);
    if (!vao)
        return;

    vao->Attrib[attrib].Divisor = divisor;

    set_attrib_binding(&ctx->GLThread, vao, attrib, attrib);

    if (divisor)
        vao->NonZeroDivisorMask |= 1u << attrib;
    else
        vao->NonZeroDivisorMask &= ~(1u << attrib);
}

static unsigned
element_size(GLint size, GLenum type)
{
    if (size == GL_BGRA)
        size = 4;

    return _mesa_bytes_per_vertex_attrib(size, type);
}

static void
attrib_pointer(struct glthread_state *glthread, struct glthread_vao *vao,
               GLuint buffer, gl_vert_attrib attrib,
               GLint size, GLenum type, GLsizei stride,
               const void *pointer)
{
    if (attrib >= VERT_ATTRIB_MAX)
        return;

    unsigned elem_size = element_size(size, type);

    vao->Attrib[attrib].ElementSize = elem_size;
    vao->Attrib[attrib].Stride = stride ? stride : elem_size;
    vao->Attrib[attrib].Pointer = pointer;
    vao->Attrib[attrib].RelativeOffset = 0;

    set_attrib_binding(glthread, vao, attrib, attrib);

    if (buffer != 0)
        vao->UserPointerMask &= ~(1u << attrib);
    else
        vao->UserPointerMask |= 1u << attrib;
}

void
_mesa_glthread_AttribPointer(struct gl_context *ctx, gl_vert_attrib attrib,
                             GLint size, GLenum type, GLsizei stride,
                             const void *pointer)
{
    struct glthread_state *glthread = &ctx->GLThread;

    attrib_pointer(glthread, glthread->CurrentVAO,
                   glthread->CurrentArrayBufferName,
                   attrib, size, type, stride, pointer);
}

void
_mesa_glthread_DSAAttribPointer(struct gl_context *ctx, GLuint vaobj,
                                GLuint buffer, gl_vert_attrib attrib,
                                GLint size, GLenum type, GLsizei stride,
                                GLintptr offset)
{
    struct glthread_state *glthread = &ctx->GLThread;
    struct glthread_vao *vao;

    vao = lookup_vao(ctx, vaobj);
    if (!vao)
        return;

    attrib_pointer(glthread, vao, buffer, attrib, size, type, stride,
                   (const void*)offset);
}

static void
attrib_format(struct glthread_state *glthread, struct glthread_vao *vao,
              GLuint attribindex, GLint size, GLenum type,
              GLuint relativeoffset)
{
    if (attribindex >= VERT_ATTRIB_GENERIC_MAX)
        return;

    unsigned elem_size = element_size(size, type);

    unsigned i = VERT_ATTRIB_GENERIC(attribindex);
    vao->Attrib[i].ElementSize = elem_size;
    vao->Attrib[i].RelativeOffset = relativeoffset;
}

void
_mesa_glthread_AttribFormat(struct gl_context *ctx, GLuint attribindex,
                            GLint size, GLenum type, GLuint relativeoffset)
{
    struct glthread_state *glthread = &ctx->GLThread;

    attrib_format(glthread, glthread->CurrentVAO, attribindex, size, type,
                  relativeoffset);
}

void
_mesa_glthread_DSAAttribFormat(struct gl_context *ctx, GLuint vaobj,
                               GLuint attribindex, GLint size, GLenum type,
                               GLuint relativeoffset)
{
    struct glthread_state *glthread = &ctx->GLThread;
    struct glthread_vao *vao = lookup_vao(ctx, vaobj);

    if (vao)
        attrib_format(glthread, vao, attribindex, size, type, relativeoffset);
}

static void
bind_vertex_buffer(struct glthread_state *glthread, struct glthread_vao *vao,
                   GLuint bindingindex, GLuint buffer, GLintptr offset,
                   GLsizei stride)
{
    if (bindingindex >= VERT_ATTRIB_GENERIC_MAX)
        return;

    unsigned i = VERT_ATTRIB_GENERIC(bindingindex);
    vao->Attrib[i].Pointer = (const void*)offset;
    vao->Attrib[i].Stride = stride;

    if (buffer != 0)
        vao->UserPointerMask &= ~(1u << i);
    else
        vao->UserPointerMask |= 1u << i;
}

void
_mesa_glthread_VertexBuffer(struct gl_context *ctx, GLuint bindingindex,
                            GLuint buffer, GLintptr offset, GLsizei stride)
{
    struct glthread_state *glthread = &ctx->GLThread;

    bind_vertex_buffer(glthread, glthread->CurrentVAO, bindingindex, buffer,
                       offset, stride);
}

void
_mesa_glthread_DSAVertexBuffer(struct gl_context *ctx, GLuint vaobj,
                               GLuint bindingindex, GLuint buffer,
                               GLintptr offset, GLsizei stride)
{
    struct glthread_state *glthread = &ctx->GLThread;
    struct glthread_vao *vao = lookup_vao(ctx, vaobj);

    if (vao)
        bind_vertex_buffer(glthread, vao, bindingindex, buffer, offset, stride);
}

void
_mesa_glthread_DSAVertexBuffers(struct gl_context *ctx, GLuint vaobj,
                                GLuint first, GLsizei count,
                                const GLuint *buffers,
                                const GLintptr *offsets,
                                const GLsizei *strides)
{
    struct glthread_state *glthread = &ctx->GLThread;
    struct glthread_vao *vao;

    vao = lookup_vao(ctx, vaobj);
    if (!vao)
        return;

    for (unsigned i = 0; i < count; i++) {
        bind_vertex_buffer(glthread, vao, first + i, buffers[i], offsets[i],
                           strides[i]);
    }
}

static void
binding_divisor(struct glthread_state *glthread, struct glthread_vao *vao,
                GLuint bindingindex, GLuint divisor)
{
    if (bindingindex >= VERT_ATTRIB_GENERIC_MAX)
        return;

    unsigned i = VERT_ATTRIB_GENERIC(bindingindex);
    vao->Attrib[i].Divisor = divisor;

    if (divisor)
        vao->NonZeroDivisorMask |= 1u << i;
    else
        vao->NonZeroDivisorMask &= ~(1u << i);
}

void
_mesa_glthread_BindingDivisor(struct gl_context *ctx, GLuint bindingindex,
                              GLuint divisor)
{
    struct glthread_state *glthread = &ctx->GLThread;

    binding_divisor(glthread, glthread->CurrentVAO, bindingindex, divisor);
}

void
_mesa_glthread_DSABindingDivisor(struct gl_context *ctx, GLuint vaobj,
                                 GLuint bindingindex, GLuint divisor)
{
    struct glthread_state *glthread = &ctx->GLThread;
    struct glthread_vao *vao = lookup_vao(ctx, vaobj);

    if (vao)
        binding_divisor(glthread, vao, bindingindex, divisor);
}

void
_mesa_glthread_AttribBinding(struct gl_context *ctx, GLuint attribindex,
                             GLuint bindingindex)
{
    struct glthread_state *glthread = &ctx->GLThread;

    if (attribindex >= VERT_ATTRIB_GENERIC_MAX ||
        bindingindex >= VERT_ATTRIB_GENERIC_MAX)
        return;

    set_attrib_binding(glthread, glthread->CurrentVAO,
                       VERT_ATTRIB_GENERIC(attribindex),
                       VERT_ATTRIB_GENERIC(bindingindex));
}

void
_mesa_glthread_DSAAttribBinding(struct gl_context *ctx, GLuint vaobj,
                                GLuint attribindex, GLuint bindingindex)
{
    struct glthread_state *glthread = &ctx->GLThread;

    if (attribindex >= VERT_ATTRIB_GENERIC_MAX ||
        bindingindex >= VERT_ATTRIB_GENERIC_MAX)
        return;

    struct glthread_vao *vao = lookup_vao(ctx, vaobj);
    if (vao) {
        set_attrib_binding(glthread, vao,
                           VERT_ATTRIB_GENERIC(attribindex),
                           VERT_ATTRIB_GENERIC(bindingindex));
    }
}

void
_mesa_glthread_DSAElementBuffer(struct gl_context *ctx, GLuint vaobj,
                                GLuint buffer)
{
    struct glthread_vao *vao = lookup_vao(ctx, vaobj);

    if (vao)
        vao->CurrentElementBufferName = buffer;
}

void
_mesa_glthread_PushClientAttrib(struct gl_context *ctx, GLbitfield mask,
                                bool set_default)
{
    struct glthread_state *glthread = &ctx->GLThread;

    if (glthread->ClientAttribStackTop >= MAX_CLIENT_ATTRIB_STACK_DEPTH)
        return;

    struct glthread_client_attrib *top =
            &glthread->ClientAttribStack[glthread->ClientAttribStackTop];

    if (mask & GL_CLIENT_VERTEX_ARRAY_BIT) {
        top->VAO = *glthread->CurrentVAO;
        top->CurrentArrayBufferName = glthread->CurrentArrayBufferName;
        top->ClientActiveTexture = glthread->ClientActiveTexture;
        top->RestartIndex = glthread->RestartIndex;
        top->PrimitiveRestart = glthread->PrimitiveRestart;
        top->PrimitiveRestartFixedIndex = glthread->PrimitiveRestartFixedIndex;
        top->Valid = true;
    } else {
        top->Valid = false;
    }

    glthread->ClientAttribStackTop++;

    if (set_default)
        _mesa_glthread_ClientAttribDefault(ctx, mask);
}

void
_mesa_glthread_PopClientAttrib(struct gl_context *ctx)
{
    struct glthread_state *glthread = &ctx->GLThread;

    if (glthread->ClientAttribStackTop == 0)
        return;

    glthread->ClientAttribStackTop--;

    struct glthread_client_attrib *top =
            &glthread->ClientAttribStack[glthread->ClientAttribStackTop];

    if (!top->Valid)
        return;

    /* Popping a delete VAO is an error. */
    struct glthread_vao *vao = NULL;
    if (top->VAO.Name) {
        vao = lookup_vao(ctx, top->VAO.Name);
        if (!vao)
            return;
    }

    /* Restore states. */
    glthread->CurrentArrayBufferName = top->CurrentArrayBufferName;
    glthread->ClientActiveTexture = top->ClientActiveTexture;
    glthread->RestartIndex = top->RestartIndex;
    glthread->PrimitiveRestart = top->PrimitiveRestart;
    glthread->PrimitiveRestartFixedIndex = top->PrimitiveRestartFixedIndex;

    if (!vao)
        vao = &glthread->DefaultVAO;

    assert(top->VAO.Name == vao->Name);
    *vao = top->VAO; /* Copy all fields. */
    glthread->CurrentVAO = vao;
}

void
_mesa_glthread_ClientAttribDefault(struct gl_context *ctx, GLbitfield mask)
{
    struct glthread_state *glthread = &ctx->GLThread;

    if (!(mask & GL_CLIENT_VERTEX_ARRAY_BIT))
        return;

    glthread->CurrentArrayBufferName = 0;
    glthread->ClientActiveTexture = 0;
    glthread->RestartIndex = 0;
    glthread->PrimitiveRestart = false;
    glthread->PrimitiveRestartFixedIndex = false;
    glthread->CurrentVAO = &glthread->DefaultVAO;
    _mesa_glthread_reset_vao(glthread->CurrentVAO);
}

void
_mesa_glthread_InterleavedArrays(struct gl_context *ctx, GLenum format,
                                 GLsizei stride, const GLvoid *pointer)
{
    struct gl_interleaved_layout layout;
    unsigned tex = VERT_ATTRIB_TEX(ctx->GLThread.ClientActiveTexture);

    if (stride < 0 || !_mesa_get_interleaved_layout(format, &layout))
        return;

    if (!stride)
        stride = layout.defstride;

    _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_EDGEFLAG, false);
    _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_COLOR_INDEX, false);
    /* XXX also disable secondary color and generic arrays? */

    /* Texcoords */
    if (layout.tflag) {
        _mesa_glthread_ClientState(ctx, NULL, tex, true);
        _mesa_glthread_AttribPointer(ctx, tex, layout.tcomps, GL_FLOAT, stride,
                                     (GLubyte *) pointer + layout.toffset);
    } else {
        _mesa_glthread_ClientState(ctx, NULL, tex, false);
    }

    /* Color */
    if (layout.cflag) {
        _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_COLOR0, true);
        _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR0, layout.ccomps,
                                     layout.ctype, stride,
                                     (GLubyte *) pointer + layout.coffset);
    } else {
        _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_COLOR0, false);
    }

    /* Normals */
    if (layout.nflag) {
        _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_NORMAL, true);
        _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_NORMAL, 3, GL_FLOAT,
                                     stride, (GLubyte *) pointer + layout.noffset);
    } else {
        _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_NORMAL, false);
    }

    /* Vertices */
    _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_POS, true);
    _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_POS, layout.vcomps, GL_FLOAT,
                                 stride, (GLubyte *) pointer + layout.voffset);
}
