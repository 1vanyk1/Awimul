#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../GL/gl.h"
#include "../GL/internal/dri_interface.h"
#include "../GL/glxtokens.h"
#include "winsys_handle.h"
#include "dri_interface.h"
#include "../zstd/common/debug.h"
#include <pthread.h>
#include "dri.h"
#include "dri_private.h"
#include "errors.h"
#include "format_info.h"
#include "st/st.h"


struct dri_screen
{
    /* st_api */
    struct st_manager base;
    struct st_api *st_api;

    /* on old libGL's invalidate doesn't get called as it should */
    boolean broken_invalidate;

    /* dri */
    __DRIscreen *sPriv;
    boolean throttle;

    struct st_config_options options;

    /* Which postprocessing filters are enabled. */
    unsigned pp_enabled[PP_FILTERS];

    /* drm */
    int fd;
    boolean can_share_buffer;

    struct pipe_loader_device *dev;

    /* gallium */
    boolean d_depth_bits_last;
    boolean sd_depth_bits_last;
    boolean auto_fake_front;
    boolean has_reset_status_query;
    enum pipe_texture_target target;

    boolean swrast_no_present;

    /* hooks filled in by dri2 & drisw */
    __DRIimage * (*lookup_egl_image)(struct dri_screen *ctx, void *handle);
    boolean (*validate_egl_image)(struct dri_screen *ctx, void *handle);
    __DRIimage * (*lookup_egl_image_validated)(struct dri_screen *ctx, void *handle);

    /* DRI exts that vary based on gallium pipe_screen caps. */
    __DRIimageExtension image_extension;
    __DRI2bufferDamageExtension buffer_damage_extension;

    /* DRI exts on this screen. Populated at init time based on device caps. */
    const __DRIextension *screen_extensions[14];

    /* OpenCL interop */
    mtx_t opencl_func_mutex;
    opencl_dri_event_add_ref_t opencl_dri_event_add_ref;
    opencl_dri_event_release_t opencl_dri_event_release;
    opencl_dri_event_wait_t opencl_dri_event_wait;
    opencl_dri_event_get_fence_t opencl_dri_event_get_fence;
};


void
pipe_loader_release(struct pipe_loader_device **devs, int ndev)
{
    int i;

    for (i = 0; i < ndev; i++)
        devs[i]->ops->release(&devs[i]);
}


void
dri_destroy_screen_helper(struct dri_screen * screen)
{
    if (screen->base.destroy)
        screen->base.destroy(&screen->base);

    if (screen->st_api && screen->st_api->destroy)
        screen->st_api->destroy(screen->st_api);

    if (screen->base.screen)
        screen->base.screen->destroy(screen->base.screen);

    mtx_destroy(&screen->opencl_func_mutex);
}

void
dri_destroy_screen(__DRIscreen * sPriv)
{
    struct dri_screen *screen = dri_screen(sPriv);

    dri_destroy_screen_helper(screen);

    pipe_loader_release(&screen->dev, 1);

    free(screen->options.force_gl_vendor);
    free(screen->options.force_gl_renderer);
    free(screen->options.mesa_extension_override);

    /* The caller in dri_util preserves the fd ownership */
    free(screen);
    sPriv->driverPrivate = NULL;
    sPriv->extensions = NULL;
}

boolean
dri2_validate_egl_image(struct dri_screen *screen, void *handle)
{
    const __DRIimageLookupExtension *loader = screen->sPriv->dri2.image;

    return loader->validateEGLImage(handle, screen->sPriv->loaderPrivate);
}

__DRIimage *
dri2_lookup_egl_image_validated(struct dri_screen *screen, void *handle)
{
    const __DRIimageLookupExtension *loader = screen->sPriv->dri2.image;

    return loader->lookupEGLImageValidated(handle, screen->sPriv->loaderPrivate);
}

__DRIimage *
dri2_lookup_egl_image(struct dri_screen *screen, void *handle)
{
    const __DRIimageLookupExtension *loader = screen->sPriv->dri2.image;
    __DRIimage *img;

    if (!loader->lookupEGLImage)
        return NULL;

    img = loader->lookupEGLImage(screen->sPriv,
                                 handle, screen->sPriv->loaderPrivate);

    return img;
}

static inline boolean
pipe_reference_described(struct pipe_reference *ptr,
                         struct pipe_reference *reference,
                         debug_reference_descriptor get_desc) {
    boolean destroy = FALSE;

    if (ptr != reference) {
        /* bump the reference.count first */
        if (reference) {
            assert(pipe_is_referenced(reference));
            p_atomic_inc(&reference->count);
            debug_reference(reference, get_desc, 1);
        }

        if (ptr) {
            assert(pipe_is_referenced(ptr));
            if (p_atomic_dec_zero(&ptr->count)) {
                destroy = TRUE;
            }
            debug_reference(ptr, get_desc, -1);
        }
    }

    return destroy;
}

static inline uint
util_format_get_blockwidth(enum pipe_format format)
{
    const struct util_format_description *desc = util_format_description(format);

    assert(desc);
    if (!desc) {
        return 1;
    }

    return desc->block.width;
}

static inline unsigned
util_format_get_nblocksx(enum pipe_format format,
                         unsigned x)
{
    unsigned blockwidth = util_format_get_blockwidth(format);
    return (x + blockwidth - 1) / blockwidth;
}

static inline uint
util_format_get_blocksizebits(enum pipe_format format)
{
    const struct util_format_description *desc = util_format_description(format);

    assert(desc);
    if (!desc) {
        return 0;
    }

    return desc->block.bits;
}

static inline uint
util_format_get_blocksize(enum pipe_format format)
{
    uint bits = util_format_get_blocksizebits(format);
    uint bytes = bits / 8;

    assert(bits % 8 == 0);
    assert(bytes > 0);
    if (bytes == 0) {
        bytes = 1;
    }

    return bytes;
}

static inline size_t
util_format_get_stride(enum pipe_format format,
                       unsigned width)
{
    return (size_t)util_format_get_nblocksx(format, width) * util_format_get_blocksize(format);
}

static inline const char *
util_format_short_name(enum pipe_format format)
{
    const struct util_format_description *desc = util_format_description(format);

    assert(desc);
    if (!desc) {
        return "???";
    }

    return desc->short_name;
}

void
debug_describe_resource(char* buf, const struct pipe_resource *ptr)
{
    switch(ptr->target)
    {
        case PIPE_BUFFER:
            sprintf(buf, "pipe_buffer<%u>", (unsigned)util_format_get_stride(ptr->format, ptr->width0));
            break;
        case PIPE_TEXTURE_1D:
            sprintf(buf, "pipe_texture1d<%u,%s,%u>", ptr->width0, util_format_short_name(ptr->format), ptr->last_level);
            break;
        case PIPE_TEXTURE_2D:
            sprintf(buf, "pipe_texture2d<%u,%u,%s,%u>", ptr->width0, ptr->height0, util_format_short_name(ptr->format), ptr->last_level);
            break;
        case PIPE_TEXTURE_RECT:
            sprintf(buf, "pipe_texture_rect<%u,%u,%s>", ptr->width0, ptr->height0, util_format_short_name(ptr->format));
            break;
        case PIPE_TEXTURE_CUBE:
            sprintf(buf, "pipe_texture_cube<%u,%u,%s,%u>", ptr->width0, ptr->height0, util_format_short_name(ptr->format), ptr->last_level);
            break;
        case PIPE_TEXTURE_3D:
            sprintf(buf, "pipe_texture3d<%u,%u,%u,%s,%u>", ptr->width0, ptr->height0, ptr->depth0, util_format_short_name(ptr->format), ptr->last_level);
            break;
        default:
            sprintf(buf, "pipe_martian_resource<%u>", ptr->target);
            break;
    }
}

static inline void
pipe_resource_reference(struct pipe_resource **dst, struct pipe_resource *src)
{
    struct pipe_resource *old_dst = *dst;

    if (pipe_reference_described(old_dst ? &old_dst->reference : NULL,
                                 src ? &src->reference : NULL,
                                 (debug_reference_descriptor)
                                         debug_describe_resource)) {
        /* Avoid recursion, which would prevent inlining this function */
        do {
            struct pipe_resource *next = old_dst->next;

            old_dst->screen->resource_destroy(old_dst->screen, old_dst);
            old_dst = next;
        } while (pipe_reference_described(old_dst ? &old_dst->reference : NULL,
                                          NULL,
                                          (debug_reference_descriptor)
                                                  debug_describe_resource));
    }
    *dst = src;
}

static const struct dri2_format_mapping dri2_format_table[] = {
        { DRM_FORMAT_ABGR16161616F, __DRI_IMAGE_FORMAT_ABGR16161616F,
                __DRI_IMAGE_COMPONENTS_RGBA,      PIPE_FORMAT_R16G16B16A16_FLOAT, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_ABGR16161616F } } },
        { DRM_FORMAT_XBGR16161616F, __DRI_IMAGE_FORMAT_XBGR16161616F,
                __DRI_IMAGE_COMPONENTS_RGB,       PIPE_FORMAT_R16G16B16X16_FLOAT, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_XBGR16161616F } } },
        { __DRI_IMAGE_FOURCC_RGBA16161616, __DRI_IMAGE_FORMAT_ABGR16161616,
                __DRI_IMAGE_COMPONENTS_RGBA,      PIPE_FORMAT_R16G16B16A16_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_ABGR16161616 } } },
        { DRM_FORMAT_ARGB2101010,   __DRI_IMAGE_FORMAT_ARGB2101010,
                __DRI_IMAGE_COMPONENTS_RGBA,      PIPE_FORMAT_B10G10R10A2_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_ARGB2101010 } } },
        { DRM_FORMAT_XRGB2101010,   __DRI_IMAGE_FORMAT_XRGB2101010,
                __DRI_IMAGE_COMPONENTS_RGB,       PIPE_FORMAT_B10G10R10X2_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_XRGB2101010 } } },
        { DRM_FORMAT_ABGR2101010,   __DRI_IMAGE_FORMAT_ABGR2101010,
                __DRI_IMAGE_COMPONENTS_RGBA,      PIPE_FORMAT_R10G10B10A2_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_ABGR2101010 } } },
        { DRM_FORMAT_XBGR2101010,   __DRI_IMAGE_FORMAT_XBGR2101010,
                __DRI_IMAGE_COMPONENTS_RGB,       PIPE_FORMAT_R10G10B10X2_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_XBGR2101010 } } },
        { DRM_FORMAT_ARGB8888,      __DRI_IMAGE_FORMAT_ARGB8888,
                __DRI_IMAGE_COMPONENTS_RGBA,      PIPE_FORMAT_BGRA8888_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_ARGB8888 } } },
        { DRM_FORMAT_ABGR8888,      __DRI_IMAGE_FORMAT_ABGR8888,
                __DRI_IMAGE_COMPONENTS_RGBA,      PIPE_FORMAT_RGBA8888_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_ABGR8888 } } },
        { __DRI_IMAGE_FOURCC_SARGB8888,     __DRI_IMAGE_FORMAT_SARGB8,
                __DRI_IMAGE_COMPONENTS_RGBA,      PIPE_FORMAT_BGRA8888_SRGB, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_SARGB8 } } },
        { DRM_FORMAT_XRGB8888,      __DRI_IMAGE_FORMAT_XRGB8888,
                __DRI_IMAGE_COMPONENTS_RGB,       PIPE_FORMAT_BGRX8888_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_XRGB8888 } } },
        { DRM_FORMAT_XBGR8888,      __DRI_IMAGE_FORMAT_XBGR8888,
                __DRI_IMAGE_COMPONENTS_RGB,       PIPE_FORMAT_RGBX8888_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_XBGR8888 } } },
        { DRM_FORMAT_ARGB1555,      __DRI_IMAGE_FORMAT_ARGB1555,
                __DRI_IMAGE_COMPONENTS_RGBA,      PIPE_FORMAT_B5G5R5A1_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_ARGB1555 } } },
        { DRM_FORMAT_RGB565,        __DRI_IMAGE_FORMAT_RGB565,
                __DRI_IMAGE_COMPONENTS_RGB,       PIPE_FORMAT_B5G6R5_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_RGB565 } } },
        { DRM_FORMAT_R8,            __DRI_IMAGE_FORMAT_R8,
                __DRI_IMAGE_COMPONENTS_R,         PIPE_FORMAT_R8_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 } } },
        { DRM_FORMAT_R16,           __DRI_IMAGE_FORMAT_R16,
                __DRI_IMAGE_COMPONENTS_R,         PIPE_FORMAT_R16_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R16 } } },
        { DRM_FORMAT_GR88,          __DRI_IMAGE_FORMAT_GR88,
                __DRI_IMAGE_COMPONENTS_RG,        PIPE_FORMAT_RG88_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_GR88 } } },
        { DRM_FORMAT_GR1616,        __DRI_IMAGE_FORMAT_GR1616,
                __DRI_IMAGE_COMPONENTS_RG,        PIPE_FORMAT_RG1616_UNORM, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_GR1616 } } },

        { DRM_FORMAT_YUV410, __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_U_V,     PIPE_FORMAT_IYUV, 3,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 1, 2, 2, __DRI_IMAGE_FORMAT_R8 },
                        { 2, 2, 2, __DRI_IMAGE_FORMAT_R8 } } },
        { DRM_FORMAT_YUV411, __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_U_V,     PIPE_FORMAT_IYUV, 3,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 1, 2, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 2, 2, 0, __DRI_IMAGE_FORMAT_R8 } } },
        { DRM_FORMAT_YUV420,        __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_U_V,     PIPE_FORMAT_IYUV, 3,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 1, 1, 1, __DRI_IMAGE_FORMAT_R8 },
                        { 2, 1, 1, __DRI_IMAGE_FORMAT_R8 } } },
        { DRM_FORMAT_YUV422,        __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_U_V,     PIPE_FORMAT_IYUV, 3,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 1, 1, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 2, 1, 0, __DRI_IMAGE_FORMAT_R8 } } },
        { DRM_FORMAT_YUV444,        __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_U_V,     PIPE_FORMAT_IYUV, 3,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 1, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 2, 0, 0, __DRI_IMAGE_FORMAT_R8 } } },

        { DRM_FORMAT_YVU410,        __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_U_V,     PIPE_FORMAT_IYUV, 3,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 2, 2, 2, __DRI_IMAGE_FORMAT_R8 },
                        { 1, 2, 2, __DRI_IMAGE_FORMAT_R8 } } },
        { DRM_FORMAT_YVU411,        __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_U_V,     PIPE_FORMAT_IYUV, 3,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 2, 2, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 1, 2, 0, __DRI_IMAGE_FORMAT_R8 } } },
        { DRM_FORMAT_YVU420,        __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_U_V,     PIPE_FORMAT_IYUV, 3,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 2, 1, 1, __DRI_IMAGE_FORMAT_R8 },
                        { 1, 1, 1, __DRI_IMAGE_FORMAT_R8 } } },
        { DRM_FORMAT_YVU422,        __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_U_V,     PIPE_FORMAT_IYUV, 3,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 2, 1, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 1, 1, 0, __DRI_IMAGE_FORMAT_R8 } } },
        { DRM_FORMAT_YVU444,        __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_U_V,     PIPE_FORMAT_IYUV, 3,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 2, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 1, 0, 0, __DRI_IMAGE_FORMAT_R8 } } },

        { DRM_FORMAT_NV12,          __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_UV,      PIPE_FORMAT_NV12, 2,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 1, 1, 1, __DRI_IMAGE_FORMAT_GR88 } } },

        { DRM_FORMAT_P010,          __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_UV,      PIPE_FORMAT_P010, 2,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R16 },
                        { 1, 1, 1, __DRI_IMAGE_FORMAT_GR1616 } } },
        { DRM_FORMAT_P012,          __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_UV,      PIPE_FORMAT_P012, 2,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R16 },
                        { 1, 1, 1, __DRI_IMAGE_FORMAT_GR1616 } } },
        { DRM_FORMAT_P016,          __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_UV,      PIPE_FORMAT_P016, 2,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R16 },
                        { 1, 1, 1, __DRI_IMAGE_FORMAT_GR1616 } } },

        { DRM_FORMAT_NV16,          __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_UV,      PIPE_FORMAT_NV12, 2,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_R8 },
                        { 1, 1, 0, __DRI_IMAGE_FORMAT_GR88 } } },

        { DRM_FORMAT_AYUV,      __DRI_IMAGE_FORMAT_ABGR8888,
                __DRI_IMAGE_COMPONENTS_AYUV,      PIPE_FORMAT_AYUV, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_ABGR8888 } } },
        { DRM_FORMAT_XYUV8888,      __DRI_IMAGE_FORMAT_XBGR8888,
                __DRI_IMAGE_COMPONENTS_XYUV,      PIPE_FORMAT_XYUV, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_XBGR8888 } } },

        { DRM_FORMAT_Y410,          __DRI_IMAGE_FORMAT_ABGR2101010,
                __DRI_IMAGE_COMPONENTS_AYUV,      PIPE_FORMAT_Y410, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_ABGR2101010 } } },

        /* Y412 is an unusual format.  It has the same layout as Y416 (i.e.,
         * 16-bits of physical storage per channel), but the low 4 bits of each
         * component are unused padding.  The writer is supposed to write zeros
         * to these bits.
         */
        { DRM_FORMAT_Y412,          __DRI_IMAGE_FORMAT_ABGR16161616,
                __DRI_IMAGE_COMPONENTS_AYUV,      PIPE_FORMAT_Y412, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_ABGR16161616 } } },
        { DRM_FORMAT_Y416,          __DRI_IMAGE_FORMAT_ABGR16161616,
                __DRI_IMAGE_COMPONENTS_AYUV,      PIPE_FORMAT_Y416, 1,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_ABGR16161616 } } },

        /* For YUYV and UYVY buffers, we set up two overlapping DRI images
         * and treat them as planar buffers in the compositors.
         * Plane 0 is GR88 and samples YU or YV pairs and places Y into
         * the R component, while plane 1 is ARGB/ABGR and samples YUYV/UYVY
         * clusters and places pairs and places U into the G component and
         * V into A.  This lets the texture sampler interpolate the Y
         * components correctly when sampling from plane 0, and interpolate
         * U and V correctly when sampling from plane 1. */
        { DRM_FORMAT_YUYV,          __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_XUXV,    PIPE_FORMAT_YUYV, 2,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_GR88 },
                        { 0, 1, 0, __DRI_IMAGE_FORMAT_ARGB8888 } } },
        { DRM_FORMAT_UYVY,          __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_UXVX,    PIPE_FORMAT_UYVY, 2,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_GR88 },
                        { 0, 1, 0, __DRI_IMAGE_FORMAT_ABGR8888 } } },

        /* The Y21x formats work in a similar fashion to the YUYV and UYVY
         * formats.
         */
        { DRM_FORMAT_Y210,          __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_XUXV,    PIPE_FORMAT_Y210, 2,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_GR1616 },
                        { 0, 1, 0, __DRI_IMAGE_FORMAT_ABGR16161616 } } },
        /* Y212 is an unusual format.  It has the same layout as Y216 (i.e.,
         * 16-bits of physical storage per channel), but the low 4 bits of each
         * component are unused padding.  The writer is supposed to write zeros
         * to these bits.
         */
        { DRM_FORMAT_Y212,          __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_XUXV,    PIPE_FORMAT_Y212, 2,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_GR1616 },
                        { 0, 1, 0, __DRI_IMAGE_FORMAT_ABGR16161616 } } },
        { DRM_FORMAT_Y216,          __DRI_IMAGE_FORMAT_NONE,
                __DRI_IMAGE_COMPONENTS_Y_XUXV,    PIPE_FORMAT_Y216, 2,
                { { 0, 0, 0, __DRI_IMAGE_FORMAT_GR1616 },
                        { 0, 1, 0, __DRI_IMAGE_FORMAT_ABGR16161616 } } },
};

const struct dri2_format_mapping *
dri2_get_mapping_by_fourcc(int fourcc)
{
    for (unsigned i = 0; i < ARRAY_SIZE(dri2_format_table); i++) {
        if (dri2_format_table[i].dri_fourcc == fourcc)
            return &dri2_format_table[i];
    }

    return NULL;
}

mesa_format
driImageFormatToGLFormat(uint32_t image_format)
{
    for (size_t i = 0; i < ARRAY_SIZE(format_mapping); i++)
        if (format_mapping[i].image_format == image_format)
            return format_mapping[i].mesa_format;

    return MESA_FORMAT_NONE;
}

uint32_t
driGLFormatToSizedInternalGLFormat(mesa_format format)
{
    for (size_t i = 0; i < ARRAY_SIZE(format_mapping); i++)
        if (format_mapping[i].mesa_format == format)
            return format_mapping[i].internal_format;

    return GL_NONE;
}

static bool
dri_get_egl_image(struct st_manager *smapi,
                  void *egl_image,
                  struct st_egl_image *stimg)
{
    struct dri_screen *screen = (struct dri_screen *)smapi;
    __DRIimage *img = NULL;
    const struct dri2_format_mapping *map;

    if (screen->lookup_egl_image_validated) {
        img = screen->lookup_egl_image_validated(screen, egl_image);
    } else if (screen->lookup_egl_image) {
        img = screen->lookup_egl_image(screen, egl_image);
    }

    if (!img)
        return FALSE;

    stimg->texture = NULL;
    pipe_resource_reference(&stimg->texture, img->texture);
    map = dri2_get_mapping_by_fourcc(img->dri_fourcc);
    stimg->format = map ? map->pipe_format : img->texture->format;
    stimg->level = img->level;
    stimg->layer = img->layer;

    if (img->imported_dmabuf && map) {
        /* Guess sized internal format for dma-bufs. Could be used
         * by EXT_EGL_image_storage.
         */
        mesa_format mesa_format = driImageFormatToGLFormat(map->dri_format);
        stimg->internalformat = driGLFormatToSizedInternalGLFormat(mesa_format);
    }

    return TRUE;
}

static int
dri_get_param(struct st_manager *smapi,
              enum st_manager_param param)
{
    struct dri_screen *screen = (struct dri_screen *)smapi;

    switch(param) {
        case ST_MANAGER_BROKEN_INVALIDATE:
            return screen->broken_invalidate;
        default:
            return 0;
    }
}

hud_add_queue_for_monitoring(struct hud_context *hud,
                             struct util_queue_monitoring *queue_info)
{
    assert(!hud->monitored_queue);
    hud->monitored_queue = queue_info;
}

static void
dri_set_background_context(struct st_context_iface *st,
                           struct util_queue_monitoring *queue_info)
{
    struct dri_context *ctx = (struct dri_context *)st->st_manager_private;
    const __DRIbackgroundCallableExtension *backgroundCallable =
            ctx->sPriv->dri2.backgroundCallable;

    /* Note: Mesa will only call this function if GL multithreading is enabled
     * We only do that if the loader exposed the __DRI_BACKGROUND_CALLABLE
     * extension. So we know that backgroundCallable is not NULL.
     */
    assert(backgroundCallable);
    backgroundCallable->setBackgroundContext(ctx->cPriv->loaderPrivate);

    if (ctx->hud)
        hud_add_queue_for_monitoring(ctx->hud, queue_info);
}

static bool
dri_validate_egl_image(struct st_manager *smapi,
                       void *egl_image)
{
    struct dri_screen *screen = (struct dri_screen *)smapi;

    return screen->validate_egl_image(screen, egl_image);
}


static const struct st_api st_gl_api;

struct st_api *
st_gl_api_create(void)
{
    return (struct st_api *) &st_gl_api;
}

int
driQueryOptioni(const driOptionCache *cache, const char *name)
{
    uint32_t i = findOption(cache, name);
    /* make sure the option is defined and has the correct type */
    assert(cache->info[i].name != NULL);
    assert(cache->info[i].type == DRI_INT || cache->info[i].type == DRI_ENUM);
    return cache->values[i]._int;
}

static void
dri_postprocessing_init(struct dri_screen *screen)
{
    unsigned i;

    for (i = 0; i < PP_FILTERS; i++) {
        screen->pp_enabled[i] = driQueryOptioni(&screen->dev->option_cache,
                                                pp_filters[i].name);
    }
}

unsigned char
driQueryOptionb(const driOptionCache *cache, const char *name)
{
    uint32_t i = findOption(cache, name);
    /* make sure the option is defined and has the correct type */
    assert(cache->info[i].name != NULL);
    assert(cache->info[i].type == DRI_BOOL);
    return cache->values[i]._bool;
}

static unsigned
dri_loader_get_cap(struct dri_screen *screen, enum dri_loader_cap cap)
{
    const __DRIdri2LoaderExtension *dri2_loader = screen->sPriv->dri2.loader;
    const __DRIimageLoaderExtension *image_loader = screen->sPriv->image.loader;

    if (dri2_loader && dri2_loader->base.version >= 4 &&
        dri2_loader->getCapability)
        return dri2_loader->getCapability(screen->sPriv->loaderPrivate, cap);

    if (image_loader && image_loader->base.version >= 2 &&
        image_loader->getCapability)
        return image_loader->getCapability(screen->sPriv->loaderPrivate, cap);

    return 0;
}

static const struct mesa_format_info *
_mesa_get_format_info(mesa_format format)
{
    const struct mesa_format_info *info = &format_info[format];
    STATIC_ASSERT(ARRAY_SIZE(format_info) == MESA_FORMAT_COUNT);

    /* The MESA_FORMAT_* enums are sparse, don't return a format info
     * for empty entries.
     */
    if (info->Name == MESA_FORMAT_NONE && format != MESA_FORMAT_NONE)
        return NULL;

    assert(info->Name == format);
    return info;
}

const char *
_mesa_get_format_name(mesa_format format)
{
    const struct mesa_format_info *info = _mesa_get_format_info(format);
    if (!info)
        return NULL;
    return info->StrName;
}

GLint
_mesa_get_format_bits(mesa_format format, GLenum pname)
{
    const struct mesa_format_info *info = _mesa_get_format_info(format);

    switch (pname) {
        case GL_RED_BITS:
        case GL_TEXTURE_RED_SIZE:
        case GL_RENDERBUFFER_RED_SIZE_EXT:
        case GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE:
        case GL_INTERNALFORMAT_RED_SIZE:
            return info->RedBits;
        case GL_GREEN_BITS:
        case GL_TEXTURE_GREEN_SIZE:
        case GL_RENDERBUFFER_GREEN_SIZE_EXT:
        case GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE:
        case GL_INTERNALFORMAT_GREEN_SIZE:
            return info->GreenBits;
        case GL_BLUE_BITS:
        case GL_TEXTURE_BLUE_SIZE:
        case GL_RENDERBUFFER_BLUE_SIZE_EXT:
        case GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE:
        case GL_INTERNALFORMAT_BLUE_SIZE:
            return info->BlueBits;
        case GL_ALPHA_BITS:
        case GL_TEXTURE_ALPHA_SIZE:
        case GL_RENDERBUFFER_ALPHA_SIZE_EXT:
        case GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE:
        case GL_INTERNALFORMAT_ALPHA_SIZE:
            return info->AlphaBits;
        case GL_TEXTURE_INTENSITY_SIZE:
            return info->IntensityBits;
        case GL_TEXTURE_LUMINANCE_SIZE:
            return info->LuminanceBits;
        case GL_INDEX_BITS:
            return 0;
        case GL_DEPTH_BITS:
        case GL_TEXTURE_DEPTH_SIZE_ARB:
        case GL_RENDERBUFFER_DEPTH_SIZE_EXT:
        case GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE:
        case GL_INTERNALFORMAT_DEPTH_SIZE:
            return info->DepthBits;
        case GL_STENCIL_BITS:
        case GL_TEXTURE_STENCIL_SIZE_EXT:
        case GL_RENDERBUFFER_STENCIL_SIZE_EXT:
        case GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE:
        case GL_INTERNALFORMAT_STENCIL_SIZE:
            return info->StencilBits;
        default:
            _mesa_problem(NULL, "bad pname in _mesa_get_format_bits()");
            return 0;
    }
}

bool
_mesa_is_format_srgb(mesa_format format)
{
    const struct mesa_format_info *info = _mesa_get_format_info(format);
    return info->IsSRGBFormat;
}

GLenum
_mesa_get_format_datatype(mesa_format format)
{
    const struct mesa_format_info *info = _mesa_get_format_info(format);
    return info->DataType;
}

__DRIconfig **
driCreateConfigs(mesa_format format,
                 const uint8_t * depth_bits, const uint8_t * stencil_bits,
                 unsigned num_depth_stencil_bits,
                 const GLenum * db_modes, unsigned num_db_modes,
                 const uint8_t * msaa_samples, unsigned num_msaa_modes,
                 GLboolean enable_accum, GLboolean color_depth_match)
{
    static const struct {
        uint32_t masks[4];
        int shifts[4];
    } format_table[] = {
            /* MESA_FORMAT_B5G6R5_UNORM */
            {{ 0x0000F800, 0x000007E0, 0x0000001F, 0x00000000 },
                    { 11, 5, 0, -1 }},
            /* MESA_FORMAT_B8G8R8X8_UNORM */
            {{ 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000 },
                    { 16, 8, 0, -1 }},
            /* MESA_FORMAT_B8G8R8A8_UNORM */
            {{ 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000 },
                    { 16, 8, 0, 24 }},
            /* MESA_FORMAT_B10G10R10X2_UNORM */
            {{ 0x3FF00000, 0x000FFC00, 0x000003FF, 0x00000000 },
                    { 20, 10, 0, -1 }},
            /* MESA_FORMAT_B10G10R10A2_UNORM */
            {{ 0x3FF00000, 0x000FFC00, 0x000003FF, 0xC0000000 },
                    { 20, 10, 0, 30 }},
            /* MESA_FORMAT_R8G8B8A8_UNORM */
            {{ 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 },
                    { 0, 8, 16, 24 }},
            /* MESA_FORMAT_R8G8B8X8_UNORM */
            {{ 0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000 },
                    { 0, 8, 16, -1 }},
            /* MESA_FORMAT_R10G10B10X2_UNORM */
            {{ 0x000003FF, 0x000FFC00, 0x3FF00000, 0x00000000 },
                    { 0, 10, 20, -1 }},
            /* MESA_FORMAT_R10G10B10A2_UNORM */
            {{ 0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000 },
                    { 0, 10, 20, 30 }},
            /* MESA_FORMAT_RGBX_FLOAT16 */
            {{ 0, 0, 0, 0},
                    { 0, 16, 32, -1 }},
            /* MESA_FORMAT_RGBA_FLOAT16 */
            {{ 0, 0, 0, 0},
                    { 0, 16, 32, 48 }},
    };

    const uint32_t * masks;
    const int * shifts;
    __DRIconfig **configs, **c;
    struct gl_config *modes;
    unsigned i, j, k, h;
    unsigned num_modes;
    unsigned num_accum_bits = (enable_accum) ? 2 : 1;
    int red_bits;
    int green_bits;
    int blue_bits;
    int alpha_bits;
    bool is_srgb;
    bool is_float;

    switch (format) {
        case MESA_FORMAT_B5G6R5_UNORM:
            masks = format_table[0].masks;
            shifts = format_table[0].shifts;
            break;
        case MESA_FORMAT_B8G8R8X8_UNORM:
        case MESA_FORMAT_B8G8R8X8_SRGB:
            masks = format_table[1].masks;
            shifts = format_table[1].shifts;
            break;
        case MESA_FORMAT_B8G8R8A8_UNORM:
        case MESA_FORMAT_B8G8R8A8_SRGB:
            masks = format_table[2].masks;
            shifts = format_table[2].shifts;
            break;
        case MESA_FORMAT_R8G8B8A8_UNORM:
        case MESA_FORMAT_R8G8B8A8_SRGB:
            masks = format_table[5].masks;
            shifts = format_table[5].shifts;
            break;
        case MESA_FORMAT_R8G8B8X8_UNORM:
        case MESA_FORMAT_R8G8B8X8_SRGB:
            masks = format_table[6].masks;
            shifts = format_table[6].shifts;
            break;
        case MESA_FORMAT_B10G10R10X2_UNORM:
            masks = format_table[3].masks;
            shifts = format_table[3].shifts;
            break;
        case MESA_FORMAT_B10G10R10A2_UNORM:
            masks = format_table[4].masks;
            shifts = format_table[4].shifts;
            break;
        case MESA_FORMAT_RGBX_FLOAT16:
            masks = format_table[9].masks;
            shifts = format_table[9].shifts;
            break;
        case MESA_FORMAT_RGBA_FLOAT16:
            masks = format_table[10].masks;
            shifts = format_table[10].shifts;
            break;
        case MESA_FORMAT_R10G10B10X2_UNORM:
            masks = format_table[7].masks;
            shifts = format_table[7].shifts;
            break;
        case MESA_FORMAT_R10G10B10A2_UNORM:
            masks = format_table[8].masks;
            shifts = format_table[8].shifts;
            break;
        default:
            fprintf(stderr, "[%s:%u] Unknown framebuffer type %s (%d).\n",
                    __func__, __LINE__,
                    _mesa_get_format_name(format), format);
            return NULL;
    }

    red_bits = _mesa_get_format_bits(format, GL_RED_BITS);
    green_bits = _mesa_get_format_bits(format, GL_GREEN_BITS);
    blue_bits = _mesa_get_format_bits(format, GL_BLUE_BITS);
    alpha_bits = _mesa_get_format_bits(format, GL_ALPHA_BITS);
    is_srgb = _mesa_is_format_srgb(format);
    is_float = _mesa_get_format_datatype(format) == GL_FLOAT;

    num_modes = num_depth_stencil_bits * num_db_modes * num_accum_bits * num_msaa_modes;
    configs = calloc(num_modes + 1, sizeof *configs);
    if (configs == NULL)
        return NULL;

    c = configs;
    for ( k = 0 ; k < num_depth_stencil_bits ; k++ ) {
        for ( i = 0 ; i < num_db_modes ; i++ ) {
            for ( h = 0 ; h < num_msaa_modes; h++ ) {
                for ( j = 0 ; j < num_accum_bits ; j++ ) {
                    if (color_depth_match &&
                        (depth_bits[k] || stencil_bits[k])) {
                        /* Depth can really only be 0, 16, 24, or 32. A 32-bit
                         * color format still matches 24-bit depth, as there
                         * is an implicit 8-bit stencil. So really we just
                         * need to make sure that color/depth are both 16 or
                         * both non-16.
                         */
                        if ((depth_bits[k] + stencil_bits[k] == 16) !=
                            (red_bits + green_bits + blue_bits + alpha_bits == 16))
                            continue;
                    }

                    *c = malloc (sizeof **c);
                    modes = &(*c)->modes;
                    c++;

                    memset(modes, 0, sizeof *modes);
                    modes->floatMode = is_float;
                    modes->redBits   = red_bits;
                    modes->greenBits = green_bits;
                    modes->blueBits  = blue_bits;
                    modes->alphaBits = alpha_bits;
                    modes->redMask   = masks[0];
                    modes->greenMask = masks[1];
                    modes->blueMask  = masks[2];
                    modes->alphaMask = masks[3];
                    modes->redShift   = shifts[0];
                    modes->greenShift = shifts[1];
                    modes->blueShift  = shifts[2];
                    modes->alphaShift = shifts[3];
                    modes->rgbBits   = modes->redBits + modes->greenBits
                                       + modes->blueBits + modes->alphaBits;

                    modes->accumRedBits   = 16 * j;
                    modes->accumGreenBits = 16 * j;
                    modes->accumBlueBits  = 16 * j;
                    modes->accumAlphaBits = 16 * j;

                    modes->stencilBits = stencil_bits[k];
                    modes->depthBits = depth_bits[k];

                    if (db_modes[i] == __DRI_ATTRIB_SWAP_NONE) {
                        modes->doubleBufferMode = GL_FALSE;
                        modes->swapMethod = __DRI_ATTRIB_SWAP_UNDEFINED;
                    }
                    else {
                        modes->doubleBufferMode = GL_TRUE;
                        modes->swapMethod = db_modes[i];
                    }

                    modes->samples = msaa_samples[h];

                    modes->sRGBCapable = is_srgb;
                }
            }
        }
    }
    *c = NULL;

    return configs;
}

__DRIconfig **driConcatConfigs(__DRIconfig **a,
                               __DRIconfig **b)
{
    __DRIconfig **all;
    int i, j, index;

    if (a == NULL || a[0] == NULL)
        return b;
    else if (b == NULL || b[0] == NULL)
        return a;

    i = 0;
    while (a[i] != NULL)
        i++;
    j = 0;
    while (b[j] != NULL)
        j++;

    all = malloc((i + j + 1) * sizeof *all);
    index = 0;
    for (i = 0; a[i] != NULL; i++)
        all[index++] = a[i];
    for (j = 0; b[j] != NULL; j++)
        all[index++] = b[j];
    all[index++] = NULL;

    free(a);
    free(b);

    return all;
}

static const __DRIconfig **
dri_fill_in_modes(struct dri_screen *screen)
{
    static const mesa_format mesa_formats[] = {
            MESA_FORMAT_B10G10R10A2_UNORM,
            MESA_FORMAT_B10G10R10X2_UNORM,
            MESA_FORMAT_R10G10B10A2_UNORM,
            MESA_FORMAT_R10G10B10X2_UNORM,
            MESA_FORMAT_B8G8R8A8_UNORM,
            MESA_FORMAT_B8G8R8X8_UNORM,
            MESA_FORMAT_B8G8R8A8_SRGB,
            MESA_FORMAT_B8G8R8X8_SRGB,
            MESA_FORMAT_B5G6R5_UNORM,
            MESA_FORMAT_RGBA_FLOAT16,
            MESA_FORMAT_RGBX_FLOAT16,
            MESA_FORMAT_R8G8B8A8_UNORM,
            MESA_FORMAT_R8G8B8X8_UNORM,
            MESA_FORMAT_R8G8B8A8_SRGB,
            MESA_FORMAT_R8G8B8X8_SRGB,
    };
    static const enum pipe_format pipe_formats[] = {
            PIPE_FORMAT_B10G10R10A2_UNORM,
            PIPE_FORMAT_B10G10R10X2_UNORM,
            PIPE_FORMAT_R10G10B10A2_UNORM,
            PIPE_FORMAT_R10G10B10X2_UNORM,
            PIPE_FORMAT_BGRA8888_UNORM,
            PIPE_FORMAT_BGRX8888_UNORM,
            PIPE_FORMAT_BGRA8888_SRGB,
            PIPE_FORMAT_BGRX8888_SRGB,
            PIPE_FORMAT_B5G6R5_UNORM,
            PIPE_FORMAT_R16G16B16A16_FLOAT,
            PIPE_FORMAT_R16G16B16X16_FLOAT,
            PIPE_FORMAT_RGBA8888_UNORM,
            PIPE_FORMAT_RGBX8888_UNORM,
            PIPE_FORMAT_RGBA8888_SRGB,
            PIPE_FORMAT_RGBX8888_SRGB,
    };
    mesa_format format;
    __DRIconfig **configs = NULL;
    uint8_t depth_bits_array[5];
    uint8_t stencil_bits_array[5];
    unsigned depth_buffer_factor;
    unsigned msaa_samples_max;
    unsigned i;
    struct pipe_screen *p_screen = screen->base.screen;
    bool pf_z16, pf_x8z24, pf_z24x8, pf_s8z24, pf_z24s8, pf_z32;
    bool mixed_color_depth;
    bool allow_rgba_ordering;
    bool allow_rgb10;
    bool allow_fp16;

    static const GLenum back_buffer_modes[] = {
            __DRI_ATTRIB_SWAP_NONE, __DRI_ATTRIB_SWAP_UNDEFINED,
            __DRI_ATTRIB_SWAP_COPY
    };

    if (driQueryOptionb(&screen->dev->option_cache, "always_have_depth_buffer")) {
        /* all visuals will have a depth buffer */
        depth_buffer_factor = 0;
    }
    else {
        depth_bits_array[0] = 0;
        stencil_bits_array[0] = 0;
        depth_buffer_factor = 1;
    }

    allow_rgba_ordering = dri_loader_get_cap(screen, DRI_LOADER_CAP_RGBA_ORDERING);
    allow_rgb10 = driQueryOptionb(&screen->dev->option_cache, "allow_rgb10_configs");
    allow_fp16 = dri_loader_get_cap(screen, DRI_LOADER_CAP_FP16);

    msaa_samples_max = (screen->st_api->feature_mask & ST_API_FEATURE_MS_VISUALS_MASK)
                       ? MSAA_VISUAL_MAX_SAMPLES : 1;

    pf_x8z24 = p_screen->is_format_supported(p_screen, PIPE_FORMAT_Z24X8_UNORM,
                                             PIPE_TEXTURE_2D, 0, 0,
                                             PIPE_BIND_DEPTH_STENCIL);
    pf_z24x8 = p_screen->is_format_supported(p_screen, PIPE_FORMAT_X8Z24_UNORM,
                                             PIPE_TEXTURE_2D, 0, 0,
                                             PIPE_BIND_DEPTH_STENCIL);
    pf_s8z24 = p_screen->is_format_supported(p_screen, PIPE_FORMAT_Z24_UNORM_S8_UINT,
                                             PIPE_TEXTURE_2D, 0, 0,
                                             PIPE_BIND_DEPTH_STENCIL);
    pf_z24s8 = p_screen->is_format_supported(p_screen, PIPE_FORMAT_S8_UINT_Z24_UNORM,
                                             PIPE_TEXTURE_2D, 0, 0,
                                             PIPE_BIND_DEPTH_STENCIL);
    pf_z16 = p_screen->is_format_supported(p_screen, PIPE_FORMAT_Z16_UNORM,
                                           PIPE_TEXTURE_2D, 0, 0,
                                           PIPE_BIND_DEPTH_STENCIL);
    pf_z32 = p_screen->is_format_supported(p_screen, PIPE_FORMAT_Z32_UNORM,
                                           PIPE_TEXTURE_2D, 0, 0,
                                           PIPE_BIND_DEPTH_STENCIL);

    if (pf_z16) {
        depth_bits_array[depth_buffer_factor] = 16;
        stencil_bits_array[depth_buffer_factor++] = 0;
    }
    if (pf_x8z24 || pf_z24x8) {
        depth_bits_array[depth_buffer_factor] = 24;
        stencil_bits_array[depth_buffer_factor++] = 0;
        screen->d_depth_bits_last = pf_x8z24;
    }
    if (pf_s8z24 || pf_z24s8) {
        depth_bits_array[depth_buffer_factor] = 24;
        stencil_bits_array[depth_buffer_factor++] = 8;
        screen->sd_depth_bits_last = pf_s8z24;
    }
    if (pf_z32) {
        depth_bits_array[depth_buffer_factor] = 32;
        stencil_bits_array[depth_buffer_factor++] = 0;
    }

    mixed_color_depth =
            p_screen->get_param(p_screen, PIPE_CAP_MIXED_COLOR_DEPTH_BITS);

    assert(ARRAY_SIZE(mesa_formats) == ARRAY_SIZE(pipe_formats));

    /* Add configs. */
    for (format = 0; format < ARRAY_SIZE(mesa_formats); format++) {
        __DRIconfig **new_configs = NULL;
        unsigned num_msaa_modes = 0; /* includes a single-sample mode */
        uint8_t msaa_modes[MSAA_VISUAL_MAX_SAMPLES];

        /* Expose only BGRA ordering if the loader doesn't support RGBA ordering. */
        if (!allow_rgba_ordering &&
            (mesa_formats[format] == MESA_FORMAT_R8G8B8A8_UNORM ||
             mesa_formats[format] == MESA_FORMAT_R8G8B8X8_UNORM ||
             mesa_formats[format] == MESA_FORMAT_R8G8B8A8_SRGB  ||
             mesa_formats[format] == MESA_FORMAT_R8G8B8X8_SRGB))
            continue;

        if (!allow_rgb10 &&
            (mesa_formats[format] == MESA_FORMAT_B10G10R10A2_UNORM ||
             mesa_formats[format] == MESA_FORMAT_B10G10R10X2_UNORM ||
             mesa_formats[format] == MESA_FORMAT_R10G10B10A2_UNORM ||
             mesa_formats[format] == MESA_FORMAT_R10G10B10X2_UNORM))
            continue;

        if (!allow_fp16 &&
            (mesa_formats[format] == MESA_FORMAT_RGBA_FLOAT16 ||
             mesa_formats[format] == MESA_FORMAT_RGBX_FLOAT16))
            continue;

        if (!p_screen->is_format_supported(p_screen, pipe_formats[format],
                                           PIPE_TEXTURE_2D, 0, 0,
                                           PIPE_BIND_RENDER_TARGET |
                                           PIPE_BIND_DISPLAY_TARGET))
            continue;

        for (i = 1; i <= msaa_samples_max; i++) {
            int samples = i > 1 ? i : 0;

            if (p_screen->is_format_supported(p_screen, pipe_formats[format],
                                              PIPE_TEXTURE_2D, samples, samples,
                                              PIPE_BIND_RENDER_TARGET)) {
                msaa_modes[num_msaa_modes++] = samples;
            }
        }

        if (num_msaa_modes) {
            /* Single-sample configs with an accumulation buffer. */
            new_configs = driCreateConfigs(mesa_formats[format],
                                           depth_bits_array, stencil_bits_array,
                                           depth_buffer_factor, back_buffer_modes,
                                           ARRAY_SIZE(back_buffer_modes),
                                           msaa_modes, 1,
                                           GL_TRUE, !mixed_color_depth);
            configs = driConcatConfigs(configs, new_configs);

            /* Multi-sample configs without an accumulation buffer. */
            if (num_msaa_modes > 1) {
                new_configs = driCreateConfigs(mesa_formats[format],
                                               depth_bits_array, stencil_bits_array,
                                               depth_buffer_factor, back_buffer_modes,
                                               ARRAY_SIZE(back_buffer_modes),
                                               msaa_modes+1, num_msaa_modes-1,
                                               GL_FALSE, !mixed_color_depth);
                configs = driConcatConfigs(configs, new_configs);
            }
        }
    }

    if (configs == NULL) {
        debug_printf("%s: driCreateConfigs failed\n", __FUNCTION__);
        return NULL;
    }

    return (const __DRIconfig **)configs;
}

const __DRIconfig **
dri_init_screen_helper(struct dri_screen *screen,
                       struct pipe_screen *pscreen)
{
    screen->base.screen = pscreen;
    screen->base.get_egl_image = dri_get_egl_image;
    screen->base.get_param = dri_get_param;
    screen->base.set_background_context = dri_set_background_context;

    if (screen->validate_egl_image)
        screen->base.validate_egl_image = dri_validate_egl_image;

    screen->st_api = st_gl_api_create();
    if (!screen->st_api)
        return NULL;

    if(pscreen->get_param(pscreen, PIPE_CAP_NPOT_TEXTURES))
        screen->target = PIPE_TEXTURE_2D;
    else
        screen->target = PIPE_TEXTURE_RECT;

    dri_postprocessing_init(screen);

    screen->st_api->query_versions(screen->st_api, &screen->base,
                                   &screen->options,
                                   &screen->sPriv->max_gl_core_version,
                                   &screen->sPriv->max_gl_compat_version,
                                   &screen->sPriv->max_gl_es1_version,
                                   &screen->sPriv->max_gl_es2_version);

    return dri_fill_in_modes(screen);
}

static inline void
call_once(once_flag *flag, void (*func)(void))
{
    pthread_once(flag, func);
}

static void
get_cpu_topology(void)
{
    /* Default. This is OK if L3 is not present or there is only one. */
    util_cpu_caps.num_L3_caches = 1;

    memset(util_cpu_caps.cpu_to_L3, 0xff, sizeof(util_cpu_caps.cpu_to_L3));

#if defined(PIPE_ARCH_X86) || defined(PIPE_ARCH_X86_64)
    /* AMD Zen */
   if (util_cpu_caps.family >= CPU_AMD_ZEN1_ZEN2 &&
       util_cpu_caps.family < CPU_AMD_LAST) {
      uint32_t regs[4];

      uint32_t saved_mask[UTIL_MAX_CPUS / 32] = {0};
      uint32_t mask[UTIL_MAX_CPUS / 32] = {0};
      bool saved = false;

      uint32_t L3_found[UTIL_MAX_CPUS] = {0};
      uint32_t num_L3_caches = 0;
      util_affinity_mask *L3_affinity_masks = NULL;

      /* Query APIC IDs from each CPU core.
       *
       * An APIC ID is a logical ID of the CPU with respect to the cache
       * hierarchy, meaning that consecutive APIC IDs are neighbours in
       * the hierarchy, e.g. sharing the same cache.
       *
       * For example, CPU 0 can have APIC ID 0 and CPU 12 can have APIC ID 1,
       * which means that both CPU 0 and 12 are next to each other.
       * (e.g. they are 2 threads belonging to 1 SMT2 core)
       *
       * We need to find out which CPUs share the same L3 cache and they can
       * be all over the place.
       *
       * Querying the APIC ID can only be done by pinning the current thread
       * to each core. The original affinity mask is saved.
       *
       * Loop over all possible CPUs even though some may be offline.
       */
      for (int16_t i = 0; i < util_cpu_caps.max_cpus && i < UTIL_MAX_CPUS; i++) {
         uint32_t cpu_bit = 1u << (i % 32);

         mask[i / 32] = cpu_bit;

         /* The assumption is that trying to bind the thread to a CPU that is
          * offline will fail.
          */
         if (util_set_current_thread_affinity(mask,
                                              !saved ? saved_mask : NULL,
                                              util_cpu_caps.num_cpu_mask_bits)) {
            saved = true;

            /* Query the APIC ID of the current core. */
            cpuid(0x00000001, regs);
            unsigned apic_id = regs[1] >> 24;

            /* Query the total core count for the CPU */
            uint32_t core_count = 1;
            if (regs[3] & (1 << 28))
               core_count = (regs[1] >> 16) & 0xff;

            core_count = util_next_power_of_two(core_count);

            /* Query the L3 cache count. */
            cpuid_count(0x8000001D, 3, regs);
            unsigned cache_level = (regs[0] >> 5) & 0x7;
            unsigned cores_per_L3 = ((regs[0] >> 14) & 0xfff) + 1;

            if (cache_level != 3)
               continue;

            unsigned local_core_id = apic_id & (core_count - 1);
            unsigned phys_id = (apic_id & ~(core_count - 1)) >> util_logbase2(core_count);
            unsigned local_l3_cache_index = local_core_id / util_next_power_of_two(cores_per_L3);
#define L3_ID(p, i) (p << 16 | i << 1 | 1);

            unsigned l3_id = L3_ID(phys_id, local_l3_cache_index);
            int idx = -1;
            for (unsigned c = 0; c < num_L3_caches; c++) {
               if (L3_found[c] == l3_id) {
                  idx = c;
                  break;
               }
            }
            if (idx == -1) {
               idx = num_L3_caches;
               L3_found[num_L3_caches++] = l3_id;
               L3_affinity_masks = realloc(L3_affinity_masks, sizeof(util_affinity_mask) * num_L3_caches);
               if (!L3_affinity_masks)
                  return;
               memset(&L3_affinity_masks[num_L3_caches - 1], 0, sizeof(util_affinity_mask));
            }
            util_cpu_caps.cpu_to_L3[i] = idx;
            L3_affinity_masks[idx][i / 32] |= cpu_bit;

         }
         mask[i / 32] = 0;
      }

      util_cpu_caps.num_L3_caches = num_L3_caches;
      util_cpu_caps.L3_affinity_mask = L3_affinity_masks;

      if (saved) {
         if (debug_get_option_dump_cpu()) {
            fprintf(stderr, "CPU <-> L3 cache mapping:\n");
            for (unsigned i = 0; i < util_cpu_caps.num_L3_caches; i++) {
               fprintf(stderr, "  - L3 %u mask = ", i);
               for (int j = util_cpu_caps.max_cpus - 1; j >= 0; j -= 32)
                  fprintf(stderr, "%08x ", util_cpu_caps.L3_affinity_mask[i][j / 32]);
               fprintf(stderr, "\n");
            }
         }

         /* Restore the original affinity mask. */
         util_set_current_thread_affinity(saved_mask, NULL,
                                          util_cpu_caps.num_cpu_mask_bits);
      } else {
         if (debug_get_option_dump_cpu())
            fprintf(stderr, "Cannot set thread affinity for any thread.\n");
      }
   }
#endif
}

DEBUG_GET_ONCE_BOOL_OPTION(dump_cpu, "GALLIUM_DUMP_CPU", false)

static void
util_cpu_detect_once(void)
{
    int available_cpus = 0;
    int total_cpus = 0;

    memset(&util_cpu_caps, 0, sizeof util_cpu_caps);

    /* Count the number of CPUs in system */
#if defined(PIPE_OS_WINDOWS)
    {
      SYSTEM_INFO system_info;
      GetSystemInfo(&system_info);
      available_cpus = MAX2(1, system_info.dwNumberOfProcessors);
   }
#elif defined(PIPE_OS_UNIX)
    #  if defined(HAS_SCHED_GETAFFINITY)
   {
      /* sched_setaffinity() can be used to further restrict the number of
       * CPUs on which the process can run.  Use sched_getaffinity() to
       * determine the true number of available CPUs.
       *
       * FIXME: The Linux manual page for sched_getaffinity describes how this
       * simple implementation will fail with > 1024 CPUs, and we'll fall back
       * to the _SC_NPROCESSORS_ONLN path.  Support for > 1024 CPUs can be
       * added to this path once someone has such a system for testing.
       */
      cpu_set_t affin;
      if (sched_getaffinity(getpid(), sizeof(affin), &affin) == 0)
         available_cpus = CPU_COUNT(&affin);
   }
#  endif

   /* Linux, FreeBSD, DragonFly, and Mac OS X should have
    * _SC_NOPROCESSORS_ONLN.  NetBSD and OpenBSD should have HW_NCPUONLINE.
    * This is what FFmpeg uses on those platforms.
    */
#  if defined(PIPE_OS_BSD) && defined(HW_NCPUONLINE)
   if (available_cpus == 0) {
      const int mib[] = { CTL_HW, HW_NCPUONLINE };
      int ncpu;
      int len = sizeof(ncpu);

      sysctl(mib, 2, &ncpu, &len, NULL, 0);
      available_cpus = ncpu;
   }
#  elif defined(_SC_NPROCESSORS_ONLN)
   if (available_cpus == 0) {
      available_cpus = sysconf(_SC_NPROCESSORS_ONLN);
      if (available_cpus == ~0)
         available_cpus = 1;
   }
#  elif defined(PIPE_OS_BSD)
   if (available_cpus == 0) {
      const int mib[] = { CTL_HW, HW_NCPU };
      int ncpu;
      int len = sizeof(ncpu);

      sysctl(mib, 2, &ncpu, &len, NULL, 0);
      available_cpus = ncpu;
   }
#  endif /* defined(PIPE_OS_BSD) */

   /* Determine the maximum number of CPUs configured in the system.  This is
    * used to properly set num_cpu_mask_bits below.  On BSDs that don't have
    * HW_NCPUONLINE, it was not clear whether HW_NCPU is the number of
    * configured or the number of online CPUs.  For that reason, prefer the
    * _SC_NPROCESSORS_CONF path on all BSDs.
    */
#  if defined(_SC_NPROCESSORS_CONF)
   total_cpus = sysconf(_SC_NPROCESSORS_CONF);
   if (total_cpus == ~0)
      total_cpus = 1;
#  elif defined(PIPE_OS_BSD)
   {
      const int mib[] = { CTL_HW, HW_NCPU };
      int ncpu;
      int len = sizeof(ncpu);

      sysctl(mib, 2, &ncpu, &len, NULL, 0);
      total_cpus = ncpu;
   }
#  endif /* defined(PIPE_OS_BSD) */
#endif /* defined(PIPE_OS_UNIX) */

    util_cpu_caps.nr_cpus = MAX2(1, available_cpus);
    total_cpus = MAX2(total_cpus, util_cpu_caps.nr_cpus);

    util_cpu_caps.max_cpus = total_cpus;
    util_cpu_caps.num_cpu_mask_bits = align(total_cpus, 32);

    /* Make the fallback cacheline size nonzero so that it can be
     * safely passed to align().
     */
    util_cpu_caps.cacheline = sizeof(void *);

#if defined(PIPE_ARCH_X86) || defined(PIPE_ARCH_X86_64)
    if (has_cpuid()) {
      uint32_t regs[4];
      uint32_t regs2[4];

      util_cpu_caps.cacheline = 32;

      /* Get max cpuid level */
      cpuid(0x00000000, regs);

      if (regs[0] >= 0x00000001) {
         unsigned int cacheline;

         cpuid (0x00000001, regs2);

         util_cpu_caps.x86_cpu_type = (regs2[0] >> 8) & 0xf;
         /* Add "extended family". */
         if (util_cpu_caps.x86_cpu_type == 0xf)
             util_cpu_caps.x86_cpu_type += ((regs2[0] >> 20) & 0xff);

         switch (util_cpu_caps.x86_cpu_type) {
         case 0x17:
            util_cpu_caps.family = CPU_AMD_ZEN1_ZEN2;
            break;
         case 0x18:
            util_cpu_caps.family = CPU_AMD_ZEN_HYGON;
            break;
         case 0x19:
            util_cpu_caps.family = CPU_AMD_ZEN3;
            break;
         default:
            if (util_cpu_caps.x86_cpu_type > 0x19)
               util_cpu_caps.family = CPU_AMD_ZEN_NEXT;
         }

         /* general feature flags */
         util_cpu_caps.has_tsc    = (regs2[3] >>  4) & 1; /* 0x0000010 */
         util_cpu_caps.has_mmx    = (regs2[3] >> 23) & 1; /* 0x0800000 */
         util_cpu_caps.has_sse    = (regs2[3] >> 25) & 1; /* 0x2000000 */
         util_cpu_caps.has_sse2   = (regs2[3] >> 26) & 1; /* 0x4000000 */
         util_cpu_caps.has_sse3   = (regs2[2] >>  0) & 1; /* 0x0000001 */
         util_cpu_caps.has_ssse3  = (regs2[2] >>  9) & 1; /* 0x0000020 */
         util_cpu_caps.has_sse4_1 = (regs2[2] >> 19) & 1;
         util_cpu_caps.has_sse4_2 = (regs2[2] >> 20) & 1;
         util_cpu_caps.has_popcnt = (regs2[2] >> 23) & 1;
         util_cpu_caps.has_avx    = ((regs2[2] >> 28) & 1) && // AVX
                                    ((regs2[2] >> 27) & 1) && // OSXSAVE
                                    ((xgetbv() & 6) == 6);    // XMM & YMM
         util_cpu_caps.has_f16c   = ((regs2[2] >> 29) & 1) && util_cpu_caps.has_avx;
         util_cpu_caps.has_fma    = ((regs2[2] >> 12) & 1) && util_cpu_caps.has_avx;
         util_cpu_caps.has_mmx2   = util_cpu_caps.has_sse; /* SSE cpus supports mmxext too */
#if defined(PIPE_ARCH_X86_64)
         util_cpu_caps.has_daz = 1;
#else
         util_cpu_caps.has_daz = util_cpu_caps.has_sse3 ||
            (util_cpu_caps.has_sse2 && sse2_has_daz());
#endif

         cacheline = ((regs2[1] >> 8) & 0xFF) * 8;
         if (cacheline > 0)
            util_cpu_caps.cacheline = cacheline;
      }
      if (util_cpu_caps.has_avx && regs[0] >= 0x00000007) {
         uint32_t regs7[4];
         cpuid_count(0x00000007, 0x00000000, regs7);
         util_cpu_caps.has_avx2 = (regs7[1] >> 5) & 1;
      }

      // check for avx512
      if (((regs2[2] >> 27) & 1) && // OSXSAVE
          (xgetbv() & (0x7 << 5)) && // OPMASK: upper-256 enabled by OS
          ((xgetbv() & 6) == 6)) { // XMM/YMM enabled by OS
         uint32_t regs3[4];
         cpuid_count(0x00000007, 0x00000000, regs3);
         util_cpu_caps.has_avx512f    = (regs3[1] >> 16) & 1;
         util_cpu_caps.has_avx512dq   = (regs3[1] >> 17) & 1;
         util_cpu_caps.has_avx512ifma = (regs3[1] >> 21) & 1;
         util_cpu_caps.has_avx512pf   = (regs3[1] >> 26) & 1;
         util_cpu_caps.has_avx512er   = (regs3[1] >> 27) & 1;
         util_cpu_caps.has_avx512cd   = (regs3[1] >> 28) & 1;
         util_cpu_caps.has_avx512bw   = (regs3[1] >> 30) & 1;
         util_cpu_caps.has_avx512vl   = (regs3[1] >> 31) & 1;
         util_cpu_caps.has_avx512vbmi = (regs3[2] >>  1) & 1;
      }

      if (regs[1] == 0x756e6547 && regs[2] == 0x6c65746e && regs[3] == 0x49656e69) {
         /* GenuineIntel */
         util_cpu_caps.has_intel = 1;
      }

      cpuid(0x80000000, regs);

      if (regs[0] >= 0x80000001) {

         cpuid(0x80000001, regs2);

         util_cpu_caps.has_mmx  |= (regs2[3] >> 23) & 1;
         util_cpu_caps.has_mmx2 |= (regs2[3] >> 22) & 1;
         util_cpu_caps.has_3dnow = (regs2[3] >> 31) & 1;
         util_cpu_caps.has_3dnow_ext = (regs2[3] >> 30) & 1;

         util_cpu_caps.has_xop = util_cpu_caps.has_avx &&
                                 ((regs2[2] >> 11) & 1);
      }

      if (regs[0] >= 0x80000006) {
         /* should we really do this if the clflush size above worked? */
         unsigned int cacheline;
         cpuid(0x80000006, regs2);
         cacheline = regs2[2] & 0xFF;
         if (cacheline > 0)
            util_cpu_caps.cacheline = cacheline;
      }

      if (!util_cpu_caps.has_sse) {
         util_cpu_caps.has_sse2 = 0;
         util_cpu_caps.has_sse3 = 0;
         util_cpu_caps.has_ssse3 = 0;
         util_cpu_caps.has_sse4_1 = 0;
      }
   }
#endif /* PIPE_ARCH_X86 || PIPE_ARCH_X86_64 */

#if defined(PIPE_ARCH_ARM) || defined(PIPE_ARCH_AARCH64)
    check_os_arm_support();
#endif

#if defined(PIPE_ARCH_PPC)
    check_os_altivec_support();
#endif /* PIPE_ARCH_PPC */

#if defined(PIPE_ARCH_MIPS64)
    check_os_mips64_support();
#endif /* PIPE_ARCH_MIPS64 */

    get_cpu_topology();

    if (debug_get_option_dump_cpu()) {
        printf("util_cpu_caps.nr_cpus = %u\n", util_cpu_caps.nr_cpus);

        printf("util_cpu_caps.x86_cpu_type = %u\n", util_cpu_caps.x86_cpu_type);
        printf("util_cpu_caps.cacheline = %u\n", util_cpu_caps.cacheline);

        printf("util_cpu_caps.has_tsc = %u\n", util_cpu_caps.has_tsc);
        printf("util_cpu_caps.has_mmx = %u\n", util_cpu_caps.has_mmx);
        printf("util_cpu_caps.has_mmx2 = %u\n", util_cpu_caps.has_mmx2);
        printf("util_cpu_caps.has_sse = %u\n", util_cpu_caps.has_sse);
        printf("util_cpu_caps.has_sse2 = %u\n", util_cpu_caps.has_sse2);
        printf("util_cpu_caps.has_sse3 = %u\n", util_cpu_caps.has_sse3);
        printf("util_cpu_caps.has_ssse3 = %u\n", util_cpu_caps.has_ssse3);
        printf("util_cpu_caps.has_sse4_1 = %u\n", util_cpu_caps.has_sse4_1);
        printf("util_cpu_caps.has_sse4_2 = %u\n", util_cpu_caps.has_sse4_2);
        printf("util_cpu_caps.has_avx = %u\n", util_cpu_caps.has_avx);
        printf("util_cpu_caps.has_avx2 = %u\n", util_cpu_caps.has_avx2);
        printf("util_cpu_caps.has_f16c = %u\n", util_cpu_caps.has_f16c);
        printf("util_cpu_caps.has_popcnt = %u\n", util_cpu_caps.has_popcnt);
        printf("util_cpu_caps.has_3dnow = %u\n", util_cpu_caps.has_3dnow);
        printf("util_cpu_caps.has_3dnow_ext = %u\n", util_cpu_caps.has_3dnow_ext);
        printf("util_cpu_caps.has_xop = %u\n", util_cpu_caps.has_xop);
        printf("util_cpu_caps.has_altivec = %u\n", util_cpu_caps.has_altivec);
        printf("util_cpu_caps.has_vsx = %u\n", util_cpu_caps.has_vsx);
        printf("util_cpu_caps.has_neon = %u\n", util_cpu_caps.has_neon);
        printf("util_cpu_caps.has_msa = %u\n", util_cpu_caps.has_msa);
        printf("util_cpu_caps.has_daz = %u\n", util_cpu_caps.has_daz);
        printf("util_cpu_caps.has_avx512f = %u\n", util_cpu_caps.has_avx512f);
        printf("util_cpu_caps.has_avx512dq = %u\n", util_cpu_caps.has_avx512dq);
        printf("util_cpu_caps.has_avx512ifma = %u\n", util_cpu_caps.has_avx512ifma);
        printf("util_cpu_caps.has_avx512pf = %u\n", util_cpu_caps.has_avx512pf);
        printf("util_cpu_caps.has_avx512er = %u\n", util_cpu_caps.has_avx512er);
        printf("util_cpu_caps.has_avx512cd = %u\n", util_cpu_caps.has_avx512cd);
        printf("util_cpu_caps.has_avx512bw = %u\n", util_cpu_caps.has_avx512bw);
        printf("util_cpu_caps.has_avx512vl = %u\n", util_cpu_caps.has_avx512vl);
        printf("util_cpu_caps.has_avx512vbmi = %u\n", util_cpu_caps.has_avx512vbmi);
        printf("util_cpu_caps.num_L3_caches = %u\n", util_cpu_caps.num_L3_caches);
        printf("util_cpu_caps.num_cpu_mask_bits = %u\n", util_cpu_caps.num_cpu_mask_bits);
    }
}

static once_flag cpu_once_flag = ONCE_FLAG_INIT;

void
util_cpu_detect(void)
{
    call_once(&cpu_once_flag, util_cpu_detect_once);
}

const driOptionDescription gallium_driconf[] = {
        DRI_CONF_SECTION_PERFORMANCE
        DRI_CONF_MESA_GLTHREAD(false)
        DRI_CONF_MESA_NO_ERROR(false)
        DRI_CONF_SECTION_END

        DRI_CONF_SECTION_QUALITY
        DRI_CONF_PP_CELSHADE(0)
        DRI_CONF_PP_NORED(0)
        DRI_CONF_PP_NOGREEN(0)
        DRI_CONF_PP_NOBLUE(0)
        DRI_CONF_PP_JIMENEZMLAA(0, 0, 32)
        DRI_CONF_PP_JIMENEZMLAA_COLOR(0, 0, 32)
        DRI_CONF_SECTION_END

        DRI_CONF_SECTION_DEBUG
        DRI_CONF_FORCE_GLSL_EXTENSIONS_WARN(false)
        DRI_CONF_DISABLE_GLSL_LINE_CONTINUATIONS(false)
        DRI_CONF_DISABLE_BLEND_FUNC_EXTENDED(false)
        DRI_CONF_DISABLE_ARB_GPU_SHADER5(false)
        DRI_CONF_FORCE_GLSL_VERSION(0)
        DRI_CONF_ALLOW_EXTRA_PP_TOKENS(false)
        DRI_CONF_ALLOW_GLSL_EXTENSION_DIRECTIVE_MIDSHADER(false)
        DRI_CONF_ALLOW_GLSL_120_SUBSET_IN_110(false)
        DRI_CONF_ALLOW_GLSL_BUILTIN_CONST_EXPRESSION(false)
        DRI_CONF_ALLOW_GLSL_RELAXED_ES(false)
        DRI_CONF_ALLOW_GLSL_BUILTIN_VARIABLE_REDECLARATION(false)
        DRI_CONF_ALLOW_GLSL_CROSS_STAGE_INTERPOLATION_MISMATCH(false)
        DRI_CONF_DO_DCE_BEFORE_CLIP_CULL_ANALYSIS(false)
        DRI_CONF_ALLOW_HIGHER_COMPAT_VERSION(false)
        DRI_CONF_ALLOW_GLSL_COMPAT_SHADERS(false)
        DRI_CONF_FORCE_GLSL_ABS_SQRT(false)
        DRI_CONF_GLSL_CORRECT_DERIVATIVES_AFTER_DISCARD(false)
        DRI_CONF_GLSL_IGNORE_WRITE_TO_READONLY_VAR(false)
        DRI_CONF_ALLOW_DRAW_OUT_OF_ORDER(false)
        DRI_CONF_GLTHREAD_NOP_CHECK_FRAMEBUFFER_STATUS(false)
        DRI_CONF_FORCE_COMPAT_PROFILE(false)
        DRI_CONF_FORCE_COMPAT_SHADERS(false)
        DRI_CONF_FORCE_GL_NAMES_REUSE(false)
        DRI_CONF_TRANSCODE_ETC(false)
        DRI_CONF_TRANSCODE_ASTC(false)
        DRI_CONF_FORCE_GL_VENDOR()
        DRI_CONF_FORCE_GL_RENDERER()
        DRI_CONF_OVERRIDE_VRAM_SIZE()
        DRI_CONF_GLX_EXTENSION_OVERRIDE()
        DRI_CONF_MESA_EXTENSION_OVERRIDE()
        DRI_CONF_INDIRECT_GL_EXTENSION_OVERRIDE()
        DRI_CONF_DISABLE_PROTECTED_CONTENT_CHECK(false)
        DRI_CONF_IGNORE_MAP_UNSYNCHRONIZED(false)
        DRI_CONF_FORCE_DIRECT_GLX_CONTEXT(false)
        DRI_CONF_ALLOW_INVALID_GLX_DESTROY_WINDOW(false)
        DRI_CONF_SECTION_END

        DRI_CONF_SECTION_MISCELLANEOUS
        DRI_CONF_ALWAYS_HAVE_DEPTH_BUFFER(false)
        DRI_CONF_GLSL_ZERO_INIT(false)
        DRI_CONF_VS_POSITION_ALWAYS_INVARIANT(false)
        DRI_CONF_VS_POSITION_ALWAYS_PRECISE(false)
        DRI_CONF_ALLOW_RGB10_CONFIGS(true)
        DRI_CONF_FORCE_INTEGER_TEX_NEAREST(false)
        DRI_CONF_SECTION_END
};

static driOptionDescription *
merge_driconf(const driOptionDescription *driver_driconf, unsigned driver_count,
              unsigned *merged_count)
{
    unsigned gallium_count = ARRAY_SIZE(gallium_driconf);
    driOptionDescription *merged = malloc((driver_count + gallium_count) *
                                          sizeof(*merged));
    if (!merged) {
        *merged_count = 0;
        return NULL;
    }

    memcpy(merged, gallium_driconf, sizeof(*merged) * gallium_count);
    memcpy(&merged[gallium_count], driver_driconf, sizeof(*merged) * driver_count);

    *merged_count = driver_count + gallium_count;
    return merged;
}

static void
pipe_loader_load_options(struct pipe_loader_device *dev)
{
    if (dev->option_info.info)
        return;

    unsigned driver_count, merged_count;
    const driOptionDescription *driver_driconf =
            dev->ops->get_driconf(dev, &driver_count);

    const driOptionDescription *merged_driconf =
            merge_driconf(driver_driconf, driver_count, &merged_count);
    driParseOptionInfo(&dev->option_info, merged_driconf, merged_count);
    free((void *)merged_driconf);
}

struct pipe_screen *
pipe_loader_create_screen_vk(struct pipe_loader_device *dev, bool sw_vk)
{
    struct pipe_screen_config config;

    util_cpu_detect();
    pipe_loader_load_options(dev);
    config.options_info = &dev->option_info;
    config.options = &dev->option_cache;

    return dev->ops->create_screen(dev, &config, sw_vk);
}

struct pipe_screen *
pipe_loader_create_screen(struct pipe_loader_device *dev)
{
    return pipe_loader_create_screen_vk(dev, false);
}

#define debug_get_option_swrast_no_present() false

struct pipe_loader_sw_device {
    struct pipe_loader_device base;
    const struct sw_driver_descriptor *dd;
#ifndef GALLIUM_STATIC_TARGETS
    struct util_dl_library *lib;
#endif
    struct sw_winsys *ws;
    int fd;
};

void
util_dl_close(struct util_dl_library *library)
{
#if defined(PIPE_OS_UNIX)
    dlclose((void *)library);
#elif defined(PIPE_OS_WINDOWS)
    FreeLibrary((HMODULE)library);
#else
    (void)library;
#endif
}

static inline int
u_file_access(const char *path, int mode) {
#if defined(PIPE_OS_UNIX)
    return access(path, mode);
#elif defined(PIPE_OS_WINDOWS)
    return _access(path, mode);
#else
    return 0;
#endif
}

struct util_dl_library *
util_dl_open(const char *filename)
{
#if defined(PIPE_OS_UNIX)
    return (struct util_dl_library *)dlopen(filename, RTLD_LAZY | RTLD_LOCAL);
#elif defined(PIPE_OS_WINDOWS)
    return (struct util_dl_library *)LoadLibraryA(filename);
#else
    return NULL;
#endif
}

const char *
util_dl_error(void)
{
#if defined(PIPE_OS_UNIX)
    return dlerror();
#elif defined(PIPE_OS_WINDOWS)
    return "unknown error";
#else
    return "unknown error";
#endif
}

struct util_dl_library *
pipe_loader_find_module(const char *driver_name,
                        const char *library_paths)
{
    struct util_dl_library *lib;
    const char *next;
    char path[PATH_MAX];
    int len, ret;

    for (next = library_paths; *next; library_paths = next + 1) {
        next = strchrnul(library_paths, ':');
        len = next - library_paths;

        if (len)
            ret = snprintf(path, sizeof(path), "%.*s/%s%s%s",
                           len, library_paths,
                           MODULE_PREFIX, driver_name, UTIL_DL_EXT);
        else
            ret = snprintf(path, sizeof(path), "%s%s%s",
                           MODULE_PREFIX, driver_name, UTIL_DL_EXT);

        if (ret > 0 && ret < sizeof(path) && u_file_access(path, 0) != -1) {
            lib = util_dl_open(path);
            if (lib) {
                return lib;
            }
            fprintf(stderr, "ERROR: Failed to load pipe driver at `%s': %s\n",
                    path, util_dl_error());
        }
    }

    return NULL;
}

static struct pipe_screen *
pipe_loader_sw_create_screen(struct pipe_loader_device *dev,
                             const struct pipe_screen_config *config, bool sw_vk)
{
    struct pipe_loader_sw_device *sdev = pipe_loader_sw_device(dev);
    struct pipe_screen *screen;

    screen = sdev->dd->create_screen(sdev->ws, sw_vk);
    if (!screen)
        sdev->ws->destroy(sdev->ws);

    return screen ? debug_screen_wrap(screen) : NULL;
}

static const struct driOptionDescription *
pipe_loader_sw_get_driconf(struct pipe_loader_device *dev, unsigned *count)
{
    *count = 0;
    return NULL;
}

void
pipe_loader_base_release(struct pipe_loader_device **dev)
{
    driDestroyOptionCache(&(*dev)->option_cache);
    driDestroyOptionInfo(&(*dev)->option_info);

    FREE(*dev);
    *dev = NULL;
}

static void
pipe_loader_sw_release(struct pipe_loader_device **dev)
{
    struct pipe_loader_sw_device *sdev =
            pipe_loader_sw_device(*dev);

#ifndef GALLIUM_STATIC_TARGETS
    if (sdev->lib)
        util_dl_close(sdev->lib);
#endif

#ifdef HAVE_PIPE_LOADER_KMS
    if (sdev->fd != -1)
      close(sdev->fd);
#endif

    pipe_loader_base_release(dev);
}


static const struct pipe_loader_ops pipe_loader_sw_ops = {
        .create_screen = pipe_loader_sw_create_screen,
        .get_driconf = pipe_loader_sw_get_driconf,
        .release = pipe_loader_sw_release
};

util_dl_proc
util_dl_get_proc_address(struct util_dl_library *library,
                         const char *procname)
{
#if defined(PIPE_OS_UNIX)
    return (util_dl_proc) pointer_to_func(dlsym((void *)library, procname));
#elif defined(PIPE_OS_WINDOWS)
    return (util_dl_proc)GetProcAddress((HMODULE)library, procname);
#else
    return (util_dl_proc)NULL;
#endif
}

static bool
pipe_loader_sw_probe_init_common(struct pipe_loader_sw_device *sdev)
{
    sdev->base.type = PIPE_LOADER_DEVICE_SOFTWARE;
    sdev->base.driver_name = "swrast";
    sdev->base.ops = &pipe_loader_sw_ops;
    sdev->fd = -1;

#ifdef GALLIUM_STATIC_TARGETS
    sdev->dd = &driver_descriptors;
   if (!sdev->dd)
      return false;
#else
    const char *search_dir = getenv("GALLIUM_PIPE_SEARCH_DIR");
    if (search_dir == NULL)
        search_dir = PIPE_SEARCH_DIR;

    sdev->lib = pipe_loader_find_module("swrast", search_dir);
    if (!sdev->lib)
        return false;

    sdev->dd = (const struct sw_driver_descriptor *)
            util_dl_get_proc_address(sdev->lib, "swrast_driver_descriptor");

    if (!sdev->dd){
        util_dl_close(sdev->lib);
        sdev->lib = NULL;
        return false;
    }
#endif

    return true;
}


static void
pipe_loader_sw_probe_teardown_common(struct pipe_loader_sw_device *sdev)
{
#ifndef GALLIUM_STATIC_TARGETS
    if (sdev->lib)
        util_dl_close(sdev->lib);
#endif
}


bool
pipe_loader_sw_probe_dri(struct pipe_loader_device **devs, const struct drisw_loader_funcs *drisw_lf)
{
    struct pipe_loader_sw_device *sdev = CALLOC_STRUCT(pipe_loader_sw_device);
    int i;

    if (!sdev)
        return false;

    if (!pipe_loader_sw_probe_init_common(sdev))
        goto fail;

    for (i = 0; sdev->dd->winsys[i].name; i++) {
        if (strcmp(sdev->dd->winsys[i].name, "dri") == 0) {
            sdev->ws = sdev->dd->winsys[i].create_winsys(drisw_lf);
            break;
        }
    }
    if (!sdev->ws)
        goto fail;

    *devs = &sdev->base;
    return true;

    fail:
    pipe_loader_sw_probe_teardown_common(sdev);
    FREE(sdev);
    return false;
}

void
pipe_loader_config_options(struct pipe_loader_device *dev)
{
//    if (!dev->option_cache.info) {
//        driParseConfigFiles(&dev->option_cache, &dev->option_info, 0,
//                            dev->driver_name, NULL, NULL, NULL, 0, NULL, 0);
//    }
}

static char *
str_dup(const char *str)
{
    char *copy;
    copy = malloc(strlen(str) + 1);
    if (!copy)
        return NULL;
    strcpy(copy, str);
    return copy;
}

struct
#ifdef _MSC_VER
    #if _WIN64
__declspec(align(16))
#else
 __declspec(align(8))
#endif
#elif defined(__LP64__)
    __attribute__((aligned(16)))
#else
        __attribute__((aligned(8)))
#endif
ralloc_header
{
#ifndef NDEBUG
/* A canary value used to determine whether a pointer is ralloc'd. */
unsigned canary;
#endif

struct ralloc_header *parent;

/* The first child (head of a linked list) */
struct ralloc_header *child;

/* Linked list of siblings */
struct ralloc_header *prev;
struct ralloc_header *next;

void (*destructor)(void *);
};

typedef struct ralloc_header ralloc_header;

static ralloc_header *
get_header(const void *ptr)
{
    ralloc_header *info = (ralloc_header *) (((char *) ptr) -
                                             sizeof(ralloc_header));
    assert(info->canary == CANARY);
    return info;
}

static void
add_child(ralloc_header *parent, ralloc_header *info)
{
    if (parent != NULL) {
        info->parent = parent;
        info->next = parent->child;
        parent->child = info;

        if (info->next != NULL)
            info->next->prev = info;
    }
}

#define PTR_FROM_HEADER(info) (((char *) info) + sizeof(ralloc_header))

void *
ralloc_size(const void *ctx, size_t size)
{
    /* Some malloc allocation doesn't always align to 16 bytes even on 64 bits
     * system, from Android bionic/tests/malloc_test.cpp:
     *  - Allocations of a size that rounds up to a multiple of 16 bytes
     *    must have at least 16 byte alignment.
     *  - Allocations of a size that rounds up to a multiple of 8 bytes and
     *    not 16 bytes, are only required to have at least 8 byte alignment.
     */
    void *block = malloc(align64(size + sizeof(ralloc_header),
                                 alignof(ralloc_header)));
    ralloc_header *info;
    ralloc_header *parent;

    if (unlikely(block == NULL))
        return NULL;

    info = (ralloc_header *) block;
    /* measurements have shown that calloc is slower (because of
     * the multiplication overflow checking?), so clear things
     * manually
     */
    info->parent = NULL;
    info->child = NULL;
    info->prev = NULL;
    info->next = NULL;
    info->destructor = NULL;

    parent = ctx != NULL ? get_header(ctx) : NULL;

    add_child(parent, info);

#ifndef NDEBUG
    info->canary = CANARY;
#endif

    return PTR_FROM_HEADER(info);
}

void *
ralloc_context(const void *ctx)
{
    return ralloc_size(ctx, 0);
}

void *
ralloc_array_size(const void *ctx, size_t size, unsigned count)
{
    if (count > SIZE_MAX/size)
        return NULL;

    return ralloc_size(ctx, size * count);
}

char *
ralloc_strdup(const void *ctx, const char *str)
{
    size_t n;
    char *ptr;

    if (unlikely(str == NULL))
        return NULL;

    n = strlen(str);
    ptr = ralloc_array(ctx, char, n + 1);
    memcpy(ptr, str, n);
    ptr[n] = '\0';
    return ptr;
}

static size_t
printf_length(const char *fmt, va_list untouched_args)
{
    int size;
    char junk;

    /* Make a copy of the va_list so the original caller can still use it */
    va_list args;
    va_copy(args, untouched_args);

#ifdef _WIN32
    /* We need to use _vcsprintf to calculate the size as vsnprintf returns -1
    * if the number of characters to write is greater than count.
    */
   size = _vscprintf(fmt, args);
   (void)junk;
#else
    size = vsnprintf(&junk, 1, fmt, args);
#endif
    assert(size >= 0);

    va_end(args);

    return size;
}

char *
ralloc_vasprintf(const void *ctx, const char *fmt, va_list args)
{
    size_t size = printf_length(fmt, args) + 1;

    char *ptr = ralloc_size(ctx, size);
    if (ptr != NULL)
        vsnprintf(ptr, size, fmt, args);

    return ptr;
}

static void *
resize(void *ptr, size_t size)
{
    ralloc_header *child, *old, *info;

    old = get_header(ptr);
    info = realloc(old, align64(size + sizeof(ralloc_header),
                                alignof(ralloc_header)));

    if (info == NULL)
        return NULL;

    /* Update parent and sibling's links to the reallocated node. */
    if (info != old && info->parent != NULL) {
        if (info->parent->child == old)
            info->parent->child = info;

        if (info->prev != NULL)
            info->prev->next = info;

        if (info->next != NULL)
            info->next->prev = info;
    }

    /* Update child->parent links for all children */
    for (child = info->child; child != NULL; child = child->next)
        child->parent = info;

    return PTR_FROM_HEADER(info);
}

bool
ralloc_vasprintf_rewrite_tail(char **str, size_t *start, const char *fmt,
                              va_list args)
{
    size_t new_length;
    char *ptr;

    assert(str != NULL);

    if (unlikely(*str == NULL)) {
        // Assuming a NULL context is probably bad, but it's expected behavior.
        *str = ralloc_vasprintf(NULL, fmt, args);
        *start = strlen(*str);
        return true;
    }

    new_length = printf_length(fmt, args);

    ptr = resize(*str, *start + new_length + 1);
    if (unlikely(ptr == NULL))
        return false;

    vsnprintf(ptr + *start, new_length + 1, fmt, args);
    *str = ptr;
    *start += new_length;
    return true;
}

bool
ralloc_vasprintf_append(char **str, const char *fmt, va_list args)
{
    size_t existing_length;
    assert(str != NULL);
    existing_length = *str ? strlen(*str) : 0;
    return ralloc_vasprintf_rewrite_tail(str, &existing_length, fmt, args);
}

bool
ralloc_asprintf_append(char **str, const char *fmt, ...)
{
    bool success;
    va_list args;
    va_start(args, fmt);
    success = ralloc_vasprintf_append(str, fmt, args);
    va_end(args);
    return success;
}

static void
unlink_block(ralloc_header *info)
{
    /* Unlink from parent & siblings */
    if (info->parent != NULL) {
        if (info->parent->child == info)
            info->parent->child = info->next;

        if (info->prev != NULL)
            info->prev->next = info->next;

        if (info->next != NULL)
            info->next->prev = info->prev;
    }
    info->parent = NULL;
    info->prev = NULL;
    info->next = NULL;
}

static void
unsafe_free(ralloc_header *info)
{
    /* Recursively free any children...don't waste time unlinking them. */
    ralloc_header *temp;
    while (info->child != NULL) {
        temp = info->child;
        info->child = temp->next;
        unsafe_free(temp);
    }

    /* Free the block itself.  Call the destructor first, if any. */
    if (info->destructor != NULL)
        info->destructor(PTR_FROM_HEADER(info));

    free(info);
}

void
ralloc_free(void *ptr)
{
    ralloc_header *info;

    if (ptr == NULL)
        return;

    info = get_header(ptr);
    unlink_block(info);
    unsafe_free(info);
}

void
SHA1Init(SHA1_CTX *context)
{

    /* SHA1 initialization constants */
    context->count = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
}

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

#if UTIL_ARCH_LITTLE_ENDIAN
# define blk0(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00) \
    |(rol(block->l[i],8)&0x00FF00FF))
#else
# define blk0(i) block->l[i]
#endif
#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15]^block->l[i&15],1))

#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

typedef union {
    uint8_t c[64];
    uint32_t l[16];
} CHAR64LONG16;

void
SHA1Transform(uint32_t state[5], const uint8_t buffer[SHA1_BLOCK_LENGTH])
{
    uint32_t a, b, c, d, e;
    uint8_t workspace[SHA1_BLOCK_LENGTH];
    CHAR64LONG16 *block = (CHAR64LONG16 *)workspace;

    (void)memcpy(block, buffer, SHA1_BLOCK_LENGTH);

    /* Copy context->state[] to working vars */
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);

    /* Add the working vars back into context.state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;

    /* Wipe variables */
    a = b = c = d = e = 0;
}

void
SHA1Update(SHA1_CTX *context, const uint8_t *data, size_t len)
{
    size_t i, j;

    j = (size_t)((context->count >> 3) & 63);
    context->count += (len << 3);
    if ((j + len) > 63) {
        (void)memcpy(&context->buffer[j], data, (i = 64-j));
        SHA1Transform(context->state, context->buffer);
        for ( ; i + 63 < len; i += 64)
            SHA1Transform(context->state, (uint8_t *)&data[i]);
        j = 0;
    } else {
        i = 0;
    }
    (void)memcpy(&context->buffer[j], &data[i], len - i);
}

void
SHA1Pad(SHA1_CTX *context)
{
    uint8_t finalcount[8];
    uint32_t i;

    for (i = 0; i < 8; i++) {
        finalcount[i] = (uint8_t)((context->count >> ((7 - (i & 7)) * 8)) & 255);	/* Endian independent */
    }
    SHA1Update(context, (uint8_t *)"\200", 1);
    while ((context->count & 504) != 448)
        SHA1Update(context, (uint8_t *)"\0", 1);
    SHA1Update(context, finalcount, 8); /* Should cause a SHA1Transform() */
}

void
SHA1Final(uint8_t digest[SHA1_DIGEST_LENGTH], SHA1_CTX *context)
{
    uint32_t i;

    SHA1Pad(context);
    for (i = 0; i < SHA1_DIGEST_LENGTH; i++) {
        digest[i] = (uint8_t)
                ((context->state[i>>2] >> ((3-(i & 3)) * 8) ) & 255);
    }
    memset(context, 0, sizeof(*context));
}

static inline void
_mesa_sha1_init(struct mesa_sha1 *ctx)
{
    SHA1Init(ctx);
}

static inline void
_mesa_sha1_update(struct mesa_sha1 *ctx, const void *data, size_t size)
{
    SHA1Update(ctx, (const unsigned char *)data, size);
}

static inline void
_mesa_sha1_final(struct mesa_sha1 *ctx, unsigned char result[20])
{
    SHA1Final(result, ctx);
}

void
_mesa_sha1_compute(const void *data, size_t size, unsigned char result[20])
{
    struct mesa_sha1 ctx;

    _mesa_sha1_init(&ctx);
    _mesa_sha1_update(&ctx, data, size);
    _mesa_sha1_final(&ctx, result);
}

static inline void
driComputeOptionsSha1(const driOptionCache *cache, unsigned char *sha1)
{
    void *ctx = ralloc_context(NULL);
    char *dri_options = ralloc_strdup(ctx, "");

    for (int i = 0; i < 1 << cache->tableSize; i++) {
        if (cache->info[i].name == NULL)
            continue;

        bool ret = false;
        switch (cache->info[i].type) {
            case DRI_BOOL:
                ret = ralloc_asprintf_append(&dri_options, "%s:%u,",
                                             cache->info[i].name,
                                             cache->values[i]._bool);
                break;
            case DRI_INT:
            case DRI_ENUM:
                ret = ralloc_asprintf_append(&dri_options, "%s:%d,",
                                             cache->info[i].name,
                                             cache->values[i]._int);
                break;
            case DRI_FLOAT:
                ret = ralloc_asprintf_append(&dri_options, "%s:%f,",
                                             cache->info[i].name,
                                             cache->values[i]._float);
                break;
            case DRI_STRING:
                ret = ralloc_asprintf_append(&dri_options, "%s:%s,",
                                             cache->info[i].name,
                                             cache->values[i]._string);
                break;
            default:
                unreachable("unsupported dri config type!");
        }

        if (!ret) {
            break;
        }
    }

    _mesa_sha1_compute(dri_options, strlen(dri_options), sha1);
    ralloc_free(ctx);
}

void
u_driconf_fill_st_options(struct st_config_options *options,
                          const struct driOptionCache *optionCache)
{
#define query_option_impl(option, type) \
   options->option = driQueryOption##type(optionCache, #option)
#define query_bool_option(option) query_option_impl(option, b)
#define query_int_option(option) query_option_impl(option, i)
#define query_string_option(option) \
   do { \
      char *option = driQueryOptionstr(optionCache, #option); \
      if (*option) \
         options->option = strdup(option); \
   } while (0)

    query_bool_option(disable_blend_func_extended);
    query_bool_option(disable_arb_gpu_shader5);
    query_bool_option(disable_glsl_line_continuations);
    query_bool_option(force_compat_shaders);
    query_bool_option(force_glsl_extensions_warn);
    query_int_option(force_glsl_version);
    query_bool_option(allow_extra_pp_tokens);
    query_bool_option(allow_glsl_extension_directive_midshader);
    query_bool_option(allow_glsl_120_subset_in_110);
    query_bool_option(allow_glsl_builtin_const_expression);
    query_bool_option(allow_glsl_relaxed_es);
    query_bool_option(allow_glsl_builtin_variable_redeclaration);
    query_bool_option(allow_higher_compat_version);
    query_bool_option(allow_glsl_compat_shaders);
    query_bool_option(glsl_ignore_write_to_readonly_var);
    query_bool_option(glsl_zero_init);
    query_bool_option(force_integer_tex_nearest);
    query_bool_option(vs_position_always_invariant);
    query_bool_option(vs_position_always_precise);
    query_bool_option(force_glsl_abs_sqrt);
    query_bool_option(allow_glsl_cross_stage_interpolation_mismatch);
    query_bool_option(do_dce_before_clip_cull_analysis);
    query_bool_option(allow_draw_out_of_order);
    query_bool_option(glthread_nop_check_framebuffer_status);
    query_bool_option(ignore_map_unsynchronized);
    query_bool_option(force_gl_names_reuse);
    query_bool_option(transcode_etc);
    query_bool_option(transcode_astc);
    query_string_option(force_gl_vendor);
    query_string_option(force_gl_renderer);
    query_string_option(mesa_extension_override);

    driComputeOptionsSha1(optionCache, options->config_options_sha1);
}

void
dri_init_options(struct dri_screen *screen)
{
    pipe_loader_config_options(screen->dev);

    struct st_config_options *options = &screen->options;
    const struct driOptionCache *optionCache = &screen->dev->option_cache;

    u_driconf_fill_st_options(options, optionCache);
}

static inline void
get_image2(__DRIdrawable *dPriv, int x, int y, int width, int height, int stride, void *data)
{
    __DRIscreen *sPriv = dPriv->driScreenPriv;
    const __DRIswrastLoaderExtension *loader = sPriv->swrast_loader;

    /* getImage2 support is only in version 3 or newer */
    if (loader->base.version < 3)
        return;

    loader->getImage2(dPriv,
                      x, y, width, height, stride,
                      data, dPriv->loaderPrivate);
}

static inline void
get_drawable_info(__DRIdrawable *dPriv, int *x, int *y, int *w, int *h)
{
    __DRIscreen *sPriv = dPriv->driScreenPriv;
    const __DRIswrastLoaderExtension *loader = sPriv->swrast_loader;

    loader->getDrawableInfo(dPriv,
                            x, y, w, h,
                            dPriv->loaderPrivate);
}

static void
drisw_get_image(struct dri_drawable *drawable,
                int x, int y, unsigned width, unsigned height, unsigned stride,
                void *data)
{
    __DRIdrawable *dPriv = drawable->dPriv;
    int draw_x, draw_y, draw_w, draw_h;

    get_drawable_info(dPriv, &draw_x, &draw_y, &draw_w, &draw_h);
    get_image2(dPriv, x, y, draw_w, draw_h, stride, data);
}

static inline void
put_image(__DRIdrawable *dPriv, void *data, unsigned width, unsigned height)
{
    __DRIscreen *sPriv = dPriv->driScreenPriv;
    const __DRIswrastLoaderExtension *loader = sPriv->swrast_loader;

    loader->putImage(dPriv, __DRI_SWRAST_IMAGE_OP_SWAP,
                     0, 0, width, height,
                     data, dPriv->loaderPrivate);
}

static inline void
put_image2(__DRIdrawable *dPriv, void *data, int x, int y,
           unsigned width, unsigned height, unsigned stride)
{
    __DRIscreen *sPriv = dPriv->driScreenPriv;
    const __DRIswrastLoaderExtension *loader = sPriv->swrast_loader;

    loader->putImage2(dPriv, __DRI_SWRAST_IMAGE_OP_SWAP,
                      x, y, width, height, stride,
                      data, dPriv->loaderPrivate);
}

static inline void
put_image_shm(__DRIdrawable *dPriv, int shmid, char *shmaddr,
              unsigned offset, unsigned offset_x, int x, int y,
              unsigned width, unsigned height, unsigned stride)
{
    __DRIscreen *sPriv = dPriv->driScreenPriv;
    const __DRIswrastLoaderExtension *loader = sPriv->swrast_loader;

    /* if we have the newer interface, don't have to add the offset_x here. */
    if (loader->base.version > 4 && loader->putImageShm2)
        loader->putImageShm2(dPriv, __DRI_SWRAST_IMAGE_OP_SWAP,
                             x, y, width, height, stride,
                             shmid, shmaddr, offset, dPriv->loaderPrivate);
    else
        loader->putImageShm(dPriv, __DRI_SWRAST_IMAGE_OP_SWAP,
                            x, y, width, height, stride,
                            shmid, shmaddr, offset + offset_x, dPriv->loaderPrivate);
}

static void
drisw_put_image(struct dri_drawable *drawable,
                void *data, unsigned width, unsigned height)
{
    __DRIdrawable *dPriv = drawable->dPriv;

    put_image(dPriv, data, width, height);
}

static void
drisw_put_image2(struct dri_drawable *drawable,
                 void *data, int x, int y, unsigned width, unsigned height,
                 unsigned stride)
{
    __DRIdrawable *dPriv = drawable->dPriv;

    put_image2(dPriv, data, x, y, width, height, stride);
}

static inline void
drisw_put_image_shm(struct dri_drawable *drawable,
                    int shmid, char *shmaddr, unsigned offset,
                    unsigned offset_x,
                    int x, int y, unsigned width, unsigned height,
                    unsigned stride)
{
    __DRIdrawable *dPriv = drawable->dPriv;

    put_image_shm(dPriv, shmid, shmaddr, offset, offset_x, x, y, width, height, stride);
}

static const struct drisw_loader_funcs drisw_lf = {
        .get_image = drisw_get_image,
        .put_image = drisw_put_image,
        .put_image2 = drisw_put_image2
};

static const struct drisw_loader_funcs drisw_shm_lf = {
        .get_image = drisw_get_image,
        .put_image = drisw_put_image,
        .put_image2 = drisw_put_image2,
        .put_image_shm = drisw_put_image_shm
};

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

int
driQueryRendererIntegerCommon(__DRIscreen *psp, int param, unsigned int *value)
{
    switch (param) {
        case __DRI2_RENDERER_VERSION: {
            static const char *const ver = PACKAGE_VERSION;
            char *endptr;
            int v[3];

            v[0] = strtol(ver, &endptr, 10);
            assert(endptr[0] == '.');
            if (endptr[0] != '.')
                return -1;

            v[1] = strtol(endptr + 1, &endptr, 10);
            assert(endptr[0] == '.');
            if (endptr[0] != '.')
                return -1;

            v[2] = strtol(endptr + 1, &endptr, 10);

            value[0] = v[0];
            value[1] = v[1];
            value[2] = v[2];
            return 0;
        }
        case __DRI2_RENDERER_PREFERRED_PROFILE:
            value[0] = (psp->max_gl_core_version != 0)
                       ? (1U << __DRI_API_OPENGL_CORE) : (1U << __DRI_API_OPENGL);
            return 0;
        case __DRI2_RENDERER_OPENGL_CORE_PROFILE_VERSION:
            value[0] = psp->max_gl_core_version / 10;
            value[1] = psp->max_gl_core_version % 10;
            return 0;
        case __DRI2_RENDERER_OPENGL_COMPATIBILITY_PROFILE_VERSION:
            value[0] = psp->max_gl_compat_version / 10;
            value[1] = psp->max_gl_compat_version % 10;
            return 0;
        case __DRI2_RENDERER_OPENGL_ES_PROFILE_VERSION:
            value[0] = psp->max_gl_es1_version / 10;
            value[1] = psp->max_gl_es1_version % 10;
            return 0;
        case __DRI2_RENDERER_OPENGL_ES2_PROFILE_VERSION:
            value[0] = psp->max_gl_es2_version / 10;
            value[1] = psp->max_gl_es2_version % 10;
            return 0;
        default:
            break;
    }

    return -1;
}

static int
dri2_query_renderer_integer(__DRIscreen *_screen, int param,
                            unsigned int *value)
{
    struct dri_screen *screen = dri_screen(_screen);

    switch (param) {
        case __DRI2_RENDERER_VENDOR_ID:
            value[0] =
                    (unsigned int)screen->base.screen->get_param(screen->base.screen,
                                                                 PIPE_CAP_VENDOR_ID);
            return 0;
        case __DRI2_RENDERER_DEVICE_ID:
            value[0] =
                    (unsigned int)screen->base.screen->get_param(screen->base.screen,
                                                                 PIPE_CAP_DEVICE_ID);
            return 0;
        case __DRI2_RENDERER_ACCELERATED:
            value[0] =
                    (unsigned int)screen->base.screen->get_param(screen->base.screen,
                                                                 PIPE_CAP_ACCELERATED);
            return 0;

        case __DRI2_RENDERER_VIDEO_MEMORY: {
            int ov = driQueryOptioni(&screen->dev->option_cache, "override_vram_size");
            value[0] =
                    (unsigned int)screen->base.screen->get_param(screen->base.screen,
                                                                 PIPE_CAP_VIDEO_MEMORY);
            if (ov >= 0)
                value[0] = MIN2(ov, value[0]);
            return 0;
        }

        case __DRI2_RENDERER_UNIFIED_MEMORY_ARCHITECTURE:
            value[0] =
                    (unsigned int)screen->base.screen->get_param(screen->base.screen,
                                                                 PIPE_CAP_UMA);
            return 0;

        case __DRI2_RENDERER_HAS_TEXTURE_3D:
            value[0] =
                    screen->base.screen->get_param(screen->base.screen,
                                                   PIPE_CAP_MAX_TEXTURE_3D_LEVELS) != 0;
            return 0;

        case __DRI2_RENDERER_HAS_FRAMEBUFFER_SRGB:
            value[0] =
                    screen->base.screen->is_format_supported(screen->base.screen,
                                                             PIPE_FORMAT_B8G8R8A8_SRGB,
                                                             PIPE_TEXTURE_2D, 0, 0,
                                                             PIPE_BIND_RENDER_TARGET);
            return 0;
        case __DRI2_RENDERER_HAS_CONTEXT_PRIORITY:
            value[0] =
                    screen->base.screen->get_param(screen->base.screen,
                                                   PIPE_CAP_CONTEXT_PRIORITY_MASK);
            if (!value[0])
                return -1;
            return 0;
        case __DRI2_RENDERER_HAS_PROTECTED_CONTENT:
            value[0] =
                    screen->base.screen->get_param(screen->base.screen,
                                                   PIPE_CAP_DEVICE_PROTECTED_CONTENT);
            if (!value[0])
                return -1;
            return 0;
        case __DRI2_RENDERER_PREFER_BACK_BUFFER_REUSE:
            value[0] =
                    screen->base.screen->get_param(screen->base.screen,
                                                   PIPE_CAP_PREFER_BACK_BUFFER_REUSE);
            return 0;
        default:
            return driQueryRendererIntegerCommon(_screen, param, value);
    }
}

static int
dri2_query_renderer_string(__DRIscreen *_screen, int param,
                           const char **value)
{
    struct dri_screen *screen = dri_screen(_screen);

    switch (param) {
        case __DRI2_RENDERER_VENDOR_ID:
            value[0] = screen->base.screen->get_vendor(screen->base.screen);
            return 0;
        case __DRI2_RENDERER_DEVICE_ID:
            value[0] = screen->base.screen->get_name(screen->base.screen);
            return 0;
        default:
            return -1;
    }
}


const __DRI2rendererQueryExtension dri2RendererQueryExtension = {
        .base = { __DRI2_RENDERER_QUERY, 1 },

        .queryInteger         = dri2_query_renderer_integer,
        .queryString          = dri2_query_renderer_string
};


float
driQueryOptionf(const driOptionCache *cache, const char *name)
{
    uint32_t i = findOption(cache, name);
    /* make sure the option is defined and has the correct type */
    assert(cache->info[i].name != NULL);
    assert(cache->info[i].type == DRI_FLOAT);
    return cache->values[i]._float;
}

unsigned char
driCheckOption(const driOptionCache *cache, const char *name,
               driOptionType type)
{
    uint32_t i = findOption(cache, name);
    return cache->info[i].name != NULL && cache->info[i].type == type;
}

static int
dri2ConfigQueryb(__DRIscreen *screen, const char *var, unsigned char *val)
{
    if (!driCheckOption(&screen->optionCache, var, DRI_BOOL))
        return -1;

    *val = driQueryOptionb(&screen->optionCache, var);

    return 0;
}

static int
dri2ConfigQueryi(__DRIscreen *screen, const char *var, int *val)
{
    if (!driCheckOption(&screen->optionCache, var, DRI_INT) &&
        !driCheckOption(&screen->optionCache, var, DRI_ENUM))
        return -1;

    *val = driQueryOptioni(&screen->optionCache, var);

    return 0;
}

static int
dri2ConfigQueryf(__DRIscreen *screen, const char *var, float *val)
{
    if (!driCheckOption(&screen->optionCache, var, DRI_FLOAT))
        return -1;

    *val = driQueryOptionf(&screen->optionCache, var);

    return 0;
}

static int
dri2ConfigQuerys(__DRIscreen *screen, const char *var, char **val)
{
    if (!driCheckOption(&screen->optionCache, var, DRI_STRING))
        return -1;

    *val = driQueryOptionstr(&screen->optionCache, var);

    return 0;
}

const __DRI2configQueryExtension dri2ConfigQueryExtension = {
        .base = { __DRI2_CONFIG_QUERY, 2 },

        .configQueryb        = dri2ConfigQueryb,
        .configQueryi        = dri2ConfigQueryi,
        .configQueryf        = dri2ConfigQueryf,
        .configQuerys        = dri2ConfigQuerys,
};

static int
dri2GalliumConfigQueryb(__DRIscreen *sPriv, const char *var,
                        unsigned char *val)
{
    struct dri_screen *screen = dri_screen(sPriv);

    if (!driCheckOption(&screen->dev->option_cache, var, DRI_BOOL))
        return dri2ConfigQueryExtension.configQueryb(sPriv, var, val);

    *val = driQueryOptionb(&screen->dev->option_cache, var);

    return 0;
}

/**
 * \brief the DRI2ConfigQueryExtension configQueryi method
 */
static int
dri2GalliumConfigQueryi(__DRIscreen *sPriv, const char *var, int *val)
{
    struct dri_screen *screen = dri_screen(sPriv);

    if (!driCheckOption(&screen->dev->option_cache, var, DRI_INT) &&
        !driCheckOption(&screen->dev->option_cache, var, DRI_ENUM))
        return dri2ConfigQueryExtension.configQueryi(sPriv, var, val);

    *val = driQueryOptioni(&screen->dev->option_cache, var);

    return 0;
}

/**
 * \brief the DRI2ConfigQueryExtension configQueryf method
 */
static int
dri2GalliumConfigQueryf(__DRIscreen *sPriv, const char *var, float *val)
{
    struct dri_screen *screen = dri_screen(sPriv);

    if (!driCheckOption(&screen->dev->option_cache, var, DRI_FLOAT))
        return dri2ConfigQueryExtension.configQueryf(sPriv, var, val);

    *val = driQueryOptionf(&screen->dev->option_cache, var);

    return 0;
}

/**
 * \brief the DRI2ConfigQueryExtension configQuerys method
 */
static int
dri2GalliumConfigQuerys(__DRIscreen *sPriv, const char *var, char **val)
{
    struct dri_screen *screen = dri_screen(sPriv);

    if (!driCheckOption(&screen->dev->option_cache, var, DRI_STRING))
        return dri2ConfigQueryExtension.configQuerys(sPriv, var, val);

    *val = driQueryOptionstr(&screen->dev->option_cache, var);

    return 0;
}

/**
 * \brief the DRI2ConfigQueryExtension struct.
 *
 * We first query the driver option cache. Then the dri2 option cache.
 */
static const __DRI2configQueryExtension dri2GalliumConfigQueryExtension = {
        .base = { __DRI2_CONFIG_QUERY, 2 },

        .configQueryb        = dri2GalliumConfigQueryb,
        .configQueryi        = dri2GalliumConfigQueryi,
        .configQueryf        = dri2GalliumConfigQueryf,
        .configQuerys        = dri2GalliumConfigQuerys,
};

const __DRInoErrorExtension dri2NoErrorExtension = {
        .base = { __DRI2_NO_ERROR, 1 },
};

const __DRI2flushControlExtension dri2FlushControlExtension = {
        .base = { __DRI2_FLUSH_CONTROL, 1 }
};

static inline void
_mesa_HashLockMutex(struct _mesa_HashTable *table)
{
    assert(table);
    simple_mtx_lock(&table->Mutex);
}

static inline void
_mesa_HashUnlockMutex(struct _mesa_HashTable *table)
{
    assert(table);
    simple_mtx_unlock(&table->Mutex);
}

static int
entry_is_free(const struct hash_entry *entry)
{
    return entry->key == NULL;
}

static int
entry_is_present(const struct hash_table *ht, struct hash_entry *entry)
{
    return entry->key != NULL && entry->key != ht->deleted_key;
}

static struct hash_entry *
hash_table_search(struct hash_table *ht, uint32_t hash, const void *key)
{
    assert(!key_pointer_is_reserved(ht, key));

    uint32_t size = ht->size;
    uint32_t start_hash_address = util_fast_urem32(hash, size, ht->size_magic);
    uint32_t double_hash = 1 + util_fast_urem32(hash, ht->rehash,
                                                ht->rehash_magic);
    uint32_t hash_address = start_hash_address;

    do {
        struct hash_entry *entry = ht->table + hash_address;

        if (entry_is_free(entry)) {
            return NULL;
        } else if (entry_is_present(ht, entry) && entry->hash == hash) {
            if (ht->key_equals_function(key, entry->key)) {
                return entry;
            }
        }

        hash_address += double_hash;
        if (hash_address >= size)
            hash_address -= size;
    } while (hash_address != start_hash_address);

    return NULL;
}

struct hash_entry *
_mesa_hash_table_search_pre_hashed(struct hash_table *ht, uint32_t hash,
                                   const void *key)
{
    assert(ht->key_hash_function == NULL || hash == ht->key_hash_function(key));
    return hash_table_search(ht, hash, key);
}

static inline void *
_mesa_HashLookup_unlocked(struct _mesa_HashTable *table, GLuint key)
{
    const struct hash_entry *entry;

    assert(table);
    assert(key);

    if (key == DELETED_KEY_VALUE)
        return table->deleted_key_data;

    entry = _mesa_hash_table_search_pre_hashed(table->ht,
                                               uint_hash(key),
                                               uint_key(key));
    if (!entry)
        return NULL;

    return entry->data;
}

void *
_mesa_HashLookup(struct _mesa_HashTable *table, GLuint key)
{
    void *res;
    _mesa_HashLockMutex(table);
    res = _mesa_HashLookup_unlocked(table, key);
    _mesa_HashUnlockMutex(table);
    return res;
}

struct gl_renderbuffer *
_mesa_lookup_renderbuffer(struct gl_context *ctx, GLuint id)
{
    struct gl_renderbuffer *rb;

    if (id == 0)
        return NULL;

    rb = (struct gl_renderbuffer *)
            _mesa_HashLookup(ctx->Shared->RenderBuffers, id);
    return rb;
}

uint32_t
driGLFormatToImageFormat(mesa_format format)
{
    for (size_t i = 0; i < ARRAY_SIZE(format_mapping); i++)
        if (format_mapping[i].mesa_format == format)
            return format_mapping[i].image_format;

    return __DRI_IMAGE_FORMAT_NONE;
}

const struct dri2_format_mapping *
dri2_get_mapping_by_format(int format)
{
    if (format == __DRI_IMAGE_FORMAT_NONE)
        return NULL;

    for (unsigned i = 0; i < ARRAY_SIZE(dri2_format_table); i++) {
        if (dri2_format_table[i].dri_format == format)
            return &dri2_format_table[i];
    }

    return NULL;
}

__DRIimage *
dri2_create_image_from_renderbuffer2(__DRIcontext *context,
                                     int renderbuffer, void *loaderPrivate,
                                     unsigned *error)
{
    struct st_context *st_ctx = (struct st_context *)dri_context(context)->st;
    struct gl_context *ctx = st_ctx->ctx;
    struct pipe_context *p_ctx = st_ctx->pipe;
    struct gl_renderbuffer *rb;
    struct pipe_resource *tex;
    __DRIimage *img;

    /* Section 3.9 (EGLImage Specification and Management) of the EGL 1.5
     * specification says:
     *
     *   "If target is EGL_GL_RENDERBUFFER and buffer is not the name of a
     *    renderbuffer object, or if buffer is the name of a multisampled
     *    renderbuffer object, the error EGL_BAD_PARAMETER is generated."
     *
     *   "If target is EGL_GL_TEXTURE_2D , EGL_GL_TEXTURE_CUBE_MAP_*,
     *    EGL_GL_RENDERBUFFER or EGL_GL_TEXTURE_3D and buffer refers to the
     *    default GL texture object (0) for the corresponding GL target, the
     *    error EGL_BAD_PARAMETER is generated."
     *   (rely on _mesa_lookup_renderbuffer returning NULL in this case)
     */
    rb = _mesa_lookup_renderbuffer(ctx, renderbuffer);
    if (!rb || rb->NumSamples > 0) {
        *error = __DRI_IMAGE_ERROR_BAD_PARAMETER;
        return NULL;
    }

    tex = rb->texture;
    if (!tex) {
        *error = __DRI_IMAGE_ERROR_BAD_PARAMETER;
        return NULL;
    }

    img = CALLOC_STRUCT(__DRIimageRec);
    if (!img) {
        *error = __DRI_IMAGE_ERROR_BAD_ALLOC;
        return NULL;
    }

    img->dri_format = driGLFormatToImageFormat(rb->Format);
    img->loader_private = loaderPrivate;
    img->sPriv = context->driScreenPriv;

    pipe_resource_reference(&img->texture, tex);

    /* If the resource supports EGL_MESA_image_dma_buf_export, make sure that
     * it's in a shareable state. Do this now while we still have the access to
     * the context.
     */
    if (dri2_get_mapping_by_format(img->dri_format))
        p_ctx->flush_resource(p_ctx, tex);

    ctx->Shared->HasExternallySharedImages = true;
    *error = __DRI_IMAGE_ERROR_SUCCESS;
    return img;
}

__DRIimage *
dri2_create_image_from_renderbuffer(__DRIcontext *context,
                                    int renderbuffer, void *loaderPrivate)
{
    unsigned error;
    return dri2_create_image_from_renderbuffer2(context, renderbuffer,
                                                loaderPrivate, &error);
}

struct gl_texture_object *
_mesa_lookup_texture(struct gl_context *ctx, GLuint id)
{
    return (struct gl_texture_object *)
            _mesa_HashLookup(ctx->Shared->TexObjects, id);
}

static inline struct pipe_resource *
st_get_texobj_resource(struct gl_texture_object *texObj)
{
    return texObj ? texObj->pt : NULL;
}

enum base_mipmap { BASE, MIPMAP };

static void
incomplete(struct gl_texture_object *t, enum base_mipmap bm,
           const char *fmt, ...)
{
    if (MESA_DEBUG_FLAGS & DEBUG_INCOMPLETE_TEXTURE) {
        va_list args;
        char s[100];

        va_start(args, fmt);
        vsnprintf(s, sizeof(s), fmt, args);
        va_end(args);

        _mesa_debug(NULL, "Texture Obj %d incomplete because: %s\n", t->Name, s);
    }

    if (bm == BASE)
        t->_BaseComplete = GL_FALSE;
    t->_MipmapComplete = GL_FALSE;
}

static bool
valid_filter_for_float(const struct gl_context *ctx,
                       const struct gl_texture_object *obj)
{
    switch (obj->Sampler.Attrib.MagFilter) {
        case GL_LINEAR:
            if (obj->_IsHalfFloat && !ctx->Extensions.OES_texture_half_float_linear) {
                return false;
            } else if (obj->_IsFloat && !ctx->Extensions.OES_texture_float_linear) {
                return false;
            }
            FALLTHROUGH;
        case GL_NEAREST:
        case GL_NEAREST_MIPMAP_NEAREST:
            break;
        default:
            unreachable("Invalid mag filter");
    }

    switch (obj->Sampler.Attrib.MinFilter) {
        case GL_LINEAR:
        case GL_NEAREST_MIPMAP_LINEAR:
        case GL_LINEAR_MIPMAP_NEAREST:
        case GL_LINEAR_MIPMAP_LINEAR:
            if (obj->_IsHalfFloat && !ctx->Extensions.OES_texture_half_float_linear) {
                return false;
            } else if (obj->_IsFloat && !ctx->Extensions.OES_texture_float_linear) {
                return false;
            }
            FALLTHROUGH;
        case GL_NEAREST:
        case GL_NEAREST_MIPMAP_NEAREST:
            break;
        default:
            unreachable("Invalid min filter");
    }

    return true;
}

static inline bool
_mesa_is_gles(const struct gl_context *ctx)
{
    return ctx->API == API_OPENGLES || ctx->API == API_OPENGLES2;
}

#ifdef HAVE___BUILTIN_FFS
#elif defined(_MSC_VER) && (_M_IX86 || _M_ARM || _M_AMD64 || _M_IA64)
#else
int
ffs(int i)
{
    int bit = 0;
    if (!i)
        return bit;
    if (!(i & 0xffff)) {
        bit += 16;
        i >>= 16;
    }
    if (!(i & 0xff)) {
        bit += 8;
        i >>= 8;
    }
    if (!(i & 0xf)) {
        bit += 4;
        i >>= 4;
    }
    if (!(i & 0x3)) {
        bit += 2;
        i >>= 2;
    }
    if (!(i & 0x1))
        bit += 1;
    return bit + 1;
}
#endif

static inline bool
util_is_power_of_two_or_zero(unsigned v)
{
    return (v & (v - 1)) == 0;
}

static inline unsigned
util_next_power_of_two(unsigned x)
{
#if defined(HAVE___BUILTIN_CLZ)
    if (x <= 1)
       return 1;

   return (1 << ((sizeof(unsigned) * 8) - __builtin_clz(x - 1)));
#else
    unsigned val = x;

    if (x <= 1)
        return 1;

    if (util_is_power_of_two_or_zero(x))
        return x;

    val--;
    val = (val >> 1) | val;
    val = (val >> 2) | val;
    val = (val >> 4) | val;
    val = (val >> 8) | val;
    val = (val >> 16) | val;
    val++;
    return val;
#endif
}

static inline bool
_mesa_has_texture_cube_map_array(const struct gl_context *ctx)
{
    return _mesa_has_ARB_texture_cube_map_array(ctx) ||
           _mesa_has_OES_texture_cube_map_array(ctx);
}

static inline bool
_mesa_is_desktop_gl(const struct gl_context *ctx)
{
    return ctx->API == API_OPENGL_COMPAT || ctx->API == API_OPENGL_CORE;
}

static inline bool
_mesa_is_gles31(const struct gl_context *ctx)
{
    return ctx->API == API_OPENGLES2 && ctx->Version >= 31;
}

GLint
_mesa_max_texture_levels(const struct gl_context *ctx, GLenum target)
{
    switch (target) {
        case GL_TEXTURE_1D:
        case GL_PROXY_TEXTURE_1D:
        case GL_TEXTURE_2D:
        case GL_PROXY_TEXTURE_2D:
            return ffs(util_next_power_of_two(ctx->Const.MaxTextureSize));
        case GL_TEXTURE_3D:
        case GL_PROXY_TEXTURE_3D:
            return ctx->Const.Max3DTextureLevels;
        case GL_TEXTURE_CUBE_MAP:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        case GL_PROXY_TEXTURE_CUBE_MAP:
            return ctx->Const.MaxCubeTextureLevels;
        case GL_TEXTURE_RECTANGLE_NV:
        case GL_PROXY_TEXTURE_RECTANGLE_NV:
            return ctx->Extensions.NV_texture_rectangle ? 1 : 0;
        case GL_TEXTURE_1D_ARRAY_EXT:
        case GL_PROXY_TEXTURE_1D_ARRAY_EXT:
        case GL_TEXTURE_2D_ARRAY_EXT:
        case GL_PROXY_TEXTURE_2D_ARRAY_EXT:
            return ctx->Extensions.EXT_texture_array
                   ? ffs(util_next_power_of_two(ctx->Const.MaxTextureSize)) : 0;
        case GL_TEXTURE_CUBE_MAP_ARRAY:
        case GL_PROXY_TEXTURE_CUBE_MAP_ARRAY:
            return _mesa_has_texture_cube_map_array(ctx)
                   ? ctx->Const.MaxCubeTextureLevels : 0;
        case GL_TEXTURE_BUFFER:
            return (_mesa_has_ARB_texture_buffer_object(ctx) ||
                    _mesa_has_OES_texture_buffer(ctx)) ? 1 : 0;
        case GL_TEXTURE_2D_MULTISAMPLE:
        case GL_PROXY_TEXTURE_2D_MULTISAMPLE:
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
        case GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY:
            return (_mesa_is_desktop_gl(ctx) || _mesa_is_gles31(ctx))
                   && ctx->Extensions.ARB_texture_multisample
                   ? 1 : 0;
        case GL_TEXTURE_EXTERNAL_OES:
            return _mesa_has_OES_EGL_image_external(ctx) ? 1 : 0;
        default:
            return 0; /* bad target */
    }
}

static inline GLuint
_mesa_num_tex_faces(GLenum target)
{
    switch (target) {
        case GL_TEXTURE_CUBE_MAP:
        case GL_PROXY_TEXTURE_CUBE_MAP:
            return 6;
        default:
            return 1;
    }
}

struct hash_entry *
_mesa_hash_table_search(struct hash_table *ht, const void *key)
{
    assert(ht->key_hash_function);
    return hash_table_search(ht, ht->key_hash_function(key), key);
}

void
_mesa_test_texobj_completeness( const struct gl_context *ctx,
                                struct gl_texture_object *t )
{
    const GLint baseLevel = t->Attrib.BaseLevel;
    const struct gl_texture_image *baseImage;
    GLint maxLevels = 0;

    /* We'll set these to FALSE if tests fail below */
    t->_BaseComplete = GL_TRUE;
    t->_MipmapComplete = GL_TRUE;

    if (t->Target == GL_TEXTURE_BUFFER) {
        /* Buffer textures are always considered complete.  The obvious case where
         * they would be incomplete (no BO attached) is actually specced to be
         * undefined rendering results.
         */
        return;
    }

    /* Detect cases where the application set the base level to an invalid
     * value.
     */
    if ((baseLevel < 0) || (baseLevel >= MAX_TEXTURE_LEVELS)) {
        incomplete(t, BASE, "base level = %d is invalid", baseLevel);
        return;
    }

    if (t->Attrib.MaxLevel < baseLevel) {
        incomplete(t, MIPMAP, "MAX_LEVEL (%d) < BASE_LEVEL (%d)",
                   t->Attrib.MaxLevel, baseLevel);
        return;
    }

    baseImage = t->Image[0][baseLevel];

    /* Always need the base level image */
    if (!baseImage) {
        incomplete(t, BASE, "Image[baseLevel=%d] == NULL", baseLevel);
        return;
    }

    /* Check width/height/depth for zero */
    if (baseImage->Width == 0 ||
        baseImage->Height == 0 ||
        baseImage->Depth == 0) {
        incomplete(t, BASE, "texture width or height or depth = 0");
        return;
    }

    /* Check if the texture values are integer */
    {
        GLenum datatype = _mesa_get_format_datatype(baseImage->TexFormat);
        t->_IsIntegerFormat = datatype == GL_INT || datatype == GL_UNSIGNED_INT;
    }

    /* Check if the texture type is Float or HalfFloatOES and ensure Min and Mag
     * filters are supported in this case.
     */
    if (_mesa_is_gles(ctx) && !valid_filter_for_float(ctx, t)) {
        incomplete(t, BASE, "Filter is not supported with Float types.");
        return;
    }

    maxLevels = _mesa_max_texture_levels(ctx, t->Target);
    if (maxLevels == 0) {
        _mesa_problem(ctx, "Bad t->Target in _mesa_test_texobj_completeness");
        return;
    }

    assert(maxLevels > 0);

    t->_MaxLevel = MIN3(t->Attrib.MaxLevel,
    /* 'p' in the GL spec */
                        (int) (baseLevel + baseImage->MaxNumLevels - 1),
    /* 'q' in the GL spec */
                        maxLevels - 1);

    if (t->Immutable) {
        /* Adjust max level for views: the data store may have more levels than
         * the view exposes.
         */
        t->_MaxLevel = MAX2(MIN2(t->_MaxLevel, t->Attrib.NumLevels - 1), 0);
    }

    /* Compute _MaxLambda = q - p in the spec used during mipmapping */
    t->_MaxLambda = (GLfloat) (t->_MaxLevel - baseLevel);

    if (t->Immutable) {
        /* This texture object was created with glTexStorage1/2/3D() so we
         * know that all the mipmap levels are the right size and all cube
         * map faces are the same size.
         * We don't need to do any of the additional checks below.
         */
        return;
    }

    if (t->Target == GL_TEXTURE_CUBE_MAP) {
        /* Make sure that all six cube map level 0 images are the same size and
         * format.
         * Note:  we know that the image's width==height (we enforce that
         * at glTexImage time) so we only need to test the width here.
         */
        GLuint face;
        assert(baseImage->Width2 == baseImage->Height);
        for (face = 1; face < 6; face++) {
            assert(t->Image[face][baseLevel] == NULL ||
                   t->Image[face][baseLevel]->Width2 ==
                   t->Image[face][baseLevel]->Height2);
            if (t->Image[face][baseLevel] == NULL ||
                t->Image[face][baseLevel]->Width2 != baseImage->Width2) {
                incomplete(t, BASE, "Cube face missing or mismatched size");
                return;
            }
            if (t->Image[face][baseLevel]->InternalFormat !=
                baseImage->InternalFormat) {
                incomplete(t, BASE, "Cube face format mismatch");
                return;
            }
            if (t->Image[face][baseLevel]->Border != baseImage->Border) {
                incomplete(t, BASE, "Cube face border size mismatch");
                return;
            }
        }
    }

    /*
     * Do mipmap consistency checking.
     * Note: we don't care about the current texture sampler state here.
     * To determine texture completeness we'll either look at _BaseComplete
     * or _MipmapComplete depending on the current minification filter mode.
     */
    {
        GLint i;
        const GLint minLevel = baseLevel;
        const GLint maxLevel = t->_MaxLevel;
        const GLuint numFaces = _mesa_num_tex_faces(t->Target);
        GLuint width, height, depth, face;

        if (minLevel > maxLevel) {
            incomplete(t, MIPMAP, "minLevel > maxLevel");
            return;
        }

        /* Get the base image's dimensions */
        width = baseImage->Width2;
        height = baseImage->Height2;
        depth = baseImage->Depth2;

        /* Note: this loop will be a no-op for RECT, BUFFER, EXTERNAL,
         * MULTISAMPLE and MULTISAMPLE_ARRAY textures
         */
        for (i = baseLevel + 1; i < maxLevels; i++) {
            /* Compute the expected size of image at level[i] */
            if (width > 1) {
                width /= 2;
            }
            if (height > 1 && t->Target != GL_TEXTURE_1D_ARRAY) {
                height /= 2;
            }
            if (depth > 1 && t->Target != GL_TEXTURE_2D_ARRAY
                && t->Target != GL_TEXTURE_CUBE_MAP_ARRAY) {
                depth /= 2;
            }

            /* loop over cube faces (or single face otherwise) */
            for (face = 0; face < numFaces; face++) {
                if (i >= minLevel && i <= maxLevel) {
                    const struct gl_texture_image *img = t->Image[face][i];

                    if (!img) {
                        incomplete(t, MIPMAP, "TexImage[%d] is missing", i);
                        return;
                    }
                    if (img->InternalFormat != baseImage->InternalFormat) {
                        incomplete(t, MIPMAP, "Format[i] != Format[baseLevel]");
                        return;
                    }
                    if (img->Border != baseImage->Border) {
                        incomplete(t, MIPMAP, "Border[i] != Border[baseLevel]");
                        return;
                    }
                    if (img->Width2 != width) {
                        incomplete(t, MIPMAP, "TexImage[%d] bad width %u", i,
                                   img->Width2);
                        return;
                    }
                    if (img->Height2 != height) {
                        incomplete(t, MIPMAP, "TexImage[%d] bad height %u", i,
                                   img->Height2);
                        return;
                    }
                    if (img->Depth2 != depth) {
                        incomplete(t, MIPMAP, "TexImage[%d] bad depth %u", i,
                                   img->Depth2);
                        return;
                    }
                }
            }

            if (width == 1 && height == 1 && depth == 1) {
                return;  /* found smallest needed mipmap, all done! */
            }
        }
    }
}

__DRIimage *
dri2_create_from_texture(__DRIcontext *context, int target, unsigned texture,
                         int depth, int level, unsigned *error,
                         void *loaderPrivate)
{
    __DRIimage *img;
    struct st_context *st_ctx = (struct st_context *)dri_context(context)->st;
    struct gl_context *ctx = st_ctx->ctx;
    struct pipe_context *p_ctx = st_ctx->pipe;
    struct gl_texture_object *obj;
    struct pipe_resource *tex;
    GLuint face = 0;

    obj = _mesa_lookup_texture(ctx, texture);
    if (!obj || obj->Target != target) {
        *error = __DRI_IMAGE_ERROR_BAD_PARAMETER;
        return NULL;
    }

    tex = st_get_texobj_resource(obj);
    if (!tex) {
        *error = __DRI_IMAGE_ERROR_BAD_PARAMETER;
        return NULL;
    }

    if (target == GL_TEXTURE_CUBE_MAP)
        face = depth;

    _mesa_test_texobj_completeness(ctx, obj);
    if (!obj->_BaseComplete || (level > 0 && !obj->_MipmapComplete)) {
        *error = __DRI_IMAGE_ERROR_BAD_PARAMETER;
        return NULL;
    }

    if (level < obj->Attrib.BaseLevel || level > obj->_MaxLevel) {
        *error = __DRI_IMAGE_ERROR_BAD_MATCH;
        return NULL;
    }

    if (target == GL_TEXTURE_3D && obj->Image[face][level]->Depth < depth) {
        *error = __DRI_IMAGE_ERROR_BAD_MATCH;
        return NULL;
    }

    img = CALLOC_STRUCT(__DRIimageRec);
    if (!img) {
        *error = __DRI_IMAGE_ERROR_BAD_ALLOC;
        return NULL;
    }

    img->level = level;
    img->layer = depth;
    img->dri_format = driGLFormatToImageFormat(obj->Image[face][level]->TexFormat);

    img->loader_private = loaderPrivate;
    img->sPriv = context->driScreenPriv;

    pipe_resource_reference(&img->texture, tex);

    /* If the resource supports EGL_MESA_image_dma_buf_export, make sure that
     * it's in a shareable state. Do this now while we still have the access to
     * the context.
     */
    if (dri2_get_mapping_by_format(img->dri_format))
        p_ctx->flush_resource(p_ctx, tex);

    ctx->Shared->HasExternallySharedImages = true;
    *error = __DRI_IMAGE_ERROR_SUCCESS;
    return img;
}

void
dri2_destroy_image(__DRIimage *img)
{
    const __DRIimageLoaderExtension *imgLoader = img->sPriv->image.loader;
    const __DRIdri2LoaderExtension *dri2Loader = img->sPriv->dri2.loader;

    if (imgLoader && imgLoader->base.version >= 4 &&
        imgLoader->destroyLoaderImageState) {
        imgLoader->destroyLoaderImageState(img->loader_private);
    } else if (dri2Loader && dri2Loader->base.version >= 5 &&
               dri2Loader->destroyLoaderImageState) {
        dri2Loader->destroyLoaderImageState(img->loader_private);
    }

    pipe_resource_reference(&img->texture, NULL);
    FREE(img);
}

static const __DRIrobustnessExtension dri2Robustness = {
        .base = { __DRI2_ROBUSTNESS, 1 }
};

static __DRIimageExtension driSWImageExtension = {
        .base = { __DRI_IMAGE, 6 },

        .createImageFromRenderbuffer  = dri2_create_image_from_renderbuffer,
        .createImageFromTexture = dri2_create_from_texture,
        .destroyImage = dri2_destroy_image,
};

struct dri2_fence {
    struct dri_screen *driscreen;
    struct pipe_fence_handle *pipe_fence;
    void *cl_event;
};

static void *
dri2_create_fence(__DRIcontext *_ctx)
{
    struct st_context_iface *stapi = dri_context(_ctx)->st;
    struct dri2_fence *fence = CALLOC_STRUCT(dri2_fence);

    if (!fence)
        return NULL;

    stapi->flush(stapi, 0, &fence->pipe_fence, NULL, NULL);

    if (!fence->pipe_fence) {
        FREE(fence);
        return NULL;
    }

    fence->driscreen = dri_screen(_ctx->driScreenPriv);
    return fence;
}

static void *
dri2_create_fence_fd(__DRIcontext *_ctx, int fd)
{
    struct st_context_iface *stapi = dri_context(_ctx)->st;
    struct pipe_context *ctx = stapi->pipe;
    struct dri2_fence *fence = CALLOC_STRUCT(dri2_fence);

    if (fd == -1) {
        /* exporting driver created fence, flush: */
        stapi->flush(stapi, ST_FLUSH_FENCE_FD, &fence->pipe_fence, NULL, NULL);
    } else {
        /* importing a foreign fence fd: */
        ctx->create_fence_fd(ctx, &fence->pipe_fence, fd, PIPE_FD_TYPE_NATIVE_SYNC);
    }
    if (!fence->pipe_fence) {
        FREE(fence);
        return NULL;
    }

    fence->driscreen = dri_screen(_ctx->driScreenPriv);
    return fence;
}

static int
dri2_get_fence_fd(__DRIscreen *_screen, void *_fence)
{
    struct dri_screen *driscreen = dri_screen(_screen);
    struct pipe_screen *screen = driscreen->base.screen;
    struct dri2_fence *fence = (struct dri2_fence*)_fence;

    return screen->fence_get_fd(screen, fence->pipe_fence);
}

static bool
dri2_load_opencl_interop(struct dri_screen *screen)
{
#if defined(RTLD_DEFAULT)
    bool success;

   mtx_lock(&screen->opencl_func_mutex);

   if (dri2_is_opencl_interop_loaded_locked(screen)) {
      mtx_unlock(&screen->opencl_func_mutex);
      return true;
   }

   screen->opencl_dri_event_add_ref =
      dlsym(RTLD_DEFAULT, "opencl_dri_event_add_ref");
   screen->opencl_dri_event_release =
      dlsym(RTLD_DEFAULT, "opencl_dri_event_release");
   screen->opencl_dri_event_wait =
      dlsym(RTLD_DEFAULT, "opencl_dri_event_wait");
   screen->opencl_dri_event_get_fence =
      dlsym(RTLD_DEFAULT, "opencl_dri_event_get_fence");

   success = dri2_is_opencl_interop_loaded_locked(screen);
   mtx_unlock(&screen->opencl_func_mutex);
   return success;
#else
    return false;
#endif
}

static void *
dri2_get_fence_from_cl_event(__DRIscreen *_screen, intptr_t cl_event)
{
    struct dri_screen *driscreen = dri_screen(_screen);
    struct dri2_fence *fence;

    if (!dri2_load_opencl_interop(driscreen))
        return NULL;

    fence = CALLOC_STRUCT(dri2_fence);
    if (!fence)
        return NULL;

    fence->cl_event = (void*)cl_event;

    if (!driscreen->opencl_dri_event_add_ref(fence->cl_event)) {
        free(fence);
        return NULL;
    }

    fence->driscreen = driscreen;
    return fence;
}

static void
dri2_destroy_fence(__DRIscreen *_screen, void *_fence)
{
    struct dri_screen *driscreen = dri_screen(_screen);
    struct pipe_screen *screen = driscreen->base.screen;
    struct dri2_fence *fence = (struct dri2_fence*)_fence;

    if (fence->pipe_fence)
        screen->fence_reference(screen, &fence->pipe_fence, NULL);
    else if (fence->cl_event)
        driscreen->opencl_dri_event_release(fence->cl_event);
    else
        assert(0);

    FREE(fence);
}

static GLboolean
dri2_client_wait_sync(__DRIcontext *_ctx, void *_fence, unsigned flags,
                      uint64_t timeout)
{
    struct dri2_fence *fence = (struct dri2_fence*)_fence;
    struct dri_screen *driscreen = fence->driscreen;
    struct pipe_screen *screen = driscreen->base.screen;

    /* No need to flush. The context was flushed when the fence was created. */

    if (fence->pipe_fence)
        return screen->fence_finish(screen, NULL, fence->pipe_fence, timeout);
    else if (fence->cl_event) {
        struct pipe_fence_handle *pipe_fence =
                driscreen->opencl_dri_event_get_fence(fence->cl_event);

        if (pipe_fence)
            return screen->fence_finish(screen, NULL, pipe_fence, timeout);
        else
            return driscreen->opencl_dri_event_wait(fence->cl_event, timeout);
    }
    else {
        assert(0);
        return false;
    }
}

static void
dri2_server_wait_sync(__DRIcontext *_ctx, void *_fence, unsigned flags)
{
    struct pipe_context *ctx = dri_context(_ctx)->st->pipe;
    struct dri2_fence *fence = (struct dri2_fence*)_fence;

    /* We might be called here with a NULL fence as a result of WaitSyncKHR
     * on a EGL_KHR_reusable_sync fence. Nothing to do here in such case.
     */
    if (!fence)
        return;

    if (ctx->fence_server_sync)
        ctx->fence_server_sync(ctx, fence->pipe_fence);
}

static unsigned dri2_fence_get_caps(__DRIscreen *_screen)
{
    struct dri_screen *driscreen = dri_screen(_screen);
    struct pipe_screen *screen = driscreen->base.screen;
    unsigned caps = 0;

    if (screen->get_param(screen, PIPE_CAP_NATIVE_FENCE_FD))
        caps |= __DRI_FENCE_CAP_NATIVE_FD;

    return caps;
}

const __DRI2fenceExtension dri2FenceExtension = {
        .base = { __DRI2_FENCE, 2 },

        .create_fence = dri2_create_fence,
        .get_fence_from_cl_event = dri2_get_fence_from_cl_event,
        .destroy_fence = dri2_destroy_fence,
        .client_wait_sync = dri2_client_wait_sync,
        .server_wait_sync = dri2_server_wait_sync,
        .get_capabilities = dri2_fence_get_caps,
        .create_fence_fd = dri2_create_fence_fd,
        .get_fence_fd = dri2_get_fence_fd,
};

static const __DRIextension *drisw_screen_extensions[] = {
        &driTexBufferExtension.base,
        &dri2RendererQueryExtension.base,
        &dri2ConfigQueryExtension.base,
        &dri2FenceExtension.base,
        &dri2NoErrorExtension.base,
        &driSWImageExtension.base,
        &dri2FlushControlExtension.base,
        NULL
};

static const __DRIextension *drisw_robust_screen_extensions[] = {
        &driTexBufferExtension.base,
        &dri2RendererQueryExtension.base,
        &dri2ConfigQueryExtension.base,
        &dri2FenceExtension.base,
        &dri2NoErrorExtension.base,
        &dri2Robustness.base,
        &driSWImageExtension.base,
        &dri2FlushControlExtension.base,
        NULL
};


static const __DRIconfig **
drisw_init_screen(__DRIscreen * sPriv)
{
    const __DRIswrastLoaderExtension *loader = sPriv->swrast_loader;
    const __DRIconfig **configs;
    struct dri_screen *screen;
    struct pipe_screen *pscreen = NULL;
    const struct drisw_loader_funcs *lf = &drisw_lf;

    screen = CALLOC_STRUCT(dri_screen);
    if (!screen)
        return NULL;

    screen->sPriv = sPriv;
    screen->fd = -1;

    screen->swrast_no_present = debug_get_option_swrast_no_present();

    sPriv->driverPrivate = (void *)screen;

    if (loader->base.version >= 4) {
        if (loader->putImageShm)
            lf = &drisw_shm_lf;
    }

    if (pipe_loader_sw_probe_dri(&screen->dev, lf)) {
        pscreen = pipe_loader_create_screen(screen->dev);
        dri_init_options(screen);
    }

    if (!pscreen)
        goto fail;

    configs = dri_init_screen_helper(screen, pscreen);
    if (!configs)
        goto fail;

    if (pscreen->get_param(pscreen, PIPE_CAP_DEVICE_RESET_STATUS_QUERY)) {
        sPriv->extensions = drisw_robust_screen_extensions;
        screen->has_reset_status_query = true;
    }
    else
        sPriv->extensions = drisw_screen_extensions;
    screen->lookup_egl_image = dri2_lookup_egl_image;

    const __DRIimageLookupExtension *image = sPriv->dri2.image;
    if (image &&
        image->base.version >= 2 &&
        image->validateEGLImage &&
        image->lookupEGLImageValidated) {
        screen->validate_egl_image = dri2_validate_egl_image;
        screen->lookup_egl_image_validated = dri2_lookup_egl_image_validated;
    }

    return configs;
    fail:
    dri_destroy_screen_helper(screen);
    if (screen->dev)
        pipe_loader_release(&screen->dev, 1);
    FREE(screen);
    return NULL;
}

static inline const char *util_format_name(enum pipe_format format)
{
    const struct util_format_description *desc = util_format_description(format);

    assert(desc);
    if (!desc) {
        return "PIPE_FORMAT_???";
    }

    return desc->name;
}

void
debug_describe_surface(char* buf, const struct pipe_surface *ptr)
{
    char res[128];
    debug_describe_resource(res, ptr->texture);
    sprintf(buf, "pipe_surface<%s,%u,%u,%u>", res, ptr->u.tex.level, ptr->u.tex.first_layer, ptr->u.tex.last_layer);
}

static inline void
pipe_surface_release(struct pipe_context *pipe, struct pipe_surface **ptr)
{
    struct pipe_surface *old = *ptr;

    if (pipe_reference_described(&old->reference, NULL,
                                 (debug_reference_descriptor)
                                         debug_describe_surface))
        pipe->surface_destroy(pipe, old);
    *ptr = NULL;
}

static inline void
pipe_surface_release_no_context(struct pipe_surface **ptr)
{
    struct pipe_surface *surf = *ptr;

    if (pipe_reference_described(&surf->reference, NULL,
                                 (debug_reference_descriptor)
                                         debug_describe_surface)) {
        /* trivially destroy pipe_surface */
        pipe_resource_reference(&surf->texture, NULL);
        free(surf);
    }
    *ptr = NULL;
}

static void
delete_renderbuffer(struct gl_context *ctx, struct gl_renderbuffer *rb)
{
    if (ctx) {
        pipe_surface_release(ctx->pipe, &rb->surface_srgb);
        pipe_surface_release(ctx->pipe, &rb->surface_linear);
    } else {
        pipe_surface_release_no_context(&rb->surface_srgb);
        pipe_surface_release_no_context(&rb->surface_linear);
    }
    rb->surface = NULL;
    pipe_resource_reference(&rb->texture, NULL);
    free(rb->data);
    free(rb->Label);
    free(rb);
}

static inline bool
_mesa_is_gles3(const struct gl_context *ctx)
{
    return ctx->API == API_OPENGLES2 && ctx->Version >= 30;
}

GLenum
_mesa_base_fbo_format(const struct gl_context *ctx, GLenum internalFormat)
{
    /*
    * Notes: some formats such as alpha, luminance, etc. were added
    * with GL_ARB_framebuffer_object.
    */
    switch (internalFormat) {
        case GL_ALPHA:
        case GL_ALPHA4:
        case GL_ALPHA8:
        case GL_ALPHA12:
        case GL_ALPHA16:
            return (ctx->API == API_OPENGL_COMPAT &&
                    ctx->Extensions.ARB_framebuffer_object) ? GL_ALPHA : 0;
        case GL_LUMINANCE:
        case GL_LUMINANCE4:
        case GL_LUMINANCE8:
        case GL_LUMINANCE12:
        case GL_LUMINANCE16:
            return (ctx->API == API_OPENGL_COMPAT &&
                    ctx->Extensions.ARB_framebuffer_object) ? GL_LUMINANCE : 0;
        case GL_LUMINANCE_ALPHA:
        case GL_LUMINANCE4_ALPHA4:
        case GL_LUMINANCE6_ALPHA2:
        case GL_LUMINANCE8_ALPHA8:
        case GL_LUMINANCE12_ALPHA4:
        case GL_LUMINANCE12_ALPHA12:
        case GL_LUMINANCE16_ALPHA16:
            return (ctx->API == API_OPENGL_COMPAT &&
                    ctx->Extensions.ARB_framebuffer_object) ? GL_LUMINANCE_ALPHA : 0;
        case GL_INTENSITY:
        case GL_INTENSITY4:
        case GL_INTENSITY8:
        case GL_INTENSITY12:
        case GL_INTENSITY16:
            return (ctx->API == API_OPENGL_COMPAT &&
                    ctx->Extensions.ARB_framebuffer_object) ? GL_INTENSITY : 0;
        case GL_RGB8:
            return GL_RGB;
        case GL_RGB:
        case GL_R3_G3_B2:
        case GL_RGB4:
        case GL_RGB5:
        case GL_RGB10:
        case GL_RGB12:
        case GL_RGB16:
            return _mesa_is_desktop_gl(ctx) ? GL_RGB : 0;
        case GL_SRGB8_EXT:
            return _mesa_is_desktop_gl(ctx) ? GL_RGB : 0;
        case GL_RGBA4:
        case GL_RGB5_A1:
        case GL_RGBA8:
            return GL_RGBA;
        case GL_RGBA:
        case GL_RGBA2:
        case GL_RGBA12:
            return _mesa_is_desktop_gl(ctx) ? GL_RGBA : 0;
        case GL_RGBA16:
            return _mesa_is_desktop_gl(ctx) || _mesa_has_EXT_texture_norm16(ctx)
                   ? GL_RGBA : 0;
        case GL_RGB10_A2:
        case GL_SRGB8_ALPHA8_EXT:
            return _mesa_is_desktop_gl(ctx) || _mesa_is_gles3(ctx) ? GL_RGBA : 0;
        case GL_STENCIL_INDEX:
        case GL_STENCIL_INDEX1_EXT:
        case GL_STENCIL_INDEX4_EXT:
        case GL_STENCIL_INDEX16_EXT:
            /* There are extensions for GL_STENCIL_INDEX1 and GL_STENCIL_INDEX4 in
       * OpenGL ES, but Mesa does not currently support them.
       */
            return _mesa_is_desktop_gl(ctx) ? GL_STENCIL_INDEX : 0;
        case GL_STENCIL_INDEX8_EXT:
            return GL_STENCIL_INDEX;
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_COMPONENT32:
            return _mesa_is_desktop_gl(ctx) ? GL_DEPTH_COMPONENT : 0;
        case GL_DEPTH_COMPONENT16:
        case GL_DEPTH_COMPONENT24:
            return GL_DEPTH_COMPONENT;
        case GL_DEPTH_STENCIL:
            return _mesa_is_desktop_gl(ctx) ? GL_DEPTH_STENCIL : 0;
        case GL_DEPTH24_STENCIL8:
            return GL_DEPTH_STENCIL;
        case GL_DEPTH_COMPONENT32F:
            return ctx->Version >= 30
                   || (ctx->API == API_OPENGL_COMPAT &&
                       ctx->Extensions.ARB_depth_buffer_float)
                   ? GL_DEPTH_COMPONENT : 0;
        case GL_DEPTH32F_STENCIL8:
            return ctx->Version >= 30
                   || (ctx->API == API_OPENGL_COMPAT &&
                       ctx->Extensions.ARB_depth_buffer_float)
                   ? GL_DEPTH_STENCIL : 0;
        case GL_RED:
            return _mesa_has_ARB_texture_rg(ctx) ? GL_RED : 0;
        case GL_R16:
            return _mesa_has_ARB_texture_rg(ctx) || _mesa_has_EXT_texture_norm16(ctx)
                   ? GL_RED : 0;
        case GL_R8:
            return ctx->API != API_OPENGLES && ctx->Extensions.ARB_texture_rg
                   ? GL_RED : 0;
        case GL_RG:
            return _mesa_has_ARB_texture_rg(ctx) ? GL_RG : 0;
        case GL_RG16:
            return _mesa_has_ARB_texture_rg(ctx) || _mesa_has_EXT_texture_norm16(ctx)
                   ? GL_RG : 0;
        case GL_RG8:
            return ctx->API != API_OPENGLES && ctx->Extensions.ARB_texture_rg
                   ? GL_RG : 0;
            /* signed normalized texture formats */
        case GL_R8_SNORM:
            return _mesa_has_EXT_texture_snorm(ctx) || _mesa_has_EXT_render_snorm(ctx)
                   ? GL_RED : 0;
        case GL_RED_SNORM:
            return _mesa_has_EXT_texture_snorm(ctx) ? GL_RED : 0;
        case GL_R16_SNORM:
            return _mesa_has_EXT_texture_snorm(ctx) ||
                   (_mesa_has_EXT_render_snorm(ctx) &&
                    _mesa_has_EXT_texture_norm16(ctx))
                   ? GL_RED : 0;
        case GL_RG8_SNORM:
            return _mesa_has_EXT_texture_snorm(ctx) || _mesa_has_EXT_render_snorm(ctx)
                   ? GL_RG : 0;
        case GL_RG_SNORM:
            return _mesa_has_EXT_texture_snorm(ctx) ? GL_RG : 0;
        case GL_RG16_SNORM:
            return _mesa_has_EXT_texture_snorm(ctx) ||
                   (_mesa_has_EXT_render_snorm(ctx) &&
                    _mesa_has_EXT_texture_norm16(ctx))
                   ? GL_RG : 0;
        case GL_RGB_SNORM:
        case GL_RGB8_SNORM:
        case GL_RGB16_SNORM:
            return _mesa_is_desktop_gl(ctx) && ctx->Extensions.EXT_texture_snorm
                   ? GL_RGB : 0;
        case GL_RGBA8_SNORM:
            return _mesa_has_EXT_texture_snorm(ctx) || _mesa_has_EXT_render_snorm(ctx)
                   ? GL_RGBA : 0;
        case GL_RGBA_SNORM:
            return _mesa_has_EXT_texture_snorm(ctx) ? GL_RGBA : 0;
        case GL_RGBA16_SNORM:
            return _mesa_has_EXT_texture_snorm(ctx) ||
                   (_mesa_has_EXT_render_snorm(ctx) &&
                    _mesa_has_EXT_texture_norm16(ctx))
                   ? GL_RGBA : 0;
        case GL_ALPHA_SNORM:
        case GL_ALPHA8_SNORM:
        case GL_ALPHA16_SNORM:
            return ctx->API == API_OPENGL_COMPAT &&
                   ctx->Extensions.EXT_texture_snorm &&
                   ctx->Extensions.ARB_framebuffer_object ? GL_ALPHA : 0;
        case GL_LUMINANCE_SNORM:
        case GL_LUMINANCE8_SNORM:
        case GL_LUMINANCE16_SNORM:
            return _mesa_is_desktop_gl(ctx) && ctx->Extensions.EXT_texture_snorm
                   ? GL_LUMINANCE : 0;
        case GL_LUMINANCE_ALPHA_SNORM:
        case GL_LUMINANCE8_ALPHA8_SNORM:
        case GL_LUMINANCE16_ALPHA16_SNORM:
            return _mesa_is_desktop_gl(ctx) && ctx->Extensions.EXT_texture_snorm
                   ? GL_LUMINANCE_ALPHA : 0;
        case GL_INTENSITY_SNORM:
        case GL_INTENSITY8_SNORM:
        case GL_INTENSITY16_SNORM:
            return _mesa_is_desktop_gl(ctx) && ctx->Extensions.EXT_texture_snorm
                   ? GL_INTENSITY : 0;

        case GL_R16F:
            return ((_mesa_is_desktop_gl(ctx) &&
                     ctx->Extensions.ARB_texture_rg &&
                     ctx->Extensions.ARB_texture_float) ||
                    _mesa_is_gles3(ctx) /* EXT_color_buffer_float */ ||
                    (_mesa_has_EXT_color_buffer_half_float(ctx) &&
                     _mesa_has_EXT_texture_rg(ctx)))
                   ? GL_RED : 0;
        case GL_R32F:
            return ((_mesa_is_desktop_gl(ctx) &&
                     ctx->Extensions.ARB_texture_rg &&
                     ctx->Extensions.ARB_texture_float) ||
                    _mesa_is_gles3(ctx) /* EXT_color_buffer_float */ )
                   ? GL_RED : 0;
        case GL_RG16F:
            return ((_mesa_is_desktop_gl(ctx) &&
                     ctx->Extensions.ARB_texture_rg &&
                     ctx->Extensions.ARB_texture_float) ||
                    _mesa_is_gles3(ctx) /* EXT_color_buffer_float */ ||
                    (_mesa_has_EXT_color_buffer_half_float(ctx) &&
                     _mesa_has_EXT_texture_rg(ctx)))
                   ? GL_RG : 0;
        case GL_RG32F:
            return ((_mesa_is_desktop_gl(ctx) &&
                     ctx->Extensions.ARB_texture_rg &&
                     ctx->Extensions.ARB_texture_float) ||
                    _mesa_is_gles3(ctx) /* EXT_color_buffer_float */ )
                   ? GL_RG : 0;
        case GL_RGB16F:
            return (_mesa_has_ARB_texture_float(ctx) ||
                    _mesa_has_EXT_color_buffer_half_float(ctx))
                   ? GL_RGB : 0;
        case GL_RGB32F:
            return (_mesa_is_desktop_gl(ctx) && ctx->Extensions.ARB_texture_float)
                   ? GL_RGB : 0;
        case GL_RGBA16F:
            return (_mesa_has_ARB_texture_float(ctx) ||
                    _mesa_is_gles3(ctx) ||
                    _mesa_has_EXT_color_buffer_half_float(ctx))
                   ? GL_RGBA : 0;
        case GL_RGBA32F:
            return ((_mesa_is_desktop_gl(ctx) &&
                     ctx->Extensions.ARB_texture_float) ||
                    _mesa_is_gles3(ctx) /* EXT_color_buffer_float */ )
                   ? GL_RGBA : 0;
        case GL_RGB9_E5:
            return (_mesa_is_desktop_gl(ctx) && ctx->Extensions.EXT_texture_shared_exponent)
                   ? GL_RGB: 0;
        case GL_ALPHA16F_ARB:
        case GL_ALPHA32F_ARB:
            return ctx->API == API_OPENGL_COMPAT &&
                   ctx->Extensions.ARB_texture_float &&
                   ctx->Extensions.ARB_framebuffer_object ? GL_ALPHA : 0;
        case GL_LUMINANCE16F_ARB:
        case GL_LUMINANCE32F_ARB:
            return ctx->API == API_OPENGL_COMPAT &&
                   ctx->Extensions.ARB_texture_float &&
                   ctx->Extensions.ARB_framebuffer_object ? GL_LUMINANCE : 0;
        case GL_LUMINANCE_ALPHA16F_ARB:
        case GL_LUMINANCE_ALPHA32F_ARB:
            return ctx->API == API_OPENGL_COMPAT &&
                   ctx->Extensions.ARB_texture_float &&
                   ctx->Extensions.ARB_framebuffer_object ? GL_LUMINANCE_ALPHA : 0;
        case GL_INTENSITY16F_ARB:
        case GL_INTENSITY32F_ARB:
            return ctx->API == API_OPENGL_COMPAT &&
                   ctx->Extensions.ARB_texture_float &&
                   ctx->Extensions.ARB_framebuffer_object ? GL_INTENSITY : 0;
        case GL_R11F_G11F_B10F:
            return ((_mesa_is_desktop_gl(ctx) && ctx->Extensions.EXT_packed_float) ||
                    _mesa_is_gles3(ctx) /* EXT_color_buffer_float */ )
                   ? GL_RGB : 0;

        case GL_RGBA8UI_EXT:
        case GL_RGBA16UI_EXT:
        case GL_RGBA32UI_EXT:
        case GL_RGBA8I_EXT:
        case GL_RGBA16I_EXT:
        case GL_RGBA32I_EXT:
            return ctx->Version >= 30
                   || (_mesa_is_desktop_gl(ctx) &&
                       ctx->Extensions.EXT_texture_integer) ? GL_RGBA : 0;

        case GL_RGB8UI_EXT:
        case GL_RGB16UI_EXT:
        case GL_RGB32UI_EXT:
        case GL_RGB8I_EXT:
        case GL_RGB16I_EXT:
        case GL_RGB32I_EXT:
            return _mesa_is_desktop_gl(ctx) && ctx->Extensions.EXT_texture_integer
                   ? GL_RGB : 0;
        case GL_R8UI:
        case GL_R8I:
        case GL_R16UI:
        case GL_R16I:
        case GL_R32UI:
        case GL_R32I:
            return ctx->Version >= 30
                   || (_mesa_is_desktop_gl(ctx) &&
                       ctx->Extensions.ARB_texture_rg &&
                       ctx->Extensions.EXT_texture_integer) ? GL_RED : 0;

        case GL_RG8UI:
        case GL_RG8I:
        case GL_RG16UI:
        case GL_RG16I:
        case GL_RG32UI:
        case GL_RG32I:
            return ctx->Version >= 30
                   || (_mesa_is_desktop_gl(ctx) &&
                       ctx->Extensions.ARB_texture_rg &&
                       ctx->Extensions.EXT_texture_integer) ? GL_RG : 0;

        case GL_INTENSITY8I_EXT:
        case GL_INTENSITY8UI_EXT:
        case GL_INTENSITY16I_EXT:
        case GL_INTENSITY16UI_EXT:
        case GL_INTENSITY32I_EXT:
        case GL_INTENSITY32UI_EXT:
            return ctx->API == API_OPENGL_COMPAT &&
                   ctx->Extensions.EXT_texture_integer &&
                   ctx->Extensions.ARB_framebuffer_object ? GL_INTENSITY : 0;

        case GL_LUMINANCE8I_EXT:
        case GL_LUMINANCE8UI_EXT:
        case GL_LUMINANCE16I_EXT:
        case GL_LUMINANCE16UI_EXT:
        case GL_LUMINANCE32I_EXT:
        case GL_LUMINANCE32UI_EXT:
            return ctx->API == API_OPENGL_COMPAT &&
                   ctx->Extensions.EXT_texture_integer &&
                   ctx->Extensions.ARB_framebuffer_object ? GL_LUMINANCE : 0;

        case GL_LUMINANCE_ALPHA8I_EXT:
        case GL_LUMINANCE_ALPHA8UI_EXT:
        case GL_LUMINANCE_ALPHA16I_EXT:
        case GL_LUMINANCE_ALPHA16UI_EXT:
        case GL_LUMINANCE_ALPHA32I_EXT:
        case GL_LUMINANCE_ALPHA32UI_EXT:
            return ctx->API == API_OPENGL_COMPAT &&
                   ctx->Extensions.EXT_texture_integer &&
                   ctx->Extensions.ARB_framebuffer_object ? GL_LUMINANCE_ALPHA : 0;

        case GL_ALPHA8I_EXT:
        case GL_ALPHA8UI_EXT:
        case GL_ALPHA16I_EXT:
        case GL_ALPHA16UI_EXT:
        case GL_ALPHA32I_EXT:
        case GL_ALPHA32UI_EXT:
            return ctx->API == API_OPENGL_COMPAT &&
                   ctx->Extensions.EXT_texture_integer &&
                   ctx->Extensions.ARB_framebuffer_object ? GL_ALPHA : 0;

        case GL_RGB10_A2UI:
            return (_mesa_is_desktop_gl(ctx) &&
                    ctx->Extensions.ARB_texture_rgb10_a2ui)
                   || _mesa_is_gles3(ctx) ? GL_RGBA : 0;

        case GL_RGB565:
            return _mesa_is_gles(ctx) || ctx->Extensions.ARB_ES2_compatibility
                   ? GL_RGB : 0;
        default:
            return 0;
    }
}

GLboolean
_mesa_is_depth_or_stencil_format(GLenum format)
{
    switch (format) {
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_COMPONENT16:
        case GL_DEPTH_COMPONENT24:
        case GL_DEPTH_COMPONENT32:
        case GL_STENCIL_INDEX:
        case GL_STENCIL_INDEX1_EXT:
        case GL_STENCIL_INDEX4_EXT:
        case GL_STENCIL_INDEX8_EXT:
        case GL_STENCIL_INDEX16_EXT:
        case GL_DEPTH_STENCIL_EXT:
        case GL_DEPTH24_STENCIL8_EXT:
        case GL_DEPTH_COMPONENT32F:
        case GL_DEPTH32F_STENCIL8:
            return GL_TRUE;
        default:
            return GL_FALSE;
    }
}

static enum pipe_format
choose_renderbuffer_format(struct gl_context *ctx,
                           GLenum internalFormat, unsigned sample_count,
                           unsigned storage_sample_count)
{
    unsigned bindings;
    if (_mesa_is_depth_or_stencil_format(internalFormat))
        bindings = PIPE_BIND_DEPTH_STENCIL;
    else
        bindings = PIPE_BIND_RENDER_TARGET;
    return st_choose_format(st_context(ctx), internalFormat, GL_NONE, GL_NONE,
                            PIPE_TEXTURE_2D, sample_count,
                            storage_sample_count, bindings,
                            false, false);
}

uint32_t
_mesa_format_image_size(mesa_format format, int width,
                        int height, int depth)
{
    const struct mesa_format_info *info = _mesa_get_format_info(format);
    uint32_t sz;
    /* Strictly speaking, a conditional isn't needed here */
    if (info->BlockWidth > 1 || info->BlockHeight > 1 || info->BlockDepth > 1) {
        /* compressed format (2D only for now) */
        const uint32_t bw = info->BlockWidth;
        const uint32_t bh = info->BlockHeight;
        const uint32_t bd = info->BlockDepth;
        const uint32_t wblocks = (width + bw - 1) / bw;
        const uint32_t hblocks = (height + bh - 1) / bh;
        const uint32_t dblocks = (depth + bd - 1) / bd;
        sz = wblocks * hblocks * dblocks * info->BytesPerBlock;
    } else
        /* non-compressed */
        sz = width * height * depth * info->BytesPerBlock;

    return sz;
}

static GLboolean
renderbuffer_alloc_sw_storage(struct gl_context *ctx,
                              struct gl_renderbuffer *rb,
                              GLenum internalFormat,
                              GLuint width, GLuint height)
{
    enum pipe_format format;
    size_t size;

    free(rb->data);
    rb->data = NULL;

    if (internalFormat == GL_RGBA16_SNORM) {
        /* Special case for software accum buffers.  Otherwise, if the
         * call to choose_renderbuffer_format() fails (because the
         * driver doesn't support signed 16-bit/channel colors) we'd
         * just return without allocating the software accum buffer.
         */
        format = PIPE_FORMAT_R16G16B16A16_SNORM;
    }
    else {
        format = choose_renderbuffer_format(ctx, internalFormat, 0, 0);

        /* Not setting gl_renderbuffer::Format here will cause
         * FRAMEBUFFER_UNSUPPORTED and ValidateFramebuffer will not be called.
         */
        if (format == PIPE_FORMAT_NONE) {
            return GL_TRUE;
        }
    }

    rb->Format = st_pipe_format_to_mesa_format(format);

    size = _mesa_format_image_size(rb->Format, width, height, 1);
    rb->data = malloc(size);
    return rb->data != NULL;
}

static inline void
pipe_surface_reference(struct pipe_surface **dst, struct pipe_surface *src)
{
    struct pipe_surface *old_dst = *dst;

    if (pipe_reference_described(old_dst ? &old_dst->reference : NULL,
                                 src ? &src->reference : NULL,
                                 (debug_reference_descriptor)
                                         debug_describe_surface))
        old_dst->context->surface_destroy(old_dst->context, old_dst);
    *dst = src;
}

GLenum
_mesa_get_linear_internalformat(GLenum format)
{
    switch (format) {
        case GL_SRGB:
            return GL_RGB;
        case GL_SRGB_ALPHA:
            return GL_RGBA;
        case GL_SRGB8:
            return GL_RGB8;
        case GL_SRGB8_ALPHA8:
            return GL_RGBA8;
        case GL_SLUMINANCE8:
            return GL_LUMINANCE8;
        case GL_SLUMINANCE:
            return GL_LUMINANCE;
        case GL_SLUMINANCE_ALPHA:
            return GL_LUMINANCE_ALPHA;
        case GL_SLUMINANCE8_ALPHA8:
            return GL_LUMINANCE8_ALPHA8;
        default:
            return format;
    }
}

static inline boolean
util_format_has_depth(const struct util_format_description *desc)
{
    return desc->colorspace == UTIL_FORMAT_COLORSPACE_ZS &&
           desc->swizzle[0] != UTIL_FORMAT_SWIZZLE_NONE;
}

static inline boolean
util_format_has_stencil(const struct util_format_description *desc)
{
    return desc->colorspace == UTIL_FORMAT_COLORSPACE_ZS &&
           desc->swizzle[1] != PIPE_SWIZZLE_NONE;
}

static inline boolean
util_format_is_depth_or_stencil(enum pipe_format format)
{
    const struct util_format_description *desc = util_format_description(format);

    assert(desc);
    if (!desc) {
        return FALSE;
    }

    return util_format_has_depth(desc) ||
           util_format_has_stencil(desc);
}

static inline boolean
util_format_is_srgb(enum pipe_format format)
{
    const struct util_format_description *desc = util_format_description(format);
    return desc->colorspace == UTIL_FORMAT_COLORSPACE_SRGB;
}

static inline enum pipe_format
util_format_srgb(enum pipe_format format)
{
    if (util_format_is_srgb(format))
        return format;

    switch (format) {
        case PIPE_FORMAT_L8_UNORM:
            return PIPE_FORMAT_L8_SRGB;
        case PIPE_FORMAT_L8A8_UNORM:
            return PIPE_FORMAT_L8A8_SRGB;
        case PIPE_FORMAT_R8G8B8_UNORM:
            return PIPE_FORMAT_R8G8B8_SRGB;
        case PIPE_FORMAT_A8B8G8R8_UNORM:
            return PIPE_FORMAT_A8B8G8R8_SRGB;
        case PIPE_FORMAT_X8B8G8R8_UNORM:
            return PIPE_FORMAT_X8B8G8R8_SRGB;
        case PIPE_FORMAT_B8G8R8A8_UNORM:
            return PIPE_FORMAT_B8G8R8A8_SRGB;
        case PIPE_FORMAT_B8G8R8X8_UNORM:
            return PIPE_FORMAT_B8G8R8X8_SRGB;
        case PIPE_FORMAT_A8R8G8B8_UNORM:
            return PIPE_FORMAT_A8R8G8B8_SRGB;
        case PIPE_FORMAT_X8R8G8B8_UNORM:
            return PIPE_FORMAT_X8R8G8B8_SRGB;
        case PIPE_FORMAT_R8G8B8A8_UNORM:
            return PIPE_FORMAT_R8G8B8A8_SRGB;
        case PIPE_FORMAT_R8G8B8X8_UNORM:
            return PIPE_FORMAT_R8G8B8X8_SRGB;
        case PIPE_FORMAT_DXT1_RGB:
            return PIPE_FORMAT_DXT1_SRGB;
        case PIPE_FORMAT_DXT1_RGBA:
            return PIPE_FORMAT_DXT1_SRGBA;
        case PIPE_FORMAT_DXT3_RGBA:
            return PIPE_FORMAT_DXT3_SRGBA;
        case PIPE_FORMAT_DXT5_RGBA:
            return PIPE_FORMAT_DXT5_SRGBA;
        case PIPE_FORMAT_B5G6R5_UNORM:
            return PIPE_FORMAT_B5G6R5_SRGB;
        case PIPE_FORMAT_BPTC_RGBA_UNORM:
            return PIPE_FORMAT_BPTC_SRGBA;
        default:
            return PIPE_FORMAT_NONE;
    }
}

static inline enum pipe_format
util_format_linear(enum pipe_format format)
{
    switch (format) {
        case PIPE_FORMAT_L8_SRGB:
            return PIPE_FORMAT_L8_UNORM;
        case PIPE_FORMAT_L8A8_SRGB:
            return PIPE_FORMAT_L8A8_UNORM;
        case PIPE_FORMAT_R8G8B8_SRGB:
            return PIPE_FORMAT_R8G8B8_UNORM;
        case PIPE_FORMAT_A8B8G8R8_SRGB:
            return PIPE_FORMAT_A8B8G8R8_UNORM;
        case PIPE_FORMAT_X8B8G8R8_SRGB:
            return PIPE_FORMAT_X8B8G8R8_UNORM;
        case PIPE_FORMAT_B8G8R8A8_SRGB:
            return PIPE_FORMAT_B8G8R8A8_UNORM;
        case PIPE_FORMAT_B8G8R8X8_SRGB:
            return PIPE_FORMAT_B8G8R8X8_UNORM;
        case PIPE_FORMAT_A8R8G8B8_SRGB:
            return PIPE_FORMAT_A8R8G8B8_UNORM;
        case PIPE_FORMAT_X8R8G8B8_SRGB:
            return PIPE_FORMAT_X8R8G8B8_UNORM;
        case PIPE_FORMAT_R8G8B8A8_SRGB:
            return PIPE_FORMAT_R8G8B8A8_UNORM;
        case PIPE_FORMAT_R8G8B8X8_SRGB:
            return PIPE_FORMAT_R8G8B8X8_UNORM;
        case PIPE_FORMAT_DXT1_SRGB:
            return PIPE_FORMAT_DXT1_RGB;
        case PIPE_FORMAT_DXT1_SRGBA:
            return PIPE_FORMAT_DXT1_RGBA;
        case PIPE_FORMAT_DXT3_SRGBA:
            return PIPE_FORMAT_DXT3_RGBA;
        case PIPE_FORMAT_DXT5_SRGBA:
            return PIPE_FORMAT_DXT5_RGBA;
        case PIPE_FORMAT_B5G6R5_SRGB:
            return PIPE_FORMAT_B5G6R5_UNORM;
        case PIPE_FORMAT_BPTC_SRGBA:
            return PIPE_FORMAT_BPTC_RGBA_UNORM;
        default:
            return format;
    }
}

static inline unsigned
u_minify(unsigned value, unsigned levels)
{
    return MAX2(1, value >> levels);
}

static inline unsigned
util_max_layer(const struct pipe_resource *r, unsigned level)
{
    switch (r->target) {
        case PIPE_TEXTURE_3D:
            return u_minify(r->depth0, level) - 1;
        case PIPE_TEXTURE_CUBE:
            assert(r->array_size == 6);
            FALLTHROUGH;
        case PIPE_TEXTURE_1D_ARRAY:
        case PIPE_TEXTURE_2D_ARRAY:
        case PIPE_TEXTURE_CUBE_ARRAY:
            return r->array_size - 1;
        default:
            return 0;
    }
}

void
_mesa_update_renderbuffer_surface(struct gl_context *ctx,
                                  struct gl_renderbuffer *rb)
{
    struct pipe_context *pipe = ctx->pipe;
    struct pipe_resource *resource = rb->texture;
    const struct gl_texture_object *stTexObj = NULL;
    unsigned rtt_width = rb->Width;
    unsigned rtt_height = rb->Height;
    unsigned rtt_depth = rb->Depth;

    /*
     * For winsys fbo, it is possible that the renderbuffer is sRGB-capable but
     * the format of rb->texture is linear (because we have no control over
     * the format).  Check rb->Format instead of rb->texture->format
     * to determine if the rb is sRGB-capable.
     */
    boolean enable_srgb = ctx->Color.sRGBEnabled &&
                          _mesa_is_format_srgb(rb->Format);
    enum pipe_format format = resource->format;

    if (rb->is_rtt) {
        stTexObj = rb->TexImage->TexObject;
        if (stTexObj->surface_based)
            format = stTexObj->surface_format;
    }

    format = enable_srgb ? util_format_srgb(format) : util_format_linear(format);

    if (resource->target == PIPE_TEXTURE_1D_ARRAY) {
        rtt_depth = rtt_height;
        rtt_height = 1;
    }

    /* find matching mipmap level size */
    unsigned level;
    for (level = 0; level <= resource->last_level; level++) {
        if (u_minify(resource->width0, level) == rtt_width &&
            u_minify(resource->height0, level) == rtt_height &&
            (resource->target != PIPE_TEXTURE_3D ||
             u_minify(resource->depth0, level) == rtt_depth)) {
            break;
        }
    }
    assert(level <= resource->last_level);

    /* determine the layer bounds */
    unsigned first_layer, last_layer;
    if (rb->rtt_layered) {
        first_layer = 0;
        last_layer = util_max_layer(rb->texture, level);
    }
    else {
        first_layer =
        last_layer = rb->rtt_face + rb->rtt_slice;
    }

    /* Adjust for texture views */
    if (rb->is_rtt && resource->array_size > 1 &&
        stTexObj->Immutable) {
        const struct gl_texture_object *tex = stTexObj;
        first_layer += tex->Attrib.MinLayer;
        if (!rb->rtt_layered)
            last_layer += tex->Attrib.MinLayer;
        else
            last_layer = MIN2(first_layer + tex->Attrib.NumLayers - 1,
                              last_layer);
    }

    struct pipe_surface **psurf =
            enable_srgb ? &rb->surface_srgb : &rb->surface_linear;
    struct pipe_surface *surf = *psurf;

    if (!surf ||
        surf->texture->nr_samples != rb->NumSamples ||
        surf->texture->nr_storage_samples != rb->NumStorageSamples ||
        surf->format != format ||
        surf->texture != resource ||
        surf->width != rtt_width ||
        surf->height != rtt_height ||
        surf->nr_samples != rb->rtt_nr_samples ||
        surf->u.tex.level != level ||
        surf->u.tex.first_layer != first_layer ||
        surf->u.tex.last_layer != last_layer) {
        /* create a new pipe_surface */
        struct pipe_surface surf_tmpl;
        memset(&surf_tmpl, 0, sizeof(surf_tmpl));
        surf_tmpl.format = format;
        surf_tmpl.nr_samples = rb->rtt_nr_samples;
        surf_tmpl.u.tex.level = level;
        surf_tmpl.u.tex.first_layer = first_layer;
        surf_tmpl.u.tex.last_layer = last_layer;

        /* create -> destroy to avoid blowing up cached surfaces */
        struct pipe_surface *surf = pipe->create_surface(pipe, resource, &surf_tmpl);
        pipe_surface_release(pipe, psurf);
        *psurf = surf;
    }
    rb->surface = *psurf;
}

static GLboolean
renderbuffer_alloc_storage(struct gl_context * ctx,
                           struct gl_renderbuffer *rb,
                           GLenum internalFormat,
                           GLuint width, GLuint height)
{
    struct st_context *st = st_context(ctx);
    struct pipe_screen *screen = ctx->screen;
    enum pipe_format format = PIPE_FORMAT_NONE;
    struct pipe_resource templ;

    /* init renderbuffer fields */
    rb->Width  = width;
    rb->Height = height;
    rb->_BaseFormat = _mesa_base_fbo_format(ctx, internalFormat);
    rb->defined = GL_FALSE;  /* undefined contents now */

    if (rb->software) {
        return renderbuffer_alloc_sw_storage(ctx, rb, internalFormat,
                                             width, height);
    }

    /* Free the old surface and texture
     */
    pipe_surface_reference(&rb->surface_srgb, NULL);
    pipe_surface_reference(&rb->surface_linear, NULL);
    rb->surface = NULL;
    pipe_resource_reference(&rb->texture, NULL);

    /* If an sRGB framebuffer is unsupported, sRGB formats behave like linear
     * formats.
     */
    if (!ctx->Extensions.EXT_sRGB) {
        internalFormat = _mesa_get_linear_internalformat(internalFormat);
    }

    /* Handle multisample renderbuffers first.
     *
     * From ARB_framebuffer_object:
     *   If <samples> is zero, then RENDERBUFFER_SAMPLES is set to zero.
     *   Otherwise <samples> represents a request for a desired minimum
     *   number of samples. Since different implementations may support
     *   different sample counts for multisampled rendering, the actual
     *   number of samples allocated for the renderbuffer image is
     *   implementation dependent.  However, the resulting value for
     *   RENDERBUFFER_SAMPLES is guaranteed to be greater than or equal
     *   to <samples> and no more than the next larger sample count supported
     *   by the implementation.
     *
     * Find the supported number of samples >= rb->NumSamples
     */
    if (rb->NumSamples > 0) {
        unsigned start, start_storage;

        if (ctx->Const.MaxSamples > 1 &&  rb->NumSamples == 1) {
            /* don't try num_samples = 1 with drivers that support real msaa */
            start = 2;
            start_storage = 2;
        } else {
            start = rb->NumSamples;
            start_storage = rb->NumStorageSamples;
        }

        if (ctx->Extensions.AMD_framebuffer_multisample_advanced) {
            if (rb->_BaseFormat == GL_DEPTH_COMPONENT ||
                rb->_BaseFormat == GL_DEPTH_STENCIL ||
                rb->_BaseFormat == GL_STENCIL_INDEX) {
                /* Find a supported depth-stencil format. */
                for (unsigned samples = start;
                     samples <= ctx->Const.MaxDepthStencilFramebufferSamples;
                     samples++) {
                    format = choose_renderbuffer_format(ctx, internalFormat,
                                                        samples, samples);

                    if (format != PIPE_FORMAT_NONE) {
                        rb->NumSamples = samples;
                        rb->NumStorageSamples = samples;
                        break;
                    }
                }
            } else {
                /* Find a supported color format, samples >= storage_samples. */
                for (unsigned storage_samples = start_storage;
                     storage_samples <= ctx->Const.MaxColorFramebufferStorageSamples;
                     storage_samples++) {
                    for (unsigned samples = MAX2(start, storage_samples);
                         samples <= ctx->Const.MaxColorFramebufferSamples;
                         samples++) {
                        format = choose_renderbuffer_format(ctx, internalFormat,
                                                            samples,
                                                            storage_samples);

                        if (format != PIPE_FORMAT_NONE) {
                            rb->NumSamples = samples;
                            rb->NumStorageSamples = storage_samples;
                            goto found;
                        }
                    }
                }
                found:;
            }
        } else {
            for (unsigned samples = start; samples <= ctx->Const.MaxSamples;
                 samples++) {
                format = choose_renderbuffer_format(ctx, internalFormat,
                                                    samples, samples);

                if (format != PIPE_FORMAT_NONE) {
                    rb->NumSamples = samples;
                    rb->NumStorageSamples = samples;
                    break;
                }
            }
        }
    } else {
        format = choose_renderbuffer_format(ctx, internalFormat, 0, 0);
    }

    /* Not setting gl_renderbuffer::Format here will cause
     * FRAMEBUFFER_UNSUPPORTED and ValidateFramebuffer will not be called.
     */
    if (format == PIPE_FORMAT_NONE) {
        return GL_TRUE;
    }

    rb->Format = st_pipe_format_to_mesa_format(format);

    if (width == 0 || height == 0) {
        /* if size is zero, nothing to allocate */
        return GL_TRUE;
    }

    /* Setup new texture template.
     */
    memset(&templ, 0, sizeof(templ));
    templ.target = st->internal_target;
    templ.format = format;
    templ.width0 = width;
    templ.height0 = height;
    templ.depth0 = 1;
    templ.array_size = 1;
    templ.nr_samples = rb->NumSamples;
    templ.nr_storage_samples = rb->NumStorageSamples;

    if (util_format_is_depth_or_stencil(format)) {
        templ.bind = PIPE_BIND_DEPTH_STENCIL;
    }
    else if (rb->Name != 0) {
        /* this is a user-created renderbuffer */
        templ.bind = PIPE_BIND_RENDER_TARGET;
    }
    else {
        /* this is a window-system buffer */
        templ.bind = (PIPE_BIND_DISPLAY_TARGET |
                      PIPE_BIND_RENDER_TARGET);
    }

    rb->texture = screen->resource_create(screen, &templ);

    if (!rb->texture)
        return FALSE;

    _mesa_update_renderbuffer_surface(ctx, rb);
    return rb->surface != NULL;
}

void
_mesa_init_renderbuffer(struct gl_renderbuffer *rb, GLuint name)
{
    GET_CURRENT_CONTEXT(ctx);

    rb->ClassID = 0;
    rb->Name = name;
    rb->RefCount = 1;
    rb->Delete = delete_renderbuffer;

    /* The rest of these should be set later by the caller of this function or
     * the AllocStorage method:
     */
    rb->AllocStorage = NULL;

    rb->Width = 0;
    rb->Height = 0;
    rb->Depth = 0;

    /* In GL 3, the initial format is GL_RGBA according to Table 6.26
     * on page 302 of the GL 3.3 spec.
     *
     * In GLES 3, the initial format is GL_RGBA4 according to Table 6.15
     * on page 258 of the GLES 3.0.4 spec.
     *
     * If the context is current, set the initial format based on the
     * specs. If the context is not current, we cannot determine the
     * API, so default to GL_RGBA.
     */
    if (ctx && _mesa_is_gles(ctx)) {
        rb->InternalFormat = GL_RGBA4;
    } else {
        rb->InternalFormat = GL_RGBA;
    }

    rb->Format = MESA_FORMAT_NONE;

    rb->AllocStorage = renderbuffer_alloc_storage;
}

static inline bool
_mesa_format_is_mesa_array_format(uint32_t f)
{
    return (f & MESA_ARRAY_FORMAT_BIT) != 0;
}

static inline enum mesa_array_format_base_format
_mesa_array_format_get_base_format(mesa_array_format f)
{
    return (enum mesa_array_format_base_format)
            ((f & MESA_ARRAY_FORMAT_BASE_FORMAT_MASK) >> 20);
}

static inline void
_mesa_array_format_get_swizzle(mesa_array_format f, uint8_t *swizzle)
{
    swizzle[0] = (f & MESA_ARRAY_FORMAT_SWIZZLE_X_MASK) >> 8;
    swizzle[1] = (f & MESA_ARRAY_FORMAT_SWIZZLE_Y_MASK) >> 11;
    swizzle[2] = (f & MESA_ARRAY_FORMAT_SWIZZLE_Z_MASK) >> 14;
    swizzle[3] = (f & MESA_ARRAY_FORMAT_SWIZZLE_W_MASK) >> 17;
}

static inline int
_mesa_array_format_get_num_channels(mesa_array_format f)
{
    return (f & MESA_ARRAY_FORMAT_NUM_CHANS_MASK) >> 5;
}

static GLenum
get_base_format_for_array_format(mesa_array_format format)
{
    uint8_t swizzle[4];
    int num_channels;

    switch (_mesa_array_format_get_base_format(format)) {
        case MESA_ARRAY_FORMAT_BASE_FORMAT_DEPTH:
            return GL_DEPTH_COMPONENT;
        case MESA_ARRAY_FORMAT_BASE_FORMAT_STENCIL:
            return GL_STENCIL_INDEX;
        case MESA_ARRAY_FORMAT_BASE_FORMAT_RGBA_VARIANTS:
            break;
    }

    _mesa_array_format_get_swizzle(format, swizzle);
    num_channels = _mesa_array_format_get_num_channels(format);

    switch (num_channels) {
        case 4:
            /* FIXME: RGBX formats have 4 channels, but their base format is GL_RGB.
             * This is not really a problem for now because we only create array
             * formats from GL format/type combinations, and these cannot specify
             * RGBX formats.
             */
            return GL_RGBA;
        case 3:
            return GL_RGB;
        case 2:
            if (swizzle[0] == 0 &&
                swizzle[1] == 0 &&
                swizzle[2] == 0 &&
                swizzle[3] == 1)
                return GL_LUMINANCE_ALPHA;
            if (swizzle[0] == 1 &&
                swizzle[1] == 1 &&
                swizzle[2] == 1 &&
                swizzle[3] == 0)
                return GL_LUMINANCE_ALPHA;
            if (swizzle[0] == 0 &&
                swizzle[1] == 1 &&
                swizzle[2] == 4 &&
                swizzle[3] == 5)
                return GL_RG;
            if (swizzle[0] == 1 &&
                swizzle[1] == 0 &&
                swizzle[2] == 4 &&
                swizzle[3] == 5)
                return GL_RG;
            break;
        case 1:
            if (swizzle[0] == 0 &&
                swizzle[1] == 0 &&
                swizzle[2] == 0 &&
                swizzle[3] == 5)
                return GL_LUMINANCE;
            if (swizzle[0] == 0 &&
                swizzle[1] == 0 &&
                swizzle[2] == 0 &&
                swizzle[3] == 0)
                return GL_INTENSITY;
            if (swizzle[0] <= MESA_FORMAT_SWIZZLE_W)
                return GL_RED;
            if (swizzle[1] <= MESA_FORMAT_SWIZZLE_W)
                return GL_GREEN;
            if (swizzle[2] <= MESA_FORMAT_SWIZZLE_W)
                return GL_BLUE;
            if (swizzle[3] <= MESA_FORMAT_SWIZZLE_W)
                return GL_ALPHA;
            break;
    }

    unreachable("Unsupported format");
}

GLenum
_mesa_get_format_base_format(uint32_t format)
{
    if (!_mesa_format_is_mesa_array_format(format)) {
        const struct mesa_format_info *info = _mesa_get_format_info(format);
        return info->BaseFormat;
    } else {
        return get_base_format_for_array_format(format);
    }
}

static inline GLboolean
_mesa_is_user_fbo(const struct gl_framebuffer *fb)
{
    return fb->Name != 0;
}

static void
validate_and_init_renderbuffer_attachment(struct gl_framebuffer *fb,
                                          gl_buffer_index bufferName,
                                          struct gl_renderbuffer *rb)
{
    assert(fb);
    assert(rb);
    assert(bufferName < BUFFER_COUNT);

    /* There should be no previous renderbuffer on this attachment point,
     * with the exception of depth/stencil since the same renderbuffer may
     * be used for both.
     */
    assert(bufferName == BUFFER_DEPTH ||
           bufferName == BUFFER_STENCIL ||
           fb->Attachment[bufferName].Renderbuffer == NULL);

    /* winsys vs. user-created buffer cross check */
    if (_mesa_is_user_fbo(fb)) {
        assert(rb->Name);
    }
    else {
        assert(!rb->Name);
    }

    fb->Attachment[bufferName].Type = GL_RENDERBUFFER_EXT;
    fb->Attachment[bufferName].Complete = GL_TRUE;
}

void
_mesa_attach_and_own_rb(struct gl_framebuffer *fb,
                        gl_buffer_index bufferName,
                        struct gl_renderbuffer *rb)
{
    assert(rb->RefCount == 1);

    validate_and_init_renderbuffer_attachment(fb, bufferName, rb);

    _mesa_reference_renderbuffer(&fb->Attachment[bufferName].Renderbuffer,
                                 NULL);
    fb->Attachment[bufferName].Renderbuffer = rb;
}

void
_mesa_attach_and_reference_rb(struct gl_framebuffer *fb,
                              gl_buffer_index bufferName,
                              struct gl_renderbuffer *rb)
{
    validate_and_init_renderbuffer_attachment(fb, bufferName, rb);
    _mesa_reference_renderbuffer(&fb->Attachment[bufferName].Renderbuffer, rb);
}

#define REMAINDER_MAGIC(divisor) \
   ((uint64_t) ~0ull / (divisor) + 1)

static const struct {
    uint32_t max_entries, size, rehash;
    uint64_t size_magic, rehash_magic;
} hash_sizes[] = {
#define ENTRY(max_entries, size, rehash) \
   { max_entries, size, rehash, \
      REMAINDER_MAGIC(size), REMAINDER_MAGIC(rehash) }

        ENTRY(2,            5,            3            ),
        ENTRY(4,            7,            5            ),
        ENTRY(8,            13,           11           ),
        ENTRY(16,           19,           17           ),
        ENTRY(32,           43,           41           ),
        ENTRY(64,           73,           71           ),
        ENTRY(128,          151,          149          ),
        ENTRY(256,          283,          281          ),
        ENTRY(512,          571,          569          ),
        ENTRY(1024,         1153,         1151         ),
        ENTRY(2048,         2269,         2267         ),
        ENTRY(4096,         4519,         4517         ),
        ENTRY(8192,         9013,         9011         ),
        ENTRY(16384,        18043,        18041        ),
        ENTRY(32768,        36109,        36107        ),
        ENTRY(65536,        72091,        72089        ),
        ENTRY(131072,       144409,       144407       ),
        ENTRY(262144,       288361,       288359       ),
        ENTRY(524288,       576883,       576881       ),
        ENTRY(1048576,      1153459,      1153457      ),
        ENTRY(2097152,      2307163,      2307161      ),
        ENTRY(4194304,      4613893,      4613891      ),
        ENTRY(8388608,      9227641,      9227639      ),
        ENTRY(16777216,     18455029,     18455027     ),
        ENTRY(33554432,     36911011,     36911009     ),
        ENTRY(67108864,     73819861,     73819859     ),
        ENTRY(134217728,    147639589,    147639587    ),
        ENTRY(268435456,    295279081,    295279079    ),
        ENTRY(536870912,    590559793,    590559791    ),
        ENTRY(1073741824,   1181116273,   1181116271   ),
        ENTRY(2147483648ul, 2362232233ul, 2362232231ul )
};

static void
hash_table_clear_fast(struct hash_table *ht)
{
    memset(ht->table, 0, sizeof(struct hash_entry) * hash_sizes[ht->size_index].size);
    ht->entries = ht->deleted_entries = 0;
}

void *
rzalloc_size(const void *ctx, size_t size)
{
    void *ptr = ralloc_size(ctx, size);

    if (likely(ptr))
        memset(ptr, 0, size);

    return ptr;
}

void *
rzalloc_array_size(const void *ctx, size_t size, unsigned count)
{
    if (count > SIZE_MAX/size)
        return NULL;

    return rzalloc_size(ctx, size * count);
}

#define rzalloc_array(ctx, type, count) \
   ((type *) rzalloc_array_size(ctx, sizeof(type), count))

void *
ralloc_parent(const void *ptr)
{
    ralloc_header *info;

    if (unlikely(ptr == NULL))
        return NULL;

    info = get_header(ptr);
    return info->parent ? PTR_FROM_HEADER(info->parent) : NULL;
}

#define hash_table_foreach(ht, entry)                                      \
   for (struct hash_entry *entry = _mesa_hash_table_next_entry(ht, NULL);  \
        entry != NULL;                                                     \
        entry = _mesa_hash_table_next_entry(ht, entry))

struct hash_entry *
_mesa_hash_table_next_entry(struct hash_table *ht,
                            struct hash_entry *entry)
{
    if (entry == NULL)
        entry = ht->table;
    else
        entry = entry + 1;

    for (; entry != ht->table + ht->size; entry++) {
        if (entry_is_present(ht, entry)) {
            return entry;
        }
    }

    return NULL;
}

static void
hash_table_insert_rehash(struct hash_table *ht, uint32_t hash,
                         const void *key, void *data)
{
    uint32_t size = ht->size;
    uint32_t start_hash_address = util_fast_urem32(hash, size, ht->size_magic);
    uint32_t double_hash = 1 + util_fast_urem32(hash, ht->rehash,
                                                ht->rehash_magic);
    uint32_t hash_address = start_hash_address;
    do {
        struct hash_entry *entry = ht->table + hash_address;

        if (likely(entry->key == NULL)) {
            entry->hash = hash;
            entry->key = key;
            entry->data = data;
            return;
        }

        hash_address += double_hash;
        if (hash_address >= size)
            hash_address -= size;
    } while (true);
}

static void
_mesa_hash_table_rehash(struct hash_table *ht, unsigned new_size_index)
{
    struct hash_table old_ht;
    struct hash_entry *table;

    if (ht->size_index == new_size_index && ht->deleted_entries == ht->max_entries) {
        hash_table_clear_fast(ht);
        assert(!ht->entries);
        return;
    }

    if (new_size_index >= ARRAY_SIZE(hash_sizes))
        return;

    table = rzalloc_array(ralloc_parent(ht->table), struct hash_entry,
    hash_sizes[new_size_index].size);
    if (table == NULL)
        return;

    old_ht = *ht;

    ht->table = table;
    ht->size_index = new_size_index;
    ht->size = hash_sizes[ht->size_index].size;
    ht->rehash = hash_sizes[ht->size_index].rehash;
    ht->size_magic = hash_sizes[ht->size_index].size_magic;
    ht->rehash_magic = hash_sizes[ht->size_index].rehash_magic;
    ht->max_entries = hash_sizes[ht->size_index].max_entries;
    ht->entries = 0;
    ht->deleted_entries = 0;

    hash_table_foreach(&old_ht, entry) {
        hash_table_insert_rehash(ht, entry->hash, entry->key, entry->data);
    }

    ht->entries = old_ht.entries;

    ralloc_free(old_ht.table);
}

static int
entry_is_deleted(const struct hash_table *ht, struct hash_entry *entry)
{
    return entry->key == ht->deleted_key;
}

static struct hash_entry *
hash_table_insert(struct hash_table *ht, uint32_t hash,
                  const void *key, void *data)
{
    struct hash_entry *available_entry = NULL;

    assert(!key_pointer_is_reserved(ht, key));

    if (ht->entries >= ht->max_entries) {
        _mesa_hash_table_rehash(ht, ht->size_index + 1);
    } else if (ht->deleted_entries + ht->entries >= ht->max_entries) {
        _mesa_hash_table_rehash(ht, ht->size_index);
    }

    uint32_t size = ht->size;
    uint32_t start_hash_address = util_fast_urem32(hash, size, ht->size_magic);
    uint32_t double_hash = 1 + util_fast_urem32(hash, ht->rehash,
                                                ht->rehash_magic);
    uint32_t hash_address = start_hash_address;
    do {
        struct hash_entry *entry = ht->table + hash_address;

        if (!entry_is_present(ht, entry)) {
            /* Stash the first available entry we find */
            if (available_entry == NULL)
                available_entry = entry;
            if (entry_is_free(entry))
                break;
        }

        /* Implement replacement when another insert happens
         * with a matching key.  This is a relatively common
         * feature of hash tables, with the alternative
         * generally being "insert the new value as well, and
         * return it first when the key is searched for".
         *
         * Note that the hash table doesn't have a delete
         * callback.  If freeing of old data pointers is
         * required to avoid memory leaks, perform a search
         * before inserting.
         */
        if (!entry_is_deleted(ht, entry) &&
            entry->hash == hash &&
            ht->key_equals_function(key, entry->key)) {
            entry->key = key;
            entry->data = data;
            return entry;
        }

        hash_address += double_hash;
        if (hash_address >= size)
            hash_address -= size;
    } while (hash_address != start_hash_address);

    if (available_entry) {
        if (entry_is_deleted(ht, available_entry))
            ht->deleted_entries--;
        available_entry->hash = hash;
        available_entry->key = key;
        available_entry->data = data;
        ht->entries++;
        return available_entry;
    }

    /* We could hit here if a required resize failed. An unchecked-malloc
     * application could ignore this result.
     */
    return NULL;
}

struct hash_entry *
_mesa_hash_table_insert(struct hash_table *ht, const void *key, void *data)
{
    assert(ht->key_hash_function);
    return hash_table_insert(ht, ht->key_hash_function(key), key, data);
}

void
_mesa_intersect_scissor_bounding_box(const struct gl_context *ctx,
                                     unsigned idx, int *bbox)
{
    if (ctx->Scissor.EnableFlags & (1u << idx)) {
        if (ctx->Scissor.ScissorArray[idx].X > bbox[0]) {
            bbox[0] = ctx->Scissor.ScissorArray[idx].X;
        }
        if (ctx->Scissor.ScissorArray[idx].Y > bbox[2]) {
            bbox[2] = ctx->Scissor.ScissorArray[idx].Y;
        }
        if (ctx->Scissor.ScissorArray[idx].X + ctx->Scissor.ScissorArray[idx].Width < bbox[1]) {
            bbox[1] = ctx->Scissor.ScissorArray[idx].X + ctx->Scissor.ScissorArray[idx].Width;
        }
        if (ctx->Scissor.ScissorArray[idx].Y + ctx->Scissor.ScissorArray[idx].Height < bbox[3]) {
            bbox[3] = ctx->Scissor.ScissorArray[idx].Y + ctx->Scissor.ScissorArray[idx].Height;
        }
        /* finally, check for empty region */
        if (bbox[0] > bbox[1]) {
            bbox[0] = bbox[1];
        }
        if (bbox[2] > bbox[3]) {
            bbox[2] = bbox[3];
        }
    }
}

static void
scissor_bounding_box(const struct gl_context *ctx,
                     const struct gl_framebuffer *buffer,
                     unsigned idx, int *bbox)
{
    bbox[0] = 0;
    bbox[2] = 0;
    bbox[1] = buffer->Width;
    bbox[3] = buffer->Height;

    _mesa_intersect_scissor_bounding_box(ctx, idx, bbox);

    assert(bbox[0] <= bbox[1]);
    assert(bbox[2] <= bbox[3]);
}

void
_mesa_update_draw_buffer_bounds(struct gl_context *ctx,
                                struct gl_framebuffer *buffer)
{
    int bbox[4];

    if (!buffer)
        return;

    /* Default to the first scissor as that's always valid */
    scissor_bounding_box(ctx, buffer, 0, bbox);
    buffer->_Xmin = bbox[0];
    buffer->_Ymin = bbox[2];
    buffer->_Xmax = bbox[1];
    buffer->_Ymax = bbox[3];
}

void
_mesa_resize_framebuffer(struct gl_context *ctx, struct gl_framebuffer *fb,
                         GLuint width, GLuint height)
{
    /* XXX I think we could check if the size is not changing
     * and return early.
     */

    /* Can only resize win-sys framebuffer objects */
    assert(_mesa_is_winsys_fbo(fb));

    for (unsigned i = 0; i < BUFFER_COUNT; i++) {
        struct gl_renderbuffer_attachment *att = &fb->Attachment[i];
        if (att->Type == GL_RENDERBUFFER_EXT && att->Renderbuffer) {
            struct gl_renderbuffer *rb = att->Renderbuffer;
            /* only resize if size is changing */
            if (rb->Width != width || rb->Height != height) {
                if (rb->AllocStorage(ctx, rb, rb->InternalFormat, width, height)) {
                    assert(rb->Width == width);
                    assert(rb->Height == height);
                }
                else {
                    _mesa_error(ctx, GL_OUT_OF_MEMORY, "Resizing framebuffer");
                    /* no return */
                }
            }
        }
    }

    fb->Width = width;
    fb->Height = height;

    if (ctx) {
        /* update scissor / window bounds */
        _mesa_update_draw_buffer_bounds(ctx, ctx->DrawBuffer);
        /* Signal new buffer state so that swrast will update its clipping
         * info (the CLIP_BIT flag).
         */
        ctx->NewState |= _NEW_BUFFERS;
    }
}