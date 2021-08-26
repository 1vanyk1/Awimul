#include "../include/c99_math.h"
#include "../main/glheader.h"
#include "../main/mtypes.h"
#include "../main/dd.h"

#include "t_context.h"
#include "t_pipeline.h"


struct point_stage_data {
    GLvector4f PointSize;
};

#define POINT_STAGE_DATA(stage) ((struct point_stage_data *)stage->privatePtr)


/**
 * Compute point size for each vertex from the vertex eye-space Z
 * coordinate and the point size attenuation factors.
 * Only done when point size attenuation is enabled and vertex program is
 * disabled.
 */
static GLboolean
run_point_stage(struct gl_context *ctx, struct tnl_pipeline_stage *stage)
{
    if (ctx->Point._Attenuated && !ctx->VertexProgram._Current) {
        struct point_stage_data *store = POINT_STAGE_DATA(stage);
        struct vertex_buffer *VB = &TNL_CONTEXT(ctx)->vb;
        const GLfloat *eyeCoord = (GLfloat *) VB->EyePtr->data + 2;
        const GLint eyeCoordStride = VB->EyePtr->stride / sizeof(GLfloat);
        const GLfloat p0 = ctx->Point.Params[0];
        const GLfloat p1 = ctx->Point.Params[1];
        const GLfloat p2 = ctx->Point.Params[2];
        const GLfloat pointSize = ctx->Point.Size;
        GLfloat (*size)[4] = store->PointSize.data;
        GLuint i;

        for (i = 0; i < VB->Count; i++) {
            const GLfloat dist = fabsf(*eyeCoord);
            const GLfloat q = p0 + dist * (p1 + dist * p2);
            const GLfloat atten = (q != 0.0F) ? (1.0f / sqrtf(q)) : 1.0F;
            size[i][0] = pointSize * atten; /* clamping done in rasterization */
            eyeCoord += eyeCoordStride;
        }

        VB->AttribPtr[_TNL_ATTRIB_POINTSIZE] = &store->PointSize;
    }

    return GL_TRUE;
}


static GLboolean
alloc_point_data(struct gl_context *ctx, struct tnl_pipeline_stage *stage)
{
    struct vertex_buffer *VB = &TNL_CONTEXT(ctx)->vb;
    struct point_stage_data *store;
    stage->privatePtr = malloc(sizeof(*store));
    store = POINT_STAGE_DATA(stage);
    if (!store)
        return GL_FALSE;

    _mesa_vector4f_alloc( &store->PointSize, 0, VB->Size, 32 );
    return GL_TRUE;
}


static void
free_point_data(struct tnl_pipeline_stage *stage)
{
    struct point_stage_data *store = POINT_STAGE_DATA(stage);
    if (store) {
        _mesa_vector4f_free( &store->PointSize );
        free( store );
        stage->privatePtr = NULL;
    }
}


const struct tnl_pipeline_stage _tnl_point_attenuation_stage =
        {
                "point size attenuation",	/* name */
                NULL,			/* stage private data */
                alloc_point_data,		/* alloc data */
                free_point_data,		/* destructor */
                NULL,
                run_point_stage		/* run */
        };
