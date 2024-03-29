#include "../main/glheader.h"
#include "../main/macros.h"

#include "../main/mtypes.h"

#include "../math/m_xform.h"

#include "t_context.h"
#include "t_pipeline.h"

/* Is there any real benefit seperating texmat from texgen?  It means
 * we need two lots of intermediate storage.  Any changes to
 * _NEW_TEXTURE will invalidate both sets -- it's only on changes to
 * *only* _NEW_TEXTURE_MATRIX that texgen survives but texmat doesn't.
 *
 * However, the seperation of this code from the complex texgen stuff
 * is very appealing.
 */
struct texmat_stage_data {
    GLvector4f texcoord[MAX_TEXTURE_COORD_UNITS];
};

#define TEXMAT_STAGE_DATA(stage) ((struct texmat_stage_data *)stage->privatePtr)



static GLboolean run_texmat_stage( struct gl_context *ctx,
                                   struct tnl_pipeline_stage *stage )
{
    struct texmat_stage_data *store = TEXMAT_STAGE_DATA(stage);
    struct vertex_buffer *VB = &TNL_CONTEXT(ctx)->vb;
    GLuint i;

    if (!ctx->Texture._TexMatEnabled || ctx->VertexProgram._Current)
        return GL_TRUE;

    /* ENABLE_TEXMAT implies that the texture matrix is not the
     * identity, so we don't have to check that here.
     */
    for (i = 0 ; i < ctx->Const.MaxTextureCoordUnits ; i++) {
        if (ctx->Texture._TexMatEnabled & ENABLE_TEXMAT(i)) {
            (void) TransformRaw( &store->texcoord[i],
                                 ctx->TextureMatrixStack[i].Top,
                                 VB->AttribPtr[_TNL_ATTRIB_TEX0 + i]);

            VB->AttribPtr[VERT_ATTRIB_TEX0+i] = &store->texcoord[i];
        }
    }

    return GL_TRUE;
}


/* Called the first time stage->run() is invoked.
 */
static GLboolean alloc_texmat_data( struct gl_context *ctx,
                                    struct tnl_pipeline_stage *stage )
{
    struct vertex_buffer *VB = &TNL_CONTEXT(ctx)->vb;
    struct texmat_stage_data *store;
    GLuint i;

    stage->privatePtr = calloc(1, sizeof(*store));
    store = TEXMAT_STAGE_DATA(stage);
    if (!store)
        return GL_FALSE;

    for (i = 0 ; i < ctx->Const.MaxTextureCoordUnits ; i++)
        _mesa_vector4f_alloc( &store->texcoord[i], 0, VB->Size, 32 );

    return GL_TRUE;
}


static void free_texmat_data( struct tnl_pipeline_stage *stage )
{
    struct texmat_stage_data *store = TEXMAT_STAGE_DATA(stage);
    GLuint i;

    if (store) {
        for (i = 0; i < MAX_TEXTURE_COORD_UNITS; i++)
            if (store->texcoord[i].data)
                _mesa_vector4f_free( &store->texcoord[i] );
        free( store );
        stage->privatePtr = NULL;
    }
}



const struct tnl_pipeline_stage _tnl_texture_transform_stage =
        {
                "texture transform",			/* name */
                NULL,				/* private data */
                alloc_texmat_data,
                free_texmat_data,			/* destructor */
                NULL,
                run_texmat_stage,
        };
