#ifndef DRI_DRAWABLE_H
#define DRI_DRAWABLE_H

#include "../../../include/pipe/p_compiler.h"
#include "../../../include/pipe/p_format.h"
#include "../../frontend/api.h"
#include "dri_util.h"

struct pipe_surface;
struct st_framebuffer;
struct dri_context;

struct dri_drawable
{
    struct st_framebuffer_iface base;
    struct st_visual stvis;

    struct dri_screen *screen;

    /* dri */
    __DRIdrawable *dPriv;
    __DRIscreen *sPriv;

    __DRIbuffer old[__DRI_BUFFER_COUNT];
    unsigned old_num;
    unsigned old_w;
    unsigned old_h;

    struct pipe_box *damage_rects;
    unsigned int num_damage_rects;

    struct pipe_resource *textures[ST_ATTACHMENT_COUNT];
    struct pipe_resource *msaa_textures[ST_ATTACHMENT_COUNT];
    unsigned int texture_mask, texture_stamp;

    struct pipe_fence_handle *throttle_fence;
    bool flushing; /* prevents recursion in dri_flush */

    /* hooks filled in by dri2 & drisw */
    void (*allocate_textures)(struct dri_context *ctx,
                              struct dri_drawable *drawable,
                              const enum st_attachment_type *statts,
                              unsigned count);

    void (*update_drawable_info)(struct dri_drawable *drawable);

    bool (*flush_frontbuffer)(struct dri_context *ctx,
                              struct dri_drawable *drawable,
                              enum st_attachment_type statt);

    void (*update_tex_buffer)(struct dri_drawable *drawable,
                              struct dri_context *ctx,
                              struct pipe_resource *res);
    void (*flush_swapbuffers)(struct dri_context *ctx,
                              struct dri_drawable *drawable);
};

static inline struct dri_drawable *
dri_drawable(__DRIdrawable * driDrawPriv)
{
    return (struct dri_drawable *) (driDrawPriv)
           ? driDrawPriv->driverPrivate : NULL;
}

/***********************************************************************
 * dri_drawable.c
 */
bool
dri_create_buffer(__DRIscreen * sPriv,
                  __DRIdrawable * dPriv,
                  const struct gl_config * visual, bool isPixmap);

void dri_destroy_buffer(__DRIdrawable * dPriv);

void
dri_drawable_get_format(struct dri_drawable *drawable,
                        enum st_attachment_type statt,
                        enum pipe_format *format,
                        unsigned *bind);

void
dri_pipe_blit(struct pipe_context *pipe,
              struct pipe_resource *dst,
              struct pipe_resource *src);

void
dri_flush(__DRIcontext *cPriv,
          __DRIdrawable *dPriv,
          unsigned flags,
          enum __DRI2throttleReason reason);

extern const __DRItexBufferExtension driTexBufferExtension;
extern const __DRI2throttleExtension dri2ThrottleExtension;
#endif

/* vim: set sw=3 ts=8 sts=3 expandtab: */
