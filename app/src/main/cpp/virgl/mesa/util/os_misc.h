#ifndef _OS_MISC_H_
#define _OS_MISC_H_

#include <stdint.h>
#include <stdbool.h>

#include "detect_os.h"


#if DETECT_OS_UNIX
#  include <signal.h> /* for kill() */
#  include <unistd.h> /* getpid() */
#endif


#ifdef  __cplusplus
extern "C" {
#endif


/*
 * Trap into the debugger.
 */
#if (defined(PIPE_ARCH_X86) || defined(PIPE_ARCH_X86_64)) && defined(PIPE_CC_GCC)
#  define os_break() __asm("int3")
#elif defined(PIPE_CC_MSVC)
#  define os_break()  __debugbreak()
#elif DETECT_OS_UNIX
#  define os_break() kill(getpid(), SIGTRAP)
#else
#  define os_break() abort()
#endif


/*
 * Abort the program.
 */
#if defined(DEBUG)
#  define os_abort() do { os_break(); abort(); } while(0)
#else
#  define os_abort() abort()
#endif


/*
 * Output a message. Message should preferably end in a newline.
 */
void
os_log_message(const char *message);


/*
 * Get an option. Should return NULL if specified option is not set.
 */
const char *
os_get_option(const char *name);


/*
 * Get the total amount of physical memory available on the system.
 */
bool
os_get_total_physical_memory(uint64_t *size);

/*
 * Amount of physical memory available to a process
 */
bool
os_get_available_system_memory(uint64_t *size);

/*
 * Size of a page
 */
bool
os_get_page_size(uint64_t *size);


#ifdef	__cplusplus
}
#endif


#endif /* _OS_MISC_H_ */
