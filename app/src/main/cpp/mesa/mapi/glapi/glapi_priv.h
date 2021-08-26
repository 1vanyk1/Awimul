#ifndef _GLAPI_PRIV_H
#define _GLAPI_PRIV_H


#define GL_GLEXT_PROTOTYPES
#include "../../../GL/gl.h"
#include "../../../GL/glext.h"

typedef int GLclampx;

#include "glapi.h"


#ifdef __cplusplus
extern "C" {
#endif

/* entrypoint */

extern void
init_glapi_relocs_once(void);


extern _glapi_proc
generate_entrypoint(unsigned int functionOffset);


extern void
fill_in_entrypoint_offset(_glapi_proc entrypoint, unsigned int offset);


extern _glapi_proc
get_entrypoint_address(unsigned int functionOffset);


/**
 * Size (in bytes) of dispatch function (entrypoint).
 */
#if defined(USE_X86_ASM)
# if defined(USE_ELF_TLS)
#  define DISPATCH_FUNCTION_SIZE  16
# else
#  define DISPATCH_FUNCTION_SIZE  32
# endif
#endif

#if defined(USE_X64_64_ASM)
# if defined(USE_ELF_TLS)
#  define DISPATCH_FUNCTION_SIZE  16
# endif
#endif


/**
 * Number of extension functions which we can dynamically add at runtime.
 *
 * Number of extension functions is also subject to the size of backing exec
 * mem we allocate. For the common case of dispatch stubs with size 16 bytes,
 * the two limits will be hit simultaneously. For larger dispatch function
 * sizes, MAX_EXTENSION_FUNCS is effectively reduced.
 */
#define MAX_EXTENSION_FUNCS 256


#ifdef __cplusplus
}
#endif

#endif
