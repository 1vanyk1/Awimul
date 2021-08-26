#ifndef SYNCOBJ_H
#define SYNCOBJ_H

#include "glheader.h"

struct _glapi_table;
struct dd_function_table;
struct gl_context;
struct gl_sync_object;

extern void
_mesa_init_sync_object_functions(struct dd_function_table *driver);

extern void
_mesa_init_sync(struct gl_context *);

extern void
_mesa_free_sync_data(struct gl_context *);

struct gl_sync_object *
_mesa_get_and_ref_sync(struct gl_context *ctx, GLsync sync, bool incRefCount);

extern void
_mesa_unref_sync_object(struct gl_context *ctx, struct gl_sync_object *syncObj,
                        int amount);

extern GLboolean GLAPIENTRY
_mesa_IsSync(GLsync sync);

void GLAPIENTRY
_mesa_DeleteSync_no_error(GLsync sync);

extern void GLAPIENTRY
_mesa_DeleteSync(GLsync sync);

GLsync GLAPIENTRY
_mesa_FenceSync_no_error(GLenum condition, GLbitfield flags);

extern GLsync GLAPIENTRY
_mesa_FenceSync(GLenum condition, GLbitfield flags);

GLenum GLAPIENTRY
_mesa_ClientWaitSync_no_error(GLsync sync, GLbitfield flags, GLuint64 timeout);

extern GLenum GLAPIENTRY
_mesa_ClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);

void GLAPIENTRY
_mesa_WaitSync_no_error(GLsync sync, GLbitfield flags, GLuint64 timeout);

extern void GLAPIENTRY
_mesa_WaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);

extern void GLAPIENTRY
_mesa_GetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length,
                GLint *values);

#endif /* SYNCOBJ_H */
