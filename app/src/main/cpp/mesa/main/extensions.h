#ifndef _EXTENSIONS_H_
#define _EXTENSIONS_H_

#include "mtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;
struct gl_extensions;

extern void _mesa_enable_sw_extensions(struct gl_context *ctx);

extern void _mesa_one_time_init_extension_overrides(void);

extern void _mesa_init_extensions(struct gl_extensions *extentions);

extern GLubyte *_mesa_make_extension_string(struct gl_context *ctx);

extern void _mesa_override_extensions(struct gl_context *ctx);

extern GLuint
_mesa_get_extension_count(struct gl_context *ctx);

extern const GLubyte *
_mesa_get_enabled_extension(struct gl_context *ctx, GLuint index);


/**
 * \brief An element of the \c extension_table.
 */
struct mesa_extension {
    /** Name of extension, such as "GL_ARB_depth_clamp". */
    const char *name;

    /** Offset (in bytes) of the corresponding member in struct gl_extensions. */
    size_t offset;

    /** Minimum version the extension requires for the given API
     * (see gl_api defined in mtypes.h). The value is equal to:
     * 10 * major_version + minor_version
     */
    uint8_t version[API_OPENGL_LAST + 1];

    /** Year the extension was proposed or approved.  Used to sort the
     * extension string chronologically. */
    uint16_t year;
};

extern const struct mesa_extension _mesa_extension_table[];


/* Generate enums for the functions below */
enum {
#define EXT(name_str, ...) MESA_EXTENSION_##name_str,
#include "extensions_table.h"
#undef EXT
    MESA_EXTENSION_COUNT
};


/** Checks if the context supports a user-facing extension */
#define EXT(name_str, driver_cap, ...) \
static inline bool \
_mesa_has_##name_str(const struct gl_context *ctx) \
{ \
   return ctx->Extensions.driver_cap && (ctx->Extensions.Version >= \
          _mesa_extension_table[MESA_EXTENSION_##name_str].version[ctx->API]); \
}
#include "extensions_table.h"
#undef EXT

/* Sometimes the driver wants to query the extension override status before
 * a context is created. These variables are filled with extension override
 * information before context creation.
 *
 * This can be useful during extension bring-up when an extension is
 * partially implemented, but cannot yet be advertised as supported.
 *
 * Use it with care and keep access read-only.
 */
extern struct gl_extensions _mesa_extension_override_enables;
extern struct gl_extensions _mesa_extension_override_disables;

#ifdef __cplusplus
}
#endif

#endif
