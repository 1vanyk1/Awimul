#include "u_format.h"
#include "u_format_bptc.h"
#include "u_format_pack.h"
#include "../format_srgb.h"
#include "../u_math.h"

#define BPTC_BLOCK_DECODE
#include "../../main/texcompress_bptc_tmp.h"

void
util_format_bptc_rgba_unorm_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                               const uint8_t *restrict src_row, unsigned src_stride,
                                               unsigned width, unsigned height)
{
    decompress_rgba_unorm(width, height,
                          src_row, src_stride,
                          dst_row, dst_stride);
}

void
util_format_bptc_rgba_unorm_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                             const uint8_t *restrict src_row, unsigned src_stride,
                                             unsigned width, unsigned height)
{
    compress_rgba_unorm(width, height,
                        src_row, src_stride,
                        dst_row, dst_stride);
}

void
util_format_bptc_rgba_unorm_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride,
                                              const uint8_t *restrict src_row, unsigned src_stride,
                                              unsigned width, unsigned height)
{
    uint8_t *temp_block;
    temp_block = malloc(width * height * 4 * sizeof(uint8_t));
    decompress_rgba_unorm(width, height,
                          src_row, src_stride,
                          temp_block, width * 4 * sizeof(uint8_t));
    util_format_r8g8b8a8_unorm_unpack_rgba_float(
            dst_row, dst_stride,
            temp_block, width * 4 * sizeof(uint8_t),
            width, height);
    free((void *) temp_block);
}

void
util_format_bptc_rgba_unorm_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride,
                                            const float *restrict src_row, unsigned src_stride,
                                            unsigned width, unsigned height)
{
    uint8_t *temp_block;
    temp_block = malloc(width * height * 4 * sizeof(uint8_t));
    util_format_r32g32b32a32_float_unpack_rgba_8unorm(
            temp_block, width * 4 * sizeof(uint8_t),
            (uint8_t *)src_row, src_stride,
            width, height);
    compress_rgba_unorm(width, height,
                        temp_block, width * 4 * sizeof(uint8_t),
                        dst_row, dst_stride);
    free((void *) temp_block);
}

void
util_format_bptc_rgba_unorm_fetch_rgba(void *restrict dst, const uint8_t *restrict src,
                                       unsigned width, unsigned height)
{
    uint8_t temp_block[4];

    fetch_rgba_unorm_from_block(src + ((width * sizeof(uint8_t)) * (height / 4) + (width / 4)) * 16,
                                temp_block, (width % 4) + (height % 4) * 4);

    util_format_read_4(PIPE_FORMAT_R8G8B8A8_UNORM,
                       dst, 4 * sizeof(float),
                       temp_block, 4 * sizeof(uint8_t),
                       0, 0, 1, 1);
}

void
util_format_bptc_srgba_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                          const uint8_t *restrict src_row, unsigned src_stride,
                                          unsigned width, unsigned height)
{
    decompress_rgba_unorm(width, height,
                          src_row, src_stride,
                          dst_row, dst_stride);
}

void
util_format_bptc_srgba_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                        const uint8_t *restrict src_row, unsigned src_stride,
                                        unsigned width, unsigned height)
{
    compress_rgba_unorm(width, height,
                        src_row, src_stride,
                        dst_row, dst_stride);
}

void
util_format_bptc_srgba_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride,
                                         const uint8_t *restrict src_row, unsigned src_stride,
                                         unsigned width, unsigned height)
{
    uint8_t *temp_block;
    temp_block = malloc(width * height * 4 * sizeof(uint8_t));
    decompress_rgba_unorm(width, height,
                          src_row, src_stride,
                          temp_block, width * 4 * sizeof(uint8_t));
    util_format_r8g8b8a8_srgb_unpack_rgba_float(dst_row, dst_stride,
                                                temp_block, width * 4 * sizeof(uint8_t),
                                                width, height);

    free((void *) temp_block);
}

void
util_format_bptc_srgba_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride,
                                       const float *restrict src_row, unsigned src_stride,
                                       unsigned width, unsigned height)
{
    compress_rgb_float(width, height,
                       src_row, src_stride,
                       dst_row, dst_stride,
                       true);
}

void
util_format_bptc_srgba_fetch_rgba(void *restrict dst, const uint8_t *restrict src,
                                  unsigned width, unsigned height)
{
    uint8_t temp_block[4];

    fetch_rgba_unorm_from_block(src + ((width * sizeof(uint8_t)) * (height / 4) + (width / 4)) * 16,
                                temp_block, (width % 4) + (height % 4) * 4);
    util_format_r8g8b8a8_srgb_fetch_rgba(dst, temp_block, 0, 0);
}

void
util_format_bptc_rgb_float_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                              const uint8_t *restrict src_row, unsigned src_stride,
                                              unsigned width, unsigned height)
{
    float *temp_block;
    temp_block = malloc(width * height * 4 * sizeof(float));
    decompress_rgb_float(width, height,
                         src_row, src_stride,
                         temp_block, width * 4 * sizeof(float),
                         true);
    util_format_r32g32b32a32_float_unpack_rgba_8unorm(
            dst_row, dst_stride,
            (const uint8_t *)temp_block, width * 4 * sizeof(float),
            width, height);
    free((void *) temp_block);
}

void
util_format_bptc_rgb_float_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                            const uint8_t *restrict src_row, unsigned src_stride,
                                            unsigned width, unsigned height)
{
    compress_rgba_unorm(width, height,
                        src_row, src_stride,
                        dst_row, dst_stride);
}

void
util_format_bptc_rgb_float_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride,
                                             const uint8_t *restrict src_row, unsigned src_stride,
                                             unsigned width, unsigned height)
{
    decompress_rgb_float(width, height,
                         src_row, src_stride,
                         dst_row, dst_stride,
                         true);
}

void
util_format_bptc_rgb_float_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride,
                                           const float *restrict src_row, unsigned src_stride,
                                           unsigned width, unsigned height)
{
    compress_rgb_float(width, height,
                       src_row, src_stride,
                       dst_row, dst_stride,
                       true);
}

void
util_format_bptc_rgb_float_fetch_rgba(void *restrict dst, const uint8_t *restrict src,
                                      unsigned width, unsigned height)
{
    fetch_rgb_float_from_block(src + ((width * sizeof(uint8_t)) * (height / 4) + (width / 4)) * 16,
                               dst, (width % 4) + (height % 4) * 4, true);
}

void
util_format_bptc_rgb_ufloat_unpack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                               const uint8_t *restrict src_row, unsigned src_stride,
                                               unsigned width, unsigned height)
{
    float *temp_block;
    temp_block = malloc(width * height * 4 * sizeof(float));
    decompress_rgb_float(width, height,
                         src_row, src_stride,
                         temp_block, width * 4 * sizeof(float),
                         false);
    util_format_r32g32b32a32_float_unpack_rgba_8unorm(
            dst_row, dst_stride,
            (const uint8_t *)temp_block, width * 4 * sizeof(float),
            width, height);
    free((void *) temp_block);
}

void
util_format_bptc_rgb_ufloat_pack_rgba_8unorm(uint8_t *restrict dst_row, unsigned dst_stride,
                                             const uint8_t *restrict src_row, unsigned src_stride,
                                             unsigned width, unsigned height)
{
    compress_rgba_unorm(width, height,
                        src_row, src_stride,
                        dst_row, dst_stride);
}

void
util_format_bptc_rgb_ufloat_unpack_rgba_float(void *restrict dst_row, unsigned dst_stride,
                                              const uint8_t *restrict src_row, unsigned src_stride,
                                              unsigned width, unsigned height)
{
    decompress_rgb_float(width, height,
                         src_row, src_stride,
                         dst_row, dst_stride,
                         false);
}

void
util_format_bptc_rgb_ufloat_pack_rgba_float(uint8_t *restrict dst_row, unsigned dst_stride,
                                            const float *restrict src_row, unsigned src_stride,
                                            unsigned width, unsigned height)
{
    compress_rgb_float(width, height,
                       src_row, src_stride,
                       dst_row, dst_stride,
                       false);
}

void
util_format_bptc_rgb_ufloat_fetch_rgba(void *restrict dst, const uint8_t *restrict src,
                                       unsigned width, unsigned height)
{
    fetch_rgb_float_from_block(src + ((width * sizeof(uint8_t)) * (height / 4) + (width / 4)) * 16,
                               dst, (width % 4) + (height % 4) * 4, false);
}
