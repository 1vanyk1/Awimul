#ifndef U_FORMAT_FXT1_H_
#define U_FORMAT_FXT1_H_


#include "../../include/pipe/p_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

void
util_format_fxt1_rgb_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_fxt1_rgb_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_fxt1_rgb_fetch_rgba_8unorm(uint8_t *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_fxt1_rgba_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_fxt1_rgba_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_fxt1_rgba_fetch_rgba_8unorm(uint8_t *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_fxt1_rgb_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_fxt1_rgb_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_fxt1_rgb_fetch_rgba(void *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_fxt1_rgba_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_fxt1_rgba_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_fxt1_rgba_fetch_rgba(void *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

#ifdef __cplusplus
}
#endif

#endif /* U_FORMAT_FXT1_H_ */
