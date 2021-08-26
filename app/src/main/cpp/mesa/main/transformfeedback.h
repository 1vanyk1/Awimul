#ifndef TRANSFORM_FEEDBACK_H
#define TRANSFORM_FEEDBACK_H

#include <stdbool.h>
#include "bufferobj.h"
#include "../util/compiler.h"
#include "glheader.h"
#include "mtypes.h"

struct _glapi_table;
struct dd_function_table;
struct gl_context;

extern void
_mesa_init_transform_feedback(struct gl_context *ctx);

extern void
_mesa_free_transform_feedback(struct gl_context *ctx);

extern void
_mesa_init_transform_feedback_functions(struct dd_function_table *driver);

extern unsigned
_mesa_compute_max_transform_feedback_vertices( struct gl_context *ctx,
                                               const struct gl_transform_feedback_object *obj,
                                               const struct gl_transform_feedback_info *info);


/*** GL_EXT_transform_feedback ***/

void GLAPIENTRY
_mesa_BeginTransformFeedback_no_error(GLenum mode);

extern void GLAPIENTRY
_mesa_BeginTransformFeedback(GLenum mode);

void GLAPIENTRY
_mesa_EndTransformFeedback_no_error(void);

extern void GLAPIENTRY
_mesa_EndTransformFeedback(void);

extern bool
_mesa_validate_buffer_range_xfb(struct gl_context *ctx,
                                struct gl_transform_feedback_object *obj,
                                GLuint index, struct gl_buffer_object *bufObj,
                                GLintptr offset, GLsizeiptr size, bool dsa);

extern void
_mesa_bind_buffer_base_transform_feedback(struct gl_context *ctx,
                                          struct gl_transform_feedback_object *obj,
                                          GLuint index,
                                          struct gl_buffer_object *bufObj,
                                          bool dsa);

void GLAPIENTRY
_mesa_BindBufferOffsetEXT_no_error(GLenum target, GLuint index, GLuint buffer,
                                   GLintptr offset);

extern void GLAPIENTRY
_mesa_BindBufferOffsetEXT(GLenum target, GLuint index, GLuint buffer,
                          GLintptr offset);

void GLAPIENTRY
_mesa_TransformFeedbackVaryings_no_error(GLuint program, GLsizei count,
                                         const GLchar *const *varyings,
                                         GLenum bufferMode);

extern void GLAPIENTRY
_mesa_TransformFeedbackVaryings(GLuint program, GLsizei count,
                                const GLchar * const *varyings,
                                GLenum bufferMode);

extern void GLAPIENTRY
_mesa_GetTransformFeedbackVarying(GLuint program, GLuint index,
                                  GLsizei bufSize, GLsizei *length,
                                  GLsizei *size, GLenum *type, GLchar *name);



/*** GL_ARB_transform_feedback2 ***/
extern void
_mesa_init_transform_feedback_object(struct gl_transform_feedback_object *obj,
                                     GLuint name);

extern void
_mesa_delete_transform_feedback_object(struct gl_context *ctx,
                                       struct gl_transform_feedback_object
                                       *obj);

struct gl_transform_feedback_object *
_mesa_lookup_transform_feedback_object(struct gl_context *ctx, GLuint name);

extern void GLAPIENTRY
_mesa_GenTransformFeedbacks(GLsizei n, GLuint *names);

extern void GLAPIENTRY
_mesa_CreateTransformFeedbacks(GLsizei n, GLuint *names);

extern GLboolean GLAPIENTRY
_mesa_IsTransformFeedback(GLuint name);

void GLAPIENTRY
_mesa_BindTransformFeedback_no_error(GLenum target, GLuint name);

extern void GLAPIENTRY
_mesa_BindTransformFeedback(GLenum target, GLuint name);

extern void GLAPIENTRY
_mesa_DeleteTransformFeedbacks(GLsizei n, const GLuint *names);

void GLAPIENTRY
_mesa_PauseTransformFeedback_no_error(void);

extern void GLAPIENTRY
_mesa_PauseTransformFeedback(void);

void GLAPIENTRY
_mesa_ResumeTransformFeedback_no_error(void);

extern void GLAPIENTRY
_mesa_ResumeTransformFeedback(void);

static inline bool
_mesa_is_xfb_active_and_unpaused(const struct gl_context *ctx)
{
    return ctx->TransformFeedback.CurrentObject->Active &&
           !ctx->TransformFeedback.CurrentObject->Paused;
}

extern bool
_mesa_transform_feedback_is_using_program(struct gl_context *ctx,
                                          struct gl_shader_program *shProg);

static inline void
_mesa_set_transform_feedback_binding(struct gl_context *ctx,
                                     struct gl_transform_feedback_object *tfObj, GLuint index,
                                     struct gl_buffer_object *bufObj,
                                     GLintptr offset, GLsizeiptr size)
{
    _mesa_reference_buffer_object(ctx, &tfObj->Buffers[index], bufObj);

    tfObj->BufferNames[index]   = bufObj ? bufObj->Name : 0;
    tfObj->Offset[index]        = offset;
    tfObj->RequestedSize[index] = size;

    if (bufObj)
        bufObj->UsageHistory |= USAGE_TRANSFORM_FEEDBACK_BUFFER;
}

static inline void
_mesa_bind_buffer_range_xfb(struct gl_context *ctx,
                            struct gl_transform_feedback_object *obj,
                            GLuint index, struct gl_buffer_object *bufObj,
                            GLintptr offset, GLsizeiptr size)
{
    /* Note: no need to FLUSH_VERTICES or flag NewTransformFeedback, because
     * transform feedback buffers can't be changed while transform feedback is
     * active.
     */

    /* The general binding point */
    _mesa_reference_buffer_object(ctx,
                                  &ctx->TransformFeedback.CurrentBuffer,
                                  bufObj);

    /* The per-attribute binding point */
    _mesa_set_transform_feedback_binding(ctx, obj, index, bufObj, offset, size);
}

/*** GL_ARB_direct_state_access ***/

extern void GLAPIENTRY
_mesa_TransformFeedbackBufferBase(GLuint xfb, GLuint index, GLuint buffer);

extern void GLAPIENTRY
_mesa_TransformFeedbackBufferRange(GLuint xfb, GLuint index, GLuint buffer,
                                   GLintptr offset, GLsizeiptr size);

extern void GLAPIENTRY
_mesa_GetTransformFeedbackiv(GLuint xfb, GLenum pname, GLint *param);

extern void GLAPIENTRY
_mesa_GetTransformFeedbacki_v(GLuint xfb, GLenum pname, GLuint index,
                              GLint *param);

extern void GLAPIENTRY
_mesa_GetTransformFeedbacki64_v(GLuint xfb, GLenum pname, GLuint index,
                                GLint64 *param);

#endif /* TRANSFORM_FEEDBACK_H */
