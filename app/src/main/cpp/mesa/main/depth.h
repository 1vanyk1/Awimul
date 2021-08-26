#ifndef DEPTH_H
#define DEPTH_H


#include "glheader.h"

struct gl_context;


extern void GLAPIENTRY
_mesa_ClearDepth( GLclampd depth );

extern void GLAPIENTRY
_mesa_ClearDepthf( GLclampf depth );

void GLAPIENTRY
_mesa_DepthFunc_no_error(GLenum func);

extern void GLAPIENTRY
_mesa_DepthFunc(GLenum func);

extern void GLAPIENTRY
_mesa_DepthMask( GLboolean flag );

extern void GLAPIENTRY
_mesa_DepthBoundsEXT( GLclampd zmin, GLclampd zmax );

extern void
_mesa_init_depth( struct gl_context * ctx );

#endif
