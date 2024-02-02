#ifndef vrend_debug_h
#define vrend_debug_h

#include "virgl_protocol.h"
#include "virgl_util.h"
#include <stdarg.h>

struct vrend_context;
void vrend_print_context_name(const struct vrend_context *ctx);

enum virgl_debug_flags {
    dbg_shader_tgsi = 1 << 0,
    dbg_shader_glsl = 1 << 1,
    dbg_shader_streamout = 1 << 2,
    dbg_shader = dbg_shader_tgsi | dbg_shader_glsl | dbg_shader_streamout,
    dbg_cmd = 1 << 3,
    dbg_object = 1 << 4,
    dbg_blit = 1 << 5,
    dbg_copy_resource = 1 << 6,
    dbg_features = 1 << 7,
    dbg_tex = 1 << 8,
    dbg_caller = 1 << 9,
    dbg_tweak =  1 << 10,
    dbg_query =  1 << 11,
    dbg_gles =  1 << 12,
    dbg_bgra = 1 << 13,
    dbg_all = (1 << 14) - 1,
    dbg_allow_guest_override = 1 << 16,
    dbg_feature_use = 1 << 17,
    dbg_khr = 1 << 18,
};

const char *vrend_get_comand_name(enum virgl_context_cmd cmd);

const char *vrend_get_object_type_name(enum virgl_object_type cmd);


void vrend_init_debug_flags(void);

int  vrend_debug_can_override(void);

int vrend_get_debug_flags(const char *flagstring);

void vrend_context_set_debug_flags(struct vrend_context *ctx, const char *flags);

unsigned vrend_debug(const struct vrend_context *ctx, enum virgl_debug_flags flag);

void vrend_debug_add_flag(enum virgl_debug_flags flag);

static inline void vrend_printf(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    virgl_logv(fmt, va);
    va_end(va);
}

#ifndef NDEBUG
#define VREND_DEBUG(flag, ctx,  ...) \
   if (vrend_debug(ctx, flag)) \
      do { \
            vrend_print_context_name(ctx); \
            vrend_printf(__VA_ARGS__); \
      } while (0)

#define VREND_DEBUG_EXT(flag, ctx, X) \
   if (vrend_debug(ctx, flag)) \
      do { \
            vrend_print_context_name(ctx); \
            X; \
      } while (0)

#define VREND_DEBUG_NOCTX(flag, ctx, ...) \
   if (vrend_debug(ctx, flag)) \
      do { \
            vrend_printf(__VA_ARGS__); \
      } while (0)

#else
#define VREND_DEBUG(flag, ctx, ...) (void)ctx
#define VREND_DEBUG_EXT(flag, ctx, X) (void)ctx
#define VREND_DEBUG_NOCTX(flag, ctx, ...) (void)ctx
#endif

#endif
