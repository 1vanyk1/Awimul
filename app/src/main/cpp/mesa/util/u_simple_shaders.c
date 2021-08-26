#include "../include/pipe/p_context.h"
#include "../include/pipe/p_shader_tokens.h"
#include "../include/pipe/p_state.h"
#include "u_simple_shaders.h"
#include "u_debug.h"
#include "u_memory.h"
#include "u_string.h"
#include "../gallium/tgsi/tgsi_dump.h"
#include "../gallium/tgsi/tgsi_strings.h"
#include "../gallium/tgsi/tgsi_ureg.h"
#include "../gallium/tgsi/tgsi_text.h"
#include <stdio.h> /* include last */



/**
 * Make simple vertex pass-through shader.
 * \param num_attribs  number of attributes to pass through
 * \param semantic_names  array of semantic names for each attribute
 * \param semantic_indexes  array of semantic indexes for each attribute
 */
void *
util_make_vertex_passthrough_shader(struct pipe_context *pipe,
                                    uint num_attribs,
                                    const enum tgsi_semantic *semantic_names,
                                    const uint *semantic_indexes,
                                    bool window_space)
{
    return util_make_vertex_passthrough_shader_with_so(pipe, num_attribs,
                                                       semantic_names,
                                                       semantic_indexes,
                                                       window_space, false, NULL);
}

void *
util_make_vertex_passthrough_shader_with_so(struct pipe_context *pipe,
                                            uint num_attribs,
                                            const enum tgsi_semantic *semantic_names,
                                            const uint *semantic_indexes,
                                            bool window_space, bool layered,
                                            const struct pipe_stream_output_info *so)
{
    struct ureg_program *ureg;
    uint i;

    ureg = ureg_create( PIPE_SHADER_VERTEX );
    if (!ureg)
        return NULL;

    if (window_space)
        ureg_property(ureg, TGSI_PROPERTY_VS_WINDOW_SPACE_POSITION, TRUE);

    for (i = 0; i < num_attribs; i++) {
        struct ureg_src src;
        struct ureg_dst dst;

        src = ureg_DECL_vs_input( ureg, i );

        dst = ureg_DECL_output( ureg,
                                semantic_names[i],
                                semantic_indexes[i]);

        ureg_MOV( ureg, dst, src );
    }

    if (layered) {
        struct ureg_src instance_id =
                ureg_DECL_system_value(ureg, TGSI_SEMANTIC_INSTANCEID, 0);
        struct ureg_dst layer = ureg_DECL_output(ureg, TGSI_SEMANTIC_LAYER, 0);

        ureg_MOV(ureg, ureg_writemask(layer, TGSI_WRITEMASK_X),
                 ureg_scalar(instance_id, TGSI_SWIZZLE_X));
    }

    ureg_END( ureg );

    return ureg_create_shader_with_so_and_destroy( ureg, pipe, so );
}


void *util_make_layered_clear_vertex_shader(struct pipe_context *pipe)
{
    const enum tgsi_semantic semantic_names[] = {TGSI_SEMANTIC_POSITION,
                                                 TGSI_SEMANTIC_GENERIC};
    const unsigned semantic_indices[] = {0, 0};

    return util_make_vertex_passthrough_shader_with_so(pipe, 2, semantic_names,
                                                       semantic_indices, false,
                                                       true, NULL);
}

/**
 * Takes position and color, and outputs position, color, and instance id.
 */
void *util_make_layered_clear_helper_vertex_shader(struct pipe_context *pipe)
{
    static const char text[] =
            "VERT\n"
            "DCL IN[0]\n"
            "DCL IN[1]\n"
            "DCL SV[0], INSTANCEID\n"
            "DCL OUT[0], POSITION\n"
            "DCL OUT[1], GENERIC[0]\n"
            "DCL OUT[2], GENERIC[1]\n"

            "MOV OUT[0], IN[0]\n"
            "MOV OUT[1], IN[1]\n"
            "MOV OUT[2].x, SV[0].xxxx\n"
            "END\n";
    struct tgsi_token tokens[1000];
    struct pipe_shader_state state = {0};

    if (!tgsi_text_translate(text, tokens, ARRAY_SIZE(tokens))) {
        assert(0);
        return NULL;
    }
    pipe_shader_state_from_tgsi(&state, tokens);
    return pipe->create_vs_state(pipe, &state);
}

/**
 * Takes position, color, and target layer, and emits vertices on that target
 * layer, with the specified color.
 */
void *util_make_layered_clear_geometry_shader(struct pipe_context *pipe)
{
    static const char text[] =
            "GEOM\n"
            "PROPERTY GS_INPUT_PRIMITIVE TRIANGLES\n"
            "PROPERTY GS_OUTPUT_PRIMITIVE TRIANGLE_STRIP\n"
            "PROPERTY GS_MAX_OUTPUT_VERTICES 3\n"
            "PROPERTY GS_INVOCATIONS 1\n"
            "DCL IN[][0], POSITION\n" /* position */
            "DCL IN[][1], GENERIC[0]\n" /* color */
            "DCL IN[][2], GENERIC[1]\n" /* vs invocation */
            "DCL OUT[0], POSITION\n"
            "DCL OUT[1], GENERIC[0]\n"
            "DCL OUT[2], LAYER\n"
            "IMM[0] INT32 {0, 0, 0, 0}\n"

            "MOV OUT[0], IN[0][0]\n"
            "MOV OUT[1], IN[0][1]\n"
            "MOV OUT[2].x, IN[0][2].xxxx\n"
            "EMIT IMM[0].xxxx\n"
            "MOV OUT[0], IN[1][0]\n"
            "MOV OUT[1], IN[1][1]\n"
            "MOV OUT[2].x, IN[1][2].xxxx\n"
            "EMIT IMM[0].xxxx\n"
            "MOV OUT[0], IN[2][0]\n"
            "MOV OUT[1], IN[2][1]\n"
            "MOV OUT[2].x, IN[2][2].xxxx\n"
            "EMIT IMM[0].xxxx\n"
            "END\n";
    struct tgsi_token tokens[1000];
    struct pipe_shader_state state = {0};

    if (!tgsi_text_translate(text, tokens, ARRAY_SIZE(tokens))) {
        assert(0);
        return NULL;
    }
    pipe_shader_state_from_tgsi(&state, tokens);
    return pipe->create_gs_state(pipe, &state);
}

static void
ureg_load_tex(struct ureg_program *ureg, struct ureg_dst out,
              struct ureg_src coord, struct ureg_src sampler,
              enum tgsi_texture_type tex_target,
              bool load_level_zero, bool use_txf)
{
    if (use_txf) {
        struct ureg_dst temp = ureg_DECL_temporary(ureg);

        ureg_F2I(ureg, temp, coord);

        if (load_level_zero)
            ureg_TXF_LZ(ureg, out, tex_target, ureg_src(temp), sampler);
        else
            ureg_TXF(ureg, out, tex_target, ureg_src(temp), sampler);
    } else {
        if (load_level_zero)
            ureg_TEX_LZ(ureg, out, tex_target, coord, sampler);
        else
            ureg_TEX(ureg, out, tex_target, coord, sampler);
    }
}

/**
 * Make simple fragment texture shader, with xrbias->float conversion:
 *  IMM {1023/510, -384/510, 0, 1}
 *  TEX TEMP[0], IN[0], SAMP[0], 2D;
 *  MAD TEMP[0].xyz TEMP[0], IMM[0].xxxx, IMM[0].yyyy
 *  MOV OUT[0], TEMP[0]
 *  END;
 *
 * \param tex_target  one of PIPE_TEXTURE_x
 */
void *
util_make_fragment_tex_shader_xrbias(struct pipe_context *pipe,
                                     enum tgsi_texture_type tex_target)
{
    struct ureg_program *ureg;
    struct ureg_src sampler;
    struct ureg_src coord;
    struct ureg_dst temp;
    struct ureg_dst out;
    struct ureg_src imm;
    enum tgsi_return_type stype = TGSI_RETURN_TYPE_FLOAT;

    ureg = ureg_create(PIPE_SHADER_FRAGMENT);
    if (!ureg)
        return NULL;

    imm = ureg_imm4f(ureg, 1023.0f/510.0f, -384.0f/510.0f, 0.0f, 1.0f);
    sampler = ureg_DECL_sampler(ureg, 0);
    ureg_DECL_sampler_view(ureg, 0, tex_target, stype, stype, stype, stype);
    coord = ureg_DECL_fs_input(ureg,
                               TGSI_SEMANTIC_GENERIC, 0,
                               TGSI_INTERPOLATE_LINEAR);
    out = ureg_DECL_output(ureg, TGSI_SEMANTIC_COLOR, 0);
    temp = ureg_DECL_temporary(ureg);

    ureg_TEX(ureg, temp, tex_target, coord, sampler);
    ureg_MAD(ureg, ureg_writemask(temp, TGSI_WRITEMASK_XYZ),
             ureg_src(temp),
             ureg_scalar(imm, TGSI_SWIZZLE_X),
             ureg_scalar(imm, TGSI_SWIZZLE_Y));
    ureg_MOV(ureg, out, ureg_src(temp));
    ureg_END(ureg);

    return ureg_create_shader_and_destroy(ureg, pipe);
}


/**
 * Make simple fragment texture shader:
 *  IMM {0,0,0,1}                         // (if writemask != 0xf)
 *  MOV TEMP[0], IMM[0]                   // (if writemask != 0xf)
 *  TEX TEMP[0].writemask, IN[0], SAMP[0], 2D;
 *   .. optional SINT <-> UINT clamping ..
 *  MOV OUT[0], TEMP[0]
 *  END;
 *
 * \param tex_target  one of TGSI_TEXTURE_x
 * \parma interp_mode  either TGSI_INTERPOLATE_LINEAR or PERSPECTIVE
 * \param writemask  mask of TGSI_WRITEMASK_x
 */
void *
util_make_fragment_tex_shader_writemask(struct pipe_context *pipe,
                                        enum tgsi_texture_type tex_target,
                                        enum tgsi_interpolate_mode interp_mode,
                                        unsigned writemask,
                                        enum tgsi_return_type stype,
                                        enum tgsi_return_type dtype,
                                        bool load_level_zero,
                                        bool use_txf)
{
    struct ureg_program *ureg;
    struct ureg_src sampler;
    struct ureg_src tex;
    struct ureg_dst temp;
    struct ureg_dst out;

    assert((stype == TGSI_RETURN_TYPE_FLOAT) == (dtype == TGSI_RETURN_TYPE_FLOAT));
    assert(interp_mode == TGSI_INTERPOLATE_LINEAR ||
           interp_mode == TGSI_INTERPOLATE_PERSPECTIVE);

    ureg = ureg_create( PIPE_SHADER_FRAGMENT );
    if (!ureg)
        return NULL;

    sampler = ureg_DECL_sampler( ureg, 0 );

    ureg_DECL_sampler_view(ureg, 0, tex_target, stype, stype, stype, stype);

    tex = ureg_DECL_fs_input( ureg,
                              TGSI_SEMANTIC_GENERIC, 0,
                              interp_mode );

    out = ureg_DECL_output( ureg,
                            TGSI_SEMANTIC_COLOR,
                            0 );

    temp = ureg_DECL_temporary(ureg);

    if (writemask != TGSI_WRITEMASK_XYZW) {
        struct ureg_src imm = ureg_imm4f( ureg, 0, 0, 0, 1 );

        ureg_MOV(ureg, temp, imm);
    }

    if (tex_target == TGSI_TEXTURE_BUFFER)
        ureg_TXF(ureg,
                 ureg_writemask(temp, writemask),
                 tex_target, tex, sampler);
    else
        ureg_load_tex(ureg, ureg_writemask(temp, writemask), tex, sampler,
                      tex_target, load_level_zero, use_txf);

    if (stype != dtype) {
        if (stype == TGSI_RETURN_TYPE_SINT) {
            assert(dtype == TGSI_RETURN_TYPE_UINT);

            ureg_IMAX(ureg, temp, ureg_src(temp), ureg_imm1i(ureg, 0));
        } else {
            assert(stype == TGSI_RETURN_TYPE_UINT);
            assert(dtype == TGSI_RETURN_TYPE_SINT);

            ureg_UMIN(ureg, temp, ureg_src(temp), ureg_imm1u(ureg, (1u << 31) - 1));
        }
    }

    ureg_MOV(ureg, out, ureg_src(temp));

    ureg_END( ureg );

    return ureg_create_shader_and_destroy( ureg, pipe );
}


/**
 * Make a simple fragment shader that sets the output color to a color
 * taken from a texture.
 * \param tex_target  one of TGSI_TEXTURE_x
 */
void *
util_make_fragment_tex_shader(struct pipe_context *pipe,
                              enum tgsi_texture_type tex_target,
                              enum tgsi_interpolate_mode interp_mode,
                              enum tgsi_return_type stype,
                              enum tgsi_return_type dtype,
                              bool load_level_zero,
                              bool use_txf)
{
    return util_make_fragment_tex_shader_writemask( pipe,
                                                    tex_target,
                                                    interp_mode,
                                                    TGSI_WRITEMASK_XYZW,
                                                    stype, dtype, load_level_zero,
                                                    use_txf);
}


/**
 * Make a simple fragment texture shader which reads the texture unit 0 and 1
 * and writes it as depth and stencil, respectively.
 */
void *
util_make_fs_blit_zs(struct pipe_context *pipe, unsigned zs_mask,
                     enum tgsi_texture_type tex_target,
                     bool load_level_zero, bool use_txf)
{
    struct ureg_program *ureg;
    struct ureg_src depth_sampler, stencil_sampler, coord;
    struct ureg_dst depth, stencil, tmp;

    ureg = ureg_create(PIPE_SHADER_FRAGMENT);
    if (!ureg)
        return NULL;

    coord = ureg_DECL_fs_input(ureg, TGSI_SEMANTIC_GENERIC, 0,
                               TGSI_INTERPOLATE_LINEAR);
    tmp = ureg_DECL_temporary(ureg);

    if (zs_mask & PIPE_MASK_Z) {
        depth_sampler = ureg_DECL_sampler(ureg, 0);
        ureg_DECL_sampler_view(ureg, 0, tex_target,
                               TGSI_RETURN_TYPE_FLOAT,
                               TGSI_RETURN_TYPE_FLOAT,
                               TGSI_RETURN_TYPE_FLOAT,
                               TGSI_RETURN_TYPE_FLOAT);

        ureg_load_tex(ureg, ureg_writemask(tmp, TGSI_WRITEMASK_X), coord,
                      depth_sampler, tex_target, load_level_zero, use_txf);

        depth = ureg_DECL_output(ureg, TGSI_SEMANTIC_POSITION, 0);
        ureg_MOV(ureg, ureg_writemask(depth, TGSI_WRITEMASK_Z),
                 ureg_scalar(ureg_src(tmp), TGSI_SWIZZLE_X));
    }

    if (zs_mask & PIPE_MASK_S) {
        stencil_sampler = ureg_DECL_sampler(ureg, zs_mask & PIPE_MASK_Z ? 1 : 0);
        ureg_DECL_sampler_view(ureg, zs_mask & PIPE_MASK_Z ? 1 : 0, tex_target,
                               TGSI_RETURN_TYPE_UINT,
                               TGSI_RETURN_TYPE_UINT,
                               TGSI_RETURN_TYPE_UINT,
                               TGSI_RETURN_TYPE_UINT);

        ureg_load_tex(ureg, ureg_writemask(tmp, TGSI_WRITEMASK_Y), coord,
                      stencil_sampler, tex_target, load_level_zero, use_txf);

        stencil = ureg_DECL_output(ureg, TGSI_SEMANTIC_STENCIL, 0);
        ureg_MOV(ureg, ureg_writemask(stencil, TGSI_WRITEMASK_Y),
                 ureg_scalar(ureg_src(tmp), TGSI_SWIZZLE_Y));
    }

    ureg_END(ureg);

    return ureg_create_shader_and_destroy(ureg, pipe);
}


/**
 * Make simple fragment color pass-through shader that replicates OUT[0]
 * to all bound colorbuffers.
 */
void *
util_make_fragment_passthrough_shader(struct pipe_context *pipe,
                                      int input_semantic,
                                      int input_interpolate,
                                      boolean write_all_cbufs)
{
    static const char shader_templ[] =
            "FRAG\n"
            "%s"
            "DCL IN[0], %s[0], %s\n"
            "DCL OUT[0], COLOR[0]\n"

            "MOV OUT[0], IN[0]\n"
            "END\n";

    char text[sizeof(shader_templ)+100];
    struct tgsi_token tokens[1000];
    struct pipe_shader_state state = {0};

    sprintf(text, shader_templ,
            write_all_cbufs ? "PROPERTY FS_COLOR0_WRITES_ALL_CBUFS 1\n" : "",
            tgsi_semantic_names[input_semantic],
            tgsi_interpolate_names[input_interpolate]);

    if (!tgsi_text_translate(text, tokens, ARRAY_SIZE(tokens))) {
        assert(0);
        return NULL;
    }
    pipe_shader_state_from_tgsi(&state, tokens);
#if 0
    tgsi_dump(state.tokens, 0);
#endif

    return pipe->create_fs_state(pipe, &state);
}


void *
util_make_empty_fragment_shader(struct pipe_context *pipe)
{
    struct ureg_program *ureg = ureg_create(PIPE_SHADER_FRAGMENT);
    if (!ureg)
        return NULL;

    ureg_END(ureg);
    return ureg_create_shader_and_destroy(ureg, pipe);
}


/**
 * Make a fragment shader that copies the input color to N output colors.
 */
void *
util_make_fragment_cloneinput_shader(struct pipe_context *pipe, int num_cbufs,
                                     int input_semantic,
                                     int input_interpolate)
{
    struct ureg_program *ureg;
    struct ureg_src src;
    struct ureg_dst dst[PIPE_MAX_COLOR_BUFS];
    int i;

    assert(num_cbufs <= PIPE_MAX_COLOR_BUFS);

    ureg = ureg_create( PIPE_SHADER_FRAGMENT );
    if (!ureg)
        return NULL;

    src = ureg_DECL_fs_input( ureg, input_semantic, 0,
                              input_interpolate );

    for (i = 0; i < num_cbufs; i++)
        dst[i] = ureg_DECL_output( ureg, TGSI_SEMANTIC_COLOR, i );

    for (i = 0; i < num_cbufs; i++)
        ureg_MOV( ureg, dst[i], src );

    ureg_END( ureg );

    return ureg_create_shader_and_destroy( ureg, pipe );
}


static void *
util_make_fs_blit_msaa_gen(struct pipe_context *pipe,
                           enum tgsi_texture_type tgsi_tex,
                           const char *samp_type,
                           const char *output_semantic,
                           const char *output_mask,
                           const char *swizzle,
                           const char *conversion_decl,
                           const char *conversion)
{
    static const char shader_templ[] =
            "FRAG\n"
            "DCL IN[0], GENERIC[0], LINEAR\n"
            "DCL SAMP[0]\n"
            "DCL SVIEW[0], %s, %s\n"
            "DCL OUT[0], %s\n"
            "DCL TEMP[0]\n"
            "%s"

            "F2U TEMP[0], IN[0]\n"
            "TXF TEMP[0], TEMP[0], SAMP[0], %s\n"
            "%s"
            "MOV OUT[0]%s, TEMP[0]%s\n"
            "END\n";

    const char *type = tgsi_texture_names[tgsi_tex];
    char text[sizeof(shader_templ)+100];
    struct tgsi_token tokens[1000];
    struct pipe_shader_state state = {0};

    assert(tgsi_tex == TGSI_TEXTURE_2D_MSAA ||
           tgsi_tex == TGSI_TEXTURE_2D_ARRAY_MSAA);

    snprintf(text, sizeof(text), shader_templ, type, samp_type,
             output_semantic, conversion_decl, type, conversion, output_mask, swizzle);

    if (!tgsi_text_translate(text, tokens, ARRAY_SIZE(tokens))) {
        puts(text);
        assert(0);
        return NULL;
    }
    pipe_shader_state_from_tgsi(&state, tokens);
#if 0
    tgsi_dump(state.tokens, 0);
#endif

    return pipe->create_fs_state(pipe, &state);
}


/**
 * Make a fragment shader that sets the output color to a color
 * fetched from a multisample texture.
 * \param tex_target  one of PIPE_TEXTURE_x
 */
void *
util_make_fs_blit_msaa_color(struct pipe_context *pipe,
                             enum tgsi_texture_type tgsi_tex,
                             enum tgsi_return_type stype,
                             enum tgsi_return_type dtype)
{
    const char *samp_type;
    const char *conversion_decl = "";
    const char *conversion = "";

    if (stype == TGSI_RETURN_TYPE_UINT) {
        samp_type = "UINT";

        if (dtype == TGSI_RETURN_TYPE_SINT) {
            conversion_decl = "IMM[0] UINT32 {2147483647, 0, 0, 0}\n";
            conversion = "UMIN TEMP[0], TEMP[0], IMM[0].xxxx\n";
        }
    } else if (stype == TGSI_RETURN_TYPE_SINT) {
        samp_type = "SINT";

        if (dtype == TGSI_RETURN_TYPE_UINT) {
            conversion_decl = "IMM[0] INT32 {0, 0, 0, 0}\n";
            conversion = "IMAX TEMP[0], TEMP[0], IMM[0].xxxx\n";
        }
    } else {
        assert(dtype == TGSI_RETURN_TYPE_FLOAT);
        samp_type = "FLOAT";
    }

    return util_make_fs_blit_msaa_gen(pipe, tgsi_tex, samp_type,
                                      "COLOR[0]", "", "", conversion_decl,
                                      conversion);
}


/**
 * Make a fragment shader that sets the output depth to a depth value
 * fetched from a multisample texture.
 * \param tex_target  one of PIPE_TEXTURE_x
 */
void *
util_make_fs_blit_msaa_depth(struct pipe_context *pipe,
                             enum tgsi_texture_type tgsi_tex)
{
    return util_make_fs_blit_msaa_gen(pipe, tgsi_tex, "FLOAT",
                                      "POSITION", ".z", ".xxxx", "", "");
}


/**
 * Make a fragment shader that sets the output stencil to a stencil value
 * fetched from a multisample texture.
 * \param tex_target  one of PIPE_TEXTURE_x
 */
void *
util_make_fs_blit_msaa_stencil(struct pipe_context *pipe,
                               enum tgsi_texture_type tgsi_tex)
{
    return util_make_fs_blit_msaa_gen(pipe, tgsi_tex, "UINT",
                                      "STENCIL", ".y", "", "", "");
}


/**
 * Make a fragment shader that sets the output depth and stencil to depth
 * and stencil values fetched from two multisample textures / samplers.
 * The sizes of both textures should match (it should be one depth-stencil
 * texture).
 * \param tex_target  one of PIPE_TEXTURE_x
 */
void *
util_make_fs_blit_msaa_depthstencil(struct pipe_context *pipe,
                                    enum tgsi_texture_type tgsi_tex)
{
    static const char shader_templ[] =
            "FRAG\n"
            "DCL IN[0], GENERIC[0], LINEAR\n"
            "DCL SAMP[0..1]\n"
            "DCL SVIEW[0], %s, FLOAT\n"
            "DCL SVIEW[1], %s, UINT\n"
            "DCL OUT[0], POSITION\n"
            "DCL OUT[1], STENCIL\n"
            "DCL TEMP[0..1]\n"

            "F2U TEMP[0], IN[0]\n"
            "TXF TEMP[1], TEMP[0], SAMP[0], %s\n"
            "MOV OUT[0].z, TEMP[1].xxxx\n"
            "TXF OUT[1].y, TEMP[0], SAMP[1], %s\n"
            "END\n";

    const char *type = tgsi_texture_names[tgsi_tex];
    char text[sizeof(shader_templ)+100];
    struct tgsi_token tokens[1000];
    struct pipe_shader_state state = {0};

    assert(tgsi_tex == TGSI_TEXTURE_2D_MSAA ||
           tgsi_tex == TGSI_TEXTURE_2D_ARRAY_MSAA);

    sprintf(text, shader_templ, type, type, type, type);

    if (!tgsi_text_translate(text, tokens, ARRAY_SIZE(tokens))) {
        assert(0);
        return NULL;
    }
    pipe_shader_state_from_tgsi(&state, tokens);
#if 0
    tgsi_dump(state.tokens, 0);
#endif

    return pipe->create_fs_state(pipe, &state);
}


void *
util_make_fs_msaa_resolve(struct pipe_context *pipe,
                          enum tgsi_texture_type tgsi_tex, unsigned nr_samples,
                          enum tgsi_return_type stype)
{
    struct ureg_program *ureg;
    struct ureg_src sampler, coord;
    struct ureg_dst out, tmp_sum, tmp_coord, tmp;
    unsigned i;

    ureg = ureg_create(PIPE_SHADER_FRAGMENT);
    if (!ureg)
        return NULL;

    /* Declarations. */
    sampler = ureg_DECL_sampler(ureg, 0);
    ureg_DECL_sampler_view(ureg, 0, tgsi_tex, stype, stype, stype, stype);
    coord = ureg_DECL_fs_input(ureg, TGSI_SEMANTIC_GENERIC, 0,
                               TGSI_INTERPOLATE_LINEAR);
    out = ureg_DECL_output(ureg, TGSI_SEMANTIC_COLOR, 0);
    tmp_sum = ureg_DECL_temporary(ureg);
    tmp_coord = ureg_DECL_temporary(ureg);
    tmp = ureg_DECL_temporary(ureg);

    /* Instructions. */
    ureg_MOV(ureg, tmp_sum, ureg_imm1f(ureg, 0));
    ureg_F2U(ureg, tmp_coord, coord);

    for (i = 0; i < nr_samples; i++) {
        /* Read one sample. */
        ureg_MOV(ureg, ureg_writemask(tmp_coord, TGSI_WRITEMASK_W),
                 ureg_imm1u(ureg, i));
        ureg_TXF(ureg, tmp, tgsi_tex, ureg_src(tmp_coord), sampler);

        if (stype == TGSI_RETURN_TYPE_UINT)
            ureg_U2F(ureg, tmp, ureg_src(tmp));
        else if (stype == TGSI_RETURN_TYPE_SINT)
            ureg_I2F(ureg, tmp, ureg_src(tmp));

        /* Add it to the sum.*/
        ureg_ADD(ureg, tmp_sum, ureg_src(tmp_sum), ureg_src(tmp));
    }

    /* Calculate the average and return. */
    ureg_MUL(ureg, tmp_sum, ureg_src(tmp_sum),
             ureg_imm1f(ureg, 1.0 / nr_samples));

    if (stype == TGSI_RETURN_TYPE_UINT)
        ureg_F2U(ureg, out, ureg_src(tmp_sum));
    else if (stype == TGSI_RETURN_TYPE_SINT)
        ureg_F2I(ureg, out, ureg_src(tmp_sum));
    else
        ureg_MOV(ureg, out, ureg_src(tmp_sum));

    ureg_END(ureg);

    return ureg_create_shader_and_destroy(ureg, pipe);
}


void *
util_make_fs_msaa_resolve_bilinear(struct pipe_context *pipe,
                                   enum tgsi_texture_type tgsi_tex,
                                   unsigned nr_samples,
                                   enum tgsi_return_type stype)
{
    struct ureg_program *ureg;
    struct ureg_src sampler, coord;
    struct ureg_dst out, tmp, top, bottom;
    struct ureg_dst tmp_coord[4], tmp_sum[4];
    unsigned i, c;

    ureg = ureg_create(PIPE_SHADER_FRAGMENT);
    if (!ureg)
        return NULL;

    /* Declarations. */
    sampler = ureg_DECL_sampler(ureg, 0);
    ureg_DECL_sampler_view(ureg, 0, tgsi_tex, stype, stype, stype, stype);
    coord = ureg_DECL_fs_input(ureg, TGSI_SEMANTIC_GENERIC, 0,
                               TGSI_INTERPOLATE_LINEAR);
    out = ureg_DECL_output(ureg, TGSI_SEMANTIC_COLOR, 0);
    for (c = 0; c < 4; c++)
        tmp_sum[c] = ureg_DECL_temporary(ureg);
    for (c = 0; c < 4; c++)
        tmp_coord[c] = ureg_DECL_temporary(ureg);
    tmp = ureg_DECL_temporary(ureg);
    top = ureg_DECL_temporary(ureg);
    bottom = ureg_DECL_temporary(ureg);

    /* Instructions. */
    for (c = 0; c < 4; c++)
        ureg_MOV(ureg, tmp_sum[c], ureg_imm1f(ureg, 0));

    /* Get 4 texture coordinates for the bilinear filter. */
    ureg_F2U(ureg, tmp_coord[0], coord); /* top-left */
    ureg_UADD(ureg, tmp_coord[1], ureg_src(tmp_coord[0]),
              ureg_imm4u(ureg, 1, 0, 0, 0)); /* top-right */
    ureg_UADD(ureg, tmp_coord[2], ureg_src(tmp_coord[0]),
              ureg_imm4u(ureg, 0, 1, 0, 0)); /* bottom-left */
    ureg_UADD(ureg, tmp_coord[3], ureg_src(tmp_coord[0]),
              ureg_imm4u(ureg, 1, 1, 0, 0)); /* bottom-right */

    for (i = 0; i < nr_samples; i++) {
        for (c = 0; c < 4; c++) {
            /* Read one sample. */
            ureg_MOV(ureg, ureg_writemask(tmp_coord[c], TGSI_WRITEMASK_W),
                     ureg_imm1u(ureg, i));
            ureg_TXF(ureg, tmp, tgsi_tex, ureg_src(tmp_coord[c]), sampler);

            if (stype == TGSI_RETURN_TYPE_UINT)
                ureg_U2F(ureg, tmp, ureg_src(tmp));
            else if (stype == TGSI_RETURN_TYPE_SINT)
                ureg_I2F(ureg, tmp, ureg_src(tmp));

            /* Add it to the sum.*/
            ureg_ADD(ureg, tmp_sum[c], ureg_src(tmp_sum[c]), ureg_src(tmp));
        }
    }

    /* Calculate the average. */
    for (c = 0; c < 4; c++)
        ureg_MUL(ureg, tmp_sum[c], ureg_src(tmp_sum[c]),
                 ureg_imm1f(ureg, 1.0 / nr_samples));

    /* Take the 4 average values and apply a standard bilinear filter. */
    ureg_FRC(ureg, tmp, coord);

    ureg_LRP(ureg, top,
             ureg_scalar(ureg_src(tmp), 0),
             ureg_src(tmp_sum[1]),
             ureg_src(tmp_sum[0]));

    ureg_LRP(ureg, bottom,
             ureg_scalar(ureg_src(tmp), 0),
             ureg_src(tmp_sum[3]),
             ureg_src(tmp_sum[2]));

    ureg_LRP(ureg, tmp,
             ureg_scalar(ureg_src(tmp), 1),
             ureg_src(bottom),
             ureg_src(top));

    /* Convert to the texture format and return. */
    if (stype == TGSI_RETURN_TYPE_UINT)
        ureg_F2U(ureg, out, ureg_src(tmp));
    else if (stype == TGSI_RETURN_TYPE_SINT)
        ureg_F2I(ureg, out, ureg_src(tmp));
    else
        ureg_MOV(ureg, out, ureg_src(tmp));

    ureg_END(ureg);

    return ureg_create_shader_and_destroy(ureg, pipe);
}

void *
util_make_geometry_passthrough_shader(struct pipe_context *pipe,
                                      uint num_attribs,
                                      const ubyte *semantic_names,
                                      const ubyte *semantic_indexes)
{
    static const unsigned zero[4] = {0, 0, 0, 0};

    struct ureg_program *ureg;
    struct ureg_dst dst[PIPE_MAX_SHADER_OUTPUTS];
    struct ureg_src src[PIPE_MAX_SHADER_INPUTS];
    struct ureg_src imm;

    unsigned i;

    ureg = ureg_create(PIPE_SHADER_GEOMETRY);
    if (!ureg)
        return NULL;

    ureg_property(ureg, TGSI_PROPERTY_GS_INPUT_PRIM, PIPE_PRIM_POINTS);
    ureg_property(ureg, TGSI_PROPERTY_GS_OUTPUT_PRIM, PIPE_PRIM_POINTS);
    ureg_property(ureg, TGSI_PROPERTY_GS_MAX_OUTPUT_VERTICES, 1);
    ureg_property(ureg, TGSI_PROPERTY_GS_INVOCATIONS, 1);
    imm = ureg_DECL_immediate_uint(ureg, zero, 4);

    /**
     * Loop over all the attribs and declare the corresponding
     * declarations in the geometry shader
     */
    for (i = 0; i < num_attribs; i++) {
        src[i] = ureg_DECL_input(ureg, semantic_names[i],
                                 semantic_indexes[i], 0, 1);
        src[i] = ureg_src_dimension(src[i], 0);
        dst[i] = ureg_DECL_output(ureg, semantic_names[i], semantic_indexes[i]);
    }

    /* MOV dst[i] src[i] */
    for (i = 0; i < num_attribs; i++) {
        ureg_MOV(ureg, dst[i], src[i]);
    }

    /* EMIT IMM[0] */
    ureg_insn(ureg, TGSI_OPCODE_EMIT, NULL, 0, &imm, 1, 0);

    /* END */
    ureg_END(ureg);

    return ureg_create_shader_and_destroy(ureg, pipe);
}


/**
 * Blit from color to ZS or from ZS to color in a manner that is equivalent
 * to memcpy.
 *
 * Color is either R32_UINT (for Z24S8 / S8Z24) or R32G32_UINT (Z32_S8X24).
 *
 * Depth and stencil samplers are used to load depth and stencil,
 * and they are packed and the result is written to a color output.
 *   OR
 * A color sampler is used to load a color value, which is unpacked and
 * written to depth and stencil shader outputs.
 */
void *
util_make_fs_pack_color_zs(struct pipe_context *pipe,
                           enum tgsi_texture_type tex_target,
                           enum pipe_format zs_format,
                           bool dst_is_color)
{
    struct ureg_program *ureg;
    struct ureg_src depth_sampler, stencil_sampler, color_sampler, coord;
    struct ureg_dst out, depth, depth_x, stencil, out_depth, out_stencil, color;

    assert(zs_format == PIPE_FORMAT_Z24_UNORM_S8_UINT || /* color is R32_UINT */
           zs_format == PIPE_FORMAT_S8_UINT_Z24_UNORM || /* color is R32_UINT */
           zs_format == PIPE_FORMAT_Z32_FLOAT_S8X24_UINT || /* color is R32G32_UINT */
           zs_format == PIPE_FORMAT_Z24X8_UNORM || /* color is R32_UINT */
           zs_format == PIPE_FORMAT_X8Z24_UNORM); /* color is R32_UINT */

    bool has_stencil = zs_format != PIPE_FORMAT_Z24X8_UNORM &&
                       zs_format != PIPE_FORMAT_X8Z24_UNORM;
    bool is_z24 = zs_format != PIPE_FORMAT_Z32_FLOAT_S8X24_UINT;
    bool z24_is_high = zs_format == PIPE_FORMAT_S8_UINT_Z24_UNORM ||
                       zs_format == PIPE_FORMAT_X8Z24_UNORM;

    ureg = ureg_create(PIPE_SHADER_FRAGMENT);
    if (!ureg)
        return NULL;

    coord = ureg_DECL_fs_input(ureg, TGSI_SEMANTIC_GENERIC, 0,
                               TGSI_INTERPOLATE_LINEAR);

    if (dst_is_color) {
        /* Load depth. */
        depth_sampler = ureg_DECL_sampler(ureg, 0);
        ureg_DECL_sampler_view(ureg, 0, tex_target,
                               TGSI_RETURN_TYPE_FLOAT,
                               TGSI_RETURN_TYPE_FLOAT,
                               TGSI_RETURN_TYPE_FLOAT,
                               TGSI_RETURN_TYPE_FLOAT);

        depth = ureg_DECL_temporary(ureg);
        depth_x = ureg_writemask(depth, TGSI_WRITEMASK_X);
        ureg_load_tex(ureg, depth_x, coord, depth_sampler, tex_target, true, true);

        /* Pack to Z24. */
        if (is_z24) {
            double imm = 0xffffff;
            struct ureg_src imm_f64 = ureg_DECL_immediate_f64(ureg, &imm, 2);
            struct ureg_dst tmp_xy = ureg_writemask(ureg_DECL_temporary(ureg),
                                                    TGSI_WRITEMASK_XY);

            ureg_F2D(ureg, tmp_xy, ureg_src(depth));
            ureg_DMUL(ureg, tmp_xy, ureg_src(tmp_xy), imm_f64);
            ureg_D2U(ureg, depth_x, ureg_src(tmp_xy));

            if (z24_is_high)
                ureg_SHL(ureg, depth_x, ureg_src(depth), ureg_imm1u(ureg, 8));
            else
                ureg_AND(ureg, depth_x, ureg_src(depth), ureg_imm1u(ureg, 0xffffff));
        }

        if (has_stencil) {
            /* Load stencil. */
            stencil_sampler = ureg_DECL_sampler(ureg, 1);
            ureg_DECL_sampler_view(ureg, 0, tex_target,
                                   TGSI_RETURN_TYPE_UINT,
                                   TGSI_RETURN_TYPE_UINT,
                                   TGSI_RETURN_TYPE_UINT,
                                   TGSI_RETURN_TYPE_UINT);

            stencil = ureg_writemask(ureg_DECL_temporary(ureg), TGSI_WRITEMASK_X);
            ureg_load_tex(ureg, stencil, coord, stencil_sampler, tex_target,
                          true, true);

            /* Pack stencil into depth. */
            if (is_z24) {
                if (!z24_is_high)
                    ureg_SHL(ureg, stencil, ureg_src(stencil), ureg_imm1u(ureg, 24));

                ureg_OR(ureg, depth_x, ureg_src(depth), ureg_src(stencil));
            }
        }

        out = ureg_DECL_output(ureg, TGSI_SEMANTIC_COLOR, 0);

        if (is_z24) {
            ureg_MOV(ureg, ureg_writemask(out, TGSI_WRITEMASK_X), ureg_src(depth));
        } else {
            /* Z32_S8X24 */
            ureg_MOV(ureg, ureg_writemask(depth, TGSI_WRITEMASK_Y),
                     ureg_scalar(ureg_src(stencil), TGSI_SWIZZLE_X));
            ureg_MOV(ureg, ureg_writemask(out, TGSI_WRITEMASK_XY), ureg_src(depth));
        }
    } else {
        color_sampler = ureg_DECL_sampler(ureg, 0);
        ureg_DECL_sampler_view(ureg, 0, tex_target,
                               TGSI_RETURN_TYPE_UINT,
                               TGSI_RETURN_TYPE_UINT,
                               TGSI_RETURN_TYPE_UINT,
                               TGSI_RETURN_TYPE_UINT);

        color = ureg_DECL_temporary(ureg);
        ureg_load_tex(ureg, color, coord, color_sampler, tex_target, true, true);

        depth = ureg_writemask(ureg_DECL_temporary(ureg), TGSI_WRITEMASK_X);
        stencil = ureg_writemask(ureg_DECL_temporary(ureg), TGSI_WRITEMASK_X);

        if (is_z24) {
            double imm = 1.0 / 0xffffff;
            struct ureg_src imm_f64 = ureg_DECL_immediate_f64(ureg, &imm, 2);
            struct ureg_dst tmp_xy = ureg_writemask(ureg_DECL_temporary(ureg),
                                                    TGSI_WRITEMASK_XY);

            ureg_UBFE(ureg, depth, ureg_src(color),
                      ureg_imm1u(ureg, z24_is_high ? 8 : 0),
                      ureg_imm1u(ureg, 24));
            ureg_U2D(ureg, tmp_xy, ureg_src(depth));
            ureg_DMUL(ureg, tmp_xy, ureg_src(tmp_xy), imm_f64);
            ureg_D2F(ureg, depth, ureg_src(tmp_xy));
        } else {
            /* depth = color.x; (Z32_S8X24) */
            ureg_MOV(ureg, depth, ureg_src(color));
        }

        out_depth = ureg_DECL_output(ureg, TGSI_SEMANTIC_POSITION, 0);
        ureg_MOV(ureg, ureg_writemask(out_depth, TGSI_WRITEMASK_Z),
                 ureg_scalar(ureg_src(depth), TGSI_SWIZZLE_X));

        if (has_stencil) {
            if (is_z24) {
                ureg_UBFE(ureg, stencil, ureg_src(color),
                          ureg_imm1u(ureg, z24_is_high ? 0 : 24),
                          ureg_imm1u(ureg, 8));
            } else {
                /* stencil = color.y[0:7]; (Z32_S8X24) */
                ureg_UBFE(ureg, stencil,
                          ureg_scalar(ureg_src(color), TGSI_SWIZZLE_Y),
                          ureg_imm1u(ureg, 0),
                          ureg_imm1u(ureg, 8));
            }

            out_stencil = ureg_DECL_output(ureg, TGSI_SEMANTIC_STENCIL, 0);
            ureg_MOV(ureg, ureg_writemask(out_stencil, TGSI_WRITEMASK_Y),
                     ureg_scalar(ureg_src(stencil), TGSI_SWIZZLE_X));
        }
    }

    ureg_END(ureg);

    return ureg_create_shader_and_destroy(ureg, pipe);
}


/**
 * Create passthrough tessellation control shader.
 * Passthrough tessellation control shader has output of vertex shader
 * as input and input of tessellation eval shader as output.
 */
void *
util_make_tess_ctrl_passthrough_shader(struct pipe_context *pipe,
                                       uint num_vs_outputs,
                                       uint num_tes_inputs,
                                       const ubyte *vs_semantic_names,
                                       const ubyte *vs_semantic_indexes,
                                       const ubyte *tes_semantic_names,
                                       const ubyte *tes_semantic_indexes,
                                       const unsigned vertices_per_patch)
{
    unsigned i, j;
    unsigned num_regs;

    struct ureg_program *ureg;
    struct ureg_dst temp, addr;
    struct ureg_src invocationID;
    struct ureg_dst dst[PIPE_MAX_SHADER_OUTPUTS];
    struct ureg_src src[PIPE_MAX_SHADER_INPUTS];

    ureg = ureg_create(PIPE_SHADER_TESS_CTRL);

    if (!ureg)
        return NULL;

    ureg_property(ureg, TGSI_PROPERTY_TCS_VERTICES_OUT, vertices_per_patch);

    num_regs = 0;

    for (i = 0; i < num_tes_inputs; i++) {
        switch (tes_semantic_names[i]) {
            case TGSI_SEMANTIC_POSITION:
            case TGSI_SEMANTIC_PSIZE:
            case TGSI_SEMANTIC_COLOR:
            case TGSI_SEMANTIC_BCOLOR:
            case TGSI_SEMANTIC_CLIPDIST:
            case TGSI_SEMANTIC_CLIPVERTEX:
            case TGSI_SEMANTIC_TEXCOORD:
            case TGSI_SEMANTIC_FOG:
            case TGSI_SEMANTIC_GENERIC:
                for (j = 0; j < num_vs_outputs; j++) {
                    if (tes_semantic_names[i] == vs_semantic_names[j] &&
                        tes_semantic_indexes[i] == vs_semantic_indexes[j]) {

                        dst[num_regs] = ureg_DECL_output(ureg,
                                                         tes_semantic_names[i],
                                                         tes_semantic_indexes[i]);
                        src[num_regs] = ureg_DECL_input(ureg, vs_semantic_names[j],
                                                        vs_semantic_indexes[j],
                                                        0, 1);

                        if (tes_semantic_names[i] == TGSI_SEMANTIC_GENERIC ||
                            tes_semantic_names[i] == TGSI_SEMANTIC_POSITION) {
                            src[num_regs] = ureg_src_dimension(src[num_regs], 0);
                            dst[num_regs] = ureg_dst_dimension(dst[num_regs], 0);
                        }

                        num_regs++;
                        break;
                    }
                }
                break;
            default:
                break;
        }
    }

    dst[num_regs] = ureg_DECL_output(ureg, TGSI_SEMANTIC_TESSOUTER,
                                     num_regs);
    src[num_regs] = ureg_DECL_constant(ureg, 0);
    num_regs++;
    dst[num_regs] = ureg_DECL_output(ureg, TGSI_SEMANTIC_TESSINNER,
                                     num_regs);
    src[num_regs] = ureg_DECL_constant(ureg, 1);
    num_regs++;

    if (vertices_per_patch > 1) {
        invocationID = ureg_DECL_system_value(ureg,
                                              TGSI_SEMANTIC_INVOCATIONID, 0);
        temp = ureg_DECL_local_temporary(ureg);
        addr = ureg_DECL_address(ureg);
        ureg_UARL(ureg, ureg_writemask(addr, TGSI_WRITEMASK_X),
                  ureg_scalar(invocationID, TGSI_SWIZZLE_X));
    }

    for (i = 0; i < num_regs; i++) {
        if (dst[i].Dimension && vertices_per_patch > 1) {
            struct ureg_src addr_x = ureg_scalar(ureg_src(addr), TGSI_SWIZZLE_X);
            ureg_MOV(ureg, temp, ureg_src_dimension_indirect(src[i],
                                                             addr_x, 0));
            ureg_MOV(ureg, ureg_dst_dimension_indirect(dst[i],
                                                       addr_x, 0), ureg_src(temp));
        }
        else
            ureg_MOV(ureg, dst[i], src[i]);
    }

    ureg_END(ureg);

    return ureg_create_shader_and_destroy(ureg, pipe);
}

void *
util_make_fs_stencil_blit(struct pipe_context *pipe, bool msaa_src)
{
    static const char shader_templ[] =
            "FRAG\n"
            "DCL IN[0], GENERIC[0], LINEAR\n"
            "DCL SAMP[0]\n"
            "DCL CONST[0][0]\n"
            "DCL TEMP[0]\n"

            "F2U TEMP[0], IN[0]\n"
            "TXF_LZ TEMP[0].x, TEMP[0], SAMP[0], %s\n"
            "AND TEMP[0].x, TEMP[0], CONST[0][0]\n"
            "USNE TEMP[0].x, TEMP[0], CONST[0][0]\n"
            "U2F TEMP[0].x, TEMP[0]\n"
            "KILL_IF -TEMP[0].xxxx\n"
            "END\n";

    char text[sizeof(shader_templ)+100];
    struct tgsi_token tokens[1000];
    struct pipe_shader_state state = { 0 };

    enum tgsi_texture_type tgsi_tex = msaa_src ? TGSI_TEXTURE_2D_MSAA :
                                      TGSI_TEXTURE_2D;

    sprintf(text, shader_templ, tgsi_texture_names[tgsi_tex]);

    if (!tgsi_text_translate(text, tokens, ARRAY_SIZE(tokens))) {
        assert(0);
        return NULL;
    }

    pipe_shader_state_from_tgsi(&state, tokens);

    return pipe->create_fs_state(pipe, &state);
}
