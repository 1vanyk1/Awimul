#ifndef T_VP_BUILD_H
#define T_VP_BUILD_H

struct gl_context;

#define TNL_FIXED_FUNCTION_STATE_FLAGS (_NEW_PROGRAM |		\
					_NEW_LIGHT |		\
					_NEW_TEXTURE |		\
					_NEW_TEXTURE_MATRIX |		\
					_NEW_TRANSFORM |	\
					_NEW_FOG |		\
					_NEW_POINT)

extern void _tnl_UpdateFixedFunctionProgram( struct gl_context *ctx );

#endif
