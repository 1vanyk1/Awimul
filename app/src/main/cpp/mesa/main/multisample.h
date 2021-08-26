#ifndef MULTISAMPLE_H
#define MULTISAMPLE_H

#include "glheader.h"

struct gl_context;

extern void GLAPIENTRY
_mesa_SampleCoverage(GLclampf value, GLboolean invert);


extern void
_mesa_init_multisample(struct gl_context *ctx);


extern void GLAPIENTRY
_mesa_GetMultisamplefv(GLenum pname, GLuint index, GLfloat* val);

void GLAPIENTRY
_mesa_SampleMaski_no_error(GLuint index, GLbitfield mask);

extern void GLAPIENTRY
_mesa_SampleMaski(GLuint index, GLbitfield mask);

void GLAPIENTRY
_mesa_MinSampleShading_no_error(GLclampf value);

extern void GLAPIENTRY
_mesa_MinSampleShading(GLclampf value);

extern GLenum
_mesa_check_sample_count(struct gl_context *ctx, GLenum target,
                         GLenum internalFormat, GLsizei samples,
                         GLsizei storageSamples);

extern void GLAPIENTRY
_mesa_AlphaToCoverageDitherControlNV_no_error(GLenum mode);

extern void GLAPIENTRY
_mesa_AlphaToCoverageDitherControlNV(GLenum mode);

#endif
