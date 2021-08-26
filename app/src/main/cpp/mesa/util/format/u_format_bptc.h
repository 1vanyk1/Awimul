#ifndef U_FORMAT_BPTC_H_
#define U_FORMAT_BPTC_H_


#include "../../include/pipe/p_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

void
util_format_bptc_rgba_unorm_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                               const uint8_t *restrict src_row, unsigned src_stride,
                                               unsigned width, unsigned height);
void
util_format_bptc_rgba_unorm_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                             const uint8_t *restrict src_row, unsigned src_stride,
                                             unsigned width, unsigned height);
void
util_format_bptc_rgba_unorm_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride,
                                              const uint8_t *restrict src_row, unsigned src_stride,
                                              unsigned width, unsigned height);
void
util_format_bptc_rgba_unorm_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride,
                                            const float *restrict src_row, unsigned src_stride,
                                            unsigned width, unsigned height);
void
util_format_bptc_rgba_unorm_fetch_rgba(void *restrict dst, const uint8_t *restrict src,
                                       unsigned width, unsigned height);

void
util_format_bptc_srgba_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                          const uint8_t *restrict src_row, unsigned src_stride,
                                          unsigned width, unsigned height);
void
util_format_bptc_srgba_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                        const uint8_t *restrict src_row, unsigned src_stride,
                                        unsigned width, unsigned height);
void
util_format_bptc_srgba_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride,
                                         const uint8_t *restrict src_row, unsigned src_stride,
                                         unsigned width, unsigned height);
void
util_format_bptc_srgba_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride,
                                       const float *restrict src_row, unsigned src_stride,
                                       unsigned width, unsigned height);
void
util_format_bptc_srgba_fetch_rgba(void *restrict dst, const uint8_t *restrict src,
                                  unsigned width, unsigned height);

void
util_format_bptc_rgb_float_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                              const uint8_t *restrict src_row, unsigned src_stride,
                                              unsigned width, unsigned height);
void
util_format_bptc_rgb_float_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                            const uint8_t *restrict src_row, unsigned src_stride,
                                            unsigned width, unsigned height);
void
util_format_bptc_rgb_float_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride,
                                             const uint8_t *restrict src_row, unsigned src_stride,
                                             unsigned width, unsigned height);
void
util_format_bptc_rgb_float_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride,
                                           const float *restrict src_row, unsigned src_stride,
                                           unsigned width, unsigned height);
void
util_format_bptc_rgb_float_fetch_rgba(void *restrict dst, const uint8_t *restrict src,
                                      unsigned width, unsigned height);

void
util_format_bptc_rgb_ufloat_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                               const uint8_t *restrict src_row, unsigned src_stride,
                                               unsigned width, unsigned height);
void
util_format_bptc_rgb_ufloat_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                             const uint8_t *restrict src_row, unsigned src_stride,
                                             unsigned width, unsigned height);
void
util_format_bptc_rgb_ufloat_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride,
                                              const uint8_t *restrict src_row, unsigned src_stride,
                                              unsigned width, unsigned height);
void
util_format_bptc_rgb_ufloat_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride,
                                            const float *restrict src_row, unsigned src_stride,
                                            unsigned width, unsigned height);
void
util_format_bptc_rgb_ufloat_fetch_rgba(void *restrict dst, const uint8_t *restrict src,
                                       unsigned width, unsigned height);
#ifdef __cplusplus
}
#endif

#endif /* U_FORMAT_BPTC_H_ */
