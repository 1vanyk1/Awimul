#ifndef _VDPAU_INTEROP_H_
#define _VDPAU_INTEROP_H_

/* driver specific functions for NV_vdpau_interop */
#ifndef VDP_FUNC_ID_BASE_DRIVER
#define VDP_FUNC_ID_BASE_DRIVER 0x2000
#endif

/* Older implementation relying on passing pipe_video_buffer and
 * pipe_resources around. Deprecated and shouldn't be used for new things.
 */
#define VDP_FUNC_ID_VIDEO_SURFACE_GALLIUM (VDP_FUNC_ID_BASE_DRIVER + 0)
#define VDP_FUNC_ID_OUTPUT_SURFACE_GALLIUM (VDP_FUNC_ID_BASE_DRIVER + 1)

struct pipe_resource;
struct pipe_video_buffer;

typedef struct pipe_video_buffer *VdpVideoSurfaceGallium(uint32_t surface);
typedef struct pipe_resource *VdpOutputSurfaceGallium(uint32_t surface);

#endif /* _VDPAU_INTEROP_H_ */
