#include "../main/glheader.h"
#include "../main/macros.h"
#include "../main/mtypes.h"

#include "../math/m_xform.h"

#include "../util/bitscan.h"
#include "../util/u_memory.h"

#include "t_context.h"
#include "t_pipeline.h"



struct vertex_stage_data {
    GLvector4f eye;
    GLvector4f clip;
    GLvector4f proj;
    GLubyte *clipmask;
    GLubyte ormask;
    GLubyte andmask;
};

#define VERTEX_STAGE_DATA(stage) ((struct vertex_stage_data *)stage->privatePtr)




/* This function implements cliptesting for user-defined clip planes.
 * The clipping of primitives to these planes is implemented in
 * t_render_clip.h.
 */
#define USER_CLIPTEST(NAME, SZ)					\
static void NAME( struct gl_context *ctx,				\
		  GLvector4f *clip,				\
		  GLubyte *clipmask,				\
		  GLubyte *clipormask,				\
		  GLubyte *clipandmask )			\
{								\
   GLbitfield mask = ctx->Transform.ClipPlanesEnabled;          \
   while (mask) {                                               \
      const int p = u_bit_scan(&mask);                          \
      GLuint nr, i;						\
      const GLfloat a = ctx->Transform._ClipUserPlane[p][0];	\
      const GLfloat b = ctx->Transform._ClipUserPlane[p][1];	\
      const GLfloat c = ctx->Transform._ClipUserPlane[p][2];	\
      const GLfloat d = ctx->Transform._ClipUserPlane[p][3];	\
      GLfloat *coord = (GLfloat *)clip->data;                   \
      GLuint stride = clip->stride;				\
      GLuint count = clip->count;				\
								\
      for (nr = 0, i = 0 ; i < count ; i++) {                   \
         GLfloat dp = coord[0] * a + coord[1] * b;		\
         if (SZ > 2) dp += coord[2] * c;			\
         if (SZ > 3) dp += coord[3] * d; else dp += d;          \
								\
         if (dp < 0) {                                          \
            nr++;						\
            clipmask[i] |= CLIP_USER_BIT;			\
         }							\
								\
         STRIDE_F(coord, stride);				\
      }                                                         \
								\
      if (nr > 0) {						\
         *clipormask |= CLIP_USER_BIT;                          \
         if (nr == count) {					\
            *clipandmask |= CLIP_USER_BIT;			\
            return;						\
         }							\
      }								\
   }								\
}


USER_CLIPTEST(userclip2, 2)
USER_CLIPTEST(userclip3, 3)
USER_CLIPTEST(userclip4, 4)

static void (*(usercliptab[5]))( struct gl_context *,
                                 GLvector4f *, GLubyte *,
                                 GLubyte *, GLubyte * ) =
        {
                NULL,
                NULL,
                userclip2,
                userclip3,
                userclip4
        };


void
tnl_clip_prepare(struct gl_context *ctx)
{
    /* Neither the x86 nor sparc asm cliptest functions have been updated
     * for ARB_depth_clamp, so force the C paths.
     */
    if (ctx->Transform.DepthClampNear && ctx->Transform.DepthClampFar) {
        static GLboolean c_funcs_installed = GL_FALSE;
        if (!c_funcs_installed) {
            init_c_cliptest();
            c_funcs_installed = GL_TRUE;
        }
    }
}



static GLboolean run_vertex_stage( struct gl_context *ctx,
                                   struct tnl_pipeline_stage *stage )
{
    struct vertex_stage_data *store = (struct vertex_stage_data *)stage->privatePtr;
    TNLcontext *tnl = TNL_CONTEXT(ctx);
    struct vertex_buffer *VB = &tnl->vb;

    if (ctx->VertexProgram._Current)
        return GL_TRUE;

    tnl_clip_prepare(ctx);

    if (ctx->_NeedEyeCoords) {
        /* Separate modelview transformation:
         * Use combined ModelProject to avoid some depth artifacts
         */
        if (ctx->ModelviewMatrixStack.Top->type == MATRIX_IDENTITY)
            VB->EyePtr = VB->AttribPtr[_TNL_ATTRIB_POS];
        else
            VB->EyePtr = TransformRaw( &store->eye,
                                       ctx->ModelviewMatrixStack.Top,
                                       VB->AttribPtr[_TNL_ATTRIB_POS]);
    }

    /* make sure the inverse is up to date */
    _math_matrix_analyse(&ctx->_ModelProjectMatrix);

    VB->ClipPtr = TransformRaw( &store->clip,
                                &ctx->_ModelProjectMatrix,
                                VB->AttribPtr[_TNL_ATTRIB_POS] );

    /* Drivers expect this to be clean to element 4...
     */
    switch (VB->ClipPtr->size) {
        case 1:
            /* impossible */
        case 2:
            _mesa_vector4f_clean_elem( VB->ClipPtr, VB->Count, 2 );
            /* fall-through */
        case 3:
            _mesa_vector4f_clean_elem( VB->ClipPtr, VB->Count, 3 );
            /* fall-through */
        case 4:
            break;
    }


    /* Cliptest and perspective divide.  Clip functions must clear
     * the clipmask.
     */
    store->ormask = 0;
    store->andmask = CLIP_FRUSTUM_BITS;

    if (tnl->NeedNdcCoords) {
        VB->NdcPtr =
                _mesa_clip_tab[VB->ClipPtr->size]( VB->ClipPtr,
                                                   &store->proj,
                                                   store->clipmask,
                                                   &store->ormask,
                                                   &store->andmask,
                                                   !(ctx->Transform.DepthClampNear &&
                                                     ctx->Transform.DepthClampFar) );
    }
    else {
        VB->NdcPtr = NULL;
        _mesa_clip_np_tab[VB->ClipPtr->size]( VB->ClipPtr,
                                              NULL,
                                              store->clipmask,
                                              &store->ormask,
                                              &store->andmask,
                                              !(ctx->Transform.DepthClampNear &&
                                                ctx->Transform.DepthClampFar) );
    }

    if (store->andmask)
        return GL_FALSE;


    /* Test userclip planes.  This contributes to VB->ClipMask, so
     * is essentially required to be in this stage.
     */
    if (ctx->Transform.ClipPlanesEnabled) {
        usercliptab[VB->ClipPtr->size]( ctx,
                                        VB->ClipPtr,
                                        store->clipmask,
                                        &store->ormask,
                                        &store->andmask );

        if (store->andmask)
            return GL_FALSE;
    }

    VB->ClipAndMask = store->andmask;
    VB->ClipOrMask = store->ormask;
    VB->ClipMask = store->clipmask;

    return GL_TRUE;
}


static GLboolean init_vertex_stage( struct gl_context *ctx,
                                    struct tnl_pipeline_stage *stage )
{
    struct vertex_buffer *VB = &TNL_CONTEXT(ctx)->vb;
    struct vertex_stage_data *store;
    GLuint size = VB->Size;

    stage->privatePtr = calloc(1, sizeof(*store));
    store = VERTEX_STAGE_DATA(stage);
    if (!store)
        return GL_FALSE;

    _mesa_vector4f_alloc( &store->eye, 0, size, 32 );
    _mesa_vector4f_alloc( &store->clip, 0, size, 32 );
    _mesa_vector4f_alloc( &store->proj, 0, size, 32 );

    store->clipmask = align_malloc(sizeof(GLubyte)*size, 32 );

    if (!store->clipmask ||
        !store->eye.data ||
        !store->clip.data ||
        !store->proj.data)
        return GL_FALSE;

    return GL_TRUE;
}

static void dtr( struct tnl_pipeline_stage *stage )
{
    struct vertex_stage_data *store = VERTEX_STAGE_DATA(stage);

    if (store) {
        _mesa_vector4f_free( &store->eye );
        _mesa_vector4f_free( &store->clip );
        _mesa_vector4f_free( &store->proj );
        align_free( store->clipmask );
        free(store);
        stage->privatePtr = NULL;
        stage->run = init_vertex_stage;
    }
}


const struct tnl_pipeline_stage _tnl_vertex_transform_stage =
        {
                "modelview/project/cliptest/divide",
                NULL,			/* private data */
                init_vertex_stage,
                dtr,				/* destructor */
                NULL,
                run_vertex_stage		/* run -- initially set to init */
        };