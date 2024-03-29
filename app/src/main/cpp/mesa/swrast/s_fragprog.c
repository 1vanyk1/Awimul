#include "../main/glheader.h"
#include "../main/macros.h"
#include "../main/samplerobj.h"
#include "../main/teximage.h"
#include "../program/prog_instruction.h"

#include "s_context.h"
#include "s_fragprog.h"
#include "s_span.h"

/**
 * \brief Should swrast use a fragment program?
 *
 * \return true if the current fragment program exists and is not the fixed
 *         function fragment program
 */
GLboolean
_swrast_use_fragment_program(struct gl_context *ctx)
{
    struct gl_program *fp = ctx->FragmentProgram._Current;
    return fp && !(fp == ctx->FragmentProgram._TexEnvProgram
                   && fp->arb.NumInstructions == 0);
}

/**
 * Apply texture object's swizzle (X/Y/Z/W/0/1) to incoming 'texel'
 * and return results in 'colorOut'.
 */
static inline void
swizzle_texel(const GLfloat texel[4], GLfloat colorOut[4], GLuint swizzle)
{
    if (swizzle == SWIZZLE_NOOP) {
        COPY_4V(colorOut, texel);
    }
    else {
        GLfloat vector[6];
        vector[SWIZZLE_X] = texel[0];
        vector[SWIZZLE_Y] = texel[1];
        vector[SWIZZLE_Z] = texel[2];
        vector[SWIZZLE_W] = texel[3];
        vector[SWIZZLE_ZERO] = 0.0F;
        vector[SWIZZLE_ONE] = 1.0F;
        colorOut[0] = vector[GET_SWZ(swizzle, 0)];
        colorOut[1] = vector[GET_SWZ(swizzle, 1)];
        colorOut[2] = vector[GET_SWZ(swizzle, 2)];
        colorOut[3] = vector[GET_SWZ(swizzle, 3)];
    }
}


/**
 * Fetch a texel with given lod.
 * Called via machine->FetchTexelLod()
 */
static void
fetch_texel_lod( struct gl_context *ctx, const GLfloat texcoord[4], GLfloat lambda,
                 GLuint unit, GLfloat color[4] )
{
    const struct gl_texture_object *texObj = ctx->Texture.Unit[unit]._Current;

    if (texObj) {
        SWcontext *swrast = SWRAST_CONTEXT(ctx);
        GLfloat rgba[4];
        const struct gl_sampler_object *samp = _mesa_get_samplerobj(ctx, unit);

        lambda = CLAMP(lambda, samp->Attrib.MinLod, samp->Attrib.MaxLod);

        swrast->TextureSample[unit](ctx, samp, ctx->Texture.Unit[unit]._Current,
                                    1, (const GLfloat (*)[4]) texcoord,
                                    &lambda, &rgba);
        swizzle_texel(rgba, color, texObj->Attrib._Swizzle);
    }
    else {
        ASSIGN_4V(color, 0.0F, 0.0F, 0.0F, 1.0F);
    }
}


/**
 * Fetch a texel with the given partial derivatives to compute a level
 * of detail in the mipmap.
 * Called via machine->FetchTexelDeriv()
 * \param lodBias  the lod bias which may be specified by a TXB instruction,
 *                 otherwise zero.
 */
static void
fetch_texel_deriv( struct gl_context *ctx, const GLfloat texcoord[4],
                   const GLfloat texdx[4], const GLfloat texdy[4],
                   GLfloat lodBias, GLuint unit, GLfloat color[4] )
{
    SWcontext *swrast = SWRAST_CONTEXT(ctx);
    const struct gl_texture_unit *texUnit = &ctx->Texture.Unit[unit];
    const struct gl_texture_object *texObj = texUnit->_Current;

    if (texObj) {
        const struct gl_texture_image *texImg = _mesa_base_tex_image(texObj);
        const struct swrast_texture_image *swImg =
                swrast_texture_image_const(texImg);
        const struct gl_sampler_object *samp = _mesa_get_samplerobj(ctx, unit);
        const GLfloat texW = (GLfloat) swImg->WidthScale;
        const GLfloat texH = (GLfloat) swImg->HeightScale;
        GLfloat lambda;
        GLfloat rgba[4];

        lambda = _swrast_compute_lambda(texdx[0], texdy[0], /* ds/dx, ds/dy */
                                        texdx[1], texdy[1], /* dt/dx, dt/dy */
                                        texdx[3], texdy[3], /* dq/dx, dq/dy */
                                        texW, texH,
                                        texcoord[0], texcoord[1], texcoord[3],
                                        1.0F / texcoord[3]);

        lambda += lodBias + texUnit->LodBias + samp->Attrib.LodBias;

        lambda = CLAMP(lambda, samp->Attrib.MinLod, samp->Attrib.MaxLod);

        swrast->TextureSample[unit](ctx, samp, ctx->Texture.Unit[unit]._Current,
                                    1, (const GLfloat (*)[4]) texcoord,
                                    &lambda, &rgba);
        swizzle_texel(rgba, color, texObj->Attrib._Swizzle);
    }
    else {
        ASSIGN_4V(color, 0.0F, 0.0F, 0.0F, 1.0F);
    }
}


/**
 * Initialize the virtual fragment program machine state prior to running
 * fragment program on a fragment.  This involves initializing the input
 * registers, condition codes, etc.
 * \param machine  the virtual machine state to init
 * \param program  the fragment program we're about to run
 * \param span  the span of pixels we'll operate on
 * \param col  which element (column) of the span we'll operate on
 */
static void
init_machine(struct gl_context *ctx, struct gl_program_machine *machine,
             const struct gl_program *program, const SWspan *span, GLuint col)
{
    GLfloat *wpos = span->array->attribs[VARYING_SLOT_POS][col];

    /* ARB_fragment_coord_conventions */
    if (program->info.fs.origin_upper_left)
        wpos[1] = ctx->DrawBuffer->Height - 1 - wpos[1];
    if (!program->info.fs.pixel_center_integer) {
        wpos[0] += 0.5F;
        wpos[1] += 0.5F;
    }

    /* Setup pointer to input attributes */
    machine->Attribs = span->array->attribs;

    machine->DerivX = (GLfloat (*)[4]) span->attrStepX;
    machine->DerivY = (GLfloat (*)[4]) span->attrStepY;
    machine->NumDeriv = VARYING_SLOT_MAX;

    machine->Samplers = program->SamplerUnits;

    /* if running a GLSL program (not ARB_fragment_program) */
    if (ctx->_Shader->CurrentProgram[MESA_SHADER_FRAGMENT]) {
        /* Store front/back facing value */
        machine->Attribs[VARYING_SLOT_FACE][col][0] = 1.0F - span->facing;
    }

    machine->CurElement = col;

    /* init call stack */
    machine->StackDepth = 0;

    machine->FetchTexelLod = fetch_texel_lod;
    machine->FetchTexelDeriv = fetch_texel_deriv;
}


/**
 * Run fragment program on the pixels in span from 'start' to 'end' - 1.
 */
static void
run_program(struct gl_context *ctx, SWspan *span, GLuint start, GLuint end)
{
    SWcontext *swrast = SWRAST_CONTEXT(ctx);
    const struct gl_program *program = ctx->FragmentProgram._Current;
    const GLbitfield64 outputsWritten = program->info.outputs_written;
    struct gl_program_machine *machine = &swrast->FragProgMachine;
    GLuint i;

    for (i = start; i < end; i++) {
        if (span->array->mask[i]) {
            init_machine(ctx, machine, program, span, i);

            if (_mesa_execute_program(ctx, program, machine)) {

                /* Store result color */
                if (outputsWritten & BITFIELD64_BIT(FRAG_RESULT_COLOR)) {
                    COPY_4V(span->array->attribs[VARYING_SLOT_COL0][i],
                            machine->Outputs[FRAG_RESULT_COLOR]);
                }
                else {
                    /* Multiple drawbuffers / render targets
                     * Note that colors beyond 0 and 1 will overwrite other
                     * attributes, such as FOGC, TEX0, TEX1, etc.  That's OK.
                     */
                    GLuint buf;
                    for (buf = 0; buf < ctx->DrawBuffer->_NumColorDrawBuffers; buf++) {
                        if (outputsWritten & BITFIELD64_BIT(FRAG_RESULT_DATA0 + buf)) {
                            COPY_4V(span->array->attribs[VARYING_SLOT_COL0 + buf][i],
                                    machine->Outputs[FRAG_RESULT_DATA0 + buf]);
                        }
                    }
                }

                /* Store result depth/z */
                if (outputsWritten & BITFIELD64_BIT(FRAG_RESULT_DEPTH)) {
                    const GLfloat depth = machine->Outputs[FRAG_RESULT_DEPTH][2];
                    if (depth <= 0.0F)
                        span->array->z[i] = 0;
                    else if (depth >= 1.0F)
                        span->array->z[i] = ctx->DrawBuffer->_DepthMax;
                    else
                        span->array->z[i] =
                                (GLuint) (depth * ctx->DrawBuffer->_DepthMaxF + 0.5F);
                }
            }
            else {
                /* killed fragment */
                span->array->mask[i] = GL_FALSE;
                span->writeAll = GL_FALSE;
            }
        }
    }
}


/**
 * Execute the current fragment program for all the fragments
 * in the given span.
 */
void
_swrast_exec_fragment_program( struct gl_context *ctx, SWspan *span )
{
    const struct gl_program *program = ctx->FragmentProgram._Current;

    /* incoming colors should be floats */
    if (program->info.inputs_read & VARYING_BIT_COL0) {
        assert(span->array->ChanType == GL_FLOAT);
    }

    run_program(ctx, span, 0, span->end);

    if (program->info.outputs_written & BITFIELD64_BIT(FRAG_RESULT_COLOR)) {
        span->interpMask &= ~SPAN_RGBA;
        span->arrayMask |= SPAN_RGBA;
    }

    if (program->info.outputs_written & BITFIELD64_BIT(FRAG_RESULT_DEPTH)) {
        span->interpMask &= ~SPAN_Z;
        span->arrayMask |= SPAN_Z;
    }
}

