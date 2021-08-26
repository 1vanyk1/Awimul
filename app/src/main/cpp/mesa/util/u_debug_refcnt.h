#ifndef U_DEBUG_REFCNT_H_
#define U_DEBUG_REFCNT_H_

#include "../include/pipe/p_config.h"
#include "../include/pipe/p_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*debug_reference_descriptor)(char*, const struct pipe_reference*);

#if defined(DEBUG)

extern int debug_refcnt_state;

void
debug_reference_slowpath(const struct pipe_reference* p,
                         debug_reference_descriptor get_desc, int change);

static inline void
debug_reference(const struct pipe_reference* p,
                debug_reference_descriptor get_desc, int change)
{
   if (debug_refcnt_state >= 0)
      debug_reference_slowpath(p, get_desc, change);
}

#else

static inline void
debug_reference(UNUSED const struct pipe_reference* p,
                UNUSED debug_reference_descriptor get_desc, UNUSED int change)
{
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* U_DEBUG_REFCNT_H_ */
