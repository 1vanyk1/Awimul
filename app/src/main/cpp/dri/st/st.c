#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../../GL/gl.h"
#include "../../GL/internal/dri_interface.h"
#include "../../GL/glxtokens.h"
#include "../winsys_handle.h"
#include "../dri_interface.h"
#include "../../zstd/common/debug.h"
#include <pthread.h>
#include "../dri.h"
#include "../dri_private.h"
#include "../errors.h"
#include "../format_info.h"

#include "st.h"

int ST_DEBUG = 0;

static void
st_framebuffer_iface_remove(struct st_manager *smapi,
                            struct st_framebuffer_iface *stfbi)
{
    struct st_manager_private *smPriv =
            (struct st_manager_private *)smapi->st_manager_private;
    struct hash_entry *entry;

    if (!smPriv || !smPriv->stfbi_ht)
        return;

    simple_mtx_lock(&smPriv->st_mutex);
    entry = _mesa_hash_table_search(smPriv->stfbi_ht, stfbi);
    if (!entry)
        goto unlock;

    _mesa_hash_table_remove(smPriv->stfbi_ht, entry);

    unlock:
    simple_mtx_unlock(&smPriv->st_mutex);
}

static void
st_api_destroy_drawable(struct st_api *stapi,
                        struct st_framebuffer_iface *stfbi)
{
    if (!stfbi)
        return;

    st_framebuffer_iface_remove(stfbi->state_manager, stfbi);
}

static void
st_api_destroy(struct st_api *stapi)
{
}

static struct st_context_iface *
st_api_get_current(struct st_api *stapi)
{
    GET_CURRENT_CONTEXT(ctx);
    struct st_context *st = ctx ? ctx->st : NULL;

    return st ? &st->iface : NULL;
}

static unsigned _min(unsigned a, unsigned b)
{
    return (a < b) ? a : b;
}

static float _maxf(float a, float b)
{
    return (a > b) ? a : b;
}

static int _clamp(int a, int min, int max)
{
    if (a < min)
        return min;
    else if (a > max)
        return max;
    else
        return a;
}

void st_init_limits(struct pipe_screen *screen,
                    struct gl_constants *c, struct gl_extensions *extensions)
{
    int supported_irs;
    unsigned sh;
    bool can_ubo = true;
    int temp;

    c->MaxTextureSize = screen->get_param(screen, PIPE_CAP_MAX_TEXTURE_2D_SIZE);
    c->MaxTextureSize = MIN2(c->MaxTextureSize, 1 << (MAX_TEXTURE_LEVELS - 1));
    c->MaxTextureMbytes = MAX2(c->MaxTextureMbytes,
                               screen->get_param(screen, PIPE_CAP_MAX_TEXTURE_MB));

    c->Max3DTextureLevels
            = _min(screen->get_param(screen, PIPE_CAP_MAX_TEXTURE_3D_LEVELS),
                   MAX_TEXTURE_LEVELS);

    c->MaxCubeTextureLevels
            = _min(screen->get_param(screen, PIPE_CAP_MAX_TEXTURE_CUBE_LEVELS),
                   MAX_TEXTURE_LEVELS);

    c->MaxTextureRectSize = _min(c->MaxTextureSize, MAX_TEXTURE_RECT_SIZE);

    c->MaxArrayTextureLayers
            = screen->get_param(screen, PIPE_CAP_MAX_TEXTURE_ARRAY_LAYERS);

    /* Define max viewport size and max renderbuffer size in terms of
     * max texture size (note: max tex RECT size = max tex 2D size).
     * If this isn't true for some hardware we'll need new PIPE_CAP_ queries.
     */
    c->MaxViewportWidth =
    c->MaxViewportHeight =
    c->MaxRenderbufferSize = c->MaxTextureRectSize;

    c->SubPixelBits =
            screen->get_param(screen, PIPE_CAP_RASTERIZER_SUBPIXEL_BITS);
    c->ViewportSubpixelBits =
            screen->get_param(screen, PIPE_CAP_VIEWPORT_SUBPIXEL_BITS);

    c->MaxDrawBuffers = c->MaxColorAttachments =
            _clamp(screen->get_param(screen, PIPE_CAP_MAX_RENDER_TARGETS),
                   1, MAX_DRAW_BUFFERS);

    c->MaxDualSourceDrawBuffers =
            _clamp(screen->get_param(screen,
                                     PIPE_CAP_MAX_DUAL_SOURCE_RENDER_TARGETS),
                   0, MAX_DRAW_BUFFERS);

    c->MaxLineWidth =
            _maxf(1.0f, screen->get_paramf(screen, PIPE_CAPF_MAX_LINE_WIDTH));
    c->MaxLineWidthAA =
            _maxf(1.0f, screen->get_paramf(screen, PIPE_CAPF_MAX_LINE_WIDTH_AA));

    c->MinLineWidth = screen->get_paramf(screen, PIPE_CAPF_MIN_LINE_WIDTH);
    c->MinLineWidthAA = screen->get_paramf(screen, PIPE_CAPF_MIN_LINE_WIDTH_AA);
    c->LineWidthGranularity = screen->get_paramf(screen, PIPE_CAPF_LINE_WIDTH_GRANULARITY);

    c->MaxPointSize =
            _maxf(1.0f, screen->get_paramf(screen, PIPE_CAPF_MAX_POINT_SIZE));
    c->MaxPointSizeAA =
            _maxf(1.0f, screen->get_paramf(screen, PIPE_CAPF_MAX_POINT_SIZE_AA));

    c->MinPointSize = MAX2(screen->get_paramf(screen, PIPE_CAPF_MIN_POINT_SIZE), 0.01);
    c->MinPointSizeAA = MAX2(screen->get_paramf(screen, PIPE_CAPF_MIN_POINT_SIZE_AA), 0.01);
    c->PointSizeGranularity = screen->get_paramf(screen, PIPE_CAPF_POINT_SIZE_GRANULARITY);

    c->MaxTextureMaxAnisotropy =
            _maxf(2.0f,
                  screen->get_paramf(screen, PIPE_CAPF_MAX_TEXTURE_ANISOTROPY));

    c->MaxTextureLodBias =
            screen->get_paramf(screen, PIPE_CAPF_MAX_TEXTURE_LOD_BIAS);

    c->QuadsFollowProvokingVertexConvention =
            screen->get_param(screen,
                              PIPE_CAP_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION);

    c->MaxUniformBlockSize =
            screen->get_shader_param(screen, PIPE_SHADER_FRAGMENT,
                                     PIPE_SHADER_CAP_MAX_CONST_BUFFER_SIZE);
    /* GL45-CTS.enhanced_layouts.ssb_member_invalid_offset_alignment fails if
     * this is larger than INT_MAX - 100. Use a nicely aligned limit.
     */
    c->MaxUniformBlockSize = MIN2(c->MaxUniformBlockSize, INT_MAX - 127);

    if (c->MaxUniformBlockSize < 16384) {
        can_ubo = false;
    }

    for (sh = 0; sh < PIPE_SHADER_TYPES; ++sh) {
        struct gl_shader_compiler_options *options;
        struct gl_program_constants *pc;
        const nir_shader_compiler_options *nir_options = NULL;

        bool prefer_nir = PIPE_SHADER_IR_NIR ==
                          screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_PREFERRED_IR);

        if (screen->get_compiler_options && prefer_nir) {
            nir_options = (const nir_shader_compiler_options *)
                    screen->get_compiler_options(screen, PIPE_SHADER_IR_NIR, sh);
        }

        const gl_shader_stage stage = tgsi_processor_to_shader_stage(sh);
        pc = &c->Program[stage];
        options = &c->ShaderCompilerOptions[stage];
        c->ShaderCompilerOptions[stage].NirOptions = nir_options;

        if (sh == PIPE_SHADER_COMPUTE) {
            if (!screen->get_param(screen, PIPE_CAP_COMPUTE))
                continue;
            supported_irs =
                    screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_SUPPORTED_IRS);
            if (!(supported_irs & ((1 << PIPE_SHADER_IR_TGSI) |
                                   (1 << PIPE_SHADER_IR_NIR))))
                continue;
        }

        pc->MaxTextureImageUnits =
                _min(screen->get_shader_param(screen, sh,
                                              PIPE_SHADER_CAP_MAX_TEXTURE_SAMPLERS),
                     MAX_TEXTURE_IMAGE_UNITS);

        pc->MaxInstructions =
        pc->MaxNativeInstructions =
                screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_MAX_INSTRUCTIONS);
        pc->MaxAluInstructions =
        pc->MaxNativeAluInstructions =
                screen->get_shader_param(screen, sh,
                                         PIPE_SHADER_CAP_MAX_ALU_INSTRUCTIONS);
        pc->MaxTexInstructions =
        pc->MaxNativeTexInstructions =
                screen->get_shader_param(screen, sh,
                                         PIPE_SHADER_CAP_MAX_TEX_INSTRUCTIONS);
        pc->MaxTexIndirections =
        pc->MaxNativeTexIndirections =
                screen->get_shader_param(screen, sh,
                                         PIPE_SHADER_CAP_MAX_TEX_INDIRECTIONS);
        pc->MaxAttribs =
        pc->MaxNativeAttribs =
                screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_MAX_INPUTS);
        pc->MaxTemps =
        pc->MaxNativeTemps =
                screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_MAX_TEMPS);
        pc->MaxAddressRegs =
        pc->MaxNativeAddressRegs = sh == PIPE_SHADER_VERTEX ? 1 : 0;

        pc->MaxUniformComponents =
                screen->get_shader_param(screen, sh,
                                         PIPE_SHADER_CAP_MAX_CONST_BUFFER_SIZE) / 4;

        /* reserve space in the default-uniform for lowered state */
        if (sh == PIPE_SHADER_VERTEX ||
            sh == PIPE_SHADER_TESS_EVAL ||
            sh == PIPE_SHADER_GEOMETRY) {

            if (!screen->get_param(screen, PIPE_CAP_CLIP_PLANES))
                pc->MaxUniformComponents -= 4 * MAX_CLIP_PLANES;

            if (!screen->get_param(screen, PIPE_CAP_POINT_SIZE_FIXED))
                pc->MaxUniformComponents -= 4;
        } else if (sh == PIPE_SHADER_FRAGMENT) {
            if (!screen->get_param(screen, PIPE_CAP_ALPHA_TEST))
                pc->MaxUniformComponents -= 4;
        }


        pc->MaxUniformComponents = MIN2(pc->MaxUniformComponents,
                                        MAX_UNIFORMS * 4);

        /* For ARB programs, prog_src_register::Index is a signed 13-bit number.
         * This gives us a limit of 4096 values - but we may need to generate
         * internal values in addition to what the source program uses.  So, we
         * drop the limit one step lower, to 2048, to be safe.
         */
        pc->MaxParameters =
        pc->MaxNativeParameters = MIN2(pc->MaxUniformComponents / 4, 2048);
        pc->MaxInputComponents =
                screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_MAX_INPUTS) * 4;
        pc->MaxOutputComponents =
                screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_MAX_OUTPUTS) * 4;


        pc->MaxUniformBlocks =
                screen->get_shader_param(screen, sh,
                                         PIPE_SHADER_CAP_MAX_CONST_BUFFERS);
        if (pc->MaxUniformBlocks)
            pc->MaxUniformBlocks -= 1; /* The first one is for ordinary uniforms. */
        pc->MaxUniformBlocks = _min(pc->MaxUniformBlocks, MAX_UNIFORM_BUFFERS);

        pc->MaxCombinedUniformComponents =
                pc->MaxUniformComponents +
                (uint64_t)c->MaxUniformBlockSize / 4 * pc->MaxUniformBlocks;

        pc->MaxShaderStorageBlocks =
                screen->get_shader_param(screen, sh,
                                         PIPE_SHADER_CAP_MAX_SHADER_BUFFERS);

        temp = screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_MAX_HW_ATOMIC_COUNTERS);
        if (temp) {
            /*
             * for separate atomic counters get the actual hw limits
             * per stage on atomic counters and buffers
             */
            pc->MaxAtomicCounters = temp;
            pc->MaxAtomicBuffers = screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_MAX_HW_ATOMIC_COUNTER_BUFFERS);
        } else if (pc->MaxShaderStorageBlocks) {
            pc->MaxAtomicCounters = MAX_ATOMIC_COUNTERS;
            /*
             * without separate atomic counters, reserve half of the available
             * SSBOs for atomic buffers, and the other half for normal SSBOs.
             */
            pc->MaxAtomicBuffers = pc->MaxShaderStorageBlocks / 2;
            pc->MaxShaderStorageBlocks -= pc->MaxAtomicBuffers;
        }
        pc->MaxImageUniforms = screen->get_shader_param(
                screen, sh, PIPE_SHADER_CAP_MAX_SHADER_IMAGES);

        /* Gallium doesn't really care about local vs. env parameters so use the
         * same limits.
         */
        pc->MaxLocalParams = MIN2(pc->MaxParameters, MAX_PROGRAM_LOCAL_PARAMS);
        pc->MaxEnvParams = MIN2(pc->MaxParameters, MAX_PROGRAM_ENV_PARAMS);

        if (screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_INTEGERS)) {
            pc->LowInt.RangeMin = 31;
            pc->LowInt.RangeMax = 30;
            pc->LowInt.Precision = 0;
            pc->MediumInt = pc->HighInt = pc->LowInt;

            if (screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_INT16)) {
                pc->LowInt.RangeMin = 15;
                pc->LowInt.RangeMax = 14;
                pc->MediumInt = pc->LowInt;
            }
        }

        if (screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_FP16)) {
            pc->LowFloat.RangeMin = 15;
            pc->LowFloat.RangeMax = 15;
            pc->LowFloat.Precision = 10;
            pc->MediumFloat = pc->LowFloat;
        }

        /* TODO: make these more fine-grained if anyone needs it */
        options->MaxIfDepth =
                screen->get_shader_param(screen, sh,
                                         PIPE_SHADER_CAP_MAX_CONTROL_FLOW_DEPTH);

        /* If we're using NIR, then leave GLSL loop handling to NIR.  If we set
         * this flag, then GLSL jump lowering will turn the breaks into something
         * that GLSL loop unrolling can't handle, and then you get linker failures
         * about samplers with non-const indexes in loops that should be unrollable.
         */
        options->EmitNoLoops = !prefer_nir &&
                               !screen->get_shader_param(screen, sh,
                                                         PIPE_SHADER_CAP_MAX_CONTROL_FLOW_DEPTH);

        options->EmitNoMainReturn =
                !screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_SUBROUTINES);

        options->EmitNoCont =
                !screen->get_shader_param(screen, sh,
                                          PIPE_SHADER_CAP_TGSI_CONT_SUPPORTED);

        options->EmitNoIndirectInput =
                !screen->get_shader_param(screen, sh,
                                          PIPE_SHADER_CAP_INDIRECT_INPUT_ADDR);
        options->EmitNoIndirectOutput =
                !screen->get_shader_param(screen, sh,
                                          PIPE_SHADER_CAP_INDIRECT_OUTPUT_ADDR);
        options->EmitNoIndirectTemp =
                !screen->get_shader_param(screen, sh,
                                          PIPE_SHADER_CAP_INDIRECT_TEMP_ADDR);
        options->EmitNoIndirectUniform =
                !screen->get_shader_param(screen, sh,
                                          PIPE_SHADER_CAP_INDIRECT_CONST_ADDR);

        if (pc->MaxNativeInstructions &&
            (options->EmitNoIndirectUniform || pc->MaxUniformBlocks < 12)) {
            can_ubo = false;
        }

        if (options->EmitNoLoops)
            options->MaxUnrollIterations =
                    MIN2(screen->get_shader_param(screen, sh,
                                                  PIPE_SHADER_CAP_MAX_INSTRUCTIONS),
                         65536);
        else
            options->MaxUnrollIterations =
                    screen->get_shader_param(screen, sh,
                                             PIPE_SHADER_CAP_MAX_UNROLL_ITERATIONS_HINT);

        if (!screen->get_param(screen, PIPE_CAP_NIR_COMPACT_ARRAYS))
            options->LowerCombinedClipCullDistance = true;

        /* NIR can do the lowering on our behalf and we'll get better results
         * because it can actually optimize SSBO access.
         */
        options->LowerBufferInterfaceBlocks = !prefer_nir;

        if (sh == PIPE_SHADER_VERTEX || sh == PIPE_SHADER_GEOMETRY) {
            if (screen->get_param(screen, PIPE_CAP_VIEWPORT_TRANSFORM_LOWERED))
                options->LowerBuiltinVariablesXfb |= VARYING_BIT_POS;
            if (screen->get_param(screen, PIPE_CAP_PSIZ_CLAMPED))
                options->LowerBuiltinVariablesXfb |= VARYING_BIT_PSIZ;
        }

        if (prefer_nir) {
            options->LowerPrecisionFloat16 =
                    screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_FP16);
            options->LowerPrecisionDerivatives =
                    screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_FP16_DERIVATIVES);
            options->LowerPrecisionInt16 =
                    screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_INT16);
            options->LowerPrecisionConstants =
                    screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_GLSL_16BIT_CONSTS);
            options->LowerPrecisionFloat16Uniforms =
                    screen->get_shader_param(screen, sh, PIPE_SHADER_CAP_FP16_CONST_BUFFERS);
        }
    }

    c->MaxUserAssignableUniformLocations =
            c->Program[MESA_SHADER_VERTEX].MaxUniformComponents +
            c->Program[MESA_SHADER_TESS_CTRL].MaxUniformComponents +
            c->Program[MESA_SHADER_TESS_EVAL].MaxUniformComponents +
            c->Program[MESA_SHADER_GEOMETRY].MaxUniformComponents +
            c->Program[MESA_SHADER_FRAGMENT].MaxUniformComponents;

    c->GLSLOptimizeConservatively =
            screen->get_param(screen, PIPE_CAP_GLSL_OPTIMIZE_CONSERVATIVELY);
    c->GLSLLowerConstArrays =
            screen->get_param(screen, PIPE_CAP_PREFER_IMM_ARRAYS_AS_CONSTBUF);
    c->GLSLTessLevelsAsInputs =
            screen->get_param(screen, PIPE_CAP_GLSL_TESS_LEVELS_AS_INPUTS);
    c->LowerTessLevel =
            !screen->get_param(screen, PIPE_CAP_NIR_COMPACT_ARRAYS);
    c->LowerCsDerivedVariables =
            !screen->get_param(screen, PIPE_CAP_CS_DERIVED_SYSTEM_VALUES_SUPPORTED);
    c->PrimitiveRestartForPatches = false;

    c->MaxCombinedTextureImageUnits =
            _min(c->Program[MESA_SHADER_VERTEX].MaxTextureImageUnits +
                 c->Program[MESA_SHADER_TESS_CTRL].MaxTextureImageUnits +
                 c->Program[MESA_SHADER_TESS_EVAL].MaxTextureImageUnits +
                 c->Program[MESA_SHADER_GEOMETRY].MaxTextureImageUnits +
                 c->Program[MESA_SHADER_FRAGMENT].MaxTextureImageUnits +
                 c->Program[MESA_SHADER_COMPUTE].MaxTextureImageUnits,
                 MAX_COMBINED_TEXTURE_IMAGE_UNITS);

    /* This depends on program constants. */
    c->MaxTextureCoordUnits
            = _min(c->Program[MESA_SHADER_FRAGMENT].MaxTextureImageUnits,
                   MAX_TEXTURE_COORD_UNITS);

    c->MaxTextureUnits =
            _min(c->Program[MESA_SHADER_FRAGMENT].MaxTextureImageUnits,
                 c->MaxTextureCoordUnits);

    c->Program[MESA_SHADER_VERTEX].MaxAttribs =
            MIN2(c->Program[MESA_SHADER_VERTEX].MaxAttribs, 16);

    c->MaxVarying = screen->get_param(screen, PIPE_CAP_MAX_VARYINGS);
    c->MaxVarying = MIN2(c->MaxVarying, MAX_VARYING);
    c->MaxGeometryOutputVertices =
            screen->get_param(screen, PIPE_CAP_MAX_GEOMETRY_OUTPUT_VERTICES);
    c->MaxGeometryTotalOutputComponents =
            screen->get_param(screen, PIPE_CAP_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS);
    c->MaxGeometryShaderInvocations =
            screen->get_param(screen, PIPE_CAP_MAX_GS_INVOCATIONS);
    c->MaxTessPatchComponents =
            MIN2(screen->get_param(screen, PIPE_CAP_MAX_SHADER_PATCH_VARYINGS),
                 MAX_VARYING) * 4;

    c->MinProgramTexelOffset =
            screen->get_param(screen, PIPE_CAP_MIN_TEXEL_OFFSET);
    c->MaxProgramTexelOffset =
            screen->get_param(screen, PIPE_CAP_MAX_TEXEL_OFFSET);

    c->MaxProgramTextureGatherComponents =
            screen->get_param(screen, PIPE_CAP_MAX_TEXTURE_GATHER_COMPONENTS);
    c->MinProgramTextureGatherOffset =
            screen->get_param(screen, PIPE_CAP_MIN_TEXTURE_GATHER_OFFSET);
    c->MaxProgramTextureGatherOffset =
            screen->get_param(screen, PIPE_CAP_MAX_TEXTURE_GATHER_OFFSET);

    c->MaxTransformFeedbackBuffers =
            screen->get_param(screen, PIPE_CAP_MAX_STREAM_OUTPUT_BUFFERS);
    c->MaxTransformFeedbackBuffers = MIN2(c->MaxTransformFeedbackBuffers,
                                          MAX_FEEDBACK_BUFFERS);
    c->MaxTransformFeedbackSeparateComponents =
            screen->get_param(screen, PIPE_CAP_MAX_STREAM_OUTPUT_SEPARATE_COMPONENTS);
    c->MaxTransformFeedbackInterleavedComponents =
            screen->get_param(screen,
                              PIPE_CAP_MAX_STREAM_OUTPUT_INTERLEAVED_COMPONENTS);
    c->MaxVertexStreams =
            MAX2(1, screen->get_param(screen, PIPE_CAP_MAX_VERTEX_STREAMS));

    /* The vertex stream must fit into pipe_stream_output_info::stream */
    assert(c->MaxVertexStreams <= 4);

    c->MaxVertexAttribStride
            = screen->get_param(screen, PIPE_CAP_MAX_VERTEX_ATTRIB_STRIDE);

    /* The value cannot be larger than that since pipe_vertex_buffer::src_offset
     * is only 16 bits.
     */
    temp = screen->get_param(screen, PIPE_CAP_MAX_VERTEX_ELEMENT_SRC_OFFSET);
    c->MaxVertexAttribRelativeOffset = MIN2(0xffff, temp);

    c->GLSLSkipStrictMaxUniformLimitCheck =
            screen->get_param(screen, PIPE_CAP_TGSI_CAN_COMPACT_CONSTANTS);

    c->UniformBufferOffsetAlignment =
            screen->get_param(screen, PIPE_CAP_CONSTANT_BUFFER_OFFSET_ALIGNMENT);

    if (can_ubo) {
        extensions->ARB_uniform_buffer_object = GL_TRUE;
        c->MaxCombinedUniformBlocks = c->MaxUniformBufferBindings =
                c->Program[MESA_SHADER_VERTEX].MaxUniformBlocks +
                c->Program[MESA_SHADER_TESS_CTRL].MaxUniformBlocks +
                c->Program[MESA_SHADER_TESS_EVAL].MaxUniformBlocks +
                c->Program[MESA_SHADER_GEOMETRY].MaxUniformBlocks +
                c->Program[MESA_SHADER_FRAGMENT].MaxUniformBlocks +
                c->Program[MESA_SHADER_COMPUTE].MaxUniformBlocks;
        assert(c->MaxCombinedUniformBlocks <= MAX_COMBINED_UNIFORM_BUFFERS);
    }

    c->GLSLFragCoordIsSysVal =
            screen->get_param(screen, PIPE_CAP_TGSI_FS_POSITION_IS_SYSVAL);
    c->GLSLPointCoordIsSysVal =
            screen->get_param(screen, PIPE_CAP_TGSI_FS_POINT_IS_SYSVAL);
    c->GLSLFrontFacingIsSysVal =
            screen->get_param(screen, PIPE_CAP_TGSI_FS_FACE_IS_INTEGER_SYSVAL);

    /* GL_ARB_get_program_binary */
    if (screen->get_disk_shader_cache && screen->get_disk_shader_cache(screen))
        c->NumProgramBinaryFormats = 1;

    c->MaxAtomicBufferBindings =
            MAX2(c->Program[MESA_SHADER_FRAGMENT].MaxAtomicBuffers,
                 c->Program[MESA_SHADER_COMPUTE].MaxAtomicBuffers);
    c->MaxAtomicBufferSize = ATOMIC_COUNTER_SIZE *
                             MAX2(c->Program[MESA_SHADER_FRAGMENT].MaxAtomicCounters,
                                  c->Program[MESA_SHADER_COMPUTE].MaxAtomicCounters);

    c->MaxCombinedAtomicBuffers =
            MIN2(screen->get_param(screen,
                                   PIPE_CAP_MAX_COMBINED_HW_ATOMIC_COUNTER_BUFFERS),
                 MAX_COMBINED_ATOMIC_BUFFERS);
    if (!c->MaxCombinedAtomicBuffers) {
        c->MaxCombinedAtomicBuffers = MAX2(
                c->Program[MESA_SHADER_VERTEX].MaxAtomicBuffers +
                c->Program[MESA_SHADER_TESS_CTRL].MaxAtomicBuffers +
                c->Program[MESA_SHADER_TESS_EVAL].MaxAtomicBuffers +
                c->Program[MESA_SHADER_GEOMETRY].MaxAtomicBuffers +
                c->Program[MESA_SHADER_FRAGMENT].MaxAtomicBuffers,
                c->Program[MESA_SHADER_COMPUTE].MaxAtomicBuffers);
        assert(c->MaxCombinedAtomicBuffers <= MAX_COMBINED_ATOMIC_BUFFERS);
    }

    c->MaxCombinedAtomicCounters =
            screen->get_param(screen, PIPE_CAP_MAX_COMBINED_HW_ATOMIC_COUNTERS);
    if (!c->MaxCombinedAtomicCounters)
        c->MaxCombinedAtomicCounters = MAX_ATOMIC_COUNTERS;

    if (c->Program[MESA_SHADER_FRAGMENT].MaxAtomicBuffers) {
        extensions->ARB_shader_atomic_counters = GL_TRUE;
        extensions->ARB_shader_atomic_counter_ops = GL_TRUE;
    }

    c->MaxCombinedShaderOutputResources = c->MaxDrawBuffers;
    c->ShaderStorageBufferOffsetAlignment =
            screen->get_param(screen, PIPE_CAP_SHADER_BUFFER_OFFSET_ALIGNMENT);
    if (c->ShaderStorageBufferOffsetAlignment) {
        c->MaxCombinedShaderStorageBlocks =
                MIN2(screen->get_param(screen, PIPE_CAP_MAX_COMBINED_SHADER_BUFFERS),
                     MAX_COMBINED_SHADER_STORAGE_BUFFERS);
        if (!c->MaxCombinedShaderStorageBlocks) {
            c->MaxCombinedShaderStorageBlocks = MAX2(
                    c->Program[MESA_SHADER_VERTEX].MaxShaderStorageBlocks +
                    c->Program[MESA_SHADER_TESS_CTRL].MaxShaderStorageBlocks +
                    c->Program[MESA_SHADER_TESS_EVAL].MaxShaderStorageBlocks +
                    c->Program[MESA_SHADER_GEOMETRY].MaxShaderStorageBlocks +
                    c->Program[MESA_SHADER_FRAGMENT].MaxShaderStorageBlocks,
                    c->Program[MESA_SHADER_COMPUTE].MaxShaderStorageBlocks);
            assert(c->MaxCombinedShaderStorageBlocks < MAX_COMBINED_SHADER_STORAGE_BUFFERS);
        }
        c->MaxShaderStorageBufferBindings = c->MaxCombinedShaderStorageBlocks;

        c->MaxCombinedShaderOutputResources +=
                c->MaxCombinedShaderStorageBlocks;
        c->MaxShaderStorageBlockSize =
                screen->get_param(screen, PIPE_CAP_MAX_SHADER_BUFFER_SIZE);
        if (c->Program[MESA_SHADER_FRAGMENT].MaxShaderStorageBlocks)
            extensions->ARB_shader_storage_buffer_object = GL_TRUE;
    }

    c->MaxCombinedImageUniforms =
            c->Program[MESA_SHADER_VERTEX].MaxImageUniforms +
            c->Program[MESA_SHADER_TESS_CTRL].MaxImageUniforms +
            c->Program[MESA_SHADER_TESS_EVAL].MaxImageUniforms +
            c->Program[MESA_SHADER_GEOMETRY].MaxImageUniforms +
            c->Program[MESA_SHADER_FRAGMENT].MaxImageUniforms +
            c->Program[MESA_SHADER_COMPUTE].MaxImageUniforms;
    c->MaxCombinedShaderOutputResources += c->MaxCombinedImageUniforms;
    c->MaxImageUnits = MAX_IMAGE_UNITS;
    if (c->Program[MESA_SHADER_FRAGMENT].MaxImageUniforms &&
        screen->get_param(screen, PIPE_CAP_IMAGE_STORE_FORMATTED)) {
        extensions->ARB_shader_image_load_store = GL_TRUE;
        extensions->ARB_shader_image_size = GL_TRUE;
    }

    /* ARB_framebuffer_no_attachments */
    c->MaxFramebufferWidth   = c->MaxViewportWidth;
    c->MaxFramebufferHeight  = c->MaxViewportHeight;
    /* NOTE: we cheat here a little by assuming that
     * PIPE_CAP_MAX_TEXTURE_ARRAY_LAYERS has the same
     * number of layers as we need, although we technically
     * could have more the generality is not really useful
     * in practicality.
     */
    c->MaxFramebufferLayers =
            screen->get_param(screen, PIPE_CAP_MAX_TEXTURE_ARRAY_LAYERS);

    c->MaxWindowRectangles =
            screen->get_param(screen, PIPE_CAP_MAX_WINDOW_RECTANGLES);

    c->SparseBufferPageSize =
            screen->get_param(screen, PIPE_CAP_SPARSE_BUFFER_PAGE_SIZE);

    c->AllowMappedBuffersDuringExecution =
            screen->get_param(screen, PIPE_CAP_ALLOW_MAPPED_BUFFERS_DURING_EXECUTION);

    c->BufferCreateMapUnsynchronizedThreadSafe =
            screen->get_param(screen, PIPE_CAP_MAP_UNSYNCHRONIZED_THREAD_SAFE);

    c->UseSTD430AsDefaultPacking =
            screen->get_param(screen, PIPE_CAP_LOAD_CONSTBUF);

    c->MaxSubpixelPrecisionBiasBits =
            screen->get_param(screen, PIPE_CAP_MAX_CONSERVATIVE_RASTER_SUBPIXEL_PRECISION_BIAS);

    c->ConservativeRasterDilateRange[0] =
            screen->get_paramf(screen, PIPE_CAPF_MIN_CONSERVATIVE_RASTER_DILATE);
    c->ConservativeRasterDilateRange[1] =
            screen->get_paramf(screen, PIPE_CAPF_MAX_CONSERVATIVE_RASTER_DILATE);
    c->ConservativeRasterDilateGranularity =
            screen->get_paramf(screen, PIPE_CAPF_CONSERVATIVE_RASTER_DILATE_GRANULARITY);

    /* limit the max combined shader output resources to a driver limit */
    temp = screen->get_param(screen, PIPE_CAP_MAX_COMBINED_SHADER_OUTPUT_RESOURCES);
    if (temp > 0 && c->MaxCombinedShaderOutputResources > temp)
        c->MaxCombinedShaderOutputResources = temp;

    c->VertexBufferOffsetIsInt32 =
            screen->get_param(screen, PIPE_CAP_SIGNED_VERTEX_BUFFER_OFFSET);

    c->AllowDynamicVAOFastPath =
            screen->get_param(screen, PIPE_CAP_ALLOW_DYNAMIC_VAO_FASTPATH);

    c->glBeginEndBufferSize =
            screen->get_param(screen, PIPE_CAP_GL_BEGIN_END_BUFFER_SIZE);

    c->MaxSparseTextureSize =
            screen->get_param(screen, PIPE_CAP_MAX_SPARSE_TEXTURE_SIZE);
    c->MaxSparse3DTextureSize =
            screen->get_param(screen, PIPE_CAP_MAX_SPARSE_3D_TEXTURE_SIZE);
    c->MaxSparseArrayTextureLayers =
            screen->get_param(screen, PIPE_CAP_MAX_SPARSE_ARRAY_TEXTURE_LAYERS);
    c->SparseTextureFullArrayCubeMipmaps =
            screen->get_param(screen, PIPE_CAP_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS);
}

#define o(x) offsetof(struct gl_extensions, x)

struct st_extension_cap_mapping {
    int extension_offset;
    int cap;
};

struct st_extension_format_mapping {
    int extension_offset[2];
    enum pipe_format format[32];

    /* If TRUE, at least one format must be supported for the extensions to be
     * advertised. If FALSE, all the formats must be supported. */
    GLboolean need_at_least_one;
};

static unsigned
get_max_samples_for_formats(struct pipe_screen *screen,
                            unsigned num_formats,
                            const enum pipe_format *formats,
                            unsigned max_samples,
                            unsigned bind)
{
    unsigned i, f;

    for (i = max_samples; i > 0; --i) {
        for (f = 0; f < num_formats; f++) {
            if (screen->is_format_supported(screen, formats[f],
                                            PIPE_TEXTURE_2D, i, i, bind)) {
                return i;
            }
        }
    }
    return 0;
}

void
_mesa_fill_supported_spirv_extensions(struct spirv_supported_extensions *ext,
                                      const struct spirv_supported_capabilities *cap)
{
    memset(ext->supported, 0, sizeof(ext->supported));

    ext->count = 0;

    ext->supported[SPV_KHR_shader_draw_parameters] = cap->draw_parameters;
    ext->supported[SPV_KHR_multiview] = cap->multiview;
    ext->supported[SPV_KHR_storage_buffer_storage_class] = true;
    ext->supported[SPV_KHR_variable_pointers] = cap->variable_pointers;
    ext->supported[SPV_AMD_gcn_shader] = cap->amd_gcn_shader;
    ext->supported[SPV_KHR_shader_ballot] = cap->subgroup_ballot;
    ext->supported[SPV_KHR_subgroup_vote] = cap->subgroup_vote;

    for (unsigned i = 0; i < SPV_EXTENSIONS_COUNT; i++) {
        if (ext->supported[i])
            ext->count++;
    }
}

static void
init_format_extensions(struct pipe_screen *screen,
                       struct gl_extensions *extensions,
                       const struct st_extension_format_mapping *mapping,
                       unsigned num_mappings,
                       enum pipe_texture_target target,
                       unsigned bind_flags)
{
    GLboolean *extension_table = (GLboolean *) extensions;
    unsigned i;
    int j;
    int num_formats = ARRAY_SIZE(mapping->format);
    int num_ext = ARRAY_SIZE(mapping->extension_offset);

    for (i = 0; i < num_mappings; i++) {
        int num_supported = 0;

        /* Examine each format in the list. */
        for (j = 0; j < num_formats && mapping[i].format[j]; j++) {
            if (screen->is_format_supported(screen, mapping[i].format[j],
                                            target, 0, 0, bind_flags)) {
                num_supported++;
            }
        }

        if (!num_supported ||
            (!mapping[i].need_at_least_one && num_supported != j)) {
            continue;
        }

        /* Enable all extensions in the list. */
        for (j = 0; j < num_ext && mapping[i].extension_offset[j]; j++)
            extension_table[mapping[i].extension_offset[j]] = GL_TRUE;
    }
}

static unsigned
get_max_samples_for_formats_advanced(struct pipe_screen *screen,
                                     unsigned num_formats,
                                     const enum pipe_format *formats,
                                     unsigned max_samples,
                                     unsigned num_storage_samples,
                                     unsigned bind)
{
    unsigned i, f;

    for (i = max_samples; i > 0; --i) {
        for (f = 0; f < num_formats; f++) {
            if (screen->is_format_supported(screen, formats[f], PIPE_TEXTURE_2D,
                                            i, num_storage_samples, bind)) {
                return i;
            }
        }
    }
    return 0;
}

void st_init_extensions(struct pipe_screen *screen,
                        struct gl_constants *consts,
                        struct gl_extensions *extensions,
                        struct st_config_options *options,
                        gl_api api)
{
    unsigned i;
    GLboolean *extension_table = (GLboolean *) extensions;

    static const struct st_extension_cap_mapping cap_mapping[] = {
            { o(ARB_base_instance),                PIPE_CAP_START_INSTANCE                   },
            { o(ARB_bindless_texture),             PIPE_CAP_BINDLESS_TEXTURE                 },
            { o(ARB_buffer_storage),               PIPE_CAP_BUFFER_MAP_PERSISTENT_COHERENT   },
            { o(ARB_clear_texture),                PIPE_CAP_CLEAR_TEXTURE                    },
            { o(ARB_clip_control),                 PIPE_CAP_CLIP_HALFZ                       },
            { o(ARB_color_buffer_float),           PIPE_CAP_VERTEX_COLOR_UNCLAMPED           },
            { o(ARB_conditional_render_inverted),  PIPE_CAP_CONDITIONAL_RENDER_INVERTED      },
            { o(ARB_copy_image),                   PIPE_CAP_COPY_BETWEEN_COMPRESSED_AND_PLAIN_FORMATS },
            { o(OES_copy_image),                   PIPE_CAP_COPY_BETWEEN_COMPRESSED_AND_PLAIN_FORMATS },
            { o(ARB_cull_distance),                PIPE_CAP_CULL_DISTANCE                    },
            { o(ARB_depth_clamp),                  PIPE_CAP_DEPTH_CLIP_DISABLE               },
            { o(ARB_derivative_control),           PIPE_CAP_TGSI_FS_FINE_DERIVATIVE          },
            { o(ARB_draw_buffers_blend),           PIPE_CAP_INDEP_BLEND_FUNC                 },
            { o(ARB_draw_indirect),                PIPE_CAP_DRAW_INDIRECT                    },
            { o(ARB_draw_instanced),               PIPE_CAP_TGSI_INSTANCEID                  },
            { o(ARB_fragment_program_shadow),      PIPE_CAP_TEXTURE_SHADOW_MAP               },
            { o(ARB_framebuffer_object),           PIPE_CAP_MIXED_FRAMEBUFFER_SIZES          },
            { o(ARB_gpu_shader_int64),             PIPE_CAP_INT64                            },
            { o(ARB_gl_spirv),                     PIPE_CAP_GL_SPIRV                         },
            { o(ARB_indirect_parameters),          PIPE_CAP_MULTI_DRAW_INDIRECT_PARAMS       },
            { o(ARB_instanced_arrays),             PIPE_CAP_VERTEX_ELEMENT_INSTANCE_DIVISOR  },
            { o(ARB_occlusion_query2),             PIPE_CAP_OCCLUSION_QUERY                  },
            { o(ARB_pipeline_statistics_query),    PIPE_CAP_QUERY_PIPELINE_STATISTICS        },
            { o(ARB_pipeline_statistics_query),    PIPE_CAP_QUERY_PIPELINE_STATISTICS_SINGLE },
            { o(ARB_point_sprite),                 PIPE_CAP_POINT_SPRITE                     },
            { o(ARB_polygon_offset_clamp),         PIPE_CAP_POLYGON_OFFSET_CLAMP             },
            { o(ARB_post_depth_coverage),          PIPE_CAP_POST_DEPTH_COVERAGE              },
            { o(ARB_query_buffer_object),          PIPE_CAP_QUERY_BUFFER_OBJECT              },
            { o(ARB_robust_buffer_access_behavior), PIPE_CAP_ROBUST_BUFFER_ACCESS_BEHAVIOR   },
            { o(ARB_sample_shading),               PIPE_CAP_SAMPLE_SHADING                   },
            { o(ARB_sample_locations),             PIPE_CAP_PROGRAMMABLE_SAMPLE_LOCATIONS    },
            { o(ARB_seamless_cube_map),            PIPE_CAP_SEAMLESS_CUBE_MAP                },
            { o(ARB_shader_ballot),                PIPE_CAP_TGSI_BALLOT                      },
            { o(ARB_shader_clock),                 PIPE_CAP_TGSI_CLOCK                       },
            { o(ARB_shader_draw_parameters),       PIPE_CAP_DRAW_PARAMETERS                  },
            { o(ARB_shader_group_vote),            PIPE_CAP_TGSI_VOTE                        },
            { o(EXT_shader_image_load_formatted),  PIPE_CAP_IMAGE_LOAD_FORMATTED             },
            { o(EXT_shader_image_load_store),      PIPE_CAP_TGSI_ATOMINC_WRAP                },
            { o(ARB_shader_stencil_export),        PIPE_CAP_SHADER_STENCIL_EXPORT            },
            { o(ARB_shader_texture_image_samples), PIPE_CAP_TGSI_TXQS                        },
            { o(ARB_shader_texture_lod),           PIPE_CAP_FRAGMENT_SHADER_TEXTURE_LOD      },
            { o(ARB_shadow),                       PIPE_CAP_TEXTURE_SHADOW_MAP               },
            { o(ARB_sparse_buffer),                PIPE_CAP_SPARSE_BUFFER_PAGE_SIZE          },
            { o(ARB_sparse_texture),               PIPE_CAP_MAX_SPARSE_TEXTURE_SIZE          },
            { o(ARB_sparse_texture2),              PIPE_CAP_QUERY_SPARSE_TEXTURE_RESIDENCY   },
            { o(ARB_spirv_extensions),             PIPE_CAP_GL_SPIRV                         },
            { o(ARB_texture_buffer_object),        PIPE_CAP_TEXTURE_BUFFER_OBJECTS           },
            { o(ARB_texture_cube_map_array),       PIPE_CAP_CUBE_MAP_ARRAY                   },
            { o(ARB_texture_filter_minmax),        PIPE_CAP_SAMPLER_REDUCTION_MINMAX_ARB     },
            { o(ARB_texture_gather),               PIPE_CAP_MAX_TEXTURE_GATHER_COMPONENTS    },
            { o(ARB_texture_mirror_clamp_to_edge), PIPE_CAP_TEXTURE_MIRROR_CLAMP_TO_EDGE     },
            { o(ARB_texture_multisample),          PIPE_CAP_TEXTURE_MULTISAMPLE              },
            { o(ARB_texture_non_power_of_two),     PIPE_CAP_NPOT_TEXTURES                    },
            { o(ARB_texture_query_lod),            PIPE_CAP_TEXTURE_QUERY_LOD                },
            { o(ARB_texture_view),                 PIPE_CAP_SAMPLER_VIEW_TARGET              },
            { o(ARB_timer_query),                  PIPE_CAP_QUERY_TIMESTAMP                  },
            { o(ARB_transform_feedback2),          PIPE_CAP_STREAM_OUTPUT_PAUSE_RESUME       },
            { o(ARB_transform_feedback3),          PIPE_CAP_STREAM_OUTPUT_INTERLEAVE_BUFFERS },
            { o(ARB_transform_feedback_overflow_query), PIPE_CAP_QUERY_SO_OVERFLOW           },
            { o(ARB_fragment_shader_interlock),    PIPE_CAP_FRAGMENT_SHADER_INTERLOCK        },

            { o(EXT_blend_equation_separate),      PIPE_CAP_BLEND_EQUATION_SEPARATE          },
            { o(EXT_demote_to_helper_invocation),  PIPE_CAP_DEMOTE_TO_HELPER_INVOCATION      },
            { o(EXT_depth_bounds_test),            PIPE_CAP_DEPTH_BOUNDS_TEST                },
            { o(EXT_disjoint_timer_query),         PIPE_CAP_QUERY_TIMESTAMP                  },
            { o(EXT_draw_buffers2),                PIPE_CAP_INDEP_BLEND_ENABLE               },
            { o(EXT_memory_object),                PIPE_CAP_MEMOBJ                           },
#ifndef _WIN32
            { o(EXT_memory_object_fd),             PIPE_CAP_MEMOBJ                           },
#endif
            { o(EXT_multisampled_render_to_texture), PIPE_CAP_SURFACE_SAMPLE_COUNT           },
            { o(EXT_semaphore),                    PIPE_CAP_FENCE_SIGNAL                     },
            { o(EXT_semaphore_fd),                 PIPE_CAP_FENCE_SIGNAL                     },
            { o(EXT_shader_samples_identical),     PIPE_CAP_SHADER_SAMPLES_IDENTICAL         },
            { o(EXT_texture_array),                PIPE_CAP_MAX_TEXTURE_ARRAY_LAYERS         },
            { o(EXT_texture_filter_anisotropic),   PIPE_CAP_ANISOTROPIC_FILTER               },
            { o(EXT_texture_filter_minmax),        PIPE_CAP_SAMPLER_REDUCTION_MINMAX         },
            { o(EXT_texture_mirror_clamp),         PIPE_CAP_TEXTURE_MIRROR_CLAMP             },
            { o(EXT_texture_shadow_lod),           PIPE_CAP_TEXTURE_SHADOW_LOD               },
            { o(EXT_texture_swizzle),              PIPE_CAP_TEXTURE_SWIZZLE                  },
            { o(EXT_transform_feedback),           PIPE_CAP_MAX_STREAM_OUTPUT_BUFFERS        },
            { o(EXT_window_rectangles),            PIPE_CAP_MAX_WINDOW_RECTANGLES            },

            { o(AMD_depth_clamp_separate),         PIPE_CAP_DEPTH_CLIP_DISABLE_SEPARATE      },
            { o(AMD_framebuffer_multisample_advanced), PIPE_CAP_FRAMEBUFFER_MSAA_CONSTRAINTS },
            { o(AMD_pinned_memory),                PIPE_CAP_RESOURCE_FROM_USER_MEMORY        },
            { o(ATI_meminfo),                      PIPE_CAP_QUERY_MEMORY_INFO                },
            { o(AMD_seamless_cubemap_per_texture), PIPE_CAP_SEAMLESS_CUBE_MAP_PER_TEXTURE    },
            { o(ATI_texture_mirror_once),          PIPE_CAP_TEXTURE_MIRROR_CLAMP             },
            { o(INTEL_conservative_rasterization), PIPE_CAP_CONSERVATIVE_RASTER_INNER_COVERAGE },
            { o(INTEL_shader_atomic_float_minmax), PIPE_CAP_ATOMIC_FLOAT_MINMAX              },
            { o(MESA_tile_raster_order),           PIPE_CAP_TILE_RASTER_ORDER                },
            { o(NV_alpha_to_coverage_dither_control), PIPE_CAP_ALPHA_TO_COVERAGE_DITHER_CONTROL },
            { o(NV_compute_shader_derivatives),    PIPE_CAP_COMPUTE_SHADER_DERIVATIVES       },
            { o(NV_conditional_render),            PIPE_CAP_CONDITIONAL_RENDER               },
            { o(NV_fill_rectangle),                PIPE_CAP_POLYGON_MODE_FILL_RECTANGLE      },
            { o(NV_primitive_restart),             PIPE_CAP_PRIMITIVE_RESTART                },
            { o(NV_shader_atomic_float),           PIPE_CAP_TGSI_ATOMFADD                    },
            { o(NV_shader_atomic_int64),           PIPE_CAP_SHADER_ATOMIC_INT64              },
            { o(NV_texture_barrier),               PIPE_CAP_TEXTURE_BARRIER                  },
            { o(NV_viewport_array2),               PIPE_CAP_VIEWPORT_MASK                    },
            { o(NV_viewport_swizzle),              PIPE_CAP_VIEWPORT_SWIZZLE                 },
            { o(NVX_gpu_memory_info),              PIPE_CAP_QUERY_MEMORY_INFO                },

            { o(OES_standard_derivatives),         PIPE_CAP_FRAGMENT_SHADER_DERIVATIVES      },
            { o(OES_texture_float_linear),         PIPE_CAP_TEXTURE_FLOAT_LINEAR             },
            { o(OES_texture_half_float_linear),    PIPE_CAP_TEXTURE_HALF_FLOAT_LINEAR        },
            { o(OES_texture_view),                 PIPE_CAP_SAMPLER_VIEW_TARGET              },
            { o(INTEL_blackhole_render),           PIPE_CAP_FRONTEND_NOOP,                   },
    };

    /* Required: render target and sampler support */
    static const struct st_extension_format_mapping rendertarget_mapping[] = {
            { { o(OES_texture_float) },
                    { PIPE_FORMAT_R32G32B32A32_FLOAT } },

            { { o(OES_texture_half_float) },
                    { PIPE_FORMAT_R16G16B16A16_FLOAT } },

            { { o(ARB_texture_rgb10_a2ui) },
                    { PIPE_FORMAT_R10G10B10A2_UINT,
                            PIPE_FORMAT_B10G10R10A2_UINT },
                    GL_TRUE }, /* at least one format must be supported */

            { { o(EXT_sRGB) },
                    { PIPE_FORMAT_A8B8G8R8_SRGB,
                            PIPE_FORMAT_B8G8R8A8_SRGB,
                            PIPE_FORMAT_R8G8B8A8_SRGB },
                    GL_TRUE }, /* at least one format must be supported */

            { { o(EXT_packed_float) },
                    { PIPE_FORMAT_R11G11B10_FLOAT } },

            { { o(EXT_texture_integer) },
                    { PIPE_FORMAT_R32G32B32A32_UINT,
                            PIPE_FORMAT_R32G32B32A32_SINT } },

            { { o(ARB_texture_rg) },
                    { PIPE_FORMAT_R8_UNORM,
                            PIPE_FORMAT_R8G8_UNORM } },

            { { o(EXT_texture_norm16) },
                    { PIPE_FORMAT_R16_UNORM,
                            PIPE_FORMAT_R16G16_UNORM,
                            PIPE_FORMAT_R16G16B16A16_UNORM } },

            { { o(EXT_render_snorm) },
                    { PIPE_FORMAT_R8_SNORM,
                            PIPE_FORMAT_R8G8_SNORM,
                            PIPE_FORMAT_R8G8B8A8_SNORM,
                            PIPE_FORMAT_R16_SNORM,
                            PIPE_FORMAT_R16G16_SNORM,
                            PIPE_FORMAT_R16G16B16A16_SNORM } },

            { { o(EXT_color_buffer_half_float) },
                    { PIPE_FORMAT_R16_FLOAT,
                            PIPE_FORMAT_R16G16_FLOAT,
                            PIPE_FORMAT_R16G16B16X16_FLOAT,
                            PIPE_FORMAT_R16G16B16A16_FLOAT } },
    };

    /* Required: render target, sampler, and blending */
    static const struct st_extension_format_mapping rt_blendable[] = {
            { { o(EXT_float_blend) },
                    { PIPE_FORMAT_R32G32B32A32_FLOAT } },
    };

    /* Required: depth stencil and sampler support */
    static const struct st_extension_format_mapping depthstencil_mapping[] = {
            { { o(ARB_depth_buffer_float) },
                    { PIPE_FORMAT_Z32_FLOAT,
                            PIPE_FORMAT_Z32_FLOAT_S8X24_UINT } },
    };

    /* Required: sampler support */
    static const struct st_extension_format_mapping texture_mapping[] = {
            { { o(ARB_texture_compression_rgtc) },
                    { PIPE_FORMAT_RGTC1_UNORM,
                            PIPE_FORMAT_RGTC1_SNORM,
                            PIPE_FORMAT_RGTC2_UNORM,
                            PIPE_FORMAT_RGTC2_SNORM } },

            { { o(EXT_texture_compression_latc) },
                    { PIPE_FORMAT_LATC1_UNORM,
                            PIPE_FORMAT_LATC1_SNORM,
                            PIPE_FORMAT_LATC2_UNORM,
                            PIPE_FORMAT_LATC2_SNORM } },

            { { o(EXT_texture_compression_s3tc),
                      o(ANGLE_texture_compression_dxt) },
                    { PIPE_FORMAT_DXT1_RGB,
                            PIPE_FORMAT_DXT1_RGBA,
                            PIPE_FORMAT_DXT3_RGBA,
                            PIPE_FORMAT_DXT5_RGBA } },

            { { o(EXT_texture_compression_s3tc_srgb) },
                    { PIPE_FORMAT_DXT1_SRGB,
                            PIPE_FORMAT_DXT1_SRGBA,
                            PIPE_FORMAT_DXT3_SRGBA,
                            PIPE_FORMAT_DXT5_SRGBA } },

            { { o(ARB_texture_compression_bptc) },
                    { PIPE_FORMAT_BPTC_RGBA_UNORM,
                            PIPE_FORMAT_BPTC_SRGBA,
                            PIPE_FORMAT_BPTC_RGB_FLOAT,
                            PIPE_FORMAT_BPTC_RGB_UFLOAT } },

            { { o(TDFX_texture_compression_FXT1) },
                    { PIPE_FORMAT_FXT1_RGB,
                            PIPE_FORMAT_FXT1_RGBA } },

            { { o(KHR_texture_compression_astc_ldr),
                      o(KHR_texture_compression_astc_sliced_3d) },
                    { PIPE_FORMAT_ASTC_4x4,
                            PIPE_FORMAT_ASTC_5x4,
                            PIPE_FORMAT_ASTC_5x5,
                            PIPE_FORMAT_ASTC_6x5,
                            PIPE_FORMAT_ASTC_6x6,
                            PIPE_FORMAT_ASTC_8x5,
                            PIPE_FORMAT_ASTC_8x6,
                            PIPE_FORMAT_ASTC_8x8,
                            PIPE_FORMAT_ASTC_10x5,
                            PIPE_FORMAT_ASTC_10x6,
                            PIPE_FORMAT_ASTC_10x8,
                            PIPE_FORMAT_ASTC_10x10,
                            PIPE_FORMAT_ASTC_12x10,
                            PIPE_FORMAT_ASTC_12x12,
                            PIPE_FORMAT_ASTC_4x4_SRGB,
                            PIPE_FORMAT_ASTC_5x4_SRGB,
                            PIPE_FORMAT_ASTC_5x5_SRGB,
                            PIPE_FORMAT_ASTC_6x5_SRGB,
                            PIPE_FORMAT_ASTC_6x6_SRGB,
                            PIPE_FORMAT_ASTC_8x5_SRGB,
                            PIPE_FORMAT_ASTC_8x6_SRGB,
                            PIPE_FORMAT_ASTC_8x8_SRGB,
                            PIPE_FORMAT_ASTC_10x5_SRGB,
                            PIPE_FORMAT_ASTC_10x6_SRGB,
                            PIPE_FORMAT_ASTC_10x8_SRGB,
                            PIPE_FORMAT_ASTC_10x10_SRGB,
                            PIPE_FORMAT_ASTC_12x10_SRGB,
                            PIPE_FORMAT_ASTC_12x12_SRGB } },

            /* ASTC software fallback support. */
            { { o(KHR_texture_compression_astc_ldr),
                      o(KHR_texture_compression_astc_sliced_3d) },
                    { PIPE_FORMAT_R8G8B8A8_UNORM,
                            PIPE_FORMAT_R8G8B8A8_SRGB } },

            { { o(EXT_texture_shared_exponent) },
                    { PIPE_FORMAT_R9G9B9E5_FLOAT } },

            { { o(EXT_texture_snorm) },
                    { PIPE_FORMAT_R8G8B8A8_SNORM } },

            { { o(EXT_texture_sRGB),
                      o(EXT_texture_sRGB_decode) },
                    { PIPE_FORMAT_A8B8G8R8_SRGB,
                            PIPE_FORMAT_B8G8R8A8_SRGB,
                            PIPE_FORMAT_A8R8G8B8_SRGB,
                            PIPE_FORMAT_R8G8B8A8_SRGB},
                    GL_TRUE }, /* at least one format must be supported */

            { { o(EXT_texture_sRGB_R8) },
                    { PIPE_FORMAT_R8_SRGB }, },

            { { o(EXT_texture_sRGB_RG8) },
                    { PIPE_FORMAT_R8G8_SRGB }, },

            { { o(EXT_texture_type_2_10_10_10_REV) },
                    { PIPE_FORMAT_R10G10B10A2_UNORM,
                            PIPE_FORMAT_B10G10R10A2_UNORM },
                    GL_TRUE }, /* at least one format must be supported */

            { { o(ATI_texture_compression_3dc) },
                    { PIPE_FORMAT_LATC2_UNORM } },

            { { o(MESA_ycbcr_texture) },
                    { PIPE_FORMAT_UYVY,
                            PIPE_FORMAT_YUYV },
                    GL_TRUE }, /* at least one format must be supported */

            { { o(OES_compressed_ETC1_RGB8_texture) },
                    { PIPE_FORMAT_ETC1_RGB8,
                            PIPE_FORMAT_R8G8B8A8_UNORM },
                    GL_TRUE }, /* at least one format must be supported */

            { { o(ARB_stencil_texturing),
                      o(ARB_texture_stencil8) },
                    { PIPE_FORMAT_X24S8_UINT,
                            PIPE_FORMAT_S8X24_UINT },
                    GL_TRUE }, /* at least one format must be supported */

            { { o(AMD_compressed_ATC_texture) },
                    { PIPE_FORMAT_ATC_RGB,
                            PIPE_FORMAT_ATC_RGBA_EXPLICIT,
                            PIPE_FORMAT_ATC_RGBA_INTERPOLATED } },
    };

    /* Required: vertex fetch support. */
    static const struct st_extension_format_mapping vertex_mapping[] = {
            { { o(EXT_vertex_array_bgra) },
                    { PIPE_FORMAT_B8G8R8A8_UNORM } },
            { { o(ARB_vertex_type_2_10_10_10_rev) },
                    { PIPE_FORMAT_R10G10B10A2_UNORM,
                            PIPE_FORMAT_B10G10R10A2_UNORM,
                            PIPE_FORMAT_R10G10B10A2_SNORM,
                            PIPE_FORMAT_B10G10R10A2_SNORM,
                            PIPE_FORMAT_R10G10B10A2_USCALED,
                            PIPE_FORMAT_B10G10R10A2_USCALED,
                            PIPE_FORMAT_R10G10B10A2_SSCALED,
                            PIPE_FORMAT_B10G10R10A2_SSCALED } },
            { { o(ARB_vertex_type_10f_11f_11f_rev) },
                    { PIPE_FORMAT_R11G11B10_FLOAT } },
    };

    static const struct st_extension_format_mapping tbo_rgb32[] = {
            { {o(ARB_texture_buffer_object_rgb32) },
                    { PIPE_FORMAT_R32G32B32_FLOAT,
                            PIPE_FORMAT_R32G32B32_UINT,
                            PIPE_FORMAT_R32G32B32_SINT,
                    } },
    };

    /* Expose the extensions which directly correspond to gallium caps. */
    for (i = 0; i < ARRAY_SIZE(cap_mapping); i++) {
        if (screen->get_param(screen, cap_mapping[i].cap)) {
            extension_table[cap_mapping[i].extension_offset] = GL_TRUE;
        }
    }

    /* EXT implies ARB here */
    if (extensions->EXT_texture_filter_minmax)
        extensions->ARB_texture_filter_minmax = GL_TRUE;

    /* Expose the extensions which directly correspond to gallium formats. */
    init_format_extensions(screen, extensions, rendertarget_mapping,
                           ARRAY_SIZE(rendertarget_mapping), PIPE_TEXTURE_2D,
                           PIPE_BIND_RENDER_TARGET | PIPE_BIND_SAMPLER_VIEW);
    init_format_extensions(screen, extensions, rt_blendable,
                           ARRAY_SIZE(rt_blendable), PIPE_TEXTURE_2D,
                           PIPE_BIND_RENDER_TARGET | PIPE_BIND_SAMPLER_VIEW |
                           PIPE_BIND_BLENDABLE);
    init_format_extensions(screen, extensions, depthstencil_mapping,
                           ARRAY_SIZE(depthstencil_mapping), PIPE_TEXTURE_2D,
                           PIPE_BIND_DEPTH_STENCIL | PIPE_BIND_SAMPLER_VIEW);
    init_format_extensions(screen, extensions, texture_mapping,
                           ARRAY_SIZE(texture_mapping), PIPE_TEXTURE_2D,
                           PIPE_BIND_SAMPLER_VIEW);
    init_format_extensions(screen, extensions, vertex_mapping,
                           ARRAY_SIZE(vertex_mapping), PIPE_BUFFER,
                           PIPE_BIND_VERTEX_BUFFER);

    /* Figure out GLSL support and set GLSLVersion to it. */
    consts->GLSLVersion = screen->get_param(screen, PIPE_CAP_GLSL_FEATURE_LEVEL);
    consts->GLSLVersionCompat =
            screen->get_param(screen, PIPE_CAP_GLSL_FEATURE_LEVEL_COMPATIBILITY);

    const unsigned ESSLVersion =
            screen->get_param(screen, PIPE_CAP_ESSL_FEATURE_LEVEL);
    const unsigned GLSLVersion =
            api == API_OPENGL_COMPAT ? consts->GLSLVersionCompat :
            consts->GLSLVersion;

    _mesa_override_glsl_version(consts);

    if (options->force_glsl_version > 0 &&
        options->force_glsl_version <= GLSLVersion) {
        consts->ForceGLSLVersion = options->force_glsl_version;
    }

    consts->ForceCompatShaders = options->force_compat_shaders;

    consts->AllowExtraPPTokens = options->allow_extra_pp_tokens;

    consts->AllowHigherCompatVersion = options->allow_higher_compat_version;
    consts->AllowGLSLCompatShaders = options->allow_glsl_compat_shaders;

    consts->ForceGLSLAbsSqrt = options->force_glsl_abs_sqrt;

    consts->AllowGLSLBuiltinVariableRedeclaration = options->allow_glsl_builtin_variable_redeclaration;

    consts->dri_config_options_sha1 = options->config_options_sha1;

    consts->AllowGLSLCrossStageInterpolationMismatch = options->allow_glsl_cross_stage_interpolation_mismatch;

    consts->DoDCEBeforeClipCullAnalysis = options->do_dce_before_clip_cull_analysis;

    consts->GLSLIgnoreWriteToReadonlyVar = options->glsl_ignore_write_to_readonly_var;

    consts->PrimitiveRestartFixedIndex =
            screen->get_param(screen, PIPE_CAP_PRIMITIVE_RESTART_FIXED_INDEX);

    /* Technically we are turning on the EXT_gpu_shader5 extension,
     * ARB_gpu_shader5 does not exist in GLES, but this flag is what
     * switches on EXT_gpu_shader5:
     */
    if (api == API_OPENGLES2 && ESSLVersion >= 320)
        extensions->ARB_gpu_shader5 = GL_TRUE;

    if (GLSLVersion >= 400 && !options->disable_arb_gpu_shader5)
        extensions->ARB_gpu_shader5 = GL_TRUE;
    if (GLSLVersion >= 410)
        extensions->ARB_shader_precision = GL_TRUE;

    /* This extension needs full OpenGL 3.2, but we don't know if that's
     * supported at this point. Only check the GLSL version. */
    if (GLSLVersion >= 150 &&
        screen->get_param(screen, PIPE_CAP_TGSI_VS_LAYER_VIEWPORT)) {
        extensions->AMD_vertex_shader_layer = GL_TRUE;
    }

    if (GLSLVersion >= 140) {
        /* Since GLSL 1.40 has support for all of the features of gpu_shader4,
         * we can always expose it if the driver can do 140. Supporting
         * gpu_shader4 on drivers without GLSL 1.40 is left for a future
         * pipe cap.
         */
        extensions->EXT_gpu_shader4 = GL_TRUE;
        extensions->EXT_texture_buffer_object = GL_TRUE;

        if (screen->get_param(screen, PIPE_CAP_TGSI_ARRAY_COMPONENTS))
            extensions->ARB_enhanced_layouts = GL_TRUE;
    }

    if (GLSLVersion >= 130) {
        consts->NativeIntegers = GL_TRUE;
        consts->MaxClipPlanes = 8;

        uint32_t drv_clip_planes = screen->get_param(screen, PIPE_CAP_CLIP_PLANES);
        /* only override for > 1 - 0 if none, 1 is MAX, >2 overrides MAX */
        if (drv_clip_planes > 1)
            consts->MaxClipPlanes = drv_clip_planes;

        if (screen->get_param(screen, PIPE_CAP_VERTEXID_NOBASE)) {
            consts->VertexID_is_zero_based = GL_TRUE;
        }

        /* Extensions that either depend on GLSL 1.30 or are a subset thereof. */
        extensions->ARB_conservative_depth = GL_TRUE;
        extensions->ARB_shading_language_packing = GL_TRUE;
        extensions->OES_depth_texture_cube_map = GL_TRUE;
        extensions->ARB_shading_language_420pack = GL_TRUE;
        extensions->ARB_texture_query_levels = GL_TRUE;

        extensions->ARB_shader_bit_encoding = GL_TRUE;

        extensions->EXT_shader_integer_mix = GL_TRUE;
        extensions->ARB_arrays_of_arrays = GL_TRUE;
        extensions->MESA_shader_integer_functions = GL_TRUE;

        if (screen->get_param(screen, PIPE_CAP_OPENCL_INTEGER_FUNCTIONS) &&
            screen->get_param(screen, PIPE_CAP_INTEGER_MULTIPLY_32X16)) {
            extensions->INTEL_shader_integer_functions2 = GL_TRUE;
        }
    } else {
        /* Optional integer support for GLSL 1.2. */
        if (screen->get_shader_param(screen, PIPE_SHADER_VERTEX,
                                     PIPE_SHADER_CAP_INTEGERS) &&
            screen->get_shader_param(screen, PIPE_SHADER_FRAGMENT,
                                     PIPE_SHADER_CAP_INTEGERS)) {
            consts->NativeIntegers = GL_TRUE;

            extensions->EXT_shader_integer_mix = GL_TRUE;
        }

        /* Integer textures make no sense before GLSL 1.30 */
        extensions->EXT_texture_integer = GL_FALSE;
        extensions->ARB_texture_rgb10_a2ui = GL_FALSE;
    }

    if (options->glsl_zero_init) {
        consts->GLSLZeroInit = 1;
    } else {
        consts->GLSLZeroInit = screen->get_param(screen, PIPE_CAP_GLSL_ZERO_INIT);
    }

    consts->ForceGLNamesReuse = options->force_gl_names_reuse;

    consts->ForceIntegerTexNearest = options->force_integer_tex_nearest;

    consts->VendorOverride = options->force_gl_vendor;
    consts->RendererOverride = options->force_gl_renderer;

    consts->UniformBooleanTrue = consts->NativeIntegers ? ~0U : fui(1.0f);

    /* Below are the cases which cannot be moved into tables easily. */

    /* The compatibility profile also requires GLSLVersionCompat >= 400. */
    if (screen->get_shader_param(screen, PIPE_SHADER_TESS_CTRL,
                                 PIPE_SHADER_CAP_MAX_INSTRUCTIONS) > 0 &&
        (api != API_OPENGL_COMPAT || consts->GLSLVersionCompat >= 400)) {
        extensions->ARB_tessellation_shader = GL_TRUE;
    }

    /* OES_geometry_shader requires instancing */
    if ((GLSLVersion >= 400 || ESSLVersion >= 310) &&
        screen->get_shader_param(screen, PIPE_SHADER_GEOMETRY,
                                 PIPE_SHADER_CAP_MAX_INSTRUCTIONS) > 0 &&
        consts->MaxGeometryShaderInvocations >= 32) {
        extensions->OES_geometry_shader = GL_TRUE;
    }

    /* Some hardware may not support indirect draws, but still wants ES
     * 3.1. This allows the extension to be enabled only in ES contexts to
     * avoid claiming hw support when there is none, and using a software
     * fallback for ES.
     */
    if (api == API_OPENGLES2 && ESSLVersion >= 310) {
        extensions->ARB_draw_indirect = GL_TRUE;
    }

    /* Needs PIPE_CAP_SAMPLE_SHADING + all the sample-related bits of
     * ARB_gpu_shader5. This enables all the per-sample shading ES extensions.
     */
    extensions->OES_sample_variables = extensions->ARB_sample_shading &&
                                       extensions->ARB_gpu_shader5;

    /* Maximum sample count. */
    {
        static const enum pipe_format color_formats[] = {
                PIPE_FORMAT_R8G8B8A8_UNORM,
                PIPE_FORMAT_B8G8R8A8_UNORM,
                PIPE_FORMAT_A8R8G8B8_UNORM,
                PIPE_FORMAT_A8B8G8R8_UNORM,
        };
        static const enum pipe_format depth_formats[] = {
                PIPE_FORMAT_Z16_UNORM,
                PIPE_FORMAT_Z24X8_UNORM,
                PIPE_FORMAT_X8Z24_UNORM,
                PIPE_FORMAT_Z32_UNORM,
                PIPE_FORMAT_Z32_FLOAT
        };
        static const enum pipe_format int_formats[] = {
                PIPE_FORMAT_R8G8B8A8_SINT
        };
        static const enum pipe_format void_formats[] = {
                PIPE_FORMAT_NONE
        };

        consts->MaxSamples =
                get_max_samples_for_formats(screen, ARRAY_SIZE(color_formats),
                                            color_formats, 16,
                                            PIPE_BIND_RENDER_TARGET);

        consts->MaxImageSamples =
                get_max_samples_for_formats(screen, ARRAY_SIZE(color_formats),
                                            color_formats, 16,
                                            PIPE_BIND_SHADER_IMAGE);

        consts->MaxColorTextureSamples =
                get_max_samples_for_formats(screen, ARRAY_SIZE(color_formats),
                                            color_formats, consts->MaxSamples,
                                            PIPE_BIND_SAMPLER_VIEW);

        consts->MaxDepthTextureSamples =
                get_max_samples_for_formats(screen, ARRAY_SIZE(depth_formats),
                                            depth_formats, consts->MaxSamples,
                                            PIPE_BIND_SAMPLER_VIEW);

        consts->MaxIntegerSamples =
                get_max_samples_for_formats(screen, ARRAY_SIZE(int_formats),
                                            int_formats, consts->MaxSamples,
                                            PIPE_BIND_SAMPLER_VIEW);

        /* ARB_framebuffer_no_attachments, assume max no. of samples 32 */
        consts->MaxFramebufferSamples =
                get_max_samples_for_formats(screen, ARRAY_SIZE(void_formats),
                                            void_formats, 32,
                                            PIPE_BIND_RENDER_TARGET);

        if (extensions->AMD_framebuffer_multisample_advanced) {
            /* AMD_framebuffer_multisample_advanced */
            /* This can be greater than storage samples. */
            consts->MaxColorFramebufferSamples =
                    get_max_samples_for_formats_advanced(screen,
                                                         ARRAY_SIZE(color_formats),
                                                         color_formats, 16,
                                                         consts->MaxSamples,
                                                         PIPE_BIND_RENDER_TARGET);

            /* If the driver supports N color samples, it means it supports
             * N samples and N storage samples. N samples >= N storage
             * samples.
             */
            consts->MaxColorFramebufferStorageSamples = consts->MaxSamples;
            consts->MaxDepthStencilFramebufferSamples =
                    consts->MaxDepthTextureSamples;

            assert(consts->MaxColorFramebufferSamples >=
                   consts->MaxDepthStencilFramebufferSamples);
            assert(consts->MaxDepthStencilFramebufferSamples >=
                   consts->MaxColorFramebufferStorageSamples);

            consts->NumSupportedMultisampleModes = 0;

            unsigned depth_samples_supported = 0;

            for (unsigned samples = 2;
                 samples <= consts->MaxDepthStencilFramebufferSamples;
                 samples++) {
                if (screen->is_format_supported(screen, PIPE_FORMAT_Z32_FLOAT,
                                                PIPE_TEXTURE_2D, samples, samples,
                                                PIPE_BIND_DEPTH_STENCIL))
                    depth_samples_supported |= 1 << samples;
            }

            for (unsigned samples = 2;
                 samples <= consts->MaxColorFramebufferSamples;
                 samples++) {
                for (unsigned depth_samples = 2;
                     depth_samples <= samples; depth_samples++) {
                    if (!(depth_samples_supported & (1 << depth_samples)))
                        continue;

                    for (unsigned storage_samples = 2;
                         storage_samples <= depth_samples; storage_samples++) {
                        if (screen->is_format_supported(screen,
                                                        PIPE_FORMAT_R8G8B8A8_UNORM,
                                                        PIPE_TEXTURE_2D,
                                                        samples,
                                                        storage_samples,
                                                        PIPE_BIND_RENDER_TARGET)) {
                            unsigned i = consts->NumSupportedMultisampleModes;

                            assert(i < ARRAY_SIZE(consts->SupportedMultisampleModes));
                            consts->SupportedMultisampleModes[i].NumColorSamples =
                                    samples;
                            consts->SupportedMultisampleModes[i].NumColorStorageSamples =
                                    storage_samples;
                            consts->SupportedMultisampleModes[i].NumDepthStencilSamples =
                                    depth_samples;
                            consts->NumSupportedMultisampleModes++;
                        }
                    }
                }
            }
        }
    }

    if (consts->MaxSamples >= 2) {
        /* Real MSAA support */
        extensions->EXT_framebuffer_multisample = GL_TRUE;
        extensions->EXT_framebuffer_multisample_blit_scaled = GL_TRUE;
    }
    else if (consts->MaxSamples > 0 &&
             screen->get_param(screen, PIPE_CAP_FAKE_SW_MSAA)) {
        /* fake MSAA support */
        consts->FakeSWMSAA = GL_TRUE;
        extensions->EXT_framebuffer_multisample = GL_TRUE;
        extensions->EXT_framebuffer_multisample_blit_scaled = GL_TRUE;
        extensions->ARB_texture_multisample = GL_TRUE;
    }

    if (consts->MaxDualSourceDrawBuffers > 0 &&
        !options->disable_blend_func_extended)
        extensions->ARB_blend_func_extended = GL_TRUE;

    if (screen->get_param(screen, PIPE_CAP_QUERY_TIME_ELAPSED) ||
        extensions->ARB_timer_query) {
        extensions->EXT_timer_query = GL_TRUE;
    }

    if (extensions->ARB_transform_feedback2 &&
        extensions->ARB_draw_instanced) {
        extensions->ARB_transform_feedback_instanced = GL_TRUE;
    }
    if (options->force_glsl_extensions_warn)
        consts->ForceGLSLExtensionsWarn = 1;

    if (options->disable_glsl_line_continuations)
        consts->DisableGLSLLineContinuations = 1;

    if (options->allow_glsl_extension_directive_midshader)
        consts->AllowGLSLExtensionDirectiveMidShader = GL_TRUE;

    if (options->allow_glsl_120_subset_in_110)
        consts->AllowGLSL120SubsetIn110 = GL_TRUE;

    if (options->allow_glsl_builtin_const_expression)
        consts->AllowGLSLBuiltinConstantExpression = GL_TRUE;

    if (options->allow_glsl_relaxed_es)
        consts->AllowGLSLRelaxedES = GL_TRUE;

    consts->MinMapBufferAlignment =
            screen->get_param(screen, PIPE_CAP_MIN_MAP_BUFFER_ALIGNMENT);

    /* The OpenGL Compatibility profile requires arbitrary buffer swizzling. */
    if (api == API_OPENGL_COMPAT &&
        screen->get_param(screen, PIPE_CAP_BUFFER_SAMPLER_VIEW_RGBA_ONLY))
        extensions->ARB_texture_buffer_object = GL_FALSE;

    if (extensions->ARB_texture_buffer_object) {
        consts->MaxTextureBufferSize =
                _min(screen->get_param(screen, PIPE_CAP_MAX_TEXTURE_BUFFER_SIZE),
                     (1u << 31) - 1);
        consts->TextureBufferOffsetAlignment =
                screen->get_param(screen, PIPE_CAP_TEXTURE_BUFFER_OFFSET_ALIGNMENT);

        if (consts->TextureBufferOffsetAlignment)
            extensions->ARB_texture_buffer_range = GL_TRUE;

        init_format_extensions(screen, extensions, tbo_rgb32,
                               ARRAY_SIZE(tbo_rgb32), PIPE_BUFFER,
                               PIPE_BIND_SAMPLER_VIEW);
    }

    extensions->OES_texture_buffer =
            consts->Program[MESA_SHADER_COMPUTE].MaxImageUniforms &&
            extensions->ARB_texture_buffer_object &&
            extensions->ARB_texture_buffer_range &&
            extensions->ARB_texture_buffer_object_rgb32;

    extensions->EXT_framebuffer_sRGB =
            screen->get_param(screen, PIPE_CAP_DEST_SURFACE_SRGB_CONTROL) &&
            extensions->EXT_sRGB;

    /* Unpacking a varying in the fragment shader costs 1 texture indirection.
     * If the number of available texture indirections is very limited, then we
     * prefer to disable varying packing rather than run the risk of varying
     * packing preventing a shader from running.
     */
    if (screen->get_shader_param(screen, PIPE_SHADER_FRAGMENT,
                                 PIPE_SHADER_CAP_MAX_TEX_INDIRECTIONS) <= 8) {
        /* We can't disable varying packing if transform feedback is available,
         * because transform feedback code assumes a packed varying layout.
         */
        if (!extensions->EXT_transform_feedback)
            consts->DisableVaryingPacking = GL_TRUE;
    }

    if (!screen->get_param(screen, PIPE_CAP_PACKED_STREAM_OUTPUT))
        consts->DisableTransformFeedbackPacking = GL_TRUE;

    if (screen->get_param(screen, PIPE_CAP_PREFER_POT_ALIGNED_VARYINGS))
        consts->PreferPOTAlignedVaryings = GL_TRUE;

    unsigned max_fb_fetch_rts = screen->get_param(screen, PIPE_CAP_FBFETCH);
    bool coherent_fb_fetch =
            screen->get_param(screen, PIPE_CAP_FBFETCH_COHERENT);

    if (screen->get_param(screen, PIPE_CAP_BLEND_EQUATION_ADVANCED))
        extensions->KHR_blend_equation_advanced = true;

    if (max_fb_fetch_rts > 0) {
        extensions->KHR_blend_equation_advanced = true;
        extensions->KHR_blend_equation_advanced_coherent = coherent_fb_fetch;

        if (max_fb_fetch_rts >=
            screen->get_param(screen, PIPE_CAP_MAX_RENDER_TARGETS)) {
            extensions->EXT_shader_framebuffer_fetch_non_coherent = true;
            extensions->EXT_shader_framebuffer_fetch = coherent_fb_fetch;
        }
    }

    consts->MaxViewports = screen->get_param(screen, PIPE_CAP_MAX_VIEWPORTS);
    if (consts->MaxViewports >= 16) {
        if (GLSLVersion >= 400) {
            consts->ViewportBounds.Min = -32768.0;
            consts->ViewportBounds.Max = 32767.0;
        } else {
            consts->ViewportBounds.Min = -16384.0;
            consts->ViewportBounds.Max = 16383.0;
        }
        extensions->ARB_viewport_array = GL_TRUE;
        extensions->ARB_fragment_layer_viewport = GL_TRUE;
        if (extensions->AMD_vertex_shader_layer)
            extensions->AMD_vertex_shader_viewport_index = GL_TRUE;
    }

    if (extensions->AMD_vertex_shader_layer &&
        extensions->AMD_vertex_shader_viewport_index &&
        screen->get_param(screen, PIPE_CAP_TGSI_TES_LAYER_VIEWPORT))
        extensions->ARB_shader_viewport_layer_array = GL_TRUE;

    /* ARB_framebuffer_no_attachments */
    if (screen->get_param(screen, PIPE_CAP_FRAMEBUFFER_NO_ATTACHMENT) &&
        ((consts->MaxSamples >= 4 && consts->MaxFramebufferLayers >= 2048) ||
         (consts->MaxFramebufferSamples >= consts->MaxSamples &&
          consts->MaxFramebufferLayers >= consts->MaxArrayTextureLayers)))
        extensions->ARB_framebuffer_no_attachments = GL_TRUE;

    /* GL_ARB_ES3_compatibility.
     * Check requirements for GLSL ES 3.00.
     */
    if (GLSLVersion >= 130 &&
        extensions->ARB_uniform_buffer_object &&
        (extensions->NV_primitive_restart ||
         consts->PrimitiveRestartFixedIndex) &&
        screen->get_shader_param(screen, PIPE_SHADER_VERTEX,
                                 PIPE_SHADER_CAP_MAX_TEXTURE_SAMPLERS) >= 16 &&
        /* Requirements for ETC2 emulation. */
        screen->is_format_supported(screen, PIPE_FORMAT_R8G8B8A8_UNORM,
                                    PIPE_TEXTURE_2D, 0, 0,
                                    PIPE_BIND_SAMPLER_VIEW) &&
        screen->is_format_supported(screen, PIPE_FORMAT_R8G8B8A8_SRGB,
                                    PIPE_TEXTURE_2D, 0, 0,
                                    PIPE_BIND_SAMPLER_VIEW) &&
        screen->is_format_supported(screen, PIPE_FORMAT_R16_UNORM,
                                    PIPE_TEXTURE_2D, 0, 0,
                                    PIPE_BIND_SAMPLER_VIEW) &&
        screen->is_format_supported(screen, PIPE_FORMAT_R16G16_UNORM,
                                    PIPE_TEXTURE_2D, 0, 0,
                                    PIPE_BIND_SAMPLER_VIEW) &&
        screen->is_format_supported(screen, PIPE_FORMAT_R16_SNORM,
                                    PIPE_TEXTURE_2D, 0, 0,
                                    PIPE_BIND_SAMPLER_VIEW) &&
        screen->is_format_supported(screen, PIPE_FORMAT_R16G16_SNORM,
                                    PIPE_TEXTURE_2D, 0, 0,
                                    PIPE_BIND_SAMPLER_VIEW)) {
        extensions->ARB_ES3_compatibility = GL_TRUE;
    }

#ifdef HAVE_ST_VDPAU
    if (screen->get_video_param &&
       screen->get_video_param(screen, PIPE_VIDEO_PROFILE_UNKNOWN,
                               PIPE_VIDEO_ENTRYPOINT_BITSTREAM,
                               PIPE_VIDEO_CAP_SUPPORTS_INTERLACED)) {
      extensions->NV_vdpau_interop = GL_TRUE;
   }
#endif

    if (screen->get_param(screen, PIPE_CAP_DOUBLES)) {
        extensions->ARB_gpu_shader_fp64 = GL_TRUE;
        extensions->ARB_vertex_attrib_64bit = GL_TRUE;
    }

    if ((ST_DEBUG & DEBUG_GREMEDY) &&
        screen->get_param(screen, PIPE_CAP_STRING_MARKER))
        extensions->GREMEDY_string_marker = GL_TRUE;

    if (screen->get_param(screen, PIPE_CAP_COMPUTE)) {
        int compute_supported_irs =
                screen->get_shader_param(screen, PIPE_SHADER_COMPUTE,
                                         PIPE_SHADER_CAP_SUPPORTED_IRS);
        if (compute_supported_irs & ((1 << PIPE_SHADER_IR_TGSI) |
                                     (1 << PIPE_SHADER_IR_NIR))) {
            enum pipe_shader_ir ir =
                    (compute_supported_irs & PIPE_SHADER_IR_NIR) ?
                    PIPE_SHADER_IR_NIR : PIPE_SHADER_IR_TGSI;
            uint64_t grid_size[3], block_size[3];
            uint64_t max_local_size, max_threads_per_block;

            screen->get_compute_param(screen, ir,
                                      PIPE_COMPUTE_CAP_MAX_GRID_SIZE, grid_size);
            screen->get_compute_param(screen, ir,
                                      PIPE_COMPUTE_CAP_MAX_BLOCK_SIZE, block_size);
            screen->get_compute_param(screen, ir,
                                      PIPE_COMPUTE_CAP_MAX_THREADS_PER_BLOCK,
                                      &max_threads_per_block);
            screen->get_compute_param(screen, ir,
                                      PIPE_COMPUTE_CAP_MAX_LOCAL_SIZE,
                                      &max_local_size);

            consts->MaxComputeWorkGroupInvocations = max_threads_per_block;
            consts->MaxComputeSharedMemorySize = max_local_size;

            for (i = 0; i < 3; i++) {
                consts->MaxComputeWorkGroupCount[i] = grid_size[i];
                consts->MaxComputeWorkGroupSize[i] = block_size[i];
            }

            extensions->ARB_compute_shader =
                    max_threads_per_block >= 1024 &&
                    extensions->ARB_shader_image_load_store &&
                    extensions->ARB_shader_atomic_counters;

            if (extensions->ARB_compute_shader) {
                uint64_t max_variable_threads_per_block = 0;

                screen->get_compute_param(screen, ir,
                                          PIPE_COMPUTE_CAP_MAX_VARIABLE_THREADS_PER_BLOCK,
                                          &max_variable_threads_per_block);

                for (i = 0; i < 3; i++) {
                    /* Clamp the values to avoid having a local work group size
                     * greater than the maximum number of invocations.
                     */
                    consts->MaxComputeVariableGroupSize[i] =
                            MIN2(consts->MaxComputeWorkGroupSize[i],
                                 max_variable_threads_per_block);
                }
                consts->MaxComputeVariableGroupInvocations =
                        max_variable_threads_per_block;

                extensions->ARB_compute_variable_group_size =
                        max_variable_threads_per_block > 0;
            }
        }
    }

    extensions->ARB_texture_float =
            extensions->OES_texture_half_float &&
            extensions->OES_texture_float;

    if (extensions->EXT_texture_filter_anisotropic &&
        screen->get_paramf(screen, PIPE_CAPF_MAX_TEXTURE_ANISOTROPY) >= 16.0)
        extensions->ARB_texture_filter_anisotropic = GL_TRUE;

    extensions->KHR_robustness = extensions->ARB_robust_buffer_access_behavior;

    /* If we support ES 3.1, we support the ES3_1_compatibility ext. However
     * there's no clean way of telling whether we would support ES 3.1 from
     * here, so copy the condition from compute_version_es2 here. A lot of
     * these are redunant, but simpler to just have a (near-)exact copy here.
     */
    extensions->ARB_ES3_1_compatibility =
            consts->Program[MESA_SHADER_FRAGMENT].MaxImageUniforms &&
            extensions->ARB_ES3_compatibility &&
            extensions->ARB_arrays_of_arrays &&
            extensions->ARB_compute_shader &&
            extensions->ARB_draw_indirect &&
            extensions->ARB_explicit_uniform_location &&
            extensions->ARB_framebuffer_no_attachments &&
            extensions->ARB_shader_atomic_counters &&
            extensions->ARB_shader_image_load_store &&
            extensions->ARB_shader_image_size &&
            extensions->ARB_shader_storage_buffer_object &&
            extensions->ARB_shading_language_packing &&
            extensions->ARB_stencil_texturing &&
            extensions->ARB_texture_multisample &&
            extensions->ARB_gpu_shader5 &&
            extensions->EXT_shader_integer_mix;

    extensions->OES_texture_cube_map_array =
            (extensions->ARB_ES3_1_compatibility || ESSLVersion >= 310) &&
            extensions->OES_geometry_shader &&
            extensions->ARB_texture_cube_map_array;

    extensions->OES_viewport_array =
            (extensions->ARB_ES3_1_compatibility || ESSLVersion >= 310) &&
            extensions->OES_geometry_shader &&
            extensions->ARB_viewport_array;

    extensions->OES_primitive_bounding_box =
            extensions->ARB_ES3_1_compatibility || ESSLVersion >= 310;

    consts->NoPrimitiveBoundingBoxOutput = true;

    extensions->ANDROID_extension_pack_es31a =
            consts->Program[MESA_SHADER_FRAGMENT].MaxImageUniforms &&
            extensions->KHR_texture_compression_astc_ldr &&
            extensions->KHR_blend_equation_advanced &&
            extensions->OES_sample_variables &&
            extensions->ARB_texture_stencil8 &&
            extensions->ARB_texture_multisample &&
            extensions->OES_copy_image &&
            extensions->ARB_draw_buffers_blend &&
            extensions->OES_geometry_shader &&
            extensions->ARB_gpu_shader5 &&
            extensions->OES_primitive_bounding_box &&
            extensions->ARB_tessellation_shader &&
            extensions->OES_texture_buffer &&
            extensions->OES_texture_cube_map_array &&
            extensions->EXT_texture_sRGB_decode;

    /* Same deal as for ARB_ES3_1_compatibility - this has to be computed
     * before overall versions are selected. Also it's actually a subset of ES
     * 3.2, since it doesn't require ASTC or advanced blending.
     */
    extensions->ARB_ES3_2_compatibility =
            extensions->ARB_ES3_1_compatibility &&
            extensions->KHR_robustness &&
            extensions->ARB_copy_image &&
            extensions->ARB_draw_buffers_blend &&
            extensions->ARB_draw_elements_base_vertex &&
            extensions->OES_geometry_shader &&
            extensions->ARB_gpu_shader5 &&
            extensions->ARB_sample_shading &&
            extensions->ARB_tessellation_shader &&
            extensions->OES_texture_buffer &&
            extensions->ARB_texture_cube_map_array &&
            extensions->ARB_texture_stencil8 &&
            extensions->ARB_texture_multisample;

    if (screen->get_param(screen, PIPE_CAP_CONSERVATIVE_RASTER_POST_SNAP_TRIANGLES) &&
        screen->get_param(screen, PIPE_CAP_CONSERVATIVE_RASTER_POST_SNAP_POINTS_LINES) &&
        screen->get_param(screen, PIPE_CAP_CONSERVATIVE_RASTER_POST_DEPTH_COVERAGE)) {
        float max_dilate;
        bool pre_snap_triangles, pre_snap_points_lines;

        max_dilate = screen->get_paramf(screen, PIPE_CAPF_MAX_CONSERVATIVE_RASTER_DILATE);

        pre_snap_triangles =
                screen->get_param(screen, PIPE_CAP_CONSERVATIVE_RASTER_PRE_SNAP_TRIANGLES);
        pre_snap_points_lines =
                screen->get_param(screen, PIPE_CAP_CONSERVATIVE_RASTER_PRE_SNAP_POINTS_LINES);

        extensions->NV_conservative_raster =
                screen->get_param(screen, PIPE_CAP_MAX_CONSERVATIVE_RASTER_SUBPIXEL_PRECISION_BIAS) > 1;

        if (extensions->NV_conservative_raster) {
            extensions->NV_conservative_raster_dilate = max_dilate >= 0.75;
            extensions->NV_conservative_raster_pre_snap_triangles = pre_snap_triangles;
            extensions->NV_conservative_raster_pre_snap =
                    pre_snap_triangles && pre_snap_points_lines;
        }
    }

    if (extensions->ARB_gl_spirv) {
        struct spirv_supported_capabilities *spirv_caps = &consts->SpirVCapabilities;

        spirv_caps->atomic_storage             = extensions->ARB_shader_atomic_counters;
        spirv_caps->demote_to_helper_invocation = extensions->EXT_demote_to_helper_invocation;
        spirv_caps->draw_parameters            = extensions->ARB_shader_draw_parameters;
        spirv_caps->derivative_group           = extensions->NV_compute_shader_derivatives;
        spirv_caps->float64                    = extensions->ARB_gpu_shader_fp64;
        spirv_caps->geometry_streams           = extensions->ARB_gpu_shader5;
        spirv_caps->image_ms_array             = extensions->ARB_shader_image_load_store &&
                                                 consts->MaxImageSamples > 1;
        spirv_caps->image_read_without_format  = extensions->EXT_shader_image_load_formatted;
        spirv_caps->image_write_without_format = extensions->ARB_shader_image_load_store;
        spirv_caps->int64                      = extensions->ARB_gpu_shader_int64;
        spirv_caps->int64_atomics              = extensions->NV_shader_atomic_int64;
        spirv_caps->post_depth_coverage        = extensions->ARB_post_depth_coverage;
        spirv_caps->shader_clock               = extensions->ARB_shader_clock;
        spirv_caps->shader_viewport_index_layer = extensions->ARB_shader_viewport_layer_array;
        spirv_caps->stencil_export             = extensions->ARB_shader_stencil_export;
        spirv_caps->storage_image_ms           = extensions->ARB_shader_image_load_store &&
                                                 consts->MaxImageSamples > 1;
        spirv_caps->subgroup_ballot            = extensions->ARB_shader_ballot;
        spirv_caps->subgroup_vote              = extensions->ARB_shader_group_vote;
        spirv_caps->tessellation               = extensions->ARB_tessellation_shader;
        spirv_caps->transform_feedback         = extensions->ARB_transform_feedback3;
        spirv_caps->variable_pointers          =
                screen->get_param(screen, PIPE_CAP_GL_SPIRV_VARIABLE_POINTERS);
        spirv_caps->integer_functions2         = extensions->INTEL_shader_integer_functions2;

        consts->SpirVExtensions = CALLOC_STRUCT(spirv_supported_extensions);
        _mesa_fill_supported_spirv_extensions(consts->SpirVExtensions, spirv_caps);
    }

    consts->AllowDrawOutOfOrder = options->allow_draw_out_of_order;
    consts->GLThreadNopCheckFramebufferStatus = options->glthread_nop_check_framebuffer_status;

    bool prefer_nir = PIPE_SHADER_IR_NIR ==
                      screen->get_shader_param(screen, PIPE_SHADER_FRAGMENT, PIPE_SHADER_CAP_PREFERRED_IR);
    const struct nir_shader_compiler_options *nir_options =
            consts->ShaderCompilerOptions[MESA_SHADER_FRAGMENT].NirOptions;

    if (prefer_nir &&
        screen->get_shader_param(screen, PIPE_SHADER_FRAGMENT, PIPE_SHADER_CAP_INTEGERS) &&
        extensions->ARB_stencil_texturing &&
        screen->get_param(screen, PIPE_CAP_DOUBLES) &&
        !(nir_options->lower_doubles_options & nir_lower_fp64_full_software))
        extensions->NV_copy_depth_to_color = TRUE;

    if (prefer_nir)
        extensions->ARB_point_sprite = GL_TRUE;
}


static unsigned
get_version(struct pipe_screen *screen,
            struct st_config_options *options, gl_api api)
{
    struct gl_constants consts = {0};
    struct gl_extensions extensions = {0};
    GLuint version;

    if (_mesa_override_gl_version_contextless(&consts, &api, &version)) {
        return version;
    }

    _mesa_init_constants(&consts, api);
    _mesa_init_extensions(&extensions);

    st_init_limits(screen, &consts, &extensions);
    st_init_extensions(screen, &consts, &extensions, options, api);
    version = _mesa_get_version(&extensions, &consts, api);
    free(consts.SpirVExtensions);
    return version;
}


static void
st_api_query_versions(struct st_api *stapi, struct st_manager *sm,
                      struct st_config_options *options,
                      int *gl_core_version,
                      int *gl_compat_version,
                      int *gl_es1_version,
                      int *gl_es2_version)
{
    *gl_core_version = get_version(sm->screen, options, API_OPENGL_CORE);
    *gl_compat_version = get_version(sm->screen, options, API_OPENGL_COMPAT);
    *gl_es1_version = get_version(sm->screen, options, API_OPENGLES);
    *gl_es2_version = get_version(sm->screen, options, API_OPENGLES2);
}

void
_mesa_reference_framebuffer_(struct gl_framebuffer **ptr,
                             struct gl_framebuffer *fb)
{
    if (*ptr) {
        /* unreference old renderbuffer */
        GLboolean deleteFlag = GL_FALSE;
        struct gl_framebuffer *oldFb = *ptr;

        simple_mtx_lock(&oldFb->Mutex);
        assert(oldFb->RefCount > 0);
        oldFb->RefCount--;
        deleteFlag = (oldFb->RefCount == 0);
        simple_mtx_unlock(&oldFb->Mutex);

        if (deleteFlag)
            oldFb->Delete(oldFb);

        *ptr = NULL;
    }

    if (fb) {
        simple_mtx_lock(&fb->Mutex);
        fb->RefCount++;
        simple_mtx_unlock(&fb->Mutex);
        *ptr = fb;
    }
}

void
st_framebuffer_reference(struct st_framebuffer **ptr,
                         struct st_framebuffer *stfb)
{
    struct gl_framebuffer *fb = stfb ? &stfb->Base : NULL;
    _mesa_reference_framebuffer((struct gl_framebuffer **) ptr, fb);
}

static inline bool
st_visual_have_buffers(const struct st_visual *visual, unsigned mask)
{
    return ((visual->buffer_mask & mask) == mask);
}

static inline uint
util_format_get_component_bits(enum pipe_format format,
                               enum util_format_colorspace colorspace,
                               uint component)
{
    const struct util_format_description *desc = util_format_description(format);
    enum util_format_colorspace desc_colorspace;

    assert(format);
    if (!format) {
        return 0;
    }

    assert(component < 4);

    /* Treat RGB and SRGB as equivalent. */
    if (colorspace == UTIL_FORMAT_COLORSPACE_SRGB) {
        colorspace = UTIL_FORMAT_COLORSPACE_RGB;
    }
    if (desc->colorspace == UTIL_FORMAT_COLORSPACE_SRGB) {
        desc_colorspace = UTIL_FORMAT_COLORSPACE_RGB;
    } else {
        desc_colorspace = desc->colorspace;
    }

    if (desc_colorspace != colorspace) {
        return 0;
    }

    switch (desc->swizzle[component]) {
        case UTIL_FORMAT_SWIZZLE_X:
            return desc->channel[0].size;
        case UTIL_FORMAT_SWIZZLE_Y:
            return desc->channel[1].size;
        case UTIL_FORMAT_SWIZZLE_Z:
            return desc->channel[2].size;
        case UTIL_FORMAT_SWIZZLE_W:
            return desc->channel[3].size;
        default:
            return 0;
    }
}

static inline boolean
util_format_is_srgb(enum pipe_format format)
{
    const struct util_format_description *desc = util_format_description(format);
    return desc->colorspace == UTIL_FORMAT_COLORSPACE_SRGB;
}

static void
st_visual_to_context_mode(const struct st_visual *visual,
                          struct gl_config *mode)
{
    memset(mode, 0, sizeof(*mode));

    if (st_visual_have_buffers(visual, ST_ATTACHMENT_BACK_LEFT_MASK))
        mode->doubleBufferMode = GL_TRUE;

    if (st_visual_have_buffers(visual,
                               ST_ATTACHMENT_FRONT_RIGHT_MASK | ST_ATTACHMENT_BACK_RIGHT_MASK))
        mode->stereoMode = GL_TRUE;

    if (visual->color_format != PIPE_FORMAT_NONE) {
        mode->redBits =
                util_format_get_component_bits(visual->color_format,
                                               UTIL_FORMAT_COLORSPACE_RGB, 0);
        mode->greenBits =
                util_format_get_component_bits(visual->color_format,
                                               UTIL_FORMAT_COLORSPACE_RGB, 1);
        mode->blueBits =
                util_format_get_component_bits(visual->color_format,
                                               UTIL_FORMAT_COLORSPACE_RGB, 2);
        mode->alphaBits =
                util_format_get_component_bits(visual->color_format,
                                               UTIL_FORMAT_COLORSPACE_RGB, 3);

        mode->rgbBits = mode->redBits +
                        mode->greenBits + mode->blueBits + mode->alphaBits;
        mode->sRGBCapable = util_format_is_srgb(visual->color_format);
    }

    if (visual->depth_stencil_format != PIPE_FORMAT_NONE) {
        mode->depthBits =
                util_format_get_component_bits(visual->depth_stencil_format,
                                               UTIL_FORMAT_COLORSPACE_ZS, 0);
        mode->stencilBits =
                util_format_get_component_bits(visual->depth_stencil_format,
                                               UTIL_FORMAT_COLORSPACE_ZS, 1);
    }

    if (visual->accum_format != PIPE_FORMAT_NONE) {
        mode->accumRedBits =
                util_format_get_component_bits(visual->accum_format,
                                               UTIL_FORMAT_COLORSPACE_RGB, 0);
        mode->accumGreenBits =
                util_format_get_component_bits(visual->accum_format,
                                               UTIL_FORMAT_COLORSPACE_RGB, 1);
        mode->accumBlueBits =
                util_format_get_component_bits(visual->accum_format,
                                               UTIL_FORMAT_COLORSPACE_RGB, 2);
        mode->accumAlphaBits =
                util_format_get_component_bits(visual->accum_format,
                                               UTIL_FORMAT_COLORSPACE_RGB, 3);
    }

    if (visual->samples > 1) {
        mode->samples = visual->samples;
    }
}

static inline enum pipe_format
util_format_srgb(enum pipe_format format)
{
    if (util_format_is_srgb(format))
        return format;

    switch (format) {
        case PIPE_FORMAT_L8_UNORM:
            return PIPE_FORMAT_L8_SRGB;
        case PIPE_FORMAT_R8_UNORM:
            return PIPE_FORMAT_R8_SRGB;
        case PIPE_FORMAT_L8A8_UNORM:
            return PIPE_FORMAT_L8A8_SRGB;
        case PIPE_FORMAT_R8G8_UNORM:
            return PIPE_FORMAT_R8G8_SRGB;
        case PIPE_FORMAT_R8G8B8_UNORM:
            return PIPE_FORMAT_R8G8B8_SRGB;
        case PIPE_FORMAT_B8G8R8_UNORM:
            return PIPE_FORMAT_B8G8R8_SRGB;
        case PIPE_FORMAT_A8B8G8R8_UNORM:
            return PIPE_FORMAT_A8B8G8R8_SRGB;
        case PIPE_FORMAT_X8B8G8R8_UNORM:
            return PIPE_FORMAT_X8B8G8R8_SRGB;
        case PIPE_FORMAT_B8G8R8A8_UNORM:
            return PIPE_FORMAT_B8G8R8A8_SRGB;
        case PIPE_FORMAT_B8G8R8X8_UNORM:
            return PIPE_FORMAT_B8G8R8X8_SRGB;
        case PIPE_FORMAT_A8R8G8B8_UNORM:
            return PIPE_FORMAT_A8R8G8B8_SRGB;
        case PIPE_FORMAT_X8R8G8B8_UNORM:
            return PIPE_FORMAT_X8R8G8B8_SRGB;
        case PIPE_FORMAT_R8G8B8A8_UNORM:
            return PIPE_FORMAT_R8G8B8A8_SRGB;
        case PIPE_FORMAT_R8G8B8X8_UNORM:
            return PIPE_FORMAT_R8G8B8X8_SRGB;
        case PIPE_FORMAT_DXT1_RGB:
            return PIPE_FORMAT_DXT1_SRGB;
        case PIPE_FORMAT_DXT1_RGBA:
            return PIPE_FORMAT_DXT1_SRGBA;
        case PIPE_FORMAT_DXT3_RGBA:
            return PIPE_FORMAT_DXT3_SRGBA;
        case PIPE_FORMAT_DXT5_RGBA:
            return PIPE_FORMAT_DXT5_SRGBA;
        case PIPE_FORMAT_R5G6B5_UNORM:
            return PIPE_FORMAT_R5G6B5_SRGB;
        case PIPE_FORMAT_B5G6R5_UNORM:
            return PIPE_FORMAT_B5G6R5_SRGB;
        case PIPE_FORMAT_BPTC_RGBA_UNORM:
            return PIPE_FORMAT_BPTC_SRGBA;
        case PIPE_FORMAT_ETC2_RGB8:
            return PIPE_FORMAT_ETC2_SRGB8;
        case PIPE_FORMAT_ETC2_RGB8A1:
            return PIPE_FORMAT_ETC2_SRGB8A1;
        case PIPE_FORMAT_ETC2_RGBA8:
            return PIPE_FORMAT_ETC2_SRGBA8;
        case PIPE_FORMAT_ASTC_4x4:
            return PIPE_FORMAT_ASTC_4x4_SRGB;
        case PIPE_FORMAT_ASTC_5x4:
            return PIPE_FORMAT_ASTC_5x4_SRGB;
        case PIPE_FORMAT_ASTC_5x5:
            return PIPE_FORMAT_ASTC_5x5_SRGB;
        case PIPE_FORMAT_ASTC_6x5:
            return PIPE_FORMAT_ASTC_6x5_SRGB;
        case PIPE_FORMAT_ASTC_6x6:
            return PIPE_FORMAT_ASTC_6x6_SRGB;
        case PIPE_FORMAT_ASTC_8x5:
            return PIPE_FORMAT_ASTC_8x5_SRGB;
        case PIPE_FORMAT_ASTC_8x6:
            return PIPE_FORMAT_ASTC_8x6_SRGB;
        case PIPE_FORMAT_ASTC_8x8:
            return PIPE_FORMAT_ASTC_8x8_SRGB;
        case PIPE_FORMAT_ASTC_10x5:
            return PIPE_FORMAT_ASTC_10x5_SRGB;
        case PIPE_FORMAT_ASTC_10x6:
            return PIPE_FORMAT_ASTC_10x6_SRGB;
        case PIPE_FORMAT_ASTC_10x8:
            return PIPE_FORMAT_ASTC_10x8_SRGB;
        case PIPE_FORMAT_ASTC_10x10:
            return PIPE_FORMAT_ASTC_10x10_SRGB;
        case PIPE_FORMAT_ASTC_12x10:
            return PIPE_FORMAT_ASTC_12x10_SRGB;
        case PIPE_FORMAT_ASTC_12x12:
            return PIPE_FORMAT_ASTC_12x12_SRGB;
        case PIPE_FORMAT_ASTC_3x3x3:
            return PIPE_FORMAT_ASTC_3x3x3_SRGB;
        case PIPE_FORMAT_ASTC_4x3x3:
            return PIPE_FORMAT_ASTC_4x3x3_SRGB;
        case PIPE_FORMAT_ASTC_4x4x3:
            return PIPE_FORMAT_ASTC_4x4x3_SRGB;
        case PIPE_FORMAT_ASTC_4x4x4:
            return PIPE_FORMAT_ASTC_4x4x4_SRGB;
        case PIPE_FORMAT_ASTC_5x4x4:
            return PIPE_FORMAT_ASTC_5x4x4_SRGB;
        case PIPE_FORMAT_ASTC_5x5x4:
            return PIPE_FORMAT_ASTC_5x5x4_SRGB;
        case PIPE_FORMAT_ASTC_5x5x5:
            return PIPE_FORMAT_ASTC_5x5x5_SRGB;
        case PIPE_FORMAT_ASTC_6x5x5:
            return PIPE_FORMAT_ASTC_6x5x5_SRGB;
        case PIPE_FORMAT_ASTC_6x6x5:
            return PIPE_FORMAT_ASTC_6x6x5_SRGB;
        case PIPE_FORMAT_ASTC_6x6x6:
            return PIPE_FORMAT_ASTC_6x6x6_SRGB;

        default:
            return PIPE_FORMAT_NONE;
    }
}

mesa_format
st_pipe_format_to_mesa_format(enum pipe_format format)
{
    mesa_format mf = format;
    if (!_mesa_get_format_name(mf))
        return MESA_FORMAT_NONE;
    return mf;
}

static inline int
mtx_init(mtx_t *mtx, int type)
{
    pthread_mutexattr_t attr;
    assert(mtx != NULL);
    if (type != mtx_plain && type != mtx_timed && type != mtx_try
        && type != (mtx_plain|mtx_recursive)
        && type != (mtx_timed|mtx_recursive)
        && type != (mtx_try|mtx_recursive))
        return thrd_error;

    if ((type & mtx_recursive) == 0) {
        pthread_mutex_init(mtx, NULL);
        return thrd_success;
    }

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(mtx, &attr);
    pthread_mutexattr_destroy(&attr);
    return thrd_success;
}

static inline void
simple_mtx_init(simple_mtx_t *mtx, int type)
{
    mtx_init(mtx, type);
}

static inline void
simple_mtx_destroy(simple_mtx_t *mtx)
{
    mtx_destroy(mtx);
}

void
_mesa_reference_renderbuffer_(struct gl_renderbuffer **ptr,
                              struct gl_renderbuffer *rb)
{
    if (*ptr) {
        /* Unreference the old renderbuffer */
        struct gl_renderbuffer *oldRb = *ptr;

        assert(oldRb->RefCount > 0);

        if (p_atomic_dec_zero(&oldRb->RefCount)) {
            GET_CURRENT_CONTEXT(ctx);
            oldRb->Delete(ctx, oldRb);
        }
    }

    if (rb) {
        /* reference new renderbuffer */
        p_atomic_inc(&rb->RefCount);
    }

    *ptr = rb;
}


static inline void
_mesa_reference_renderbuffer(struct gl_renderbuffer **ptr,
                             struct gl_renderbuffer *rb)
{
    if (*ptr != rb)
        _mesa_reference_renderbuffer_(ptr, rb);
}

static GLboolean
valid_texture_object(const struct gl_texture_object *tex)
{
    switch (tex->Target) {
        case 0:
        case GL_TEXTURE_1D:
        case GL_TEXTURE_2D:
        case GL_TEXTURE_3D:
        case GL_TEXTURE_CUBE_MAP:
        case GL_TEXTURE_RECTANGLE_NV:
        case GL_TEXTURE_1D_ARRAY_EXT:
        case GL_TEXTURE_2D_ARRAY_EXT:
        case GL_TEXTURE_BUFFER:
        case GL_TEXTURE_EXTERNAL_OES:
        case GL_TEXTURE_CUBE_MAP_ARRAY:
        case GL_TEXTURE_2D_MULTISAMPLE:
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            return GL_TRUE;
        case 0x99:
            _mesa_problem(NULL, "invalid reference to a deleted texture object");
            return GL_FALSE;
        default:
            _mesa_problem(NULL, "invalid texture object Target 0x%x, Id = %u",
                          tex->Target, tex->Name);
            return GL_FALSE;
    }
}

static void
st_remove_private_references(struct st_sampler_view *sv)
{
    if (sv->private_refcount) {
        assert(sv->private_refcount > 0);
        p_atomic_add(&sv->view->reference.count, -sv->private_refcount);
        sv->private_refcount = 0;
    }
}

void
debug_describe_resource(char* buf, const struct pipe_resource *ptr)
{
    switch(ptr->target)
    {
        case PIPE_BUFFER:
            sprintf(buf, "pipe_buffer<%u>", (unsigned)util_format_get_stride(ptr->format, ptr->width0));
            break;
        case PIPE_TEXTURE_1D:
            sprintf(buf, "pipe_texture1d<%u,%s,%u>", ptr->width0, util_format_short_name(ptr->format), ptr->last_level);
            break;
        case PIPE_TEXTURE_2D:
            sprintf(buf, "pipe_texture2d<%u,%u,%s,%u>", ptr->width0, ptr->height0, util_format_short_name(ptr->format), ptr->last_level);
            break;
        case PIPE_TEXTURE_RECT:
            sprintf(buf, "pipe_texture_rect<%u,%u,%s>", ptr->width0, ptr->height0, util_format_short_name(ptr->format));
            break;
        case PIPE_TEXTURE_CUBE:
            sprintf(buf, "pipe_texture_cube<%u,%u,%s,%u>", ptr->width0, ptr->height0, util_format_short_name(ptr->format), ptr->last_level);
            break;
        case PIPE_TEXTURE_3D:
            sprintf(buf, "pipe_texture3d<%u,%u,%u,%s,%u>", ptr->width0, ptr->height0, ptr->depth0, util_format_short_name(ptr->format), ptr->last_level);
            break;
        case PIPE_TEXTURE_1D_ARRAY:
            sprintf(buf, "pipe_texture_1darray<%u,%u,%s,%u>", ptr->width0, ptr->array_size, util_format_short_name(ptr->format), ptr->last_level);
            break;
        case PIPE_TEXTURE_2D_ARRAY:
            sprintf(buf, "pipe_texture_2darray<%u,%u,%u,%s,%u>", ptr->width0, ptr->height0, ptr->array_size, util_format_short_name(ptr->format), ptr->last_level);
            break;
        case PIPE_TEXTURE_CUBE_ARRAY:
            sprintf(buf, "pipe_texture_cubearray<%u,%u,%u,%s,%u>", ptr->width0, ptr->height0, ptr->array_size, util_format_short_name(ptr->format), ptr->last_level);
            break;
        default:
            sprintf(buf, "pipe_martian_resource<%u>", ptr->target);
            break;
    }
}

void
debug_describe_sampler_view(char* buf, const struct pipe_sampler_view *ptr)
{
    char res[128];
    debug_describe_resource(res, ptr->texture);
    sprintf(buf, "pipe_sampler_view<%s,%s>", res, util_format_short_name(ptr->format));
}

static inline void
pipe_sampler_view_reference(struct pipe_sampler_view **dst,
                            struct pipe_sampler_view *src)
{
    struct pipe_sampler_view *old_dst = *dst;

    if (pipe_reference_described(old_dst ? &old_dst->reference : NULL,
                                 src ? &src->reference : NULL,
                                 (debug_reference_descriptor)
                                         debug_describe_sampler_view))
        old_dst->context->sampler_view_destroy(old_dst->context, old_dst);
    *dst = src;
}

static inline void list_addtail(struct list_head *item, struct list_head *list)
{
    item->next = list;
    item->prev = list->prev;
    list->prev->next = item;
    list->prev = item;
}

void
st_save_zombie_sampler_view(struct st_context *st,
                            struct pipe_sampler_view *view)
{
    struct st_zombie_sampler_view_node *entry;

    assert(view->context == st->pipe);

    entry = MALLOC_STRUCT(st_zombie_sampler_view_node);
    if (!entry)
        return;

    entry->view = view;

    /* We need a mutex since this function may be called from one thread
     * while free_zombie_resource_views() is called from another.
     */
    simple_mtx_lock(&st->zombie_sampler_views.mutex);
    list_addtail(&entry->node, &st->zombie_sampler_views.list.node);
    simple_mtx_unlock(&st->zombie_sampler_views.mutex);
}

void
st_texture_release_all_sampler_views(struct st_context *st,
                                     struct gl_texture_object *stObj)
{
    /* TODO: This happens while a texture is deleted, because the Driver API
     * is asymmetric: the driver allocates the texture object memory, but
     * mesa/main frees it.
     */
    if (!stObj->sampler_views)
        return;

    simple_mtx_lock(&stObj->validate_mutex);
    struct st_sampler_views *views = stObj->sampler_views;
    for (unsigned i = 0; i < views->count; ++i) {
        struct st_sampler_view *stsv = &views->views[i];
        if (stsv->view) {
            st_remove_private_references(stsv);

            if (stsv->st && stsv->st != st) {
                /* Transfer this reference to the zombie list.  It will
                 * likely be freed when the zombie list is freed.
                 */
                st_save_zombie_sampler_view(stsv->st, stsv->view);
                stsv->view = NULL;
            } else {
                pipe_sampler_view_reference(&stsv->view, NULL);
            }
        }
    }
    views->count = 0;
    simple_mtx_unlock(&stObj->validate_mutex);
}

void
st_delete_texture_sampler_views(struct st_context *st,
                                struct st_texture_object *stObj)
{
    st_texture_release_all_sampler_views(st, stObj);

    /* Free the container of the current per-context sampler views */
    assert(stObj->sampler_views->count == 0);
    free(stObj->sampler_views);
    stObj->sampler_views = NULL;

    /* Free old sampler view containers */
    while (stObj->sampler_views_old) {
        struct st_sampler_views *views = stObj->sampler_views_old;
        stObj->sampler_views_old = views->next;
        free(views);
    }
}

void
_mesa_delete_texture_image(struct gl_context *ctx,
                           struct gl_texture_image *texImage)
{
    assert(ctx->Driver.FreeTextureImageBuffer);
    ctx->Driver.FreeTextureImageBuffer( ctx, texImage );
    free(texImage);
}

#define util_dynarray_foreach(buf, type, elem) \
   for (type *elem = (type *)(buf)->data; \
        elem < (type *)((char *)(buf)->data + (buf)->size); elem++)

#define util_dynarray_delete_unordered(buf, type, v)                    \
   do {                                                                 \
      unsigned num_elements = (buf)->size / sizeof(type);               \
      unsigned i;                                                       \
      for (i = 0; i < num_elements; i++) {                              \
         type __v = *util_dynarray_element((buf), type, (i));           \
         if (v == __v) {                                                \
            memcpy(util_dynarray_element((buf), type, (i)),             \
                   util_dynarray_pop_ptr((buf), type), sizeof(type));   \
            break;                                                      \
         }                                                              \
      }                                                                 \
   } while (0)

struct util_dynarray
{
    void *mem_ctx;
    void *data;
    unsigned size;
    unsigned capacity;
};

static inline void
util_dynarray_init(struct util_dynarray *buf, void *mem_ctx)
{
    memset(buf, 0, sizeof(*buf));
    buf->mem_ctx = mem_ctx;
}


static inline void
util_dynarray_fini(struct util_dynarray *buf)
{
    if (buf->data) {
        if (buf->mem_ctx) {
            ralloc_free(buf->data);
        } else {
            free(buf->data);
        }
        util_dynarray_init(buf, buf->mem_ctx);
    }
}

#define util_dynarray_element(buf, type, idx) ((type*)(buf)->data + (idx))
#define util_dynarray_pop_ptr(buf, type) (type*)((char*)(buf)->data + ((buf)->size -= sizeof(type)))

struct hash_key_u64 {
    uint64_t value;
};

static struct hash_entry *
hash_table_u64_search(struct hash_table_u64 *ht, uint64_t key)
{
    if (sizeof(void *) == 8) {
        return _mesa_hash_table_search(ht->table, (void *)(uintptr_t)key);
    } else {
        struct hash_key_u64 _key = { .value = key };
        return _mesa_hash_table_search(ht->table, &_key);
    }
}

#define FREED_KEY_VALUE 0

void
_mesa_hash_table_u64_remove(struct hash_table_u64 *ht, uint64_t key)
{
    struct hash_entry *entry;

    if (key == FREED_KEY_VALUE) {
        ht->freed_key_data = NULL;
        return;
    }

    if (key == DELETED_KEY_VALUE) {
        ht->deleted_key_data = NULL;
        return;
    }

    entry = hash_table_u64_search(ht, key);
    if (!entry)
        return;

    if (sizeof(void *) == 8) {
        _mesa_hash_table_remove(ht->table, entry);
    } else {
        struct hash_key *_key = (struct hash_key *)entry->key;

        _mesa_hash_table_remove(ht->table, entry);
        free(_key);
    }
}

static void
delete_texture_handle(struct gl_context *ctx, GLuint64 id)
{
    mtx_lock(&ctx->Shared->HandlesMutex);
    _mesa_hash_table_u64_remove(ctx->Shared->TextureHandles, id);
    mtx_unlock(&ctx->Shared->HandlesMutex);

    ctx->Driver.DeleteTextureHandle(ctx, id);
}

static void
delete_image_handle(struct gl_context *ctx, GLuint64 id)
{
    mtx_lock(&ctx->Shared->HandlesMutex);
    _mesa_hash_table_u64_remove(ctx->Shared->ImageHandles, id);
    mtx_unlock(&ctx->Shared->HandlesMutex);

    ctx->Driver.DeleteImageHandle(ctx, id);
}

void
_mesa_delete_texture_handles(struct gl_context *ctx,
                             struct gl_texture_object *texObj)
{
    /* Texture handles */
    util_dynarray_foreach(&texObj->SamplerHandles,
    struct gl_texture_handle_object *, texHandleObj) {
        struct gl_sampler_object *sampObj = (*texHandleObj)->sampObj;

        if (sampObj) {
            /* Delete the handle in the separate sampler object. */
            util_dynarray_delete_unordered(&sampObj->Handles,
            struct gl_texture_handle_object *,
            *texHandleObj);
        }
        delete_texture_handle(ctx, (*texHandleObj)->handle);
        free(*texHandleObj);
    }
    util_dynarray_fini(&texObj->SamplerHandles);

    /* Image handles */
    util_dynarray_foreach(&texObj->ImageHandles,
    struct gl_image_handle_object *, imgHandleObj) {
        delete_image_handle(ctx, (*imgHandleObj)->handle);
        free(*imgHandleObj);
    }
    util_dynarray_fini(&texObj->ImageHandles);
}

void
_mesa_reference_buffer_object_(struct gl_context *ctx,
                               struct gl_buffer_object **ptr,
                               struct gl_buffer_object *bufObj,
                               bool shared_binding)
{
    if (*ptr) {
        /* Unreference the old buffer */
        struct gl_buffer_object *oldObj = *ptr;

        assert(oldObj->RefCount >= 1);

        /* Count references only if the context doesn't own the buffer or if
         * ptr is a binding point shared by multiple contexts (such as a texture
         * buffer object being a buffer bound within a texture object).
         */
        if (shared_binding || ctx != oldObj->Ctx) {
            if (p_atomic_dec_zero(&oldObj->RefCount)) {
                assert(ctx->Driver.DeleteBuffer);
                ctx->Driver.DeleteBuffer(ctx, oldObj);
            }
        } else if (ctx == oldObj->Ctx) {
            /* Update the private ref count. */
            assert(oldObj->CtxRefCount >= 1);
            oldObj->CtxRefCount--;
        }

        *ptr = NULL;
    }
    assert(!*ptr);

    if (bufObj) {
        /* reference new buffer */
        if (shared_binding || ctx != bufObj->Ctx)
            p_atomic_inc(&bufObj->RefCount);
        else if (ctx == bufObj->Ctx)
            bufObj->CtxRefCount++;

        *ptr = bufObj;
    }
}

static inline void
_mesa_reference_buffer_object_shared(struct gl_context *ctx,
                                     struct gl_buffer_object **ptr,
                                     struct gl_buffer_object *bufObj)
{
    if (*ptr != bufObj)
        _mesa_reference_buffer_object_(ctx, ptr, bufObj, true);
}

void
_mesa_delete_texture_object(struct gl_context *ctx,
                            struct gl_texture_object *texObj)
{
    GLuint i, face;

    /* Set Target to an invalid value.  With some assertions elsewhere
     * we can try to detect possible use of deleted textures.
     */
    texObj->Target = 0x99;

    pipe_resource_reference(&texObj->pt, NULL);
    st_delete_texture_sampler_views(ctx->st, texObj);
    simple_mtx_destroy(&texObj->validate_mutex);

    /* free the texture images */
    for (face = 0; face < 6; face++) {
        for (i = 0; i < MAX_TEXTURE_LEVELS; i++) {
            if (texObj->Image[face][i]) {
                _mesa_delete_texture_image(ctx, texObj->Image[face][i]);
            }
        }
    }

    /* Delete all texture/image handles. */
    _mesa_delete_texture_handles(ctx, texObj);

    _mesa_reference_buffer_object_shared(ctx, &texObj->BufferObject, NULL);
    free(texObj->Label);

    /* free this object */
    FREE(texObj);
}


void
_mesa_reference_texobj_(struct gl_texture_object **ptr,
                        struct gl_texture_object *tex)
{
    assert(ptr);

    if (*ptr) {
        /* Unreference the old texture */
        struct gl_texture_object *oldTex = *ptr;

        assert(valid_texture_object(oldTex));
        (void) valid_texture_object; /* silence warning in release builds */

        assert(oldTex->RefCount > 0);

        if (p_atomic_dec_zero(&oldTex->RefCount)) {
            /* Passing in the context drastically changes the driver code for
             * framebuffer deletion.
             */
            GET_CURRENT_CONTEXT(ctx);
            if (ctx)
                _mesa_delete_texture_object(ctx, oldTex);
            else
                _mesa_problem(NULL, "Unable to delete texture, no context");
        }
    }

    if (tex) {
        /* reference new texture */
        assert(valid_texture_object(tex));
        assert(tex->RefCount > 0);

        p_atomic_inc(&tex->RefCount);
    }

    *ptr = tex;
}

static inline void
_mesa_reference_texobj(struct gl_texture_object **ptr,
                       struct gl_texture_object *tex)
{
    if (*ptr != tex)
        _mesa_reference_texobj_(ptr, tex);
}

void
_mesa_free_framebuffer_data(struct gl_framebuffer *fb)
{
    assert(fb);
    assert(fb->RefCount == 0);

    simple_mtx_destroy(&fb->Mutex);

    for (unsigned i = 0; i < BUFFER_COUNT; i++) {
        struct gl_renderbuffer_attachment *att = &fb->Attachment[i];
        if (att->Renderbuffer) {
            _mesa_reference_renderbuffer(&att->Renderbuffer, NULL);
        }
        if (att->Texture) {
            _mesa_reference_texobj(&att->Texture, NULL);
        }
        assert(!att->Renderbuffer);
        assert(!att->Texture);
        att->Type = GL_NONE;
    }

    free(fb->SampleLocationTable);
    fb->SampleLocationTable = NULL;
}

void
_mesa_destroy_framebuffer(struct gl_framebuffer *fb)
{
    if (fb) {
        _mesa_free_framebuffer_data(fb);
        free(fb->Label);
        free(fb);
    }
}

static void
compute_depth_max(struct gl_framebuffer *fb)
{
    if (fb->Visual.depthBits == 0) {
        /* Special case.  Even if we don't have a depth buffer we need
         * good values for DepthMax for Z vertex transformation purposes
         * and for per-fragment fog computation.
         */
        fb->_DepthMax = (1 << 16) - 1;
    }
    else if (fb->Visual.depthBits < 32) {
        fb->_DepthMax = (1 << fb->Visual.depthBits) - 1;
    }
    else {
        /* Special case since shift values greater than or equal to the
         * number of bits in the left hand expression's type are undefined.
         */
        fb->_DepthMax = 0xffffffff;
    }
    fb->_DepthMaxF = (GLfloat) fb->_DepthMax;

    /* Minimum resolvable depth value, for polygon offset */
    fb->_MRD = (GLfloat)1.0 / fb->_DepthMaxF;
}

void
_mesa_initialize_window_framebuffer(struct gl_framebuffer *fb,
                                    const struct gl_config *visual)
{
    assert(fb);
    assert(visual);

    memset(fb, 0, sizeof(struct gl_framebuffer));

    simple_mtx_init(&fb->Mutex, mtx_plain);

    fb->RefCount = 1;

    /* save the visual */
    fb->Visual = *visual;

    /* Init read/draw renderbuffer state */
    if (visual->doubleBufferMode) {
        fb->_NumColorDrawBuffers = 1;
        fb->ColorDrawBuffer[0] = GL_BACK;
        fb->_ColorDrawBufferIndexes[0] = BUFFER_BACK_LEFT;
        fb->ColorReadBuffer = GL_BACK;
        fb->_ColorReadBufferIndex = BUFFER_BACK_LEFT;
    }
    else {
        fb->_NumColorDrawBuffers = 1;
        fb->ColorDrawBuffer[0] = GL_FRONT;
        fb->_ColorDrawBufferIndexes[0] = BUFFER_FRONT_LEFT;
        fb->ColorReadBuffer = GL_FRONT;
        fb->_ColorReadBufferIndex = BUFFER_FRONT_LEFT;
    }

    fb->Delete = _mesa_destroy_framebuffer;
    fb->_Status = GL_FRAMEBUFFER_COMPLETE_EXT;
    fb->_AllColorBuffersFixedPoint = !visual->floatMode;
    fb->_HasSNormOrFloatColorBuffer = visual->floatMode;
    fb->_HasAttachments = true;
    fb->FlipY = true;

    fb->SampleLocationTable = NULL;
    fb->ProgrammableSampleLocations = 0;
    fb->SampleLocationPixelGrid = 0;

    compute_depth_max(fb);
}

static struct gl_renderbuffer *
st_new_renderbuffer_fb(enum pipe_format format, unsigned samples, boolean sw)
{
    struct gl_renderbuffer *rb;

    rb = CALLOC_STRUCT(gl_renderbuffer);
    if (!rb) {
        _mesa_error(NULL, GL_OUT_OF_MEMORY, "creating renderbuffer");
        return NULL;
    }

    _mesa_init_renderbuffer(rb, 0);
    rb->ClassID = 0x4242; /* just a unique value */
    rb->NumSamples = samples;
    rb->NumStorageSamples = samples;
    rb->Format = st_pipe_format_to_mesa_format(format);
    rb->_BaseFormat = _mesa_get_format_base_format(rb->Format);
    rb->software = sw;

    switch (format) {
        case PIPE_FORMAT_B10G10R10A2_UNORM:
        case PIPE_FORMAT_R10G10B10A2_UNORM:
            rb->InternalFormat = GL_RGB10_A2;
            break;
        case PIPE_FORMAT_R10G10B10X2_UNORM:
        case PIPE_FORMAT_B10G10R10X2_UNORM:
            rb->InternalFormat = GL_RGB10;
            break;
        case PIPE_FORMAT_R8G8B8A8_UNORM:
        case PIPE_FORMAT_B8G8R8A8_UNORM:
        case PIPE_FORMAT_A8R8G8B8_UNORM:
            rb->InternalFormat = GL_RGBA8;
            break;
        case PIPE_FORMAT_R8G8B8X8_UNORM:
        case PIPE_FORMAT_B8G8R8X8_UNORM:
        case PIPE_FORMAT_X8R8G8B8_UNORM:
        case PIPE_FORMAT_R8G8B8_UNORM:
            rb->InternalFormat = GL_RGB8;
            break;
        case PIPE_FORMAT_R8G8B8A8_SRGB:
        case PIPE_FORMAT_B8G8R8A8_SRGB:
        case PIPE_FORMAT_A8R8G8B8_SRGB:
            rb->InternalFormat = GL_SRGB8_ALPHA8;
            break;
        case PIPE_FORMAT_R8G8B8X8_SRGB:
        case PIPE_FORMAT_B8G8R8X8_SRGB:
        case PIPE_FORMAT_X8R8G8B8_SRGB:
            rb->InternalFormat = GL_SRGB8;
            break;
        case PIPE_FORMAT_B5G5R5A1_UNORM:
            rb->InternalFormat = GL_RGB5_A1;
            break;
        case PIPE_FORMAT_B4G4R4A4_UNORM:
            rb->InternalFormat = GL_RGBA4;
            break;
        case PIPE_FORMAT_B5G6R5_UNORM:
            rb->InternalFormat = GL_RGB565;
            break;
        case PIPE_FORMAT_Z16_UNORM:
            rb->InternalFormat = GL_DEPTH_COMPONENT16;
            break;
        case PIPE_FORMAT_Z32_UNORM:
            rb->InternalFormat = GL_DEPTH_COMPONENT32;
            break;
        case PIPE_FORMAT_Z24_UNORM_S8_UINT:
        case PIPE_FORMAT_S8_UINT_Z24_UNORM:
            rb->InternalFormat = GL_DEPTH24_STENCIL8_EXT;
            break;
        case PIPE_FORMAT_Z24X8_UNORM:
        case PIPE_FORMAT_X8Z24_UNORM:
            rb->InternalFormat = GL_DEPTH_COMPONENT24;
            break;
        case PIPE_FORMAT_S8_UINT:
            rb->InternalFormat = GL_STENCIL_INDEX8_EXT;
            break;
        case PIPE_FORMAT_R16G16B16A16_SNORM:
            /* accum buffer */
            rb->InternalFormat = GL_RGBA16_SNORM;
            break;
        case PIPE_FORMAT_R16G16B16A16_UNORM:
            rb->InternalFormat = GL_RGBA16;
            break;
        case PIPE_FORMAT_R16G16B16_UNORM:
            rb->InternalFormat = GL_RGB16;
            break;
        case PIPE_FORMAT_R8_UNORM:
            rb->InternalFormat = GL_R8;
            break;
        case PIPE_FORMAT_R8G8_UNORM:
            rb->InternalFormat = GL_RG8;
            break;
        case PIPE_FORMAT_R16_UNORM:
            rb->InternalFormat = GL_R16;
            break;
        case PIPE_FORMAT_R16G16_UNORM:
            rb->InternalFormat = GL_RG16;
            break;
        case PIPE_FORMAT_R32G32B32A32_FLOAT:
            rb->InternalFormat = GL_RGBA32F;
            break;
        case PIPE_FORMAT_R32G32B32X32_FLOAT:
        case PIPE_FORMAT_R32G32B32_FLOAT:
            rb->InternalFormat = GL_RGB32F;
            break;
        case PIPE_FORMAT_R16G16B16A16_FLOAT:
            rb->InternalFormat = GL_RGBA16F;
            break;
        case PIPE_FORMAT_R16G16B16X16_FLOAT:
            rb->InternalFormat = GL_RGB16F;
            break;
        default:
            _mesa_problem(NULL,
                          "Unexpected format %s in st_new_renderbuffer_fb",
                          util_format_name(format));
            FREE(rb);
            return NULL;
    }

    rb->surface = NULL;

    return rb;
}


static bool
st_framebuffer_add_renderbuffer(struct st_framebuffer *stfb,
                                gl_buffer_index idx, bool prefer_srgb)
{
    struct gl_renderbuffer *rb;
    enum pipe_format format;
    bool sw;

    assert(_mesa_is_winsys_fbo(&stfb->Base));

    /* do not distinguish depth/stencil buffers */
    if (idx == BUFFER_STENCIL)
        idx = BUFFER_DEPTH;

    switch (idx) {
        case BUFFER_DEPTH:
            format = stfb->iface->visual->depth_stencil_format;
            sw = false;
            break;
        case BUFFER_ACCUM:
            format = stfb->iface->visual->accum_format;
            sw = true;
            break;
        default:
            format = stfb->iface->visual->color_format;
            if (prefer_srgb)
                format = util_format_srgb(format);
            sw = false;
            break;
    }

    if (format == PIPE_FORMAT_NONE)
        return false;

    rb = st_new_renderbuffer_fb(format, stfb->iface->visual->samples, sw);
    if (!rb)
        return false;

    if (idx != BUFFER_DEPTH) {
        _mesa_attach_and_own_rb(&stfb->Base, idx, rb);
        return true;
    }

    bool rb_ownership_taken = false;
    if (util_format_get_component_bits(format, UTIL_FORMAT_COLORSPACE_ZS, 0)) {
        _mesa_attach_and_own_rb(&stfb->Base, BUFFER_DEPTH, rb);
        rb_ownership_taken = true;
    }

    if (util_format_get_component_bits(format, UTIL_FORMAT_COLORSPACE_ZS, 1)) {
        if (rb_ownership_taken)
            _mesa_attach_and_reference_rb(&stfb->Base, BUFFER_STENCIL, rb);
        else
            _mesa_attach_and_own_rb(&stfb->Base, BUFFER_STENCIL, rb);
    }

    return true;
}

static inline enum st_attachment_type
buffer_index_to_attachment(gl_buffer_index index)
{
    enum st_attachment_type statt;

    switch (index) {
        case BUFFER_FRONT_LEFT:
            statt = ST_ATTACHMENT_FRONT_LEFT;
            break;
        case BUFFER_BACK_LEFT:
            statt = ST_ATTACHMENT_BACK_LEFT;
            break;
        case BUFFER_FRONT_RIGHT:
            statt = ST_ATTACHMENT_FRONT_RIGHT;
            break;
        case BUFFER_BACK_RIGHT:
            statt = ST_ATTACHMENT_BACK_RIGHT;
            break;
        case BUFFER_DEPTH:
            statt = ST_ATTACHMENT_DEPTH_STENCIL;
            break;
        case BUFFER_ACCUM:
            statt = ST_ATTACHMENT_ACCUM;
            break;
        default:
            statt = ST_ATTACHMENT_INVALID;
            break;
    }

    return statt;
}

static void
st_framebuffer_update_attachments(struct st_framebuffer *stfb)
{
    gl_buffer_index idx;

    stfb->num_statts = 0;

    for (enum st_attachment_type i = 0; i < ST_ATTACHMENT_COUNT; i++)
        stfb->statts[i] = ST_ATTACHMENT_INVALID;

    for (idx = 0; idx < BUFFER_COUNT; idx++) {
        struct st_renderbuffer *strb;
        enum st_attachment_type statt;

        strb = st_renderbuffer(stfb->Base.Attachment[idx].Renderbuffer);
        if (!strb || strb->software)
            continue;

        statt = buffer_index_to_attachment(idx);
        if (statt != ST_ATTACHMENT_INVALID &&
            st_visual_have_buffers(stfb->iface->visual, 1 << statt))
            stfb->statts[stfb->num_statts++] = statt;
    }
    stfb->stamp++;
}

static struct st_framebuffer *
st_framebuffer_create(struct st_context *st,
                      struct st_framebuffer_iface *stfbi)
{
    struct st_framebuffer *stfb;
    struct gl_config mode;
    gl_buffer_index idx;
    bool prefer_srgb = false;

    if (!stfbi)
        return NULL;

    stfb = CALLOC_STRUCT(st_framebuffer);
    if (!stfb)
        return NULL;

    st_visual_to_context_mode(stfbi->visual, &mode);

    /*
     * For desktop GL, sRGB framebuffer write is controlled by both the
     * capability of the framebuffer and GL_FRAMEBUFFER_SRGB.  We should
     * advertise the capability when the pipe driver (and core Mesa) supports
     * it so that applications can enable sRGB write when they want to.
     *
     * This is not to be confused with GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB.  When
     * the attribute is GLX_TRUE, it tells the st manager to pick a color
     * format such that util_format_srgb(visual->color_format) can be supported
     * by the pipe driver.  We still need to advertise the capability here.
     *
     * For GLES, however, sRGB framebuffer write is initially only controlled
     * by the capability of the framebuffer, with GL_EXT_sRGB_write_control
     * control is given back to the applications, but GL_FRAMEBUFFER_SRGB is
     * still enabled by default since this is the behaviour when
     * EXT_sRGB_write_control is not available. Since GL_EXT_sRGB_write_control
     * brings GLES on par with desktop GLs EXT_framebuffer_sRGB, in mesa this
     * is also expressed by using the same extension flag
     */
    if (_mesa_has_EXT_framebuffer_sRGB(st->ctx)) {
        struct pipe_screen *screen = st->screen;
        const enum pipe_format srgb_format =
                util_format_srgb(stfbi->visual->color_format);

        if (srgb_format != PIPE_FORMAT_NONE &&
            st_pipe_format_to_mesa_format(srgb_format) != MESA_FORMAT_NONE &&
            screen->is_format_supported(screen, srgb_format,
                                        PIPE_TEXTURE_2D, stfbi->visual->samples,
                                        stfbi->visual->samples,
                                        (PIPE_BIND_DISPLAY_TARGET |
                                         PIPE_BIND_RENDER_TARGET))) {
            mode.sRGBCapable = GL_TRUE;
            /* Since GL_FRAMEBUFFER_SRGB is enabled by default on GLES we must not
             * create renderbuffers with an sRGB format derived from the
             * visual->color_format, but we still want sRGB for desktop GL.
             */
            prefer_srgb = _mesa_is_desktop_gl(st->ctx);
        }
    }

    _mesa_initialize_window_framebuffer(&stfb->Base, &mode);

    stfb->iface = stfbi;
    stfb->iface_ID = stfbi->ID;
    stfb->iface_stamp = p_atomic_read(&stfbi->stamp) - 1;

    /* add the color buffer */
    idx = stfb->Base._ColorDrawBufferIndexes[0];
    if (!st_framebuffer_add_renderbuffer(stfb, idx, prefer_srgb)) {
        free(stfb);
        return NULL;
    }

    st_framebuffer_add_renderbuffer(stfb, BUFFER_DEPTH, false);
    st_framebuffer_add_renderbuffer(stfb, BUFFER_ACCUM, false);

    stfb->stamp = 0;
    st_framebuffer_update_attachments(stfb);

    return stfb;
}

static bool
st_framebuffer_iface_insert(struct st_manager *smapi,
                            struct st_framebuffer_iface *stfbi)
{
    struct st_manager_private *smPriv =
            (struct st_manager_private *)smapi->st_manager_private;
    struct hash_entry *entry;

    assert(smPriv);
    assert(smPriv->stfbi_ht);

    simple_mtx_lock(&smPriv->st_mutex);
    entry = _mesa_hash_table_insert(smPriv->stfbi_ht, stfbi, stfbi);
    simple_mtx_unlock(&smPriv->st_mutex);

    return entry != NULL;
}

static inline void list_add(struct list_head *item, struct list_head *list)
{
    item->prev = list;
    item->next = list->next;
    list->next->prev = item;
    list->next = item;
}

static struct st_framebuffer *
st_framebuffer_reuse_or_create(struct st_context *st,
                               struct gl_framebuffer *fb,
                               struct st_framebuffer_iface *stfbi)
{
    struct st_framebuffer *cur = NULL, *stfb = NULL;

    if (!stfbi)
        return NULL;

    /* Check if there is already a framebuffer object for the specified
     * framebuffer interface in this context. If there is one, use it.
     */
    LIST_FOR_EACH_ENTRY(cur, &st->winsys_buffers, head) {
        if (cur->iface_ID == stfbi->ID) {
            st_framebuffer_reference(&stfb, cur);
            break;
        }
    }

    /* If there is not already a framebuffer object, create one */
    if (stfb == NULL) {
        cur = st_framebuffer_create(st, stfbi);

        if (cur) {
            /* add the referenced framebuffer interface object to
             * the framebuffer interface object hash table.
             */
            if (!st_framebuffer_iface_insert(stfbi->state_manager, stfbi)) {
                st_framebuffer_reference(&cur, NULL);
                return NULL;
            }

            /* add to the context's winsys buffers list */
            list_add(&cur->head, &st->winsys_buffers);

            st_framebuffer_reference(&stfb, cur);
        }
    }

    return stfb;
}

static inline gl_buffer_index
attachment_to_buffer_index(enum st_attachment_type statt)
{
    gl_buffer_index index;

    switch (statt) {
        case ST_ATTACHMENT_FRONT_LEFT:
            index = BUFFER_FRONT_LEFT;
            break;
        case ST_ATTACHMENT_BACK_LEFT:
            index = BUFFER_BACK_LEFT;
            break;
        case ST_ATTACHMENT_FRONT_RIGHT:
            index = BUFFER_FRONT_RIGHT;
            break;
        case ST_ATTACHMENT_BACK_RIGHT:
            index = BUFFER_BACK_RIGHT;
            break;
        case ST_ATTACHMENT_DEPTH_STENCIL:
            index = BUFFER_DEPTH;
            break;
        case ST_ATTACHMENT_ACCUM:
            index = BUFFER_ACCUM;
            break;
        default:
            index = BUFFER_COUNT;
            break;
    }

    return index;
}

void
u_surface_default_template(struct pipe_surface *surf,
                           const struct pipe_resource *texture)
{
    memset(surf, 0, sizeof(*surf));

    surf->format = texture->format;
}

void
st_set_ws_renderbuffer_surface(struct st_renderbuffer *strb,
                               struct pipe_surface *surf)
{
    pipe_surface_reference(&strb->surface_srgb, NULL);
    pipe_surface_reference(&strb->surface_linear, NULL);

    if (util_format_is_srgb(surf->format))
        pipe_surface_reference(&strb->surface_srgb, surf);
    else
        pipe_surface_reference(&strb->surface_linear, surf);

    strb->surface = surf; /* just assign, don't ref */
    pipe_resource_reference(&strb->texture, surf->texture);

    strb->Base.Width = surf->width;
    strb->Base.Height = surf->height;
}

static void
st_framebuffer_validate(struct st_framebuffer *stfb,
                        struct st_context *st)
{
    struct pipe_resource *textures[ST_ATTACHMENT_COUNT];
    uint width, height;
    unsigned i;
    bool changed = false;
    int32_t new_stamp;

    new_stamp = p_atomic_read(&stfb->iface->stamp);
    if (stfb->iface_stamp == new_stamp)
        return;

    memset(textures, 0, stfb->num_statts * sizeof(textures[0]));

    /* validate the fb */
    do {
        if (!stfb->iface->validate(&st->iface, stfb->iface, stfb->statts,
                                   stfb->num_statts, textures))
            return;

        stfb->iface_stamp = new_stamp;
        new_stamp = p_atomic_read(&stfb->iface->stamp);
    } while(stfb->iface_stamp != new_stamp);

    width = stfb->Base.Width;
    height = stfb->Base.Height;

    for (i = 0; i < stfb->num_statts; i++) {
        struct st_renderbuffer *strb;
        struct pipe_surface *ps, surf_tmpl;
        gl_buffer_index idx;

        if (!textures[i])
            continue;

        idx = attachment_to_buffer_index(stfb->statts[i]);
        if (idx >= BUFFER_COUNT) {
            pipe_resource_reference(&textures[i], NULL);
            continue;
        }

        strb = st_renderbuffer(stfb->Base.Attachment[idx].Renderbuffer);
        assert(strb);
        if (strb->texture == textures[i]) {
            pipe_resource_reference(&textures[i], NULL);
            continue;
        }

        u_surface_default_template(&surf_tmpl, textures[i]);
        ps = st->pipe->create_surface(st->pipe, textures[i], &surf_tmpl);
        if (ps) {
            st_set_ws_renderbuffer_surface(strb, ps);
            pipe_surface_reference(&ps, NULL);

            changed = true;

            width = strb->Base.Width;
            height = strb->Base.Height;
        }

        pipe_resource_reference(&textures[i], NULL);
    }

    if (changed) {
        ++stfb->stamp;
        _mesa_resize_framebuffer(st->ctx, &stfb->Base, width, height);
    }
}

static void
delete_dummy_framebuffer(struct gl_framebuffer *fb)
{
    /* no op */
}

static struct gl_framebuffer IncompleteFramebuffer = {
        .Mutex = _SIMPLE_MTX_INITIALIZER_NP,
        .Delete = delete_dummy_framebuffer,
};

struct gl_framebuffer *
_mesa_get_incomplete_framebuffer(void)
{
    return &IncompleteFramebuffer;
}

static GLboolean
check_compatible(const struct gl_context *ctx,
                 const struct gl_framebuffer *buffer)
{
    const struct gl_config *ctxvis = &ctx->Visual;
    const struct gl_config *bufvis = &buffer->Visual;

    if (buffer == _mesa_get_incomplete_framebuffer())
        return GL_TRUE;

#define check_component(foo)           \
   if (ctxvis->foo && bufvis->foo &&   \
       ctxvis->foo != bufvis->foo)     \
      return GL_FALSE

    check_component(redShift);
    check_component(greenShift);
    check_component(blueShift);
    check_component(redBits);
    check_component(greenBits);
    check_component(blueBits);
    check_component(depthBits);
    check_component(stencilBits);

#undef check_component

    return GL_TRUE;
}

static inline GLboolean
_mesa_inside_begin_end(const struct gl_context *ctx)
{
    return ctx->Driver.CurrentExecPrimitive != PRIM_OUTSIDE_BEGIN_END;
}

static inline struct gl_context *
gl_context_from_vbo_exec(struct vbo_exec_context *exec)
{
    return container_of(exec, struct gl_context, vbo_context.exec);
}

unsigned
vbo_copy_vertices(struct gl_context *ctx,
                  GLenum mode,
                  unsigned start, unsigned *pcount, bool begin,
                  unsigned vertex_size,
                  bool in_dlist,
                  fi_type *dst,
                  const fi_type *src)
{
    const unsigned count = *pcount;
    unsigned copy = 0;

    switch (mode) {
        case GL_POINTS:
            return 0;
        case GL_LINES:
            copy = count % 2;
            break;
        case GL_TRIANGLES:
            copy = count % 3;
            break;
        case GL_QUADS:
        case GL_LINES_ADJACENCY:
            copy = count % 4;
            break;
        case GL_TRIANGLES_ADJACENCY:
            copy = count % 6;
            break;
        case GL_LINE_STRIP:
            copy = MIN2(1, count);
            break;
        case GL_LINE_STRIP_ADJACENCY:
            /* We need to copy 3 vertices, because:
             *    Last strip:  ---o---o---x     (last line)
             *    Next strip:     x---o---o---  (next line)
             */
            copy = MIN2(3, count);
            break;
        case GL_PATCHES:
            if (in_dlist) {
                /* We don't know the value of GL_PATCH_VERTICES when compiling
                 * a display list.
                 *
                 * Fail an assertion in debug builds and use the value of 3
                 * in release builds, which is more likely than any other value.
                 */
                assert(!"patch_vertices is unknown");
                copy = count % 3;
            } else {
                copy = count % ctx->TessCtrlProgram.patch_vertices;
            }
            break;
        case GL_LINE_LOOP:
            if (!in_dlist && begin == 0) {
                /* We're dealing with the second or later section of a split/wrapped
                 * GL_LINE_LOOP.  Since we're converting line loops to line strips,
                 * we've already incremented the last_prim->start counter by one to
                 * skip the 0th vertex in the loop.  We need to undo that (effectively
                 * subtract one from last_prim->start) so that we copy the 0th vertex
                 * to the next vertex buffer.
                 */
                assert(start > 0);
                src -= vertex_size;
            }
            FALLTHROUGH;
        case GL_TRIANGLE_FAN:
        case GL_POLYGON:
            if (count == 0) {
                return 0;
            } else if (count == 1) {
                memcpy(dst, src + 0, vertex_size * sizeof(GLfloat));
                return 1;
            } else {
                memcpy(dst, src + 0, vertex_size * sizeof(GLfloat));
                memcpy(dst + vertex_size, src + (count - 1) * vertex_size,
                       vertex_size * sizeof(GLfloat));
                return 2;
            }
        case GL_TRIANGLE_STRIP:
            /* Draw an even number of triangles to keep front/back facing the same. */
            *pcount -= count % 2;
            FALLTHROUGH;
        case GL_QUAD_STRIP:
            if (count <= 1)
                copy = count;
            else
                copy = 2 + (count % 2);
            break;
        case PRIM_OUTSIDE_BEGIN_END:
            return 0;
        case GL_TRIANGLE_STRIP_ADJACENCY:
            /* TODO: Splitting tri strips with adjacency is too complicated. */
        default:
            unreachable("Unexpected primitive type");
            return 0;
    }

    memcpy(dst, src + (count - copy) * vertex_size,
           copy * vertex_size * sizeof(GLfloat));
    return copy;
}

static GLuint
vbo_exec_copy_vertices(struct vbo_exec_context *exec)
{
    struct gl_context *ctx = gl_context_from_vbo_exec(exec);
    const GLuint sz = exec->vtx.vertex_size;
    fi_type *dst = exec->vtx.copied.buffer;
    unsigned last = exec->vtx.prim_count - 1;
    unsigned start = exec->vtx.draw[last].start;
    const fi_type *src = exec->vtx.buffer_map + start * sz;

    return vbo_copy_vertices(ctx, ctx->Driver.CurrentExecPrimitive,
                             start,
                             &exec->vtx.draw[last].count,
                             exec->vtx.markers[last].begin,
                             sz, false, dst, src);
}

static void
vbo_exec_bind_arrays(struct gl_context *ctx)
{
    struct vbo_context *vbo = vbo_context(ctx);
    struct gl_vertex_array_object *vao = vbo->VAO;
    struct vbo_exec_context *exec = &vbo->exec;

    GLintptr buffer_offset;
    if (exec->vtx.bufferobj) {
        assert(exec->vtx.bufferobj->Mappings[MAP_INTERNAL].Pointer);
        buffer_offset = exec->vtx.bufferobj->Mappings[MAP_INTERNAL].Offset +
                        exec->vtx.buffer_offset;
    } else {
        /* Ptr into ordinary app memory */
        buffer_offset = (GLbyte *)exec->vtx.buffer_map - (GLbyte *)NULL;
    }

    const gl_vertex_processing_mode mode = ctx->VertexProgram._VPMode;

    /* Compute the bitmasks of vao_enabled arrays */
    GLbitfield vao_enabled = _vbo_get_vao_enabled_from_vbo(mode, exec->vtx.enabled);

    /* At first disable arrays no longer needed */
    _mesa_disable_vertex_array_attribs(ctx, vao, VERT_BIT_ALL & ~vao_enabled);
    assert((~vao_enabled & vao->Enabled) == 0);

    /* Bind the buffer object */
    const GLuint stride = exec->vtx.vertex_size*sizeof(GLfloat);
    _mesa_bind_vertex_buffer(ctx, vao, 0, exec->vtx.bufferobj, buffer_offset,
                             stride, false, false);

    /* Retrieve the mapping from VBO_ATTRIB to VERT_ATTRIB space
     * Note that the position/generic0 aliasing is done in the VAO.
     */
    const GLubyte *const vao_to_vbo_map = _vbo_attribute_alias_map[mode];
    /* Now set the enabled arrays */
    GLbitfield mask = vao_enabled;
    while (mask) {
        const int vao_attr = u_bit_scan(&mask);
        const GLubyte vbo_attr = vao_to_vbo_map[vao_attr];

        const GLubyte size = exec->vtx.attr[vbo_attr].size;
        const GLenum16 type = exec->vtx.attr[vbo_attr].type;
        const GLuint offset = (GLuint)((GLbyte *)exec->vtx.attrptr[vbo_attr] -
                                       (GLbyte *)exec->vtx.vertex);
        assert(offset <= ctx->Const.MaxVertexAttribRelativeOffset);

        /* Set and enable */
        _vbo_set_attrib_format(ctx, vao, vao_attr, buffer_offset,
                               size, type, offset);

        /* The vao is initially created with all bindings set to 0. */
        assert(vao->VertexAttrib[vao_attr].BufferBindingIndex == 0);
    }
    _mesa_enable_vertex_array_attribs(ctx, vao, vao_enabled);
    assert(vao_enabled == vao->Enabled);
    assert(!exec->vtx.bufferobj ||
           (vao_enabled & ~vao->VertexAttribBufferMask) == 0);

    _mesa_set_draw_vao(ctx, vao, _vbo_get_vao_filter(mode));
}

void
vbo_exec_vtx_flush(struct vbo_exec_context *exec)
{
    struct gl_context *ctx = gl_context_from_vbo_exec(exec);

    /* Only unmap if persistent mappings are unsupported. */
    bool persistent_mapping = ctx->Extensions.ARB_buffer_storage &&
                              exec->vtx.bufferobj &&
                              exec->vtx.buffer_map;

    if (exec->vtx.prim_count &&
        exec->vtx.vert_count) {

        exec->vtx.copied.nr = vbo_exec_copy_vertices(exec);

        if (exec->vtx.copied.nr != exec->vtx.vert_count) {
            /* Prepare and set the exec draws internal VAO for drawing. */
            vbo_exec_bind_arrays(ctx);

            if (ctx->NewState)
                _mesa_update_state(ctx);

            if (!persistent_mapping)
                vbo_exec_vtx_unmap(exec);

            assert(ctx->NewState == 0);

            if (0)
                printf("%s %d %d\n", __func__, exec->vtx.prim_count,
                       exec->vtx.vert_count);

            exec->vtx.info.vertices_per_patch =
                    ctx->TessCtrlProgram.patch_vertices;

            ctx->Driver.DrawGalliumComplex(ctx, &exec->vtx.info,
                                           exec->vtx.draw,
                                           exec->vtx.mode,
                                           NULL,
                                           exec->vtx.prim_count);

            /* Get new storage -- unless asked not to. */
            if (!persistent_mapping)
                vbo_exec_vtx_map(exec);
        }
    }

    if (persistent_mapping) {
        exec->vtx.buffer_used += (exec->vtx.buffer_ptr - exec->vtx.buffer_map) *
                                 sizeof(float);
        exec->vtx.buffer_map = exec->vtx.buffer_ptr;

        /* Set the buffer offset for the next draw. */
        exec->vtx.buffer_offset = exec->vtx.buffer_used;

        if (!vbo_exec_buffer_has_space(exec)) {
            /* This will allocate a new buffer. */
            vbo_exec_vtx_unmap(exec);
            vbo_exec_vtx_map(exec);
        }
    }

    if (exec->vtx.vertex_size == 0)
        exec->vtx.max_vert = 0;
    else
        exec->vtx.max_vert = vbo_compute_max_verts(exec);

    exec->vtx.buffer_ptr = exec->vtx.buffer_map;
    exec->vtx.prim_count = 0;
    exec->vtx.vert_count = 0;
}

static void
vbo_exec_FlushVertices_internal(struct vbo_exec_context *exec, unsigned flags)
{
    struct gl_context *ctx = gl_context_from_vbo_exec(exec);

    if (flags & FLUSH_STORED_VERTICES) {
        if (exec->vtx.vert_count) {
            vbo_exec_vtx_flush(exec);
        }

        if (exec->vtx.vertex_size) {
            vbo_exec_copy_to_current(exec);
            vbo_reset_all_attr(exec);
        }

        /* All done. */
        ctx->Driver.NeedFlush = 0;
    } else {
        assert(flags == FLUSH_UPDATE_CURRENT);

        /* Note that the vertex size is unchanged.
         * (vbo_reset_all_attr isn't called)
         */
        vbo_exec_copy_to_current(exec);

        /* Only FLUSH_UPDATE_CURRENT is done. */
        ctx->Driver.NeedFlush = ~FLUSH_UPDATE_CURRENT;
    }
}

void
vbo_exec_FlushVertices(struct gl_context *ctx, GLuint flags)
{
    struct vbo_exec_context *exec = &vbo_context(ctx)->exec;

#ifndef NDEBUG
    /* debug check: make sure we don't get called recursively */
    exec->flush_call_depth++;
    assert(exec->flush_call_depth == 1);
#endif

    if (_mesa_inside_begin_end(ctx)) {
        /* We've had glBegin but not glEnd! */
#ifndef NDEBUG
        exec->flush_call_depth--;
        assert(exec->flush_call_depth == 0);
#endif
        return;
    }

    /* Flush (draw). */
    vbo_exec_FlushVertices_internal(exec, flags);

#ifndef NDEBUG
    exec->flush_call_depth--;
    assert(exec->flush_call_depth == 0);
#endif
}

void
_mesa_flush(struct gl_context *ctx)
{
    FLUSH_VERTICES(ctx, 0, 0);
    if (ctx->Driver.Flush) {
        ctx->Driver.Flush(ctx);
    }
}

GLboolean
_mesa_make_current( struct gl_context *newCtx,
                    struct gl_framebuffer *drawBuffer,
                    struct gl_framebuffer *readBuffer )
{
    GET_CURRENT_CONTEXT(curCtx);

    if (MESA_VERBOSE & VERBOSE_API)
        _mesa_debug(newCtx, "_mesa_make_current()\n");

    /* Check that the context's and framebuffer's visuals are compatible.
     */
    if (newCtx && drawBuffer && newCtx->WinSysDrawBuffer != drawBuffer) {
        if (!check_compatible(newCtx, drawBuffer)) {
            _mesa_warning(newCtx,
                          "MakeCurrent: incompatible visuals for context and drawbuffer");
            return GL_FALSE;
        }
    }
    if (newCtx && readBuffer && newCtx->WinSysReadBuffer != readBuffer) {
        if (!check_compatible(newCtx, readBuffer)) {
            _mesa_warning(newCtx,
                          "MakeCurrent: incompatible visuals for context and readbuffer");
            return GL_FALSE;
        }
    }

    if (curCtx &&
        (curCtx->WinSysDrawBuffer || curCtx->WinSysReadBuffer) &&
        /* make sure this context is valid for flushing */
        curCtx != newCtx &&
        curCtx->Const.ContextReleaseBehavior ==
        GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH) {
        _mesa_flush(curCtx);
    }

    /* Call this periodically to detect when the user has begun using
     * GL rendering from multiple threads.
     */
    _glapi_check_multithread();

    if (!newCtx) {
        _glapi_set_dispatch(NULL);  /* none current */
        /* We need old ctx to correctly release Draw/ReadBuffer
         * and avoid a surface leak in st_renderbuffer_delete.
         * Therefore, first drop buffers then set new ctx to NULL.
         */
        if (curCtx) {
            _mesa_reference_framebuffer(&curCtx->WinSysDrawBuffer, NULL);
            _mesa_reference_framebuffer(&curCtx->WinSysReadBuffer, NULL);
        }
        _glapi_set_context(NULL);
        assert(_mesa_get_current_context() == NULL);
    }
    else {
        _glapi_set_context((void *) newCtx);
        assert(_mesa_get_current_context() == newCtx);
        _glapi_set_dispatch(newCtx->CurrentClientDispatch);

        if (drawBuffer && readBuffer) {
            assert(_mesa_is_winsys_fbo(drawBuffer));
            assert(_mesa_is_winsys_fbo(readBuffer));
            _mesa_reference_framebuffer(&newCtx->WinSysDrawBuffer, drawBuffer);
            _mesa_reference_framebuffer(&newCtx->WinSysReadBuffer, readBuffer);

            /*
             * Only set the context's Draw/ReadBuffer fields if they're NULL
             * or not bound to a user-created FBO.
             */
            if (!newCtx->DrawBuffer || _mesa_is_winsys_fbo(newCtx->DrawBuffer)) {
                _mesa_reference_framebuffer(&newCtx->DrawBuffer, drawBuffer);
                /* Update the FBO's list of drawbuffers/renderbuffers.
                 * For winsys FBOs this comes from the GL state (which may have
                 * changed since the last time this FBO was bound).
                 */
                _mesa_update_draw_buffers(newCtx);
                _mesa_update_allow_draw_out_of_order(newCtx);
                _mesa_update_valid_to_render_state(newCtx);
            }
            if (!newCtx->ReadBuffer || _mesa_is_winsys_fbo(newCtx->ReadBuffer)) {
                _mesa_reference_framebuffer(&newCtx->ReadBuffer, readBuffer);
                /* In _mesa_initialize_window_framebuffer, for single-buffered
                 * visuals, the ColorReadBuffer is set to be GL_FRONT, even with
                 * GLES contexts. When calling read_buffer, we verify we are reading
                 * from GL_BACK in is_legal_es3_readbuffer_enum.  But the default is
                 * incorrect, and certain dEQP tests check this.  So fix it here.
                 */
                if (_mesa_is_gles(newCtx) &&
                    !newCtx->ReadBuffer->Visual.doubleBufferMode)
                    if (newCtx->ReadBuffer->ColorReadBuffer == GL_FRONT)
                        newCtx->ReadBuffer->ColorReadBuffer = GL_BACK;
            }

            /* XXX only set this flag if we're really changing the draw/read
             * framebuffer bindings.
             */
            newCtx->NewState |= _NEW_BUFFERS;

            check_init_viewport(newCtx, drawBuffer->Width, drawBuffer->Height);
        }

        if (newCtx->FirstTimeCurrent) {
            handle_first_current(newCtx);
            newCtx->FirstTimeCurrent = GL_FALSE;
        }
    }

    return GL_TRUE;
}

static bool
st_api_make_current(struct st_api *stapi, struct st_context_iface *stctxi,
                    struct st_framebuffer_iface *stdrawi,
                    struct st_framebuffer_iface *streadi)
{
    struct st_context *st = (struct st_context *) stctxi;
    struct gl_framebuffer *stdraw, *stread;
    bool ret;

    if (st) {
        /* reuse or create the draw fb */
        stdraw = st_framebuffer_reuse_or_create(st,
                                                st->ctx->WinSysDrawBuffer, stdrawi);
        if (streadi != stdrawi) {
            /* do the same for the read fb */
            stread = st_framebuffer_reuse_or_create(st,
                                                    st->ctx->WinSysReadBuffer, streadi);
        }
        else {
            stread = NULL;
            /* reuse the draw fb for the read fb */
            if (stdraw)
                _mesa_reference_framebuffer(&stread, stdraw);
        }

        /* If framebuffers were asked for, we'd better have allocated them */
        if ((stdrawi && !stdraw) || (streadi && !stread))
            return false;

        if (stdraw && stread) {
            st_framebuffer_validate(stdraw, st);
            if (stread != stdraw)
                st_framebuffer_validate(stread, st);

            ret = _mesa_make_current(st->ctx, stdraw, stread);

            st->draw_stamp = stdraw->stamp - 1;
            st->read_stamp = stread->stamp - 1;
            st_context_validate(st, stdraw, stread);
        }
        else {
            struct gl_framebuffer *incomplete = _mesa_get_incomplete_framebuffer();
            ret = _mesa_make_current(st->ctx, incomplete, incomplete);
        }

        _mesa_reference_framebuffer(&stdraw, NULL);
        _mesa_reference_framebuffer(&stread, NULL);

        /* Purge the context's winsys_buffers list in case any
         * of the referenced drawables no longer exist.
         */
        st_framebuffers_purge(st);
    }
    else {
        GET_CURRENT_CONTEXT(ctx);

        if (ctx) {
            /* Before releasing the context, release its associated
             * winsys buffers first. Then purge the context's winsys buffers list
             * to free the resources of any winsys buffers that no longer have
             * an existing drawable.
             */
            ret = _mesa_make_current(ctx, NULL, NULL);
            st_framebuffers_purge(ctx->st);
        }

        ret = _mesa_make_current(NULL, NULL, NULL);
    }

    return ret;
}


static const struct st_api st_gl_api = {
        .name = "Mesa " PACKAGE_VERSION,
        .api = ST_API_OPENGL,
        .profile_mask = ST_PROFILE_DEFAULT_MASK |
                        ST_PROFILE_OPENGL_CORE_MASK |
                        ST_PROFILE_OPENGL_ES1_MASK |
                        ST_PROFILE_OPENGL_ES2_MASK |
                        0,
        .feature_mask = ST_API_FEATURE_MS_VISUALS_MASK,
        .destroy = st_api_destroy,
        .query_versions = st_api_query_versions,
        .create_context = st_api_create_context,
        .make_current = st_api_make_current,
        .get_current = st_api_get_current,
        .destroy_drawable = st_api_destroy_drawable,
};

static inline struct st_context *
st_context(struct gl_context *ctx)
{
    return ctx->st;
}

static inline struct st_renderbuffer *
st_renderbuffer(struct gl_renderbuffer *rb)
{
    return (struct st_renderbuffer *) rb;
}

enum pipe_format
st_choose_format(struct st_context *st, GLenum internalFormat,
                 GLenum format, GLenum type,
                 enum pipe_texture_target target, unsigned sample_count,
                 unsigned storage_sample_count,
                 unsigned bindings, bool swap_bytes, bool allow_dxt)
{
    struct pipe_screen *screen = st->screen;
    unsigned i;
    int j;
    enum pipe_format pf;

    /* can't render to compressed formats at this time */
    if (_mesa_is_compressed_format(st->ctx, internalFormat)
        && (bindings & ~PIPE_BIND_SAMPLER_VIEW)) {
        return PIPE_FORMAT_NONE;
    }

    /* If we have an unsized internalFormat, and the driver supports a format
     * that exactly matches format/type such that we can just memcpy, pick that
     * (unless the format wouldn't still be unorm, which is the expectation for
     * unsized formats).
     */
    if (_mesa_is_enum_format_unsized(internalFormat) && format != 0 &&
        _mesa_is_type_unsigned(type)) {
        pf = st_choose_matching_format(st, bindings, format, type,
                                       swap_bytes);

        if (pf != PIPE_FORMAT_NONE &&
            (!bindings || screen->is_format_supported(screen, pf, target, sample_count,
                                                      storage_sample_count, bindings)) &&
            _mesa_get_format_base_format(st_pipe_format_to_mesa_format(pf)) ==
            internalFormat) {
            goto success;
        }
    }

    /* For an unsized GL_RGB but a 2_10_10_10 type, try to pick one of the
     * 2_10_10_10 formats.  This is important for
     * GL_EXT_texture_type_2_10_10_10_EXT support, which says that these
     * formats are not color-renderable.  Mesa's check for making those
     * non-color-renderable is based on our chosen format being 2101010.
     */
    if (type == GL_UNSIGNED_INT_2_10_10_10_REV) {
        if (internalFormat == GL_RGB)
            internalFormat = GL_RGB10;
        else if (internalFormat == GL_RGBA)
            internalFormat = GL_RGB10_A2;
    }

    if (type == GL_UNSIGNED_SHORT_5_5_5_1) {
        if (internalFormat == GL_RGB)
            internalFormat = GL_RGB5;
        else if (internalFormat == GL_RGBA)
            internalFormat = GL_RGB5_A1;
    }

    /* search table for internalFormat */
    for (i = 0; i < ARRAY_SIZE(format_map); i++) {
        const struct format_mapping *mapping = &format_map[i];
        for (j = 0; mapping->glFormats[j]; j++) {
            if (mapping->glFormats[j] == internalFormat) {
                /* Found the desired internal format.  Find first pipe format
                 * which is supported by the driver.
                 */
                pf = find_supported_format(screen, mapping->pipeFormats,
                                           target, sample_count,
                                           storage_sample_count, bindings,
                                           allow_dxt);
                goto success;
            }
        }
    }

    _mesa_problem(NULL, "unhandled format!\n");
    return PIPE_FORMAT_NONE;

    success:
    if (0) {
        debug_printf("%s(fmt=%s, type=%s, intFmt=%s) = %s\n",
                     __FUNCTION__,
                     _mesa_enum_to_string(format),
                     _mesa_enum_to_string(type),
                     _mesa_enum_to_string(internalFormat),
                     util_format_name(pf));
    }
    return pf;
}