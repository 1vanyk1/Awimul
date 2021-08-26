#ifndef U_FORMAT_OTHER_H_
#define U_FORMAT_OTHER_H_


#include "../../include/pipe/p_compiler.h"


void
util_format_r9g9b9e5_float_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride,
                                             const uint8_t *restrict src_row, unsigned src_stride,
                                             unsigned width, unsigned height);

void
util_format_r9g9b9e5_float_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride,
                                           const float *restrict src_row, unsigned src_stride,
                                           unsigned width, unsigned height);

void
util_format_r9g9b9e5_float_fetch_rgba(void *restrict dst, const uint8_t *restrict src,
                                      unsigned i, unsigned j);

void
util_format_r9g9b9e5_float_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                              const uint8_t *restrict src_row, unsigned src_stride,
                                              unsigned width, unsigned height);

void
util_format_r9g9b9e5_float_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                            const uint8_t *restrict src_row, unsigned src_stride,
                                            unsigned width, unsigned height);


void
util_format_r11g11b10_float_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride,
                                              const uint8_t *restrict src_row, unsigned src_stride,
                                              unsigned width, unsigned height);

void
util_format_r11g11b10_float_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride,
                                            const float *restrict src_row, unsigned src_stride,
                                            unsigned width, unsigned height);

void
util_format_r11g11b10_float_fetch_rgba(void *restrict dst, const uint8_t *restrict src,
                                       unsigned i, unsigned j);

void
util_format_r11g11b10_float_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                               const uint8_t *restrict src_row, unsigned src_stride,
                                               unsigned width, unsigned height);

void
util_format_r11g11b10_float_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                             const uint8_t *restrict src_row, unsigned src_stride,
                                             unsigned width, unsigned height);


void
util_format_r8g8bx_snorm_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride,
                                           const uint8_t *restrict src_row, unsigned src_stride,
                                           unsigned width, unsigned height);

void
util_format_r8g8bx_snorm_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride,
                                         const float *restrict src_row, unsigned src_stride,
                                         unsigned width, unsigned height);

void
util_format_r8g8bx_snorm_fetch_rgba(void *restrict dst, const uint8_t *restrict src,
                                    unsigned i, unsigned j);

void
util_format_r8g8bx_snorm_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                            const uint8_t *restrict src_row, unsigned src_stride,
                                            unsigned width, unsigned height);

void
util_format_r8g8bx_snorm_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                          const uint8_t *restrict src_row, unsigned src_stride,
                                          unsigned width, unsigned height);

#endif /* U_FORMAT_OTHER_H_ */
