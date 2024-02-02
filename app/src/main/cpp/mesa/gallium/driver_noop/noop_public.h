#ifndef NOOP_PUBLIC_H
#define NOOP_PUBLIC_H

#ifdef __cplusplus
extern "C" {
#endif

struct pipe_screen;
struct pipe_screen *noop_screen_create(struct pipe_screen *screen);

#ifdef __cplusplus
}
#endif

#endif
