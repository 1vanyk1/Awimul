#include "errors.h"
#include "glheader.h"
#include "mtypes.h"
#include "macros.h"
#include "enums.h"
#include "ffvertex_prog.h"
#include "../program/program.h"
#include "../program/prog_cache.h"
#include "../program/prog_instruction.h"
#include "../program/prog_parameter.h"
#include "../program/prog_print.h"
#include "../program/prog_statevars.h"
#include "../util/bitscan.h"


/** Max of number of lights and texture coord units */
#define NUM_UNITS MAX2(MAX_TEXTURE_COORD_UNITS, MAX_LIGHTS)

struct state_key {
    GLbitfield varying_vp_inputs;

    unsigned fragprog_inputs_read:12;

    unsigned light_color_material_mask:12;
    unsigned light_global_enabled:1;
    unsigned light_local_viewer:1;
    unsigned light_twoside:1;
    unsigned material_shininess_is_zero:1;
    unsigned need_eye_coords:1;
    unsigned normalize:1;
    unsigned rescale_normals:1;

    unsigned fog_distance_mode:2;
    unsigned separate_specular:1;
    unsigned point_attenuated:1;

    struct {
        unsigned char light_enabled:1;
        unsigned char light_eyepos3_is_zero:1;
        unsigned char light_spotcutoff_is_180:1;
        unsigned char light_attenuated:1;
        unsigned char texmat_enabled:1;
        unsigned char coord_replace:1;
        unsigned char texgen_enabled:1;
        unsigned char texgen_mode0:4;
        unsigned char texgen_mode1:4;
        unsigned char texgen_mode2:4;
        unsigned char texgen_mode3:4;
    } unit[NUM_UNITS];
};


#define TXG_NONE           0
#define TXG_OBJ_LINEAR     1
#define TXG_EYE_LINEAR     2
#define TXG_SPHERE_MAP     3
#define TXG_REFLECTION_MAP 4
#define TXG_NORMAL_MAP     5

static GLuint translate_texgen( GLboolean enabled, GLenum mode )
{
    if (!enabled)
        return TXG_NONE;

    switch (mode) {
        case GL_OBJECT_LINEAR: return TXG_OBJ_LINEAR;
        case GL_EYE_LINEAR: return TXG_EYE_LINEAR;
        case GL_SPHERE_MAP: return TXG_SPHERE_MAP;
        case GL_REFLECTION_MAP_NV: return TXG_REFLECTION_MAP;
        case GL_NORMAL_MAP_NV: return TXG_NORMAL_MAP;
        default: return TXG_NONE;
    }
}

#define FDM_EYE_RADIAL    0
#define FDM_EYE_PLANE     1
#define FDM_EYE_PLANE_ABS 2
#define FDM_FROM_ARRAY    3

static GLuint translate_fog_distance_mode(GLenum source, GLenum mode)
{
    if (source == GL_FRAGMENT_DEPTH_EXT) {
        switch (mode) {
            case GL_EYE_RADIAL_NV:
                return FDM_EYE_RADIAL;
            case GL_EYE_PLANE:
                return FDM_EYE_PLANE;
            default: /* shouldn't happen; fall through to a sensible default */
            case GL_EYE_PLANE_ABSOLUTE_NV:
                return FDM_EYE_PLANE_ABS;
        }
    } else {
        return FDM_FROM_ARRAY;
    }
}

static GLboolean check_active_shininess( struct gl_context *ctx,
                                         const struct state_key *key,
                                         GLuint side )
{
    GLuint attr = MAT_ATTRIB_FRONT_SHININESS + side;

    if ((key->varying_vp_inputs & VERT_BIT_COLOR0) &&
        (key->light_color_material_mask & (1 << attr)))
        return GL_TRUE;

    if (key->varying_vp_inputs & VERT_BIT_MAT(attr))
        return GL_TRUE;

    if (ctx->Light.Material.Attrib[attr][0] != 0.0F)
        return GL_TRUE;

    return GL_FALSE;
}


static void make_state_key( struct gl_context *ctx, struct state_key *key )
{
    const struct gl_program *fp = ctx->FragmentProgram._Current;
    GLbitfield mask;

    memset(key, 0, sizeof(struct state_key));

    /* This now relies on texenvprogram.c being active:
     */
    assert(fp);

    key->need_eye_coords = ctx->_NeedEyeCoords;

    key->fragprog_inputs_read = fp->info.inputs_read;
    key->varying_vp_inputs = ctx->VertexProgram._VaryingInputs;

    if (ctx->RenderMode == GL_FEEDBACK) {
        /* make sure the vertprog emits color and tex0 */
        key->fragprog_inputs_read |= (VARYING_BIT_COL0 | VARYING_BIT_TEX0);
    }

    if (ctx->Light.Enabled) {
        key->light_global_enabled = 1;

        if (ctx->Light.Model.LocalViewer)
            key->light_local_viewer = 1;

        if (ctx->Light.Model.TwoSide)
            key->light_twoside = 1;

        if (ctx->Light.Model.ColorControl == GL_SEPARATE_SPECULAR_COLOR)
            key->separate_specular = 1;

        if (ctx->Light.ColorMaterialEnabled) {
            key->light_color_material_mask = ctx->Light._ColorMaterialBitmask;
        }

        mask = ctx->Light._EnabledLights;
        while (mask) {
            const int i = u_bit_scan(&mask);
            struct gl_light_uniforms *lu = &ctx->Light.LightSource[i];

            key->unit[i].light_enabled = 1;

            if (lu->EyePosition[3] == 0.0F)
                key->unit[i].light_eyepos3_is_zero = 1;

            if (lu->SpotCutoff == 180.0F)
                key->unit[i].light_spotcutoff_is_180 = 1;

            if (lu->ConstantAttenuation != 1.0F ||
                lu->LinearAttenuation != 0.0F ||
                lu->QuadraticAttenuation != 0.0F)
                key->unit[i].light_attenuated = 1;
        }

        if (check_active_shininess(ctx, key, 0)) {
            key->material_shininess_is_zero = 0;
        }
        else if (key->light_twoside &&
                 check_active_shininess(ctx, key, 1)) {
            key->material_shininess_is_zero = 0;
        }
        else {
            key->material_shininess_is_zero = 1;
        }
    }

    if (ctx->Transform.Normalize)
        key->normalize = 1;

    if (ctx->Transform.RescaleNormals)
        key->rescale_normals = 1;

    /* Only distinguish fog parameters if we actually need */
    if (key->fragprog_inputs_read & VARYING_BIT_FOGC)
        key->fog_distance_mode =
                translate_fog_distance_mode(ctx->Fog.FogCoordinateSource,
                                            ctx->Fog.FogDistanceMode);

    if (ctx->Point._Attenuated)
        key->point_attenuated = 1;

    mask = ctx->Texture._EnabledCoordUnits | ctx->Texture._TexGenEnabled
           | ctx->Texture._TexMatEnabled | ctx->Point.CoordReplace;
    while (mask) {
        const int i = u_bit_scan(&mask);
        struct gl_fixedfunc_texture_unit *texUnit =
                &ctx->Texture.FixedFuncUnit[i];

        if (ctx->Point.PointSprite)
            if (ctx->Point.CoordReplace & (1u << i))
                key->unit[i].coord_replace = 1;

        if (ctx->Texture._TexMatEnabled & ENABLE_TEXMAT(i))
            key->unit[i].texmat_enabled = 1;

        if (texUnit->TexGenEnabled) {
            key->unit[i].texgen_enabled = 1;

            key->unit[i].texgen_mode0 =
                    translate_texgen( texUnit->TexGenEnabled & (1<<0),
                                      texUnit->GenS.Mode );
            key->unit[i].texgen_mode1 =
                    translate_texgen( texUnit->TexGenEnabled & (1<<1),
                                      texUnit->GenT.Mode );
            key->unit[i].texgen_mode2 =
                    translate_texgen( texUnit->TexGenEnabled & (1<<2),
                                      texUnit->GenR.Mode );
            key->unit[i].texgen_mode3 =
                    translate_texgen( texUnit->TexGenEnabled & (1<<3),
                                      texUnit->GenQ.Mode );
        }
    }
}



/* Very useful debugging tool - produces annotated listing of
 * generated program with line/function references for each
 * instruction back into this file:
 */
#define DISASSEM 0


/* Use uregs to represent registers internally, translate to Mesa's
 * expected formats on emit.
 *
 * NOTE: These are passed by value extensively in this file rather
 * than as usual by pointer reference.  If this disturbs you, try
 * remembering they are just 32bits in size.
 *
 * GCC is smart enough to deal with these dword-sized structures in
 * much the same way as if I had defined them as dwords and was using
 * macros to access and set the fields.  This is much nicer and easier
 * to evolve.
 */
struct ureg {
    GLuint file:4;
    GLint idx:9;      /* relative addressing may be negative */
    /* sizeof(idx) should == sizeof(prog_src_reg::Index) */
    GLuint negate:1;
    GLuint swz:12;
    GLuint pad:6;
};


struct tnl_program {
    const struct state_key *state;
    struct gl_program *program;
    struct gl_program_parameter_list *state_params;
    GLuint max_inst;  /** number of instructions allocated for program */
    GLboolean mvp_with_dp4;

    GLuint temp_in_use;
    GLuint temp_reserved;

    struct ureg eye_position;
    struct ureg eye_position_z;
    struct ureg eye_position_normalized;
    struct ureg transformed_normal;
    struct ureg identity;

    GLuint materials;
    GLuint color_materials;
};


static const struct ureg undef = {
        PROGRAM_UNDEFINED,
        0,
        0,
        0,
        0
};

/* Local shorthand:
 */
#define X    SWIZZLE_X
#define Y    SWIZZLE_Y
#define Z    SWIZZLE_Z
#define W    SWIZZLE_W


/* Construct a ureg:
 */
static struct ureg make_ureg(GLuint file, GLint idx)
{
    struct ureg reg;
    reg.file = file;
    reg.idx = idx;
    reg.negate = 0;
    reg.swz = SWIZZLE_NOOP;
    reg.pad = 0;
    return reg;
}


static struct ureg negate( struct ureg reg )
{
    reg.negate ^= 1;
    return reg;
}


static struct ureg swizzle( struct ureg reg, int x, int y, int z, int w )
{
    reg.swz = MAKE_SWIZZLE4(GET_SWZ(reg.swz, x),
                            GET_SWZ(reg.swz, y),
                            GET_SWZ(reg.swz, z),
                            GET_SWZ(reg.swz, w));
    return reg;
}


static struct ureg swizzle1( struct ureg reg, int x )
{
    return swizzle(reg, x, x, x, x);
}


static struct ureg get_temp( struct tnl_program *p )
{
    int bit = ffs( ~p->temp_in_use );
    if (!bit) {
        _mesa_problem(NULL, "%s: out of temporaries\n", __FILE__);
        exit(1);
    }

    if ((GLuint) bit > p->program->arb.NumTemporaries)
        p->program->arb.NumTemporaries = bit;

    p->temp_in_use |= 1<<(bit-1);
    return make_ureg(PROGRAM_TEMPORARY, bit-1);
}


static struct ureg reserve_temp( struct tnl_program *p )
{
    struct ureg temp = get_temp( p );
    p->temp_reserved |= 1<<temp.idx;
    return temp;
}


static void release_temp( struct tnl_program *p, struct ureg reg )
{
    if (reg.file == PROGRAM_TEMPORARY) {
        p->temp_in_use &= ~(1<<reg.idx);
        p->temp_in_use |= p->temp_reserved; /* can't release reserved temps */
    }
}

static void release_temps( struct tnl_program *p )
{
    p->temp_in_use = p->temp_reserved;
}


static struct ureg register_param4(struct tnl_program *p,
                                   GLint s0,
                                   GLint s1,
                                   GLint s2,
                                   GLint s3)
{
    gl_state_index16 tokens[STATE_LENGTH];
    GLint idx;
    tokens[0] = s0;
    tokens[1] = s1;
    tokens[2] = s2;
    tokens[3] = s3;
    idx = _mesa_add_state_reference(p->state_params, tokens);
    return make_ureg(PROGRAM_STATE_VAR, idx);
}


#define register_param1(p,s0)          register_param4(p,s0,0,0,0)
#define register_param2(p,s0,s1)       register_param4(p,s0,s1,0,0)
#define register_param3(p,s0,s1,s2)    register_param4(p,s0,s1,s2,0)



/**
 * \param input  one of VERT_ATTRIB_x tokens.
 */
static struct ureg register_input( struct tnl_program *p, GLuint input )
{
    assert(input < VERT_ATTRIB_MAX);

    if (p->state->varying_vp_inputs & VERT_BIT(input)) {
        p->program->info.inputs_read |= VERT_BIT(input);
        return make_ureg(PROGRAM_INPUT, input);
    }
    else {
        return register_param2(p, STATE_CURRENT_ATTRIB, input);
    }
}


/**
 * \param input  one of VARYING_SLOT_x tokens.
 */
static struct ureg register_output( struct tnl_program *p, GLuint output )
{
    p->program->info.outputs_written |= BITFIELD64_BIT(output);
    return make_ureg(PROGRAM_OUTPUT, output);
}


static struct ureg register_const4f( struct tnl_program *p,
                                     GLfloat s0,
                                     GLfloat s1,
                                     GLfloat s2,
                                     GLfloat s3)
{
    gl_constant_value values[4];
    GLint idx;
    GLuint swizzle;
    values[0].f = s0;
    values[1].f = s1;
    values[2].f = s2;
    values[3].f = s3;
    idx = _mesa_add_unnamed_constant(p->program->Parameters, values, 4,
                                     &swizzle );
    assert(swizzle == SWIZZLE_NOOP);
    return make_ureg(PROGRAM_CONSTANT, idx);
}

#define register_const1f(p, s0)         register_const4f(p, s0, 0, 0, 1)
#define register_scalar_const(p, s0)    register_const4f(p, s0, s0, s0, s0)
#define register_const2f(p, s0, s1)     register_const4f(p, s0, s1, 0, 1)
#define register_const3f(p, s0, s1, s2) register_const4f(p, s0, s1, s2, 1)

static GLboolean is_undef( struct ureg reg )
{
    return reg.file == PROGRAM_UNDEFINED;
}


static struct ureg get_identity_param( struct tnl_program *p )
{
    if (is_undef(p->identity))
        p->identity = register_const4f(p, 0,0,0,1);

    return p->identity;
}

static void register_matrix_param5( struct tnl_program *p,
                                    GLint s0, /* modelview, projection, etc */
                                    GLint s1, /* texture matrix number */
                                    GLint s2, /* first row */
                                    GLint s3, /* last row */
                                    struct ureg *matrix )
{
    GLint i;

    /* This is a bit sad as the support is there to pull the whole
     * matrix out in one go:
     */
    for (i = 0; i <= s3 - s2; i++)
        matrix[i] = register_param4(p, s0, s1, i, i);
}


static void emit_arg( struct prog_src_register *src,
                      struct ureg reg )
{
    src->File = reg.file;
    src->Index = reg.idx;
    src->Swizzle = reg.swz;
    src->Negate = reg.negate ? NEGATE_XYZW : NEGATE_NONE;
    src->RelAddr = 0;
    /* Check that bitfield sizes aren't exceeded */
    assert(src->Index == reg.idx);
}


static void emit_dst( struct prog_dst_register *dst,
                      struct ureg reg, GLuint mask )
{
    dst->File = reg.file;
    dst->Index = reg.idx;
    /* allow zero as a shorthand for xyzw */
    dst->WriteMask = mask ? mask : WRITEMASK_XYZW;
    /* Check that bitfield sizes aren't exceeded */
    assert(dst->Index == reg.idx);
}


static void debug_insn( struct prog_instruction *inst, const char *fn,
                        GLuint line )
{
    if (DISASSEM) {
        static const char *last_fn;

        if (fn != last_fn) {
            last_fn = fn;
            printf("%s:\n", fn);
        }

        printf("%d:\t", line);
        _mesa_print_instruction(inst);
    }
}


static void emit_op3fn(struct tnl_program *p,
                       enum prog_opcode op,
                       struct ureg dest,
                       GLuint mask,
                       struct ureg src0,
                       struct ureg src1,
                       struct ureg src2,
                       const char *fn,
                       GLuint line)
{
    GLuint nr;
    struct prog_instruction *inst;

    assert(p->program->arb.NumInstructions <= p->max_inst);

    if (p->program->arb.NumInstructions == p->max_inst) {
        /* need to extend the program's instruction array */
        struct prog_instruction *newInst;

        /* double the size */
        p->max_inst *= 2;

        newInst =
                rzalloc_array(p->program, struct prog_instruction, p->max_inst);
        if (!newInst) {
            _mesa_error(NULL, GL_OUT_OF_MEMORY, "vertex program build");
            return;
        }

        _mesa_copy_instructions(newInst, p->program->arb.Instructions,
                                p->program->arb.NumInstructions);

        ralloc_free(p->program->arb.Instructions);

        p->program->arb.Instructions = newInst;
    }

    nr = p->program->arb.NumInstructions++;

    inst = &p->program->arb.Instructions[nr];
    inst->Opcode = (enum prog_opcode) op;

    emit_arg( &inst->SrcReg[0], src0 );
    emit_arg( &inst->SrcReg[1], src1 );
    emit_arg( &inst->SrcReg[2], src2 );

    emit_dst( &inst->DstReg, dest, mask );

    debug_insn(inst, fn, line);
}


#define emit_op3(p, op, dst, mask, src0, src1, src2) \
   emit_op3fn(p, op, dst, mask, src0, src1, src2, __func__, __LINE__)

#define emit_op2(p, op, dst, mask, src0, src1) \
    emit_op3fn(p, op, dst, mask, src0, src1, undef, __func__, __LINE__)

#define emit_op1(p, op, dst, mask, src0) \
    emit_op3fn(p, op, dst, mask, src0, undef, undef, __func__, __LINE__)


static struct ureg make_temp( struct tnl_program *p, struct ureg reg )
{
    if (reg.file == PROGRAM_TEMPORARY &&
        !(p->temp_reserved & (1<<reg.idx)))
        return reg;
    else {
        struct ureg temp = get_temp(p);
        emit_op1(p, OPCODE_MOV, temp, 0, reg);
        return temp;
    }
}


/* Currently no tracking performed of input/output/register size or
 * active elements.  Could be used to reduce these operations, as
 * could the matrix type.
 */
static void emit_matrix_transform_vec4( struct tnl_program *p,
                                        struct ureg dest,
                                        const struct ureg *mat,
                                        struct ureg src)
{
    emit_op2(p, OPCODE_DP4, dest, WRITEMASK_X, src, mat[0]);
    emit_op2(p, OPCODE_DP4, dest, WRITEMASK_Y, src, mat[1]);
    emit_op2(p, OPCODE_DP4, dest, WRITEMASK_Z, src, mat[2]);
    emit_op2(p, OPCODE_DP4, dest, WRITEMASK_W, src, mat[3]);
}


/* This version is much easier to implement if writemasks are not
 * supported natively on the target or (like SSE), the target doesn't
 * have a clean/obvious dotproduct implementation.
 */
static void emit_transpose_matrix_transform_vec4( struct tnl_program *p,
                                                  struct ureg dest,
                                                  const struct ureg *mat,
                                                  struct ureg src)
{
    struct ureg tmp;

    if (dest.file != PROGRAM_TEMPORARY)
        tmp = get_temp(p);
    else
        tmp = dest;

    emit_op2(p, OPCODE_MUL, tmp, 0, swizzle1(src,X), mat[0]);
    emit_op3(p, OPCODE_MAD, tmp, 0, swizzle1(src,Y), mat[1], tmp);
    emit_op3(p, OPCODE_MAD, tmp, 0, swizzle1(src,Z), mat[2], tmp);
    emit_op3(p, OPCODE_MAD, dest, 0, swizzle1(src,W), mat[3], tmp);

    if (dest.file != PROGRAM_TEMPORARY)
        release_temp(p, tmp);
}


static void emit_matrix_transform_vec3( struct tnl_program *p,
                                        struct ureg dest,
                                        const struct ureg *mat,
                                        struct ureg src)
{
    emit_op2(p, OPCODE_DP3, dest, WRITEMASK_X, src, mat[0]);
    emit_op2(p, OPCODE_DP3, dest, WRITEMASK_Y, src, mat[1]);
    emit_op2(p, OPCODE_DP3, dest, WRITEMASK_Z, src, mat[2]);
}


static void emit_normalize_vec3( struct tnl_program *p,
                                 struct ureg dest,
                                 struct ureg src )
{
    struct ureg tmp = get_temp(p);
    emit_op2(p, OPCODE_DP3, tmp, WRITEMASK_X, src, src);
    emit_op1(p, OPCODE_RSQ, tmp, WRITEMASK_X, tmp);
    emit_op2(p, OPCODE_MUL, dest, 0, src, swizzle1(tmp, X));
    release_temp(p, tmp);
}


static void emit_passthrough( struct tnl_program *p,
                              GLuint input,
                              GLuint output )
{
    struct ureg out = register_output(p, output);
    emit_op1(p, OPCODE_MOV, out, 0, register_input(p, input));
}


static struct ureg get_eye_position( struct tnl_program *p )
{
    if (is_undef(p->eye_position)) {
        struct ureg pos = register_input( p, VERT_ATTRIB_POS );
        struct ureg modelview[4];

        p->eye_position = reserve_temp(p);

        if (p->mvp_with_dp4) {
            register_matrix_param5( p, STATE_MODELVIEW_MATRIX, 0, 0, 3,
                                    modelview );

            emit_matrix_transform_vec4(p, p->eye_position, modelview, pos);
        }
        else {
            register_matrix_param5( p, STATE_MODELVIEW_MATRIX_TRANSPOSE, 0, 0, 3,
                                    modelview );

            emit_transpose_matrix_transform_vec4(p, p->eye_position, modelview, pos);
        }
    }

    return p->eye_position;
}


static struct ureg get_eye_position_z( struct tnl_program *p )
{
    if (!is_undef(p->eye_position))
        return swizzle1(p->eye_position, Z);

    if (is_undef(p->eye_position_z)) {
        struct ureg pos = register_input( p, VERT_ATTRIB_POS );
        struct ureg modelview[4];

        p->eye_position_z = reserve_temp(p);

        register_matrix_param5( p, STATE_MODELVIEW_MATRIX, 0, 0, 3,
                                modelview );

        emit_op2(p, OPCODE_DP4, p->eye_position_z, 0, pos, modelview[2]);
    }

    return p->eye_position_z;
}


static struct ureg get_eye_position_normalized( struct tnl_program *p )
{
    if (is_undef(p->eye_position_normalized)) {
        struct ureg eye = get_eye_position(p);
        p->eye_position_normalized = reserve_temp(p);
        emit_normalize_vec3(p, p->eye_position_normalized, eye);
    }

    return p->eye_position_normalized;
}


static struct ureg get_transformed_normal( struct tnl_program *p )
{
    if (is_undef(p->transformed_normal) &&
        !p->state->need_eye_coords &&
        !p->state->normalize &&
        !(p->state->need_eye_coords == p->state->rescale_normals))
    {
        p->transformed_normal = register_input(p, VERT_ATTRIB_NORMAL );
    }
    else if (is_undef(p->transformed_normal))
    {
        struct ureg normal = register_input(p, VERT_ATTRIB_NORMAL );
        struct ureg mvinv[3];
        struct ureg transformed_normal = reserve_temp(p);

        if (p->state->need_eye_coords) {
            register_matrix_param5( p, STATE_MODELVIEW_MATRIX_INVTRANS, 0, 0, 2,
                                    mvinv );

            /* Transform to eye space:
             */
            emit_matrix_transform_vec3( p, transformed_normal, mvinv, normal );
            normal = transformed_normal;
        }

        /* Normalize/Rescale:
         */
        if (p->state->normalize) {
            emit_normalize_vec3( p, transformed_normal, normal );
            normal = transformed_normal;
        }
        else if (p->state->need_eye_coords == p->state->rescale_normals) {
            /* This is already adjusted for eye/non-eye rendering:
             */
            struct ureg rescale = register_param1(p, STATE_NORMAL_SCALE);

            emit_op2( p, OPCODE_MUL, transformed_normal, 0, normal, rescale );
            normal = transformed_normal;
        }

        assert(normal.file == PROGRAM_TEMPORARY);
        p->transformed_normal = normal;
    }

    return p->transformed_normal;
}


static void build_hpos( struct tnl_program *p )
{
    struct ureg pos = register_input( p, VERT_ATTRIB_POS );
    struct ureg hpos = register_output( p, VARYING_SLOT_POS );
    struct ureg mvp[4];

    if (p->mvp_with_dp4) {
        register_matrix_param5( p, STATE_MVP_MATRIX, 0, 0, 3,
                                mvp );
        emit_matrix_transform_vec4( p, hpos, mvp, pos );
    }
    else {
        register_matrix_param5( p, STATE_MVP_MATRIX_TRANSPOSE, 0, 0, 3,
                                mvp );
        emit_transpose_matrix_transform_vec4( p, hpos, mvp, pos );
    }
}


static GLuint material_attrib( GLuint side, GLuint property )
{
    switch (property) {
        case STATE_AMBIENT:
            return MAT_ATTRIB_FRONT_AMBIENT + side;
        case STATE_DIFFUSE:
            return MAT_ATTRIB_FRONT_DIFFUSE + side;
        case STATE_SPECULAR:
            return MAT_ATTRIB_FRONT_SPECULAR + side;
        case STATE_EMISSION:
            return MAT_ATTRIB_FRONT_EMISSION + side;
        case STATE_SHININESS:
            return MAT_ATTRIB_FRONT_SHININESS + side;
        default:
            unreachable("invalid value");
    }
}


/**
 * Get a bitmask of which material values vary on a per-vertex basis.
 */
static void set_material_flags( struct tnl_program *p )
{
    p->color_materials = 0;
    p->materials = 0;

    if (p->state->varying_vp_inputs & VERT_BIT_COLOR0) {
        p->materials =
        p->color_materials = p->state->light_color_material_mask;
    }

    p->materials |= ((p->state->varying_vp_inputs & VERT_BIT_MAT_ALL)
            >> VERT_ATTRIB_MAT(0));
}


static struct ureg get_material( struct tnl_program *p, GLuint side,
                                 GLuint property )
{
    GLuint attrib = material_attrib(side, property);

    if (p->color_materials & (1<<attrib))
        return register_input(p, VERT_ATTRIB_COLOR0);
    else if (p->materials & (1<<attrib)) {
        /* Put material values in the GENERIC slots -- they are not used
         * for anything in fixed function mode.
         */
        return register_input( p, VERT_ATTRIB_MAT(attrib) );
    }
    else
        return register_param2(p, STATE_MATERIAL, attrib);
}

#define SCENE_COLOR_BITS(side) (( MAT_BIT_FRONT_EMISSION | \
				   MAT_BIT_FRONT_AMBIENT | \
				   MAT_BIT_FRONT_DIFFUSE) << (side))


/**
 * Either return a precalculated constant value or emit code to
 * calculate these values dynamically in the case where material calls
 * are present between begin/end pairs.
 *
 * Probably want to shift this to the program compilation phase - if
 * we always emitted the calculation here, a smart compiler could
 * detect that it was constant (given a certain set of inputs), and
 * lift it out of the main loop.  That way the programs created here
 * would be independent of the vertex_buffer details.
 */
static struct ureg get_scenecolor( struct tnl_program *p, GLuint side )
{
    if (p->materials & SCENE_COLOR_BITS(side)) {
        struct ureg lm_ambient = register_param1(p, STATE_LIGHTMODEL_AMBIENT);
        struct ureg material_emission = get_material(p, side, STATE_EMISSION);
        struct ureg material_ambient = get_material(p, side, STATE_AMBIENT);
        struct ureg material_diffuse = get_material(p, side, STATE_DIFFUSE);
        struct ureg tmp = make_temp(p, material_diffuse);
        emit_op3(p, OPCODE_MAD, tmp, WRITEMASK_XYZ, lm_ambient,
                 material_ambient, material_emission);
        return tmp;
    }
    else
        return register_param2( p, STATE_LIGHTMODEL_SCENECOLOR, side );
}


static struct ureg get_lightprod( struct tnl_program *p, GLuint light,
                                  GLuint side, GLuint property )
{
    GLuint attrib = material_attrib(side, property);
    if (p->materials & (1<<attrib)) {
        struct ureg light_value =
                register_param3(p, STATE_LIGHT, light, property);
        struct ureg material_value = get_material(p, side, property);
        struct ureg tmp = get_temp(p);
        emit_op2(p, OPCODE_MUL, tmp, 0, light_value, material_value);
        return tmp;
    }
    else
        return register_param3(p, STATE_LIGHTPROD, light, attrib);
}


static struct ureg calculate_light_attenuation( struct tnl_program *p,
                                                GLuint i,
                                                struct ureg VPpli,
                                                struct ureg dist )
{
    struct ureg attenuation = register_param3(p, STATE_LIGHT, i,
                                              STATE_ATTENUATION);
    struct ureg att = undef;

    /* Calculate spot attenuation:
     */
    if (!p->state->unit[i].light_spotcutoff_is_180) {
        struct ureg spot_dir_norm = register_param2(p, STATE_LIGHT_SPOT_DIR_NORMALIZED, i);
        struct ureg spot = get_temp(p);
        struct ureg slt = get_temp(p);

        att = get_temp(p);

        emit_op2(p, OPCODE_DP3, spot, 0, negate(VPpli), spot_dir_norm);
        emit_op2(p, OPCODE_SLT, slt, 0, swizzle1(spot_dir_norm,W), spot);
        emit_op1(p, OPCODE_ABS, spot, 0, spot);
        emit_op2(p, OPCODE_POW, spot, 0, spot, swizzle1(attenuation, W));
        emit_op2(p, OPCODE_MUL, att, 0, slt, spot);

        release_temp(p, spot);
        release_temp(p, slt);
    }

    /* Calculate distance attenuation(See formula (2.4) at glspec 2.1 page 62):
     *
     * Skip the calucation when _dist_ is undefined(light_eyepos3_is_zero)
     */
    if (p->state->unit[i].light_attenuated && !is_undef(dist)) {
        if (is_undef(att))
            att = get_temp(p);
        /* 1/d,d,d,1/d */
        emit_op1(p, OPCODE_RCP, dist, WRITEMASK_YZ, dist);
        /* 1,d,d*d,1/d */
        emit_op2(p, OPCODE_MUL, dist, WRITEMASK_XZ, dist, swizzle1(dist,Y));
        /* 1/dist-atten */
        emit_op2(p, OPCODE_DP3, dist, 0, attenuation, dist);

        if (!p->state->unit[i].light_spotcutoff_is_180) {
            /* dist-atten */
            emit_op1(p, OPCODE_RCP, dist, 0, dist);
            /* spot-atten * dist-atten */
            emit_op2(p, OPCODE_MUL, att, 0, dist, att);
        }
        else {
            /* dist-atten */
            emit_op1(p, OPCODE_RCP, att, 0, dist);
        }
    }

    return att;
}


/**
 * Compute:
 *   lit.y = MAX(0, dots.x)
 *   lit.z = SLT(0, dots.x)
 */
static void emit_degenerate_lit( struct tnl_program *p,
                                 struct ureg lit,
                                 struct ureg dots )
{
    struct ureg id = get_identity_param(p);  /* id = {0,0,0,1} */

    /* Note that lit.x & lit.w will not be examined.  Note also that
     * dots.xyzw == dots.xxxx.
     */

    /* MAX lit, id, dots;
     */
    emit_op2(p, OPCODE_MAX, lit, WRITEMASK_XYZW, id, dots);

    /* result[2] = (in > 0 ? 1 : 0)
     * SLT lit.z, id.z, dots;   # lit.z = (0 < dots.z) ? 1 : 0
     */
    emit_op2(p, OPCODE_SLT, lit, WRITEMASK_Z, swizzle1(id,Z), dots);
}


/* Need to add some addtional parameters to allow lighting in object
 * space - STATE_SPOT_DIRECTION and STATE_HALF_VECTOR implicitly assume eye
 * space lighting.
 */
static void build_lighting( struct tnl_program *p )
{
    const GLboolean twoside = p->state->light_twoside;
    const GLboolean separate = p->state->separate_specular;
    GLuint nr_lights = 0, count = 0;
    struct ureg normal = get_transformed_normal(p);
    struct ureg lit = get_temp(p);
    struct ureg dots = get_temp(p);
    struct ureg _col0 = undef, _col1 = undef;
    struct ureg _bfc0 = undef, _bfc1 = undef;
    GLuint i;

    /*
     * NOTE:
     * dots.x = dot(normal, VPpli)
     * dots.y = dot(normal, halfAngle)
     * dots.z = back.shininess
     * dots.w = front.shininess
     */

    for (i = 0; i < MAX_LIGHTS; i++)
        if (p->state->unit[i].light_enabled)
            nr_lights++;

    set_material_flags(p);

    {
        if (!p->state->material_shininess_is_zero) {
            struct ureg shininess = get_material(p, 0, STATE_SHININESS);
            emit_op1(p, OPCODE_MOV, dots, WRITEMASK_W, swizzle1(shininess,X));
            release_temp(p, shininess);
        }

        _col0 = make_temp(p, get_scenecolor(p, 0));
        if (separate)
            _col1 = make_temp(p, get_identity_param(p));
        else
            _col1 = _col0;
    }

    if (twoside) {
        if (!p->state->material_shininess_is_zero) {
            /* Note that we negate the back-face specular exponent here.
             * The negation will be un-done later in the back-face code below.
             */
            struct ureg shininess = get_material(p, 1, STATE_SHININESS);
            emit_op1(p, OPCODE_MOV, dots, WRITEMASK_Z,
                     negate(swizzle1(shininess,X)));
            release_temp(p, shininess);
        }

        _bfc0 = make_temp(p, get_scenecolor(p, 1));
        if (separate)
            _bfc1 = make_temp(p, get_identity_param(p));
        else
            _bfc1 = _bfc0;
    }

    /* If no lights, still need to emit the scenecolor.
     */
    {
        struct ureg res0 = register_output( p, VARYING_SLOT_COL0 );
        emit_op1(p, OPCODE_MOV, res0, 0, _col0);
    }

    if (separate) {
        struct ureg res1 = register_output( p, VARYING_SLOT_COL1 );
        emit_op1(p, OPCODE_MOV, res1, 0, _col1);
    }

    if (twoside) {
        struct ureg res0 = register_output( p, VARYING_SLOT_BFC0 );
        emit_op1(p, OPCODE_MOV, res0, 0, _bfc0);
    }

    if (twoside && separate) {
        struct ureg res1 = register_output( p, VARYING_SLOT_BFC1 );
        emit_op1(p, OPCODE_MOV, res1, 0, _bfc1);
    }

    if (nr_lights == 0) {
        release_temps(p);
        return;
    }

    /* Declare light products first to place them sequentially next to each
     * other for optimal constant uploads.
     */
    struct ureg lightprod_front[MAX_LIGHTS][3];
    struct ureg lightprod_back[MAX_LIGHTS][3];

    for (i = 0; i < MAX_LIGHTS; i++) {
        if (p->state->unit[i].light_enabled) {
            lightprod_front[i][0] = get_lightprod(p, i, 0, STATE_AMBIENT);
            if (twoside)
                lightprod_back[i][0] = get_lightprod(p, i, 1, STATE_AMBIENT);

            lightprod_front[i][1] = get_lightprod(p, i, 0, STATE_DIFFUSE);
            if (twoside)
                lightprod_back[i][1] = get_lightprod(p, i, 1, STATE_DIFFUSE);

            lightprod_front[i][2] = get_lightprod(p, i, 0, STATE_SPECULAR);
            if (twoside)
                lightprod_back[i][2] = get_lightprod(p, i, 1, STATE_SPECULAR);
        }
    }

    /* Add more variables now that we'll use later, so that they are nicely
     * sorted in the parameter list.
     */
    for (i = 0; i < MAX_LIGHTS; i++) {
        if (p->state->unit[i].light_enabled) {
            if (p->state->unit[i].light_eyepos3_is_zero)
                register_param2(p, STATE_LIGHT_POSITION_NORMALIZED, i);
            else
                register_param2(p, STATE_LIGHT_POSITION, i);
        }
    }
    for (i = 0; i < MAX_LIGHTS; i++) {
        if (p->state->unit[i].light_enabled)
            register_param3(p, STATE_LIGHT, i, STATE_ATTENUATION);
    }

    for (i = 0; i < MAX_LIGHTS; i++) {
        if (p->state->unit[i].light_enabled) {
            struct ureg half = undef;
            struct ureg att = undef, VPpli = undef;
            struct ureg dist = undef;

            count++;
            if (p->state->unit[i].light_eyepos3_is_zero) {
                VPpli = register_param2(p, STATE_LIGHT_POSITION_NORMALIZED, i);
            } else {
                struct ureg Ppli = register_param2(p, STATE_LIGHT_POSITION, i);
                struct ureg V = get_eye_position(p);

                VPpli = get_temp(p);
                dist = get_temp(p);

                /* Calculate VPpli vector
                 */
                emit_op2(p, OPCODE_SUB, VPpli, 0, Ppli, V);

                /* Normalize VPpli.  The dist value also used in
                 * attenuation below.
                 */
                emit_op2(p, OPCODE_DP3, dist, 0, VPpli, VPpli);
                emit_op1(p, OPCODE_RSQ, dist, 0, dist);
                emit_op2(p, OPCODE_MUL, VPpli, 0, VPpli, dist);
            }

            /* Calculate attenuation:
             */
            att = calculate_light_attenuation(p, i, VPpli, dist);
            release_temp(p, dist);

            /* Calculate viewer direction, or use infinite viewer:
             */
            if (!p->state->material_shininess_is_zero) {
                if (p->state->light_local_viewer) {
                    struct ureg eye_hat = get_eye_position_normalized(p);
                    half = get_temp(p);
                    emit_op2(p, OPCODE_SUB, half, 0, VPpli, eye_hat);
                    emit_normalize_vec3(p, half, half);
                } else if (p->state->unit[i].light_eyepos3_is_zero) {
                    half = register_param2(p, STATE_LIGHT_HALF_VECTOR, i);
                } else {
                    struct ureg z_dir = swizzle(get_identity_param(p),X,Y,W,Z);
                    half = get_temp(p);
                    emit_op2(p, OPCODE_ADD, half, 0, VPpli, z_dir);
                    emit_normalize_vec3(p, half, half);
                }
            }

            /* Calculate dot products:
             */
            if (p->state->material_shininess_is_zero) {
                emit_op2(p, OPCODE_DP3, dots, 0, normal, VPpli);
            }
            else {
                emit_op2(p, OPCODE_DP3, dots, WRITEMASK_X, normal, VPpli);
                emit_op2(p, OPCODE_DP3, dots, WRITEMASK_Y, normal, half);
            }

            /* Front face lighting:
             */
            {
                struct ureg ambient = lightprod_front[i][0];
                struct ureg diffuse = lightprod_front[i][1];
                struct ureg specular = lightprod_front[i][2];
                struct ureg res0, res1;
                GLuint mask0, mask1;

                if (count == nr_lights) {
                    if (separate) {
                        mask0 = WRITEMASK_XYZ;
                        mask1 = WRITEMASK_XYZ;
                        res0 = register_output( p, VARYING_SLOT_COL0 );
                        res1 = register_output( p, VARYING_SLOT_COL1 );
                    }
                    else {
                        mask0 = 0;
                        mask1 = WRITEMASK_XYZ;
                        res0 = _col0;
                        res1 = register_output( p, VARYING_SLOT_COL0 );
                    }
                }
                else {
                    mask0 = 0;
                    mask1 = 0;
                    res0 = _col0;
                    res1 = _col1;
                }

                if (!is_undef(att)) {
                    /* light is attenuated by distance */
                    emit_op1(p, OPCODE_LIT, lit, 0, dots);
                    emit_op2(p, OPCODE_MUL, lit, 0, lit, att);
                    emit_op3(p, OPCODE_MAD, _col0, 0, swizzle1(lit,X), ambient, _col0);
                }
                else if (!p->state->material_shininess_is_zero) {
                    /* there's a non-zero specular term */
                    emit_op1(p, OPCODE_LIT, lit, 0, dots);
                    emit_op2(p, OPCODE_ADD, _col0, 0, ambient, _col0);
                }
                else {
                    /* no attenutation, no specular */
                    emit_degenerate_lit(p, lit, dots);
                    emit_op2(p, OPCODE_ADD, _col0, 0, ambient, _col0);
                }

                emit_op3(p, OPCODE_MAD, res0, mask0, swizzle1(lit,Y), diffuse, _col0);
                emit_op3(p, OPCODE_MAD, res1, mask1, swizzle1(lit,Z), specular, _col1);

                release_temp(p, ambient);
                release_temp(p, diffuse);
                release_temp(p, specular);
            }

            /* Back face lighting:
             */
            if (twoside) {
                struct ureg ambient = lightprod_back[i][0];
                struct ureg diffuse = lightprod_back[i][1];
                struct ureg specular = lightprod_back[i][2];
                struct ureg res0, res1;
                GLuint mask0, mask1;

                if (count == nr_lights) {
                    if (separate) {
                        mask0 = WRITEMASK_XYZ;
                        mask1 = WRITEMASK_XYZ;
                        res0 = register_output( p, VARYING_SLOT_BFC0 );
                        res1 = register_output( p, VARYING_SLOT_BFC1 );
                    }
                    else {
                        mask0 = 0;
                        mask1 = WRITEMASK_XYZ;
                        res0 = _bfc0;
                        res1 = register_output( p, VARYING_SLOT_BFC0 );
                    }
                }
                else {
                    res0 = _bfc0;
                    res1 = _bfc1;
                    mask0 = 0;
                    mask1 = 0;
                }

                /* For the back face we need to negate the X and Y component
                 * dot products.  dots.Z has the negated back-face specular
                 * exponent.  We swizzle that into the W position.  This
                 * negation makes the back-face specular term positive again.
                 */
                dots = negate(swizzle(dots,X,Y,W,Z));

                if (!is_undef(att)) {
                    emit_op1(p, OPCODE_LIT, lit, 0, dots);
                    emit_op2(p, OPCODE_MUL, lit, 0, lit, att);
                    emit_op3(p, OPCODE_MAD, _bfc0, 0, swizzle1(lit,X), ambient, _bfc0);
                }
                else if (!p->state->material_shininess_is_zero) {
                    emit_op1(p, OPCODE_LIT, lit, 0, dots);
                    emit_op2(p, OPCODE_ADD, _bfc0, 0, ambient, _bfc0); /**/
                }
                else {
                    emit_degenerate_lit(p, lit, dots);
                    emit_op2(p, OPCODE_ADD, _bfc0, 0, ambient, _bfc0);
                }

                emit_op3(p, OPCODE_MAD, res0, mask0, swizzle1(lit,Y), diffuse, _bfc0);
                emit_op3(p, OPCODE_MAD, res1, mask1, swizzle1(lit,Z), specular, _bfc1);
                /* restore dots to its original state for subsequent lights
                 * by negating and swizzling again.
                 */
                dots = negate(swizzle(dots,X,Y,W,Z));

                release_temp(p, ambient);
                release_temp(p, diffuse);
                release_temp(p, specular);
            }

            release_temp(p, half);
            release_temp(p, VPpli);
            release_temp(p, att);
        }
    }

    release_temps( p );
}


static void build_fog( struct tnl_program *p )
{
    struct ureg fog = register_output(p, VARYING_SLOT_FOGC);
    struct ureg input;

    switch (p->state->fog_distance_mode) {
        case FDM_EYE_RADIAL: { /* Z = sqrt(Xe*Xe + Ye*Ye + Ze*Ze) */
            struct ureg tmp = get_temp(p);
            input = get_eye_position(p);
            emit_op2(p, OPCODE_DP3, tmp, WRITEMASK_X, input, input);
            emit_op1(p, OPCODE_RSQ, tmp, WRITEMASK_X, tmp);
            emit_op1(p, OPCODE_RCP, fog, WRITEMASK_X, tmp);
            break;
        }
        case FDM_EYE_PLANE: /* Z = Ze */
            input = get_eye_position_z(p);
            emit_op1(p, OPCODE_MOV, fog, WRITEMASK_X, input);
            break;
        case FDM_EYE_PLANE_ABS: /* Z = abs(Ze) */
            input = get_eye_position_z(p);
            emit_op1(p, OPCODE_ABS, fog, WRITEMASK_X, input);
            break;
        case FDM_FROM_ARRAY:
            input = swizzle1(register_input(p, VERT_ATTRIB_FOG), X);
            emit_op1(p, OPCODE_ABS, fog, WRITEMASK_X, input);
            break;
        default:
            assert(!"Bad fog mode in build_fog()");
            break;
    }

    emit_op1(p, OPCODE_MOV, fog, WRITEMASK_YZW, get_identity_param(p));
}


static void build_reflect_texgen( struct tnl_program *p,
                                  struct ureg dest,
                                  GLuint writemask )
{
    struct ureg normal = get_transformed_normal(p);
    struct ureg eye_hat = get_eye_position_normalized(p);
    struct ureg tmp = get_temp(p);

    /* n.u */
    emit_op2(p, OPCODE_DP3, tmp, 0, normal, eye_hat);
    /* 2n.u */
    emit_op2(p, OPCODE_ADD, tmp, 0, tmp, tmp);
    /* (-2n.u)n + u */
    emit_op3(p, OPCODE_MAD, dest, writemask, negate(tmp), normal, eye_hat);

    release_temp(p, tmp);
}


static void build_sphere_texgen( struct tnl_program *p,
                                 struct ureg dest,
                                 GLuint writemask )
{
    struct ureg normal = get_transformed_normal(p);
    struct ureg eye_hat = get_eye_position_normalized(p);
    struct ureg tmp = get_temp(p);
    struct ureg half = register_scalar_const(p, .5);
    struct ureg r = get_temp(p);
    struct ureg inv_m = get_temp(p);
    struct ureg id = get_identity_param(p);

    /* Could share the above calculations, but it would be
     * a fairly odd state for someone to set (both sphere and
     * reflection active for different texture coordinate
     * components.  Of course - if two texture units enable
     * reflect and/or sphere, things start to tilt in favour
     * of seperating this out:
     */

    /* n.u */
    emit_op2(p, OPCODE_DP3, tmp, 0, normal, eye_hat);
    /* 2n.u */
    emit_op2(p, OPCODE_ADD, tmp, 0, tmp, tmp);
    /* (-2n.u)n + u */
    emit_op3(p, OPCODE_MAD, r, 0, negate(tmp), normal, eye_hat);
    /* r + 0,0,1 */
    emit_op2(p, OPCODE_ADD, tmp, 0, r, swizzle(id,X,Y,W,Z));
    /* rx^2 + ry^2 + (rz+1)^2 */
    emit_op2(p, OPCODE_DP3, tmp, 0, tmp, tmp);
    /* 2/m */
    emit_op1(p, OPCODE_RSQ, tmp, 0, tmp);
    /* 1/m */
    emit_op2(p, OPCODE_MUL, inv_m, 0, tmp, half);
    /* r/m + 1/2 */
    emit_op3(p, OPCODE_MAD, dest, writemask, r, inv_m, half);

    release_temp(p, tmp);
    release_temp(p, r);
    release_temp(p, inv_m);
}


static void build_texture_transform( struct tnl_program *p )
{
    GLuint i, j;

    for (i = 0; i < MAX_TEXTURE_COORD_UNITS; i++) {

        if (!(p->state->fragprog_inputs_read & VARYING_BIT_TEX(i)))
            continue;

        if (p->state->unit[i].coord_replace)
            continue;

        if (p->state->unit[i].texgen_enabled ||
            p->state->unit[i].texmat_enabled) {

            GLuint texmat_enabled = p->state->unit[i].texmat_enabled;
            struct ureg out = register_output(p, VARYING_SLOT_TEX0 + i);
            struct ureg out_texgen = undef;

            if (p->state->unit[i].texgen_enabled) {
                GLuint copy_mask = 0;
                GLuint sphere_mask = 0;
                GLuint reflect_mask = 0;
                GLuint normal_mask = 0;
                GLuint modes[4];

                if (texmat_enabled)
                    out_texgen = get_temp(p);
                else
                    out_texgen = out;

                modes[0] = p->state->unit[i].texgen_mode0;
                modes[1] = p->state->unit[i].texgen_mode1;
                modes[2] = p->state->unit[i].texgen_mode2;
                modes[3] = p->state->unit[i].texgen_mode3;

                for (j = 0; j < 4; j++) {
                    switch (modes[j]) {
                        case TXG_OBJ_LINEAR: {
                            struct ureg obj = register_input(p, VERT_ATTRIB_POS);
                            struct ureg plane =
                                    register_param3(p, STATE_TEXGEN, i,
                                                    STATE_TEXGEN_OBJECT_S + j);

                            emit_op2(p, OPCODE_DP4, out_texgen, WRITEMASK_X << j,
                                     obj, plane );
                            break;
                        }
                        case TXG_EYE_LINEAR: {
                            struct ureg eye = get_eye_position(p);
                            struct ureg plane =
                                    register_param3(p, STATE_TEXGEN, i,
                                                    STATE_TEXGEN_EYE_S + j);

                            emit_op2(p, OPCODE_DP4, out_texgen, WRITEMASK_X << j,
                                     eye, plane );
                            break;
                        }
                        case TXG_SPHERE_MAP:
                            sphere_mask |= WRITEMASK_X << j;
                            break;
                        case TXG_REFLECTION_MAP:
                            reflect_mask |= WRITEMASK_X << j;
                            break;
                        case TXG_NORMAL_MAP:
                            normal_mask |= WRITEMASK_X << j;
                            break;
                        case TXG_NONE:
                            copy_mask |= WRITEMASK_X << j;
                    }
                }

                if (sphere_mask) {
                    build_sphere_texgen(p, out_texgen, sphere_mask);
                }

                if (reflect_mask) {
                    build_reflect_texgen(p, out_texgen, reflect_mask);
                }

                if (normal_mask) {
                    struct ureg normal = get_transformed_normal(p);
                    emit_op1(p, OPCODE_MOV, out_texgen, normal_mask, normal );
                }

                if (copy_mask) {
                    struct ureg in = register_input(p, VERT_ATTRIB_TEX0+i);
                    emit_op1(p, OPCODE_MOV, out_texgen, copy_mask, in );
                }
            }

            if (texmat_enabled) {
                struct ureg texmat[4];
                struct ureg in = (!is_undef(out_texgen) ?
                                  out_texgen :
                                  register_input(p, VERT_ATTRIB_TEX0+i));
                if (p->mvp_with_dp4) {
                    register_matrix_param5( p, STATE_TEXTURE_MATRIX, i, 0, 3,
                                            texmat );
                    emit_matrix_transform_vec4( p, out, texmat, in );
                }
                else {
                    register_matrix_param5( p, STATE_TEXTURE_MATRIX_TRANSPOSE, i, 0, 3,
                                            texmat );
                    emit_transpose_matrix_transform_vec4( p, out, texmat, in );
                }
            }

            release_temps(p);
        }
        else {
            emit_passthrough(p, VERT_ATTRIB_TEX0+i, VARYING_SLOT_TEX0+i);
        }
    }
}


/**
 * Point size attenuation computation.
 */
static void build_atten_pointsize( struct tnl_program *p )
{
    struct ureg eye = get_eye_position_z(p);
    struct ureg state_size = register_param1(p, STATE_POINT_SIZE_CLAMPED);
    struct ureg state_attenuation = register_param1(p, STATE_POINT_ATTENUATION);
    struct ureg out = register_output(p, VARYING_SLOT_PSIZ);
    struct ureg ut = get_temp(p);

    /* dist = |eyez| */
    emit_op1(p, OPCODE_ABS, ut, WRITEMASK_Y, swizzle1(eye, Z));
    /* p1 + dist * (p2 + dist * p3); */
    emit_op3(p, OPCODE_MAD, ut, WRITEMASK_X, swizzle1(ut, Y),
             swizzle1(state_attenuation, Z), swizzle1(state_attenuation, Y));
    emit_op3(p, OPCODE_MAD, ut, WRITEMASK_X, swizzle1(ut, Y),
             ut, swizzle1(state_attenuation, X));

    /* 1 / sqrt(factor) */
    emit_op1(p, OPCODE_RSQ, ut, WRITEMASK_X, ut );

#if 0
    /* out = pointSize / sqrt(factor) */
   emit_op2(p, OPCODE_MUL, out, WRITEMASK_X, ut, state_size);
#else
    /* this is a good place to clamp the point size since there's likely
     * no hardware registers to clamp point size at rasterization time.
     */
    emit_op2(p, OPCODE_MUL, ut, WRITEMASK_X, ut, state_size);
    emit_op2(p, OPCODE_MAX, ut, WRITEMASK_X, ut, swizzle1(state_size, Y));
    emit_op2(p, OPCODE_MIN, out, WRITEMASK_X, ut, swizzle1(state_size, Z));
#endif

    release_temp(p, ut);
}


/**
 * Pass-though per-vertex point size, from user's point size array.
 */
static void build_array_pointsize( struct tnl_program *p )
{
    struct ureg in = register_input(p, VERT_ATTRIB_POINT_SIZE);
    struct ureg out = register_output(p, VARYING_SLOT_PSIZ);
    emit_op1(p, OPCODE_MOV, out, WRITEMASK_X, in);
}


static void build_tnl_program( struct tnl_program *p )
{
    /* Emit the program, starting with the modelview, projection transforms:
     */
    build_hpos(p);

    /* Lighting calculations:
     */
    if (p->state->fragprog_inputs_read & (VARYING_BIT_COL0|VARYING_BIT_COL1)) {
        if (p->state->light_global_enabled)
            build_lighting(p);
        else {
            if (p->state->fragprog_inputs_read & VARYING_BIT_COL0)
                emit_passthrough(p, VERT_ATTRIB_COLOR0, VARYING_SLOT_COL0);

            if (p->state->fragprog_inputs_read & VARYING_BIT_COL1)
                emit_passthrough(p, VERT_ATTRIB_COLOR1, VARYING_SLOT_COL1);
        }
    }

    if (p->state->fragprog_inputs_read & VARYING_BIT_FOGC)
        build_fog(p);

    if (p->state->fragprog_inputs_read & VARYING_BITS_TEX_ANY)
        build_texture_transform(p);

    if (p->state->point_attenuated)
        build_atten_pointsize(p);
    else if (p->state->varying_vp_inputs & VERT_BIT_POINT_SIZE)
        build_array_pointsize(p);

    /* Finish up:
     */
    emit_op1(p, OPCODE_END, undef, 0, undef);

    /* Disassemble:
     */
    if (DISASSEM) {
        printf ("\n");
    }
}


static void
create_new_program( const struct state_key *key,
                    struct gl_program *program,
                    GLboolean mvp_with_dp4,
                    GLuint max_temps)
{
    struct tnl_program p;

    memset(&p, 0, sizeof(p));
    p.state = key;
    p.program = program;
    p.eye_position = undef;
    p.eye_position_z = undef;
    p.eye_position_normalized = undef;
    p.transformed_normal = undef;
    p.identity = undef;
    p.temp_in_use = 0;
    p.mvp_with_dp4 = mvp_with_dp4;

    if (max_temps >= sizeof(int) * 8)
        p.temp_reserved = 0;
    else
        p.temp_reserved = ~((1<<max_temps)-1);

    /* Start by allocating 32 instructions.
     * If we need more, we'll grow the instruction array as needed.
     */
    p.max_inst = 32;
    p.program->arb.Instructions =
            rzalloc_array(program, struct prog_instruction, p.max_inst);
    p.program->String = NULL;
    p.program->arb.NumInstructions =
    p.program->arb.NumTemporaries =
    p.program->arb.NumParameters =
    p.program->arb.NumAttributes = p.program->arb.NumAddressRegs = 0;
    p.program->Parameters = _mesa_new_parameter_list();
    p.program->info.inputs_read = 0;
    p.program->info.outputs_written = 0;
    p.state_params = _mesa_new_parameter_list();

    build_tnl_program( &p );

    _mesa_add_separate_state_parameters(p.program, p.state_params);
    _mesa_free_parameter_list(p.state_params);
}


/**
 * Return a vertex program which implements the current fixed-function
 * transform/lighting/texgen operations.
 */
struct gl_program *
_mesa_get_fixed_func_vertex_program(struct gl_context *ctx)
{
    struct gl_program *prog;
    struct state_key key;

    /* We only update ctx->VertexProgram._VaryingInputs when in VP_MODE_FF _VPMode */
    assert(VP_MODE_FF == ctx->VertexProgram._VPMode);

    /* Grab all the relevant state and put it in a single structure:
     */
    make_state_key(ctx, &key);

    /* Look for an already-prepared program for this state:
     */
    prog = _mesa_search_program_cache(ctx->VertexProgram.Cache, &key,
                                      sizeof(key));

    if (!prog) {
        /* OK, we'll have to build a new one */
        if (0)
            printf("Build new TNL program\n");

        prog = ctx->Driver.NewProgram(ctx, MESA_SHADER_VERTEX, 0, true);
        if (!prog)
            return NULL;

        create_new_program( &key, prog,
                            ctx->Const.ShaderCompilerOptions[MESA_SHADER_VERTEX].OptimizeForAOS,
                            ctx->Const.Program[MESA_SHADER_VERTEX].MaxTemps );

        if (ctx->Driver.ProgramStringNotify)
            ctx->Driver.ProgramStringNotify(ctx, GL_VERTEX_PROGRAM_ARB, prog);

        _mesa_program_cache_insert(ctx, ctx->VertexProgram.Cache, &key,
                                   sizeof(key), prog);
    }

    return prog;
}
