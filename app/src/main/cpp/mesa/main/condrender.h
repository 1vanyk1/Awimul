#ifndef CONDRENDER_H
#define CONDRENDER_H


#include "glheader.h"
#include "context.h"


void GLAPIENTRY
_mesa_BeginConditionalRender_no_error(GLuint queryId, GLenum mode);

extern void GLAPIENTRY
_mesa_BeginConditionalRender(GLuint queryId, GLenum mode);

void APIENTRY
_mesa_EndConditionalRender_no_error(void);

extern void APIENTRY
_mesa_EndConditionalRender(void);

extern GLboolean
_mesa_check_conditional_render(struct gl_context *ctx);


#endif /* CONDRENDER_H */
