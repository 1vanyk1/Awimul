#include "u_gen_mipmap.h"
#include "format/u_format.h"
#include "u_inlines.h"


/**
 * Generate mipmap images.  It's assumed all needed texture memory is
 * already allocated.
 *
 * \param pt      the texture to generate mipmap levels for
 * \param format  format of texture
 * \param first_layer  the first layer to generate mipmap levels for
 *                     (ignored for 3D textures)
 * \param last_layer  the last layer to generate mipmap levels for
 *                    (ignored for 3D textures)
 * \param base_level  the first mipmap level to use as a src
 * \param last_level  the last mipmap level to generate
 * \param filter  the minification filter used to generate mipmap levels with
 *                one of PIPE_TEX_FILTER_LINEAR, PIPE_TEX_FILTER_NEAREST
 */
boolean
util_gen_mipmap(struct pipe_context *pipe, struct pipe_resource *pt,
                enum pipe_format format, uint base_level, uint last_level,
                uint first_layer, uint last_layer, uint filter)
{
    struct pipe_screen *screen = pipe->screen;
    struct pipe_blit_info blit;
    uint dstLevel;
    boolean is_zs = util_format_is_depth_or_stencil(format);
    boolean has_depth =
            util_format_has_depth(util_format_description(format));

    /* nothing to do for stencil-only formats */
    if (is_zs && !has_depth)
        return TRUE;

    /* nothing to do for integer formats */
    if (!is_zs && util_format_is_pure_integer(format))
        return TRUE;

    if (!screen->is_format_supported(screen, format, pt->target,
                                     pt->nr_samples, pt->nr_storage_samples,
                                     PIPE_BIND_SAMPLER_VIEW |
                                     (is_zs ? PIPE_BIND_DEPTH_STENCIL :
                                      PIPE_BIND_RENDER_TARGET))) {
        return FALSE;
    }

    /* The texture object should have room for the levels which we're
     * about to generate.
     */
    assert(last_level <= pt->last_level);

    /* If this fails, why are we here? */
    assert(last_level > base_level);
    assert(filter == PIPE_TEX_FILTER_LINEAR ||
           filter == PIPE_TEX_FILTER_NEAREST);

    memset(&blit, 0, sizeof(blit));
    blit.src.resource = blit.dst.resource = pt;
    blit.src.format = blit.dst.format = format;
    /* don't set the stencil mask, stencil shouldn't be changed */
    blit.mask = is_zs ? PIPE_MASK_Z : PIPE_MASK_RGBA;
    blit.filter = filter;

    for (dstLevel = base_level + 1; dstLevel <= last_level; dstLevel++) {
        blit.src.level = dstLevel - 1;
        blit.dst.level = dstLevel;

        blit.src.box.width = u_minify(pt->width0, blit.src.level);
        blit.src.box.height = u_minify(pt->height0, blit.src.level);

        blit.dst.box.width = u_minify(pt->width0, blit.dst.level);
        blit.dst.box.height = u_minify(pt->height0, blit.dst.level);

        if (pt->target == PIPE_TEXTURE_3D) {
            /* generate all layers/slices at once */
            blit.src.box.z = blit.dst.box.z = 0;
            blit.src.box.depth = util_num_layers(pt, blit.src.level);
            blit.dst.box.depth = util_num_layers(pt, blit.dst.level);
        }
        else {
            blit.src.box.z = blit.dst.box.z = first_layer;
            blit.src.box.depth = blit.dst.box.depth =
                    (last_layer + 1 - first_layer);
        }

        pipe->blit(pipe, &blit);
    }
    return TRUE;
}
