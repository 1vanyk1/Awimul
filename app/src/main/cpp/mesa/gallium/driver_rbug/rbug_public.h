#ifndef RBUG_PUBLIC_H
#define RBUG_PUBLIC_H

#ifdef __cplusplus
extern "C" {
#endif

struct pipe_screen;
struct pipe_context;

struct pipe_screen *
rbug_screen_create(struct pipe_screen *screen);

bool
rbug_enabled(void);

#ifdef __cplusplus
}
#endif

#endif /* RBUG_PUBLIC_H */
