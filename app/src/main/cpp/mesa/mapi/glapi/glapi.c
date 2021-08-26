#include "glapi.h"
#include "../u_current.h"

/*
 * Global variables, _glapi_get_context, and _glapi_get_dispatch are defined in
 * u_current.c.
 */

#ifdef USE_ELF_TLS
/* not used, but defined for compatibility */
const struct _glapi_table *_glapi_Dispatch;
const void *_glapi_Context;
#endif /* USE_ELF_TLS */
