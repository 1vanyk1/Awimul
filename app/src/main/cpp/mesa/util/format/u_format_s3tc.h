#ifndef U_FORMAT_S3TC_H_
#define U_FORMAT_S3TC_H_


#include "../../include/pipe/p_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

enum util_format_dxtn {
    UTIL_FORMAT_DXT1_RGB = 0x83F0,
    UTIL_FORMAT_DXT1_RGBA = 0x83F1,
    UTIL_FORMAT_DXT3_RGBA = 0x83F2,
    UTIL_FORMAT_DXT5_RGBA = 0x83F3
};


typedef void
(*util_format_dxtn_fetch_t)( int src_stride,
                             const uint8_t *src,
                             int col, int row,
                             uint8_t *dst );

typedef void
(*util_format_dxtn_pack_t)( int src_comps,
                            int width, int height,
                            const uint8_t *src,
                            enum util_format_dxtn dst_format,
                            uint8_t *dst,
                            int dst_stride);

extern util_format_dxtn_fetch_t util_format_dxt1_rgb_fetch;
extern util_format_dxtn_fetch_t util_format_dxt1_rgba_fetch;
extern util_format_dxtn_fetch_t util_format_dxt3_rgba_fetch;
extern util_format_dxtn_fetch_t util_format_dxt5_rgba_fetch;

extern util_format_dxtn_pack_t util_format_dxtn_pack;


void
util_format_dxt1_rgb_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_rgb_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_rgb_fetch_rgba_8unorm(uint8_t *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt1_rgba_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_rgba_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_rgba_fetch_rgba_8unorm(uint8_t *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt3_rgba_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt3_rgba_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt3_rgba_fetch_rgba_8unorm(uint8_t *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt5_rgba_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt5_rgba_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt5_rgba_fetch_rgba_8unorm(uint8_t *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt1_srgb_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_srgb_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_srgb_fetch_rgba_8unorm(uint8_t *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt1_srgba_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_srgba_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_srgba_fetch_rgba_8unorm(uint8_t *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt3_srgba_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt3_srgba_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt3_srgba_fetch_rgba_8unorm(uint8_t *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt5_srgba_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt5_srgba_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt5_srgba_fetch_rgba_8unorm(uint8_t *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);


void
util_format_dxt1_rgb_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_rgb_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_rgb_fetch_rgba(void *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt1_rgba_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_rgba_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_rgba_fetch_rgba(void *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt3_rgba_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt3_rgba_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt3_rgba_fetch_rgba(void *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt5_rgba_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt5_rgba_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt5_rgba_fetch_rgba(void *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt1_srgb_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_srgb_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_srgb_fetch_rgba(void *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt1_srgba_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_srgba_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt1_srgba_fetch_rgba(void *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt3_srgba_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt3_srgba_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt3_srgba_fetch_rgba(void *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

void
util_format_dxt5_srgba_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride, const uint8_t *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt5_srgba_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride, const float *restrict src_row, unsigned src_stride, unsigned width, unsigned height);

void
util_format_dxt5_srgba_fetch_rgba(void *restrict dst, const uint8_t *restrict src, unsigned i, unsigned j);

#ifdef __cplusplus
}
#endif

#endif /* U_FORMAT_S3TC_H_ */
