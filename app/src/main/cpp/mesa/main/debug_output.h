#ifndef DEBUG_OUTPUT_H
#define DEBUG_OUTPUT_H


#include <stdio.h>
#include <stdarg.h>
#include "glheader.h"
#include "menums.h"


#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;

void
_mesa_init_debug_output(struct gl_context *ctx);

void
_mesa_destroy_debug_output(struct gl_context *ctx);

void
_mesa_debug_get_id(GLuint *id);

bool
_mesa_set_debug_state_int(struct gl_context *ctx, GLenum pname, GLint val);

GLint
_mesa_get_debug_state_int(struct gl_context *ctx, GLenum pname);

void *
_mesa_get_debug_state_ptr(struct gl_context *ctx, GLenum pname);

void
_mesa_log_msg(struct gl_context *ctx, enum mesa_debug_source source,
              enum mesa_debug_type type, GLuint id,
              enum mesa_debug_severity severity, GLint len, const char *buf);

bool
_mesa_debug_is_message_enabled(const struct gl_debug_state *debug,
                               enum mesa_debug_source source,
                               enum mesa_debug_type type,
                               GLuint id,
                               enum mesa_debug_severity severity);

void GLAPIENTRY
_mesa_DebugMessageInsert(GLenum source, GLenum type, GLuint id,
                         GLenum severity, GLint length,
                         const GLchar* buf);

GLuint GLAPIENTRY
_mesa_GetDebugMessageLog(GLuint count, GLsizei logSize, GLenum* sources,
                         GLenum* types, GLenum* ids, GLenum* severities,
                         GLsizei* lengths, GLchar* messageLog);

void GLAPIENTRY
_mesa_DebugMessageControl(GLenum source, GLenum type, GLenum severity,
                          GLsizei count, const GLuint *ids,
                          GLboolean enabled);

void GLAPIENTRY
_mesa_DebugMessageCallback(GLDEBUGPROC callback,
                           const void *userParam);

void GLAPIENTRY
_mesa_PushDebugGroup(GLenum source, GLuint id, GLsizei length,
                     const GLchar *message);

void GLAPIENTRY
_mesa_PopDebugGroup(void);

void GLAPIENTRY
_mesa_StringMarkerGREMEDY(GLsizei len, const GLvoid *string);


#ifdef __cplusplus
}
#endif


#endif /* DEBUG_OUTPUT_H */
