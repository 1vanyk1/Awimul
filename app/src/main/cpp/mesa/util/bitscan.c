#include "bitscan.h"

#ifdef HAVE___BUILTIN_FFS
#elif defined(_MSC_VER) && (_M_IX86 || _M_ARM || _M_AMD64 || _M_IA64)
#else
int
ffs(int i)
{
    int bit = 0;
    if (!i)
        return bit;
    if (!(i & 0xffff)) {
        bit += 16;
        i >>= 16;
    }
    if (!(i & 0xff)) {
        bit += 8;
        i >>= 8;
    }
    if (!(i & 0xf)) {
        bit += 4;
        i >>= 4;
    }
    if (!(i & 0x3)) {
        bit += 2;
        i >>= 2;
    }
    if (!(i & 0x1))
        bit += 1;
    return bit + 1;
}
#endif

#ifdef HAVE___BUILTIN_FFSLL
#elif defined(_MSC_VER) && (_M_AMD64 || _M_ARM64 || _M_IA64)
#else
int
ffsll(long long int val)
{
    int bit;

    bit = ffs((unsigned) (val & 0xffffffff));
    if (bit != 0)
        return bit;

    bit = ffs((unsigned) (val >> 32));
    if (bit != 0)
        return 32 + bit;

    return 0;
}
#endif
