#ifndef U_DEBUG_STACK_H_
#define U_DEBUG_STACK_H_

#include <stdio.h>

#ifdef HAVE_LIBUNWIND
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#endif

/**
 * @file
 * Stack backtracing.
 *
 * @author Jose Fonseca <jfonseca@vmware.com>
 */


#ifdef	__cplusplus
extern "C" {
#endif


/**
 * Represent a frame from a stack backtrace.
 *
#if defined(PIPE_OS_WINDOWS) && !defined(HAVE_LIBUNWIND)
 * XXX: Do not change this. (passed to Windows' CaptureStackBackTrace())
#endif
 *
 * TODO: This should be refactored as a void * typedef.
 */
struct debug_stack_frame
{
#if defined(HAVE_ANDROID_PLATFORM) || defined(HAVE_LIBUNWIND)
    const char *procname;
   uint64_t start_ip;
   unsigned off;
   const char *map;
   unsigned int map_off;
#else
    const void *function;
#endif
};


void
debug_backtrace_capture(struct debug_stack_frame *backtrace,
                        unsigned start_frame,
                        unsigned nr_frames);

void
debug_backtrace_dump(const struct debug_stack_frame *backtrace,
                     unsigned nr_frames);

void
debug_backtrace_print(FILE *f,
                      const struct debug_stack_frame *backtrace,
                      unsigned nr_frames);

#ifdef	__cplusplus
}
#endif

#endif /* U_DEBUG_STACK_H_ */
