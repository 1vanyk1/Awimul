#include "../main/glheader.h"
#include "../main/context.h"
#include "../main/macros.h"
#include "program.h"
#include "prog_instruction.h"
#include "prog_optimize.h"
#include "prog_parameter.h"
#include <stdbool.h>

static bool
src_regs_are_constant(const struct prog_instruction *inst, unsigned num_srcs)
{
    unsigned i;

    for (i = 0; i < num_srcs; i++) {
        if (inst->SrcReg[i].File != PROGRAM_CONSTANT)
            return false;
        if (inst->SrcReg[i].RelAddr)
            return false;
    }

    return true;
}

static struct prog_src_register
src_reg_for_float(struct gl_program *prog, float val)
{
    struct prog_src_register src;
    unsigned swiz;

    memset(&src, 0, sizeof(src));

    src.File = PROGRAM_CONSTANT;
    src.Index = _mesa_add_unnamed_constant(prog->Parameters,
                                           (gl_constant_value *) &val, 1, &swiz);
    src.Swizzle = swiz;
    return src;
}

static struct prog_src_register
src_reg_for_vec4(struct gl_program *prog, const float *val)
{
    struct prog_src_register src;
    unsigned swiz;

    memset(&src, 0, sizeof(src));

    src.File = PROGRAM_CONSTANT;
    src.Index = _mesa_add_unnamed_constant(prog->Parameters,
                                           (gl_constant_value *) val, 4, &swiz);
    src.Swizzle = swiz;
    return src;
}

static bool
src_regs_are_same(const struct prog_src_register *a,
                  const struct prog_src_register *b)
{
    return (a->File == b->File)
           && (a->Index == b->Index)
           && (a->Swizzle == b->Swizzle)
           && (a->Negate == b->Negate)
           && (a->RelAddr == 0)
           && (b->RelAddr == 0);
}

static void
get_value(struct gl_program *prog, struct prog_src_register *r, float *data)
{
    unsigned pvo = prog->Parameters->Parameters[r->Index].ValueOffset;
    const gl_constant_value *const value =
            prog->Parameters->ParameterValues + pvo;

    data[0] = value[GET_SWZ(r->Swizzle, 0)].f;
    data[1] = value[GET_SWZ(r->Swizzle, 1)].f;
    data[2] = value[GET_SWZ(r->Swizzle, 2)].f;
    data[3] = value[GET_SWZ(r->Swizzle, 3)].f;

    if (r->Negate & 0x01) {
        data[0] = -data[0];
    }

    if (r->Negate & 0x02) {
        data[1] = -data[1];
    }

    if (r->Negate & 0x04) {
        data[2] = -data[2];
    }

    if (r->Negate & 0x08) {
        data[3] = -data[3];
    }
}

/**
 * Try to replace instructions that produce a constant result with simple moves
 *
 * The hope is that a following copy propagation pass will eliminate the
 * unnecessary move instructions.
 */
GLboolean
_mesa_constant_fold(struct gl_program *prog)
{
    bool progress = false;
    unsigned i;

    for (i = 0; i < prog->arb.NumInstructions; i++) {
        struct prog_instruction *const inst = &prog->arb.Instructions[i];

        switch (inst->Opcode) {
            case OPCODE_ADD:
                if (src_regs_are_constant(inst, 2)) {
                    float a[4];
                    float b[4];
                    float result[4];

                    get_value(prog, &inst->SrcReg[0], a);
                    get_value(prog, &inst->SrcReg[1], b);

                    result[0] = a[0] + b[0];
                    result[1] = a[1] + b[1];
                    result[2] = a[2] + b[2];
                    result[3] = a[3] + b[3];

                    inst->Opcode = OPCODE_MOV;
                    inst->SrcReg[0] = src_reg_for_vec4(prog, result);

                    inst->SrcReg[1].File = PROGRAM_UNDEFINED;
                    inst->SrcReg[1].Swizzle = SWIZZLE_NOOP;

                    progress = true;
                }
                break;

            case OPCODE_CMP:
                /* FINISHME: We could also optimize CMP instructions where the first
                 * FINISHME: source is a constant that is either all < 0.0 or all
                 * FINISHME: >= 0.0.
                 */
                if (src_regs_are_constant(inst, 3)) {
                    float a[4];
                    float b[4];
                    float c[4];
                    float result[4];

                    get_value(prog, &inst->SrcReg[0], a);
                    get_value(prog, &inst->SrcReg[1], b);
                    get_value(prog, &inst->SrcReg[2], c);

                    result[0] = a[0] < 0.0f ? b[0] : c[0];
                    result[1] = a[1] < 0.0f ? b[1] : c[1];
                    result[2] = a[2] < 0.0f ? b[2] : c[2];
                    result[3] = a[3] < 0.0f ? b[3] : c[3];

                    inst->Opcode = OPCODE_MOV;
                    inst->SrcReg[0] = src_reg_for_vec4(prog, result);

                    inst->SrcReg[1].File = PROGRAM_UNDEFINED;
                    inst->SrcReg[1].Swizzle = SWIZZLE_NOOP;
                    inst->SrcReg[2].File = PROGRAM_UNDEFINED;
                    inst->SrcReg[2].Swizzle = SWIZZLE_NOOP;

                    progress = true;
                }
                break;

            case OPCODE_DP2:
            case OPCODE_DP3:
            case OPCODE_DP4:
                if (src_regs_are_constant(inst, 2)) {
                    float a[4];
                    float b[4];
                    float result;

                    get_value(prog, &inst->SrcReg[0], a);
                    get_value(prog, &inst->SrcReg[1], b);

                    result = (a[0] * b[0]) + (a[1] * b[1]);

                    if (inst->Opcode >= OPCODE_DP3)
                        result += a[2] * b[2];

                    if (inst->Opcode == OPCODE_DP4)
                        result += a[3] * b[3];

                    inst->Opcode = OPCODE_MOV;
                    inst->SrcReg[0] = src_reg_for_float(prog, result);

                    inst->SrcReg[1].File = PROGRAM_UNDEFINED;
                    inst->SrcReg[1].Swizzle = SWIZZLE_NOOP;

                    progress = true;
                }
                break;

            case OPCODE_MUL:
                if (src_regs_are_constant(inst, 2)) {
                    float a[4];
                    float b[4];
                    float result[4];

                    get_value(prog, &inst->SrcReg[0], a);
                    get_value(prog, &inst->SrcReg[1], b);

                    result[0] = a[0] * b[0];
                    result[1] = a[1] * b[1];
                    result[2] = a[2] * b[2];
                    result[3] = a[3] * b[3];

                    inst->Opcode = OPCODE_MOV;
                    inst->SrcReg[0] = src_reg_for_vec4(prog, result);

                    inst->SrcReg[1].File = PROGRAM_UNDEFINED;
                    inst->SrcReg[1].Swizzle = SWIZZLE_NOOP;

                    progress = true;
                }
                break;

            case OPCODE_SGE:
                if (src_regs_are_constant(inst, 2)) {
                    float a[4];
                    float b[4];
                    float result[4];

                    get_value(prog, &inst->SrcReg[0], a);
                    get_value(prog, &inst->SrcReg[1], b);

                    result[0] = (a[0] >= b[0]) ? 1.0f : 0.0f;
                    result[1] = (a[1] >= b[1]) ? 1.0f : 0.0f;
                    result[2] = (a[2] >= b[2]) ? 1.0f : 0.0f;
                    result[3] = (a[3] >= b[3]) ? 1.0f : 0.0f;

                    inst->Opcode = OPCODE_MOV;
                    inst->SrcReg[0] = src_reg_for_vec4(prog, result);

                    inst->SrcReg[1].File = PROGRAM_UNDEFINED;
                    inst->SrcReg[1].Swizzle = SWIZZLE_NOOP;

                    progress = true;
                } else if (src_regs_are_same(&inst->SrcReg[0], &inst->SrcReg[1])) {
                    inst->Opcode = OPCODE_MOV;
                    inst->SrcReg[0] = src_reg_for_float(prog, 1.0f);

                    inst->SrcReg[1].File = PROGRAM_UNDEFINED;
                    inst->SrcReg[1].Swizzle = SWIZZLE_NOOP;

                    progress = true;
                }
                break;

            case OPCODE_SLT:
                if (src_regs_are_constant(inst, 2)) {
                    float a[4];
                    float b[4];
                    float result[4];

                    get_value(prog, &inst->SrcReg[0], a);
                    get_value(prog, &inst->SrcReg[1], b);

                    result[0] = (a[0] < b[0]) ? 1.0f : 0.0f;
                    result[1] = (a[1] < b[1]) ? 1.0f : 0.0f;
                    result[2] = (a[2] < b[2]) ? 1.0f : 0.0f;
                    result[3] = (a[3] < b[3]) ? 1.0f : 0.0f;

                    inst->Opcode = OPCODE_MOV;
                    inst->SrcReg[0] = src_reg_for_vec4(prog, result);

                    inst->SrcReg[1].File = PROGRAM_UNDEFINED;
                    inst->SrcReg[1].Swizzle = SWIZZLE_NOOP;

                    progress = true;
                } else if (src_regs_are_same(&inst->SrcReg[0], &inst->SrcReg[1])) {
                    inst->Opcode = OPCODE_MOV;
                    inst->SrcReg[0] = src_reg_for_float(prog, 0.0f);

                    inst->SrcReg[1].File = PROGRAM_UNDEFINED;
                    inst->SrcReg[1].Swizzle = SWIZZLE_NOOP;

                    progress = true;
                }
                break;

            default:
                break;
        }
    }

    return progress;
}
