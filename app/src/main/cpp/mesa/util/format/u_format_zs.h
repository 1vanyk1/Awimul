#ifndef U_FORMAT_ZS_H_
#define U_FORMAT_ZS_H_


#include "../../include/pipe/p_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

void
util_format_s8_uint_unpack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_s8_uint_pack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z16_unorm_unpack_z_float(float *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z16_unorm_pack_z_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z16_unorm_unpack_z_32unorm(uint32_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z16_unorm_pack_z_32unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint32_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_unorm_unpack_z_float(float *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_unorm_pack_z_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_unorm_unpack_z_32unorm(uint32_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_unorm_pack_z_32unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint32_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_float_unpack_z_float(float *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_float_pack_z_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_float_unpack_z_32unorm(uint32_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_float_pack_z_32unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint32_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z24_unorm_s8_uint_unpack_z_float(float *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z24_unorm_s8_uint_pack_z_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z24_unorm_s8_uint_unpack_z_32unorm(uint32_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z24_unorm_s8_uint_pack_z_32unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint32_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z24_unorm_s8_uint_unpack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z24_unorm_s8_uint_pack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_z24_unorm_s8_uint_pack_separate(uint8_t *restrict dst_row, unsigned dst_stride, const uint32_t *z_src_row, unsigned z_src_stride, const uint8_t *s_src_row, unsigned s_src_stride, unsigned width, unsigned height);

void
util_format_z24_unorm_s8_uint_unpack_z24(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_z24_unorm_s8_uint_pack_z24(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_s8_uint_z24_unorm_unpack_z_float(float *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_s8_uint_z24_unorm_pack_z_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_s8_uint_z24_unorm_unpack_z_32unorm(uint32_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_s8_uint_z24_unorm_pack_z_32unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint32_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_s8_uint_z24_unorm_unpack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_s8_uint_z24_unorm_pack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z24x8_unorm_unpack_z_float(float *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z24x8_unorm_pack_z_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z24x8_unorm_unpack_z_32unorm(uint32_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z24x8_unorm_pack_z_32unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint32_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_x8z24_unorm_unpack_z_float(float *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_x8z24_unorm_pack_z_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_x8z24_unorm_unpack_z_32unorm(uint32_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_x8z24_unorm_pack_z_32unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint32_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_float_s8x24_uint_unpack_z_float(float *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_float_s8x24_uint_pack_z_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_float_s8x24_uint_unpack_z_32unorm(uint32_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_float_s8x24_uint_pack_z_32unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint32_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_float_s8x24_uint_unpack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z32_float_s8x24_uint_pack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_z16_unorm_s8_uint_unpack_z_float(float *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z16_unorm_s8_uint_pack_z_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z16_unorm_s8_uint_unpack_z_32unorm(uint32_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z16_unorm_s8_uint_pack_z_32unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint32_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z16_unorm_s8_uint_unpack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);


void
util_format_z16_unorm_s8_uint_pack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_x24s8_uint_unpack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_x24s8_uint_pack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_s8x24_uint_unpack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_s8x24_uint_pack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_x32_s8x24_uint_unpack_s_8uint(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_x32_s8x24_uint_pack_s_8uint(uint8_t *restrict dst_row, unsigned dst_sride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

#ifdef __cplusplus
}
#endif

#endif /* U_FORMAT_ZS_H_ */
