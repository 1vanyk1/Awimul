#ifndef ATTRIB_H
#define ATTRIB_H


#include "glheader.h"

struct _glapi_table;
struct gl_context;

extern void GLAPIENTRY
_mesa_PushAttrib( GLbitfield mask );

extern void GLAPIENTRY
_mesa_PopAttrib( void );

extern void GLAPIENTRY
_mesa_PushClientAttrib( GLbitfield mask );

extern void GLAPIENTRY
_mesa_PopClientAttrib( void );

extern void GLAPIENTRY
_mesa_ClientAttribDefaultEXT( GLbitfield mask );

extern void GLAPIENTRY
_mesa_PushClientAttribDefaultEXT( GLbitfield mask );

extern void
_mesa_init_attrib( struct gl_context *ctx );

extern void
_mesa_free_attrib_data( struct gl_context *ctx );

#endif /* ATTRIB_H */
