#if defined(__WINE_PSHPACK_H15)

/* Depth > 15 */
#  error "Alignment nesting > 15 is not supported"

#else

#  if !defined(__WINE_PSHPACK_H)
#    define __WINE_PSHPACK_H  2
/* Depth == 1 */
#  elif !defined(__WINE_PSHPACK_H2)
#    define __WINE_PSHPACK_H2 2
     /* Depth == 2 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H3)
#    define __WINE_PSHPACK_H3 2
     /* Depth == 3 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H4)
#    define __WINE_PSHPACK_H4 2
     /* Depth == 4 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H5)
#    define __WINE_PSHPACK_H5 2
     /* Depth == 5 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H6)
#    define __WINE_PSHPACK_H6 2
     /* Depth == 6 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H7)
#    define __WINE_PSHPACK_H7 2
     /* Depth == 7 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H8)
#    define __WINE_PSHPACK_H8 2
     /* Depth == 8 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H9)
#    define __WINE_PSHPACK_H9 2
     /* Depth == 9 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H10)
#    define __WINE_PSHPACK_H10 2
     /* Depth == 10 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H11)
#    define __WINE_PSHPACK_H11 2
     /* Depth == 11 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H12)
#    define __WINE_PSHPACK_H12 2
     /* Depth == 12 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H13)
#    define __WINE_PSHPACK_H13 2
     /* Depth == 13 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H14)
#    define __WINE_PSHPACK_H14 2
     /* Depth == 14 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H15)
#    define __WINE_PSHPACK_H15 2
     /* Depth == 15 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  endif

#  if defined(_MSC_VER) && (_MSC_VER >= 800)
#   pragma warning(disable:4103)
#  endif

#  pragma pack(2)

#endif