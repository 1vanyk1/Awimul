#ifndef FORMAT_PACK_H
#define FORMAT_PACK_H


#include "../util/format/u_format.h"
#include "formats.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Pack a float Z value to dest address */
typedef void (*mesa_pack_float_z_func)(const float *src, void *dst);

/** Pack a uint32_t Z value to dest address */
typedef void (*mesa_pack_uint_z_func)(const uint32_t *src, void *dst);

/** Pack a uint8_t stencil value to dest address */
typedef void (*mesa_pack_ubyte_stencil_func)(const uint8_t *src, void *dst);




extern mesa_pack_float_z_func
_mesa_get_pack_float_z_func(mesa_format format);


extern mesa_pack_uint_z_func
_mesa_get_pack_uint_z_func(mesa_format format);


extern mesa_pack_ubyte_stencil_func
_mesa_get_pack_ubyte_stencil_func(mesa_format format);


static inline void
_mesa_pack_float_rgba_row(mesa_format format, uint32_t n,
                          const float src[][4], void *dst)
{
    util_format_pack_rgba(format, dst, src, n);
}

extern void
_mesa_pack_ubyte_rgba_row(mesa_format format, uint32_t n,
                          const uint8_t src[][4], void *dst);

static inline void
_mesa_pack_uint_rgba_row(mesa_format format, uint32_t n,
                         const uint32_t src[][4], void *dst)
{
    util_format_pack_rgba(format, dst, src, n);
}

extern void
_mesa_pack_ubyte_rgba_rect(mesa_format format, uint32_t width, uint32_t height,
                           const uint8_t *src, int32_t srcRowStride,
                           void *dst, int32_t dstRowStride);

extern void
_mesa_pack_float_z_row(mesa_format format, uint32_t n,
                       const float *src, void *dst);

extern void
_mesa_pack_uint_z_row(mesa_format format, uint32_t n,
                      const uint32_t *src, void *dst);

extern void
_mesa_pack_ubyte_stencil_row(mesa_format format, uint32_t n,
                             const uint8_t *src, void *dst);

extern void
_mesa_pack_uint_24_8_depth_stencil_row(mesa_format format, uint32_t n,
                                       const uint32_t *src, void *dst);

#ifdef __cplusplus
}
#endif

#endif
