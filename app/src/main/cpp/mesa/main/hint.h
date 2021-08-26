#ifndef HINT_H
#define HINT_H

#include "glheader.h"

struct gl_context;

extern void GLAPIENTRY
_mesa_Hint( GLenum target, GLenum mode );

/* GL_KHR_parallel_shader_compile */
extern void GLAPIENTRY
_mesa_MaxShaderCompilerThreadsKHR(GLuint count);

extern void
_mesa_init_hint( struct gl_context * ctx );

#endif
