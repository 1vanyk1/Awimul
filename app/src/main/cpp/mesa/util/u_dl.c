#include "../include/pipe/p_config.h"
#include "../include/pipe/p_compiler.h"

#if defined(PIPE_OS_UNIX)
#include <dlfcn.h>
#endif
#if defined(PIPE_OS_WINDOWS)
#include <windows.h>
#endif

#include "u_dl.h"
#include "u_pointer.h"


struct util_dl_library *
util_dl_open(const char *filename)
{
#if defined(PIPE_OS_UNIX)
    return (struct util_dl_library *)dlopen(filename, RTLD_LAZY | RTLD_LOCAL);
#elif defined(PIPE_OS_WINDOWS)
    return (struct util_dl_library *)LoadLibraryA(filename);
#else
    return NULL;
#endif
}


util_dl_proc
util_dl_get_proc_address(struct util_dl_library *library,
                         const char *procname)
{
#if defined(PIPE_OS_UNIX)
    return (util_dl_proc) pointer_to_func(dlsym((void *)library, procname));
#elif defined(PIPE_OS_WINDOWS)
    return (util_dl_proc)GetProcAddress((HMODULE)library, procname);
#else
    return (util_dl_proc)NULL;
#endif
}


void
util_dl_close(struct util_dl_library *library)
{
#if defined(PIPE_OS_UNIX)
    dlclose((void *)library);
#elif defined(PIPE_OS_WINDOWS)
    FreeLibrary((HMODULE)library);
#else
    (void)library;
#endif
}


const char *
util_dl_error(void)
{
#if defined(PIPE_OS_UNIX)
    return dlerror();
#elif defined(PIPE_OS_WINDOWS)
    return "unknown error";
#else
    return "unknown error";
#endif
}
