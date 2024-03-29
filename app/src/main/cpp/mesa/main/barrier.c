#include "context.h"
#include "barrier.h"


static void
_mesa_texture_barrier(struct gl_context *ctx)
{
    /* no-op */
}

void
_mesa_init_barrier_functions(struct dd_function_table *driver)
{
    driver->TextureBarrier = _mesa_texture_barrier;
}

void GLAPIENTRY
_mesa_TextureBarrierNV(void)
{
    GET_CURRENT_CONTEXT(ctx);

    if (!ctx->Extensions.NV_texture_barrier) {
        _mesa_error(ctx, GL_INVALID_OPERATION,
                    "glTextureBarrier(not supported)");
        return;
    }

    ctx->Driver.TextureBarrier(ctx);
}

void GLAPIENTRY
_mesa_MemoryBarrier(GLbitfield barriers)
{
    GET_CURRENT_CONTEXT(ctx);

    if (ctx->Driver.MemoryBarrier)
        ctx->Driver.MemoryBarrier(ctx, barriers);
}

static ALWAYS_INLINE void
memory_barrier_by_region(struct gl_context *ctx, GLbitfield barriers,
                         bool no_error)
{
    GLbitfield all_allowed_bits = GL_ATOMIC_COUNTER_BARRIER_BIT |
                                  GL_FRAMEBUFFER_BARRIER_BIT |
                                  GL_SHADER_IMAGE_ACCESS_BARRIER_BIT |
                                  GL_SHADER_STORAGE_BARRIER_BIT |
                                  GL_TEXTURE_FETCH_BARRIER_BIT |
                                  GL_UNIFORM_BARRIER_BIT;

    if (ctx->Driver.MemoryBarrier) {
        /* From section 7.11.2 of the OpenGL ES 3.1 specification:
         *
         *    "When barriers is ALL_BARRIER_BITS, shader memory accesses will be
         *     synchronized relative to all these barrier bits, but not to other
         *     barrier bits specific to MemoryBarrier."
         *
         * That is, if barriers is the special value GL_ALL_BARRIER_BITS, then all
         * barriers allowed by glMemoryBarrierByRegion should be activated."
         */
        if (barriers == GL_ALL_BARRIER_BITS) {
            ctx->Driver.MemoryBarrier(ctx, all_allowed_bits);
            return;
        }

        /* From section 7.11.2 of the OpenGL ES 3.1 specification:
         *
         *    "An INVALID_VALUE error is generated if barriers is not the special
         *     value ALL_BARRIER_BITS, and has any bits set other than those
         *     described above."
         */
        if (!no_error && (barriers & ~all_allowed_bits) != 0) {
            _mesa_error(ctx, GL_INVALID_VALUE,
                        "glMemoryBarrierByRegion(unsupported barrier bit");
        }

        ctx->Driver.MemoryBarrier(ctx, barriers);
    }
}

void GLAPIENTRY
_mesa_MemoryBarrierByRegion_no_error(GLbitfield barriers)
{
    GET_CURRENT_CONTEXT(ctx);
    memory_barrier_by_region(ctx, barriers, true);
}

void GLAPIENTRY
_mesa_MemoryBarrierByRegion(GLbitfield barriers)
{
    GET_CURRENT_CONTEXT(ctx);
    memory_barrier_by_region(ctx, barriers, false);
}

void GLAPIENTRY
_mesa_BlendBarrier(void)
{
    GET_CURRENT_CONTEXT(ctx);

    if (!ctx->Extensions.KHR_blend_equation_advanced) {
        _mesa_error(ctx, GL_INVALID_OPERATION,
                    "glBlendBarrier(not supported)");
        return;
    }

    ctx->Driver.FramebufferFetchBarrier(ctx);
}

void GLAPIENTRY
_mesa_FramebufferFetchBarrierEXT(void)
{
    GET_CURRENT_CONTEXT(ctx);

    if (!ctx->Extensions.EXT_shader_framebuffer_fetch_non_coherent) {
        _mesa_error(ctx, GL_INVALID_OPERATION,
                    "glFramebufferFetchBarrierEXT(not supported)");
        return;
    }

    ctx->Driver.FramebufferFetchBarrier(ctx);
}
