#ifndef DD_PUBLIC_H_
#define DD_PUBLIC_H_

struct pipe_screen;

#ifdef __cplusplus
extern "C" {
#endif

struct pipe_screen *
ddebug_screen_create(struct pipe_screen *screen);

#ifdef __cplusplus
}
#endif

#endif /* DD_PUBLIC_H_ */
