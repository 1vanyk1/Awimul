#ifndef U_FORMAT_ETC1_H_
#define U_FORMAT_ETC1_H_

void
util_format_etc1_rgb8_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_etc1_rgb8_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_etc1_rgb8_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_etc1_rgb8_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_etc1_rgb8_fetch_rgba(void *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

#endif /* U_FORMAT_ETC1_H_ */
