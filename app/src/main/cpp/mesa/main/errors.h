#ifndef ERRORS_H
#define ERRORS_H


#include <stdio.h>
#include <stdarg.h>
#include "glheader.h"
#include "menums.h"


#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;

extern void
_mesa_warning( struct gl_context *gc, const char *fmtString, ... ) PRINTFLIKE(2, 3);

extern void
_mesa_problem( const struct gl_context *ctx, const char *fmtString, ... ) PRINTFLIKE(2, 3);

extern void
_mesa_error( struct gl_context *ctx, GLenum error, const char *fmtString, ... ) PRINTFLIKE(3, 4);

extern void
_mesa_error_no_memory(const char *caller);

extern void
_mesa_debug( const struct gl_context *ctx, const char *fmtString, ... ) PRINTFLIKE(2, 3);

extern void
_mesa_log(const char *fmtString, ...) PRINTFLIKE(1, 2);

extern FILE *
_mesa_get_log_file(void);

void
_mesa_shader_debug(struct gl_context *ctx, GLenum type, GLuint *id,
                   const char *msg);

extern void
_mesa_gl_vdebugf(struct gl_context *ctx,
                 GLuint *id,
                 enum mesa_debug_source source,
                 enum mesa_debug_type type,
                 enum mesa_debug_severity severity,
                 const char *fmtString,
                 va_list args);

extern void
_mesa_gl_debugf(struct gl_context *ctx,
                GLuint *id,
                enum mesa_debug_source source,
                enum mesa_debug_type type,
                enum mesa_debug_severity severity,
                const char *fmtString, ...) PRINTFLIKE(6, 7);

extern size_t
_mesa_gl_debug(struct gl_context *ctx,
               GLuint *id,
               enum mesa_debug_source source,
               enum mesa_debug_type type,
               enum mesa_debug_severity severity,
               const char *msg);

#define _mesa_perf_debug(ctx, sev, ...) do {                              \
   static GLuint msg_id = 0;                                              \
   if (unlikely(ctx->Const.ContextFlags & GL_CONTEXT_FLAG_DEBUG_BIT)) {   \
      _mesa_gl_debugf(ctx, &msg_id,                                       \
                      MESA_DEBUG_SOURCE_API,                              \
                      MESA_DEBUG_TYPE_PERFORMANCE,                        \
                      sev,                                                \
                      __VA_ARGS__);                                       \
   }                                                                      \
} while (0)

void GLAPIENTRY
_mesa_InternalSetError(GLenum error);

#ifdef __cplusplus
}
#endif


#endif /* ERRORS_H */
