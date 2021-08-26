#ifndef _C99_ALLOCA_H_
#define _C99_ALLOCA_H_


#if defined(_MSC_VER)

#  include <malloc.h>

#  define alloca _alloca

#elif defined(__sun) || defined(__CYGWIN__)

#  include <alloca.h>

#else /* !defined(_MSC_VER) */

#  include <stdlib.h>

#endif /* !defined(_MSC_VER) */


#endif
