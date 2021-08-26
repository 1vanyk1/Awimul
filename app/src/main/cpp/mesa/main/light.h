#ifndef LIGHT_H
#define LIGHT_H


#include <stdbool.h>
#include "glheader.h"

struct gl_context;
struct gl_light;
struct gl_material;

extern void GLAPIENTRY
_mesa_ShadeModel( GLenum mode );

extern void GLAPIENTRY
_mesa_ProvokingVertex(GLenum mode);


extern void GLAPIENTRY
_mesa_ColorMaterial( GLenum face, GLenum mode );

extern void GLAPIENTRY
_mesa_Lightf( GLenum light, GLenum pname, GLfloat param );

extern void GLAPIENTRY
_mesa_Lightfv( GLenum light, GLenum pname, const GLfloat *params );

extern void GLAPIENTRY
_mesa_Lightiv( GLenum light, GLenum pname, const GLint *params );

extern void GLAPIENTRY
_mesa_Lighti( GLenum light, GLenum pname, GLint param );

extern void GLAPIENTRY
_mesa_LightModelf( GLenum pname, GLfloat param );

extern void GLAPIENTRY
_mesa_LightModelfv( GLenum pname, const GLfloat *params );

extern void GLAPIENTRY
_mesa_LightModeli( GLenum pname, GLint param );

extern void GLAPIENTRY
_mesa_LightModeliv( GLenum pname, const GLint *params );

extern void GLAPIENTRY
_mesa_GetLightfv( GLenum light, GLenum pname, GLfloat *params );

extern void GLAPIENTRY
_mesa_GetLightiv( GLenum light, GLenum pname, GLint *params );

extern void GLAPIENTRY
_mesa_GetMaterialfv( GLenum face, GLenum pname, GLfloat *params );

extern void GLAPIENTRY
_mesa_GetMaterialiv( GLenum face, GLenum pname, GLint *params );


extern void
_mesa_light(struct gl_context *ctx, GLuint lnum, GLenum pname, const GLfloat *params);


extern GLuint _mesa_material_bitmask( struct gl_context *ctx,
                                      GLenum face, GLenum pname,
                                      GLuint legal,
                                      const char * );

extern GLbitfield _mesa_update_lighting( struct gl_context *ctx );
extern void _mesa_update_light_materials(struct gl_context *ctx);
extern bool _mesa_update_tnl_spaces( struct gl_context *ctx, GLuint new_state );

extern void _mesa_update_material( struct gl_context *ctx,
                                   GLuint bitmask );

extern void _mesa_update_color_material( struct gl_context *ctx,
                                         const GLfloat rgba[4] );

extern void _mesa_init_lighting( struct gl_context *ctx );

extern void _mesa_allow_light_in_model( struct gl_context *ctx, GLboolean flag );

#endif
