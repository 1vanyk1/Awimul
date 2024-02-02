#ifndef _VDPAU_FUNCS_H_
#define _VDPAU_FUNCS_H_

#include "vdpau_dmabuf.h"

/* Used for implementing NV_vdpau_interop */
static inline enum pipe_format
VdpFormatRGBAToPipe(uint32_t vdpau_format)
{
    switch (vdpau_format) {
        case VDP_RGBA_FORMAT_R8:
            return PIPE_FORMAT_R8_UNORM;
        case VDP_RGBA_FORMAT_R8G8:
            return PIPE_FORMAT_R8G8_UNORM;
        case VDP_RGBA_FORMAT_A8:
            return PIPE_FORMAT_A8_UNORM;
        case VDP_RGBA_FORMAT_B10G10R10A2:
            return PIPE_FORMAT_B10G10R10A2_UNORM;
        case VDP_RGBA_FORMAT_B8G8R8A8:
            return PIPE_FORMAT_B8G8R8A8_UNORM;
        case VDP_RGBA_FORMAT_R10G10B10A2:
            return PIPE_FORMAT_R10G10B10A2_UNORM;
        case VDP_RGBA_FORMAT_R8G8B8A8:
            return PIPE_FORMAT_R8G8B8A8_UNORM;
        default:
            assert(0);
    }

    return PIPE_FORMAT_NONE;
}

#endif /* _VDPAU_FUNCS_H_ */
