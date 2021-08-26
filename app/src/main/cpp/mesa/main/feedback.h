#ifndef FEEDBACK_H
#define FEEDBACK_H


#include "mtypes.h"


void GLAPIENTRY
_mesa_FeedbackBuffer( GLsizei size, GLenum type, GLfloat *buffer );
void GLAPIENTRY
_mesa_PassThrough( GLfloat token );
void GLAPIENTRY
_mesa_SelectBuffer( GLsizei size, GLuint *buffer );
void GLAPIENTRY
_mesa_InitNames( void );
void GLAPIENTRY
_mesa_LoadName( GLuint name );
void GLAPIENTRY
_mesa_PushName( GLuint name );
void GLAPIENTRY
_mesa_PopName( void );
GLint GLAPIENTRY
_mesa_RenderMode( GLenum mode );

extern void
_mesa_feedback_vertex( struct gl_context *ctx,
                       const GLfloat win[4],
                       const GLfloat color[4],
                       const GLfloat texcoord[4] );


static inline void
_mesa_feedback_token( struct gl_context *ctx, GLfloat token )
{
    if (ctx->Feedback.Count < ctx->Feedback.BufferSize) {
        ctx->Feedback.Buffer[ctx->Feedback.Count] = token;
    }
    ctx->Feedback.Count++;
}


extern void
_mesa_update_hitflag( struct gl_context *ctx, GLfloat z );


extern void
_mesa_init_feedback( struct gl_context *ctx );

#endif /* FEEDBACK_H */
