#if defined(__WINE_PSHPACK_H15)

/* Depth > 15 */
#  error "Alignment nesting > 15 is not supported"

#else

#  if !defined(__WINE_PSHPACK_H)
#    define __WINE_PSHPACK_H  8
/* Depth == 1 */
#  elif !defined(__WINE_PSHPACK_H2)
#    define __WINE_PSHPACK_H2 8
     /* Depth == 2 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H3)
#    define __WINE_PSHPACK_H3 8
     /* Depth == 3 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H4)
#    define __WINE_PSHPACK_H4 8
     /* Depth == 4 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H5)
#    define __WINE_PSHPACK_H5 8
     /* Depth == 5 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H6)
#    define __WINE_PSHPACK_H6 8
     /* Depth == 6 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H7)
#    define __WINE_PSHPACK_H7 8
     /* Depth == 7 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H8)
#    define __WINE_PSHPACK_H8 8
     /* Depth == 8 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H9)
#    define __WINE_PSHPACK_H9 8
     /* Depth == 9 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H10)
#    define __WINE_PSHPACK_H10 8
     /* Depth == 10 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H11)
#    define __WINE_PSHPACK_H11 8
     /* Depth == 11 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H12)
#    define __WINE_PSHPACK_H12 8
     /* Depth == 12 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H13)
#    define __WINE_PSHPACK_H13 8
     /* Depth == 13 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H14)
#    define __WINE_PSHPACK_H14 8
     /* Depth == 14 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  elif !defined(__WINE_PSHPACK_H15)
#    define __WINE_PSHPACK_H15 8
     /* Depth == 15 */
#    define __WINE_INTERNAL_POPPACK
#    include "poppack.h"
#  endif

#  if defined(_MSC_VER) && (_MSC_VER >= 800)
#   pragma warning(disable:4103)
#  endif

#  pragma pack(8)

#endif