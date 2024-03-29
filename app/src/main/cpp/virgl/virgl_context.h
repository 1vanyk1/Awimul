#ifndef VIRGL_CONTEXT_H
#define VIRGL_CONTEXT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "virglrenderer_hw.h"
#include "virgl_resource.h"

struct vrend_transfer_info;
struct pipe_resource;

struct virgl_context_blob {
    /* valid fd or pipe resource */
    enum virgl_resource_fd_type type;
    union {
        int fd;
        struct pipe_resource *pipe_resource;
    } u;

    uint32_t map_info;

    void *renderer_data;
};

struct virgl_context;

typedef void (*virgl_context_fence_retire)(struct virgl_context *ctx,
                                           uint64_t queue_id,
                                           void *fence_cookie);

/**
 * Base class for renderer contexts.  For example, vrend_decode_ctx is a
 * subclass of virgl_context.
 */
struct virgl_context {
    uint32_t ctx_id;

    enum virgl_renderer_capset capset_id;

    /*
     * Each fence goes through submitted, signaled, and retired.  This callback
     * is called from virgl_context::retire_fences to retire signaled fences of
     * each queue.  When a queue has multiple signaled fences by the time
     * virgl_context::retire_fences is called, this callback might not be called
     * on all fences but only on the latest one, depending on the flags of the
     * fences.
     */
    virgl_context_fence_retire fence_retire;

    void (*destroy)(struct virgl_context *ctx);

    void (*attach_resource)(struct virgl_context *ctx,
                            struct virgl_resource *res);
    void (*detach_resource)(struct virgl_context *ctx,
                            struct virgl_resource *res);

    int (*transfer_3d)(struct virgl_context *ctx,
                       struct virgl_resource *res,
                       const struct vrend_transfer_info *info,
                       int transfer_mode);

    /* These are used to create a virgl_resource from a context object.
     *
     * get_blob returns a virgl_context_blob from which a virgl_resource can be
     * created.  get_blob_done is optional and allows the context to associate
     * the newly created resource with the context object.
     *
     * Note that get_blob is a one-time thing.  The context object might be
     * destroyed or reject subsequent get_blob calls.
     */
    int (*get_blob)(struct virgl_context *ctx,
                    uint64_t blob_id,
                    uint32_t blob_flags,
                    struct virgl_context_blob *blob);
    void (*get_blob_done)(struct virgl_context *ctx,
                          uint32_t res_id,
                          struct virgl_context_blob *blob);

    int (*submit_cmd)(struct virgl_context *ctx,
                      const void *buffer,
                      size_t size);

    /*
     * Return an fd that is readable whenever there is any signaled fence in
     * any queue, or -1 if not supported.
     */
    int (*get_fencing_fd)(struct virgl_context *ctx);

    /* retire signaled fences of all queues */
    void (*retire_fences)(struct virgl_context *ctx);

    /* submit a fence to the queue identified by queue_id */
    int (*submit_fence)(struct virgl_context *ctx,
                        uint32_t flags,
                        uint64_t queue_id,
                        void *fence_cookie);
};

struct virgl_context_foreach_args {
    bool (*callback)(struct virgl_context *ctx, void *data);
    void *data;
};

int
virgl_context_table_init(void);

void
virgl_context_table_cleanup(void);

void
virgl_context_table_reset(void);

int
virgl_context_add(struct virgl_context *ctx);

void
virgl_context_remove(uint32_t ctx_id);

struct virgl_context *
virgl_context_lookup(uint32_t ctx_id);

void
virgl_context_foreach(const struct virgl_context_foreach_args *args);

#endif /* VIRGL_CONTEXT_H */
