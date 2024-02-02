#ifndef DRI_CONTEXT_H
#define DRI_CONTEXT_H

#include "dri_util.h"
#include "../../../include/pipe/p_compiler.h"
#include "../../hud/hud_context.h"

struct pipe_context;
struct pipe_fence;
struct st_api;
struct st_context_iface;
struct dri_drawable;

struct dri_context
{
    /* dri */
    __DRIscreen *sPriv;
    __DRIcontext *cPriv;
    __DRIdrawable *dPriv;
    __DRIdrawable *rPriv;

    unsigned int bind_count;

    /**
     * True if the __DRIdrawable's current __DRIimageBufferMask is
     * __DRI_IMAGE_BUFFER_SHARED.
     */
    bool is_shared_buffer_bound;

    /* gallium */
    struct st_api *stapi;
    struct st_context_iface *st;
    struct pp_queue_t *pp;
    struct hud_context *hud;
};

static inline struct dri_context *
dri_context(__DRIcontext * driContextPriv)
{
    if (!driContextPriv)
        return NULL;
    return (struct dri_context *)driContextPriv->driverPrivate;
}

/***********************************************************************
 * dri_context.c
 */
void dri_destroy_context(__DRIcontext * driContextPriv);

boolean dri_unbind_context(__DRIcontext * driContextPriv);

boolean
dri_make_current(__DRIcontext * driContextPriv,
                 __DRIdrawable * driDrawPriv,
                 __DRIdrawable * driReadPriv);

struct dri_context *
dri_get_current(__DRIscreen * driScreenPriv);

boolean
dri_create_context(gl_api api,
                   const struct gl_config * visual,
                   __DRIcontext * driContextPriv,
                   const struct __DriverContextConfig *ctx_config,
                   unsigned *error,
                   void *sharedContextPrivate);

#endif

/* vim: set sw=3 ts=8 sts=3 expandtab: */
