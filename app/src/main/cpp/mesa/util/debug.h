#ifndef _UTIL_DEBUG_H
#define _UTIL_DEBUG_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct debug_control {
    const char * string;
    uint64_t     flag;
};

uint64_t
parse_debug_string(const char *debug,
                   const struct debug_control *control);
bool
comma_separated_list_contains(const char *list, const char *s);
bool
env_var_as_boolean(const char *var_name, bool default_value);
unsigned
env_var_as_unsigned(const char *var_name, unsigned default_value);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UTIL_DEBUG_H */
