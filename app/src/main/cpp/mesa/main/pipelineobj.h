#ifndef PIPELINEOBJ_H
#define PIPELINEOBJ_H

#include "glheader.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _glapi_table;
struct gl_context;
struct gl_pipeline_object;

extern void
_mesa_delete_pipeline_object(struct gl_context *ctx, struct gl_pipeline_object *obj);

extern void
_mesa_init_pipeline(struct gl_context *ctx);

extern void
_mesa_free_pipeline_data(struct gl_context *ctx);

extern struct gl_pipeline_object *
_mesa_lookup_pipeline_object(struct gl_context *ctx, GLuint id);

extern void
_mesa_reference_pipeline_object_(struct gl_context *ctx,
                                 struct gl_pipeline_object **ptr,
                                 struct gl_pipeline_object *obj);

static inline void
_mesa_reference_pipeline_object(struct gl_context *ctx,
                                struct gl_pipeline_object **ptr,
                                struct gl_pipeline_object *obj)
{
    if (*ptr != obj)
        _mesa_reference_pipeline_object_(ctx, ptr, obj);
}

extern void
_mesa_bind_pipeline(struct gl_context *ctx,
                    struct gl_pipeline_object *pipe);

extern GLboolean
_mesa_validate_program_pipeline(struct gl_context * ctx,
                                struct gl_pipeline_object *pipe);


void GLAPIENTRY
_mesa_UseProgramStages_no_error(GLuint pipeline, GLbitfield stages,
                                GLuint prog);
extern void GLAPIENTRY
_mesa_UseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program);

void GLAPIENTRY
_mesa_ActiveShaderProgram_no_error(GLuint pipeline, GLuint program);
extern void GLAPIENTRY
_mesa_ActiveShaderProgram(GLuint pipeline, GLuint program);

void GLAPIENTRY
_mesa_BindProgramPipeline_no_error(GLuint pipeline);
extern void GLAPIENTRY
_mesa_BindProgramPipeline(GLuint pipeline);

extern void GLAPIENTRY
_mesa_DeleteProgramPipelines(GLsizei n, const GLuint *pipelines);

void GLAPIENTRY
_mesa_GenProgramPipelines_no_error(GLsizei n, GLuint *pipelines);

extern void GLAPIENTRY
_mesa_GenProgramPipelines(GLsizei n, GLuint *pipelines);

void GLAPIENTRY
_mesa_CreateProgramPipelines_no_error(GLsizei n, GLuint *pipelines);

void GLAPIENTRY
_mesa_CreateProgramPipelines(GLsizei n, GLuint *pipelines);

extern GLboolean GLAPIENTRY
_mesa_IsProgramPipeline(GLuint pipeline);

extern void GLAPIENTRY
_mesa_GetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint *params);

extern void GLAPIENTRY
_mesa_ValidateProgramPipeline(GLuint pipeline);

extern void GLAPIENTRY
_mesa_GetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize,
                                GLsizei *length, GLchar *infoLog);

#ifdef __cplusplus
}
#endif

#endif /* PIPELINEOBJ_H */
