#include "../main/glheader.h"
#include "../main/context.h"
#include "prog_parameter.h"
#include "prog_statevars.h"
#include "program.h"
#include "programopt.h"
#include "prog_instruction.h"


/**
 * This function inserts instructions for coordinate modelview * projection
 * into a vertex program.
 * May be used to implement the position_invariant option.
 */
static void
insert_mvp_dp4_code(struct gl_context *ctx, struct gl_program *vprog)
{
    struct prog_instruction *newInst;
    const GLuint origLen = vprog->arb.NumInstructions;
    const GLuint newLen = origLen + 4;
    GLuint i;

    /*
     * Setup state references for the modelview/projection matrix.
     * XXX we should check if these state vars are already declared.
     */
    static const gl_state_index16 mvpState[4][STATE_LENGTH] = {
            { STATE_MVP_MATRIX, 0, 0, 0 },  /* state.matrix.mvp.row[0] */
            { STATE_MVP_MATRIX, 0, 1, 1 },  /* state.matrix.mvp.row[1] */
            { STATE_MVP_MATRIX, 0, 2, 2 },  /* state.matrix.mvp.row[2] */
            { STATE_MVP_MATRIX, 0, 3, 3 },  /* state.matrix.mvp.row[3] */
    };
    GLint mvpRef[4];

    for (i = 0; i < 4; i++) {
        mvpRef[i] = _mesa_add_state_reference(vprog->Parameters, mvpState[i]);
    }

    /* Alloc storage for new instructions */
    newInst = rzalloc_array(vprog, struct prog_instruction, newLen);
    if (!newInst) {
        _mesa_error(ctx, GL_OUT_OF_MEMORY,
                    "glProgramString(inserting position_invariant code)");
        return;
    }

    /*
     * Generated instructions:
     * newInst[0] = DP4 result.position.x, mvp.row[0], vertex.position;
     * newInst[1] = DP4 result.position.y, mvp.row[1], vertex.position;
     * newInst[2] = DP4 result.position.z, mvp.row[2], vertex.position;
     * newInst[3] = DP4 result.position.w, mvp.row[3], vertex.position;
     */
    _mesa_init_instructions(newInst, 4);
    for (i = 0; i < 4; i++) {
        newInst[i].Opcode = OPCODE_DP4;
        newInst[i].DstReg.File = PROGRAM_OUTPUT;
        newInst[i].DstReg.Index = VARYING_SLOT_POS;
        newInst[i].DstReg.WriteMask = (WRITEMASK_X << i);
        newInst[i].SrcReg[0].File = PROGRAM_STATE_VAR;
        newInst[i].SrcReg[0].Index = mvpRef[i];
        newInst[i].SrcReg[0].Swizzle = SWIZZLE_NOOP;
        newInst[i].SrcReg[1].File = PROGRAM_INPUT;
        newInst[i].SrcReg[1].Index = VERT_ATTRIB_POS;
        newInst[i].SrcReg[1].Swizzle = SWIZZLE_NOOP;
    }

    /* Append original instructions after new instructions */
    _mesa_copy_instructions (newInst + 4, vprog->arb.Instructions, origLen);

    /* free old instructions */
    ralloc_free(vprog->arb.Instructions);

    /* install new instructions */
    vprog->arb.Instructions = newInst;
    vprog->arb.NumInstructions = newLen;
    vprog->info.inputs_read |= VERT_BIT_POS;
    vprog->info.outputs_written |= BITFIELD64_BIT(VARYING_SLOT_POS);
}


static void
insert_mvp_mad_code(struct gl_context *ctx, struct gl_program *vprog)
{
    struct prog_instruction *newInst;
    const GLuint origLen = vprog->arb.NumInstructions;
    const GLuint newLen = origLen + 4;
    GLuint hposTemp;
    GLuint i;

    /*
     * Setup state references for the modelview/projection matrix.
     * XXX we should check if these state vars are already declared.
     */
    static const gl_state_index16 mvpState[4][STATE_LENGTH] = {
            { STATE_MVP_MATRIX_TRANSPOSE, 0, 0, 0 },
            { STATE_MVP_MATRIX_TRANSPOSE, 0, 1, 1 },
            { STATE_MVP_MATRIX_TRANSPOSE, 0, 2, 2 },
            { STATE_MVP_MATRIX_TRANSPOSE, 0, 3, 3 },
    };
    GLint mvpRef[4];

    for (i = 0; i < 4; i++) {
        mvpRef[i] = _mesa_add_state_reference(vprog->Parameters, mvpState[i]);
    }

    /* Alloc storage for new instructions */
    newInst = rzalloc_array(vprog, struct prog_instruction, newLen);
    if (!newInst) {
        _mesa_error(ctx, GL_OUT_OF_MEMORY,
                    "glProgramString(inserting position_invariant code)");
        return;
    }

    /* TEMP hposTemp; */
    hposTemp = vprog->arb.NumTemporaries++;

    /*
     * Generated instructions:
     *    emit_op2(p, OPCODE_MUL, tmp, 0, swizzle1(src,X), mat[0]);
     *    emit_op3(p, OPCODE_MAD, tmp, 0, swizzle1(src,Y), mat[1], tmp);
     *    emit_op3(p, OPCODE_MAD, tmp, 0, swizzle1(src,Z), mat[2], tmp);
     *    emit_op3(p, OPCODE_MAD, dest, 0, swizzle1(src,W), mat[3], tmp);
     */
    _mesa_init_instructions(newInst, 4);

    newInst[0].Opcode = OPCODE_MUL;
    newInst[0].DstReg.File = PROGRAM_TEMPORARY;
    newInst[0].DstReg.Index = hposTemp;
    newInst[0].DstReg.WriteMask = WRITEMASK_XYZW;
    newInst[0].SrcReg[0].File = PROGRAM_INPUT;
    newInst[0].SrcReg[0].Index = VERT_ATTRIB_POS;
    newInst[0].SrcReg[0].Swizzle = SWIZZLE_XXXX;
    newInst[0].SrcReg[1].File = PROGRAM_STATE_VAR;
    newInst[0].SrcReg[1].Index = mvpRef[0];
    newInst[0].SrcReg[1].Swizzle = SWIZZLE_NOOP;

    for (i = 1; i <= 2; i++) {
        newInst[i].Opcode = OPCODE_MAD;
        newInst[i].DstReg.File = PROGRAM_TEMPORARY;
        newInst[i].DstReg.Index = hposTemp;
        newInst[i].DstReg.WriteMask = WRITEMASK_XYZW;
        newInst[i].SrcReg[0].File = PROGRAM_INPUT;
        newInst[i].SrcReg[0].Index = VERT_ATTRIB_POS;
        newInst[i].SrcReg[0].Swizzle = MAKE_SWIZZLE4(i,i,i,i);
        newInst[i].SrcReg[1].File = PROGRAM_STATE_VAR;
        newInst[i].SrcReg[1].Index = mvpRef[i];
        newInst[i].SrcReg[1].Swizzle = SWIZZLE_NOOP;
        newInst[i].SrcReg[2].File = PROGRAM_TEMPORARY;
        newInst[i].SrcReg[2].Index = hposTemp;
        newInst[1].SrcReg[2].Swizzle = SWIZZLE_NOOP;
    }

    newInst[3].Opcode = OPCODE_MAD;
    newInst[3].DstReg.File = PROGRAM_OUTPUT;
    newInst[3].DstReg.Index = VARYING_SLOT_POS;
    newInst[3].DstReg.WriteMask = WRITEMASK_XYZW;
    newInst[3].SrcReg[0].File = PROGRAM_INPUT;
    newInst[3].SrcReg[0].Index = VERT_ATTRIB_POS;
    newInst[3].SrcReg[0].Swizzle = SWIZZLE_WWWW;
    newInst[3].SrcReg[1].File = PROGRAM_STATE_VAR;
    newInst[3].SrcReg[1].Index = mvpRef[3];
    newInst[3].SrcReg[1].Swizzle = SWIZZLE_NOOP;
    newInst[3].SrcReg[2].File = PROGRAM_TEMPORARY;
    newInst[3].SrcReg[2].Index = hposTemp;
    newInst[3].SrcReg[2].Swizzle = SWIZZLE_NOOP;


    /* Append original instructions after new instructions */
    _mesa_copy_instructions (newInst + 4, vprog->arb.Instructions, origLen);

    /* free old instructions */
    ralloc_free(vprog->arb.Instructions);

    /* install new instructions */
    vprog->arb.Instructions = newInst;
    vprog->arb.NumInstructions = newLen;
    vprog->info.inputs_read |= VERT_BIT_POS;
    vprog->info.outputs_written |= BITFIELD64_BIT(VARYING_SLOT_POS);
}


void
_mesa_insert_mvp_code(struct gl_context *ctx, struct gl_program *vprog)
{
    if (ctx->Const.ShaderCompilerOptions[MESA_SHADER_VERTEX].OptimizeForAOS)
        insert_mvp_dp4_code( ctx, vprog );
    else
        insert_mvp_mad_code( ctx, vprog );
}






/**
 * Append instructions to implement fog
 *
 * The \c fragment.fogcoord input is used to compute the fog blend factor.
 *
 * \param ctx      The GL context
 * \param fprog    Fragment program that fog instructions will be appended to.
 * \param fog_mode Fog mode.  One of \c GL_EXP, \c GL_EXP2, or \c GL_LINEAR.
 * \param saturate True if writes to color outputs should be clamped to [0, 1]
 *
 * \note
 * This function sets \c VARYING_BIT_FOGC in \c fprog->info.inputs_read.
 *
 * \todo With a little work, this function could be adapted to add fog code
 * to vertex programs too.
 */
void
_mesa_append_fog_code(struct gl_context *ctx, struct gl_program *fprog,
                      GLenum fog_mode, GLboolean saturate)
{
    static const gl_state_index16 fogPStateOpt[STATE_LENGTH]
            = { STATE_FOG_PARAMS_OPTIMIZED, 0, 0 };
    static const gl_state_index16 fogColorState[STATE_LENGTH]
            = { STATE_FOG_COLOR, 0, 0, 0 };
    struct prog_instruction *newInst, *inst;
    const GLuint origLen = fprog->arb.NumInstructions;
    const GLuint newLen = origLen + 5;
    GLuint i;
    GLint fogPRefOpt, fogColorRef; /* state references */
    GLuint colorTemp, fogFactorTemp; /* temporary registerss */

    if (fog_mode == GL_NONE) {
        _mesa_problem(ctx, "_mesa_append_fog_code() called for fragment program"
                           " with fog_mode == GL_NONE");
        return;
    }

    if (!(fprog->info.outputs_written & (1 << FRAG_RESULT_COLOR))) {
        /* program doesn't output color, so nothing to do */
        return;
    }

    /* Alloc storage for new instructions */
    newInst = rzalloc_array(fprog, struct prog_instruction, newLen);
    if (!newInst) {
        _mesa_error(ctx, GL_OUT_OF_MEMORY,
                    "glProgramString(inserting fog_option code)");
        return;
    }

    /* Copy orig instructions into new instruction buffer */
    _mesa_copy_instructions(newInst, fprog->arb.Instructions, origLen);

    /* PARAM fogParamsRefOpt = internal optimized fog params; */
    fogPRefOpt
            = _mesa_add_state_reference(fprog->Parameters, fogPStateOpt);
    /* PARAM fogColorRef = state.fog.color; */
    fogColorRef
            = _mesa_add_state_reference(fprog->Parameters, fogColorState);

    /* TEMP colorTemp; */
    colorTemp = fprog->arb.NumTemporaries++;
    /* TEMP fogFactorTemp; */
    fogFactorTemp = fprog->arb.NumTemporaries++;

    /* Scan program to find where result.color is written */
    inst = newInst;
    for (i = 0; i < fprog->arb.NumInstructions; i++) {
        if (inst->Opcode == OPCODE_END)
            break;
        if (inst->DstReg.File == PROGRAM_OUTPUT &&
            inst->DstReg.Index == FRAG_RESULT_COLOR) {
            /* change the instruction to write to colorTemp w/ clamping */
            inst->DstReg.File = PROGRAM_TEMPORARY;
            inst->DstReg.Index = colorTemp;
            inst->Saturate = saturate;
            /* don't break (may be several writes to result.color) */
        }
        inst++;
    }
    assert(inst->Opcode == OPCODE_END); /* we'll overwrite this inst */

    _mesa_init_instructions(inst, 5);

    /* emit instructions to compute fog blending factor */
    /* this is always clamped to [0, 1] regardless of fragment clamping */
    if (fog_mode == GL_LINEAR) {
        /* MAD fogFactorTemp.x, fragment.fogcoord.x, fogPRefOpt.x, fogPRefOpt.y; */
        inst->Opcode = OPCODE_MAD;
        inst->DstReg.File = PROGRAM_TEMPORARY;
        inst->DstReg.Index = fogFactorTemp;
        inst->DstReg.WriteMask = WRITEMASK_X;
        inst->SrcReg[0].File = PROGRAM_INPUT;
        inst->SrcReg[0].Index = VARYING_SLOT_FOGC;
        inst->SrcReg[0].Swizzle = SWIZZLE_XXXX;
        inst->SrcReg[1].File = PROGRAM_STATE_VAR;
        inst->SrcReg[1].Index = fogPRefOpt;
        inst->SrcReg[1].Swizzle = SWIZZLE_XXXX;
        inst->SrcReg[2].File = PROGRAM_STATE_VAR;
        inst->SrcReg[2].Index = fogPRefOpt;
        inst->SrcReg[2].Swizzle = SWIZZLE_YYYY;
        inst->Saturate = GL_TRUE;
        inst++;
    }
    else {
        assert(fog_mode == GL_EXP || fog_mode == GL_EXP2);
        /* fogPRefOpt.z = d/ln(2), fogPRefOpt.w = d/sqrt(ln(2) */
        /* EXP: MUL fogFactorTemp.x, fogPRefOpt.z, fragment.fogcoord.x; */
        /* EXP2: MUL fogFactorTemp.x, fogPRefOpt.w, fragment.fogcoord.x; */
        inst->Opcode = OPCODE_MUL;
        inst->DstReg.File = PROGRAM_TEMPORARY;
        inst->DstReg.Index = fogFactorTemp;
        inst->DstReg.WriteMask = WRITEMASK_X;
        inst->SrcReg[0].File = PROGRAM_STATE_VAR;
        inst->SrcReg[0].Index = fogPRefOpt;
        inst->SrcReg[0].Swizzle
                = (fog_mode == GL_EXP) ? SWIZZLE_ZZZZ : SWIZZLE_WWWW;
        inst->SrcReg[1].File = PROGRAM_INPUT;
        inst->SrcReg[1].Index = VARYING_SLOT_FOGC;
        inst->SrcReg[1].Swizzle = SWIZZLE_XXXX;
        inst++;
        if (fog_mode == GL_EXP2) {
            /* MUL fogFactorTemp.x, fogFactorTemp.x, fogFactorTemp.x; */
            inst->Opcode = OPCODE_MUL;
            inst->DstReg.File = PROGRAM_TEMPORARY;
            inst->DstReg.Index = fogFactorTemp;
            inst->DstReg.WriteMask = WRITEMASK_X;
            inst->SrcReg[0].File = PROGRAM_TEMPORARY;
            inst->SrcReg[0].Index = fogFactorTemp;
            inst->SrcReg[0].Swizzle = SWIZZLE_XXXX;
            inst->SrcReg[1].File = PROGRAM_TEMPORARY;
            inst->SrcReg[1].Index = fogFactorTemp;
            inst->SrcReg[1].Swizzle = SWIZZLE_XXXX;
            inst++;
        }
        /* EX2_SAT fogFactorTemp.x, -fogFactorTemp.x; */
        inst->Opcode = OPCODE_EX2;
        inst->DstReg.File = PROGRAM_TEMPORARY;
        inst->DstReg.Index = fogFactorTemp;
        inst->DstReg.WriteMask = WRITEMASK_X;
        inst->SrcReg[0].File = PROGRAM_TEMPORARY;
        inst->SrcReg[0].Index = fogFactorTemp;
        inst->SrcReg[0].Negate = NEGATE_XYZW;
        inst->SrcReg[0].Swizzle = SWIZZLE_XXXX;
        inst->Saturate = GL_TRUE;
        inst++;
    }
    /* LRP result.color.xyz, fogFactorTemp.xxxx, colorTemp, fogColorRef; */
    inst->Opcode = OPCODE_LRP;
    inst->DstReg.File = PROGRAM_OUTPUT;
    inst->DstReg.Index = FRAG_RESULT_COLOR;
    inst->DstReg.WriteMask = WRITEMASK_XYZ;
    inst->SrcReg[0].File = PROGRAM_TEMPORARY;
    inst->SrcReg[0].Index = fogFactorTemp;
    inst->SrcReg[0].Swizzle = SWIZZLE_XXXX;
    inst->SrcReg[1].File = PROGRAM_TEMPORARY;
    inst->SrcReg[1].Index = colorTemp;
    inst->SrcReg[1].Swizzle = SWIZZLE_NOOP;
    inst->SrcReg[2].File = PROGRAM_STATE_VAR;
    inst->SrcReg[2].Index = fogColorRef;
    inst->SrcReg[2].Swizzle = SWIZZLE_NOOP;
    inst++;
    /* MOV result.color.w, colorTemp.x;  # copy alpha */
    inst->Opcode = OPCODE_MOV;
    inst->DstReg.File = PROGRAM_OUTPUT;
    inst->DstReg.Index = FRAG_RESULT_COLOR;
    inst->DstReg.WriteMask = WRITEMASK_W;
    inst->SrcReg[0].File = PROGRAM_TEMPORARY;
    inst->SrcReg[0].Index = colorTemp;
    inst->SrcReg[0].Swizzle = SWIZZLE_NOOP;
    inst++;
    /* END; */
    inst->Opcode = OPCODE_END;
    inst++;

    /* free old instructions */
    ralloc_free(fprog->arb.Instructions);

    /* install new instructions */
    fprog->arb.Instructions = newInst;
    fprog->arb.NumInstructions = inst - newInst;
    fprog->info.inputs_read |= VARYING_BIT_FOGC;
    assert(fprog->info.outputs_written & (1 << FRAG_RESULT_COLOR));
}



static GLboolean
is_texture_instruction(const struct prog_instruction *inst)
{
    switch (inst->Opcode) {
        case OPCODE_TEX:
        case OPCODE_TXB:
        case OPCODE_TXD:
        case OPCODE_TXL:
        case OPCODE_TXP:
            return GL_TRUE;
        default:
            return GL_FALSE;
    }
}


/**
 * Count the number of texure indirections in the given program.
 * The program's NumTexIndirections field will be updated.
 * See the GL_ARB_fragment_program spec (issue 24) for details.
 * XXX we count texture indirections in texenvprogram.c (maybe use this code
 * instead and elsewhere).
 */
void
_mesa_count_texture_indirections(struct gl_program *prog)
{
    GLuint indirections = 1;
    GLbitfield tempsOutput = 0x0;
    GLbitfield aluTemps = 0x0;
    GLuint i;

    for (i = 0; i < prog->arb.NumInstructions; i++) {
        const struct prog_instruction *inst = prog->arb.Instructions + i;

        if (is_texture_instruction(inst)) {
            if (((inst->SrcReg[0].File == PROGRAM_TEMPORARY) &&
                 (tempsOutput & (1 << inst->SrcReg[0].Index))) ||
                ((inst->Opcode != OPCODE_KIL) &&
                 (inst->DstReg.File == PROGRAM_TEMPORARY) &&
                 (aluTemps & (1 << inst->DstReg.Index))))
            {
                indirections++;
                tempsOutput = 0x0;
                aluTemps = 0x0;
            }
        }
        else {
            GLuint j;
            for (j = 0; j < 3; j++) {
                if (inst->SrcReg[j].File == PROGRAM_TEMPORARY)
                    aluTemps |= (1 << inst->SrcReg[j].Index);
            }
            if (inst->DstReg.File == PROGRAM_TEMPORARY)
                aluTemps |= (1 << inst->DstReg.Index);
        }

        if ((inst->Opcode != OPCODE_KIL) && (inst->DstReg.File == PROGRAM_TEMPORARY))
            tempsOutput |= (1 << inst->DstReg.Index);
    }

    prog->arb.NumTexIndirections = indirections;
}


/**
 * Count number of texture instructions in given program and update the
 * program's NumTexInstructions field.
 */
void
_mesa_count_texture_instructions(struct gl_program *prog)
{
    GLuint i;
    prog->arb.NumTexInstructions = 0;
    for (i = 0; i < prog->arb.NumInstructions; i++) {
        prog->arb.NumTexInstructions +=
                is_texture_instruction(prog->arb.Instructions + i);
    }
}


/**
 * Scan/rewrite program to remove reads of custom (output) registers.
 * The passed type has to be PROGRAM_OUTPUT.
 * On some hardware, trying to read an output register causes trouble.
 * So, rewrite the program to use a temporary register in this case.
 */
void
_mesa_remove_output_reads(struct gl_program *prog, gl_register_file type)
{
    GLuint i;
    GLint outputMap[VARYING_SLOT_MAX];
    GLuint numVaryingReads = 0;
    GLboolean usedTemps[MAX_PROGRAM_TEMPS];
    GLuint firstTemp = 0;

    _mesa_find_used_registers(prog, PROGRAM_TEMPORARY,
                              usedTemps, MAX_PROGRAM_TEMPS);

    assert(type == PROGRAM_OUTPUT);

    for (i = 0; i < VARYING_SLOT_MAX; i++)
        outputMap[i] = -1;

    /* look for instructions which read from varying vars */
    for (i = 0; i < prog->arb.NumInstructions; i++) {
        struct prog_instruction *inst = prog->arb.Instructions + i;
        const GLuint numSrc = _mesa_num_inst_src_regs(inst->Opcode);
        GLuint j;
        for (j = 0; j < numSrc; j++) {
            if (inst->SrcReg[j].File == type) {
                /* replace the read with a temp reg */
                const GLuint var = inst->SrcReg[j].Index;
                if (outputMap[var] == -1) {
                    numVaryingReads++;
                    outputMap[var] = _mesa_find_free_register(usedTemps,
                                                              MAX_PROGRAM_TEMPS,
                                                              firstTemp);
                    firstTemp = outputMap[var] + 1;
                }
                inst->SrcReg[j].File = PROGRAM_TEMPORARY;
                inst->SrcReg[j].Index = outputMap[var];
            }
        }
    }

    if (numVaryingReads == 0)
        return; /* nothing to be done */

    /* look for instructions which write to the varying vars identified above */
    for (i = 0; i < prog->arb.NumInstructions; i++) {
        struct prog_instruction *inst = prog->arb.Instructions + i;
        if (inst->DstReg.File == type &&
            outputMap[inst->DstReg.Index] >= 0) {
            /* change inst to write to the temp reg, instead of the varying */
            inst->DstReg.File = PROGRAM_TEMPORARY;
            inst->DstReg.Index = outputMap[inst->DstReg.Index];
        }
    }

    /* insert new instructions to copy the temp vars to the varying vars */
    {
        struct prog_instruction *inst;
        GLint endPos, var;

        /* Look for END instruction and insert the new varying writes */
        endPos = -1;
        for (i = 0; i < prog->arb.NumInstructions; i++) {
            struct prog_instruction *inst = prog->arb.Instructions + i;
            if (inst->Opcode == OPCODE_END) {
                endPos = i;
                _mesa_insert_instructions(prog, i, numVaryingReads);
                break;
            }
        }

        assert(endPos >= 0);

        /* insert new MOV instructions here */
        inst = prog->arb.Instructions + endPos;
        for (var = 0; var < VARYING_SLOT_MAX; var++) {
            if (outputMap[var] >= 0) {
                /* MOV VAR[var], TEMP[tmp]; */
                inst->Opcode = OPCODE_MOV;
                inst->DstReg.File = type;
                inst->DstReg.Index = var;
                inst->SrcReg[0].File = PROGRAM_TEMPORARY;
                inst->SrcReg[0].Index = outputMap[var];
                inst++;
            }
        }
    }
}

void
_mesa_program_fragment_position_to_sysval(struct gl_program *prog)
{
    GLuint i;

    if (prog->Target != GL_FRAGMENT_PROGRAM_ARB ||
        !(prog->info.inputs_read & BITFIELD64_BIT(VARYING_SLOT_POS)))
        return;

    prog->info.inputs_read &= ~BITFIELD64_BIT(VARYING_SLOT_POS);
    BITSET_SET(prog->info.system_values_read, SYSTEM_VALUE_FRAG_COORD);

    for (i = 0; i < prog->arb.NumInstructions; i++) {
        struct prog_instruction *inst = prog->arb.Instructions + i;
        const GLuint numSrc = _mesa_num_inst_src_regs(inst->Opcode);
        GLuint j;

        for (j = 0; j < numSrc; j++) {
            if (inst->SrcReg[j].File == PROGRAM_INPUT &&
                inst->SrcReg[j].Index == VARYING_SLOT_POS) {
                inst->SrcReg[j].File = PROGRAM_SYSTEM_VALUE;
                inst->SrcReg[j].Index = SYSTEM_VALUE_FRAG_COORD;
            }
        }
    }
}
