#ifndef _M_VECTOR_ASM_H_
#define _M_VECTOR_ASM_H_

/* This file is a set of defines usable by the old FF TNL assembly code for
 * referencing GLvector4f and GLmatrix structs.
 */

#define VEC_DIRTY_0        0x1
#define VEC_DIRTY_1        0x2
#define VEC_DIRTY_2        0x4
#define VEC_DIRTY_3        0x8

#define VEC_SIZE_1   VEC_DIRTY_0
#define VEC_SIZE_2   (VEC_DIRTY_0|VEC_DIRTY_1)
#define VEC_SIZE_3   (VEC_DIRTY_0|VEC_DIRTY_1|VEC_DIRTY_2)
#define VEC_SIZE_4   (VEC_DIRTY_0|VEC_DIRTY_1|VEC_DIRTY_2|VEC_DIRTY_3)

/* If you add a new field, please add it to the STATIC_ASSERTs in
 * _mesa_vector4f_init().
 */
#define V4F_DATA   0
#define V4F_START  (V4F_DATA + MATH_ASM_PTR_SIZE)
#define V4F_COUNT  (V4F_START + MATH_ASM_PTR_SIZE)
#define V4F_STRIDE (V4F_COUNT + 4)
#define V4F_SIZE   (V4F_STRIDE + 4)
#define V4F_FLAGS  (V4F_SIZE + 4)

/* If you add a new field, please add it to the STATIC_ASSERTs in
 * _math_matrix_set_identity().
 */
#define MATRIX_M   0
#define MATRIX_INV (MATRIX_M + 16 * 4)

#endif /* _M_VECTOR_ASM_H */
