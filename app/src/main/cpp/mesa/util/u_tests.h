#ifndef U_TESTS_H
#define U_TESTS_H

#include "../include/pipe/p_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

struct pipe_screen;
struct pipe_context;
struct pipe_resource;

void util_test_constant_buffer(struct pipe_context *ctx,
                               struct pipe_resource *constbuf);
void util_run_tests(struct pipe_screen *screen);

#ifdef __cplusplus
}
#endif

#endif
