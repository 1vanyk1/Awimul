#ifndef TGSI_UTIL_H
#define TGSI_UTIL_H

#include <stdbool.h>
#include "../../include/pipe/p_shader_tokens.h"

#if defined __cplusplus
extern "C" {
#endif

struct tgsi_src_register;
struct tgsi_full_src_register;
struct tgsi_full_instruction;

void *
tgsi_align_128bit(void *unaligned);

unsigned
tgsi_util_get_src_register_swizzle(const struct tgsi_src_register *reg,
                                   unsigned component);


unsigned
tgsi_util_get_full_src_register_swizzle(
        const struct tgsi_full_src_register *reg,
        unsigned component );

void
tgsi_util_set_src_register_swizzle(struct tgsi_src_register *reg,
                                   unsigned swizzle,
                                   unsigned component);

unsigned
tgsi_util_get_inst_usage_mask(const struct tgsi_full_instruction *inst,
                              unsigned src_idx);

struct tgsi_src_register
tgsi_util_get_src_from_ind(const struct tgsi_ind_register *reg);

int
tgsi_util_get_texture_coord_dim(enum tgsi_texture_type tgsi_tex);

int
tgsi_util_get_shadow_ref_src_index(enum tgsi_texture_type tgsi_tex);

bool
tgsi_is_shadow_target(enum tgsi_texture_type target);


static inline bool
tgsi_is_msaa_target(enum tgsi_texture_type target)
{
    return (target == TGSI_TEXTURE_2D_MSAA ||
            target == TGSI_TEXTURE_2D_ARRAY_MSAA);
}

static inline bool
tgsi_is_array_sampler(enum tgsi_texture_type target)
{
    return target == TGSI_TEXTURE_1D_ARRAY ||
           target == TGSI_TEXTURE_SHADOW1D_ARRAY ||
           target == TGSI_TEXTURE_2D_ARRAY ||
           target == TGSI_TEXTURE_SHADOW2D_ARRAY ||
           target == TGSI_TEXTURE_CUBE_ARRAY ||
           target == TGSI_TEXTURE_SHADOWCUBE_ARRAY ||
           target == TGSI_TEXTURE_2D_ARRAY_MSAA;
}

#if defined __cplusplus
}
#endif

#endif /* TGSI_UTIL_H */
