#ifndef EPOXY_COMMON_H
#define EPOXY_COMMON_H

#ifdef __cplusplus
# define EPOXY_BEGIN_DECLS      extern "C" {
# define EPOXY_END_DECLS        }
#else
# define EPOXY_BEGIN_DECLS
# define EPOXY_END_DECLS
#endif

#ifndef EPOXY_PUBLIC
# if defined(_MSC_VER)
#  define EPOXY_PUBLIC __declspec(dllimport) extern
# else
#  define EPOXY_PUBLIC extern
# endif
#endif

#if defined(_MSC_VER) && !defined(__bool_true_false_are_defined) && (_MSC_VER < 1800)
typedef unsigned char bool;
# define false 0
# define true 1
#else
# include <stdbool.h>
#endif

EPOXY_BEGIN_DECLS

EPOXY_PUBLIC bool epoxy_extension_in_string(const char *extension_list,
                                            const char *ext);

EPOXY_END_DECLS

#endif /* EPOXY_COMMON_H */