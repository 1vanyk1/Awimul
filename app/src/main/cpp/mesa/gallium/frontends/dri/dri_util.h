#ifndef _DRI_UTIL_H_
#define _DRI_UTIL_H_

#include "../../../../GL/gl.h"
#include "../../../../GL/internal/dri_interface.h"
#include "../../../main/menums.h"
#include "../../../main/formats.h"
#include "../../../util/xmlconfig.h"
#include <stdbool.h>

struct gl_config;
struct gl_context;

/**
 * Extensions.
 */
extern const __DRIcoreExtension driCoreExtension;
extern const __DRIswrastExtension driSWRastExtension;
extern const __DRIdri2Extension driDRI2Extension;
extern const __DRI2configQueryExtension dri2ConfigQueryExtension;
extern const __DRIcopySubBufferExtension driCopySubBufferExtension;
extern const __DRI2flushControlExtension dri2FlushControlExtension;

/**
 * Description of the attributes used to create a config.
 *
 * This is passed as the context_config parameter to CreateContext. The idea
 * with this struct is that it can be extended without having to modify all of
 * the drivers. The first three members (major/minor_version and flags) are
 * always valid, but the remaining members are only valid if the corresponding
 * flag is set for the attribute. If the flag is not set then the default
 * value should be assumed. That way the driver can quickly check if any
 * attributes were set that it doesn't understand and report an error.
 */
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

#define __DRIVER_CONTEXT_ATTRIB_RESET_STRATEGY   (1 << 0)
#define __DRIVER_CONTEXT_ATTRIB_PRIORITY         (1 << 1)
#define __DRIVER_CONTEXT_ATTRIB_RELEASE_BEHAVIOR (1 << 2)

/**
 * Driver callback functions.
 *
 * Each DRI driver must have one of these structures with all the pointers set
 * to appropriate functions within the driver.
 *
 * When glXCreateContext() is called, for example, it'll call a helper function
 * dri_util.c which in turn will jump through the \a CreateContext pointer in
 * this structure.
 */
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

extern const struct __DriverAPIRec driDriverAPI;
extern const struct __DriverAPIRec *globalDriverAPI;

/**
 * Per-screen private driver information.
 */
struct __DRIscreenRec {
    /**
     * Driver-specific entrypoints provided by the driver's
     * __DRIDriverVtableExtensionRec.
     */
    const struct __DriverAPIRec *driver;

    /**
     * Current screen's number
     */
    int myNum;

    /**
     * File descriptor returned when the kernel device driver is opened.
     *
     * Used to:
     *   - authenticate client to kernel
     *   - map the frame buffer, SAREA, etc.
     *   - close the kernel device driver
     */
    int fd;

    /**
     * Device-dependent private information (not stored in the SAREA).
     *
     * This pointer is never touched by the DRI layer.
     */
    void *driverPrivate;

    void *loaderPrivate;

    int max_gl_core_version;
    int max_gl_compat_version;
    int max_gl_es1_version;
    int max_gl_es2_version;

    const __DRIextension **extensions;

    const __DRIswrastLoaderExtension *swrast_loader;

    struct {
        /* Flag to indicate that this is a DRI2 screen.  Many of the above
         * fields will not be valid or initializaed in that case. */
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

/**
 * Per-context private driver information.
 */
struct __DRIcontextRec {
    /**
     * Device driver's private context data.  This structure is opaque.
     */
    void *driverPrivate;

    /**
     * The loaders's private context data.  This structure is opaque.
     */
    void *loaderPrivate;

    /**
     * Pointer to drawable currently bound to this context for drawing.
     */
    __DRIdrawable *driDrawablePriv;

    /**
     * Pointer to drawable currently bound to this context for reading.
     */
    __DRIdrawable *driReadablePriv;

    /**
     * Pointer to screen on which this context was created.
     */
    __DRIscreen *driScreenPriv;

    struct {
        int draw_stamp;
        int read_stamp;
    } dri2;
};

/**
 * Per-drawable private DRI driver information.
 */
struct __DRIdrawableRec {
    /**
     * Driver's private drawable information.
     *
     * This structure is opaque.
     */
    void *driverPrivate;

    /**
     * Private data from the loader.  We just hold on to it and pass
     * it back when calling into loader provided functions.
     */
    void *loaderPrivate;

    /**
     * Pointer to context to which this drawable is currently bound.
     */
    __DRIcontext *driContextPriv;

    /**
     * Pointer to screen on which this drawable was created.
     */
    __DRIscreen *driScreenPriv;

    /**
     * Reference count for number of context's currently bound to this
     * drawable.
     *
     * Once it reaches zero, the drawable can be destroyed.
     *
     * \note This behavior will change with GLX 1.3.
     */
    int refcount;

    /**
     * Last value of the stamp.
     *
     * If this differs from the value stored at __DRIdrawable::dri2.stamp,
     * then the drawable information has been modified by the X server, and the
     * drawable information (below) should be retrieved from the X server.
     */
    unsigned int lastStamp;

    int w, h;

    /**
     * Drawable timestamp.  Increased when the loader calls invalidate.
     */
    struct {
        unsigned int stamp;
    } dri2;
};

extern uint32_t
driGLFormatToImageFormat(mesa_format format);

extern uint32_t
driGLFormatToSizedInternalGLFormat(mesa_format format);

extern mesa_format
driImageFormatToGLFormat(uint32_t image_format);

extern void
dri2InvalidateDrawable(__DRIdrawable *drawable);

extern const __DRIimageDriverExtension driImageDriverExtension;

extern const __DRInoErrorExtension dri2NoErrorExtension;

#endif /* _DRI_UTIL_H_ */
