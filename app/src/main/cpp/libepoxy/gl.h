#ifndef EPOXY_GL_H
#define EPOXY_GL_H

#include "common.h"

#if defined(__gl_h_) || defined(__glext_h_)
#error epoxy/gl.h must be included before (or in place of) GL/gl.h
#else
#define __gl_h_
#define __glext_h_
#endif

#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1
#define KHRONOS_APIATTRIBUTES

#ifndef _WIN32
/* APIENTRY and GLAPIENTRY are not used on Linux or Mac. */
#define APIENTRY
#define GLAPIENTRY
#define EPOXY_CALLSPEC
#define GLAPI
#define KHRONOS_APIENTRY
#define KHRONOS_APICALL

#else
#ifndef APIENTRY
#define APIENTRY __stdcall
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY APIENTRY
#endif

#ifndef EPOXY_CALLSPEC
#define EPOXY_CALLSPEC __stdcall
#endif

#ifndef GLAPI
#define GLAPI extern
#endif

#define KHRONOS_APIENTRY __stdcall
#define KHRONOS_APICALL __declspec(dllimport) __stdcall

#endif /* _WIN32 */

#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif

#ifndef GLAPIENTRYP
#define GLAPIENTRYP GLAPIENTRY *
#endif

EPOXY_BEGIN_DECLS

#include "gl_generated.h"

        EPOXY_PUBLIC bool epoxy_has_gl_extension(const char *extension);
EPOXY_PUBLIC bool epoxy_is_desktop_gl(void);
EPOXY_PUBLIC int epoxy_gl_version(void);
EPOXY_PUBLIC int epoxy_glsl_version(void);

/*
 * the type of the stub function that the failure handler must return;
 * this function will be called on subsequent calls to the same bogus
 * function name
 */
typedef void (*epoxy_resolver_stub_t)(void);

/* the type of the failure handler itself */
typedef epoxy_resolver_stub_t
(*epoxy_resolver_failure_handler_t)(const char *name);

EPOXY_PUBLIC epoxy_resolver_failure_handler_t
epoxy_set_resolver_failure_handler(epoxy_resolver_failure_handler_t handler);

EPOXY_END_DECLS

#endif /* EPOXY_GL_H */