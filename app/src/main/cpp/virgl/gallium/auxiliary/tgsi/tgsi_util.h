#ifndef TGSI_UTIL_H
#define TGSI_UTIL_H

#if defined __cplusplus
extern "C" {
#endif

struct tgsi_src_register;
struct tgsi_full_src_register;
struct tgsi_full_instruction;

void *
tgsi_align_128bit(
        void *unaligned );

unsigned
tgsi_util_get_src_register_swizzle(
        const struct tgsi_src_register *reg,
        unsigned component );


unsigned
tgsi_util_get_full_src_register_swizzle(
        const struct tgsi_full_src_register *reg,
        unsigned component );

void
tgsi_util_set_src_register_swizzle(
        struct tgsi_src_register *reg,
        unsigned swizzle,
        unsigned component );

#define TGSI_UTIL_SIGN_CLEAR    0   /* Force positive */
#define TGSI_UTIL_SIGN_SET      1   /* Force negative */
#define TGSI_UTIL_SIGN_TOGGLE   2   /* Negate */
#define TGSI_UTIL_SIGN_KEEP     3   /* No change */

unsigned
tgsi_util_get_full_src_register_sign_mode(
        const struct tgsi_full_src_register *reg,
        unsigned component );

void
tgsi_util_set_full_src_register_sign_mode(
        struct tgsi_full_src_register *reg,
        unsigned sign_mode );

unsigned
tgsi_util_get_inst_usage_mask(const struct tgsi_full_instruction *inst,
                              unsigned src_idx);

struct tgsi_src_register
tgsi_util_get_src_from_ind(const struct tgsi_ind_register *reg);

int
tgsi_util_get_texture_coord_dim(int tgsi_tex, int *shadow_or_sample);

#if defined __cplusplus
}
#endif

#endif /* TGSI_UTIL_H */
