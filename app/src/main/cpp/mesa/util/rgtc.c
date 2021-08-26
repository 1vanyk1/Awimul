#include <inttypes.h>
#include "macros.h"

#include "rgtc.h"

#define RGTC_DEBUG 0

#define TAG(x) util_format_unsigned_##x

#define TYPE unsigned char
#define T_MIN 0
#define T_MAX 0xff

#include "texcompress_rgtc_tmp.h"

#undef TAG
#undef TYPE
#undef T_MIN
#undef T_MAX

#define TAG(x) util_format_signed_##x
#define TYPE signed char
#define T_MIN (signed char)-128
#define T_MAX (signed char)127

#include "texcompress_rgtc_tmp.h"

#undef TAG
#undef TYPE
#undef T_MIN
#undef T_MAX

