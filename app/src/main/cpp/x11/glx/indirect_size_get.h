#if !defined( _INDIRECT_SIZE_GET_H_ )
#define _INDIRECT_SIZE_GET_H_

/**
 * \file
 * Prototypes for functions used to determine the number of data elements in
 * various GLX protocol messages.
 *
 * \author Ian Romanick <idr@us.ibm.com>
 */

#include "../headers/xfuncproto.h"

#if defined(__GNUC__) || (defined(__SUNPRO_C) && (__SUNPRO_C >= 0x590))
#define PURE __attribute__((pure))
#else
#define PURE
#endif

#if defined(__i386__) && defined(__GNUC__) && !defined(__CYGWIN__) && !defined(__MINGW32__)
#define FASTCALL __attribute__((fastcall))
#else
#define FASTCALL
#endif

extern _X_INTERNAL PURE FASTCALL GLint __glGetBooleanv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetDoublev_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetFloatv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetIntegerv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetLightfv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetLightiv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetMaterialfv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetMaterialiv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetTexEnvfv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetTexEnviv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetTexGendv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetTexGenfv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetTexGeniv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetTexParameterfv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetTexParameteriv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetTexLevelParameterfv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetTexLevelParameteriv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetPointerv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint
__glGetColorTableParameterfv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint
__glGetColorTableParameteriv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint
__glGetConvolutionParameterfv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint
__glGetConvolutionParameteriv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetHistogramParameterfv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetHistogramParameteriv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetMinmaxParameterfv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetMinmaxParameteriv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetQueryObjectiv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetQueryObjectuiv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetQueryiv_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint __glGetProgramivARB_size(GLenum);
extern _X_INTERNAL PURE FASTCALL GLint
__glGetFramebufferAttachmentParameteriv_size(GLenum);

#undef PURE
#undef FASTCALL

#endif                          /* !defined( _INDIRECT_SIZE_GET_H_ ) */
