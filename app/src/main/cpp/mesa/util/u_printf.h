#ifndef U_PRINTF_H
#define U_PRINTF_H

#ifdef __cplusplus

#include <string>

/* find next valid printf specifier in a C++ std::string */
size_t util_printf_next_spec_pos(const std::string &s, size_t pos);

extern "C" {
#endif

/* find next valid printf specifier in a C string wrapper */
size_t util_printf_next_spec_pos(const char *str, size_t pos);
#ifdef __cplusplus
}
#endif

#endif
