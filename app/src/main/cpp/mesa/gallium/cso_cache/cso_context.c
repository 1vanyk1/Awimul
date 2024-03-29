#include "../../include/pipe/p_state.h"
#include "../../util/u_draw.h"
#include "../../util/u_framebuffer.h"
#include "../../util/u_inlines.h"
#include "../../util/u_math.h"
#include "../../util/u_memory.h"
#include "../../util/u_vbuf.h"
#include "../tgsi/tgsi_parse.h"

#include "cso_context.h"
#include "cso_cache.h"
#include "cso_hash.h"

/**
 * Per-shader sampler information.
 */
struct sampler_info
{
    struct cso_sampler *cso_samplers[PIPE_MAX_SAMPLERS];
    void *samplers[PIPE_MAX_SAMPLERS];
};



struct cso_context {
    struct pipe_context *pipe;

    struct u_vbuf *vbuf;
    struct u_vbuf *vbuf_current;
    bool always_use_vbuf;

    boolean has_geometry_shader;
    boolean has_tessellation;
    boolean has_compute_shader;
    boolean has_streamout;

    unsigned saved_state;  /**< bitmask of CSO_BIT_x flags */

    struct sampler_info fragment_samplers_saved;
    struct sampler_info samplers[PIPE_SHADER_TYPES];

    /* Temporary number until cso_single_sampler_done is called.
     * It tracks the highest sampler seen in cso_single_sampler.
     */
    int max_sampler_seen;

    unsigned nr_so_targets;
    struct pipe_stream_output_target *so_targets[PIPE_MAX_SO_BUFFERS];

    unsigned nr_so_targets_saved;
    struct pipe_stream_output_target *so_targets_saved[PIPE_MAX_SO_BUFFERS];

    /** Current and saved state.
     * The saved state is used as a 1-deep stack.
     */
    void *blend, *blend_saved;
    void *depth_stencil, *depth_stencil_saved;
    void *rasterizer, *rasterizer_saved;
    void *fragment_shader, *fragment_shader_saved;
    void *vertex_shader, *vertex_shader_saved;
    void *geometry_shader, *geometry_shader_saved;
    void *tessctrl_shader, *tessctrl_shader_saved;
    void *tesseval_shader, *tesseval_shader_saved;
    void *compute_shader;
    void *velements, *velements_saved;
    struct pipe_query *render_condition, *render_condition_saved;
    uint render_condition_mode, render_condition_mode_saved;
    boolean render_condition_cond, render_condition_cond_saved;

    struct pipe_framebuffer_state fb, fb_saved;
    struct pipe_viewport_state vp, vp_saved;
    unsigned sample_mask, sample_mask_saved;
    unsigned min_samples, min_samples_saved;
    struct pipe_stencil_ref stencil_ref, stencil_ref_saved;

    /* This should be last to keep all of the above together in memory. */
    struct cso_cache cache;
};

struct pipe_context *cso_get_pipe_context(struct cso_context *cso)
{
    return cso->pipe;
}

static inline boolean delete_cso(struct cso_context *ctx,
                                 void *state, enum cso_cache_type type)
{
    switch (type) {
        case CSO_BLEND:
            if (ctx->blend == ((struct cso_blend*)state)->data)
                return false;
            break;
        case CSO_DEPTH_STENCIL_ALPHA:
            if (ctx->depth_stencil == ((struct cso_depth_stencil_alpha*)state)->data)
                return false;
            break;
        case CSO_RASTERIZER:
            if (ctx->rasterizer == ((struct cso_rasterizer*)state)->data)
                return false;
            break;
        case CSO_VELEMENTS:
            if (ctx->velements == ((struct cso_velements*)state)->data)
                return false;
            break;
        case CSO_SAMPLER:
            /* nothing to do for samplers */
            break;
        default:
            assert(0);
    }

    cso_delete_state(ctx->pipe, state, type);
    return true;
}

static inline void
sanitize_hash(struct cso_hash *hash, enum cso_cache_type type,
              int max_size, void *user_data)
{
    struct cso_context *ctx = (struct cso_context *)user_data;
    /* if we're approach the maximum size, remove fourth of the entries
     * otherwise every subsequent call will go through the same */
    int hash_size = cso_hash_size(hash);
    int max_entries = (max_size > hash_size) ? max_size : hash_size;
    int to_remove =  (max_size < max_entries) * max_entries/4;
    struct cso_hash_iter iter;
    struct cso_sampler **samplers_to_restore = NULL;
    unsigned to_restore = 0;

    if (hash_size > max_size)
        to_remove += hash_size - max_size;

    if (to_remove == 0)
        return;

    if (type == CSO_SAMPLER) {
        int i, j;

        samplers_to_restore = MALLOC(PIPE_SHADER_TYPES * PIPE_MAX_SAMPLERS *
                                     sizeof(*samplers_to_restore));

        /* Temporarily remove currently bound sampler states from the hash
         * table, to prevent them from being deleted
         */
        for (i = 0; i < PIPE_SHADER_TYPES; i++) {
            for (j = 0; j < PIPE_MAX_SAMPLERS; j++) {
                struct cso_sampler *sampler = ctx->samplers[i].cso_samplers[j];

                if (sampler && cso_hash_take(hash, sampler->hash_key))
                    samplers_to_restore[to_restore++] = sampler;
            }
        }
    }

    iter = cso_hash_first_node(hash);
    while (to_remove) {
        /*remove elements until we're good */
        /*fixme: currently we pick the nodes to remove at random*/
        void *cso = cso_hash_iter_data(iter);

        if (!cso)
            break;

        if (delete_cso(ctx, cso, type)) {
            iter = cso_hash_erase(hash, iter);
            --to_remove;
        } else
            iter = cso_hash_iter_next(iter);
    }

    if (type == CSO_SAMPLER) {
        /* Put currently bound sampler states back into the hash table */
        while (to_restore--) {
            struct cso_sampler *sampler = samplers_to_restore[to_restore];

            cso_hash_insert(hash, sampler->hash_key, sampler);
        }

        FREE(samplers_to_restore);
    }
}

static void cso_init_vbuf(struct cso_context *cso, unsigned flags)
{
    struct u_vbuf_caps caps;
    bool uses_user_vertex_buffers = !(flags & CSO_NO_USER_VERTEX_BUFFERS);
    bool needs64b = !(flags & CSO_NO_64B_VERTEX_BUFFERS);

    u_vbuf_get_caps(cso->pipe->screen, &caps, needs64b);

    /* Enable u_vbuf if needed. */
    if (caps.fallback_always ||
        (uses_user_vertex_buffers &&
         caps.fallback_only_for_user_vbuffers)) {
        cso->vbuf = u_vbuf_create(cso->pipe, &caps);
        cso->vbuf_current = cso->vbuf;
        cso->always_use_vbuf = caps.fallback_always;
    }
}

struct cso_context *
cso_create_context(struct pipe_context *pipe, unsigned flags)
{
    struct cso_context *ctx = CALLOC_STRUCT(cso_context);
    if (!ctx)
        return NULL;

    cso_cache_init(&ctx->cache, pipe);
    cso_cache_set_sanitize_callback(&ctx->cache, sanitize_hash, ctx);

    ctx->pipe = pipe;
    ctx->sample_mask = ~0;

    cso_init_vbuf(ctx, flags);

    /* Enable for testing: */
    if (0) cso_set_maximum_cache_size(&ctx->cache, 4);

    if (pipe->screen->get_shader_param(pipe->screen, PIPE_SHADER_GEOMETRY,
                                       PIPE_SHADER_CAP_MAX_INSTRUCTIONS) > 0) {
        ctx->has_geometry_shader = TRUE;
    }
    if (pipe->screen->get_shader_param(pipe->screen, PIPE_SHADER_TESS_CTRL,
                                       PIPE_SHADER_CAP_MAX_INSTRUCTIONS) > 0) {
        ctx->has_tessellation = TRUE;
    }
    if (pipe->screen->get_shader_param(pipe->screen, PIPE_SHADER_COMPUTE,
                                       PIPE_SHADER_CAP_MAX_INSTRUCTIONS) > 0) {
        int supported_irs =
                pipe->screen->get_shader_param(pipe->screen, PIPE_SHADER_COMPUTE,
                                               PIPE_SHADER_CAP_SUPPORTED_IRS);
        if (supported_irs & ((1 << PIPE_SHADER_IR_TGSI) |
                             (1 << PIPE_SHADER_IR_NIR))) {
            ctx->has_compute_shader = TRUE;
        }
    }
    if (pipe->screen->get_param(pipe->screen,
                                PIPE_CAP_MAX_STREAM_OUTPUT_BUFFERS) != 0) {
        ctx->has_streamout = TRUE;
    }

    ctx->max_sampler_seen = -1;
    return ctx;
}

/**
 * Free the CSO context.
 */
void cso_destroy_context( struct cso_context *ctx )
{
    unsigned i;

    if (ctx->pipe) {
        ctx->pipe->bind_blend_state( ctx->pipe, NULL );
        ctx->pipe->bind_rasterizer_state( ctx->pipe, NULL );

        {
            static struct pipe_sampler_view *views[PIPE_MAX_SHADER_SAMPLER_VIEWS] = { NULL };
            static struct pipe_shader_buffer ssbos[PIPE_MAX_SHADER_BUFFERS] = { 0 };
            static void *zeros[PIPE_MAX_SAMPLERS] = { NULL };
            struct pipe_screen *scr = ctx->pipe->screen;
            enum pipe_shader_type sh;
            for (sh = 0; sh < PIPE_SHADER_TYPES; sh++) {
                switch (sh) {
                    case PIPE_SHADER_GEOMETRY:
                        if (!ctx->has_geometry_shader)
                            continue;
                        break;
                    case PIPE_SHADER_TESS_CTRL:
                    case PIPE_SHADER_TESS_EVAL:
                        if (!ctx->has_tessellation)
                            continue;
                        break;
                    case PIPE_SHADER_COMPUTE:
                        if (!ctx->has_compute_shader)
                            continue;
                        break;
                    default:
                        break;
                }

                int maxsam = scr->get_shader_param(scr, sh,
                                                   PIPE_SHADER_CAP_MAX_TEXTURE_SAMPLERS);
                int maxview = scr->get_shader_param(scr, sh,
                                                    PIPE_SHADER_CAP_MAX_SAMPLER_VIEWS);
                int maxssbo = scr->get_shader_param(scr, sh,
                                                    PIPE_SHADER_CAP_MAX_SHADER_BUFFERS);
                int maxcb = scr->get_shader_param(scr, sh,
                                                  PIPE_SHADER_CAP_MAX_CONST_BUFFERS);
                int maximg = scr->get_shader_param(scr, sh,
                                                   PIPE_SHADER_CAP_MAX_SHADER_IMAGES);
                assert(maxsam <= PIPE_MAX_SAMPLERS);
                assert(maxview <= PIPE_MAX_SHADER_SAMPLER_VIEWS);
                assert(maxssbo <= PIPE_MAX_SHADER_BUFFERS);
                assert(maxcb <= PIPE_MAX_CONSTANT_BUFFERS);
                assert(maximg <= PIPE_MAX_SHADER_IMAGES);
                if (maxsam > 0) {
                    ctx->pipe->bind_sampler_states(ctx->pipe, sh, 0, maxsam, zeros);
                }
                if (maxview > 0) {
                    ctx->pipe->set_sampler_views(ctx->pipe, sh, 0, maxview, 0, views);
                }
                if (maxssbo > 0) {
                    ctx->pipe->set_shader_buffers(ctx->pipe, sh, 0, maxssbo, ssbos, 0);
                }
                if (maximg > 0) {
                    ctx->pipe->set_shader_images(ctx->pipe, sh, 0, 0, maximg, NULL);
                }
                for (int i = 0; i < maxcb; i++) {
                    ctx->pipe->set_constant_buffer(ctx->pipe, sh, i, false, NULL);
                }
            }
        }

        ctx->pipe->bind_depth_stencil_alpha_state( ctx->pipe, NULL );
        ctx->pipe->bind_fs_state( ctx->pipe, NULL );
        ctx->pipe->set_constant_buffer(ctx->pipe, PIPE_SHADER_FRAGMENT, 0, false, NULL);
        ctx->pipe->bind_vs_state( ctx->pipe, NULL );
        ctx->pipe->set_constant_buffer(ctx->pipe, PIPE_SHADER_VERTEX, 0, false, NULL);
        if (ctx->has_geometry_shader) {
            ctx->pipe->bind_gs_state(ctx->pipe, NULL);
        }
        if (ctx->has_tessellation) {
            ctx->pipe->bind_tcs_state(ctx->pipe, NULL);
            ctx->pipe->bind_tes_state(ctx->pipe, NULL);
        }
        if (ctx->has_compute_shader) {
            ctx->pipe->bind_compute_state(ctx->pipe, NULL);
        }
        ctx->pipe->bind_vertex_elements_state( ctx->pipe, NULL );

        if (ctx->has_streamout)
            ctx->pipe->set_stream_output_targets(ctx->pipe, 0, NULL, NULL);
    }

    util_unreference_framebuffer_state(&ctx->fb);
    util_unreference_framebuffer_state(&ctx->fb_saved);

    for (i = 0; i < PIPE_MAX_SO_BUFFERS; i++) {
        pipe_so_target_reference(&ctx->so_targets[i], NULL);
        pipe_so_target_reference(&ctx->so_targets_saved[i], NULL);
    }

    cso_cache_delete(&ctx->cache);

    if (ctx->vbuf)
        u_vbuf_destroy(ctx->vbuf);
    FREE( ctx );
}


/* Those function will either find the state of the given template
 * in the cache or they will create a new state from the given
 * template, insert it in the cache and return it.
 */

/*
 * If the driver returns 0 from the create method then they will assign
 * the data member of the cso to be the template itself.
 */

enum pipe_error cso_set_blend(struct cso_context *ctx,
                              const struct pipe_blend_state *templ)
{
    unsigned key_size, hash_key;
    struct cso_hash_iter iter;
    void *handle;

    key_size = templ->independent_blend_enable ?
               sizeof(struct pipe_blend_state) :
               (char *)&(templ->rt[1]) - (char *)templ;
    hash_key = cso_construct_key((void*)templ, key_size);
    iter = cso_find_state_template(&ctx->cache, hash_key, CSO_BLEND,
                                   (void*)templ, key_size);

    if (cso_hash_iter_is_null(iter)) {
        struct cso_blend *cso = MALLOC(sizeof(struct cso_blend));
        if (!cso)
            return PIPE_ERROR_OUT_OF_MEMORY;

        memset(&cso->state, 0, sizeof cso->state);
        memcpy(&cso->state, templ, key_size);
        cso->data = ctx->pipe->create_blend_state(ctx->pipe, &cso->state);

        iter = cso_insert_state(&ctx->cache, hash_key, CSO_BLEND, cso);
        if (cso_hash_iter_is_null(iter)) {
            FREE(cso);
            return PIPE_ERROR_OUT_OF_MEMORY;
        }

        handle = cso->data;
    }
    else {
        handle = ((struct cso_blend *)cso_hash_iter_data(iter))->data;
    }

    if (ctx->blend != handle) {
        ctx->blend = handle;
        ctx->pipe->bind_blend_state(ctx->pipe, handle);
    }
    return PIPE_OK;
}

static void
cso_save_blend(struct cso_context *ctx)
{
    assert(!ctx->blend_saved);
    ctx->blend_saved = ctx->blend;
}

static void
cso_restore_blend(struct cso_context *ctx)
{
    if (ctx->blend != ctx->blend_saved) {
        ctx->blend = ctx->blend_saved;
        ctx->pipe->bind_blend_state(ctx->pipe, ctx->blend_saved);
    }
    ctx->blend_saved = NULL;
}



enum pipe_error
cso_set_depth_stencil_alpha(struct cso_context *ctx,
                            const struct pipe_depth_stencil_alpha_state *templ)
{
    unsigned key_size = sizeof(struct pipe_depth_stencil_alpha_state);
    unsigned hash_key = cso_construct_key((void*)templ, key_size);
    struct cso_hash_iter iter = cso_find_state_template(&ctx->cache,
                                                        hash_key,
                                                        CSO_DEPTH_STENCIL_ALPHA,
                                                        (void*)templ, key_size);
    void *handle;

    if (cso_hash_iter_is_null(iter)) {
        struct cso_depth_stencil_alpha *cso =
                MALLOC(sizeof(struct cso_depth_stencil_alpha));
        if (!cso)
            return PIPE_ERROR_OUT_OF_MEMORY;

        memcpy(&cso->state, templ, sizeof(*templ));
        cso->data = ctx->pipe->create_depth_stencil_alpha_state(ctx->pipe,
                                                                &cso->state);

        iter = cso_insert_state(&ctx->cache, hash_key,
                                CSO_DEPTH_STENCIL_ALPHA, cso);
        if (cso_hash_iter_is_null(iter)) {
            FREE(cso);
            return PIPE_ERROR_OUT_OF_MEMORY;
        }

        handle = cso->data;
    }
    else {
        handle = ((struct cso_depth_stencil_alpha *)
                cso_hash_iter_data(iter))->data;
    }

    if (ctx->depth_stencil != handle) {
        ctx->depth_stencil = handle;
        ctx->pipe->bind_depth_stencil_alpha_state(ctx->pipe, handle);
    }
    return PIPE_OK;
}

static void
cso_save_depth_stencil_alpha(struct cso_context *ctx)
{
    assert(!ctx->depth_stencil_saved);
    ctx->depth_stencil_saved = ctx->depth_stencil;
}

static void
cso_restore_depth_stencil_alpha(struct cso_context *ctx)
{
    if (ctx->depth_stencil != ctx->depth_stencil_saved) {
        ctx->depth_stencil = ctx->depth_stencil_saved;
        ctx->pipe->bind_depth_stencil_alpha_state(ctx->pipe,
                                                  ctx->depth_stencil_saved);
    }
    ctx->depth_stencil_saved = NULL;
}



enum pipe_error cso_set_rasterizer(struct cso_context *ctx,
                                   const struct pipe_rasterizer_state *templ)
{
    unsigned key_size = sizeof(struct pipe_rasterizer_state);
    unsigned hash_key = cso_construct_key((void*)templ, key_size);
    struct cso_hash_iter iter = cso_find_state_template(&ctx->cache,
                                                        hash_key,
                                                        CSO_RASTERIZER,
                                                        (void*)templ, key_size);
    void *handle = NULL;

    /* We can't have both point_quad_rasterization (sprites) and point_smooth
     * (round AA points) enabled at the same time.
     */
    assert(!(templ->point_quad_rasterization && templ->point_smooth));

    if (cso_hash_iter_is_null(iter)) {
        struct cso_rasterizer *cso = MALLOC(sizeof(struct cso_rasterizer));
        if (!cso)
            return PIPE_ERROR_OUT_OF_MEMORY;

        memcpy(&cso->state, templ, sizeof(*templ));
        cso->data = ctx->pipe->create_rasterizer_state(ctx->pipe, &cso->state);

        iter = cso_insert_state(&ctx->cache, hash_key, CSO_RASTERIZER, cso);
        if (cso_hash_iter_is_null(iter)) {
            FREE(cso);
            return PIPE_ERROR_OUT_OF_MEMORY;
        }

        handle = cso->data;
    }
    else {
        handle = ((struct cso_rasterizer *)cso_hash_iter_data(iter))->data;
    }

    if (ctx->rasterizer != handle) {
        ctx->rasterizer = handle;
        ctx->pipe->bind_rasterizer_state(ctx->pipe, handle);
    }
    return PIPE_OK;
}

static void
cso_save_rasterizer(struct cso_context *ctx)
{
    assert(!ctx->rasterizer_saved);
    ctx->rasterizer_saved = ctx->rasterizer;
}

static void
cso_restore_rasterizer(struct cso_context *ctx)
{
    if (ctx->rasterizer != ctx->rasterizer_saved) {
        ctx->rasterizer = ctx->rasterizer_saved;
        ctx->pipe->bind_rasterizer_state(ctx->pipe, ctx->rasterizer_saved);
    }
    ctx->rasterizer_saved = NULL;
}


void cso_set_fragment_shader_handle(struct cso_context *ctx, void *handle )
{
    if (ctx->fragment_shader != handle) {
        ctx->fragment_shader = handle;
        ctx->pipe->bind_fs_state(ctx->pipe, handle);
    }
}

static void
cso_save_fragment_shader(struct cso_context *ctx)
{
    assert(!ctx->fragment_shader_saved);
    ctx->fragment_shader_saved = ctx->fragment_shader;
}

static void
cso_restore_fragment_shader(struct cso_context *ctx)
{
    if (ctx->fragment_shader_saved != ctx->fragment_shader) {
        ctx->pipe->bind_fs_state(ctx->pipe, ctx->fragment_shader_saved);
        ctx->fragment_shader = ctx->fragment_shader_saved;
    }
    ctx->fragment_shader_saved = NULL;
}


void cso_set_vertex_shader_handle(struct cso_context *ctx, void *handle)
{
    if (ctx->vertex_shader != handle) {
        ctx->vertex_shader = handle;
        ctx->pipe->bind_vs_state(ctx->pipe, handle);
    }
}

static void
cso_save_vertex_shader(struct cso_context *ctx)
{
    assert(!ctx->vertex_shader_saved);
    ctx->vertex_shader_saved = ctx->vertex_shader;
}

static void
cso_restore_vertex_shader(struct cso_context *ctx)
{
    if (ctx->vertex_shader_saved != ctx->vertex_shader) {
        ctx->pipe->bind_vs_state(ctx->pipe, ctx->vertex_shader_saved);
        ctx->vertex_shader = ctx->vertex_shader_saved;
    }
    ctx->vertex_shader_saved = NULL;
}


void cso_set_framebuffer(struct cso_context *ctx,
                         const struct pipe_framebuffer_state *fb)
{
    if (memcmp(&ctx->fb, fb, sizeof(*fb)) != 0) {
        util_copy_framebuffer_state(&ctx->fb, fb);
        ctx->pipe->set_framebuffer_state(ctx->pipe, fb);
    }
}

static void
cso_save_framebuffer(struct cso_context *ctx)
{
    util_copy_framebuffer_state(&ctx->fb_saved, &ctx->fb);
}

static void
cso_restore_framebuffer(struct cso_context *ctx)
{
    if (memcmp(&ctx->fb, &ctx->fb_saved, sizeof(ctx->fb))) {
        util_copy_framebuffer_state(&ctx->fb, &ctx->fb_saved);
        ctx->pipe->set_framebuffer_state(ctx->pipe, &ctx->fb);
        util_unreference_framebuffer_state(&ctx->fb_saved);
    }
}


void cso_set_viewport(struct cso_context *ctx,
                      const struct pipe_viewport_state *vp)
{
    if (memcmp(&ctx->vp, vp, sizeof(*vp))) {
        ctx->vp = *vp;
        ctx->pipe->set_viewport_states(ctx->pipe, 0, 1, vp);
    }
}

/**
 * Setup viewport state for given width and height (position is always (0,0)).
 * Invert the Y axis if 'invert' is true.
 */
void
cso_set_viewport_dims(struct cso_context *ctx,
                      float width, float height, boolean invert)
{
    struct pipe_viewport_state vp;
    vp.scale[0] = width * 0.5f;
    vp.scale[1] = height * (invert ? -0.5f : 0.5f);
    vp.scale[2] = 0.5f;
    vp.translate[0] = 0.5f * width;
    vp.translate[1] = 0.5f * height;
    vp.translate[2] = 0.5f;
    vp.swizzle_x = PIPE_VIEWPORT_SWIZZLE_POSITIVE_X;
    vp.swizzle_y = PIPE_VIEWPORT_SWIZZLE_POSITIVE_Y;
    vp.swizzle_z = PIPE_VIEWPORT_SWIZZLE_POSITIVE_Z;
    vp.swizzle_w = PIPE_VIEWPORT_SWIZZLE_POSITIVE_W;
    cso_set_viewport(ctx, &vp);
}

static void
cso_save_viewport(struct cso_context *ctx)
{
    ctx->vp_saved = ctx->vp;
}


static void
cso_restore_viewport(struct cso_context *ctx)
{
    if (memcmp(&ctx->vp, &ctx->vp_saved, sizeof(ctx->vp))) {
        ctx->vp = ctx->vp_saved;
        ctx->pipe->set_viewport_states(ctx->pipe, 0, 1, &ctx->vp);
    }
}

void cso_set_sample_mask(struct cso_context *ctx, unsigned sample_mask)
{
    if (ctx->sample_mask != sample_mask) {
        ctx->sample_mask = sample_mask;
        ctx->pipe->set_sample_mask(ctx->pipe, sample_mask);
    }
}

static void
cso_save_sample_mask(struct cso_context *ctx)
{
    ctx->sample_mask_saved = ctx->sample_mask;
}

static void
cso_restore_sample_mask(struct cso_context *ctx)
{
    cso_set_sample_mask(ctx, ctx->sample_mask_saved);
}

void cso_set_min_samples(struct cso_context *ctx, unsigned min_samples)
{
    if (ctx->min_samples != min_samples && ctx->pipe->set_min_samples) {
        ctx->min_samples = min_samples;
        ctx->pipe->set_min_samples(ctx->pipe, min_samples);
    }
}

static void
cso_save_min_samples(struct cso_context *ctx)
{
    ctx->min_samples_saved = ctx->min_samples;
}

static void
cso_restore_min_samples(struct cso_context *ctx)
{
    cso_set_min_samples(ctx, ctx->min_samples_saved);
}

void cso_set_stencil_ref(struct cso_context *ctx,
                         const struct pipe_stencil_ref sr)
{
    if (memcmp(&ctx->stencil_ref, &sr, sizeof(ctx->stencil_ref))) {
        ctx->stencil_ref = sr;
        ctx->pipe->set_stencil_ref(ctx->pipe, sr);
    }
}

static void
cso_save_stencil_ref(struct cso_context *ctx)
{
    ctx->stencil_ref_saved = ctx->stencil_ref;
}


static void
cso_restore_stencil_ref(struct cso_context *ctx)
{
    if (memcmp(&ctx->stencil_ref, &ctx->stencil_ref_saved,
               sizeof(ctx->stencil_ref))) {
        ctx->stencil_ref = ctx->stencil_ref_saved;
        ctx->pipe->set_stencil_ref(ctx->pipe, ctx->stencil_ref);
    }
}

void cso_set_render_condition(struct cso_context *ctx,
                              struct pipe_query *query,
                              boolean condition,
                              enum pipe_render_cond_flag mode)
{
    struct pipe_context *pipe = ctx->pipe;

    if (ctx->render_condition != query ||
        ctx->render_condition_mode != mode ||
        ctx->render_condition_cond != condition) {
        pipe->render_condition(pipe, query, condition, mode);
        ctx->render_condition = query;
        ctx->render_condition_cond = condition;
        ctx->render_condition_mode = mode;
    }
}

static void
cso_save_render_condition(struct cso_context *ctx)
{
    ctx->render_condition_saved = ctx->render_condition;
    ctx->render_condition_cond_saved = ctx->render_condition_cond;
    ctx->render_condition_mode_saved = ctx->render_condition_mode;
}

static void
cso_restore_render_condition(struct cso_context *ctx)
{
    cso_set_render_condition(ctx, ctx->render_condition_saved,
                             ctx->render_condition_cond_saved,
                             ctx->render_condition_mode_saved);
}

void cso_set_geometry_shader_handle(struct cso_context *ctx, void *handle)
{
    assert(ctx->has_geometry_shader || !handle);

    if (ctx->has_geometry_shader && ctx->geometry_shader != handle) {
        ctx->geometry_shader = handle;
        ctx->pipe->bind_gs_state(ctx->pipe, handle);
    }
}

static void
cso_save_geometry_shader(struct cso_context *ctx)
{
    if (!ctx->has_geometry_shader) {
        return;
    }

    assert(!ctx->geometry_shader_saved);
    ctx->geometry_shader_saved = ctx->geometry_shader;
}

static void
cso_restore_geometry_shader(struct cso_context *ctx)
{
    if (!ctx->has_geometry_shader) {
        return;
    }

    if (ctx->geometry_shader_saved != ctx->geometry_shader) {
        ctx->pipe->bind_gs_state(ctx->pipe, ctx->geometry_shader_saved);
        ctx->geometry_shader = ctx->geometry_shader_saved;
    }
    ctx->geometry_shader_saved = NULL;
}

void cso_set_tessctrl_shader_handle(struct cso_context *ctx, void *handle)
{
    assert(ctx->has_tessellation || !handle);

    if (ctx->has_tessellation && ctx->tessctrl_shader != handle) {
        ctx->tessctrl_shader = handle;
        ctx->pipe->bind_tcs_state(ctx->pipe, handle);
    }
}

static void
cso_save_tessctrl_shader(struct cso_context *ctx)
{
    if (!ctx->has_tessellation) {
        return;
    }

    assert(!ctx->tessctrl_shader_saved);
    ctx->tessctrl_shader_saved = ctx->tessctrl_shader;
}

static void
cso_restore_tessctrl_shader(struct cso_context *ctx)
{
    if (!ctx->has_tessellation) {
        return;
    }

    if (ctx->tessctrl_shader_saved != ctx->tessctrl_shader) {
        ctx->pipe->bind_tcs_state(ctx->pipe, ctx->tessctrl_shader_saved);
        ctx->tessctrl_shader = ctx->tessctrl_shader_saved;
    }
    ctx->tessctrl_shader_saved = NULL;
}

void cso_set_tesseval_shader_handle(struct cso_context *ctx, void *handle)
{
    assert(ctx->has_tessellation || !handle);

    if (ctx->has_tessellation && ctx->tesseval_shader != handle) {
        ctx->tesseval_shader = handle;
        ctx->pipe->bind_tes_state(ctx->pipe, handle);
    }
}

static void
cso_save_tesseval_shader(struct cso_context *ctx)
{
    if (!ctx->has_tessellation) {
        return;
    }

    assert(!ctx->tesseval_shader_saved);
    ctx->tesseval_shader_saved = ctx->tesseval_shader;
}

static void
cso_restore_tesseval_shader(struct cso_context *ctx)
{
    if (!ctx->has_tessellation) {
        return;
    }

    if (ctx->tesseval_shader_saved != ctx->tesseval_shader) {
        ctx->pipe->bind_tes_state(ctx->pipe, ctx->tesseval_shader_saved);
        ctx->tesseval_shader = ctx->tesseval_shader_saved;
    }
    ctx->tesseval_shader_saved = NULL;
}

void cso_set_compute_shader_handle(struct cso_context *ctx, void *handle)
{
    assert(ctx->has_compute_shader || !handle);

    if (ctx->has_compute_shader && ctx->compute_shader != handle) {
        ctx->compute_shader = handle;
        ctx->pipe->bind_compute_state(ctx->pipe, handle);
    }
}

static void
cso_set_vertex_elements_direct(struct cso_context *ctx,
                               const struct cso_velems_state *velems)
{
    unsigned key_size, hash_key;
    struct cso_hash_iter iter;
    void *handle;

    /* Need to include the count into the stored state data too.
     * Otherwise first few count pipe_vertex_elements could be identical
     * even if count is different, and there's no guarantee the hash would
     * be different in that case neither.
     */
    key_size = sizeof(struct pipe_vertex_element) * velems->count +
               sizeof(unsigned);
    hash_key = cso_construct_key((void*)velems, key_size);
    iter = cso_find_state_template(&ctx->cache, hash_key, CSO_VELEMENTS,
                                   (void*)velems, key_size);

    if (cso_hash_iter_is_null(iter)) {
        struct cso_velements *cso = MALLOC(sizeof(struct cso_velements));
        if (!cso)
            return;

        memcpy(&cso->state, velems, key_size);
        cso->data = ctx->pipe->create_vertex_elements_state(ctx->pipe,
                                                            velems->count,
                                                            &cso->state.velems[0]);

        iter = cso_insert_state(&ctx->cache, hash_key, CSO_VELEMENTS, cso);
        if (cso_hash_iter_is_null(iter)) {
            FREE(cso);
            return;
        }

        handle = cso->data;
    }
    else {
        handle = ((struct cso_velements *)cso_hash_iter_data(iter))->data;
    }

    if (ctx->velements != handle) {
        ctx->velements = handle;
        ctx->pipe->bind_vertex_elements_state(ctx->pipe, handle);
    }
}

enum pipe_error
cso_set_vertex_elements(struct cso_context *ctx,
                        const struct cso_velems_state *velems)
{
    struct u_vbuf *vbuf = ctx->vbuf_current;

    if (vbuf) {
        u_vbuf_set_vertex_elements(vbuf, velems);
        return PIPE_OK;
    }

    cso_set_vertex_elements_direct(ctx, velems);
    return PIPE_OK;
}

static void
cso_save_vertex_elements(struct cso_context *ctx)
{
    struct u_vbuf *vbuf = ctx->vbuf_current;

    if (vbuf) {
        u_vbuf_save_vertex_elements(vbuf);
        return;
    }

    assert(!ctx->velements_saved);
    ctx->velements_saved = ctx->velements;
}

static void
cso_restore_vertex_elements(struct cso_context *ctx)
{
    struct u_vbuf *vbuf = ctx->vbuf_current;

    if (vbuf) {
        u_vbuf_restore_vertex_elements(vbuf);
        return;
    }

    if (ctx->velements != ctx->velements_saved) {
        ctx->velements = ctx->velements_saved;
        ctx->pipe->bind_vertex_elements_state(ctx->pipe, ctx->velements_saved);
    }
    ctx->velements_saved = NULL;
}

/* vertex buffers */

void cso_set_vertex_buffers(struct cso_context *ctx,
                            unsigned start_slot, unsigned count,
                            const struct pipe_vertex_buffer *buffers)
{
    struct u_vbuf *vbuf = ctx->vbuf_current;

    if (!count)
        return;

    if (vbuf) {
        u_vbuf_set_vertex_buffers(vbuf, start_slot, count, 0, false, buffers);
        return;
    }

    struct pipe_context *pipe = ctx->pipe;
    pipe->set_vertex_buffers(pipe, start_slot, count, 0, false, buffers);
}

/**
 * Set vertex buffers and vertex elements. Skip u_vbuf if it's only needed
 * for user vertex buffers and user vertex buffers are not set by this call.
 * u_vbuf will be disabled. To re-enable u_vbuf, call this function again.
 *
 * Skipping u_vbuf decreases CPU overhead for draw calls that don't need it,
 * such as VBOs, glBegin/End, and display lists.
 *
 * Internal operations that do "save states, draw, restore states" shouldn't
 * use this, because the states are only saved in either cso_context or
 * u_vbuf, not both.
 */
void
cso_set_vertex_buffers_and_elements(struct cso_context *ctx,
                                    const struct cso_velems_state *velems,
                                    unsigned vb_count,
                                    unsigned unbind_trailing_vb_count,
                                    bool take_ownership,
                                    bool uses_user_vertex_buffers,
                                    const struct pipe_vertex_buffer *vbuffers)
{
    struct u_vbuf *vbuf = ctx->vbuf;
    struct pipe_context *pipe = ctx->pipe;

    if (vbuf && (ctx->always_use_vbuf || uses_user_vertex_buffers)) {
        if (!ctx->vbuf_current) {
            /* Unbind all buffers in cso_context, because we'll use u_vbuf. */
            unsigned unbind_vb_count = vb_count + unbind_trailing_vb_count;
            if (unbind_vb_count)
                pipe->set_vertex_buffers(pipe, 0, 0, unbind_vb_count, false, NULL);

            /* Unset this to make sure the CSO is re-bound on the next use. */
            ctx->velements = NULL;
            ctx->vbuf_current = vbuf;
            unbind_trailing_vb_count = 0;
        }

        if (vb_count || unbind_trailing_vb_count) {
            u_vbuf_set_vertex_buffers(vbuf, 0, vb_count,
                                      unbind_trailing_vb_count,
                                      take_ownership, vbuffers);
        }
        u_vbuf_set_vertex_elements(vbuf, velems);
        return;
    }

    if (ctx->vbuf_current) {
        /* Unbind all buffers in u_vbuf, because we'll use cso_context. */
        unsigned unbind_vb_count = vb_count + unbind_trailing_vb_count;
        if (unbind_vb_count)
            u_vbuf_set_vertex_buffers(vbuf, 0, 0, unbind_vb_count, false, NULL);

        /* Unset this to make sure the CSO is re-bound on the next use. */
        u_vbuf_unset_vertex_elements(vbuf);
        ctx->vbuf_current = NULL;
        unbind_trailing_vb_count = 0;
    }

    if (vb_count || unbind_trailing_vb_count) {
        pipe->set_vertex_buffers(pipe, 0, vb_count, unbind_trailing_vb_count,
                                 take_ownership, vbuffers);
    }
    cso_set_vertex_elements_direct(ctx, velems);
}

void
cso_single_sampler(struct cso_context *ctx, enum pipe_shader_type shader_stage,
                   unsigned idx, const struct pipe_sampler_state *templ)
{
    if (templ) {
        unsigned key_size = sizeof(struct pipe_sampler_state);
        unsigned hash_key = cso_construct_key((void*)templ, key_size);
        struct cso_sampler *cso;
        struct cso_hash_iter iter =
                cso_find_state_template(&ctx->cache,
                                        hash_key, CSO_SAMPLER,
                                        (void *) templ, key_size);

        if (cso_hash_iter_is_null(iter)) {
            cso = MALLOC(sizeof(struct cso_sampler));
            if (!cso)
                return;

            memcpy(&cso->state, templ, sizeof(*templ));
            cso->data = ctx->pipe->create_sampler_state(ctx->pipe, &cso->state);
            cso->hash_key = hash_key;

            iter = cso_insert_state(&ctx->cache, hash_key, CSO_SAMPLER, cso);
            if (cso_hash_iter_is_null(iter)) {
                FREE(cso);
                return;
            }
        }
        else {
            cso = cso_hash_iter_data(iter);
        }

        ctx->samplers[shader_stage].cso_samplers[idx] = cso;
        ctx->samplers[shader_stage].samplers[idx] = cso->data;
        ctx->max_sampler_seen = MAX2(ctx->max_sampler_seen, (int)idx);
    }
}


/**
 * Send staged sampler state to the driver.
 */
void
cso_single_sampler_done(struct cso_context *ctx,
                        enum pipe_shader_type shader_stage)
{
    struct sampler_info *info = &ctx->samplers[shader_stage];

    if (ctx->max_sampler_seen == -1)
        return;

    ctx->pipe->bind_sampler_states(ctx->pipe, shader_stage, 0,
                                   ctx->max_sampler_seen + 1,
                                   info->samplers);
    ctx->max_sampler_seen = -1;
}


/*
 * If the function encouters any errors it will return the
 * last one. Done to always try to set as many samplers
 * as possible.
 */
void
cso_set_samplers(struct cso_context *ctx,
                 enum pipe_shader_type shader_stage,
                 unsigned nr,
                 const struct pipe_sampler_state **templates)
{
    for (unsigned i = 0; i < nr; i++)
        cso_single_sampler(ctx, shader_stage, i, templates[i]);

    cso_single_sampler_done(ctx, shader_stage);
}

static void
cso_save_fragment_samplers(struct cso_context *ctx)
{
    struct sampler_info *info = &ctx->samplers[PIPE_SHADER_FRAGMENT];
    struct sampler_info *saved = &ctx->fragment_samplers_saved;

    memcpy(saved->cso_samplers, info->cso_samplers,
           sizeof(info->cso_samplers));
    memcpy(saved->samplers, info->samplers, sizeof(info->samplers));
}


static void
cso_restore_fragment_samplers(struct cso_context *ctx)
{
    struct sampler_info *info = &ctx->samplers[PIPE_SHADER_FRAGMENT];
    struct sampler_info *saved = &ctx->fragment_samplers_saved;

    memcpy(info->cso_samplers, saved->cso_samplers,
           sizeof(info->cso_samplers));
    memcpy(info->samplers, saved->samplers, sizeof(info->samplers));

    for (int i = PIPE_MAX_SAMPLERS - 1; i >= 0; i--) {
        if (info->samplers[i]) {
            ctx->max_sampler_seen = i;
            break;
        }
    }

    cso_single_sampler_done(ctx, PIPE_SHADER_FRAGMENT);
}


void
cso_set_stream_outputs(struct cso_context *ctx,
                       unsigned num_targets,
                       struct pipe_stream_output_target **targets,
                       const unsigned *offsets)
{
    struct pipe_context *pipe = ctx->pipe;
    uint i;

    if (!ctx->has_streamout) {
        assert(num_targets == 0);
        return;
    }

    if (ctx->nr_so_targets == 0 && num_targets == 0) {
        /* Nothing to do. */
        return;
    }

    /* reference new targets */
    for (i = 0; i < num_targets; i++) {
        pipe_so_target_reference(&ctx->so_targets[i], targets[i]);
    }
    /* unref extra old targets, if any */
    for (; i < ctx->nr_so_targets; i++) {
        pipe_so_target_reference(&ctx->so_targets[i], NULL);
    }

    pipe->set_stream_output_targets(pipe, num_targets, targets,
                                    offsets);
    ctx->nr_so_targets = num_targets;
}

static void
cso_save_stream_outputs(struct cso_context *ctx)
{
    uint i;

    if (!ctx->has_streamout) {
        return;
    }

    ctx->nr_so_targets_saved = ctx->nr_so_targets;

    for (i = 0; i < ctx->nr_so_targets; i++) {
        assert(!ctx->so_targets_saved[i]);
        pipe_so_target_reference(&ctx->so_targets_saved[i], ctx->so_targets[i]);
    }
}

static void
cso_restore_stream_outputs(struct cso_context *ctx)
{
    struct pipe_context *pipe = ctx->pipe;
    uint i;
    unsigned offset[PIPE_MAX_SO_BUFFERS];

    if (!ctx->has_streamout) {
        return;
    }

    if (ctx->nr_so_targets == 0 && ctx->nr_so_targets_saved == 0) {
        /* Nothing to do. */
        return;
    }

    assert(ctx->nr_so_targets_saved <= PIPE_MAX_SO_BUFFERS);
    for (i = 0; i < ctx->nr_so_targets_saved; i++) {
        pipe_so_target_reference(&ctx->so_targets[i], NULL);
        /* move the reference from one pointer to another */
        ctx->so_targets[i] = ctx->so_targets_saved[i];
        ctx->so_targets_saved[i] = NULL;
        /* -1 means append */
        offset[i] = (unsigned)-1;
    }
    for (; i < ctx->nr_so_targets; i++) {
        pipe_so_target_reference(&ctx->so_targets[i], NULL);
    }

    pipe->set_stream_output_targets(pipe, ctx->nr_so_targets_saved,
                                    ctx->so_targets, offset);

    ctx->nr_so_targets = ctx->nr_so_targets_saved;
    ctx->nr_so_targets_saved = 0;
}


/**
 * Save all the CSO state items specified by the state_mask bitmask
 * of CSO_BIT_x flags.
 */
void
cso_save_state(struct cso_context *cso, unsigned state_mask)
{
    assert(cso->saved_state == 0);

    cso->saved_state = state_mask;

    if (state_mask & CSO_BIT_BLEND)
        cso_save_blend(cso);
    if (state_mask & CSO_BIT_DEPTH_STENCIL_ALPHA)
        cso_save_depth_stencil_alpha(cso);
    if (state_mask & CSO_BIT_FRAGMENT_SAMPLERS)
        cso_save_fragment_samplers(cso);
    if (state_mask & CSO_BIT_FRAGMENT_SHADER)
        cso_save_fragment_shader(cso);
    if (state_mask & CSO_BIT_FRAMEBUFFER)
        cso_save_framebuffer(cso);
    if (state_mask & CSO_BIT_GEOMETRY_SHADER)
        cso_save_geometry_shader(cso);
    if (state_mask & CSO_BIT_MIN_SAMPLES)
        cso_save_min_samples(cso);
    if (state_mask & CSO_BIT_RASTERIZER)
        cso_save_rasterizer(cso);
    if (state_mask & CSO_BIT_RENDER_CONDITION)
        cso_save_render_condition(cso);
    if (state_mask & CSO_BIT_SAMPLE_MASK)
        cso_save_sample_mask(cso);
    if (state_mask & CSO_BIT_STENCIL_REF)
        cso_save_stencil_ref(cso);
    if (state_mask & CSO_BIT_STREAM_OUTPUTS)
        cso_save_stream_outputs(cso);
    if (state_mask & CSO_BIT_TESSCTRL_SHADER)
        cso_save_tessctrl_shader(cso);
    if (state_mask & CSO_BIT_TESSEVAL_SHADER)
        cso_save_tesseval_shader(cso);
    if (state_mask & CSO_BIT_VERTEX_ELEMENTS)
        cso_save_vertex_elements(cso);
    if (state_mask & CSO_BIT_VERTEX_SHADER)
        cso_save_vertex_shader(cso);
    if (state_mask & CSO_BIT_VIEWPORT)
        cso_save_viewport(cso);
    if (state_mask & CSO_BIT_PAUSE_QUERIES)
        cso->pipe->set_active_query_state(cso->pipe, false);
}


/**
 * Restore the state which was saved by cso_save_state().
 */
void
cso_restore_state(struct cso_context *cso)
{
    unsigned state_mask = cso->saved_state;

    assert(state_mask);

    if (state_mask & CSO_BIT_BLEND)
        cso_restore_blend(cso);
    if (state_mask & CSO_BIT_DEPTH_STENCIL_ALPHA)
        cso_restore_depth_stencil_alpha(cso);
    if (state_mask & CSO_BIT_FRAGMENT_SAMPLERS)
        cso_restore_fragment_samplers(cso);
    if (state_mask & CSO_BIT_FRAGMENT_SHADER)
        cso_restore_fragment_shader(cso);
    if (state_mask & CSO_BIT_FRAMEBUFFER)
        cso_restore_framebuffer(cso);
    if (state_mask & CSO_BIT_GEOMETRY_SHADER)
        cso_restore_geometry_shader(cso);
    if (state_mask & CSO_BIT_MIN_SAMPLES)
        cso_restore_min_samples(cso);
    if (state_mask & CSO_BIT_RASTERIZER)
        cso_restore_rasterizer(cso);
    if (state_mask & CSO_BIT_RENDER_CONDITION)
        cso_restore_render_condition(cso);
    if (state_mask & CSO_BIT_SAMPLE_MASK)
        cso_restore_sample_mask(cso);
    if (state_mask & CSO_BIT_STENCIL_REF)
        cso_restore_stencil_ref(cso);
    if (state_mask & CSO_BIT_STREAM_OUTPUTS)
        cso_restore_stream_outputs(cso);
    if (state_mask & CSO_BIT_TESSCTRL_SHADER)
        cso_restore_tessctrl_shader(cso);
    if (state_mask & CSO_BIT_TESSEVAL_SHADER)
        cso_restore_tesseval_shader(cso);
    if (state_mask & CSO_BIT_VERTEX_ELEMENTS)
        cso_restore_vertex_elements(cso);
    if (state_mask & CSO_BIT_VERTEX_SHADER)
        cso_restore_vertex_shader(cso);
    if (state_mask & CSO_BIT_VIEWPORT)
        cso_restore_viewport(cso);
    if (state_mask & CSO_BIT_PAUSE_QUERIES)
        cso->pipe->set_active_query_state(cso->pipe, true);

    cso->saved_state = 0;
}



/* drawing */

void
cso_draw_vbo(struct cso_context *cso,
             const struct pipe_draw_info *info,
             const struct pipe_draw_indirect_info *indirect,
             const struct pipe_draw_start_count draw)
{
    struct u_vbuf *vbuf = cso->vbuf_current;

    /* We can't have both indirect drawing and SO-vertex-count drawing */
    assert(!indirect ||
           indirect->buffer == NULL ||
           indirect->count_from_stream_output == NULL);

    /* We can't have SO-vertex-count drawing with an index buffer */
    assert(info->index_size == 0 ||
           !indirect ||
           indirect->count_from_stream_output == NULL);

    if (vbuf) {
        u_vbuf_draw_vbo(vbuf, info, indirect, draw);
    } else {
        struct pipe_context *pipe = cso->pipe;
        pipe->draw_vbo(pipe, info, indirect, &draw, 1);
    }
}

/* info->draw_id can be changed by the callee if increment_draw_id is true. */
void
cso_multi_draw(struct cso_context *cso,
               struct pipe_draw_info *info,
               const struct pipe_draw_start_count *draws,
               unsigned num_draws)
{
    struct u_vbuf *vbuf = cso->vbuf_current;

    if (vbuf) {
        /* Increase refcount to be able to use take_index_buffer_ownership with
         * all draws.
         */
        if (num_draws > 1 && info->take_index_buffer_ownership)
            p_atomic_add(&info->index.resource->reference.count, num_draws - 1);

        for (unsigned i = 0; i < num_draws; i++) {
            u_vbuf_draw_vbo(vbuf, info, NULL, draws[i]);

            if (info->increment_draw_id)
                info->drawid++;
        }
    } else {
        struct pipe_context *pipe = cso->pipe;

        pipe->draw_vbo(pipe, info, NULL, draws, num_draws);
    }
}

void
cso_draw_arrays(struct cso_context *cso, uint mode, uint start, uint count)
{
    struct pipe_draw_info info;
    struct pipe_draw_start_count draw;

    util_draw_init_info(&info);

    info.mode = mode;
    info.index_bounds_valid = true;
    info.min_index = start;
    info.max_index = start + count - 1;

    draw.start = start;
    draw.count = count;

    cso_draw_vbo(cso, &info, NULL, draw);
}

void
cso_draw_arrays_instanced(struct cso_context *cso, uint mode,
                          uint start, uint count,
                          uint start_instance, uint instance_count)
{
    struct pipe_draw_info info;
    struct pipe_draw_start_count draw;

    util_draw_init_info(&info);

    info.mode = mode;
    info.index_bounds_valid = true;
    info.min_index = start;
    info.max_index = start + count - 1;
    info.start_instance = start_instance;
    info.instance_count = instance_count;

    draw.start = start;
    draw.count = count;

    cso_draw_vbo(cso, &info, NULL, draw);
}
