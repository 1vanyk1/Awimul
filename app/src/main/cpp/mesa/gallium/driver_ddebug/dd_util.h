#ifndef DD_UTIL_H
#define DD_UTIL_H

#include <stdio.h>
#include <errno.h>

#include "../../include/c99_alloca.h"
#include "../../os/os_process.h"
#include "../../util/u_atomic.h"
#include "../../util/u_debug.h"
#include "../../util/u_string.h"

#include "../../include/pipe/p_config.h"
#if defined(PIPE_OS_UNIX)
#include <unistd.h>
#include <sys/stat.h>
#elif defined(PIPE_OS_WINDOWS)
#include <direct.h>
#include <process.h>
#define mkdir(dir, mode) _mkdir(dir)
#endif

struct pipe_screen;

/* name of the directory in home */
#define DD_DIR "ddebug_dumps"

void
dd_get_debug_filename_and_mkdir(char *buf, size_t buflen, bool verbose);

FILE *
dd_get_debug_file(bool verbose);

void
dd_parse_apitrace_marker(const char *string, int len, unsigned *call_number);

void
dd_write_header(FILE *f, struct pipe_screen *screen, unsigned apitrace_call_number);

#endif /* DD_UTIL_H */
