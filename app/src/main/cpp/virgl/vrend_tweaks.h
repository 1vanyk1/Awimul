#ifndef VREND_TWEAKS_H
#define VREND_TWEAKS_H

#include "virgl_protocol.h"

#include <stdint.h>
#include <stdbool.h>

struct vrend_context_tweaks {
    uint32_t active_tweaks;
    int32_t tf3_samples_passed_factor;
};

bool vrend_get_tweak_is_active(struct vrend_context_tweaks *ctx,
                               enum vrend_tweak_type t);

bool vrend_get_tweak_is_active_with_params(struct vrend_context_tweaks *ctx,
                                           enum vrend_tweak_type t, void *params);

void vrend_set_active_tweaks(struct vrend_context_tweaks *ctx, uint32_t tweak_id, uint32_t tweak_value);


void vrend_set_tweak_from_env(struct vrend_context_tweaks *ctx);

#endif // VREND_TWEAKS_H
