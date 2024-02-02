#ifndef U_DL_H_
#define U_DL_H_


#include "../include/pipe/p_config.h"

#ifdef	__cplusplus
extern "C" {
#endif

#if defined(PIPE_OS_WINDOWS)
#  define UTIL_DL_EXT ".dll"
#  define UTIL_DL_PREFIX ""
#elif defined(PIPE_OS_APPLE)
#  define UTIL_DL_EXT ".dylib"
#  define UTIL_DL_PREFIX "lib"
#else
#  define UTIL_DL_EXT ".so"
#  define UTIL_DL_PREFIX "lib"
#endif


struct util_dl_library;


typedef void (*util_dl_proc)(void);


/**
 * Open a library dynamically.
 */
struct util_dl_library *
util_dl_open(const char *filename);


/**
 * Lookup a function in a library.
 */
util_dl_proc
util_dl_get_proc_address(struct util_dl_library *library,
                         const char *procname);


/**
 * Close a library.
 */
void
util_dl_close(struct util_dl_library *library);


/**
 * Return most recent error message.
 */
const char *
util_dl_error(void);

#ifdef	__cplusplus
}
#endif

#endif /* U_DL_H_ */
