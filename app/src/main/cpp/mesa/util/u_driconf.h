#ifndef U_DRICONF_H_
#define U_DRICONF_H_

#include "xmlconfig.h"
#include "../gallium/frontend/api.h"

#ifdef __cplusplus
extern "C" {
#endif

void
u_driconf_fill_st_options(struct st_config_options *options,
                          const struct driOptionCache *optionCache);

#ifdef __cplusplus
}
#endif

#endif /* U_DRICONF_H_ */
