#ifndef U_FILE_H_
#define U_FILE_H_

#if defined(PIPE_OS_UNIX)
#include <unistd.h>
#endif
#if defined(PIPE_OS_WINDOWS)
#include <io.h>
#endif

#ifdef	__cplusplus
extern "C" {
#endif

static inline int
u_file_access(const char *path, int mode) {
#if defined(PIPE_OS_UNIX)
    return access(path, mode);
#elif defined(PIPE_OS_WINDOWS)
    return _access(path, mode);
#else
    return 0;
#endif
}

#ifdef	__cplusplus
}
#endif

#endif /* U_FILE_H_ */
