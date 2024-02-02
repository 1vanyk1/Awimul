#include "../../util/u_inlines.h"
#include "../../util/u_hash_table.h"
#include "../../util/u_memory.h"
#include "../../util/simple_list.h"

#include "tr_screen.h"
#include "tr_context.h"
#include "tr_texture.h"


struct pipe_surface *
trace_surf_create(struct trace_context *tr_ctx,
                  struct pipe_resource *res,
                  struct pipe_surface *surface)
{
    struct trace_surface *tr_surf;

    if (!surface)
        goto error;

    assert(surface->texture == res);

    tr_surf = CALLOC_STRUCT(trace_surface);
    if (!tr_surf)
        goto error;

    memcpy(&tr_surf->base, surface, sizeof(struct pipe_surface));
    tr_surf->base.context = &tr_ctx->base;

    pipe_reference_init(&tr_surf->base.reference, 1);
    tr_surf->base.texture = NULL;
    pipe_resource_reference(&tr_surf->base.texture, res);
    tr_surf->surface = surface;

    return &tr_surf->base;

    error:
    pipe_surface_reference(&surface, NULL);
    return NULL;
}


void
trace_surf_destroy(struct trace_surface *tr_surf)
{
    pipe_resource_reference(&tr_surf->base.texture, NULL);
    pipe_surface_reference(&tr_surf->surface, NULL);
    FREE(tr_surf);
}


struct pipe_transfer *
trace_transfer_create(struct trace_context *tr_ctx,
                      struct pipe_resource *res,
                      struct pipe_transfer *transfer)
{
    struct trace_transfer *tr_trans;

    if (!transfer)
        goto error;

    tr_trans = CALLOC_STRUCT(trace_transfer);
    if (!tr_trans)
        goto error;

    memcpy(&tr_trans->base, transfer, tr_ctx->threaded ? sizeof(struct threaded_transfer) : sizeof(struct pipe_transfer));

    tr_trans->base.b.resource = NULL;
    tr_trans->transfer = transfer;

    pipe_resource_reference(&tr_trans->base.b.resource, res);
    assert(tr_trans->base.b.resource == res);

    return &tr_trans->base.b;

    error:
    if (res->target == PIPE_BUFFER)
        tr_ctx->pipe->buffer_unmap(tr_ctx->pipe, transfer);
    else
        tr_ctx->pipe->texture_unmap(tr_ctx->pipe, transfer);
    return NULL;
}


void
trace_transfer_destroy(struct trace_context *tr_context,
                       struct trace_transfer *tr_trans)
{
    pipe_resource_reference(&tr_trans->base.b.resource, NULL);
    FREE(tr_trans);
}

