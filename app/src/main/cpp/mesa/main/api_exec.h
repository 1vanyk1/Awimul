#ifndef API_EXEC_H
#define API_EXEC_H

#ifdef __cplusplus
extern "C" {
#endif

struct _glapi_table;
struct gl_context;

extern struct _glapi_table *
_mesa_alloc_dispatch_table(void);

extern void
_mesa_initialize_exec_table(struct gl_context *ctx);

extern void
_mesa_initialize_dispatch_tables(struct gl_context *ctx);

extern struct _glapi_table *
_mesa_new_nop_table(unsigned numEntries);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
