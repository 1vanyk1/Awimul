#ifndef PROCESS_H
#define PROCESS_H

#include <stddef.h>

const char *
util_get_process_name(void);

size_t
util_get_process_exec_path(char* process_path, size_t len);

#endif
