#ifndef FORMAT_UNPACK_H
#define FORMAT_UNPACK_H

#include "../util/format/u_format.h"
#include "formats.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void
_mesa_unpack_rgba_row(mesa_format format, uint32_t n,
                      const void *src, float dst[][4])
{
    util_format_unpack_rgba(format, dst, src, n);
}

extern void
_mesa_unpack_ubyte_rgba_row(mesa_format format, uint32_t n,
                            const void *src, uint8_t dst[][4]);

static inline void
_mesa_unpack_uint_rgba_row(mesa_format format, uint32_t n,
                           const void *src, uint32_t dst[][4])
{
    util_format_unpack_rgba(format, dst, src, n);
}

extern void
_mesa_unpack_rgba_block(mesa_format format,
                        const void *src, int32_t srcRowStride,
                        float dst[][4], int32_t dstRowStride,
                        uint32_t x, uint32_t y, uint32_t width, uint32_t height);

static inline void
_mesa_unpack_float_z_row(mesa_format format, uint32_t n,
                         const void *src, float *dst)
{
    util_format_unpack_z_float((enum pipe_format)format, dst, src, n);
}


static inline void
_mesa_unpack_uint_z_row(mesa_format format, uint32_t n,
                        const void *src, uint32_t *dst)
{
    util_format_unpack_z_32unorm((enum pipe_format)format, dst, src, n);
}

static inline void
_mesa_unpack_ubyte_stencil_row(mesa_format format, uint32_t n,
                               const void *src, uint8_t *dst)
{
    util_format_unpack_s_8uint((enum pipe_format)format, dst, src, n);
}

void
_mesa_unpack_uint_24_8_depth_stencil_row(mesa_format format, uint32_t n,
                                         const void *src, uint32_t *dst);

void
_mesa_unpack_float_32_uint_24_8_depth_stencil_row(mesa_format format,
                                                  uint32_t n,
                                                  const void *src,
                                                  uint32_t *dst);

#ifdef __cplusplus
}
#endif

#endif /* FORMAT_UNPACK_H */
