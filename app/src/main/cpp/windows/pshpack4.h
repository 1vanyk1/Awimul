#if defined(__WINE_PSHPACK_H15)

/* Depth > 15 */
#  error "Alignment nesting > 15 is not supported"

#else

#  if !defined(__WINE_PSHPACK_H)
#    define __WINE_PSHPACK_H  4
/* Depth == 1 */
#  elif !defined(__WINE_PSHPACK_H2)
#    define __WINE_PSHPACK_H2 4
     /* Depth == 2 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H3)
#    define __WINE_PSHPACK_H3 4
     /* Depth == 3 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H4)
#    define __WINE_PSHPACK_H4 4
     /* Depth == 4 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H5)
#    define __WINE_PSHPACK_H5 4
     /* Depth == 5 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H6)
#    define __WINE_PSHPACK_H6 4
     /* Depth == 6 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H7)
#    define __WINE_PSHPACK_H7 4
     /* Depth == 7 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H8)
#    define __WINE_PSHPACK_H8 4
     /* Depth == 8 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H9)
#    define __WINE_PSHPACK_H9 4
     /* Depth == 9 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H10)
#    define __WINE_PSHPACK_H10 4
     /* Depth == 10 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H11)
#    define __WINE_PSHPACK_H11 4
     /* Depth == 11 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H12)
#    define __WINE_PSHPACK_H12 4
     /* Depth == 12 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H13)
#    define __WINE_PSHPACK_H13 4
     /* Depth == 13 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H14)
#    define __WINE_PSHPACK_H14 4
     /* Depth == 14 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H15)
#    define __WINE_PSHPACK_H15 4
     /* Depth == 15 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  endif

#  if defined(_MSC_VER) && (_MSC_VER >= 800)
#   pragma warning(disable:4103)
#  endif

#  pragma pack(4)

#endif