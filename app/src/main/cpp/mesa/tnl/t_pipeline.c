#include "../main/glheader.h"
#include "../main/context.h"

#include "../main/mtypes.h"

#include "t_context.h"
#include "t_pipeline.h"
#include "t_vp_build.h"
#include "t_vertex.h"

void _tnl_install_pipeline( struct gl_context *ctx,
                            const struct tnl_pipeline_stage **stages )
{
    TNLcontext *tnl = TNL_CONTEXT(ctx);
    GLuint i;

    tnl->pipeline.new_state = ~0;

    /* Create a writeable copy of each stage.
     */
    for (i = 0 ; i < MAX_PIPELINE_STAGES && stages[i] ; i++) {
        struct tnl_pipeline_stage *s = &tnl->pipeline.stages[i];
        memcpy(s, stages[i], sizeof(*s));
        if (s->create)
            s->create(ctx, s);
    }

    tnl->pipeline.nr_stages = i;
}

void _tnl_destroy_pipeline( struct gl_context *ctx )
{
    TNLcontext *tnl = TNL_CONTEXT(ctx);
    GLuint i;

    for (i = 0 ; i < tnl->pipeline.nr_stages ; i++) {
        struct tnl_pipeline_stage *s = &tnl->pipeline.stages[i];
        if (s->destroy)
            s->destroy(s);
    }

    tnl->pipeline.nr_stages = 0;
}



static GLuint check_input_changes( struct gl_context *ctx )
{
    TNLcontext *tnl = TNL_CONTEXT(ctx);
    GLuint i;

    for (i = 0; i <= _TNL_LAST_MAT; i++) {
        if (tnl->vb.AttribPtr[i]->size != tnl->pipeline.last_attrib_size[i] ||
            tnl->vb.AttribPtr[i]->stride != tnl->pipeline.last_attrib_stride[i]) {
            tnl->pipeline.last_attrib_size[i] = tnl->vb.AttribPtr[i]->size;
            tnl->pipeline.last_attrib_stride[i] = tnl->vb.AttribPtr[i]->stride;
            tnl->pipeline.input_changes |= 1<<i;
        }
    }

    return tnl->pipeline.input_changes;
}


static GLuint check_output_changes( struct gl_context *ctx )
{
#if 0
    TNLcontext *tnl = TNL_CONTEXT(ctx);

   for (i = 0; i < VARYING_SLOT_MAX; i++) {
      if (tnl->vb.ResultPtr[i]->size != tnl->last_result_size[i] ||
	  tnl->vb.ResultPtr[i]->stride != tnl->last_result_stride[i]) {
	 tnl->last_result_size[i] = tnl->vb.ResultPtr[i]->size;
	 tnl->last_result_stride[i] = tnl->vb.ResultPtr[i]->stride;
	 tnl->pipeline.output_changes |= 1<<i;
      }
   }

   if (tnl->pipeline.output_changes)
      tnl->Driver.NotifyOutputChanges( ctx, tnl->pipeline.output_changes );

   return tnl->pipeline.output_changes;
#else
    return ~0;
#endif
}

/**
 * START/END_FAST_MATH macros:
 *
 * START_FAST_MATH: Set x86 FPU to faster, 32-bit precision mode (and save
 *                  original mode to a temporary).
 * END_FAST_MATH: Restore x86 FPU to original mode.
 */
#if defined(__GNUC__) && defined(__i386__)
/*
 * Set the x86 FPU control word to guarentee only 32 bits of precision
 * are stored in registers.  Allowing the FPU to store more introduces
 * differences between situations where numbers are pulled out of memory
 * vs. situations where the compiler is able to optimize register usage.
 *
 * In the worst case, we force the compiler to use a memory access to
 * truncate the float, by specifying the 'volatile' keyword.
 */
/* Hardware default: All exceptions masked, extended double precision,
 * round to nearest (IEEE compliant):
 */
#define DEFAULT_X86_FPU		0x037f
/* All exceptions masked, single precision, round to nearest:
 */
#define FAST_X86_FPU		0x003f
/* The fldcw instruction will cause any pending FP exceptions to be
 * raised prior to entering the block, and we clear any pending
 * exceptions before exiting the block.  Hence, asm code has free
 * reign over the FPU while in the fast math block.
 */
#if defined(NO_FAST_MATH)
#define START_FAST_MATH(x)						\
do {									\
   static GLuint mask = DEFAULT_X86_FPU;				\
   __asm__ ( "fnstcw %0" : "=m" (*&(x)) );				\
   __asm__ ( "fldcw %0" : : "m" (mask) );				\
} while (0)
#else
#define START_FAST_MATH(x)						\
do {									\
   static GLuint mask = FAST_X86_FPU;					\
   __asm__ ( "fnstcw %0" : "=m" (*&(x)) );				\
   __asm__ ( "fldcw %0" : : "m" (mask) );				\
} while (0)
#endif
/* Restore original FPU mode, and clear any exceptions that may have
 * occurred in the FAST_MATH block.
 */
#define END_FAST_MATH(x)						\
do {									\
   __asm__ ( "fnclex ; fldcw %0" : : "m" (*&(x)) );			\
} while (0)

#elif defined(_MSC_VER) && defined(_M_IX86)
#define DEFAULT_X86_FPU		0x037f /* See GCC comments above */
#define FAST_X86_FPU		0x003f /* See GCC comments above */
#if defined(NO_FAST_MATH)
#define START_FAST_MATH(x) do {\
	static GLuint mask = DEFAULT_X86_FPU;\
	__asm fnstcw word ptr [x]\
	__asm fldcw word ptr [mask]\
} while(0)
#else
#define START_FAST_MATH(x) do {\
	static GLuint mask = FAST_X86_FPU;\
	__asm fnstcw word ptr [x]\
	__asm fldcw word ptr [mask]\
} while(0)
#endif
#define END_FAST_MATH(x) do {\
	__asm fnclex\
	__asm fldcw word ptr [x]\
} while(0)

#else
#define START_FAST_MATH(x)  x = 0
#define END_FAST_MATH(x)  (void)(x)
#endif


void _tnl_run_pipeline( struct gl_context *ctx )
{
    TNLcontext *tnl = TNL_CONTEXT(ctx);
    unsigned short __tmp;
    GLuint i;

    if (!tnl->vb.Count)
        return;

    /* Check for changed input sizes or change in stride to/from zero
     * (ie const or non-const).
     */
    if (check_input_changes( ctx ) || tnl->pipeline.new_state) {
        if (ctx->VertexProgram._MaintainTnlProgram)
            _tnl_UpdateFixedFunctionProgram( ctx );

        for (i = 0; i < tnl->pipeline.nr_stages ; i++) {
            struct tnl_pipeline_stage *s = &tnl->pipeline.stages[i];
            if (s->validate)
                s->validate( ctx, s );
        }

        tnl->pipeline.new_state = 0;
        tnl->pipeline.input_changes = 0;

        /* Pipeline can only change its output in response to either a
         * statechange or an input size/stride change.  No other changes
         * are allowed.
         */
        if (check_output_changes( ctx ))
            _tnl_notify_pipeline_output_change( ctx );
    }

#ifndef _OPENMP
    /* Don't adjust FPU precision mode in case multiple threads are to be used.
     * This would require that the additional threads also changed the FPU mode
     * which is quite a mess as this had to be done in all parallelized sections;
     * otherwise the master thread and all other threads are running in different
     * modes, producing inconsistent results.
     * Note that all x64 implementations don't define/use START_FAST_MATH, so
     * this is "hack" is only used in i386 mode
     */
    START_FAST_MATH(__tmp);
#endif

    for (i = 0; i < tnl->pipeline.nr_stages ; i++) {
        struct tnl_pipeline_stage *s = &tnl->pipeline.stages[i];
        if (!s->run( ctx, s ))
            break;
    }

#ifndef _OPENMP
    END_FAST_MATH(__tmp);
#endif
}



/* The default pipeline.  This is useful for software rasterizers, and
 * simple hardware rasterizers.  For customization, I don't recommend
 * tampering with the internals of these stages in the way that
 * drivers did in Mesa 3.4.  These stages are basically black boxes,
 * and should be left intact.
 *
 * To customize the pipeline, consider:
 *
 * - removing redundant stages (making sure that the software rasterizer
 *   can cope with this on fallback paths).  An example is fog
 *   coordinate generation, which is not required in the FX driver.
 *
 * - replacing general-purpose machine-independent stages with
 *   general-purpose machine-specific stages.  There is no example of
 *   this to date, though it must be borne in mind that all subsequent
 *   stages that reference the output of the new stage must cope with
 *   any machine-specific data introduced.  This may not be easy
 *   unless there are no such stages (ie the new stage is the last in
 *   the pipe).
 *
 * - inserting optimized (but specialized) stages ahead of the
 *   general-purpose fallback implementation.  For example, the old
 *   fastpath mechanism, which only works when the VB->Elts input is
 *   available, can be duplicated by placing the fastpath stage at the
 *   head of this pipeline.  Such specialized stages are currently
 *   constrained to have no outputs (ie. they must either finish the *
 *   pipeline by returning GL_FALSE from run(), or do nothing).
 *
 * Some work can be done to lift some of the restrictions in the final
 * case, if it becomes necessary to do so.
 */
const struct tnl_pipeline_stage *_tnl_default_pipeline[] = {
        &_tnl_vertex_transform_stage,
        &_tnl_normal_transform_stage,
        &_tnl_lighting_stage,
        &_tnl_texgen_stage,
        &_tnl_texture_transform_stage,
        &_tnl_point_attenuation_stage,
        &_tnl_vertex_program_stage,
        &_tnl_fog_coordinate_stage,
        &_tnl_render_stage,
        NULL
};

const struct tnl_pipeline_stage *_tnl_vp_pipeline[] = {
        &_tnl_vertex_program_stage,
        &_tnl_render_stage,
        NULL
};
