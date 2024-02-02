#ifndef AWIMUL_ST_H
#define AWIMUL_ST_H

struct st_manager
{
    struct pipe_screen *screen;
    bool (*get_egl_image)(struct st_manager *smapi,
                          void *egl_image,
                          struct st_egl_image *out);
    bool (*validate_egl_image)(struct st_manager *smapi,
                               void *egl_image);
    int (*get_param)(struct st_manager *smapi,
                     enum st_manager_param param);
    void (*set_background_context)(struct st_context_iface *stctxi,
                                   struct util_queue_monitoring *queue_info);
    void (*destroy)(struct st_manager *smapi);
    void *st_manager_private;
};

struct st_manager_private
{
    struct hash_table *stfbi_ht; /* framebuffer iface objects hash table */
    simple_mtx_t st_mutex;
};

struct st_framebuffer
{
    struct gl_framebuffer Base;

    struct st_framebuffer_iface *iface;
    enum st_attachment_type statts[ST_ATTACHMENT_COUNT];
    unsigned num_statts;
    int32_t stamp;
    int32_t iface_stamp;
    uint32_t iface_ID;

    /* list of framebuffer objects */
    struct list_head head;
};

extern void
_mesa_reference_framebuffer_(struct gl_framebuffer **ptr,
                             struct gl_framebuffer *fb);

static inline void
_mesa_reference_framebuffer(struct gl_framebuffer **ptr,
                            struct gl_framebuffer *fb)
{
    if (*ptr != fb)
        _mesa_reference_framebuffer_(ptr, fb);
}

struct st_visual
{
    /**
     * Available buffers.  Bitfield of ST_ATTACHMENT_*_MASK bits.
     */
    unsigned buffer_mask;

    /**
     * Buffer formats.  The formats are always set even when the buffer is
     * not available.
     */
    enum pipe_format color_format;
    enum pipe_format depth_stencil_format;
    enum pipe_format accum_format;
    unsigned samples;

    /**
     * Desired render buffer.
     */
    enum st_attachment_type render_buffer;
};

struct st_sampler_view
{
    struct pipe_sampler_view *view;

    /** The context which created this view */
    struct st_context *st;

    /** The glsl version of the shader seen during validation */
    bool glsl130_or_later;
    /** Derived from the sampler's sRGBDecode state during validation */
    bool srgb_skip_decode;

    /* This mechanism allows passing sampler view references to the driver
     * without using atomics to increase the reference count.
     *
     * This private refcount can be decremented without atomics but only one
     * context (st above) can use this counter (so that it's only used by
     * 1 thread).
     *
     * This number is atomically added to view->reference.count at
     * initialization. If it's never used, the same number is atomically
     * subtracted from view->reference.count before destruction. If this
     * number is decremented, we can pass one reference to the driver without
     * touching reference.count with atomics. At destruction we only subtract
     * the number of references we have not returned. This can possibly turn
     * a million atomic increments into 1 add and 1 subtract atomic op over
     * the whole lifetime of an app.
     */
    int private_refcount;
};

struct st_sampler_views
{
    struct st_sampler_views *next;
    uint32_t max;
    uint32_t count;
    struct st_sampler_view views[0];
};

struct st_zombie_sampler_view_node
{
    struct pipe_sampler_view *view;
    struct list_head node;
};

struct st_renderbuffer
{
    struct gl_renderbuffer Base;
    struct pipe_resource *texture;
    /* This points to either "surface_linear" or "surface_srgb".
     * It doesn't hold the pipe_surface reference. The other two do.
     */
    struct pipe_surface *surface;
    struct pipe_surface *surface_linear;
    struct pipe_surface *surface_srgb;
    GLboolean defined;        /**< defined contents? */

    struct pipe_transfer *transfer; /**< only used when mapping the resource */

    /**
     * Used only when hardware accumulation buffers are not supported.
     */
    boolean software;
    void *data;

    bool use_readpix_cache;

    /* Inputs from Driver.RenderTexture, don't use directly. */
    boolean is_rtt; /**< whether Driver.RenderTexture was called */
    unsigned rtt_face, rtt_slice;
    boolean rtt_layered; /**< whether glFramebufferTexture was called */
    unsigned rtt_nr_samples; /**< from FramebufferTexture2DMultisampleEXT */
};

#define ST_CALLOC_STRUCT(T)   (struct T *) calloc(1, sizeof(struct T))

static inline struct st_context *st_context(struct gl_context *ctx);

enum pipe_format
st_choose_format(struct st_context *st, GLenum internalFormat,
                 GLenum format, GLenum type,
                 enum pipe_texture_target target, unsigned sample_count,
                 unsigned storage_sample_count,
                 unsigned bindings, bool swap_bytes, bool allow_dxt);

mesa_format st_pipe_format_to_mesa_format(enum pipe_format format);

static inline struct st_renderbuffer * st_renderbuffer(struct gl_renderbuffer *rb);

static inline void pipe_surface_release_no_context(struct pipe_surface **ptr);

static inline void
_mesa_reference_renderbuffer(struct gl_renderbuffer **ptr,
                             struct gl_renderbuffer *rb);

#endif //AWIMUL_ST_H
