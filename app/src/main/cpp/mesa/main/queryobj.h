#ifndef QUERYOBJ_H
#define QUERYOBJ_H


#include "mtypes.h"
#include "hash.h"


static inline struct gl_query_object *
_mesa_lookup_query_object(struct gl_context *ctx, GLuint id)
{
    return (struct gl_query_object *)
            _mesa_HashLookupLocked(ctx->Query.QueryObjects, id);
}


extern void
_mesa_init_query_object_functions(struct dd_function_table *driver);

extern void
_mesa_init_queryobj(struct gl_context *ctx);

extern void
_mesa_free_queryobj_data(struct gl_context *ctx);

extern void
_mesa_delete_query(struct gl_context *ctx, struct gl_query_object *q);

void GLAPIENTRY
_mesa_GenQueries(GLsizei n, GLuint *ids);
void GLAPIENTRY
_mesa_CreateQueries(GLenum target, GLsizei n, GLuint *ids);
void GLAPIENTRY
_mesa_DeleteQueries(GLsizei n, const GLuint *ids);
GLboolean GLAPIENTRY
_mesa_IsQuery(GLuint id);
void GLAPIENTRY
_mesa_BeginQueryIndexed(GLenum target, GLuint index, GLuint id);
void GLAPIENTRY
_mesa_EndQueryIndexed(GLenum target, GLuint index);
void GLAPIENTRY
_mesa_BeginQuery(GLenum target, GLuint id);
void GLAPIENTRY
_mesa_EndQuery(GLenum target);
void GLAPIENTRY
_mesa_QueryCounter(GLuint id, GLenum target);
void GLAPIENTRY
_mesa_GetQueryIndexediv(GLenum target, GLuint index, GLenum pname,
GLint *params);
void GLAPIENTRY
_mesa_GetQueryiv(GLenum target, GLenum pname, GLint *params);
void GLAPIENTRY
_mesa_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params);
void GLAPIENTRY
_mesa_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params);
void GLAPIENTRY
_mesa_GetQueryObjecti64v(GLuint id, GLenum pname, GLint64EXT *params);
void GLAPIENTRY
_mesa_GetQueryObjectui64v(GLuint id, GLenum pname, GLuint64EXT *params);
void GLAPIENTRY
_mesa_GetQueryBufferObjectiv(GLuint id, GLuint buffer, GLenum pname,
GLintptr offset);
void GLAPIENTRY
_mesa_GetQueryBufferObjectuiv(GLuint id, GLuint buffer, GLenum pname,
GLintptr offset);
void GLAPIENTRY
_mesa_GetQueryBufferObjecti64v(GLuint id, GLuint buffer, GLenum pname,
GLintptr offset);
void GLAPIENTRY
_mesa_GetQueryBufferObjectui64v(GLuint id, GLuint buffer, GLenum pname,
GLintptr offset);

#endif /* QUERYOBJ_H */
