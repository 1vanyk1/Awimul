#ifndef API_VALIDATE_H
#define API_VALIDATE_H

#include "mtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gl_buffer_object;
struct gl_context;
struct gl_transform_feedback_object;


extern GLenum
_mesa_valid_prim_mode(struct gl_context *ctx, GLenum mode);

extern void
_mesa_update_valid_to_render_state(struct gl_context *ctx);

/**
 * Is 'mode' a valid value for glBegin(), glDrawArrays(), glDrawElements(),
 * etc?  The set of legal values depends on whether geometry shaders/programs
 * are supported.
 * Note: This may be called during display list compilation.
 */
static inline bool
_mesa_is_valid_prim_mode(const struct gl_context *ctx, GLenum mode)
{
    /* All primitive types are less than 32, which allows us to use a mask. */
    return mode < 32 && (1u << mode) & ctx->SupportedPrimMask;
}

#ifdef __cplusplus
}
#endif

#endif
