#include "../main/arrayobj.h"
#include "../main/bufferobj.h"

#include "vbo_private.h"


/**
 * Called at context creation time.
 */
void vbo_save_init( struct gl_context *ctx )
{
    struct vbo_context *vbo = vbo_context(ctx);
    struct vbo_save_context *save = &vbo->save;

    vbo_save_api_init( save );

    for (gl_vertex_processing_mode vpm = VP_MODE_FF; vpm < VP_MODE_MAX; ++vpm)
        save->VAO[vpm] = NULL;

    save->no_current_update = false;

    ctx->Driver.CurrentSavePrimitive = PRIM_OUTSIDE_BEGIN_END;
}


void vbo_save_destroy( struct gl_context *ctx )
{
    struct vbo_context *vbo = vbo_context(ctx);
    struct vbo_save_context *save = &vbo->save;

    for (gl_vertex_processing_mode vpm = VP_MODE_FF; vpm < VP_MODE_MAX; ++vpm)
        _mesa_reference_vao(ctx, &save->VAO[vpm], NULL);

    if (save->prim_store) {
        if ( --save->prim_store->refcount == 0 ) {
            free(save->prim_store->prims);
            free(save->prim_store);
            save->prim_store = NULL;
        }
    }
    if (save->vertex_store) {
        _mesa_reference_buffer_object(ctx, &save->vertex_store->bufferobj, NULL);
        free(save->vertex_store);
        save->vertex_store = NULL;
    }

    _mesa_reference_buffer_object(ctx, &save->previous_ib, NULL);
}
