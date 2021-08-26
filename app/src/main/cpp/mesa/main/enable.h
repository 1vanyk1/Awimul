#ifndef ENABLE_H
#define ENABLE_H


#include "glheader.h"

struct gl_context;


extern void
_mesa_update_derived_primitive_restart_state(struct gl_context *ctx);

extern void
_mesa_set_enable( struct gl_context* ctx, GLenum cap, GLboolean state );

extern void GLAPIENTRY
_mesa_Disable( GLenum cap );

extern void GLAPIENTRY
_mesa_Enable( GLenum cap );

extern GLboolean GLAPIENTRY
_mesa_IsEnabled( GLenum cap );

extern void
_mesa_set_enablei(struct gl_context *ctx, GLenum cap, GLuint index, GLboolean state);

extern void GLAPIENTRY
_mesa_Disablei( GLenum cap, GLuint index );

extern void GLAPIENTRY
_mesa_Enablei( GLenum cap, GLuint index );

extern GLboolean GLAPIENTRY
_mesa_IsEnabledi( GLenum cap, GLuint index );

extern void GLAPIENTRY
_mesa_EnableClientState( GLenum cap );

extern void GLAPIENTRY
_mesa_EnableClientStateiEXT( GLenum cap, GLuint index );

extern void GLAPIENTRY
_mesa_EnableVertexArrayEXT( GLuint vaobj, GLenum cap );

extern void GLAPIENTRY
_mesa_DisableClientState( GLenum cap );

extern void GLAPIENTRY
_mesa_DisableClientStateiEXT( GLenum cap, GLuint index );

extern void GLAPIENTRY
_mesa_DisableVertexArrayEXT( GLuint vaobj, GLenum cap );

extern void
_mesa_set_multisample(struct gl_context *ctx, GLboolean state);

extern void
_mesa_set_framebuffer_srgb(struct gl_context *ctx, GLboolean state);



#endif
