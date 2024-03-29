#include "../../targets/dri/dri_screen.h"
#include "dri_context.h"
#include "dri_drawable.h"

#include "../../../include/pipe/p_screen.h"
#include "../../../util/format/u_format.h"
#include "../../../util/u_memory.h"
#include "../../../util/u_inlines.h"

static uint32_t drifb_ID = 0;

static bool
dri_st_framebuffer_validate(struct st_context_iface *stctx,
                            struct st_framebuffer_iface *stfbi,
                            const enum st_attachment_type *statts,
                            unsigned count,
                            struct pipe_resource **out)
{
    struct dri_context *ctx = (struct dri_context *)stctx->st_manager_private;
    struct dri_drawable *drawable =
            (struct dri_drawable *) stfbi->st_manager_private;
    struct dri_screen *screen = dri_screen(drawable->sPriv);
    unsigned statt_mask, new_mask;
    bool new_stamp;
    int i;
    unsigned int lastStamp;
    struct pipe_resource **textures =
            drawable->stvis.samples > 1 ? drawable->msaa_textures
                                        : drawable->textures;

    statt_mask = 0x0;
    for (i = 0; i < count; i++)
        statt_mask |= (1 << statts[i]);

    /* record newly allocated textures */
    new_mask = (statt_mask & ~drawable->texture_mask);

    /*
     * dPriv->dri2.stamp is the server stamp.  dPriv->lastStamp is the
     * client stamp.  It has the value of the server stamp when last
     * checked.
     */
    do {
        lastStamp = drawable->dPriv->lastStamp;
        new_stamp = (drawable->texture_stamp != lastStamp);

        if (new_stamp || new_mask || screen->broken_invalidate) {
            if (new_stamp && drawable->update_drawable_info)
                drawable->update_drawable_info(drawable);

            drawable->allocate_textures(ctx, drawable, statts, count);

            /* add existing textures */
            for (i = 0; i < ST_ATTACHMENT_COUNT; i++) {
                if (textures[i])
                    statt_mask |= (1 << i);
            }

            drawable->texture_stamp = lastStamp;
            drawable->texture_mask = statt_mask;
        }
    } while (lastStamp != drawable->dPriv->lastStamp);

    /* Flush the pending set_damage_region request. */
    struct pipe_screen *pscreen = screen->base.screen;

    if (new_mask & (1 << ST_ATTACHMENT_BACK_LEFT) &&
        pscreen->set_damage_region) {
        struct pipe_resource *resource = textures[ST_ATTACHMENT_BACK_LEFT];

        pscreen->set_damage_region(pscreen, resource,
                                   drawable->num_damage_rects,
                                   drawable->damage_rects);
    }

    if (!out)
        return true;

    /* Set the window-system buffers for the gallium frontend. */
    for (i = 0; i < count; i++)
        pipe_resource_reference(&out[i], textures[statts[i]]);

    return true;
}

static bool
dri_st_framebuffer_flush_front(struct st_context_iface *stctx,
                               struct st_framebuffer_iface *stfbi,
                               enum st_attachment_type statt)
{
    struct dri_context *ctx = (struct dri_context *)stctx->st_manager_private;
    struct dri_drawable *drawable =
            (struct dri_drawable *) stfbi->st_manager_private;

    /* XXX remove this and just set the correct one on the framebuffer */
    return drawable->flush_frontbuffer(ctx, drawable, statt);
}

/**
 * The gallium frontend framebuffer interface flush_swapbuffers callback
 */
static bool
dri_st_framebuffer_flush_swapbuffers(struct st_context_iface *stctx,
                                     struct st_framebuffer_iface *stfbi)
{
    struct dri_context *ctx = (struct dri_context *)stctx->st_manager_private;
    struct dri_drawable *drawable =
            (struct dri_drawable *) stfbi->st_manager_private;

    if (drawable->flush_swapbuffers)
        drawable->flush_swapbuffers(ctx, drawable);

    return true;
}

/**
 * This is called when we need to set up GL rendering to a new X window.
 */
bool
dri_create_buffer(__DRIscreen * sPriv,
                  __DRIdrawable * dPriv,
                  const struct gl_config * visual, bool isPixmap)
{
    struct dri_screen *screen = sPriv->driverPrivate;
    struct dri_drawable *drawable = NULL;

    if (isPixmap)
        goto fail;		       /* not implemented */

    drawable = CALLOC_STRUCT(dri_drawable);
    if (drawable == NULL)
        goto fail;

    dri_fill_st_visual(&drawable->stvis, screen, visual);

    /* setup the st_framebuffer_iface */
    drawable->base.visual = &drawable->stvis;
    drawable->base.flush_front = dri_st_framebuffer_flush_front;
    drawable->base.validate = dri_st_framebuffer_validate;
    drawable->base.flush_swapbuffers = dri_st_framebuffer_flush_swapbuffers;
    drawable->base.st_manager_private = (void *) drawable;

    drawable->screen = screen;
    drawable->sPriv = sPriv;
    drawable->dPriv = dPriv;

    dPriv->driverPrivate = (void *)drawable;
    p_atomic_set(&drawable->base.stamp, 1);
    drawable->base.ID = p_atomic_inc_return(&drifb_ID);
    drawable->base.state_manager = &screen->base;

    return true;
    fail:
    FREE(drawable);
    return false;
}

void
dri_destroy_buffer(__DRIdrawable * dPriv)
{
    struct dri_drawable *drawable = dri_drawable(dPriv);
    struct dri_screen *screen = drawable->screen;
    struct st_api *stapi = screen->st_api;
    int i;

    for (i = 0; i < ST_ATTACHMENT_COUNT; i++)
        pipe_resource_reference(&drawable->textures[i], NULL);
    for (i = 0; i < ST_ATTACHMENT_COUNT; i++)
        pipe_resource_reference(&drawable->msaa_textures[i], NULL);

    screen->base.screen->fence_reference(screen->base.screen,
                                         &drawable->throttle_fence, NULL);

    /* Notify the st manager that this drawable is no longer valid */
    stapi->destroy_drawable(stapi, &drawable->base);

    FREE(drawable->damage_rects);
    FREE(drawable);
}

/**
 * Validate the texture at an attachment.  Allocate the texture if it does not
 * exist.  Used by the TFP extension.
 */
static void
dri_drawable_validate_att(struct dri_context *ctx,
                          struct dri_drawable *drawable,
                          enum st_attachment_type statt)
{
    enum st_attachment_type statts[ST_ATTACHMENT_COUNT];
    unsigned i, count = 0;

    /* check if buffer already exists */
    if (drawable->texture_mask & (1 << statt))
        return;

    /* make sure DRI2 does not destroy existing buffers */
    for (i = 0; i < ST_ATTACHMENT_COUNT; i++) {
        if (drawable->texture_mask & (1 << i)) {
            statts[count++] = i;
        }
    }
    statts[count++] = statt;

    drawable->texture_stamp = drawable->dPriv->lastStamp - 1;

    drawable->base.validate(ctx->st, &drawable->base, statts, count, NULL);
}

/**
 * These are used for GLX_EXT_texture_from_pixmap
 */
static void
dri_set_tex_buffer2(__DRIcontext *pDRICtx, GLint target,
                    GLint format, __DRIdrawable *dPriv)
{
    struct dri_context *ctx = dri_context(pDRICtx);
    struct st_context_iface *st = ctx->st;
    struct dri_drawable *drawable = dri_drawable(dPriv);
    struct pipe_resource *pt;

    if (st->thread_finish)
        st->thread_finish(st);

    dri_drawable_validate_att(ctx, drawable, ST_ATTACHMENT_FRONT_LEFT);

    /* Use the pipe resource associated with the X drawable */
    pt = drawable->textures[ST_ATTACHMENT_FRONT_LEFT];

    if (pt) {
        enum pipe_format internal_format = pt->format;

        if (format == __DRI_TEXTURE_FORMAT_RGB)  {
            /* only need to cover the formats recognized by dri_fill_st_visual */
            switch (internal_format) {
                case PIPE_FORMAT_R16G16B16A16_FLOAT:
                    internal_format = PIPE_FORMAT_R16G16B16X16_FLOAT;
                    break;
                case PIPE_FORMAT_B10G10R10A2_UNORM:
                    internal_format = PIPE_FORMAT_B10G10R10X2_UNORM;
                    break;
                case PIPE_FORMAT_R10G10B10A2_UNORM:
                    internal_format = PIPE_FORMAT_R10G10B10X2_UNORM;
                    break;
                case PIPE_FORMAT_BGRA8888_UNORM:
                    internal_format = PIPE_FORMAT_BGRX8888_UNORM;
                    break;
                case PIPE_FORMAT_ARGB8888_UNORM:
                    internal_format = PIPE_FORMAT_XRGB8888_UNORM;
                    break;
                default:
                    break;
            }
        }

        drawable->update_tex_buffer(drawable, ctx, pt);

        ctx->st->teximage(ctx->st,
                          (target == GL_TEXTURE_2D) ? ST_TEXTURE_2D : ST_TEXTURE_RECT,
                          0, internal_format, pt, false);
    }
}

static void
dri_set_tex_buffer(__DRIcontext *pDRICtx, GLint target,
                   __DRIdrawable *dPriv)
{
    dri_set_tex_buffer2(pDRICtx, target, __DRI_TEXTURE_FORMAT_RGBA, dPriv);
}

const __DRItexBufferExtension driTexBufferExtension = {
        .base = { __DRI_TEX_BUFFER, 2 },

        .setTexBuffer       = dri_set_tex_buffer,
        .setTexBuffer2      = dri_set_tex_buffer2,
        .releaseTexBuffer   = NULL,
};

/**
 * Get the format and binding of an attachment.
 */
void
dri_drawable_get_format(struct dri_drawable *drawable,
                        enum st_attachment_type statt,
                        enum pipe_format *format,
                        unsigned *bind)
{
    switch (statt) {
        case ST_ATTACHMENT_FRONT_LEFT:
        case ST_ATTACHMENT_BACK_LEFT:
        case ST_ATTACHMENT_FRONT_RIGHT:
        case ST_ATTACHMENT_BACK_RIGHT:
            /* Other pieces of the driver stack get confused and behave incorrectly
             * when they get an sRGB drawable. st/mesa receives "drawable->stvis"
             * though other means and handles it correctly, so we don't really need
             * to use an sRGB format here.
             */
            *format = util_format_linear(drawable->stvis.color_format);
            *bind = PIPE_BIND_RENDER_TARGET | PIPE_BIND_SAMPLER_VIEW;
            break;
        case ST_ATTACHMENT_DEPTH_STENCIL:
            *format = drawable->stvis.depth_stencil_format;
            *bind = PIPE_BIND_DEPTH_STENCIL; /* XXX sampler? */
            break;
        default:
            *format = PIPE_FORMAT_NONE;
            *bind = 0;
            break;
    }
}

void
dri_pipe_blit(struct pipe_context *pipe,
              struct pipe_resource *dst,
              struct pipe_resource *src)
{
    struct pipe_blit_info blit;

    if (!dst || !src)
        return;

    /* From the GL spec, version 4.2, section 4.1.11 (Additional Multisample
     *  Fragment Operations):
     *
     *      If a framebuffer object is not bound, after all operations have
     *      been completed on the multisample buffer, the sample values for
     *      each color in the multisample buffer are combined to produce a
     *      single color value, and that value is written into the
     *      corresponding color buffers selected by DrawBuffer or
     *      DrawBuffers. An implementation may defer the writing of the color
     *      buffers until a later time, but the state of the framebuffer must
     *      behave as if the color buffers were updated as each fragment was
     *      processed. The method of combination is not specified. If the
     *      framebuffer contains sRGB values, then it is recommended that the
     *      an average of sample values is computed in a linearized space, as
     *      for blending (see section 4.1.7).
     *
     * In other words, to do a resolve operation in a linear space, we have
     * to set sRGB formats if the original resources were sRGB, so don't use
     * util_format_linear.
     */

    memset(&blit, 0, sizeof(blit));
    blit.dst.resource = dst;
    blit.dst.box.width = dst->width0;
    blit.dst.box.height = dst->height0;
    blit.dst.box.depth = 1;
    blit.dst.format = dst->format;
    blit.src.resource = src;
    blit.src.box.width = src->width0;
    blit.src.box.height = src->height0;
    blit.src.box.depth = 1;
    blit.src.format = src->format;
    blit.mask = PIPE_MASK_RGBA;
    blit.filter = PIPE_TEX_FILTER_NEAREST;

    pipe->blit(pipe, &blit);
}

static void
dri_postprocessing(struct dri_context *ctx,
                   struct dri_drawable *drawable,
                   enum st_attachment_type att)
{
    struct pipe_resource *src = drawable->textures[att];
    struct pipe_resource *zsbuf = drawable->textures[ST_ATTACHMENT_DEPTH_STENCIL];

    if (ctx->pp && src)
        pp_run(ctx->pp, src, src, zsbuf);
}

struct notify_before_flush_cb_args {
    struct dri_context *ctx;
    struct dri_drawable *drawable;
    unsigned flags;
    enum __DRI2throttleReason reason;
    bool swap_msaa_buffers;
};

static void
notify_before_flush_cb(void* _args)
{
    struct notify_before_flush_cb_args *args = (struct notify_before_flush_cb_args *) _args;
    struct st_context_iface *st = args->ctx->st;
    struct pipe_context *pipe = st->pipe;

    if (args->drawable->stvis.samples > 1 &&
        (args->reason == __DRI2_THROTTLE_SWAPBUFFER ||
         args->reason == __DRI2_THROTTLE_COPYSUBBUFFER)) {
        /* Resolve the MSAA back buffer. */
        dri_pipe_blit(st->pipe,
                      args->drawable->textures[ST_ATTACHMENT_BACK_LEFT],
                      args->drawable->msaa_textures[ST_ATTACHMENT_BACK_LEFT]);

        if (args->reason == __DRI2_THROTTLE_SWAPBUFFER &&
            args->drawable->msaa_textures[ST_ATTACHMENT_FRONT_LEFT] &&
            args->drawable->msaa_textures[ST_ATTACHMENT_BACK_LEFT]) {
            args->swap_msaa_buffers = true;
        }

        /* FRONT_LEFT is resolved in drawable->flush_frontbuffer. */
    }

    dri_postprocessing(args->ctx, args->drawable, ST_ATTACHMENT_BACK_LEFT);

    if (pipe->invalidate_resource &&
        (args->flags & __DRI2_FLUSH_INVALIDATE_ANCILLARY)) {
        if (args->drawable->textures[ST_ATTACHMENT_DEPTH_STENCIL])
            pipe->invalidate_resource(pipe, args->drawable->textures[ST_ATTACHMENT_DEPTH_STENCIL]);
        if (args->drawable->msaa_textures[ST_ATTACHMENT_DEPTH_STENCIL])
            pipe->invalidate_resource(pipe, args->drawable->msaa_textures[ST_ATTACHMENT_DEPTH_STENCIL]);
    }

    if (args->ctx->hud) {
        hud_run(args->ctx->hud, args->ctx->st->cso_context,
                args->drawable->textures[ST_ATTACHMENT_BACK_LEFT]);
    }

    pipe->flush_resource(pipe, args->drawable->textures[ST_ATTACHMENT_BACK_LEFT]);
}

/**
 * DRI2 flush extension, the flush_with_flags function.
 *
 * \param context           the context
 * \param drawable          the drawable to flush
 * \param flags             a combination of _DRI2_FLUSH_xxx flags
 * \param throttle_reason   the reason for throttling, 0 = no throttling
 */
void
dri_flush(__DRIcontext *cPriv,
          __DRIdrawable *dPriv,
          unsigned flags,
          enum __DRI2throttleReason reason)
{
    struct dri_context *ctx = dri_context(cPriv);
    struct dri_drawable *drawable = dri_drawable(dPriv);
    struct st_context_iface *st;
    unsigned flush_flags;
    struct notify_before_flush_cb_args args = { 0 };

    if (!ctx) {
        assert(0);
        return;
    }

    st = ctx->st;
    if (st->thread_finish)
        st->thread_finish(st);

    if (drawable) {
        /* prevent recursion */
        if (drawable->flushing)
            return;

        drawable->flushing = true;
    }
    else {
        flags &= ~__DRI2_FLUSH_DRAWABLE;
    }

    if ((flags & __DRI2_FLUSH_DRAWABLE) &&
        drawable->textures[ST_ATTACHMENT_BACK_LEFT]) {
        /* We can't do operations on the back buffer here, because there
         * may be some pending operations that will get flushed by the
         * call to st->flush (eg: FLUSH_VERTICES).
         * Instead we register a callback to be notified when all operations
         * have been submitted but before the call to st_flush.
         */
        args.ctx = ctx;
        args.drawable = drawable;
        args.flags = flags;
        args.reason = reason;
    }

    flush_flags = 0;
    if (flags & __DRI2_FLUSH_CONTEXT)
        flush_flags |= ST_FLUSH_FRONT;
    if (reason == __DRI2_THROTTLE_SWAPBUFFER)
        flush_flags |= ST_FLUSH_END_OF_FRAME;

    /* Flush the context and throttle if needed. */
    if (dri_screen(ctx->sPriv)->throttle &&
        drawable &&
        (reason == __DRI2_THROTTLE_SWAPBUFFER ||
         reason == __DRI2_THROTTLE_FLUSHFRONT)) {

        struct pipe_screen *screen = drawable->screen->base.screen;
        struct pipe_fence_handle *new_fence = NULL;

        st->flush(st, flush_flags, &new_fence, args.ctx ? notify_before_flush_cb : NULL, &args);

        /* throttle on the previous fence */
        if (drawable->throttle_fence) {
            screen->fence_finish(screen, NULL, drawable->throttle_fence, PIPE_TIMEOUT_INFINITE);
            screen->fence_reference(screen, &drawable->throttle_fence, NULL);
        }
        drawable->throttle_fence = new_fence;
    }
    else if (flags & (__DRI2_FLUSH_DRAWABLE | __DRI2_FLUSH_CONTEXT)) {
        st->flush(st, flush_flags, NULL, args.ctx ? notify_before_flush_cb : NULL, &args);
    }

    if (drawable) {
        drawable->flushing = false;
    }

    /* Swap the MSAA front and back buffers, so that reading
     * from the front buffer after SwapBuffers returns what was
     * in the back buffer.
     */
    if (args.swap_msaa_buffers) {
        struct pipe_resource *tmp =
                drawable->msaa_textures[ST_ATTACHMENT_FRONT_LEFT];

        drawable->msaa_textures[ST_ATTACHMENT_FRONT_LEFT] =
                drawable->msaa_textures[ST_ATTACHMENT_BACK_LEFT];
        drawable->msaa_textures[ST_ATTACHMENT_BACK_LEFT] = tmp;

        /* Now that we have swapped the buffers, this tells the gallium
         * frontend to revalidate the framebuffer.
         */
        p_atomic_inc(&drawable->base.stamp);
    }
}

/**
 * dri_throttle - A DRI2ThrottleExtension throttling function.
 */
static void
dri_throttle(__DRIcontext *cPriv, __DRIdrawable *dPriv,
             enum __DRI2throttleReason reason)
{
    dri_flush(cPriv, dPriv, 0, reason);
}


const __DRI2throttleExtension dri2ThrottleExtension = {
        .base = { __DRI2_THROTTLE, 1 },

        .throttle          = dri_throttle,
};


/* vim: set sw=3 ts=8 sts=3 expandtab: */
