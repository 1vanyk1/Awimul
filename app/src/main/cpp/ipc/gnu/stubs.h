#include "../bits/wordsize.h"

#if __WORDSIZE == 32
# include "stubs-32.h"
#elif __WORDSIZE == 64
# include "stubs-64.h"
#else
# error "unexpected value for __WORDSIZE macro"
#endif