#ifndef DRIVERFUNCS_H
#define DRIVERFUNCS_H

#ifdef __cplusplus
extern "C" {
#endif

extern void
_mesa_init_driver_functions(struct dd_function_table *driver);


extern void
_mesa_init_driver_state(struct gl_context *ctx);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
