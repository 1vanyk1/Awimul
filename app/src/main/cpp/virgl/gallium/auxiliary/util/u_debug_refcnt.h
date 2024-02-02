#ifndef U_DEBUG_REFCNT_H_
#define U_DEBUG_REFCNT_H_

#include "../../../mesa/pipe/p_config.h"
#include "../../include/pipe/p_state.h"

#include "../../../mesa/util/u_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*debug_reference_descriptor)(char*, const struct pipe_reference*);

static inline void
debug_reference(UNUSED const struct pipe_reference* p,
                UNUSED debug_reference_descriptor get_desc, UNUSED int change)
{
}

#ifdef __cplusplus
}
#endif

#endif /* U_DEBUG_REFCNT_H_ */
