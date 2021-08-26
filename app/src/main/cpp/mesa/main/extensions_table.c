#include "extensions.h"

/**
 * Given a member \c x of struct gl_extensions, return offset of
 * \c x in bytes.
 */
#define o(x) offsetof(struct gl_extensions, x)

/**
 * \brief Table of supported OpenGL extensions for all API's.
 */
const struct mesa_extension _mesa_extension_table[] = {
#define EXT(name_str, driver_cap, gll_ver, glc_ver, gles_ver, gles2_ver, yyyy) \
        { .name = "GL_" #name_str, .offset = o(driver_cap), \
          .version = { \
            [API_OPENGL_COMPAT] = gll_ver, \
            [API_OPENGL_CORE]   = glc_ver, \
            [API_OPENGLES]      = gles_ver, \
            [API_OPENGLES2]     = gles2_ver, \
           }, \
           .year = yyyy \
        },
#include "extensions_table.h"
#undef EXT
};
