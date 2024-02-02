#ifndef AWIMUL_DRI_H
#define AWIMUL_DRI_H
#include "../GL/internal/dri_interface.h"
#include "dri_private.h"

#ifndef MESA_DEBUG_FLAGS
int MESA_DEBUG_FLAGS = 0;
#endif

extern const __DRIextension **__driDriverGetExtensions_swrast(void);

static uint32_t findOption(const driOptionCache *cache, const char *name);

extern void driParseOptionInfo(driOptionCache *info, const driOptionDescription *configOptions, unsigned numOptions);

extern void driDestroyOptionCache(driOptionCache *cache);

extern void driDestroyOptionInfo(driOptionCache *info);

char * driQueryOptionstr(const driOptionCache *cache, const char *name);

static inline void simple_mtx_lock(simple_mtx_t *mtx);

static inline void simple_mtx_unlock(simple_mtx_t *mtx);

static inline uint32_t
util_fast_urem32(uint32_t n, uint32_t d, uint64_t magic)
{
    uint64_t lowbits = magic * n;
    uint32_t result = _mul32by64_hi(d, lowbits);
    assert(result == n % d);
    return result;
}

#if __cplusplus >= 201703L || __STDC_VERSION__ > 201710L
/* Standard C++17/C23 attribute */
#define FALLTHROUGH [[fallthrough]]
#elif __has_attribute(fallthrough)
/* Non-standard but supported by at least gcc and clang */
#define FALLTHROUGH __attribute__((fallthrough))
#else
#define FALLTHROUGH do { } while(0)
#endif

#define u_current_get_table_internal _glapi_get_dispatch
#define u_current_get_context_internal _glapi_get_context

extern void _mesa_hash_table_remove(struct hash_table *ht, struct hash_entry *entry);

extern struct hash_entry *_mesa_hash_table_search(struct hash_table *ht, const void *key);

extern void *_glapi_Context;

void * u_current_get_context_internal(void);

bool _mesa_override_gl_version_contextless(struct gl_constants *consts, gl_api *apiOut, GLuint *versionOut);

void _mesa_init_constants(struct gl_constants *consts, gl_api api);

void _mesa_init_extensions(struct gl_extensions *extensions);

static inline gl_shader_stage
tgsi_processor_to_shader_stage(unsigned processor)
{
    switch (processor) {
        case PIPE_SHADER_FRAGMENT:  return MESA_SHADER_FRAGMENT;
        case PIPE_SHADER_VERTEX:    return MESA_SHADER_VERTEX;
        case PIPE_SHADER_GEOMETRY:  return MESA_SHADER_GEOMETRY;
        case PIPE_SHADER_TESS_CTRL: return MESA_SHADER_TESS_CTRL;
        case PIPE_SHADER_TESS_EVAL: return MESA_SHADER_TESS_EVAL;
        case PIPE_SHADER_COMPUTE:   return MESA_SHADER_COMPUTE;
        default:
            unreachable("invalid TGSI processor");
    }
}

GLuint _mesa_get_version(const struct gl_extensions *extensions, struct gl_constants *consts, gl_api api);

void _mesa_override_glsl_version(struct gl_constants *consts);

static inline bool _mesa_is_desktop_gl(const struct gl_context *ctx);

const char *_mesa_get_format_name(mesa_format format);

static inline void pipe_resource_reference(struct pipe_resource **dst, struct pipe_resource *src);

static inline const char *util_format_short_name(enum pipe_format format);

static inline size_t util_format_get_stride(enum pipe_format format, unsigned width);

static inline boolean pipe_reference_described(struct pipe_reference *ptr, struct pipe_reference *reference, debug_reference_descriptor get_desc);

void ralloc_free(void *ptr);

static inline int mtx_lock(mtx_t *mtx);

static inline int mtx_unlock(mtx_t *mtx);

static inline const char *util_format_name(enum pipe_format format);

void _mesa_init_renderbuffer(struct gl_renderbuffer *rb, GLuint name);

static inline void simple_mtx_init(simple_mtx_t *mtx, int type);

GLenum _mesa_get_format_base_format(uint32_t format);

static inline void pipe_surface_release(struct pipe_context *pipe, struct pipe_surface **ptr);

void
_mesa_attach_and_own_rb(struct gl_framebuffer *fb,
                        gl_buffer_index bufferName,
                        struct gl_renderbuffer *rb);

void
_mesa_attach_and_reference_rb(struct gl_framebuffer *fb,
                              gl_buffer_index bufferName,
                              struct gl_renderbuffer *rb);

struct hash_entry *_mesa_hash_table_insert(struct hash_table *ht, const void *key, void *data);

static inline void pipe_surface_reference(struct pipe_surface **dst, struct pipe_surface *src);

void _mesa_resize_framebuffer(struct gl_context *ctx, struct gl_framebuffer *fb, GLuint width, GLuint height);

#endif //AWIMUL_DRI_H
