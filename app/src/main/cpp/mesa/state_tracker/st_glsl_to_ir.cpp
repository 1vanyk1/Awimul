#include "../compiler/glsl/glsl_parser_extras.h"
#include "../compiler/glsl/ir_optimization.h"
#include "../compiler/glsl/program.h"

#include "st_nir.h"
#include "st_shader_cache.h"
#include "st_glsl_to_tgsi.h"
#include "st_program.h"

#include "../gallium/tgsi/tgsi_from_mesa.h"

extern "C" {

/**
 * Link a shader.
 * Called via ctx->Driver.LinkShader()
 * This is a shared function that branches off to either GLSL IR -> TGSI or
 * GLSL IR -> NIR
 */
GLboolean
st_link_shader(struct gl_context *ctx, struct gl_shader_program *prog)
{
    GLboolean ret;
    struct st_context *sctx = st_context(ctx);
    struct pipe_context *pctx = sctx->pipe;
    struct pipe_screen *pscreen = sctx->screen;

    enum pipe_shader_ir preferred_ir = (enum pipe_shader_ir)
            pscreen->get_shader_param(pscreen, PIPE_SHADER_VERTEX,
                                      PIPE_SHADER_CAP_PREFERRED_IR);
    bool use_nir = preferred_ir == PIPE_SHADER_IR_NIR;

    /* Return early if we are loading the shader from on-disk cache */
    if (st_load_ir_from_disk_cache(ctx, prog, use_nir)) {
        return GL_TRUE;
    }

    assert(prog->data->LinkStatus);

    /* Skip the GLSL steps when using SPIR-V. */
    if (prog->data->spirv) {
        assert(use_nir);
        return st_link_nir(ctx, prog);
    }

    for (unsigned i = 0; i < MESA_SHADER_STAGES; i++) {
        if (prog->_LinkedShaders[i] == NULL)
            continue;

        struct gl_linked_shader *shader = prog->_LinkedShaders[i];
        exec_list *ir = shader->ir;
        gl_shader_stage stage = shader->Stage;
        const struct gl_shader_compiler_options *options =
                &ctx->Const.ShaderCompilerOptions[stage];

        /* If there are forms of indirect addressing that the driver
         * cannot handle, perform the lowering pass.
         */
        if (options->EmitNoIndirectInput || options->EmitNoIndirectOutput ||
            options->EmitNoIndirectTemp || options->EmitNoIndirectUniform) {
            lower_variable_index_to_cond_assign(stage, ir,
                                                options->EmitNoIndirectInput,
                                                options->EmitNoIndirectOutput,
                                                options->EmitNoIndirectTemp,
                                                options->EmitNoIndirectUniform);
        }

        enum pipe_shader_type ptarget = pipe_shader_type_from_mesa(stage);
        bool have_dround = pscreen->get_shader_param(pscreen, ptarget,
                                                     PIPE_SHADER_CAP_TGSI_DROUND_SUPPORTED);
        bool have_dfrexp = pscreen->get_shader_param(pscreen, ptarget,
                                                     PIPE_SHADER_CAP_TGSI_DFRACEXP_DLDEXP_SUPPORTED);
        bool have_ldexp = pscreen->get_shader_param(pscreen, ptarget,
                                                    PIPE_SHADER_CAP_TGSI_LDEXP_SUPPORTED);

        if (!pscreen->get_param(pscreen, PIPE_CAP_INT64_DIVMOD))
            lower_64bit_integer_instructions(ir, DIV64 | MOD64);

        if (ctx->Extensions.ARB_shading_language_packing) {
            unsigned lower_inst = LOWER_PACK_SNORM_2x16 |
                                  LOWER_UNPACK_SNORM_2x16 |
                                  LOWER_PACK_UNORM_2x16 |
                                  LOWER_UNPACK_UNORM_2x16 |
                                  LOWER_PACK_SNORM_4x8 |
                                  LOWER_UNPACK_SNORM_4x8 |
                                  LOWER_UNPACK_UNORM_4x8 |
                                  LOWER_PACK_UNORM_4x8;

            if (ctx->Extensions.ARB_gpu_shader5)
                lower_inst |= LOWER_PACK_USE_BFI |
                              LOWER_PACK_USE_BFE;
            if (!ctx->st->has_half_float_packing)
                lower_inst |= LOWER_PACK_HALF_2x16 |
                              LOWER_UNPACK_HALF_2x16;

            lower_packing_builtins(ir, lower_inst);
        }

        if (!pscreen->get_param(pscreen, PIPE_CAP_TEXTURE_GATHER_OFFSETS))
            lower_offset_arrays(ir);
        do_mat_op_to_vec(ir);

        if (stage == MESA_SHADER_FRAGMENT && pscreen->get_param(pscreen, PIPE_CAP_FBFETCH))
            lower_blend_equation_advanced(
                    shader, ctx->Extensions.KHR_blend_equation_advanced_coherent);

        lower_instructions(ir,
                           (use_nir ? 0 : MOD_TO_FLOOR) |
                           FDIV_TO_MUL_RCP |
                           EXP_TO_EXP2 |
                           LOG_TO_LOG2 |
                           MUL64_TO_MUL_AND_MUL_HIGH |
                           (have_ldexp ? 0 : LDEXP_TO_ARITH) |
                           (have_dfrexp ? 0 : DFREXP_DLDEXP_TO_ARITH) |
                           CARRY_TO_ARITH |
                           BORROW_TO_ARITH |
                           (have_dround ? 0 : DOPS_TO_DFRAC) |
                           (options->EmitNoPow ? POW_TO_EXP2 : 0) |
                           (!ctx->Const.NativeIntegers ? INT_DIV_TO_MUL_RCP : 0) |
                           (options->EmitNoSat ? SAT_TO_CLAMP : 0) |
                           (ctx->Const.ForceGLSLAbsSqrt ? SQRT_TO_ABS_SQRT : 0) |
                           /* Assume that if ARB_gpu_shader5 is not supported
                            * then all of the extended integer functions need
                            * lowering.  It may be necessary to add some caps
                            * for individual instructions.
                            */
                           (!ctx->Extensions.ARB_gpu_shader5
                            ? BIT_COUNT_TO_MATH |
                              EXTRACT_TO_SHIFTS |
                              INSERT_TO_SHIFTS |
                              REVERSE_TO_SHIFTS |
                              FIND_LSB_TO_FLOAT_CAST |
                              FIND_MSB_TO_FLOAT_CAST |
                              IMUL_HIGH_TO_MUL
                            : 0));

        do_vec_index_to_cond_assign(ir);
        lower_vector_insert(ir, true);
        lower_quadop_vector(ir, false);
        if (options->MaxIfDepth == 0) {
            lower_discard(ir);
        }

        validate_ir_tree(ir);
    }

    build_program_resource_list(&ctx->Const, prog, use_nir);

    if (use_nir)
        ret = st_link_nir(ctx, prog);
    else
        ret = st_link_tgsi(ctx, prog);

    if (pctx->link_shader) {
        void *driver_handles[PIPE_SHADER_TYPES];
        memset(driver_handles, 0, sizeof(driver_handles));

        for (uint32_t i = 0; i < MESA_SHADER_STAGES; ++i) {
            struct gl_linked_shader *shader = prog->_LinkedShaders[i];
            if (shader) {
                struct st_program *stp = st_program(shader->Program);
                if (stp && stp->variants) {
                    enum pipe_shader_type type = pipe_shader_type_from_mesa(shader->Stage);
                    driver_handles[type] = stp->variants->driver_shader;
                }
            }
        }

        pctx->link_shader(pctx, driver_handles);
    }

    return ret;
}

} /* extern "C" */
