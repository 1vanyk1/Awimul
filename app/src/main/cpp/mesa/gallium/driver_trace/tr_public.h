#ifndef TR_PUBLIC_H
#define TR_PUBLIC_H

#include "../../include/pipe/p_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

struct pipe_screen;
struct pipe_context;

struct pipe_screen *
trace_screen_create(struct pipe_screen *screen);

bool
trace_enabled(void);

#ifdef __cplusplus
}
#endif

#endif /* TR_PUBLIC_H */
