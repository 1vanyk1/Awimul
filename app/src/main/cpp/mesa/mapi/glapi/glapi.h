#ifndef _GLAPI_H
#define _GLAPI_H

#include "../../util/macros.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifdef _GLAPI_NO_EXPORTS
#  define _GLAPI_EXPORT
#else /* _GLAPI_NO_EXPORTS */
#  ifdef _WIN32
#    ifdef _GLAPI_DLL_EXPORTS
#      define _GLAPI_EXPORT __declspec(dllexport)
#    else
#      define _GLAPI_EXPORT __declspec(dllimport)
#    endif
#  elif defined(__GNUC__)
#    define _GLAPI_EXPORT __attribute__((visibility("default")))
#  else
#    define _GLAPI_EXPORT
#  endif
#endif /* _GLAPI_NO_EXPORTS */


typedef void (*_glapi_proc)(void);

typedef void (*_glapi_nop_handler_proc)(const char *name);

struct _glapi_table;


//#if defined (USE_ELF_TLS)
//
//#ifdef _MSC_VER
//extern __declspec(thread) struct _glapi_table * _glapi_tls_Dispatch;
//extern __declspec(thread) void * _glapi_tls_Context;
//#else
//_GLAPI_EXPORT extern __thread struct _glapi_table * _glapi_tls_Dispatch
//    __attribute__((tls_model("initial-exec")));
//
//_GLAPI_EXPORT extern __thread void * _glapi_tls_Context
//    __attribute__((tls_model("initial-exec")));
//#endif
//
//_GLAPI_EXPORT extern const struct _glapi_table *_glapi_Dispatch;
//_GLAPI_EXPORT extern const void *_glapi_Context;
//
//# define GET_DISPATCH() _glapi_tls_Dispatch
//# define GET_CURRENT_CONTEXT(C)  struct gl_context *C = (struct gl_context *) _glapi_tls_Context
//
//#else

_GLAPI_EXPORT extern struct _glapi_table *_glapi_Dispatch;
_GLAPI_EXPORT extern void *_glapi_Context;

#define GET_DISPATCH() \
     (likely(_glapi_Dispatch) ? _glapi_Dispatch : _glapi_get_dispatch())

#define GET_CURRENT_CONTEXT(C)  struct gl_context *C = (struct gl_context *) \
     (likely(_glapi_Context) ? _glapi_Context : _glapi_get_context())

//#endif /* defined (USE_ELF_TLS) */


_GLAPI_EXPORT void
_glapi_destroy_multithread(void);


_GLAPI_EXPORT void
_glapi_check_multithread(void);


_GLAPI_EXPORT void
_glapi_set_context(void *context);


_GLAPI_EXPORT void *
_glapi_get_context(void);


_GLAPI_EXPORT void
_glapi_set_dispatch(struct _glapi_table *dispatch);


_GLAPI_EXPORT struct _glapi_table *
_glapi_get_dispatch(void);


_GLAPI_EXPORT unsigned int
_glapi_get_dispatch_table_size(void);


_GLAPI_EXPORT int
_glapi_add_dispatch( const char * const * function_names,
                     const char * parameter_signature );

_GLAPI_EXPORT int
_glapi_get_proc_offset(const char *funcName);


_GLAPI_EXPORT _glapi_proc
_glapi_get_proc_address(const char *funcName);


_GLAPI_EXPORT const char *
_glapi_get_proc_name(unsigned int offset);


#if defined(GLX_USE_APPLEGL) || defined(GLX_USE_WINDOWSGL)
_GLAPI_EXPORT struct _glapi_table *
_glapi_create_table_from_handle(void *handle, const char *symbol_prefix);

_GLAPI_EXPORT void
_glapi_table_patch(struct _glapi_table *, const char *name, void *wrapper);
#endif


_GLAPI_EXPORT void
_glapi_set_nop_handler(_glapi_nop_handler_proc func);

/** Return pointer to new dispatch table filled with no-op functions */
_GLAPI_EXPORT struct _glapi_table *
_glapi_new_nop_table(unsigned num_entries);


/** Deprecated function */
_GLAPI_EXPORT unsigned long
_glthread_GetID(void);


/*
 * These stubs are kept so that the old DRI drivers still load.
 */
_GLAPI_EXPORT void
_glapi_noop_enable_warnings(unsigned char enable);


_GLAPI_EXPORT void
_glapi_set_warning_func(_glapi_proc func);


#ifdef __cplusplus
}
#endif

#endif /* _GLAPI_H */
