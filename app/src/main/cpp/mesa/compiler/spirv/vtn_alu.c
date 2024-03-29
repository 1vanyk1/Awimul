#include <math.h>
#include "vtn_private.h"
#include "spirv_info.h"

/*
 * Normally, column vectors in SPIR-V correspond to a single NIR SSA
 * definition. But for matrix multiplies, we want to do one routine for
 * multiplying a matrix by a matrix and then pretend that vectors are matrices
 * with one column. So we "wrap" these things, and unwrap the result before we
 * send it off.
 */

static struct vtn_ssa_value *
wrap_matrix(struct vtn_builder *b, struct vtn_ssa_value *val)
{
    if (val == NULL)
        return NULL;

    if (glsl_type_is_matrix(val->type))
        return val;

    struct vtn_ssa_value *dest = rzalloc(b, struct vtn_ssa_value);
    dest->type = glsl_get_bare_type(val->type);
    dest->elems = ralloc_array(b, struct vtn_ssa_value *, 1);
    dest->elems[0] = val;

    return dest;
}

static struct vtn_ssa_value *
unwrap_matrix(struct vtn_ssa_value *val)
{
    if (glsl_type_is_matrix(val->type))
        return val;

    return val->elems[0];
}

static struct vtn_ssa_value *
matrix_multiply(struct vtn_builder *b,
                struct vtn_ssa_value *_src0, struct vtn_ssa_value *_src1)
{

    struct vtn_ssa_value *src0 = wrap_matrix(b, _src0);
    struct vtn_ssa_value *src1 = wrap_matrix(b, _src1);
    struct vtn_ssa_value *src0_transpose = wrap_matrix(b, _src0->transposed);
    struct vtn_ssa_value *src1_transpose = wrap_matrix(b, _src1->transposed);

    unsigned src0_rows = glsl_get_vector_elements(src0->type);
    unsigned src0_columns = glsl_get_matrix_columns(src0->type);
    unsigned src1_columns = glsl_get_matrix_columns(src1->type);

    const struct glsl_type *dest_type;
    if (src1_columns > 1) {
        dest_type = glsl_matrix_type(glsl_get_base_type(src0->type),
                                     src0_rows, src1_columns);
    } else {
        dest_type = glsl_vector_type(glsl_get_base_type(src0->type), src0_rows);
    }
    struct vtn_ssa_value *dest = vtn_create_ssa_value(b, dest_type);

    dest = wrap_matrix(b, dest);

    bool transpose_result = false;
    if (src0_transpose && src1_transpose) {
        /* transpose(A) * transpose(B) = transpose(B * A) */
        src1 = src0_transpose;
        src0 = src1_transpose;
        src0_transpose = NULL;
        src1_transpose = NULL;
        transpose_result = true;
    }

    if (src0_transpose && !src1_transpose &&
        glsl_get_base_type(src0->type) == GLSL_TYPE_FLOAT) {
        /* We already have the rows of src0 and the columns of src1 available,
         * so we can just take the dot product of each row with each column to
         * get the result.
         */

        for (unsigned i = 0; i < src1_columns; i++) {
            nir_ssa_def *vec_src[4];
            for (unsigned j = 0; j < src0_rows; j++) {
                vec_src[j] = nir_fdot(&b->nb, src0_transpose->elems[j]->def,
                                      src1->elems[i]->def);
            }
            dest->elems[i]->def = nir_vec(&b->nb, vec_src, src0_rows);
        }
    } else {
        /* We don't handle the case where src1 is transposed but not src0, since
         * the general case only uses individual components of src1 so the
         * optimizer should chew through the transpose we emitted for src1.
         */

        for (unsigned i = 0; i < src1_columns; i++) {
            /* dest[i] = sum(src0[j] * src1[i][j] for all j) */
            dest->elems[i]->def =
                    nir_fmul(&b->nb, src0->elems[src0_columns - 1]->def,
                             nir_channel(&b->nb, src1->elems[i]->def, src0_columns - 1));
            for (int j = src0_columns - 2; j >= 0; j--) {
                dest->elems[i]->def =
                        nir_fadd(&b->nb, dest->elems[i]->def,
                                 nir_fmul(&b->nb, src0->elems[j]->def,
                                          nir_channel(&b->nb, src1->elems[i]->def, j)));
            }
        }
    }

    dest = unwrap_matrix(dest);

    if (transpose_result)
        dest = vtn_ssa_transpose(b, dest);

    return dest;
}

static struct vtn_ssa_value *
mat_times_scalar(struct vtn_builder *b,
                 struct vtn_ssa_value *mat,
                 nir_ssa_def *scalar)
{
    struct vtn_ssa_value *dest = vtn_create_ssa_value(b, mat->type);
    for (unsigned i = 0; i < glsl_get_matrix_columns(mat->type); i++) {
        if (glsl_base_type_is_integer(glsl_get_base_type(mat->type)))
            dest->elems[i]->def = nir_imul(&b->nb, mat->elems[i]->def, scalar);
        else
            dest->elems[i]->def = nir_fmul(&b->nb, mat->elems[i]->def, scalar);
    }

    return dest;
}

static struct vtn_ssa_value *
vtn_handle_matrix_alu(struct vtn_builder *b, SpvOp opcode,
                      struct vtn_ssa_value *src0, struct vtn_ssa_value *src1)
{
    switch (opcode) {
        case SpvOpFNegate: {
            struct vtn_ssa_value *dest = vtn_create_ssa_value(b, src0->type);
            unsigned cols = glsl_get_matrix_columns(src0->type);
            for (unsigned i = 0; i < cols; i++)
                dest->elems[i]->def = nir_fneg(&b->nb, src0->elems[i]->def);
            return dest;
        }

        case SpvOpFAdd: {
            struct vtn_ssa_value *dest = vtn_create_ssa_value(b, src0->type);
            unsigned cols = glsl_get_matrix_columns(src0->type);
            for (unsigned i = 0; i < cols; i++)
                dest->elems[i]->def =
                        nir_fadd(&b->nb, src0->elems[i]->def, src1->elems[i]->def);
            return dest;
        }

        case SpvOpFSub: {
            struct vtn_ssa_value *dest = vtn_create_ssa_value(b, src0->type);
            unsigned cols = glsl_get_matrix_columns(src0->type);
            for (unsigned i = 0; i < cols; i++)
                dest->elems[i]->def =
                        nir_fsub(&b->nb, src0->elems[i]->def, src1->elems[i]->def);
            return dest;
        }

        case SpvOpTranspose:
            return vtn_ssa_transpose(b, src0);

        case SpvOpMatrixTimesScalar:
            if (src0->transposed) {
                return vtn_ssa_transpose(b, mat_times_scalar(b, src0->transposed,
                                                             src1->def));
            } else {
                return mat_times_scalar(b, src0, src1->def);
            }
            break;

        case SpvOpVectorTimesMatrix:
        case SpvOpMatrixTimesVector:
        case SpvOpMatrixTimesMatrix:
            if (opcode == SpvOpVectorTimesMatrix) {
                return matrix_multiply(b, vtn_ssa_transpose(b, src1), src0);
            } else {
                return matrix_multiply(b, src0, src1);
            }
            break;

        default: vtn_fail_with_opcode("unknown matrix opcode", opcode);
    }
}

static nir_alu_type
convert_op_src_type(SpvOp opcode)
{
    switch (opcode) {
        case SpvOpFConvert:
        case SpvOpConvertFToS:
        case SpvOpConvertFToU:
            return nir_type_float;
        case SpvOpSConvert:
        case SpvOpConvertSToF:
        case SpvOpSatConvertSToU:
            return nir_type_int;
        case SpvOpUConvert:
        case SpvOpConvertUToF:
        case SpvOpSatConvertUToS:
            return nir_type_uint;
        default:
            unreachable("Unhandled conversion op");
    }
}

static nir_alu_type
convert_op_dst_type(SpvOp opcode)
{
    switch (opcode) {
        case SpvOpFConvert:
        case SpvOpConvertSToF:
        case SpvOpConvertUToF:
            return nir_type_float;
        case SpvOpSConvert:
        case SpvOpConvertFToS:
        case SpvOpSatConvertUToS:
            return nir_type_int;
        case SpvOpUConvert:
        case SpvOpConvertFToU:
        case SpvOpSatConvertSToU:
            return nir_type_uint;
        default:
            unreachable("Unhandled conversion op");
    }
}

nir_op
vtn_nir_alu_op_for_spirv_opcode(struct vtn_builder *b,
                                SpvOp opcode, bool *swap, bool *exact,
                                unsigned src_bit_size, unsigned dst_bit_size)
{
    /* Indicates that the first two arguments should be swapped.  This is
     * used for implementing greater-than and less-than-or-equal.
     */
    *swap = false;

    *exact = false;

    switch (opcode) {
        case SpvOpSNegate:            return nir_op_ineg;
        case SpvOpFNegate:            return nir_op_fneg;
        case SpvOpNot:                return nir_op_inot;
        case SpvOpIAdd:               return nir_op_iadd;
        case SpvOpFAdd:               return nir_op_fadd;
        case SpvOpISub:               return nir_op_isub;
        case SpvOpFSub:               return nir_op_fsub;
        case SpvOpIMul:               return nir_op_imul;
        case SpvOpFMul:               return nir_op_fmul;
        case SpvOpUDiv:               return nir_op_udiv;
        case SpvOpSDiv:               return nir_op_idiv;
        case SpvOpFDiv:               return nir_op_fdiv;
        case SpvOpUMod:               return nir_op_umod;
        case SpvOpSMod:               return nir_op_imod;
        case SpvOpFMod:               return nir_op_fmod;
        case SpvOpSRem:               return nir_op_irem;
        case SpvOpFRem:               return nir_op_frem;

        case SpvOpShiftRightLogical:     return nir_op_ushr;
        case SpvOpShiftRightArithmetic:  return nir_op_ishr;
        case SpvOpShiftLeftLogical:      return nir_op_ishl;
        case SpvOpLogicalOr:             return nir_op_ior;
        case SpvOpLogicalEqual:          return nir_op_ieq;
        case SpvOpLogicalNotEqual:       return nir_op_ine;
        case SpvOpLogicalAnd:            return nir_op_iand;
        case SpvOpLogicalNot:            return nir_op_inot;
        case SpvOpBitwiseOr:             return nir_op_ior;
        case SpvOpBitwiseXor:            return nir_op_ixor;
        case SpvOpBitwiseAnd:            return nir_op_iand;
        case SpvOpSelect:                return nir_op_bcsel;
        case SpvOpIEqual:                return nir_op_ieq;

        case SpvOpBitFieldInsert:        return nir_op_bitfield_insert;
        case SpvOpBitFieldSExtract:      return nir_op_ibitfield_extract;
        case SpvOpBitFieldUExtract:      return nir_op_ubitfield_extract;
        case SpvOpBitReverse:            return nir_op_bitfield_reverse;

        case SpvOpUCountLeadingZerosINTEL: return nir_op_uclz;
            /* SpvOpUCountTrailingZerosINTEL is handled elsewhere. */
        case SpvOpAbsISubINTEL:          return nir_op_uabs_isub;
        case SpvOpAbsUSubINTEL:          return nir_op_uabs_usub;
        case SpvOpIAddSatINTEL:          return nir_op_iadd_sat;
        case SpvOpUAddSatINTEL:          return nir_op_uadd_sat;
        case SpvOpIAverageINTEL:         return nir_op_ihadd;
        case SpvOpUAverageINTEL:         return nir_op_uhadd;
        case SpvOpIAverageRoundedINTEL:  return nir_op_irhadd;
        case SpvOpUAverageRoundedINTEL:  return nir_op_urhadd;
        case SpvOpISubSatINTEL:          return nir_op_isub_sat;
        case SpvOpUSubSatINTEL:          return nir_op_usub_sat;
        case SpvOpIMul32x16INTEL:        return nir_op_imul_32x16;
        case SpvOpUMul32x16INTEL:        return nir_op_umul_32x16;

            /* The ordered / unordered operators need special implementation besides
             * the logical operator to use since they also need to check if operands are
             * ordered.
             */
        case SpvOpFOrdEqual:                            *exact = true;  return nir_op_feq;
        case SpvOpFUnordEqual:                          *exact = true;  return nir_op_feq;
        case SpvOpINotEqual:                                            return nir_op_ine;
        case SpvOpLessOrGreater:                        /* Deprecated, use OrdNotEqual */
        case SpvOpFOrdNotEqual:                         *exact = true;  return nir_op_fneu;
        case SpvOpFUnordNotEqual:                       *exact = true;  return nir_op_fneu;
        case SpvOpULessThan:                                            return nir_op_ult;
        case SpvOpSLessThan:                                            return nir_op_ilt;
        case SpvOpFOrdLessThan:                         *exact = true;  return nir_op_flt;
        case SpvOpFUnordLessThan:                       *exact = true;  return nir_op_flt;
        case SpvOpUGreaterThan:          *swap = true;                  return nir_op_ult;
        case SpvOpSGreaterThan:          *swap = true;                  return nir_op_ilt;
        case SpvOpFOrdGreaterThan:       *swap = true;  *exact = true;  return nir_op_flt;
        case SpvOpFUnordGreaterThan:     *swap = true;  *exact = true;  return nir_op_flt;
        case SpvOpULessThanEqual:        *swap = true;                  return nir_op_uge;
        case SpvOpSLessThanEqual:        *swap = true;                  return nir_op_ige;
        case SpvOpFOrdLessThanEqual:     *swap = true;  *exact = true;  return nir_op_fge;
        case SpvOpFUnordLessThanEqual:   *swap = true;  *exact = true;  return nir_op_fge;
        case SpvOpUGreaterThanEqual:                                    return nir_op_uge;
        case SpvOpSGreaterThanEqual:                                    return nir_op_ige;
        case SpvOpFOrdGreaterThanEqual:                 *exact = true;  return nir_op_fge;
        case SpvOpFUnordGreaterThanEqual:               *exact = true;  return nir_op_fge;

            /* Conversions: */
        case SpvOpQuantizeToF16:         return nir_op_fquantize2f16;
        case SpvOpUConvert:
        case SpvOpConvertFToU:
        case SpvOpConvertFToS:
        case SpvOpConvertSToF:
        case SpvOpConvertUToF:
        case SpvOpSConvert:
        case SpvOpFConvert: {
            nir_alu_type src_type = convert_op_src_type(opcode) | src_bit_size;
            nir_alu_type dst_type = convert_op_dst_type(opcode) | dst_bit_size;
            return nir_type_conversion_op(src_type, dst_type, nir_rounding_mode_undef);
        }

        case SpvOpPtrCastToGeneric:   return nir_op_mov;
        case SpvOpGenericCastToPtr:   return nir_op_mov;

            /* Derivatives: */
        case SpvOpDPdx:         return nir_op_fddx;
        case SpvOpDPdy:         return nir_op_fddy;
        case SpvOpDPdxFine:     return nir_op_fddx_fine;
        case SpvOpDPdyFine:     return nir_op_fddy_fine;
        case SpvOpDPdxCoarse:   return nir_op_fddx_coarse;
        case SpvOpDPdyCoarse:   return nir_op_fddy_coarse;

        case SpvOpIsNormal:     return nir_op_fisnormal;
        case SpvOpIsFinite:     return nir_op_fisfinite;

        default:
            vtn_fail("No NIR equivalent: %u", opcode);
    }
}

static void
handle_no_contraction(struct vtn_builder *b, struct vtn_value *val, int member,
                      const struct vtn_decoration *dec, void *_void)
{
    vtn_assert(dec->scope == VTN_DEC_DECORATION);
    if (dec->decoration != SpvDecorationNoContraction)
        return;

    b->nb.exact = true;
}

void
vtn_handle_no_contraction(struct vtn_builder *b, struct vtn_value *val)
{
    vtn_foreach_decoration(b, val, handle_no_contraction, NULL);
}

nir_rounding_mode
vtn_rounding_mode_to_nir(struct vtn_builder *b, SpvFPRoundingMode mode)
{
    switch (mode) {
        case SpvFPRoundingModeRTE:
            return nir_rounding_mode_rtne;
        case SpvFPRoundingModeRTZ:
            return nir_rounding_mode_rtz;
        case SpvFPRoundingModeRTP:
            vtn_fail_if(b->shader->info.stage != MESA_SHADER_KERNEL,
                        "FPRoundingModeRTP is only supported in kernels");
            return nir_rounding_mode_ru;
        case SpvFPRoundingModeRTN:
            vtn_fail_if(b->shader->info.stage != MESA_SHADER_KERNEL,
                        "FPRoundingModeRTN is only supported in kernels");
            return nir_rounding_mode_rd;
        default:
            vtn_fail("Unsupported rounding mode: %s",
                     spirv_fproundingmode_to_string(mode));
            break;
    }
}

struct conversion_opts {
    nir_rounding_mode rounding_mode;
    bool saturate;
};

static void
handle_conversion_opts(struct vtn_builder *b, struct vtn_value *val, int member,
                       const struct vtn_decoration *dec, void *_opts)
{
    struct conversion_opts *opts = _opts;

    switch (dec->decoration) {
        case SpvDecorationFPRoundingMode:
            opts->rounding_mode = vtn_rounding_mode_to_nir(b, dec->operands[0]);
            break;

        case SpvDecorationSaturatedConversion:
            vtn_fail_if(b->shader->info.stage != MESA_SHADER_KERNEL,
                        "Saturated conversions are only allowed in kernels");
            opts->saturate = true;
            break;

        default:
            break;
    }
}

static void
handle_no_wrap(struct vtn_builder *b, struct vtn_value *val, int member,
               const struct vtn_decoration *dec, void *_alu)
{
    nir_alu_instr *alu = _alu;
    switch (dec->decoration) {
        case SpvDecorationNoSignedWrap:
            alu->no_signed_wrap = true;
            break;
        case SpvDecorationNoUnsignedWrap:
            alu->no_unsigned_wrap = true;
            break;
        default:
            /* Do nothing. */
            break;
    }
}

void
vtn_handle_alu(struct vtn_builder *b, SpvOp opcode,
               const uint32_t *w, unsigned count)
{
    struct vtn_value *dest_val = vtn_untyped_value(b, w[2]);
    const struct glsl_type *dest_type = vtn_get_type(b, w[1])->type;

    vtn_handle_no_contraction(b, dest_val);

    /* Collect the various SSA sources */
    const unsigned num_inputs = count - 3;
    struct vtn_ssa_value *vtn_src[4] = { NULL, };
    for (unsigned i = 0; i < num_inputs; i++)
        vtn_src[i] = vtn_ssa_value(b, w[i + 3]);

    if (glsl_type_is_matrix(vtn_src[0]->type) ||
        (num_inputs >= 2 && glsl_type_is_matrix(vtn_src[1]->type))) {
        vtn_push_ssa_value(b, w[2],
                           vtn_handle_matrix_alu(b, opcode, vtn_src[0], vtn_src[1]));
        b->nb.exact = b->exact;
        return;
    }

    struct vtn_ssa_value *dest = vtn_create_ssa_value(b, dest_type);
    nir_ssa_def *src[4] = { NULL, };
    for (unsigned i = 0; i < num_inputs; i++) {
        vtn_assert(glsl_type_is_vector_or_scalar(vtn_src[i]->type));
        src[i] = vtn_src[i]->def;
    }

    switch (opcode) {
        case SpvOpAny:
            dest->def = nir_bany(&b->nb, src[0]);
            break;

        case SpvOpAll:
            dest->def = nir_ball(&b->nb, src[0]);
            break;

        case SpvOpOuterProduct: {
            for (unsigned i = 0; i < src[1]->num_components; i++) {
                dest->elems[i]->def =
                        nir_fmul(&b->nb, src[0], nir_channel(&b->nb, src[1], i));
            }
            break;
        }

        case SpvOpDot:
            dest->def = nir_fdot(&b->nb, src[0], src[1]);
            break;

        case SpvOpIAddCarry:
            vtn_assert(glsl_type_is_struct_or_ifc(dest_type));
            dest->elems[0]->def = nir_iadd(&b->nb, src[0], src[1]);
            dest->elems[1]->def = nir_uadd_carry(&b->nb, src[0], src[1]);
            break;

        case SpvOpISubBorrow:
            vtn_assert(glsl_type_is_struct_or_ifc(dest_type));
            dest->elems[0]->def = nir_isub(&b->nb, src[0], src[1]);
            dest->elems[1]->def = nir_usub_borrow(&b->nb, src[0], src[1]);
            break;

        case SpvOpUMulExtended: {
            vtn_assert(glsl_type_is_struct_or_ifc(dest_type));
            nir_ssa_def *umul = nir_umul_2x32_64(&b->nb, src[0], src[1]);
            dest->elems[0]->def = nir_unpack_64_2x32_split_x(&b->nb, umul);
            dest->elems[1]->def = nir_unpack_64_2x32_split_y(&b->nb, umul);
            break;
        }

        case SpvOpSMulExtended: {
            vtn_assert(glsl_type_is_struct_or_ifc(dest_type));
            nir_ssa_def *smul = nir_imul_2x32_64(&b->nb, src[0], src[1]);
            dest->elems[0]->def = nir_unpack_64_2x32_split_x(&b->nb, smul);
            dest->elems[1]->def = nir_unpack_64_2x32_split_y(&b->nb, smul);
            break;
        }

        case SpvOpFwidth:
            dest->def = nir_fadd(&b->nb,
                                 nir_fabs(&b->nb, nir_fddx(&b->nb, src[0])),
                                 nir_fabs(&b->nb, nir_fddy(&b->nb, src[0])));
            break;
        case SpvOpFwidthFine:
            dest->def = nir_fadd(&b->nb,
                                 nir_fabs(&b->nb, nir_fddx_fine(&b->nb, src[0])),
                                 nir_fabs(&b->nb, nir_fddy_fine(&b->nb, src[0])));
            break;
        case SpvOpFwidthCoarse:
            dest->def = nir_fadd(&b->nb,
                                 nir_fabs(&b->nb, nir_fddx_coarse(&b->nb, src[0])),
                                 nir_fabs(&b->nb, nir_fddy_coarse(&b->nb, src[0])));
            break;

        case SpvOpVectorTimesScalar:
            /* The builder will take care of splatting for us. */
            dest->def = nir_fmul(&b->nb, src[0], src[1]);
            break;

        case SpvOpIsNan: {
            const bool save_exact = b->nb.exact;

            b->nb.exact = true;
            dest->def = nir_fneu(&b->nb, src[0], src[0]);
            b->nb.exact = save_exact;
            break;
        }

        case SpvOpOrdered: {
            const bool save_exact = b->nb.exact;

            b->nb.exact = true;
            dest->def = nir_iand(&b->nb, nir_feq(&b->nb, src[0], src[0]),
                                 nir_feq(&b->nb, src[1], src[1]));
            b->nb.exact = save_exact;
            break;
        }

        case SpvOpUnordered: {
            const bool save_exact = b->nb.exact;

            b->nb.exact = true;
            dest->def = nir_ior(&b->nb, nir_fneu(&b->nb, src[0], src[0]),
                                nir_fneu(&b->nb, src[1], src[1]));
            b->nb.exact = save_exact;
            break;
        }

        case SpvOpIsInf: {
            nir_ssa_def *inf = nir_imm_floatN_t(&b->nb, INFINITY, src[0]->bit_size);
            dest->def = nir_ieq(&b->nb, nir_fabs(&b->nb, src[0]), inf);
            break;
        }

        case SpvOpFUnordEqual:
        case SpvOpFUnordNotEqual:
        case SpvOpFUnordLessThan:
        case SpvOpFUnordGreaterThan:
        case SpvOpFUnordLessThanEqual:
        case SpvOpFUnordGreaterThanEqual: {
            bool swap;
            bool unused_exact;
            unsigned src_bit_size = glsl_get_bit_size(vtn_src[0]->type);
            unsigned dst_bit_size = glsl_get_bit_size(dest_type);
            nir_op op = vtn_nir_alu_op_for_spirv_opcode(b, opcode, &swap,
                                                        &unused_exact,
                                                        src_bit_size, dst_bit_size);

            if (swap) {
                nir_ssa_def *tmp = src[0];
                src[0] = src[1];
                src[1] = tmp;
            }

            const bool save_exact = b->nb.exact;

            b->nb.exact = true;

            dest->def =
                    nir_ior(&b->nb,
                            nir_build_alu(&b->nb, op, src[0], src[1], NULL, NULL),
                            nir_ior(&b->nb,
                                    nir_fneu(&b->nb, src[0], src[0]),
                                    nir_fneu(&b->nb, src[1], src[1])));

            b->nb.exact = save_exact;
            break;
        }

        case SpvOpLessOrGreater:
        case SpvOpFOrdNotEqual: {
            /* For all the SpvOpFOrd* comparisons apart from NotEqual, the value
             * from the ALU will probably already be false if the operands are not
             * ordered so we don’t need to handle it specially.
             */
            bool swap;
            bool exact;
            unsigned src_bit_size = glsl_get_bit_size(vtn_src[0]->type);
            unsigned dst_bit_size = glsl_get_bit_size(dest_type);
            nir_op op = vtn_nir_alu_op_for_spirv_opcode(b, opcode, &swap, &exact,
                                                        src_bit_size, dst_bit_size);

            assert(!swap);
            assert(exact);

            const bool save_exact = b->nb.exact;

            b->nb.exact = true;

            dest->def =
                    nir_iand(&b->nb,
                             nir_build_alu(&b->nb, op, src[0], src[1], NULL, NULL),
                             nir_iand(&b->nb,
                                      nir_feq(&b->nb, src[0], src[0]),
                                      nir_feq(&b->nb, src[1], src[1])));

            b->nb.exact = save_exact;
            break;
        }

        case SpvOpUConvert:
        case SpvOpConvertFToU:
        case SpvOpConvertFToS:
        case SpvOpConvertSToF:
        case SpvOpConvertUToF:
        case SpvOpSConvert:
        case SpvOpFConvert:
        case SpvOpSatConvertSToU:
        case SpvOpSatConvertUToS: {
            unsigned src_bit_size = glsl_get_bit_size(vtn_src[0]->type);
            unsigned dst_bit_size = glsl_get_bit_size(dest_type);
            nir_alu_type src_type = convert_op_src_type(opcode) | src_bit_size;
            nir_alu_type dst_type = convert_op_dst_type(opcode) | dst_bit_size;

            struct conversion_opts opts = {
                    .rounding_mode = nir_rounding_mode_undef,
                    .saturate = false,
            };
            vtn_foreach_decoration(b, dest_val, handle_conversion_opts, &opts);

            if (opcode == SpvOpSatConvertSToU || opcode == SpvOpSatConvertUToS)
                opts.saturate = true;

            if (b->shader->info.stage == MESA_SHADER_KERNEL) {
                if (opts.rounding_mode == nir_rounding_mode_undef && !opts.saturate) {
                    nir_op op = nir_type_conversion_op(src_type, dst_type,
                                                       nir_rounding_mode_undef);
                    dest->def = nir_build_alu(&b->nb, op, src[0], NULL, NULL, NULL);
                } else {
                    dest->def = nir_convert_alu_types(&b->nb, dst_bit_size, src[0],
                                                      src_type, dst_type,
                                                      opts.rounding_mode, opts.saturate);
                }
            } else {
                vtn_fail_if(opts.rounding_mode != nir_rounding_mode_undef &&
                            dst_type != nir_type_float16,
                            "Rounding modes are only allowed on conversions to "
                            "16-bit float types");
                nir_op op = nir_type_conversion_op(src_type, dst_type,
                                                   opts.rounding_mode);
                dest->def = nir_build_alu(&b->nb, op, src[0], NULL, NULL, NULL);
            }
            break;
        }

        case SpvOpBitFieldInsert:
        case SpvOpBitFieldSExtract:
        case SpvOpBitFieldUExtract:
        case SpvOpShiftLeftLogical:
        case SpvOpShiftRightArithmetic:
        case SpvOpShiftRightLogical: {
            bool swap;
            bool exact;
            unsigned src0_bit_size = glsl_get_bit_size(vtn_src[0]->type);
            unsigned dst_bit_size = glsl_get_bit_size(dest_type);
            nir_op op = vtn_nir_alu_op_for_spirv_opcode(b, opcode, &swap, &exact,
                                                        src0_bit_size, dst_bit_size);

            assert(!exact);

            assert (op == nir_op_ushr || op == nir_op_ishr || op == nir_op_ishl ||
                    op == nir_op_bitfield_insert || op == nir_op_ubitfield_extract ||
                    op == nir_op_ibitfield_extract);

            for (unsigned i = 0; i < nir_op_infos[op].num_inputs; i++) {
                unsigned src_bit_size =
                        nir_alu_type_get_type_size(nir_op_infos[op].input_types[i]);
                if (src_bit_size == 0)
                    continue;
                if (src_bit_size != src[i]->bit_size) {
                    assert(src_bit_size == 32);
                    /* Convert the Shift, Offset and Count  operands to 32 bits, which is the bitsize
                     * supported by the NIR instructions. See discussion here:
                     *
                     * https://lists.freedesktop.org/archives/mesa-dev/2018-April/193026.html
                     */
                    src[i] = nir_u2u32(&b->nb, src[i]);
                }
            }
            dest->def = nir_build_alu(&b->nb, op, src[0], src[1], src[2], src[3]);
            break;
        }

        case SpvOpSignBitSet:
            dest->def = nir_i2b(&b->nb,
                                nir_ushr(&b->nb, src[0], nir_imm_int(&b->nb, src[0]->bit_size - 1)));
            break;

        case SpvOpUCountTrailingZerosINTEL:
            dest->def = nir_umin(&b->nb,
                                 nir_find_lsb(&b->nb, src[0]),
                                 nir_imm_int(&b->nb, 32u));
            break;

        case SpvOpBitCount: {
            /* bit_count always returns int32, but the SPIR-V opcode just says the return
             * value needs to be big enough to store the number of bits.
             */
            dest->def = nir_u2u(&b->nb, nir_bit_count(&b->nb, src[0]), glsl_get_bit_size(dest_type));
            break;
        }

        default: {
            bool swap;
            bool exact;
            unsigned src_bit_size = glsl_get_bit_size(vtn_src[0]->type);
            unsigned dst_bit_size = glsl_get_bit_size(dest_type);
            nir_op op = vtn_nir_alu_op_for_spirv_opcode(b, opcode, &swap,
                                                        &exact,
                                                        src_bit_size, dst_bit_size);

            if (swap) {
                nir_ssa_def *tmp = src[0];
                src[0] = src[1];
                src[1] = tmp;
            }

            switch (op) {
                case nir_op_ishl:
                case nir_op_ishr:
                case nir_op_ushr:
                    if (src[1]->bit_size != 32)
                        src[1] = nir_u2u32(&b->nb, src[1]);
                    break;
                default:
                    break;
            }

            const bool save_exact = b->nb.exact;

            if (exact)
                b->nb.exact = true;

            dest->def = nir_build_alu(&b->nb, op, src[0], src[1], src[2], src[3]);

            b->nb.exact = save_exact;
            break;
        } /* default */
    }

    switch (opcode) {
        case SpvOpIAdd:
        case SpvOpIMul:
        case SpvOpISub:
        case SpvOpShiftLeftLogical:
        case SpvOpSNegate: {
            nir_alu_instr *alu = nir_instr_as_alu(dest->def->parent_instr);
            vtn_foreach_decoration(b, dest_val, handle_no_wrap, alu);
            break;
        }
        default:
            /* Do nothing. */
            break;
    }

    vtn_push_ssa_value(b, w[2], dest);

    b->nb.exact = b->exact;
}

void
vtn_handle_bitcast(struct vtn_builder *b, const uint32_t *w, unsigned count)
{
    vtn_assert(count == 4);
    /* From the definition of OpBitcast in the SPIR-V 1.2 spec:
     *
     *    "If Result Type has the same number of components as Operand, they
     *    must also have the same component width, and results are computed per
     *    component.
     *
     *    If Result Type has a different number of components than Operand, the
     *    total number of bits in Result Type must equal the total number of
     *    bits in Operand. Let L be the type, either Result Type or Operand’s
     *    type, that has the larger number of components. Let S be the other
     *    type, with the smaller number of components. The number of components
     *    in L must be an integer multiple of the number of components in S.
     *    The first component (that is, the only or lowest-numbered component)
     *    of S maps to the first components of L, and so on, up to the last
     *    component of S mapping to the last components of L. Within this
     *    mapping, any single component of S (mapping to multiple components of
     *    L) maps its lower-ordered bits to the lower-numbered components of L."
     */

    struct vtn_type *type = vtn_get_type(b, w[1]);
    struct nir_ssa_def *src = vtn_get_nir_ssa(b, w[3]);

    vtn_fail_if(src->num_components * src->bit_size !=
                glsl_get_vector_elements(type->type) * glsl_get_bit_size(type->type),
                "Source and destination of OpBitcast must have the same "
                "total number of bits");
    nir_ssa_def *val =
            nir_bitcast_vector(&b->nb, src, glsl_get_bit_size(type->type));
    vtn_push_nir_ssa(b, w[2], val);
}
