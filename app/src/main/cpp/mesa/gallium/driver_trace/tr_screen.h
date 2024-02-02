#ifndef TR_SCREEN_H_
#define TR_SCREEN_H_


#include "../../include/pipe/p_screen.h"
#include "../../os/os_thread.h"
#include "../../util/u_threaded_context.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * It often happens that new data is written directly to the user buffers
 * without mapping/unmapping. This flag marks user buffers, so that their
 * contents can be dumped before being used by the pipe context.
 */
#define TRACE_FLAG_USER_BUFFER  (1 << 31)


struct trace_screen
{
    struct pipe_screen base;

    struct pipe_screen *screen;
    tc_is_resource_busy is_resource_busy;
    bool trace_tc;
};


struct trace_screen *
trace_screen(struct pipe_screen *screen);


#ifdef __cplusplus
}
#endif

#endif /* TR_SCREEN_H_ */
