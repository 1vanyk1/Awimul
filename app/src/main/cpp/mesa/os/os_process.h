#ifndef OS_PROCESS_H
#define OS_PROCESS_H


#include "../include/pipe/p_compiler.h"


extern boolean
os_get_process_name(char *str, size_t size);


extern boolean
os_get_command_line(char *cmdline, size_t size);


#endif /* OS_PROCESS_H */
