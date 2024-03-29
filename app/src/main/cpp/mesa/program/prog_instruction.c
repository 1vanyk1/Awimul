#include <stdio.h>
#include <assert.h>

#include "../main/glheader.h"
#include "prog_instruction.h"
#include "prog_parameter.h"


/**
 * Initialize program instruction fields to defaults.
 * \param inst  first instruction to initialize
 * \param count  number of instructions to initialize
 */
void
_mesa_init_instructions(struct prog_instruction *inst, GLuint count)
{
    GLuint i;

    memset(inst, 0, count * sizeof(struct prog_instruction));

    for (i = 0; i < count; i++) {
        inst[i].SrcReg[0].File = PROGRAM_UNDEFINED;
        inst[i].SrcReg[0].Swizzle = SWIZZLE_NOOP;
        inst[i].SrcReg[1].File = PROGRAM_UNDEFINED;
        inst[i].SrcReg[1].Swizzle = SWIZZLE_NOOP;
        inst[i].SrcReg[2].File = PROGRAM_UNDEFINED;
        inst[i].SrcReg[2].Swizzle = SWIZZLE_NOOP;

        inst[i].DstReg.File = PROGRAM_UNDEFINED;
        inst[i].DstReg.WriteMask = WRITEMASK_XYZW;

        inst[i].Saturate = GL_FALSE;
    }
}


/**
 * Copy an array of program instructions.
 * \param dest  pointer to destination.
 * \param src  pointer to source.
 * \param n  number of instructions to copy.
 * \return pointer to destination.
 */
struct prog_instruction *
_mesa_copy_instructions(struct prog_instruction *dest,
                        const struct prog_instruction *src, GLuint n)
{
    memcpy(dest, src, n * sizeof(struct prog_instruction));
    return dest;
}


/**
 * Basic info about each instruction
 */
struct instruction_info
{
    enum prog_opcode Opcode;
    const char *Name;
    GLuint NumSrcRegs;
    GLuint NumDstRegs;
};

/**
 * Instruction info
 * \note Opcode should equal array index!
 */
static const struct instruction_info InstInfo[MAX_OPCODE] = {
        { OPCODE_NOP,    "NOP",     0, 0 },
        { OPCODE_ABS,    "ABS",     1, 1 },
        { OPCODE_ADD,    "ADD",     2, 1 },
        { OPCODE_ARL,    "ARL",     1, 1 },
        { OPCODE_BGNLOOP,"BGNLOOP", 0, 0 },
        { OPCODE_BGNSUB, "BGNSUB",  0, 0 },
        { OPCODE_BRK,    "BRK",     0, 0 },
        { OPCODE_CAL,    "CAL",     0, 0 },
        { OPCODE_CMP,    "CMP",     3, 1 },
        { OPCODE_CONT,   "CONT",    0, 0 },
        { OPCODE_COS,    "COS",     1, 1 },
        { OPCODE_DDX,    "DDX",     1, 1 },
        { OPCODE_DDY,    "DDY",     1, 1 },
        { OPCODE_DP2,    "DP2",     2, 1 },
        { OPCODE_DP3,    "DP3",     2, 1 },
        { OPCODE_DP4,    "DP4",     2, 1 },
        { OPCODE_DPH,    "DPH",     2, 1 },
        { OPCODE_DST,    "DST",     2, 1 },
        { OPCODE_ELSE,   "ELSE",    0, 0 },
        { OPCODE_END,    "END",     0, 0 },
        { OPCODE_ENDIF,  "ENDIF",   0, 0 },
        { OPCODE_ENDLOOP,"ENDLOOP", 0, 0 },
        { OPCODE_ENDSUB, "ENDSUB",  0, 0 },
        { OPCODE_EX2,    "EX2",     1, 1 },
        { OPCODE_EXP,    "EXP",     1, 1 },
        { OPCODE_FLR,    "FLR",     1, 1 },
        { OPCODE_FRC,    "FRC",     1, 1 },
        { OPCODE_IF,     "IF",      1, 0 },
        { OPCODE_KIL,    "KIL",     1, 0 },
        { OPCODE_LG2,    "LG2",     1, 1 },
        { OPCODE_LIT,    "LIT",     1, 1 },
        { OPCODE_LOG,    "LOG",     1, 1 },
        { OPCODE_LRP,    "LRP",     3, 1 },
        { OPCODE_MAD,    "MAD",     3, 1 },
        { OPCODE_MAX,    "MAX",     2, 1 },
        { OPCODE_MIN,    "MIN",     2, 1 },
        { OPCODE_MOV,    "MOV",     1, 1 },
        { OPCODE_MUL,    "MUL",     2, 1 },
        { OPCODE_NOISE1, "NOISE1",  1, 1 },
        { OPCODE_NOISE2, "NOISE2",  1, 1 },
        { OPCODE_NOISE3, "NOISE3",  1, 1 },
        { OPCODE_NOISE4, "NOISE4",  1, 1 },
        { OPCODE_POW,    "POW",     2, 1 },
        { OPCODE_RCP,    "RCP",     1, 1 },
        { OPCODE_RET,    "RET",     0, 0 },
        { OPCODE_RSQ,    "RSQ",     1, 1 },
        { OPCODE_SCS,    "SCS",     1, 1 },
        { OPCODE_SGE,    "SGE",     2, 1 },
        { OPCODE_SIN,    "SIN",     1, 1 },
        { OPCODE_SLT,    "SLT",     2, 1 },
        { OPCODE_SSG,    "SSG",     1, 1 },
        { OPCODE_SUB,    "SUB",     2, 1 },
        { OPCODE_SWZ,    "SWZ",     1, 1 },
        { OPCODE_TEX,    "TEX",     1, 1 },
        { OPCODE_TXB,    "TXB",     1, 1 },
        { OPCODE_TXD,    "TXD",     3, 1 },
        { OPCODE_TXL,    "TXL",     1, 1 },
        { OPCODE_TXP,    "TXP",     1, 1 },
        { OPCODE_TRUNC,  "TRUNC",   1, 1 },
        { OPCODE_XPD,    "XPD",     2, 1 }
};


/**
 * Return the number of src registers for the given instruction/opcode.
 */
GLuint
_mesa_num_inst_src_regs(enum prog_opcode opcode)
{
    assert(opcode < MAX_OPCODE);
    assert(opcode == InstInfo[opcode].Opcode);
    assert(OPCODE_XPD == InstInfo[OPCODE_XPD].Opcode);
    return InstInfo[opcode].NumSrcRegs;
}


/**
 * Return the number of dst registers for the given instruction/opcode.
 */
GLuint
_mesa_num_inst_dst_regs(enum prog_opcode opcode)
{
    assert(opcode < MAX_OPCODE);
    assert(opcode == InstInfo[opcode].Opcode);
    assert(OPCODE_XPD == InstInfo[OPCODE_XPD].Opcode);
    return InstInfo[opcode].NumDstRegs;
}


GLboolean
_mesa_is_tex_instruction(enum prog_opcode opcode)
{
    return (opcode == OPCODE_TEX ||
            opcode == OPCODE_TXB ||
            opcode == OPCODE_TXD ||
            opcode == OPCODE_TXL ||
            opcode == OPCODE_TXP);
}


/**
 * Check if there's a potential src/dst register data dependency when
 * using SOA execution.
 * Example:
 *   MOV T, T.yxwz;
 * This would expand into:
 *   MOV t0, t1;
 *   MOV t1, t0;
 *   MOV t2, t3;
 *   MOV t3, t2;
 * The second instruction will have the wrong value for t0 if executed as-is.
 */
GLboolean
_mesa_check_soa_dependencies(const struct prog_instruction *inst)
{
    GLuint i, chan;

    if (inst->DstReg.WriteMask == WRITEMASK_X ||
        inst->DstReg.WriteMask == WRITEMASK_Y ||
        inst->DstReg.WriteMask == WRITEMASK_Z ||
        inst->DstReg.WriteMask == WRITEMASK_W ||
        inst->DstReg.WriteMask == 0x0) {
        /* no chance of data dependency */
        return GL_FALSE;
    }

    /* loop over src regs */
    for (i = 0; i < 3; i++) {
        if (inst->SrcReg[i].File == inst->DstReg.File &&
            inst->SrcReg[i].Index == inst->DstReg.Index) {
            /* loop over dest channels */
            GLuint channelsWritten = 0x0;
            for (chan = 0; chan < 4; chan++) {
                if (inst->DstReg.WriteMask & (1 << chan)) {
                    /* check if we're reading a channel that's been written */
                    GLuint swizzle = GET_SWZ(inst->SrcReg[i].Swizzle, chan);
                    if (swizzle <= SWIZZLE_W &&
                        (channelsWritten & (1 << swizzle))) {
                        return GL_TRUE;
                    }

                    channelsWritten |= (1 << chan);
                }
            }
        }
    }
    return GL_FALSE;
}


/**
 * Return string name for given program opcode.
 */
const char *
_mesa_opcode_string(enum prog_opcode opcode)
{
    if (opcode < MAX_OPCODE)
        return InstInfo[opcode].Name;
    else {
        static char s[20];
        snprintf(s, sizeof(s), "OP%u", opcode);
        return s;
    }
}

