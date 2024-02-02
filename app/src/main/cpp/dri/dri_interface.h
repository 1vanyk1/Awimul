#ifndef AWIMUL_DRI_INTERFACE_H
#define AWIMUL_DRI_INTERFACE_H

#include "../gl4es/gl/gles.h"
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#define __DRI_SWRAST "DRI_SWRast"

typedef unsigned char boolean;
#ifndef TRUE
#define TRUE  true
#endif
#ifndef FALSE
#define FALSE false
#endif

typedef struct __DRIswrastExtensionRec		__DRIswrastExtension;
typedef struct __DRIswrastLoaderExtensionRec __DRIswrastLoaderExtension;
typedef struct __DRIdri2LoaderExtensionRec	__DRIdri2LoaderExtension;
typedef struct __DRIimageLoaderExtensionRec     __DRIimageLoaderExtension;
typedef struct __DRIextensionRec		__DRIextension;
typedef struct __DRIscreenRec		__DRIscreen;
typedef struct __DRIconfigRec		__DRIconfig;
typedef struct __DRIdrawableRec		__DRIdrawable;
typedef struct __DRIcontextRec		__DRIcontext;
typedef struct __DRIbufferRec			__DRIbuffer;
typedef struct __DRIimageRec          __DRIimage;

typedef enum driOptionType {
    DRI_BOOL, DRI_ENUM, DRI_INT, DRI_FLOAT, DRI_STRING, DRI_SECTION
} driOptionType;


typedef enum
{
    API_OPENGL_COMPAT,      /* legacy / compatibility contexts */
    API_OPENGLES,
    API_OPENGLES2,
    API_OPENGL_CORE,
    API_OPENGL_LAST = API_OPENGL_CORE
} gl_api;


typedef union driOptionValue {
    unsigned char _bool;
    int _int;
    float _float;
    char *_string;
} driOptionValue;


typedef struct driOptionRange {
    driOptionValue start;
    driOptionValue end;
} driOptionRange;


typedef struct driOptionInfo {
    char *name;
    driOptionType type;
    driOptionRange range;
} driOptionInfo;


typedef struct driOptionCache {
    driOptionInfo *info;
    driOptionValue *values;
    unsigned int tableSize;
} driOptionCache;

struct gl_config
{
    GLboolean floatMode;
    GLuint doubleBufferMode;
    GLuint stereoMode;

    GLint redBits, greenBits, blueBits, alphaBits;	/* bits per comp */
    GLuint redMask, greenMask, blueMask, alphaMask;
    GLint redShift, greenShift, blueShift, alphaShift;
    GLint rgbBits;		/* total bits for rgb */

    GLint accumRedBits, accumGreenBits, accumBlueBits, accumAlphaBits;
    GLint depthBits;
    GLint stencilBits;

    /* ARB_multisample / SGIS_multisample */
    GLuint samples;

    /* OML_swap_method */
    GLint swapMethod;

    /* EXT_framebuffer_sRGB */
    GLint sRGBCapable;
};

struct __DRIconfigRec {
    struct gl_config modes;
};

struct __DRIdrawableRec {
    void *driverPrivate;
    void *loaderPrivate;
    __DRIcontext *driContextPriv;
    __DRIscreen *driScreenPriv;
    int refcount;
    unsigned int lastStamp;
    int w, h;
    struct {
        unsigned int stamp;
    } dri2;
};

struct __DRIcontextRec {
    void *driverPrivate;
    void *loaderPrivate;
    __DRIdrawable *driDrawablePriv;
    __DRIdrawable *driReadablePriv;
    __DRIscreen *driScreenPriv;
    struct {
        int draw_stamp;
        int read_stamp;
    } dri2;
};

struct __DRIimageRec {
    struct pipe_resource *texture;
    unsigned level;
    unsigned layer;
    uint32_t dri_format;
    uint32_t dri_fourcc;
    uint32_t dri_components;
    unsigned use;
    unsigned plane;
    void *loader_private;
    boolean imported_dmabuf;
    enum __DRIYUVColorSpace yuv_color_space;
    enum __DRISampleRange sample_range;
    enum __DRIChromaSiting horizontal_siting;
    enum __DRIChromaSiting vertical_siting;

    /* DRI loader screen */
    __DRIscreen *sPriv;
};

typedef struct __DRImutableRenderBufferLoaderExtensionRec __DRImutableRenderBufferLoaderExtension;
struct __DRImutableRenderBufferLoaderExtensionRec {
    __DRIextension base;
    void (*displaySharedBuffer)(__DRIdrawable *drawable, int fence_fd,
                                void *loaderPrivate);
};

struct __DRIscreenRec {
    const struct __DriverAPIRec *driver;
    int myNum;
    int fd;
    void *driverPrivate;
    void *loaderPrivate;
    int max_gl_core_version;
    int max_gl_compat_version;
    int max_gl_es1_version;
    int max_gl_es2_version;
    const __DRIextension **extensions;
    const __DRIswrastLoaderExtension *swrast_loader;
    struct {
        const __DRIdri2LoaderExtension *loader;
        const __DRIimageLookupExtension *image;
        const __DRIuseInvalidateExtension *useInvalidate;
        const __DRIbackgroundCallableExtension *backgroundCallable;
    } dri2;
    struct {
        const __DRIimageLoaderExtension *loader;
    } image;
    struct {
        const __DRImutableRenderBufferLoaderExtension *loader;
    } mutableRenderBuffer;
    driOptionCache optionInfo;
    driOptionCache optionCache;
    unsigned int api_mask;
};

struct __DriverAPIRec {
    const __DRIconfig **(*InitScreen) (__DRIscreen * priv);

    void (*DestroyScreen)(__DRIscreen *driScrnPriv);

    GLboolean (*CreateContext)(gl_api api,
                               const struct gl_config *glVis,
                               __DRIcontext *driContextPriv,
                               const struct __DriverContextConfig *ctx_config,
                               unsigned *error,
                               void *sharedContextPrivate);

    void (*DestroyContext)(__DRIcontext *driContextPriv);

    GLboolean (*CreateBuffer)(__DRIscreen *driScrnPriv,
                              __DRIdrawable *driDrawPriv,
                              const struct gl_config *glVis,
                              GLboolean pixmapBuffer);

    void (*DestroyBuffer)(__DRIdrawable *driDrawPriv);

    void (*SwapBuffers)(__DRIdrawable *driDrawPriv);

    GLboolean (*MakeCurrent)(__DRIcontext *driContextPriv,
                             __DRIdrawable *driDrawPriv,
                             __DRIdrawable *driReadPriv);

    GLboolean (*UnbindContext)(__DRIcontext *driContextPriv);

    __DRIbuffer *(*AllocateBuffer) (__DRIscreen *screenPrivate,
                                    unsigned int attachment,
                                    unsigned int format,
                                    int width, int height);

    void (*ReleaseBuffer) (__DRIscreen *screenPrivate, __DRIbuffer *buffer);

    void (*CopySubBuffer)(__DRIdrawable *driDrawPriv, int x, int y,
                          int w, int h);
};

#define __DRI_CTX_ATTRIB_MAJOR_VERSION		0
#define __DRI_CTX_ATTRIB_MINOR_VERSION		1
#define __DRI_CTX_ATTRIB_FLAGS			2

/**
 * \requires __DRI2_ROBUSTNESS.
 */
#define __DRI_CTX_ATTRIB_RESET_STRATEGY		3

#define __DRI_CTX_FLAG_DEBUG			0x00000001
#define __DRI_CTX_FLAG_FORWARD_COMPATIBLE	0x00000002

/**
 * \requires __DRI2_ROBUSTNESS.
 */
#define __DRI_CTX_FLAG_ROBUST_BUFFER_ACCESS	0x00000004

/**
 * \requires __DRI2_NO_ERROR.
 *
 */
#define __DRI_CTX_FLAG_NO_ERROR			0x00000008

/**
 * \name Context reset strategies.
 */
/*@{*/
#define __DRI_CTX_RESET_NO_NOTIFICATION		0
#define __DRI_CTX_RESET_LOSE_CONTEXT		1
/*@}*/

#define __DRI_CTX_ATTRIB_PRIORITY		4

#define __DRI_CTX_PRIORITY_LOW			0
#define __DRI_CTX_PRIORITY_MEDIUM		1
#define __DRI_CTX_PRIORITY_HIGH			2

/**
 * \name Context release behaviors.
 */
#define __DRI_CTX_ATTRIB_RELEASE_BEHAVIOR	5

#define __DRI_CTX_RELEASE_BEHAVIOR_NONE         0
#define __DRI_CTX_RELEASE_BEHAVIOR_FLUSH        1

/** Success! */
#define __DRI_CTX_ERROR_SUCCESS			0

/** Memory allocation failure */
#define __DRI_CTX_ERROR_NO_MEMORY		1

/** Client requested an API (e.g., OpenGL ES 2.0) that the driver can't do. */
#define __DRI_CTX_ERROR_BAD_API			2

/** Client requested an API version that the driver can't do. */
#define __DRI_CTX_ERROR_BAD_VERSION		3

/** Client requested a flag or combination of flags the driver can't do. */
#define __DRI_CTX_ERROR_BAD_FLAG		4

/** Client requested an attribute the driver doesn't understand. */
#define __DRI_CTX_ERROR_UNKNOWN_ATTRIBUTE	5

/** Client requested a flag the driver doesn't understand. */
#define __DRI_CTX_ERROR_UNKNOWN_FLAG		6

#define __DRI_API_OPENGL	0	/**< OpenGL compatibility profile */
#define __DRI_API_GLES		1	/**< OpenGL ES 1.x */
#define __DRI_API_GLES2		2	/**< OpenGL ES 2.x */
#define __DRI_API_OPENGL_CORE	3	/**< OpenGL 3.2+ core profile */
#define __DRI_API_GLES3		4	/**< OpenGL ES 3.x */

#define __DRIVER_CONTEXT_ATTRIB_RESET_STRATEGY   (1 << 0)
#define __DRIVER_CONTEXT_ATTRIB_PRIORITY         (1 << 1)
#define __DRIVER_CONTEXT_ATTRIB_RELEASE_BEHAVIOR (1 << 2)

struct __DriverContextConfig {
    /* These members are always valid */
    unsigned major_version;
    unsigned minor_version;
    uint32_t flags;

    /* Flags describing which of the remaining members are valid */
    uint32_t attribute_mask;

    /* Only valid if __DRIVER_CONTEXT_ATTRIB_RESET_STRATEGY is set */
    int reset_strategy;

    /* Only valid if __DRIVER_CONTEXT_PRIORITY is set */
    unsigned priority;

    /* Only valid if __DRIVER_CONTEXT_ATTRIB_RELEASE_BEHAVIOR is set */
    int release_behavior;
};

//
//enum dri_loader_cap {
//    DRI_LOADER_CAP_RGBA_ORDERING,
//    DRI_LOADER_CAP_FP16,
//};
//
//
//typedef struct __DRIextensionRec		__DRIextension;
//typedef struct __DRIbufferRec			__DRIbuffer;
//
//typedef struct __DRIdisplayRec		__DRIdisplay;
//typedef struct __DRIscreenRec		__DRIscreen;
//typedef struct __DRIcontextRec		__DRIcontext;
//typedef struct __DRIdrawableRec		__DRIdrawable;
//typedef struct __DRIconfigRec		__DRIconfig;
//typedef struct __DRIframebufferRec	__DRIframebuffer;
//typedef struct __DRIversionRec		__DRIversion;
//typedef struct __DRIimageRec          __DRIimage;
//
//typedef struct __DRIswrastLoaderExtensionRec __DRIswrastLoaderExtension;
//typedef struct __DRIdri2LoaderExtensionRec	__DRIdri2LoaderExtension;
//typedef struct __DRIimageLoaderExtensionRec     __DRIimageLoaderExtension;
//
//
//#define __DRI_SWRAST_IMAGE_OP_DRAW	1
//#define __DRI_SWRAST_IMAGE_OP_CLEAR	2
//#define __DRI_SWRAST_IMAGE_OP_SWAP	3
//
//
//#define __DRI_BUFFER_COUNT		11
//
//
//#define PIPE_HANDLE_USAGE_FRAMEBUFFER_WRITE  (1 << 1)
//
//
//struct pipe_reference
//{
//    int32_t count; /* atomic */
//};
//
//
//enum pipe_texture_target {
//    PIPE_BUFFER           = 0,
//    PIPE_TEXTURE_1D       = 1,
//    PIPE_TEXTURE_2D       = 2,
//    PIPE_TEXTURE_3D       = 3,
//    PIPE_TEXTURE_CUBE     = 4,
//    PIPE_TEXTURE_RECT     = 5,
//    PIPE_TEXTURE_1D_ARRAY = 6,
//    PIPE_TEXTURE_2D_ARRAY = 7,
//    PIPE_TEXTURE_CUBE_ARRAY = 8,
//    PIPE_MAX_TEXTURE_TYPES
//};
//
//
//enum pipe_format {
//    PIPE_FORMAT_NONE,
//    PIPE_FORMAT_B8G8R8A8_UNORM,
//    PIPE_FORMAT_B8G8R8X8_UNORM,
//    PIPE_FORMAT_A8R8G8B8_UNORM,
//    PIPE_FORMAT_X8R8G8B8_UNORM,
//    PIPE_FORMAT_B5G5R5A1_UNORM,
//    PIPE_FORMAT_R4G4B4A4_UNORM,
//    PIPE_FORMAT_B4G4R4A4_UNORM,
//    PIPE_FORMAT_R5G6B5_UNORM,
//    PIPE_FORMAT_B5G6R5_UNORM,
//    PIPE_FORMAT_R10G10B10A2_UNORM,
//    PIPE_FORMAT_L8_UNORM,    /**< ubyte luminance */
//    PIPE_FORMAT_A8_UNORM,    /**< ubyte alpha */
//    PIPE_FORMAT_I8_UNORM,    /**< ubyte intensity */
//    PIPE_FORMAT_L8A8_UNORM,  /**< ubyte alpha, luminance */
//    PIPE_FORMAT_L16_UNORM,   /**< ushort luminance */
//    PIPE_FORMAT_UYVY,
//    PIPE_FORMAT_YUYV,
//    PIPE_FORMAT_Z16_UNORM,
//    PIPE_FORMAT_Z16_UNORM_S8_UINT,
//    PIPE_FORMAT_Z32_UNORM,
//    PIPE_FORMAT_Z32_FLOAT,
//    PIPE_FORMAT_Z24_UNORM_S8_UINT,
//    PIPE_FORMAT_S8_UINT_Z24_UNORM,
//    PIPE_FORMAT_Z24X8_UNORM,
//    PIPE_FORMAT_X8Z24_UNORM,
//    PIPE_FORMAT_S8_UINT,     /**< ubyte stencil */
//    PIPE_FORMAT_R64_FLOAT,
//    PIPE_FORMAT_R64G64_FLOAT,
//    PIPE_FORMAT_R64G64B64_FLOAT,
//    PIPE_FORMAT_R64G64B64A64_FLOAT,
//    PIPE_FORMAT_R32_FLOAT,
//    PIPE_FORMAT_R32G32_FLOAT,
//    PIPE_FORMAT_R32G32B32_FLOAT,
//    PIPE_FORMAT_R32G32B32A32_FLOAT,
//    PIPE_FORMAT_R32_UNORM,
//    PIPE_FORMAT_R32G32_UNORM,
//    PIPE_FORMAT_R32G32B32_UNORM,
//    PIPE_FORMAT_R32G32B32A32_UNORM,
//    PIPE_FORMAT_R32_USCALED,
//    PIPE_FORMAT_R32G32_USCALED,
//    PIPE_FORMAT_R32G32B32_USCALED,
//    PIPE_FORMAT_R32G32B32A32_USCALED,
//    PIPE_FORMAT_R32_SNORM,
//    PIPE_FORMAT_R32G32_SNORM,
//    PIPE_FORMAT_R32G32B32_SNORM,
//    PIPE_FORMAT_R32G32B32A32_SNORM,
//    PIPE_FORMAT_R32_SSCALED,
//    PIPE_FORMAT_R32G32_SSCALED,
//    PIPE_FORMAT_R32G32B32_SSCALED,
//    PIPE_FORMAT_R32G32B32A32_SSCALED,
//    PIPE_FORMAT_R16_UNORM,
//    PIPE_FORMAT_R16G16_UNORM,
//    PIPE_FORMAT_R16G16B16_UNORM,
//    PIPE_FORMAT_R16G16B16A16_UNORM,
//    PIPE_FORMAT_R16_USCALED,
//    PIPE_FORMAT_R16G16_USCALED,
//    PIPE_FORMAT_R16G16B16_USCALED,
//    PIPE_FORMAT_R16G16B16A16_USCALED,
//    PIPE_FORMAT_R16_SNORM,
//    PIPE_FORMAT_R16G16_SNORM,
//    PIPE_FORMAT_R16G16B16_SNORM,
//    PIPE_FORMAT_R16G16B16A16_SNORM,
//    PIPE_FORMAT_R16_SSCALED,
//    PIPE_FORMAT_R16G16_SSCALED,
//    PIPE_FORMAT_R16G16B16_SSCALED,
//    PIPE_FORMAT_R16G16B16A16_SSCALED,
//    PIPE_FORMAT_R8_UNORM,
//    PIPE_FORMAT_R8G8_UNORM,
//    PIPE_FORMAT_R8G8B8_UNORM,
//    PIPE_FORMAT_B8G8R8_UNORM,
//    PIPE_FORMAT_R8G8B8A8_UNORM,
//    PIPE_FORMAT_X8B8G8R8_UNORM,
//    PIPE_FORMAT_R8_USCALED,
//    PIPE_FORMAT_R8G8_USCALED,
//    PIPE_FORMAT_R8G8B8_USCALED,
//    PIPE_FORMAT_B8G8R8_USCALED,
//    PIPE_FORMAT_R8G8B8A8_USCALED,
//    PIPE_FORMAT_B8G8R8A8_USCALED,
//    PIPE_FORMAT_A8B8G8R8_USCALED,
//    PIPE_FORMAT_R8_SNORM,
//    PIPE_FORMAT_R8G8_SNORM,
//    PIPE_FORMAT_R8G8B8_SNORM,
//    PIPE_FORMAT_B8G8R8_SNORM,
//    PIPE_FORMAT_R8G8B8A8_SNORM,
//    PIPE_FORMAT_B8G8R8A8_SNORM,
//    PIPE_FORMAT_R8_SSCALED,
//    PIPE_FORMAT_R8G8_SSCALED,
//    PIPE_FORMAT_R8G8B8_SSCALED,
//    PIPE_FORMAT_B8G8R8_SSCALED,
//    PIPE_FORMAT_R8G8B8A8_SSCALED,
//    PIPE_FORMAT_B8G8R8A8_SSCALED,
//    PIPE_FORMAT_A8B8G8R8_SSCALED,
//    PIPE_FORMAT_R32_FIXED,
//    PIPE_FORMAT_R32G32_FIXED,
//    PIPE_FORMAT_R32G32B32_FIXED,
//    PIPE_FORMAT_R32G32B32A32_FIXED,
//    PIPE_FORMAT_R16_FLOAT,
//    PIPE_FORMAT_R16G16_FLOAT,
//    PIPE_FORMAT_R16G16B16_FLOAT,
//    PIPE_FORMAT_R16G16B16A16_FLOAT,
//
//    /* sRGB formats */
//    PIPE_FORMAT_L8_SRGB,
//    PIPE_FORMAT_R8_SRGB,
//    PIPE_FORMAT_L8A8_SRGB,
//    PIPE_FORMAT_R8G8_SRGB,
//    PIPE_FORMAT_R8G8B8_SRGB,
//    PIPE_FORMAT_B8G8R8_SRGB,
//    PIPE_FORMAT_A8B8G8R8_SRGB,
//    PIPE_FORMAT_X8B8G8R8_SRGB,
//    PIPE_FORMAT_B8G8R8A8_SRGB,
//    PIPE_FORMAT_B8G8R8X8_SRGB,
//    PIPE_FORMAT_A8R8G8B8_SRGB,
//    PIPE_FORMAT_X8R8G8B8_SRGB,
//    PIPE_FORMAT_R8G8B8A8_SRGB,
//
//    /* compressed formats */
//    PIPE_FORMAT_DXT1_RGB,
//    PIPE_FORMAT_DXT1_RGBA,
//    PIPE_FORMAT_DXT3_RGBA,
//    PIPE_FORMAT_DXT5_RGBA,
//
//    /* sRGB, compressed */
//    PIPE_FORMAT_DXT1_SRGB,
//    PIPE_FORMAT_DXT1_SRGBA,
//    PIPE_FORMAT_DXT3_SRGBA,
//    PIPE_FORMAT_DXT5_SRGBA,
//
//    /* rgtc compressed */
//    PIPE_FORMAT_RGTC1_UNORM,
//    PIPE_FORMAT_RGTC1_SNORM,
//    PIPE_FORMAT_RGTC2_UNORM,
//    PIPE_FORMAT_RGTC2_SNORM,
//
//    PIPE_FORMAT_R8G8_B8G8_UNORM,
//    PIPE_FORMAT_G8R8_G8B8_UNORM,
//
//    /* mixed formats */
//    PIPE_FORMAT_R8SG8SB8UX8U_NORM,
//    PIPE_FORMAT_R5SG5SB6U_NORM,
//
//    /* TODO: re-order these */
//    PIPE_FORMAT_A8B8G8R8_UNORM,
//    PIPE_FORMAT_B5G5R5X1_UNORM,
//    PIPE_FORMAT_R10G10B10A2_USCALED,
//    PIPE_FORMAT_R11G11B10_FLOAT,
//    PIPE_FORMAT_R9G9B9E5_FLOAT,
//    PIPE_FORMAT_Z32_FLOAT_S8X24_UINT,
//    PIPE_FORMAT_R1_UNORM,
//    PIPE_FORMAT_R10G10B10X2_USCALED,
//    PIPE_FORMAT_R10G10B10X2_SNORM,
//    PIPE_FORMAT_L4A4_UNORM,
//    PIPE_FORMAT_A2R10G10B10_UNORM,
//    PIPE_FORMAT_A2B10G10R10_UNORM,
//    PIPE_FORMAT_B10G10R10A2_UNORM,
//    PIPE_FORMAT_R10SG10SB10SA2U_NORM,
//    PIPE_FORMAT_R8G8Bx_SNORM,
//    PIPE_FORMAT_R8G8B8X8_UNORM,
//    PIPE_FORMAT_B4G4R4X4_UNORM,
//
//    /* some stencil samplers formats */
//    PIPE_FORMAT_X24S8_UINT,
//    PIPE_FORMAT_S8X24_UINT,
//    PIPE_FORMAT_X32_S8X24_UINT,
//
//    PIPE_FORMAT_R3G3B2_UNORM,
//    PIPE_FORMAT_B2G3R3_UNORM,
//    PIPE_FORMAT_L16A16_UNORM,
//    PIPE_FORMAT_A16_UNORM,
//    PIPE_FORMAT_I16_UNORM,
//
//    PIPE_FORMAT_LATC1_UNORM,
//    PIPE_FORMAT_LATC1_SNORM,
//    PIPE_FORMAT_LATC2_UNORM,
//    PIPE_FORMAT_LATC2_SNORM,
//
//    PIPE_FORMAT_A8_SNORM,
//    PIPE_FORMAT_L8_SNORM,
//    PIPE_FORMAT_L8A8_SNORM,
//    PIPE_FORMAT_I8_SNORM,
//    PIPE_FORMAT_A16_SNORM,
//    PIPE_FORMAT_L16_SNORM,
//    PIPE_FORMAT_L16A16_SNORM,
//    PIPE_FORMAT_I16_SNORM,
//
//    PIPE_FORMAT_A16_FLOAT,
//    PIPE_FORMAT_L16_FLOAT,
//    PIPE_FORMAT_L16A16_FLOAT,
//    PIPE_FORMAT_I16_FLOAT,
//    PIPE_FORMAT_A32_FLOAT,
//    PIPE_FORMAT_L32_FLOAT,
//    PIPE_FORMAT_L32A32_FLOAT,
//    PIPE_FORMAT_I32_FLOAT,
//
//    PIPE_FORMAT_YV12,
//    PIPE_FORMAT_YV16,
//    PIPE_FORMAT_IYUV,  /**< aka I420 */
//    PIPE_FORMAT_NV12,
//    PIPE_FORMAT_NV21,
//
//    /* PIPE_FORMAT_Y8_U8_V8_420_UNORM = IYUV */
//    /* PIPE_FORMAT_Y8_U8V8_420_UNORM = NV12 */
//    PIPE_FORMAT_Y8_U8_V8_422_UNORM,
//    PIPE_FORMAT_Y8_U8V8_422_UNORM,
//    PIPE_FORMAT_Y8_U8_V8_444_UNORM,
//
//    PIPE_FORMAT_Y16_U16_V16_420_UNORM,
//    /* PIPE_FORMAT_Y16_U16V16_420_UNORM */
//    PIPE_FORMAT_Y16_U16_V16_422_UNORM,
//    PIPE_FORMAT_Y16_U16V16_422_UNORM,
//    PIPE_FORMAT_Y16_U16_V16_444_UNORM,
//
//    PIPE_FORMAT_A4R4_UNORM,
//    PIPE_FORMAT_R4A4_UNORM,
//    PIPE_FORMAT_R8A8_UNORM,
//    PIPE_FORMAT_A8R8_UNORM,
//
//    PIPE_FORMAT_R10G10B10A2_SSCALED,
//    PIPE_FORMAT_R10G10B10A2_SNORM,
//
//    PIPE_FORMAT_B10G10R10A2_USCALED,
//    PIPE_FORMAT_B10G10R10A2_SSCALED,
//    PIPE_FORMAT_B10G10R10A2_SNORM,
//
//    PIPE_FORMAT_R8_UINT,
//    PIPE_FORMAT_R8G8_UINT,
//    PIPE_FORMAT_R8G8B8_UINT,
//    PIPE_FORMAT_R8G8B8A8_UINT,
//
//    PIPE_FORMAT_R8_SINT,
//    PIPE_FORMAT_R8G8_SINT,
//    PIPE_FORMAT_R8G8B8_SINT,
//    PIPE_FORMAT_R8G8B8A8_SINT,
//
//    PIPE_FORMAT_R16_UINT,
//    PIPE_FORMAT_R16G16_UINT,
//    PIPE_FORMAT_R16G16B16_UINT,
//    PIPE_FORMAT_R16G16B16A16_UINT,
//
//    PIPE_FORMAT_R16_SINT,
//    PIPE_FORMAT_R16G16_SINT,
//    PIPE_FORMAT_R16G16B16_SINT,
//    PIPE_FORMAT_R16G16B16A16_SINT,
//
//    PIPE_FORMAT_R32_UINT,
//    PIPE_FORMAT_R32G32_UINT,
//    PIPE_FORMAT_R32G32B32_UINT,
//    PIPE_FORMAT_R32G32B32A32_UINT,
//
//    PIPE_FORMAT_R32_SINT,
//    PIPE_FORMAT_R32G32_SINT,
//    PIPE_FORMAT_R32G32B32_SINT,
//    PIPE_FORMAT_R32G32B32A32_SINT,
//
//    PIPE_FORMAT_R64_UINT,
//    PIPE_FORMAT_R64_SINT,
//
//    PIPE_FORMAT_A8_UINT,
//    PIPE_FORMAT_I8_UINT,
//    PIPE_FORMAT_L8_UINT,
//    PIPE_FORMAT_L8A8_UINT,
//
//    PIPE_FORMAT_A8_SINT,
//    PIPE_FORMAT_I8_SINT,
//    PIPE_FORMAT_L8_SINT,
//    PIPE_FORMAT_L8A8_SINT,
//
//    PIPE_FORMAT_A16_UINT,
//    PIPE_FORMAT_I16_UINT,
//    PIPE_FORMAT_L16_UINT,
//    PIPE_FORMAT_L16A16_UINT,
//
//    PIPE_FORMAT_A16_SINT,
//    PIPE_FORMAT_I16_SINT,
//    PIPE_FORMAT_L16_SINT,
//    PIPE_FORMAT_L16A16_SINT,
//
//    PIPE_FORMAT_A32_UINT,
//    PIPE_FORMAT_I32_UINT,
//    PIPE_FORMAT_L32_UINT,
//    PIPE_FORMAT_L32A32_UINT,
//
//    PIPE_FORMAT_A32_SINT,
//    PIPE_FORMAT_I32_SINT,
//    PIPE_FORMAT_L32_SINT,
//    PIPE_FORMAT_L32A32_SINT,
//
//    PIPE_FORMAT_B8G8R8_UINT,
//    PIPE_FORMAT_B8G8R8A8_UINT,
//
//    PIPE_FORMAT_B8G8R8_SINT,
//    PIPE_FORMAT_B8G8R8A8_SINT,
//
//    PIPE_FORMAT_A8R8G8B8_UINT,
//    PIPE_FORMAT_A8B8G8R8_UINT,
//    PIPE_FORMAT_A2R10G10B10_UINT,
//    PIPE_FORMAT_A2B10G10R10_UINT,
//    PIPE_FORMAT_B10G10R10A2_UINT,
//    PIPE_FORMAT_B10G10R10A2_SINT,
//    PIPE_FORMAT_R5G6B5_UINT,
//    PIPE_FORMAT_B5G6R5_UINT,
//    PIPE_FORMAT_R5G5B5A1_UINT,
//    PIPE_FORMAT_B5G5R5A1_UINT,
//    PIPE_FORMAT_A1R5G5B5_UINT,
//    PIPE_FORMAT_A1B5G5R5_UINT,
//    PIPE_FORMAT_R4G4B4A4_UINT,
//    PIPE_FORMAT_B4G4R4A4_UINT,
//    PIPE_FORMAT_A4R4G4B4_UINT,
//    PIPE_FORMAT_A4B4G4R4_UINT,
//    PIPE_FORMAT_R3G3B2_UINT,
//    PIPE_FORMAT_B2G3R3_UINT,
//
//    PIPE_FORMAT_ETC1_RGB8,
//
//    PIPE_FORMAT_R8G8_R8B8_UNORM,
//    PIPE_FORMAT_G8R8_B8R8_UNORM,
//
//    PIPE_FORMAT_R8G8B8X8_SNORM,
//    PIPE_FORMAT_R8G8B8X8_SRGB,
//    PIPE_FORMAT_R8G8B8X8_UINT,
//    PIPE_FORMAT_R8G8B8X8_SINT,
//    PIPE_FORMAT_B10G10R10X2_UNORM,
//    PIPE_FORMAT_R16G16B16X16_UNORM,
//    PIPE_FORMAT_R16G16B16X16_SNORM,
//    PIPE_FORMAT_R16G16B16X16_FLOAT,
//    PIPE_FORMAT_R16G16B16X16_UINT,
//    PIPE_FORMAT_R16G16B16X16_SINT,
//    PIPE_FORMAT_R32G32B32X32_FLOAT,
//    PIPE_FORMAT_R32G32B32X32_UINT,
//    PIPE_FORMAT_R32G32B32X32_SINT,
//
//    PIPE_FORMAT_R8A8_SNORM,
//    PIPE_FORMAT_R16A16_UNORM,
//    PIPE_FORMAT_R16A16_SNORM,
//    PIPE_FORMAT_R16A16_FLOAT,
//    PIPE_FORMAT_R32A32_FLOAT,
//    PIPE_FORMAT_R8A8_UINT,
//    PIPE_FORMAT_R8A8_SINT,
//    PIPE_FORMAT_R16A16_UINT,
//    PIPE_FORMAT_R16A16_SINT,
//    PIPE_FORMAT_R32A32_UINT,
//    PIPE_FORMAT_R32A32_SINT,
//    PIPE_FORMAT_R10G10B10A2_UINT,
//    PIPE_FORMAT_R10G10B10A2_SINT,
//
//    PIPE_FORMAT_B5G6R5_SRGB,
//
//    PIPE_FORMAT_BPTC_RGBA_UNORM,
//    PIPE_FORMAT_BPTC_SRGBA,
//    PIPE_FORMAT_BPTC_RGB_FLOAT,
//    PIPE_FORMAT_BPTC_RGB_UFLOAT,
//
//    PIPE_FORMAT_G8R8_UNORM,
//    PIPE_FORMAT_G8R8_SNORM,
//    PIPE_FORMAT_G16R16_UNORM,
//    PIPE_FORMAT_G16R16_SNORM,
//
//    PIPE_FORMAT_A8B8G8R8_SNORM,
//    PIPE_FORMAT_X8B8G8R8_SNORM,
//
//    PIPE_FORMAT_ETC2_RGB8,
//    PIPE_FORMAT_ETC2_SRGB8,
//    PIPE_FORMAT_ETC2_RGB8A1,
//    PIPE_FORMAT_ETC2_SRGB8A1,
//    PIPE_FORMAT_ETC2_RGBA8,
//    PIPE_FORMAT_ETC2_SRGBA8,
//    PIPE_FORMAT_ETC2_R11_UNORM,
//    PIPE_FORMAT_ETC2_R11_SNORM,
//    PIPE_FORMAT_ETC2_RG11_UNORM,
//    PIPE_FORMAT_ETC2_RG11_SNORM,
//
//    PIPE_FORMAT_ASTC_4x4,
//    PIPE_FORMAT_ASTC_5x4,
//    PIPE_FORMAT_ASTC_5x5,
//    PIPE_FORMAT_ASTC_6x5,
//    PIPE_FORMAT_ASTC_6x6,
//    PIPE_FORMAT_ASTC_8x5,
//    PIPE_FORMAT_ASTC_8x6,
//    PIPE_FORMAT_ASTC_8x8,
//    PIPE_FORMAT_ASTC_10x5,
//    PIPE_FORMAT_ASTC_10x6,
//    PIPE_FORMAT_ASTC_10x8,
//    PIPE_FORMAT_ASTC_10x10,
//    PIPE_FORMAT_ASTC_12x10,
//    PIPE_FORMAT_ASTC_12x12,
//
//    PIPE_FORMAT_ASTC_4x4_SRGB,
//    PIPE_FORMAT_ASTC_5x4_SRGB,
//    PIPE_FORMAT_ASTC_5x5_SRGB,
//    PIPE_FORMAT_ASTC_6x5_SRGB,
//    PIPE_FORMAT_ASTC_6x6_SRGB,
//    PIPE_FORMAT_ASTC_8x5_SRGB,
//    PIPE_FORMAT_ASTC_8x6_SRGB,
//    PIPE_FORMAT_ASTC_8x8_SRGB,
//    PIPE_FORMAT_ASTC_10x5_SRGB,
//    PIPE_FORMAT_ASTC_10x6_SRGB,
//    PIPE_FORMAT_ASTC_10x8_SRGB,
//    PIPE_FORMAT_ASTC_10x10_SRGB,
//    PIPE_FORMAT_ASTC_12x10_SRGB,
//    PIPE_FORMAT_ASTC_12x12_SRGB,
//
//    PIPE_FORMAT_ASTC_3x3x3,
//    PIPE_FORMAT_ASTC_4x3x3,
//    PIPE_FORMAT_ASTC_4x4x3,
//    PIPE_FORMAT_ASTC_4x4x4,
//    PIPE_FORMAT_ASTC_5x4x4,
//    PIPE_FORMAT_ASTC_5x5x4,
//    PIPE_FORMAT_ASTC_5x5x5,
//    PIPE_FORMAT_ASTC_6x5x5,
//    PIPE_FORMAT_ASTC_6x6x5,
//    PIPE_FORMAT_ASTC_6x6x6,
//
//    PIPE_FORMAT_ASTC_3x3x3_SRGB,
//    PIPE_FORMAT_ASTC_4x3x3_SRGB,
//    PIPE_FORMAT_ASTC_4x4x3_SRGB,
//    PIPE_FORMAT_ASTC_4x4x4_SRGB,
//    PIPE_FORMAT_ASTC_5x4x4_SRGB,
//    PIPE_FORMAT_ASTC_5x5x4_SRGB,
//    PIPE_FORMAT_ASTC_5x5x5_SRGB,
//    PIPE_FORMAT_ASTC_6x5x5_SRGB,
//    PIPE_FORMAT_ASTC_6x6x5_SRGB,
//    PIPE_FORMAT_ASTC_6x6x6_SRGB,
//
//    PIPE_FORMAT_FXT1_RGB,
//    PIPE_FORMAT_FXT1_RGBA,
//
//    PIPE_FORMAT_P010,
//    PIPE_FORMAT_P012,
//    PIPE_FORMAT_P016,
//
//    PIPE_FORMAT_R10G10B10X2_UNORM,
//    PIPE_FORMAT_A1R5G5B5_UNORM,
//    PIPE_FORMAT_A1B5G5R5_UNORM,
//    PIPE_FORMAT_X1B5G5R5_UNORM,
//    PIPE_FORMAT_R5G5B5A1_UNORM,
//    PIPE_FORMAT_A4R4G4B4_UNORM,
//    PIPE_FORMAT_A4B4G4R4_UNORM,
//
//    PIPE_FORMAT_G8R8_SINT,
//    PIPE_FORMAT_A8B8G8R8_SINT,
//    PIPE_FORMAT_X8B8G8R8_SINT,
//
//    PIPE_FORMAT_ATC_RGB,
//    PIPE_FORMAT_ATC_RGBA_EXPLICIT,
//    PIPE_FORMAT_ATC_RGBA_INTERPOLATED,
//
//    PIPE_FORMAT_Z24_UNORM_S8_UINT_AS_R8G8B8A8,
//
//    PIPE_FORMAT_AYUV,
//    PIPE_FORMAT_XYUV,
//
//    PIPE_FORMAT_R8_G8B8_420_UNORM,
//
//    PIPE_FORMAT_B8G8R8X8_SNORM,
//    PIPE_FORMAT_B8G8R8X8_UINT,
//    PIPE_FORMAT_B8G8R8X8_SINT,
//    PIPE_FORMAT_A8R8G8B8_SNORM,
//    PIPE_FORMAT_A8R8G8B8_SINT,
//    PIPE_FORMAT_X8R8G8B8_SNORM,
//    PIPE_FORMAT_X8R8G8B8_SINT,
//    PIPE_FORMAT_R5G5B5X1_UNORM,
//    PIPE_FORMAT_X1R5G5B5_UNORM,
//    PIPE_FORMAT_R4G4B4X4_UNORM,
//    PIPE_FORMAT_B10G10R10X2_SNORM,
//    PIPE_FORMAT_R5G6B5_SRGB,
//
//    PIPE_FORMAT_COUNT
//};
//
//
//struct pipe_resource
//{
//    struct pipe_reference reference;
//    struct pipe_screen *screen;
//    enum pipe_texture_target target;
//    enum pipe_format format;
//
//    unsigned width0;
//    unsigned height0;
//    unsigned depth0;
//    unsigned array_size;
//
//    unsigned last_level:8;
//    unsigned nr_samples:8;
//    unsigned usage:8;
//
//    unsigned bind;
//    unsigned flags;
//};
//
//enum pipe_cap {
//    PIPE_CAP_NPOT_TEXTURES = 1,
//    PIPE_CAP_TWO_SIDED_STENCIL = 2,
//    PIPE_CAP_MAX_DUAL_SOURCE_RENDER_TARGETS = 4,
//    PIPE_CAP_ANISOTROPIC_FILTER = 5,
//    PIPE_CAP_POINT_SPRITE = 6,
//    PIPE_CAP_MAX_RENDER_TARGETS = 7,
//    PIPE_CAP_OCCLUSION_QUERY = 8,
//    PIPE_CAP_QUERY_TIME_ELAPSED = 9,
//    PIPE_CAP_TEXTURE_SHADOW_MAP = 10,
//    PIPE_CAP_TEXTURE_SWIZZLE = 11,
//    PIPE_CAP_MAX_TEXTURE_2D_LEVELS = 12,
//    PIPE_CAP_MAX_TEXTURE_3D_LEVELS = 13,
//    PIPE_CAP_MAX_TEXTURE_CUBE_LEVELS = 14,
//    PIPE_CAP_TEXTURE_MIRROR_CLAMP = 25,
//    PIPE_CAP_BLEND_EQUATION_SEPARATE = 28,
//    PIPE_CAP_SM3 = 29,  /*< Shader Model, supported */
//    PIPE_CAP_MAX_STREAM_OUTPUT_BUFFERS = 30,
//    PIPE_CAP_PRIMITIVE_RESTART = 31,
//    /** blend enables and write masks per rendertarget */
//    PIPE_CAP_INDEP_BLEND_ENABLE = 33,
//    /** different blend funcs per rendertarget */
//    PIPE_CAP_INDEP_BLEND_FUNC = 34,
//    PIPE_CAP_MAX_TEXTURE_ARRAY_LAYERS = 36,
//    PIPE_CAP_TGSI_FS_COORD_ORIGIN_UPPER_LEFT = 37,
//    PIPE_CAP_TGSI_FS_COORD_ORIGIN_LOWER_LEFT = 38,
//    PIPE_CAP_TGSI_FS_COORD_PIXEL_CENTER_HALF_INTEGER = 39,
//    PIPE_CAP_TGSI_FS_COORD_PIXEL_CENTER_INTEGER = 40,
//    PIPE_CAP_DEPTH_CLIP_DISABLE = 41,
//    PIPE_CAP_SHADER_STENCIL_EXPORT = 42,
//    PIPE_CAP_TGSI_INSTANCEID = 43,
//    PIPE_CAP_VERTEX_ELEMENT_INSTANCE_DIVISOR = 44,
//    PIPE_CAP_FRAGMENT_COLOR_CLAMPED = 45,
//    PIPE_CAP_MIXED_COLORBUFFER_FORMATS = 46,
//    PIPE_CAP_SEAMLESS_CUBE_MAP = 47,
//    PIPE_CAP_SEAMLESS_CUBE_MAP_PER_TEXTURE = 48,
//    PIPE_CAP_MIN_TEXEL_OFFSET = 50,
//    PIPE_CAP_MAX_TEXEL_OFFSET = 51,
//    PIPE_CAP_CONDITIONAL_RENDER = 52,
//    PIPE_CAP_TEXTURE_BARRIER = 53,
//    PIPE_CAP_MAX_STREAM_OUTPUT_SEPARATE_COMPONENTS = 55,
//    PIPE_CAP_MAX_STREAM_OUTPUT_INTERLEAVED_COMPONENTS = 56,
//    PIPE_CAP_STREAM_OUTPUT_PAUSE_RESUME = 57,
//    PIPE_CAP_TGSI_CAN_COMPACT_CONSTANTS = 59, /* temporary */
//    PIPE_CAP_VERTEX_COLOR_UNCLAMPED = 60,
//    PIPE_CAP_VERTEX_COLOR_CLAMPED = 61,
//    PIPE_CAP_GLSL_FEATURE_LEVEL = 62,
//    PIPE_CAP_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION = 63,
//    PIPE_CAP_USER_VERTEX_BUFFERS = 64,
//    PIPE_CAP_VERTEX_BUFFER_OFFSET_4BYTE_ALIGNED_ONLY = 65,
//    PIPE_CAP_VERTEX_BUFFER_STRIDE_4BYTE_ALIGNED_ONLY = 66,
//    PIPE_CAP_VERTEX_ELEMENT_SRC_OFFSET_4BYTE_ALIGNED_ONLY = 67,
//    PIPE_CAP_COMPUTE = 68,
//    PIPE_CAP_USER_INDEX_BUFFERS = 69,
//    PIPE_CAP_USER_CONSTANT_BUFFERS = 70,
//    PIPE_CAP_CONSTANT_BUFFER_OFFSET_ALIGNMENT = 71,
//    PIPE_CAP_START_INSTANCE = 72,
//    PIPE_CAP_QUERY_TIMESTAMP = 73,
//    PIPE_CAP_TEXTURE_MULTISAMPLE = 74,
//    PIPE_CAP_MIN_MAP_BUFFER_ALIGNMENT = 75,
//    PIPE_CAP_CUBE_MAP_ARRAY = 76,
//    PIPE_CAP_TEXTURE_BUFFER_OBJECTS = 77,
//    PIPE_CAP_TEXTURE_BUFFER_OFFSET_ALIGNMENT = 78,
//    PIPE_CAP_TGSI_TEXCOORD = 79,
//    PIPE_CAP_PREFER_BLIT_BASED_TEXTURE_TRANSFER = 80,
//    PIPE_CAP_QUERY_PIPELINE_STATISTICS = 81,
//    PIPE_CAP_TEXTURE_BORDER_COLOR_QUIRK = 82,
//    PIPE_CAP_MAX_TEXTURE_BUFFER_SIZE = 83,
//    PIPE_CAP_MAX_VIEWPORTS = 84,
//    PIPE_CAP_ENDIANNESS = 85,
//    PIPE_CAP_MIXED_FRAMEBUFFER_SIZES = 86,
//    PIPE_CAP_TGSI_VS_LAYER_VIEWPORT = 87,
//    PIPE_CAP_MAX_GEOMETRY_OUTPUT_VERTICES = 88,
//    PIPE_CAP_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS = 89,
//    PIPE_CAP_MAX_TEXTURE_GATHER_COMPONENTS = 90,
//    PIPE_CAP_TEXTURE_GATHER_SM5 = 91,
//    PIPE_CAP_BUFFER_MAP_PERSISTENT_COHERENT = 92,
//    PIPE_CAP_FAKE_SW_MSAA = 93,
//    PIPE_CAP_TEXTURE_QUERY_LOD = 94,
//    PIPE_CAP_MIN_TEXTURE_GATHER_OFFSET = 95,
//    PIPE_CAP_MAX_TEXTURE_GATHER_OFFSET = 96,
//    PIPE_CAP_SAMPLE_SHADING = 97,
//    PIPE_CAP_TEXTURE_GATHER_OFFSETS = 98,
//    PIPE_CAP_TGSI_VS_WINDOW_SPACE_POSITION = 99,
//    PIPE_CAP_MAX_VERTEX_STREAMS = 100,
//    PIPE_CAP_DRAW_INDIRECT = 101,
//    PIPE_CAP_TGSI_FS_FINE_DERIVATIVE = 102,
//    PIPE_CAP_VENDOR_ID = 103,
//    PIPE_CAP_DEVICE_ID = 104,
//    PIPE_CAP_ACCELERATED = 105,
//    PIPE_CAP_VIDEO_MEMORY = 106,
//    PIPE_CAP_UMA = 107,
//    PIPE_CAP_CONDITIONAL_RENDER_INVERTED = 108,
//    PIPE_CAP_MAX_VERTEX_ATTRIB_STRIDE = 109,
//    PIPE_CAP_SAMPLER_VIEW_TARGET = 110,
//    PIPE_CAP_CLIP_HALFZ = 111,
//    PIPE_CAP_VERTEXID_NOBASE = 112,
//    PIPE_CAP_POLYGON_OFFSET_CLAMP = 113,
//};
//
//enum pipe_capf
//{
//    PIPE_CAPF_MAX_LINE_WIDTH,
//    PIPE_CAPF_MAX_LINE_WIDTH_AA,
//    PIPE_CAPF_MAX_POINT_WIDTH,
//    PIPE_CAPF_MAX_POINT_WIDTH_AA,
//    PIPE_CAPF_MAX_TEXTURE_ANISOTROPY,
//    PIPE_CAPF_MAX_TEXTURE_LOD_BIAS,
//    PIPE_CAPF_GUARD_BAND_LEFT,
//    PIPE_CAPF_GUARD_BAND_TOP,
//    PIPE_CAPF_GUARD_BAND_RIGHT,
//    PIPE_CAPF_GUARD_BAND_BOTTOM
//};
//
//enum pipe_shader_type {
//    PIPE_SHADER_VERTEX,
//    PIPE_SHADER_FRAGMENT,
//    PIPE_SHADER_GEOMETRY,
//    PIPE_SHADER_TESS_CTRL,
//    PIPE_SHADER_TESS_EVAL,
//    PIPE_SHADER_COMPUTE,
//    PIPE_SHADER_TYPES,
//};
//
//enum pipe_shader_cap
//{
//    PIPE_SHADER_CAP_MAX_INSTRUCTIONS, /* if 0, it means the stage is unsupported */
//    PIPE_SHADER_CAP_MAX_ALU_INSTRUCTIONS,
//    PIPE_SHADER_CAP_MAX_TEX_INSTRUCTIONS,
//    PIPE_SHADER_CAP_MAX_TEX_INDIRECTIONS,
//    PIPE_SHADER_CAP_MAX_CONTROL_FLOW_DEPTH,
//    PIPE_SHADER_CAP_MAX_INPUTS,
//    PIPE_SHADER_CAP_MAX_OUTPUTS,
//    PIPE_SHADER_CAP_MAX_CONST_BUFFER_SIZE,
//    PIPE_SHADER_CAP_MAX_CONST_BUFFERS,
//    PIPE_SHADER_CAP_MAX_TEMPS,
//    /* boolean caps */
//    PIPE_SHADER_CAP_TGSI_CONT_SUPPORTED,
//    PIPE_SHADER_CAP_INDIRECT_INPUT_ADDR,
//    PIPE_SHADER_CAP_INDIRECT_OUTPUT_ADDR,
//    PIPE_SHADER_CAP_INDIRECT_TEMP_ADDR,
//    PIPE_SHADER_CAP_INDIRECT_CONST_ADDR,
//    PIPE_SHADER_CAP_SUBROUTINES, /* BGNSUB, ENDSUB, CAL, RET */
//    PIPE_SHADER_CAP_INTEGERS,
//    PIPE_SHADER_CAP_INT64_ATOMICS,
//    PIPE_SHADER_CAP_FP16,
//    PIPE_SHADER_CAP_FP16_DERIVATIVES,
//    PIPE_SHADER_CAP_FP16_CONST_BUFFERS,
//    PIPE_SHADER_CAP_INT16,
//    PIPE_SHADER_CAP_GLSL_16BIT_CONSTS,
//    PIPE_SHADER_CAP_MAX_TEXTURE_SAMPLERS,
//    PIPE_SHADER_CAP_PREFERRED_IR,
//    PIPE_SHADER_CAP_TGSI_SQRT_SUPPORTED,
//    PIPE_SHADER_CAP_MAX_SAMPLER_VIEWS,
//    PIPE_SHADER_CAP_TGSI_DROUND_SUPPORTED, /* all rounding modes */
//    PIPE_SHADER_CAP_TGSI_DFRACEXP_DLDEXP_SUPPORTED,
//    PIPE_SHADER_CAP_TGSI_FMA_SUPPORTED,
//    PIPE_SHADER_CAP_TGSI_ANY_INOUT_DECL_RANGE,
//    PIPE_SHADER_CAP_MAX_UNROLL_ITERATIONS_HINT,
//    PIPE_SHADER_CAP_MAX_SHADER_BUFFERS,
//    PIPE_SHADER_CAP_SUPPORTED_IRS,
//    PIPE_SHADER_CAP_MAX_SHADER_IMAGES,
//    PIPE_SHADER_CAP_LOWER_IF_THRESHOLD,
//    PIPE_SHADER_CAP_TGSI_SKIP_MERGE_REGISTERS,
//    PIPE_SHADER_CAP_TGSI_LDEXP_SUPPORTED,
//    PIPE_SHADER_CAP_MAX_HW_ATOMIC_COUNTERS,
//    PIPE_SHADER_CAP_MAX_HW_ATOMIC_COUNTER_BUFFERS,
//};
//
//enum pipe_video_profile
//{
//    PIPE_VIDEO_PROFILE_UNKNOWN,
//    PIPE_VIDEO_PROFILE_MPEG1,
//    PIPE_VIDEO_PROFILE_MPEG2_SIMPLE,
//    PIPE_VIDEO_PROFILE_MPEG2_MAIN,
//    PIPE_VIDEO_PROFILE_MPEG4_SIMPLE,
//    PIPE_VIDEO_PROFILE_MPEG4_ADVANCED_SIMPLE,
//    PIPE_VIDEO_PROFILE_VC1_SIMPLE,
//    PIPE_VIDEO_PROFILE_VC1_MAIN,
//    PIPE_VIDEO_PROFILE_VC1_ADVANCED,
//    PIPE_VIDEO_PROFILE_MPEG4_AVC_BASELINE,
//    PIPE_VIDEO_PROFILE_MPEG4_AVC_CONSTRAINED_BASELINE,
//    PIPE_VIDEO_PROFILE_MPEG4_AVC_MAIN,
//    PIPE_VIDEO_PROFILE_MPEG4_AVC_EXTENDED,
//    PIPE_VIDEO_PROFILE_MPEG4_AVC_HIGH,
//    PIPE_VIDEO_PROFILE_MPEG4_AVC_HIGH10,
//    PIPE_VIDEO_PROFILE_MPEG4_AVC_HIGH422,
//    PIPE_VIDEO_PROFILE_MPEG4_AVC_HIGH444,
//    PIPE_VIDEO_PROFILE_HEVC_MAIN,
//    PIPE_VIDEO_PROFILE_HEVC_MAIN_10,
//    PIPE_VIDEO_PROFILE_HEVC_MAIN_STILL,
//    PIPE_VIDEO_PROFILE_HEVC_MAIN_12,
//    PIPE_VIDEO_PROFILE_HEVC_MAIN_444,
//    PIPE_VIDEO_PROFILE_JPEG_BASELINE,
//    PIPE_VIDEO_PROFILE_VP9_PROFILE0,
//    PIPE_VIDEO_PROFILE_VP9_PROFILE2,
//    PIPE_VIDEO_PROFILE_AV1_MAIN,
//    PIPE_VIDEO_PROFILE_MAX
//};
//
//enum pipe_video_entrypoint
//{
//    PIPE_VIDEO_ENTRYPOINT_UNKNOWN,
//    PIPE_VIDEO_ENTRYPOINT_BITSTREAM,
//    PIPE_VIDEO_ENTRYPOINT_IDCT,
//    PIPE_VIDEO_ENTRYPOINT_MC,
//    PIPE_VIDEO_ENTRYPOINT_ENCODE
//};
//
//enum pipe_video_cap
//{
//    PIPE_VIDEO_CAP_SUPPORTED = 0,
//    PIPE_VIDEO_CAP_NPOT_TEXTURES = 1,
//    PIPE_VIDEO_CAP_MAX_WIDTH = 2,
//    PIPE_VIDEO_CAP_MAX_HEIGHT = 3,
//    PIPE_VIDEO_CAP_PREFERED_FORMAT = 4,
//    PIPE_VIDEO_CAP_PREFERS_INTERLACED = 5,
//    PIPE_VIDEO_CAP_SUPPORTS_PROGRESSIVE = 6,
//    PIPE_VIDEO_CAP_SUPPORTS_INTERLACED = 7,
//    PIPE_VIDEO_CAP_MAX_LEVEL = 8,
//    PIPE_VIDEO_CAP_STACKED_FRAMES = 9
//};
//
//enum pipe_shader_ir
//{
//    PIPE_SHADER_IR_TGSI,
//    PIPE_SHADER_IR_LLVM,
//    PIPE_SHADER_IR_NATIVE
//};
//
//enum pipe_compute_cap
//{
//    PIPE_COMPUTE_CAP_IR_TARGET,
//    PIPE_COMPUTE_CAP_GRID_DIMENSION,
//    PIPE_COMPUTE_CAP_MAX_GRID_SIZE,
//    PIPE_COMPUTE_CAP_MAX_BLOCK_SIZE,
//    PIPE_COMPUTE_CAP_MAX_THREADS_PER_BLOCK,
//    PIPE_COMPUTE_CAP_MAX_GLOBAL_SIZE,
//    PIPE_COMPUTE_CAP_MAX_LOCAL_SIZE,
//    PIPE_COMPUTE_CAP_MAX_PRIVATE_SIZE,
//    PIPE_COMPUTE_CAP_MAX_INPUT_SIZE,
//    PIPE_COMPUTE_CAP_MAX_MEM_ALLOC_SIZE,
//    PIPE_COMPUTE_CAP_MAX_CLOCK_FREQUENCY,
//    PIPE_COMPUTE_CAP_MAX_COMPUTE_UNITS,
//    PIPE_COMPUTE_CAP_IMAGES_SUPPORTED
//};
//
//enum pipe_resource_param
//{
//    PIPE_RESOURCE_PARAM_NPLANES,
//    PIPE_RESOURCE_PARAM_STRIDE,
//    PIPE_RESOURCE_PARAM_OFFSET,
//    PIPE_RESOURCE_PARAM_MODIFIER,
//    PIPE_RESOURCE_PARAM_HANDLE_TYPE_SHARED,
//    PIPE_RESOURCE_PARAM_HANDLE_TYPE_KMS,
//    PIPE_RESOURCE_PARAM_HANDLE_TYPE_FD,
//    PIPE_RESOURCE_PARAM_LAYER_STRIDE,
//};
//
//struct pipe_screen {
//    unsigned num_contexts;
//    struct u_transfer_helper *transfer_helper;
//    void (*destroy)( struct pipe_screen * );
//    const char *(*get_name)( struct pipe_screen * );
//    const char *(*get_vendor)( struct pipe_screen * );
//    const char *(*get_device_vendor)( struct pipe_screen * );
//    int (*get_param)( struct pipe_screen *, enum pipe_cap param );
//    float (*get_paramf)( struct pipe_screen *, enum pipe_capf param );
//    int (*get_shader_param)( struct pipe_screen *, enum pipe_shader_type shader,
//                             enum pipe_shader_cap param );
//    int (*get_video_param)( struct pipe_screen *,
//                            enum pipe_video_profile profile,
//                            enum pipe_video_entrypoint entrypoint,
//                            enum pipe_video_cap param );
//    int (*get_compute_param)(struct pipe_screen *,
//                             enum pipe_shader_ir ir_type,
//                             enum pipe_compute_cap param,
//                             void *ret);
//    void (*get_sample_pixel_grid)(struct pipe_screen *, unsigned sample_count,
//                                  unsigned *out_width, unsigned *out_height);
//    uint64_t (*get_timestamp)(struct pipe_screen *);
//    struct pipe_context * (*context_create)(struct pipe_screen *screen,
//                                            void *priv, unsigned flags);
//    bool (*is_format_supported)( struct pipe_screen *,
//                                 enum pipe_format format,
//                                 enum pipe_texture_target target,
//                                 unsigned sample_count,
//                                 unsigned storage_sample_count,
//                                 unsigned bindings );
//    bool (*is_video_format_supported)( struct pipe_screen *,
//                                       enum pipe_format format,
//                                       enum pipe_video_profile profile,
//                                       enum pipe_video_entrypoint entrypoint );
//    bool (*can_create_resource)(struct pipe_screen *screen,
//                                const struct pipe_resource *templat);
//    struct pipe_resource * (*resource_create)(struct pipe_screen *,
//                                              const struct pipe_resource *templat);
//    struct pipe_resource * (*resource_create_front)(struct pipe_screen *,
//                                                    const struct pipe_resource *templat,
//                                                    const void *map_front_private);
//    struct pipe_resource * (*resource_from_handle)(struct pipe_screen *,
//                                                   const struct pipe_resource *templat,
//                                                   struct winsys_handle *handle,
//                                                   unsigned usage);
//    struct pipe_resource * (*resource_from_user_memory)(struct pipe_screen *,
//                                                        const struct pipe_resource *t,
//                                                        void *user_memory);
//    bool (*check_resource_capability)(struct pipe_screen *screen,
//                                      struct pipe_resource *resource,
//                                      unsigned bind);
//    bool (*resource_get_handle)(struct pipe_screen *,
//                                struct pipe_context *context,
//                                struct pipe_resource *tex,
//                                struct winsys_handle *handle,
//                                unsigned usage);
//    bool (*resource_get_param)(struct pipe_screen *screen,
//                               struct pipe_context *context,
//                               struct pipe_resource *resource,
//                               unsigned plane,
//                               unsigned layer,
//                               unsigned level,
//                               enum pipe_resource_param param,
//                               unsigned handle_usage,
//                               uint64_t *value);
//    void (*resource_get_info)(struct pipe_screen *screen,
//                              struct pipe_resource *resource,
//                              unsigned *stride,
//                              unsigned *offset);
//    void (*resource_changed)(struct pipe_screen *, struct pipe_resource *pt);
//    void (*resource_destroy)(struct pipe_screen *,
//                             struct pipe_resource *pt);
//    void (*flush_frontbuffer)( struct pipe_screen *screen,
//                               struct pipe_context *ctx,
//                               struct pipe_resource *resource,
//                               unsigned level, unsigned layer,
//                               void *winsys_drawable_handle,
//                               struct pipe_box *subbox );
//    void (*fence_reference)( struct pipe_screen *screen,
//                             struct pipe_fence_handle **ptr,
//                             struct pipe_fence_handle *fence );
//    bool (*fence_finish)(struct pipe_screen *screen,
//                         struct pipe_context *ctx,
//                         struct pipe_fence_handle *fence,
//                         uint64_t timeout);
//    int (*fence_get_fd)(struct pipe_screen *screen,
//                        struct pipe_fence_handle *fence);
//    int (*get_driver_query_info)(struct pipe_screen *screen,
//                                 unsigned index,
//                                 struct pipe_driver_query_info *info);
//    int (*get_driver_query_group_info)(struct pipe_screen *screen,
//                                       unsigned index,
//                                       struct pipe_driver_query_group_info *info);
//    void (*query_memory_info)(struct pipe_screen *screen,
//                              struct pipe_memory_info *info);
//    const void *(*get_compiler_options)(struct pipe_screen *screen,
//                                        enum pipe_shader_ir ir,
//                                        enum pipe_shader_type shader);
//    struct disk_cache *(*get_disk_shader_cache)(struct pipe_screen *screen);
//    struct pipe_resource * (*resource_create_with_modifiers)(
//            struct pipe_screen *,
//            const struct pipe_resource *templat,
//            const uint64_t *modifiers, int count);
//    void (*query_dmabuf_modifiers)(struct pipe_screen *screen,
//                                   enum pipe_format format, int max,
//                                   uint64_t *modifiers,
//                                   unsigned int *external_only, int *count);
//    struct pipe_memory_object *(*memobj_create_from_handle)(struct pipe_screen *screen,
//                                                            struct winsys_handle *handle,
//                                                            bool dedicated);
//    void (*memobj_destroy)(struct pipe_screen *screen,
//                           struct pipe_memory_object *memobj);
//    struct pipe_resource * (*resource_from_memobj)(struct pipe_screen *screen,
//                                                   const struct pipe_resource *t,
//                                                   struct pipe_memory_object *memobj,
//                                                   uint64_t offset);
//    void (*get_driver_uuid)(struct pipe_screen *screen, char *uuid);
//    void (*get_device_uuid)(struct pipe_screen *screen, char *uuid);
//    void (*set_max_shader_compiler_threads)(struct pipe_screen *screen,
//                                            unsigned max_threads);
//    bool (*is_parallel_shader_compilation_finished)(struct pipe_screen *screen,
//                                                    void *shader,
//                                                    unsigned shader_type);
//    void (*set_damage_region)(struct pipe_screen *screen,
//                              struct pipe_resource *resource,
//                              unsigned int nrects,
//                              const struct pipe_box *rects);
//    void (*finalize_nir)(struct pipe_screen *screen, void *nir, bool optimize);
//    struct pipe_resource * (*resource_create_unbacked)(struct pipe_screen *,
//                                                       const struct pipe_resource *templat,
//                                                       uint64_t *size_required);
//    struct pipe_memory_allocation *(*allocate_memory)(struct pipe_screen *screen,
//                                                      uint64_t size);
//    void (*free_memory)(struct pipe_screen *screen,
//                        struct pipe_memory_allocation *);
//    void (*resource_bind_backing)(struct pipe_screen *screen,
//                                  struct pipe_resource *pt,
//                                  struct pipe_memory_allocation *pmem,
//                                  uint64_t offset);
//    void *(*map_memory)(struct pipe_screen *screen,
//                        struct pipe_memory_allocation *pmem);
//    void (*unmap_memory)(struct pipe_screen *screen,
//                         struct pipe_memory_allocation *pmem);
//    bool (*is_dmabuf_modifier_supported)(struct pipe_screen *screen,
//                                         uint64_t modifier, enum pipe_format,
//                                         bool *external_only);
//    unsigned int (*get_dmabuf_modifier_planes)(struct pipe_screen *screen,
//                                               uint64_t modifier,
//                                               enum pipe_format format);
//};
//
//
//enum st_attachment_type {
//    ST_ATTACHMENT_FRONT_LEFT,
//    ST_ATTACHMENT_BACK_LEFT,
//    ST_ATTACHMENT_FRONT_RIGHT,
//    ST_ATTACHMENT_BACK_RIGHT,
//    ST_ATTACHMENT_DEPTH_STENCIL,
//    ST_ATTACHMENT_ACCUM,
//
//    ST_ATTACHMENT_COUNT,
//    ST_ATTACHMENT_INVALID = -1
//};
//
//
//struct st_framebuffer_iface
//{
//    int32_t stamp;
//    uint32_t ID;
//    struct st_manager *state_manager;
//    void *st_manager_private;
//    const struct st_visual *visual;
//    bool (*flush_front)(struct st_context_iface *stctx,
//                        struct st_framebuffer_iface *stfbi,
//                        enum st_attachment_type statt);
//    bool (*validate)(struct st_context_iface *stctx,
//                     struct st_framebuffer_iface *stfbi,
//                     const enum st_attachment_type *statts,
//                     unsigned count,
//                     struct pipe_resource **out);
//    bool (*flush_swapbuffers) (struct st_context_iface *stctx,
//                               struct st_framebuffer_iface *stfbi);
//};
//
//
//struct st_visual
//{
//    /**
//     * Available buffers.  Bitfield of ST_ATTACHMENT_*_MASK bits.
//     */
//    unsigned buffer_mask;
//
//    /**
//     * Buffer formats.  The formats are always set even when the buffer is
//     * not available.
//     */
//    enum pipe_format color_format;
//    enum pipe_format depth_stencil_format;
//    enum pipe_format accum_format;
//    unsigned samples;
//
//    /**
//     * Desired render buffer.
//     */
//    enum st_attachment_type render_buffer;
//};
//
//
//typedef enum driOptionType {
//    DRI_BOOL, DRI_ENUM, DRI_INT, DRI_FLOAT, DRI_STRING, DRI_SECTION
//} driOptionType;
//
//
//typedef union driOptionValue {
//    unsigned char _bool;
//    int _int;
//    float _float;
//    char *_string;
//} driOptionValue;
//
//
//typedef struct driOptionRange {
//    driOptionValue start;
//    driOptionValue end;
//} driOptionRange;
//
//
//typedef struct driOptionInfo {
//    char *name;
//    driOptionType type;
//    driOptionRange range;
//} driOptionInfo;
//
//
//typedef struct driOptionCache {
//    driOptionInfo *info;
//    driOptionValue *values;
//    unsigned int tableSize;
//} driOptionCache;
//
//
//struct __DRIextensionRec {
//    const char *name;
//    int version;
//};
//
//
//struct __DRIbufferRec {
//    unsigned int attachment;
//    unsigned int name;
//    unsigned int pitch;
//    unsigned int cpp;
//    unsigned int flags;
//};
//
//
//struct __DRIdrawableRec {
//    void *driverPrivate;
//    void *loaderPrivate;
//    __DRIcontext *driContextPriv;
//    __DRIscreen *driScreenPriv;
//    int refcount;
//    unsigned int lastStamp;
//    int w, h;
//    struct {
//        unsigned int stamp;
//    } dri2;
//};
//
//
//enum __DRIYUVColorSpace {
//    __DRI_YUV_COLOR_SPACE_UNDEFINED = 0,
//    __DRI_YUV_COLOR_SPACE_ITU_REC601 = 0x327F,
//    __DRI_YUV_COLOR_SPACE_ITU_REC709 = 0x3280,
//    __DRI_YUV_COLOR_SPACE_ITU_REC2020 = 0x3281
//};
//
//enum __DRISampleRange {
//    __DRI_YUV_RANGE_UNDEFINED = 0,
//    __DRI_YUV_FULL_RANGE = 0x3282,
//    __DRI_YUV_NARROW_RANGE = 0x3283
//};
//
//enum __DRIChromaSiting {
//    __DRI_YUV_CHROMA_SITING_UNDEFINED = 0,
//    __DRI_YUV_CHROMA_SITING_0 = 0x3284,
//    __DRI_YUV_CHROMA_SITING_0_5 = 0x3285
//};
//
//
//struct dri_drawable
//{
//    struct st_framebuffer_iface base;
//    struct st_visual stvis;
//
//    struct dri_screen *screen;
//
//    /* dri */
//    __DRIdrawable *dPriv;
//    __DRIscreen *sPriv;
//
//    __DRIbuffer old[__DRI_BUFFER_COUNT];
//    unsigned old_num;
//    unsigned old_w;
//    unsigned old_h;
//
//    struct pipe_box *damage_rects;
//    unsigned int num_damage_rects;
//
//    struct pipe_resource *textures[ST_ATTACHMENT_COUNT];
//    struct pipe_resource *msaa_textures[ST_ATTACHMENT_COUNT];
//    unsigned int texture_mask, texture_stamp;
//
//    struct pipe_fence_handle *throttle_fence;
//    bool flushing; /* prevents recursion in dri_flush */
//
//    /* hooks filled in by dri2 & drisw */
//    void (*allocate_textures)(struct dri_context *ctx,
//                              struct dri_drawable *drawable,
//                              const enum st_attachment_type *statts,
//                              unsigned count);
//
//    void (*update_drawable_info)(struct dri_drawable *drawable);
//
//    bool (*flush_frontbuffer)(struct dri_context *ctx,
//                              struct dri_drawable *drawable,
//                              enum st_attachment_type statt);
//
//    void (*update_tex_buffer)(struct dri_drawable *drawable,
//                              struct dri_context *ctx,
//                              struct pipe_resource *res);
//    void (*flush_swapbuffers)(struct dri_context *ctx,
//                              struct dri_drawable *drawable);
//};
//
//
//struct __DRIimageRec {
//    struct pipe_resource *texture;
//    unsigned level;
//    unsigned layer;
//    uint32_t dri_format;
//    uint32_t dri_fourcc;
//    uint32_t dri_components;
//    unsigned use;
//    unsigned plane;
//    void *loader_private;
//    boolean imported_dmabuf;
//    enum __DRIYUVColorSpace yuv_color_space;
//    enum __DRISampleRange sample_range;
//    enum __DRIChromaSiting horizontal_siting;
//    enum __DRIChromaSiting vertical_siting;
//
//    /* DRI loader screen */
//    __DRIscreen *sPriv;
//};
//
//
//typedef struct __DRIimageLookupExtensionRec __DRIimageLookupExtension;
//struct __DRIimageLookupExtensionRec {
//    __DRIextension base;
//    __DRIimage *(*lookupEGLImage)(__DRIscreen *screen, void *image,
//                                  void *loaderPrivate);
//    unsigned char (*validateEGLImage)(void *image, void *loaderPrivate);
//    __DRIimage *(*lookupEGLImageValidated)(void *image, void *loaderPrivate);
//};
//
//
//typedef struct __DRIuseInvalidateExtensionRec __DRIuseInvalidateExtension;
//struct __DRIuseInvalidateExtensionRec {
//    __DRIextension base;
//};
//
//
//typedef struct __DRIbackgroundCallableExtensionRec __DRIbackgroundCallableExtension;
//struct __DRIbackgroundCallableExtensionRec {
//    __DRIextension base;
//    void (*setBackgroundContext)(void *loaderPrivate);
//    unsigned char (*isThreadSafe)(void *loaderPrivate);
//};
//
//
//struct __DRIimageLoaderExtensionRec {
//    __DRIextension base;
//    int (*getBuffers)(__DRIdrawable *driDrawable,
//                      unsigned int format,
//                      uint32_t *stamp,
//                      void *loaderPrivate,
//                      uint32_t buffer_mask,
//                      struct __DRIimageList *buffers);
//    void (*flushFrontBuffer)(__DRIdrawable *driDrawable, void *loaderPrivate);
//    unsigned (*getCapability)(void *loaderPrivate, enum dri_loader_cap cap);
//    void (*flushSwapBuffers)(__DRIdrawable *driDrawable, void *loaderPrivate);
//    void (*destroyLoaderImageState)(void *loaderPrivate);
//};
//
//
//typedef struct __DRImutableRenderBufferLoaderExtensionRec __DRImutableRenderBufferLoaderExtension;
//struct __DRImutableRenderBufferLoaderExtensionRec {
//    __DRIextension base;
//    void (*displaySharedBuffer)(__DRIdrawable *drawable, int fence_fd,
//                                void *loaderPrivate);
//};
//
//
//struct __DRIscreenRec {
//    const struct __DriverAPIRec *driver;
//    int myNum;
//    int fd;
//    void *driverPrivate;
//    void *loaderPrivate;
//    int max_gl_core_version;
//    int max_gl_compat_version;
//    int max_gl_es1_version;
//    int max_gl_es2_version;
//    const __DRIextension **extensions;
//    const __DRIswrastLoaderExtension *swrast_loader;
//    struct {
//        const __DRIdri2LoaderExtension *loader;
//        const __DRIimageLookupExtension *image;
//        const __DRIuseInvalidateExtension *useInvalidate;
//        const __DRIbackgroundCallableExtension *backgroundCallable;
//    } dri2;
//    struct {
//        const __DRIimageLoaderExtension *loader;
//    } image;
//    struct {
//        const __DRImutableRenderBufferLoaderExtension *loader;
//    } mutableRenderBuffer;
//    driOptionCache optionInfo;
//    driOptionCache optionCache;
//    unsigned int api_mask;
//};
//
//
//struct __DriverAPIRec {
//    const __DRIconfig **(*InitScreen) (__DRIscreen * priv);
//
//    void (*DestroyScreen)(__DRIscreen *driScrnPriv);
//
//    GLboolean (*CreateContext)(gl_api api,
//                               const struct gl_config *glVis,
//                               __DRIcontext *driContextPriv,
//                               const struct __DriverContextConfig *ctx_config,
//                               unsigned *error,
//                               void *sharedContextPrivate);
//
//    void (*DestroyContext)(__DRIcontext *driContextPriv);
//
//    GLboolean (*CreateBuffer)(__DRIscreen *driScrnPriv,
//                              __DRIdrawable *driDrawPriv,
//                              const struct gl_config *glVis,
//                              GLboolean pixmapBuffer);
//
//    void (*DestroyBuffer)(__DRIdrawable *driDrawPriv);
//
//    void (*SwapBuffers)(__DRIdrawable *driDrawPriv);
//
//    GLboolean (*MakeCurrent)(__DRIcontext *driContextPriv,
//                             __DRIdrawable *driDrawPriv,
//                             __DRIdrawable *driReadPriv);
//
//    GLboolean (*UnbindContext)(__DRIcontext *driContextPriv);
//
//    __DRIbuffer *(*AllocateBuffer) (__DRIscreen *screenPrivate,
//                                    unsigned int attachment,
//                                    unsigned int format,
//                                    int width, int height);
//
//    void (*ReleaseBuffer) (__DRIscreen *screenPrivate, __DRIbuffer *buffer);
//
//    void (*CopySubBuffer)(__DRIdrawable *driDrawPriv, int x, int y,
//                          int w, int h);
//};
//
//struct __DRIswrastLoaderExtensionRec {
//    __DRIextension base;
//    void (*getDrawableInfo)(__DRIdrawable *drawable,
//                            int *x, int *y, int *width, int *height,
//                            void *loaderPrivate);
//    void (*putImage)(__DRIdrawable *drawable, int op,
//                     int x, int y, int width, int height,
//                     char *data, void *loaderPrivate);
//    void (*getImage)(__DRIdrawable *readable,
//                     int x, int y, int width, int height,
//                     char *data, void *loaderPrivate);
//    void (*putImage2)(__DRIdrawable *drawable, int op,
//                      int x, int y, int width, int height, int stride,
//                      char *data, void *loaderPrivate);
//    void (*getImage2)(__DRIdrawable *readable,
//                      int x, int y, int width, int height, int stride,
//                      char *data, void *loaderPrivate);
//    void (*putImageShm)(__DRIdrawable *drawable, int op,
//                        int x, int y, int width, int height, int stride,
//                        int shmid, char *shmaddr, unsigned offset,
//                        void *loaderPrivate);
//    void (*getImageShm)(__DRIdrawable *readable,
//                        int x, int y, int width, int height,
//                        int shmid, void *loaderPrivate);
//    void (*putImageShm2)(__DRIdrawable *drawable, int op,
//                         int x, int y,
//                         int width, int height, int stride,
//                         int shmid, char *shmaddr, unsigned offset,
//                         void *loaderPrivate);
//    unsigned char (*getImageShm2)(__DRIdrawable *readable,
//                                  int x, int y, int width, int height,
//                                  int shmid, void *loaderPrivate);
//};
//
//
//struct __DRIdri2LoaderExtensionRec {
//    __DRIextension base;
//    __DRIbuffer *(*getBuffers)(__DRIdrawable *driDrawable,
//                               int *width, int *height,
//                               unsigned int *attachments, int count,
//                               int *out_count, void *loaderPrivate);
//    void (*flushFrontBuffer)(__DRIdrawable *driDrawable, void *loaderPrivate);
//    __DRIbuffer *(*getBuffersWithFormat)(__DRIdrawable *driDrawable,
//                                         int *width, int *height,
//                                         unsigned int *attachments, int count,
//                                         int *out_count, void *loaderPrivate);
//    unsigned (*getCapability)(void *loaderPrivate, enum dri_loader_cap cap);
//    void (*destroyLoaderImageState)(void *loaderPrivate);
//};
//
//
//
//extern const struct __DriverAPIRec driDriverAPI;

#define STRING_CONF_MAXLEN 1024

#define unreachable(str) assert(!str)

#ifndef ARRAY_SIZE
#  define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

typedef struct driEnumDescription {
    int value;
    const char *desc;
} driEnumDescription;

typedef struct driOptionDescription {
    const char *desc;

    driOptionInfo info;
    driOptionValue value;
    driEnumDescription enums[4];
} driOptionDescription;

#define DRI_CONF_SECTION(text) { .desc = text, .info = { .type = DRI_SECTION } },
#define DRI_CONF_SECTION_END

#define DRI_CONF_VBLANK_NEVER 0
#define DRI_CONF_VBLANK_DEF_INTERVAL_0 1
#define DRI_CONF_VBLANK_DEF_INTERVAL_1 2
#define DRI_CONF_VBLANK_ALWAYS_SYNC 3

#define DRI_CONF_SECTION_DEBUG DRI_CONF_SECTION("Debugging")
#define DRI_CONF_SECTION_PERFORMANCE DRI_CONF_SECTION("Performance")

#define DRI_CONF_RANGE_I(min, max)              \
      .range = {                                \
         .start = { ._int = min },              \
         .end = { ._int = max },                \
      }                                         \

#define DRI_CONF_RANGE_F(min, max)              \
      .range = {                                \
         .start = { ._float = min },            \
         .end = { ._float = max },              \
      }

#define DRI_CONF_OPT_S_NODEF(_name, _desc) {                    \
      .desc = _desc,                                            \
      .info = {                                                 \
         .name = #_name,                                        \
         .type = DRI_STRING,                                    \
      },                                                        \
      .value = { ._string = "" },                               \
   },

#define DRI_CONF_ENUM(_value,text) { .value = _value, .desc = text },

#define DRI_CONF_OPT_E(_name, def, min, max, _desc, values) {   \
      .desc = _desc,                                            \
      .info = {                                                 \
         .name = #_name,                                        \
         .type = DRI_ENUM,                                      \
         DRI_CONF_RANGE_I(min, max),                            \
      },                                                        \
      .value = { ._int = def },                                 \
      .enums = { values },                                      \
   },

#define DRI_CONF_OPT_B(_name, def, _desc) {                     \
      .desc = _desc,                                            \
      .info = {                                                 \
         .name = #_name,                                        \
         .type = DRI_BOOL,                                      \
      },                                                        \
      .value = { ._bool = def },                                \
   },

#define DRI_CONF_SECTION_MISCELLANEOUS DRI_CONF_SECTION("Miscellaneous")

#define DRI_CONF_ALWAYS_HAVE_DEPTH_BUFFER(def) \
   DRI_CONF_OPT_B(always_have_depth_buffer, def, \
                  "Create all visuals with a depth buffer")

#define DRI_CONF_GLSL_ZERO_INIT(def) \
   DRI_CONF_OPT_B(glsl_zero_init, def, \
                  "Force uninitialized variables to default to zero")

#define DRI_CONF_VS_POSITION_ALWAYS_INVARIANT(def) \
   DRI_CONF_OPT_B(vs_position_always_invariant, def, \
                  "Force the vertex shader's gl_Position output to be considered 'invariant'")

#define DRI_CONF_VS_POSITION_ALWAYS_PRECISE(def) \
   DRI_CONF_OPT_B(vs_position_always_precise, def, \
                  "Force the vertex shader's gl_Position output to be considered 'precise'")

#define DRI_CONF_ALLOW_RGB10_CONFIGS(def) \
   DRI_CONF_OPT_B(allow_rgb10_configs, def, \
                  "Allow exposure of visuals and fbconfigs with rgb10a2 formats")

#define DRI_CONF_ALLOW_RGB565_CONFIGS(def) \
   DRI_CONF_OPT_B(allow_rgb565_configs, def, \
                  "Allow exposure of visuals and fbconfigs with rgb565 formats")

#define DRI_CONF_FORCE_INTEGER_TEX_NEAREST(def) \
   DRI_CONF_OPT_B(force_integer_tex_nearest, def, \
                  "Force integer textures to use nearest filtering")

#define DRI_CONF_GLX_EXTENSION_OVERRIDE() \
   DRI_CONF_OPT_S_NODEF(glx_extension_override, \
                  "Allow enabling/disabling a list of GLX extensions")

#define DRI_CONF_INDIRECT_GL_EXTENSION_OVERRIDE() \
   DRI_CONF_OPT_S_NODEF(indirect_gl_extension_override, \
                  "Allow enabling/disabling a list of indirect-GL extensions")

#define DRI_CONF_VBLANK_MODE(def) \
   DRI_CONF_OPT_E(vblank_mode, def, 0, 3, \
                  "Synchronization with vertical refresh (swap intervals)", \
                  DRI_CONF_ENUM(0,"Never synchronize with vertical refresh, ignore application's choice") \
                  DRI_CONF_ENUM(1,"Initial swap interval 0, obey application's choice") \
                  DRI_CONF_ENUM(2,"Initial swap interval 1, obey application's choice") \
                  DRI_CONF_ENUM(3,"Always synchronize with vertical refresh, application chooses the minimum swap interval"))

#define DRI_CONF_SECTION_QUALITY DRI_CONF_SECTION("Image Quality")

#define DRI_CONF_PRECISE_TRIG(def) \
   DRI_CONF_OPT_B(precise_trig, def, \
                  "Prefer accuracy over performance in trig functions")

#define DRI_CONF_PP_CELSHADE(def) \
   DRI_CONF_OPT_E(pp_celshade, def, 0, 1, \
                  "A post-processing filter to cel-shade the output", \
                  { 0 } )

#define DRI_CONF_PP_NORED(def) \
   DRI_CONF_OPT_E(pp_nored, def, 0, 1, \
                  "A post-processing filter to remove the red channel", \
                  { 0 } )

#define DRI_CONF_PP_NOGREEN(def) \
   DRI_CONF_OPT_E(pp_nogreen, def, 0, 1, \
                  "A post-processing filter to remove the green channel", \
                  { 0 } )

#define DRI_CONF_PP_NOBLUE(def) \
   DRI_CONF_OPT_E(pp_noblue, def, 0, 1, \
                  "A post-processing filter to remove the blue channel", \
                  { 0 } )

#define DRI_CONF_PP_JIMENEZMLAA(def,min,max) \
   DRI_CONF_OPT_I(pp_jimenezmlaa, def, min, max, \
                  "Morphological anti-aliasing based on Jimenez' MLAA. 0 to disable, 8 for default quality")

#define DRI_CONF_PP_JIMENEZMLAA_COLOR(def,min,max) \
   DRI_CONF_OPT_I(pp_jimenezmlaa_color, def, min, max, \
                  "Morphological anti-aliasing based on Jimenez' MLAA. 0 to disable, 8 for default quality. Color version, usable with 2d GL apps")

#define DRI_CONF_ALWAYS_FLUSH_BATCH(def) \
   DRI_CONF_OPT_B(always_flush_batch, def,                              \
                  "Enable flushing batchbuffer after each draw call")

#define DRI_CONF_ALWAYS_FLUSH_CACHE(def) \
   DRI_CONF_OPT_B(always_flush_cache, def, \
                  "Enable flushing GPU caches with each draw call")

#define DRI_CONF_DISABLE_THROTTLING(def) \
   DRI_CONF_OPT_B(disable_throttling, def, \
                  "Disable throttling on first batch after flush")

#define DRI_CONF_FORCE_GLSL_EXTENSIONS_WARN(def) \
   DRI_CONF_OPT_B(force_glsl_extensions_warn, def, \
                  "Force GLSL extension default behavior to 'warn'")

#define DRI_CONF_DISABLE_BLEND_FUNC_EXTENDED(def) \
   DRI_CONF_OPT_B(disable_blend_func_extended, def, \
                  "Disable dual source blending")

#define DRI_CONF_DISABLE_ARB_GPU_SHADER5(def) \
   DRI_CONF_OPT_B(disable_arb_gpu_shader5, def, \
                  "Disable GL_ARB_gpu_shader5")

#define DRI_CONF_DUAL_COLOR_BLEND_BY_LOCATION(def) \
   DRI_CONF_OPT_B(dual_color_blend_by_location, def, \
                  "Identify dual color blending sources by location rather than index")

#define DRI_CONF_DISABLE_GLSL_LINE_CONTINUATIONS(def) \
   DRI_CONF_OPT_B(disable_glsl_line_continuations, def, \
                  "Disable backslash-based line continuations in GLSL source")

#define DRI_CONF_FORCE_GLSL_VERSION(def) \
   DRI_CONF_OPT_I(force_glsl_version, def, 0, 999, \
                  "Force a default GLSL version for shaders that lack an explicit #version line")

#define DRI_CONF_ALLOW_EXTRA_PP_TOKENS(def) \
   DRI_CONF_OPT_B(allow_extra_pp_tokens, def, \
                  "Allow extra tokens at end of preprocessor directives.")

#define DRI_CONF_ALLOW_GLSL_EXTENSION_DIRECTIVE_MIDSHADER(def) \
   DRI_CONF_OPT_B(allow_glsl_extension_directive_midshader, def, \
                  "Allow GLSL #extension directives in the middle of shaders")

#define DRI_CONF_ALLOW_GLSL_120_SUBSET_IN_110(def) \
   DRI_CONF_OPT_B(allow_glsl_120_subset_in_110, def, \
                  "Allow a subset of GLSL 1.20 in GLSL 1.10 as needed by SPECviewperf13")

#define DRI_CONF_ALLOW_GLSL_BUILTIN_CONST_EXPRESSION(def) \
   DRI_CONF_OPT_B(allow_glsl_builtin_const_expression, def, \
                  "Allow builtins as part of constant expressions")

#define DRI_CONF_ALLOW_GLSL_RELAXED_ES(def) \
   DRI_CONF_OPT_B(allow_glsl_relaxed_es, def, \
                  "Allow some relaxation of GLSL ES shader restrictions")

#define DRI_CONF_ALLOW_GLSL_BUILTIN_VARIABLE_REDECLARATION(def) \
   DRI_CONF_OPT_B(allow_glsl_builtin_variable_redeclaration, def, \
                  "Allow GLSL built-in variables to be redeclared verbatim")

#define DRI_CONF_ALLOW_HIGHER_COMPAT_VERSION(def) \
   DRI_CONF_OPT_B(allow_higher_compat_version, def, \
                  "Allow a higher compat profile (version 3.1+) for apps that request it")

#define DRI_CONF_ALLOW_GLSL_COMPAT_SHADERS(def) \
   DRI_CONF_OPT_B(allow_glsl_compat_shaders, def, \
                  "Allow in GLSL: #version xxx compatibility")

#define DRI_CONF_FORCE_GLSL_ABS_SQRT(def) \
   DRI_CONF_OPT_B(force_glsl_abs_sqrt, def,                             \
                  "Force computing the absolute value for sqrt() and inversesqrt()")

#define DRI_CONF_GLSL_CORRECT_DERIVATIVES_AFTER_DISCARD(def) \
   DRI_CONF_OPT_B(glsl_correct_derivatives_after_discard, def, \
                  "Implicit and explicit derivatives after a discard behave as if the discard didn't happen")

#define DRI_CONF_GLSL_IGNORE_WRITE_TO_READONLY_VAR(def) \
   DRI_CONF_OPT_B(glsl_ignore_write_to_readonly_var, def, \
                  "Forces the GLSL compiler to ignore writes to readonly vars rather than throwing an error")

#define DRI_CONF_ALLOW_GLSL_CROSS_STAGE_INTERPOLATION_MISMATCH(def) \
   DRI_CONF_OPT_B(allow_glsl_cross_stage_interpolation_mismatch, def,   \
                  "Allow interpolation qualifier mismatch across shader stages")

#define DRI_CONF_DO_DCE_BEFORE_CLIP_CULL_ANALYSIS(def) \
   DRI_CONF_OPT_B(do_dce_before_clip_cull_analysis, def,   \
                  "Use dead code elimitation before checking for invalid Clip*/CullDistance variables usage.")

#define DRI_CONF_ALLOW_DRAW_OUT_OF_ORDER(def) \
   DRI_CONF_OPT_B(allow_draw_out_of_order, def, \
                  "Allow out-of-order draw optimizations. Set when Z fighting doesn't have to be accurate.")

#define DRI_CONF_GLTHREAD_NOP_CHECK_FRAMEBUFFER_STATUS(def) \
   DRI_CONF_OPT_B(glthread_nop_check_framebuffer_status, def, \
                  "glthread always returns GL_FRAMEBUFFER_COMPLETE to prevent synchronization.")

#define DRI_CONF_FORCE_GL_VENDOR() \
   DRI_CONF_OPT_S_NODEF(force_gl_vendor, "Override GPU vendor string.")

#define DRI_CONF_FORCE_GL_RENDERER() \
   DRI_CONF_OPT_S_NODEF(force_gl_renderer, "Override GPU renderer string.")

#define DRI_CONF_FORCE_COMPAT_PROFILE(def) \
   DRI_CONF_OPT_B(force_compat_profile, def, \
                  "Force an OpenGL compatibility context")

#define DRI_CONF_FORCE_COMPAT_SHADERS(def) \
   DRI_CONF_OPT_B(force_compat_shaders, def, \
                  "Force OpenGL compatibility shaders")

#define DRI_CONF_FORCE_DIRECT_GLX_CONTEXT(def) \
   DRI_CONF_OPT_B(force_direct_glx_context, def, \
                  "Force direct GLX context (even if indirect is requested)")

#define DRI_CONF_ALLOW_INVALID_GLX_DESTROY_WINDOW(def) \
   DRI_CONF_OPT_B(allow_invalid_glx_destroy_window, def, \
                  "Allow passing an invalid window into glXDestroyWindow")

#define DRI_CONF_OVERRIDE_VRAM_SIZE() \
   DRI_CONF_OPT_I(override_vram_size, -1, -1, 2147483647, \
                  "Override the VRAM size advertised to the application in MiB (-1 = default)")

#define DRI_CONF_FORCE_GL_NAMES_REUSE(def) \
   DRI_CONF_OPT_B(force_gl_names_reuse, def, "Force GL names reuse")

#define DRI_CONF_TRANSCODE_ETC(def) \
   DRI_CONF_OPT_B(transcode_etc, def, "Transcode ETC formats to DXTC if unsupported")

#define DRI_CONF_TRANSCODE_ASTC(def) \
   DRI_CONF_OPT_B(transcode_astc, def, "Transcode ASTC formats to DXTC if unsupported")

#define DRI_CONF_MESA_EXTENSION_OVERRIDE() \
   DRI_CONF_OPT_S_NODEF(mesa_extension_override, \
                  "Allow enabling/disabling a list of extensions")

#define DRI_CONF_GLX_EXTENSION_OVERRIDE() \
   DRI_CONF_OPT_S_NODEF(glx_extension_override, \
                  "Allow enabling/disabling a list of GLX extensions")

#define DRI_CONF_INDIRECT_GL_EXTENSION_OVERRIDE() \
   DRI_CONF_OPT_S_NODEF(indirect_gl_extension_override, \
                  "Allow enabling/disabling a list of indirect-GL extensions")

#define DRI_CONF_DISABLE_PROTECTED_CONTENT_CHECK(def) \
   DRI_CONF_OPT_B(disable_protected_content_check, def, \
                  "Don't reject image import if protected_content attribute doesn't match")

#define DRI_CONF_IGNORE_MAP_UNSYNCHRONIZED(def) \
   DRI_CONF_OPT_B(ignore_map_unsynchronized, def, \
                  "Ignore GL_MAP_UNSYNCHRONIZED_BIT, workaround for games that use it incorrectly")

#define DRI_CONF_VK_DONT_CARE_AS_LOAD(def) \
   DRI_CONF_OPT_B(vk_dont_care_as_load, def, \
                  "Treat VK_ATTACHMENT_LOAD_OP_DONT_CARE as LOAD_OP_LOAD, workaround on tiler GPUs for games that confuse these two load ops")


struct gl_precision
{
    GLushort RangeMin;   /**< min value exponent */
    GLushort RangeMax;   /**< max value exponent */
    GLushort Precision;  /**< number of mantissa bits */
};

#ifndef STATIC_ASSERT
#define STATIC_ASSERT(COND) do { } while (0)
#endif

enum {
    thrd_success = 0, // succeeded
    thrd_timeout,     // timeout
    thrd_error,       // failed
    thrd_busy,        // resource busy
    thrd_nomem        // out of memory
};

typedef pthread_mutex_t mtx_t;
typedef mtx_t simple_mtx_t;

#define _MTX_INITIALIZER_NP PTHREAD_MUTEX_INITIALIZER
#define _SIMPLE_MTX_INITIALIZER_NP _MTX_INITIALIZER_NP

static simple_mtx_t override_lock = _SIMPLE_MTX_INITIALIZER_NP;

typedef unsigned short GLenum16;
typedef unsigned char GLbitfield8;
typedef unsigned short GLbitfield16;
typedef GLuint64 GLbitfield64;

struct gl_program_constants
{
    /* logical limits */
    GLuint MaxInstructions;
    GLuint MaxAluInstructions;
    GLuint MaxTexInstructions;
    GLuint MaxTexIndirections;
    GLuint MaxAttribs;
    GLuint MaxTemps;
    GLuint MaxAddressRegs;
    GLuint MaxAddressOffset;  /**< [-MaxAddressOffset, MaxAddressOffset-1] */
    GLuint MaxParameters;
    GLuint MaxLocalParams;
    GLuint MaxEnvParams;
    /* native/hardware limits */
    GLuint MaxNativeInstructions;
    GLuint MaxNativeAluInstructions;
    GLuint MaxNativeTexInstructions;
    GLuint MaxNativeTexIndirections;
    GLuint MaxNativeAttribs;
    GLuint MaxNativeTemps;
    GLuint MaxNativeAddressRegs;
    GLuint MaxNativeParameters;
    /* For shaders */
    GLuint MaxUniformComponents;  /**< Usually == MaxParameters * 4 */

    /**
     * \name Per-stage input / output limits
     *
     * Previous to OpenGL 3.2, the intrastage data limits were advertised with
     * a single value: GL_MAX_VARYING_COMPONENTS (GL_MAX_VARYING_VECTORS in
     * ES).  This is stored as \c gl_constants::MaxVarying.
     *
     * Starting with OpenGL 3.2, the limits are advertised with per-stage
     * variables.  Each stage as a certain number of outputs that it can feed
     * to the next stage and a certain number inputs that it can consume from
     * the previous stage.
     *
     * Vertex shader inputs do not participate this in this accounting.
     * These are tracked exclusively by \c gl_program_constants::MaxAttribs.
     *
     * Fragment shader outputs do not participate this in this accounting.
     * These are tracked exclusively by \c gl_constants::MaxDrawBuffers.
     */
    /*@{*/
    GLuint MaxInputComponents;
    GLuint MaxOutputComponents;
    /*@}*/

    /* ES 2.0 and GL_ARB_ES2_compatibility */
    struct gl_precision LowFloat, MediumFloat, HighFloat;
    struct gl_precision LowInt, MediumInt, HighInt;
    /* GL_ARB_uniform_buffer_object */
    GLuint MaxUniformBlocks;
    uint64_t MaxCombinedUniformComponents;
    GLuint MaxTextureImageUnits;

    /* GL_ARB_shader_atomic_counters */
    GLuint MaxAtomicBuffers;
    GLuint MaxAtomicCounters;

    /* GL_ARB_shader_image_load_store */
    GLuint MaxImageUniforms;

    /* GL_ARB_shader_storage_buffer_object */
    GLuint MaxShaderStorageBlocks;
};

enum {
    MESA_FORMAT_SWIZZLE_X = 0,
    MESA_FORMAT_SWIZZLE_Y = 1,
    MESA_FORMAT_SWIZZLE_Z = 2,
    MESA_FORMAT_SWIZZLE_W = 3,
    MESA_FORMAT_SWIZZLE_ZERO = 4,
    MESA_FORMAT_SWIZZLE_ONE = 5,
    MESA_FORMAT_SWIZZLE_NONE = 6,
};

typedef enum
{
    MESA_SHADER_NONE = -1,
    MESA_SHADER_VERTEX = 0,
    MESA_SHADER_TESS_CTRL = 1,
    MESA_SHADER_TESS_EVAL = 2,
    MESA_SHADER_GEOMETRY = 3,
    MESA_SHADER_FRAGMENT = 4,
    MESA_SHADER_COMPUTE = 5,

    /* Vulkan-only stages. */
    MESA_SHADER_TASK         = 6,
    MESA_SHADER_MESH         = 7,
    MESA_SHADER_RAYGEN       = 8,
    MESA_SHADER_ANY_HIT      = 9,
    MESA_SHADER_CLOSEST_HIT  = 10,
    MESA_SHADER_MISS         = 11,
    MESA_SHADER_INTERSECTION = 12,
    MESA_SHADER_CALLABLE     = 13,

    /* must be last so it doesn't affect the GL pipeline */
    MESA_SHADER_KERNEL = 14,
} gl_shader_stage;

#define MESA_SHADER_STAGES (MESA_SHADER_COMPUTE + 1)

struct gl_shader_compiler_options
{
    /** Driver-selectable options: */
    GLboolean EmitNoLoops;
    GLboolean EmitNoCont;                  /**< Emit CONT opcode? */
    GLboolean EmitNoMainReturn;            /**< Emit CONT/RET opcodes? */
    GLboolean EmitNoPow;                   /**< Emit POW opcodes? */
    GLboolean EmitNoSat;                   /**< Emit SAT opcodes? */
    GLboolean LowerCombinedClipCullDistance; /** Lower gl_ClipDistance and
                                              * gl_CullDistance together from
                                              * float[8] to vec4[2]
                                              **/
    GLbitfield LowerBuiltinVariablesXfb;   /**< Which builtin variables should
                                           * be lowered for transform feedback
                                           **/

    /**
     * If we can lower the precision of variables based on precision
     * qualifiers
     */
    GLboolean LowerPrecisionFloat16;
    GLboolean LowerPrecisionInt16;
    GLboolean LowerPrecisionDerivatives;
    GLboolean LowerPrecisionFloat16Uniforms;

    /**
     * This enables lowering of 16b constants.  Some drivers may not
     * to lower constants to 16b (ie. if the hw can do automatic
     * narrowing on constant load)
     */
    GLboolean LowerPrecisionConstants;

    /**
     * \name Forms of indirect addressing the driver cannot do.
     */
    /*@{*/
    GLboolean EmitNoIndirectInput;   /**< No indirect addressing of inputs */
    GLboolean EmitNoIndirectOutput;  /**< No indirect addressing of outputs */
    GLboolean EmitNoIndirectTemp;    /**< No indirect addressing of temps */
    GLboolean EmitNoIndirectUniform; /**< No indirect addressing of constants */
    GLboolean EmitNoIndirectSampler; /**< No indirect addressing of samplers */
    /*@}*/

    GLuint MaxIfDepth;               /**< Maximum nested IF blocks */
    GLuint MaxUnrollIterations;

    /**
     * Optimize code for array of structures backends.
     *
     * This is a proxy for:
     *   - preferring DP4 instructions (rather than MUL/MAD) for
     *     matrix * vector operations, such as position transformation.
     */
    GLboolean OptimizeForAOS;

    /** Lower UBO and SSBO access to intrinsics. */
    GLboolean LowerBufferInterfaceBlocks;

    /** Clamp UBO and SSBO block indices so they don't go out-of-bounds. */
    GLboolean ClampBlockIndicesToArrayBounds;

    /** (driconf) Force gl_Position to be considered invariant */
    GLboolean PositionAlwaysInvariant;

    const struct nir_shader_compiler_options *NirOptions;
};

enum SpvExtension {
    SPV_KHR_16bit_storage = 0,
    SPV_KHR_device_group,
    SPV_KHR_multiview,
    SPV_KHR_shader_ballot,
    SPV_KHR_shader_draw_parameters,
    SPV_KHR_storage_buffer_storage_class,
    SPV_KHR_subgroup_vote,
    SPV_KHR_variable_pointers,
    SPV_AMD_gcn_shader,
    SPV_EXTENSIONS_COUNT
};

struct spirv_supported_extensions {
    /** Flags the supported extensions. Array to make it easier to iterate. */
    bool supported[SPV_EXTENSIONS_COUNT];

    /** Number of supported extensions */
    unsigned int count;
};

struct spirv_supported_capabilities {
    bool address;
    bool atomic_storage;
    bool demote_to_helper_invocation;
    bool derivative_group;
    bool descriptor_array_dynamic_indexing;
    bool descriptor_array_non_uniform_indexing;
    bool descriptor_indexing;
    bool device_group;
    bool draw_parameters;
    bool float16_atomic_min_max;
    bool float32_atomic_add;
    bool float32_atomic_min_max;
    bool float64;
    bool float64_atomic_add;
    bool float64_atomic_min_max;
    bool fragment_shader_sample_interlock;
    bool fragment_shader_pixel_interlock;
    bool fragment_shading_rate;
    bool generic_pointers;
    bool geometry_streams;
    bool image_ms_array;
    bool image_read_without_format;
    bool image_write_without_format;
    bool image_atomic_int64;
    bool int8;
    bool int16;
    bool int64;
    bool int64_atomics;
    bool integer_functions2;
    bool kernel;
    bool kernel_image;
    bool kernel_image_read_write;
    bool literal_sampler;
    bool min_lod;
    bool multiview;
    bool physical_storage_buffer_address;
    bool post_depth_coverage;
    bool printf;
    bool ray_tracing;
    bool ray_query;
    bool ray_traversal_primitive_culling;
    bool runtime_descriptor_array;
    bool float_controls;
    bool shader_clock;
    bool shader_viewport_index_layer;
    bool sparse_residency;
    bool stencil_export;
    bool storage_8bit;
    bool storage_16bit;
    bool storage_image_ms;
    bool subgroup_arithmetic;
    bool subgroup_ballot;
    bool subgroup_basic;
    bool subgroup_quad;
    bool subgroup_shuffle;
    bool subgroup_vote;
    bool tessellation;
    bool transform_feedback;
    bool variable_pointers;
    bool vk_memory_model;
    bool vk_memory_model_device_scope;
    bool workgroup_memory_explicit_layout;
    bool float16;
    bool amd_fragment_mask;
    bool amd_gcn_shader;
    bool amd_shader_ballot;
    bool amd_trinary_minmax;
    bool amd_image_read_write_lod;
    bool amd_shader_explicit_vertex_parameter;
    bool amd_image_gather_bias_lod;

    bool intel_subgroup_shuffle;
    bool intel_subgroup_buffer_block_io;
};

struct gl_constants
{
    /**
     * Bitmask of valid primitive types supported by the driver,
     */
    GLbitfield DriverSupportedPrimMask;

    GLuint MaxTextureMbytes;      /**< Max memory per image, in MB */
    GLuint MaxTextureSize;        /**< Max 1D/2D texture size, in pixels*/
    GLuint Max3DTextureLevels;    /**< Max mipmap levels for 3D textures */
    GLuint MaxCubeTextureLevels;  /**< Max mipmap levels for cube textures */
    GLuint MaxArrayTextureLayers; /**< Max layers in array textures */
    GLuint MaxTextureRectSize;    /**< Max rectangle texture size, in pixes */
    GLuint MaxTextureCoordUnits;
    GLuint MaxCombinedTextureImageUnits;
    GLuint MaxTextureUnits; /**< = MIN(CoordUnits, FragmentProgram.ImageUnits) */
    GLfloat MaxTextureMaxAnisotropy;  /**< GL_EXT_texture_filter_anisotropic */
    GLfloat MaxTextureLodBias;        /**< GL_EXT_texture_lod_bias */
    GLuint MaxTextureBufferSize;      /**< GL_ARB_texture_buffer_object */

    GLuint TextureBufferOffsetAlignment; /**< GL_ARB_texture_buffer_range */

    GLuint MaxArrayLockSize;

    GLint SubPixelBits;

    GLfloat MinPointSize, MaxPointSize;	     /**< aliased */
    GLfloat MinPointSizeAA, MaxPointSizeAA;   /**< antialiased */
    GLfloat PointSizeGranularity;
    GLfloat MinLineWidth, MaxLineWidth;       /**< aliased */
    GLfloat MinLineWidthAA, MaxLineWidthAA;   /**< antialiased */
    GLfloat LineWidthGranularity;

    GLuint MaxClipPlanes;
    GLuint MaxLights;
    GLfloat MaxShininess;                     /**< GL_NV_light_max_exponent */
    GLfloat MaxSpotExponent;                  /**< GL_NV_light_max_exponent */

    GLuint MaxViewportWidth, MaxViewportHeight;
    GLuint MaxViewports;                      /**< GL_ARB_viewport_array */
    GLuint ViewportSubpixelBits;              /**< GL_ARB_viewport_array */
    struct {
        GLfloat Min;
        GLfloat Max;
    } ViewportBounds;                         /**< GL_ARB_viewport_array */
    GLuint MaxWindowRectangles;               /**< GL_EXT_window_rectangles */

    struct gl_program_constants Program[MESA_SHADER_STAGES];
    GLuint MaxProgramMatrices;
    GLuint MaxProgramMatrixStackDepth;

    struct {
        GLuint SamplesPassed;
        GLuint TimeElapsed;
        GLuint Timestamp;
        GLuint PrimitivesGenerated;
        GLuint PrimitivesWritten;
        GLuint VerticesSubmitted;
        GLuint PrimitivesSubmitted;
        GLuint VsInvocations;
        GLuint TessPatches;
        GLuint TessInvocations;
        GLuint GsInvocations;
        GLuint GsPrimitives;
        GLuint FsInvocations;
        GLuint ComputeInvocations;
        GLuint ClInPrimitives;
        GLuint ClOutPrimitives;
    } QueryCounterBits;

    GLuint MaxDrawBuffers;    /**< GL_ARB_draw_buffers */

    GLuint MaxColorAttachments;   /**< GL_EXT_framebuffer_object */
    GLuint MaxRenderbufferSize;   /**< GL_EXT_framebuffer_object */
    GLuint MaxSamples;            /**< GL_ARB_framebuffer_object */

    /**
     * GL_ARB_framebuffer_no_attachments
     */
    GLuint MaxFramebufferWidth;
    GLuint MaxFramebufferHeight;
    GLuint MaxFramebufferLayers;
    GLuint MaxFramebufferSamples;

    /** Number of varying vectors between any two shader stages. */
    GLuint MaxVarying;

    /** @{
     * GL_ARB_uniform_buffer_object
     */
    GLuint MaxCombinedUniformBlocks;
    GLuint MaxUniformBufferBindings;
    GLuint MaxUniformBlockSize;
    GLuint UniformBufferOffsetAlignment;
    /** @} */

    /** @{
     * GL_ARB_shader_storage_buffer_object
     */
    GLuint MaxCombinedShaderStorageBlocks;
    GLuint MaxShaderStorageBufferBindings;
    GLuint MaxShaderStorageBlockSize;
    GLuint ShaderStorageBufferOffsetAlignment;
    /** @} */

    /**
     * GL_ARB_explicit_uniform_location
     */
    GLuint MaxUserAssignableUniformLocations;

    /** geometry shader */
    GLuint MaxGeometryOutputVertices;
    GLuint MaxGeometryTotalOutputComponents;
    GLuint MaxGeometryShaderInvocations;

    GLuint GLSLVersion;  /**< Desktop GLSL version supported (ex: 120 = 1.20) */
    GLuint GLSLVersionCompat;  /**< Desktop compat GLSL version supported  */

    /**
     * Changes default GLSL extension behavior from "error" to "warn".  It's out
     * of spec, but it can make some apps work that otherwise wouldn't.
     */
    GLboolean ForceGLSLExtensionsWarn;

    /**
     * Force all shaders to behave as if they were declared with the
     * compatibility token.
     */
    GLboolean ForceCompatShaders;

    /**
     * If non-zero, forces GLSL shaders to behave as if they began
     * with "#version ForceGLSLVersion".
     */
    GLuint ForceGLSLVersion;

    /**
     * Allow GLSL #extension directives in the middle of shaders.
     */
    GLboolean AllowGLSLExtensionDirectiveMidShader;

    /**
     * Allow a subset of GLSL 1.20 in GLSL 1.10 as needed by SPECviewperf13.
     */
    GLboolean AllowGLSL120SubsetIn110;

    /**
     * Allow builtins as part of constant expressions. This was not allowed
     * until GLSL 1.20 this allows it everywhere.
     */
    GLboolean AllowGLSLBuiltinConstantExpression;

    /**
     * Allow some relaxation of GLSL ES shader restrictions. This encompasses
     * a number of relaxations to the ES shader rules.
     */
    GLboolean AllowGLSLRelaxedES;

    /**
     * Allow GLSL built-in variables to be redeclared verbatim
     */
    GLboolean AllowGLSLBuiltinVariableRedeclaration;

    /**
     * Allow GLSL interpolation qualifier mismatch across shader stages.
     */
    GLboolean AllowGLSLCrossStageInterpolationMismatch;

    /**
     * Allow creating a higher compat profile (version 3.1+) for apps that
     * request it. Be careful when adding that driconf option because some
     * features are unimplemented and might not work correctly.
     */
    GLboolean AllowHigherCompatVersion;

    /**
     * Allow GLSL shaders with the compatibility version directive
     * in non-compatibility profiles. (for shader-db)
     */
    GLboolean AllowGLSLCompatShaders;

    /**
     * Allow extra tokens at end of preprocessor directives. The CTS now tests
     * to make sure these are not allowed. However, previously drivers would
     * allow them to exist and just issue a warning so some old applications
     * depend on this.
     */
    GLboolean AllowExtraPPTokens;

    /**
     * The spec forbids a shader to "statically write both gl_ClipVertex
     * and gl_ClipDistance".
     * This option adds a tolerance for shader that statically writes to
     * both but at least one of the write can be removed by a dead code
     * elimination pass.
     */
    GLboolean DoDCEBeforeClipCullAnalysis;

    /**
     * Force computing the absolute value for sqrt() and inversesqrt() to follow
     * D3D9 when apps rely on this behaviour.
     */
    GLboolean ForceGLSLAbsSqrt;

    /**
     * Forces the GLSL compiler to ignore writes to readonly vars rather than
     * throwing an error.
     */
    GLboolean GLSLIgnoreWriteToReadonlyVar;

    /**
     * Types of variable to default initialized to zero. Supported values are:
     *   - 0: no zero initialization
     *   - 1: all shader variables and gl_FragColor are initialiazed to 0
     *   - 2: same as 1, but shader out variables are *not* initialized, while
     *        function out variables are now initialized.
     */
    GLchar GLSLZeroInit;

    /**
     * Force GL names reuse. Needed by SPECviewperf13.
     */
    GLboolean ForceGLNamesReuse;

    /**
     * Treat integer textures using GL_LINEAR filters as GL_NEAREST.
     */
    GLboolean ForceIntegerTexNearest;

    /**
     * Does the driver support real 32-bit integers?  (Otherwise, integers are
     * simulated via floats.)
     */
    GLboolean NativeIntegers;

    /**
     * Does VertexID count from zero or from base vertex?
     *
     * \note
     * If desktop GLSL 1.30 or GLSL ES 3.00 are not supported, this field is
     * ignored and need not be set.
     */
    bool VertexID_is_zero_based;

    /**
     * If the driver supports real 32-bit integers, what integer value should be
     * used for boolean true in uniform uploads?  (Usually 1 or ~0.)
     */
    GLuint UniformBooleanTrue;

    /**
     * Maximum amount of time, measured in nanseconds, that the server can wait.
     */
    GLuint64 MaxServerWaitTimeout;

    /** GL_EXT_provoking_vertex */
    GLboolean QuadsFollowProvokingVertexConvention;

    /** GL_ARB_viewport_array */
    GLenum16 LayerAndVPIndexProvokingVertex;

    /** OpenGL version 3.0 */
    GLbitfield ContextFlags;  /**< Ex: GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT */

    /** OpenGL version 3.2 */
    GLbitfield ProfileMask;   /**< Mask of CONTEXT_x_PROFILE_BIT */

    /** OpenGL version 4.4 */
    GLuint MaxVertexAttribStride;

    /** GL_EXT_transform_feedback */
    GLuint MaxTransformFeedbackBuffers;
    GLuint MaxTransformFeedbackSeparateComponents;
    GLuint MaxTransformFeedbackInterleavedComponents;
    GLuint MaxVertexStreams;

    /** GL_EXT_gpu_shader4 */
    GLint MinProgramTexelOffset, MaxProgramTexelOffset;

    /** GL_ARB_texture_gather */
    GLuint MinProgramTextureGatherOffset;
    GLuint MaxProgramTextureGatherOffset;
    GLuint MaxProgramTextureGatherComponents;

    /* GL_ARB_robustness */
    GLenum16 ResetStrategy;

    /* GL_KHR_robustness */
    GLboolean RobustAccess;

    /* GL_ARB_blend_func_extended */
    GLuint MaxDualSourceDrawBuffers;

    /**
     * For drivers which can do a better job at eliminating unused uniforms
     * than the GLSL compiler.
     *
     * XXX Remove these as soon as a better solution is available.
     */
    GLboolean GLSLSkipStrictMaxUniformLimitCheck;

    /**
     * Whether gl_FragCoord, gl_PointCoord and gl_FrontFacing
     * are system values.
     **/
    bool GLSLFragCoordIsSysVal;
    bool GLSLPointCoordIsSysVal;
    bool GLSLFrontFacingIsSysVal;

    /**
     * Run the minimum amount of GLSL optimizations to be able to link
     * shaders optimally (eliminate dead varyings and uniforms) and just do
     * all the necessary lowering.
     */
    bool GLSLOptimizeConservatively;

    /**
     * Whether to call lower_const_arrays_to_uniforms() during linking.
     */
    bool GLSLLowerConstArrays;

    /**
     * True if gl_TessLevelInner/Outer[] in the TES should be inputs
     * (otherwise, they're system values).
     */
    bool GLSLTessLevelsAsInputs;

    /** GL_ARB_map_buffer_alignment */
    GLuint MinMapBufferAlignment;

    /**
     * Disable varying packing.  This is out of spec, but potentially useful
     * for older platforms that supports a limited number of texture
     * indirections--on these platforms, unpacking the varyings in the fragment
     * shader increases the number of texture indirections by 1, which might
     * make some shaders not executable at all.
     *
     * Drivers that support transform feedback must set this value to GL_FALSE.
     */
    GLboolean DisableVaryingPacking;

    /**
     * Disable varying packing if used for transform feedback.  This is needed
     * for some drivers (e.g. Panfrost) where transform feedback requires
     * unpacked varyings.
     *
     * This variable is mutually exlusive with DisableVaryingPacking.
     */
    GLboolean DisableTransformFeedbackPacking;

    /**
     * Align varyings to POT in a slot
     *
     * Drivers that prefer varyings to be aligned to POT must set this value to GL_TRUE
     */
    GLboolean PreferPOTAlignedVaryings;


    /**
     * UBOs and SSBOs can be packed tightly by the OpenGL implementation when
     * layout is set as shared (the default) or packed. However most Mesa drivers
     * just use STD140 for these layouts. This flag allows drivers to use STD430
     * for packed and shared layouts which allows arrays to be packed more
     * tightly.
     */
    bool UseSTD430AsDefaultPacking;

    /**
     * Should meaningful names be generated for compiler temporary variables?
     *
     * Generally, it is not useful to have the compiler generate "meaningful"
     * names for temporary variables that it creates.  This can, however, be a
     * useful debugging aid.  In Mesa debug builds or release builds when
     * MESA_GLSL is set at run-time, meaningful names will be generated.
     * Drivers can also force names to be generated by setting this field.
     * For example, the i965 driver may set it when INTEL_DEBUG=vs (to dump
     * vertex shader assembly) is set at run-time.
     */
    bool GenerateTemporaryNames;

    /*
     * Maximum value supported for an index in DrawElements and friends.
     *
     * This must be at least (1ull<<24)-1.  The default value is
     * (1ull<<32)-1.
     *
     * \since ES 3.0 or GL_ARB_ES3_compatibility
     * \sa _mesa_init_constants
     */
    GLuint64 MaxElementIndex;

    /**
     * Disable interpretation of line continuations (lines ending with a
     * backslash character ('\') in GLSL source.
     */
    GLboolean DisableGLSLLineContinuations;

    /** GL_ARB_texture_multisample */
    GLint MaxColorTextureSamples;
    GLint MaxDepthTextureSamples;
    GLint MaxIntegerSamples;

    /** GL_AMD_framebuffer_multisample_advanced */
    GLint MaxColorFramebufferSamples;
    GLint MaxColorFramebufferStorageSamples;
    GLint MaxDepthStencilFramebufferSamples;

    /* An array of supported MSAA modes allowing different sample
     * counts per attachment type.
     */
    struct {
        GLint NumColorSamples;
        GLint NumColorStorageSamples;
        GLint NumDepthStencilSamples;
    } SupportedMultisampleModes[40];
    GLint NumSupportedMultisampleModes;

    /** GL_ARB_shader_atomic_counters */
    GLuint MaxAtomicBufferBindings;
    GLuint MaxAtomicBufferSize;
    GLuint MaxCombinedAtomicBuffers;
    GLuint MaxCombinedAtomicCounters;

    /** GL_ARB_vertex_attrib_binding */
    GLint MaxVertexAttribRelativeOffset;
    GLint MaxVertexAttribBindings;

    /* GL_ARB_shader_image_load_store */
    GLuint MaxImageUnits;
    GLuint MaxCombinedShaderOutputResources;
    GLuint MaxImageSamples;
    GLuint MaxCombinedImageUniforms;

    /** GL_ARB_compute_shader */
    GLuint MaxComputeWorkGroupCount[3]; /* Array of x, y, z dimensions */
    GLuint MaxComputeWorkGroupSize[3]; /* Array of x, y, z dimensions */
    GLuint MaxComputeWorkGroupInvocations;
    GLuint MaxComputeSharedMemorySize;

    /** GL_ARB_compute_variable_group_size */
    GLuint MaxComputeVariableGroupSize[3]; /* Array of x, y, z dimensions */
    GLuint MaxComputeVariableGroupInvocations;

    /** GL_ARB_gpu_shader5 */
    GLfloat MinFragmentInterpolationOffset;
    GLfloat MaxFragmentInterpolationOffset;

    GLboolean FakeSWMSAA;

    /** GL_KHR_context_flush_control */
    GLenum16 ContextReleaseBehavior;

    struct gl_shader_compiler_options ShaderCompilerOptions[MESA_SHADER_STAGES];

    /** GL_ARB_tessellation_shader */
    GLuint MaxPatchVertices;
    GLuint MaxTessGenLevel;
    GLuint MaxTessPatchComponents;
    GLuint MaxTessControlTotalOutputComponents;
    bool LowerTessLevel; /**< Lower gl_TessLevel* from float[n] to vecn? */
    bool PrimitiveRestartForPatches;
    bool LowerCsDerivedVariables;    /**< Lower gl_GlobalInvocationID and
                                     *   gl_LocalInvocationIndex based on
                                     *   other builtin variables. */

    /** GL_OES_primitive_bounding_box */
    bool NoPrimitiveBoundingBoxOutput;

    /** GL_ARB_sparse_buffer */
    GLuint SparseBufferPageSize;

    /** Used as an input for sha1 generation in the on-disk shader cache */
    unsigned char *dri_config_options_sha1;

    /** When drivers are OK with mapped buffers during draw and other calls. */
    bool AllowMappedBuffersDuringExecution;

    /**
     * Whether buffer creation, unsynchronized mapping, unmapping, and
     * deletion is thread-safe.
     */
    bool BufferCreateMapUnsynchronizedThreadSafe;

    /** GL_ARB_get_program_binary */
    GLuint NumProgramBinaryFormats;

    /** GL_NV_conservative_raster */
    GLuint MaxSubpixelPrecisionBiasBits;

    /** GL_NV_conservative_raster_dilate */
    GLfloat ConservativeRasterDilateRange[2];
    GLfloat ConservativeRasterDilateGranularity;

    /** Is the drivers uniform storage packed or padded to 16 bytes. */
    bool PackedDriverUniformStorage;

    /** Does the driver make use of the NIR based GLSL linker */
    bool UseNIRGLSLLinker;

    /** Wether or not glBitmap uses red textures rather than alpha */
    bool BitmapUsesRed;

    /** Whether the vertex buffer offset is a signed 32-bit integer. */
    bool VertexBufferOffsetIsInt32;

    /** Whether out-of-order draw (Begin/End) optimizations are allowed. */
    bool AllowDrawOutOfOrder;

    /** Whether to allow the fast path for frequently updated VAOs. */
    bool AllowDynamicVAOFastPath;

    /** Whether the driver can support primitive restart with a fixed index.
     * This is essentially a subset of NV_primitive_restart with enough support
     * to be able to enable GLES 3.1. Some hardware can support this but not the
     * full NV extension with arbitrary restart indices.
     */
    bool PrimitiveRestartFixedIndex;

    /** GL_ARB_gl_spirv */
    struct spirv_supported_capabilities SpirVCapabilities;

    /** GL_ARB_spirv_extensions */
    struct spirv_supported_extensions *SpirVExtensions;

    char *VendorOverride;
    char *RendererOverride;

    /** Buffer size used to upload vertices from glBegin/glEnd. */
    unsigned glBeginEndBufferSize;

    /** Whether the driver doesn't want x/y/width/height clipped based on src size
     *  when doing a copy texture operation (eg: may want out-of-bounds reads that
     *  produce 0 instead of leaving the texture content undefined).
     */
    bool NoClippingOnCopyTex;

    /**
     * Force glthread to always return GL_FRAMEBUFFER_COMPLETE to prevent
     * synchronization. Used for apps that call it every frame or multiple times
     * a frame, but always getting framebuffer completeness.
     */
    bool GLThreadNopCheckFramebufferStatus;

    /** GL_ARB_sparse_texture */
    GLuint MaxSparseTextureSize;
    GLuint MaxSparse3DTextureSize;
    GLuint MaxSparseArrayTextureLayers;
    bool SparseTextureFullArrayCubeMipmaps;
};


#define __ATTRIB(attrib, field) case attrib: *value = config->modes.field; break

struct gl_extensions
{
    GLboolean dummy;  /* don't remove this! */
    GLboolean dummy_true;  /* Set true by _mesa_init_extensions(). */
    GLboolean ANGLE_texture_compression_dxt;
    GLboolean ARB_ES2_compatibility;
    GLboolean ARB_ES3_compatibility;
    GLboolean ARB_ES3_1_compatibility;
    GLboolean ARB_ES3_2_compatibility;
    GLboolean ARB_arrays_of_arrays;
    GLboolean ARB_base_instance;
    GLboolean ARB_bindless_texture;
    GLboolean ARB_blend_func_extended;
    GLboolean ARB_buffer_storage;
    GLboolean ARB_clear_texture;
    GLboolean ARB_clip_control;
    GLboolean ARB_color_buffer_float;
    GLboolean ARB_compatibility;
    GLboolean ARB_compute_shader;
    GLboolean ARB_compute_variable_group_size;
    GLboolean ARB_conditional_render_inverted;
    GLboolean ARB_conservative_depth;
    GLboolean ARB_copy_image;
    GLboolean ARB_cull_distance;
    GLboolean EXT_color_buffer_half_float;
    GLboolean ARB_depth_buffer_float;
    GLboolean ARB_depth_clamp;
    GLboolean ARB_derivative_control;
    GLboolean ARB_draw_buffers_blend;
    GLboolean ARB_draw_elements_base_vertex;
    GLboolean ARB_draw_indirect;
    GLboolean ARB_draw_instanced;
    GLboolean ARB_fragment_coord_conventions;
    GLboolean ARB_fragment_layer_viewport;
    GLboolean ARB_fragment_program;
    GLboolean ARB_fragment_program_shadow;
    GLboolean ARB_fragment_shader;
    GLboolean ARB_framebuffer_no_attachments;
    GLboolean ARB_framebuffer_object;
    GLboolean ARB_fragment_shader_interlock;
    GLboolean ARB_enhanced_layouts;
    GLboolean ARB_explicit_attrib_location;
    GLboolean ARB_explicit_uniform_location;
    GLboolean ARB_gl_spirv;
    GLboolean ARB_gpu_shader5;
    GLboolean ARB_gpu_shader_fp64;
    GLboolean ARB_gpu_shader_int64;
    GLboolean ARB_half_float_vertex;
    GLboolean ARB_indirect_parameters;
    GLboolean ARB_instanced_arrays;
    GLboolean ARB_internalformat_query;
    GLboolean ARB_internalformat_query2;
    GLboolean ARB_map_buffer_range;
    GLboolean ARB_occlusion_query;
    GLboolean ARB_occlusion_query2;
    GLboolean ARB_pipeline_statistics_query;
    GLboolean ARB_point_sprite;
    GLboolean ARB_polygon_offset_clamp;
    GLboolean ARB_post_depth_coverage;
    GLboolean ARB_query_buffer_object;
    GLboolean ARB_robust_buffer_access_behavior;
    GLboolean ARB_sample_locations;
    GLboolean ARB_sample_shading;
    GLboolean ARB_seamless_cube_map;
    GLboolean ARB_shader_atomic_counter_ops;
    GLboolean ARB_shader_atomic_counters;
    GLboolean ARB_shader_ballot;
    GLboolean ARB_shader_bit_encoding;
    GLboolean ARB_shader_clock;
    GLboolean ARB_shader_draw_parameters;
    GLboolean ARB_shader_group_vote;
    GLboolean ARB_shader_image_load_store;
    GLboolean ARB_shader_image_size;
    GLboolean ARB_shader_precision;
    GLboolean ARB_shader_stencil_export;
    GLboolean ARB_shader_storage_buffer_object;
    GLboolean ARB_shader_texture_image_samples;
    GLboolean ARB_shader_texture_lod;
    GLboolean ARB_shader_viewport_layer_array;
    GLboolean ARB_shading_language_packing;
    GLboolean ARB_shading_language_420pack;
    GLboolean ARB_shadow;
    GLboolean ARB_sparse_buffer;
    GLboolean ARB_sparse_texture;
    GLboolean ARB_sparse_texture2;
    GLboolean ARB_sparse_texture_clamp;
    GLboolean ARB_stencil_texturing;
    GLboolean ARB_spirv_extensions;
    GLboolean ARB_sync;
    GLboolean ARB_tessellation_shader;
    GLboolean ARB_texture_buffer_object;
    GLboolean ARB_texture_buffer_object_rgb32;
    GLboolean ARB_texture_buffer_range;
    GLboolean ARB_texture_compression_bptc;
    GLboolean ARB_texture_compression_rgtc;
    GLboolean ARB_texture_cube_map_array;
    GLboolean ARB_texture_filter_anisotropic;
    GLboolean ARB_texture_filter_minmax;
    GLboolean ARB_texture_float;
    GLboolean ARB_texture_gather;
    GLboolean ARB_texture_mirror_clamp_to_edge;
    GLboolean ARB_texture_multisample;
    GLboolean ARB_texture_non_power_of_two;
    GLboolean ARB_texture_stencil8;
    GLboolean ARB_texture_query_levels;
    GLboolean ARB_texture_query_lod;
    GLboolean ARB_texture_rg;
    GLboolean ARB_texture_rgb10_a2ui;
    GLboolean ARB_texture_view;
    GLboolean ARB_timer_query;
    GLboolean ARB_transform_feedback2;
    GLboolean ARB_transform_feedback3;
    GLboolean ARB_transform_feedback_instanced;
    GLboolean ARB_transform_feedback_overflow_query;
    GLboolean ARB_uniform_buffer_object;
    GLboolean ARB_vertex_attrib_64bit;
    GLboolean ARB_vertex_program;
    GLboolean ARB_vertex_shader;
    GLboolean ARB_vertex_type_10f_11f_11f_rev;
    GLboolean ARB_vertex_type_2_10_10_10_rev;
    GLboolean ARB_viewport_array;
    GLboolean EXT_blend_equation_separate;
    GLboolean EXT_demote_to_helper_invocation;
    GLboolean EXT_depth_bounds_test;
    GLboolean EXT_disjoint_timer_query;
    GLboolean EXT_draw_buffers2;
    GLboolean EXT_EGL_image_storage;
    GLboolean EXT_float_blend;
    GLboolean EXT_framebuffer_multisample;
    GLboolean EXT_framebuffer_multisample_blit_scaled;
    GLboolean EXT_framebuffer_sRGB;
    GLboolean EXT_gpu_program_parameters;
    GLboolean EXT_gpu_shader4;
    GLboolean EXT_memory_object;
    GLboolean EXT_memory_object_fd;
    GLboolean EXT_multisampled_render_to_texture;
    GLboolean EXT_packed_float;
    GLboolean EXT_pixel_buffer_object;
    GLboolean EXT_provoking_vertex;
    GLboolean EXT_render_snorm;
    GLboolean EXT_semaphore;
    GLboolean EXT_semaphore_fd;
    GLboolean EXT_shader_image_load_formatted;
    GLboolean EXT_shader_image_load_store;
    GLboolean EXT_shader_integer_mix;
    GLboolean EXT_shader_samples_identical;
    GLboolean EXT_sRGB;
    GLboolean EXT_stencil_two_side;
    GLboolean EXT_texture_array;
    GLboolean EXT_texture_buffer_object;
    GLboolean EXT_texture_compression_latc;
    GLboolean EXT_texture_compression_s3tc;
    GLboolean EXT_texture_compression_s3tc_srgb;
    GLboolean EXT_texture_env_dot3;
    GLboolean EXT_texture_filter_anisotropic;
    GLboolean EXT_texture_filter_minmax;
    GLboolean EXT_texture_integer;
    GLboolean EXT_texture_mirror_clamp;
    GLboolean EXT_texture_norm16;
    GLboolean EXT_texture_shadow_lod;
    GLboolean EXT_texture_shared_exponent;
    GLboolean EXT_texture_snorm;
    GLboolean EXT_texture_sRGB;
    GLboolean EXT_texture_sRGB_R8;
    GLboolean EXT_texture_sRGB_RG8;
    GLboolean EXT_texture_sRGB_decode;
    GLboolean EXT_texture_swizzle;
    GLboolean EXT_texture_type_2_10_10_10_REV;
    GLboolean EXT_transform_feedback;
    GLboolean EXT_timer_query;
    GLboolean EXT_vertex_array_bgra;
    GLboolean EXT_window_rectangles;
    GLboolean OES_copy_image;
    GLboolean OES_primitive_bounding_box;
    GLboolean OES_sample_variables;
    GLboolean OES_standard_derivatives;
    GLboolean OES_texture_buffer;
    GLboolean OES_texture_cube_map_array;
    GLboolean OES_texture_view;
    GLboolean OES_viewport_array;
    /* vendor extensions */
    GLboolean AMD_compressed_ATC_texture;
    GLboolean AMD_framebuffer_multisample_advanced;
    GLboolean AMD_depth_clamp_separate;
    GLboolean AMD_performance_monitor;
    GLboolean AMD_pinned_memory;
    GLboolean AMD_seamless_cubemap_per_texture;
    GLboolean AMD_vertex_shader_layer;
    GLboolean AMD_vertex_shader_viewport_index;
    GLboolean ANDROID_extension_pack_es31a;
    GLboolean APPLE_object_purgeable;
    GLboolean ATI_meminfo;
    GLboolean ATI_texture_compression_3dc;
    GLboolean ATI_texture_mirror_once;
    GLboolean ATI_texture_env_combine3;
    GLboolean ATI_fragment_shader;
    GLboolean GREMEDY_string_marker;
    GLboolean INTEL_blackhole_render;
    GLboolean INTEL_conservative_rasterization;
    GLboolean INTEL_performance_query;
    GLboolean INTEL_shader_atomic_float_minmax;
    GLboolean INTEL_shader_integer_functions2;
    GLboolean KHR_blend_equation_advanced;
    GLboolean KHR_blend_equation_advanced_coherent;
    GLboolean KHR_robustness;
    GLboolean KHR_texture_compression_astc_hdr;
    GLboolean KHR_texture_compression_astc_ldr;
    GLboolean KHR_texture_compression_astc_sliced_3d;
    GLboolean MESA_framebuffer_flip_y;
    GLboolean MESA_pack_invert;
    GLboolean MESA_tile_raster_order;
    GLboolean EXT_shader_framebuffer_fetch;
    GLboolean EXT_shader_framebuffer_fetch_non_coherent;
    GLboolean MESA_shader_integer_functions;
    GLboolean MESA_window_pos;
    GLboolean MESA_ycbcr_texture;
    GLboolean NV_alpha_to_coverage_dither_control;
    GLboolean NV_compute_shader_derivatives;
    GLboolean NV_conditional_render;
    GLboolean NV_copy_depth_to_color;
    GLboolean NV_copy_image;
    GLboolean NV_fill_rectangle;
    GLboolean NV_fog_distance;
    GLboolean NV_primitive_restart;
    GLboolean NV_shader_atomic_float;
    GLboolean NV_shader_atomic_int64;
    GLboolean NV_texture_barrier;
    GLboolean NV_texture_env_combine4;
    GLboolean NV_texture_rectangle;
    GLboolean NV_vdpau_interop;
    GLboolean NV_conservative_raster;
    GLboolean NV_conservative_raster_dilate;
    GLboolean NV_conservative_raster_pre_snap_triangles;
    GLboolean NV_conservative_raster_pre_snap;
    GLboolean NV_viewport_array2;
    GLboolean NV_viewport_swizzle;
    GLboolean NVX_gpu_memory_info;
    GLboolean TDFX_texture_compression_FXT1;
    GLboolean OES_EGL_image;
    GLboolean OES_draw_texture;
    GLboolean OES_depth_texture_cube_map;
    GLboolean OES_EGL_image_external;
    GLboolean OES_texture_float;
    GLboolean OES_texture_float_linear;
    GLboolean OES_texture_half_float;
    GLboolean OES_texture_half_float_linear;
    GLboolean OES_compressed_ETC1_RGB8_texture;
    GLboolean OES_geometry_shader;
    GLboolean OES_texture_compression_astc;
    GLboolean extension_sentinel;
    /** The extension string */
    const GLubyte *String;
    /** Number of supported extensions */
    GLuint Count;
    /**
     * The context version which extension helper functions compare against.
     * By default, the value is equal to ctx->Version. This changes to ~0
     * while meta is in progress.
     */
    GLubyte Version;
};


#define MAX_TEXTURE_MBYTES 1024
#define MAX_TEXTURE_RECT_SIZE 16384
#define MAX_ARRAY_TEXTURE_LAYERS 64
#define MAX_TEXTURE_MAX_ANISOTROPY 16.0
#define MAX_TEXTURE_LOD_BIAS 14.0
#define MAX_ARRAY_LOCK_SIZE 3000
#define SUB_PIXEL_BITS 4
#define MIN_POINT_SIZE 1.0
#define MAX_POINT_SIZE 60.0
#define POINT_SIZE_GRANULARITY 0.1
#define MIN_LINE_WIDTH 1.0
#define MAX_LINE_WIDTH 10.0
#define LINE_WIDTH_GRANULARITY 0.1
#define MAX_TEXTURE_MBYTES 1024
#define MAX_TEXTURE_LEVELS 15
#define MAX_TEXTURE_RECT_SIZE 16384
#define MAX_LIGHTS 8
#define MAX_PROGRAM_LOCAL_PARAMS       4096
#define MAX_UNIFORMS                   4096
#define MAX_UNIFORM_BUFFERS            15 /* + 1 default uniform buffer */
#define MAX_SHADER_STORAGE_BUFFERS     16
#define MAX_COMBINED_UNIFORM_BUFFERS   (MAX_UNIFORM_BUFFERS * 6)
#define MAX_COMBINED_SHADER_STORAGE_BUFFERS   (MAX_SHADER_STORAGE_BUFFERS * 6)
#define MAX_ATOMIC_COUNTERS            4096
#define MAX_COMBINED_ATOMIC_BUFFERS    (MAX_UNIFORM_BUFFERS * 6)
#define ATOMIC_COUNTER_SIZE            4
#define MAX_IMAGE_UNIFORMS             32
#define MAX_IMAGE_UNITS                (MAX_IMAGE_UNIFORMS * 6)
#define MAX_PROGRAM_INSTRUCTIONS       (16 * 1024)
#define MAX_PROGRAM_ENV_PARAMS         256
#define MAX_TEXTURE_IMAGE_UNITS 32
#define MAX_PROGRAM_MATRICES           8
#define MAX_PROGRAM_MATRIX_STACK_DEPTH 4
#define MAX_PROGRAM_CALL_DEPTH         8
#define MAX_PROGRAM_TEMPS              256
#define MAX_PROGRAM_ADDRESS_REGS       1
#define MAX_SAMPLERS                   MAX_TEXTURE_IMAGE_UNITS
#define MAX_PROGRAM_INPUTS             32
#define MAX_PROGRAM_OUTPUTS            64
#define MAX_VERTEX_PROGRAM_ADDRESS_REGS 1
#define MAX_VERTEX_PROGRAM_PARAMS       MAX_UNIFORMS
#define MAX_FRAGMENT_PROGRAM_ADDRESS_REGS 0
#define MAX_FRAGMENT_PROGRAM_PARAMS       64
#define MAX_FRAGMENT_PROGRAM_INPUTS       12

#define SYS_proxy 9999
#define MAX_EVAL_ORDER 30
#define MAX_TEX 16
#define MAX_LIGHT 8
#define MAX_STACK_PROJECTION	16
#define MAX_STACK_TEXTURE	16
#define MAX_STACK_MODELVIEW	64
#define MAX_STACK_COLOR 16
#define MAX_STACK_ARB_MATRIX 8
#define MAX_CLIP_PLANES 6
#define MAX_VATTRIB 32
#define MAX_MAP_SIZE 256
#define MAX_ARB_MATRIX  8
#define MAX_VTX_PROG_ENV_PARAMS 96
#define MAX_VTX_PROG_LOC_PARAMS 96
#define MAX_FRG_PROG_ENV_PARAMS 24
#define MAX_FRG_PROG_LOC_PARAMS 24
#define MAX_DRAW_BUFFERS    16
#define MAX_COMBINED_TEXTURE_IMAGE_UNITS (MAX_TEXTURE_IMAGE_UNITS * 6)
#define MAX_COLOR_ATTACHMENTS 8
#define MAX_RENDERBUFFER_SIZE 16384
#define MAX_GEOMETRY_UNIFORM_COMPONENTS              512
#define MAX_GEOMETRY_OUTPUT_VERTICES                 256
#define MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS         1024
#define MAX_GEOMETRY_SHADER_INVOCATIONS     32
#define MAX_FEEDBACK_BUFFERS 4
#define MAX_FEEDBACK_ATTRIBS 32
#define MAX_GEOMETRY_SHADER_INVOCATIONS     32
#define MIN_FRAGMENT_INTERPOLATION_OFFSET   -0.5
#define MAX_FRAGMENT_INTERPOLATION_OFFSET   0.5
#define FRAGMENT_INTERPOLATION_OFFSET_BITS  4
#define MAX_VERTEX_STREAMS                  4
#define MAX_TESS_GEN_LEVEL 64
#define MAX_PATCH_VERTICES 32
#define MAX_TESS_PATCH_COMPONENTS 120
#define MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS 4096

struct gl_renderbuffer_attachment
{
    GLenum16 Type; /**< \c GL_NONE or \c GL_TEXTURE or \c GL_RENDERBUFFER_EXT */
    GLboolean Complete;

    /**
     * If \c Type is \c GL_RENDERBUFFER_EXT, this stores a pointer to the
     * application supplied renderbuffer object.
     */
    struct gl_renderbuffer *Renderbuffer;

    /**
     * If \c Type is \c GL_TEXTURE, this stores a pointer to the application
     * supplied texture object.
     */
    struct gl_texture_object *Texture;
    GLuint TextureLevel; /**< Attached mipmap level. */
    GLsizei NumSamples;  /**< from FramebufferTexture2DMultisampleEXT */
    GLuint CubeMapFace;  /**< 0 .. 5, for cube map textures. */
    GLuint Zoffset;      /**< Slice for 3D textures,  or layer for both 1D
                         * and 2D array textures */
    GLboolean Layered;
};

#endif //AWIMUL_DRI_INTERFACE_H
