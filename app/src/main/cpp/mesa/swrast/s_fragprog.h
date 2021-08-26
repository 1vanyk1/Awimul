#ifndef S_FRAGPROG_H
#define S_FRAGPROG_H


#include "s_span.h"

struct gl_context;

GLboolean
_swrast_use_fragment_program(struct gl_context *ctx);

extern void
_swrast_exec_fragment_program(struct gl_context *ctx, SWspan *span);


#endif /* S_FRAGPROG_H */

