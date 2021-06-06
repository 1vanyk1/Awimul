#ifndef _XFUNCS_H_
# define _XFUNCS_H_
# include "xosdefs.h"
/* the old Xfuncs.h, for pre-R6 */
# if !(defined(XFree86LOADER) && defined(IN_MODULE))
#  ifdef X_USEBFUNCS
void bcopy();
void bzero();
int bcmp();
#  else
#   if defined(SYSV) && !defined(__SCO__) && !defined(__sun) && !defined(__UNIXWARE__) && !defined(_AIX)
#    include <memory.h>
void bcopy();
#    define bzero(b,len) memset(b, 0, len)
#    define bcmp(b1,b2,len) memcmp(b1, b2, len)
#   else
#    include "../../../../../../../SDK/ndk/21.3.6528147/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/string.h"
#    if defined(__SCO__) || defined(__sun) || defined(__UNIXWARE__) || defined(__CYGWIN__) || defined(_AIX) || defined(__APPLE__)
#     include <strings.h>
#    endif
#    define _XFUNCS_H_INCLUDED_STRING_H
#   endif
#  endif /* X_USEBFUNCS */
/* the new Xfuncs.h */
/* the ANSI C way */
#  ifndef _XFUNCS_H_INCLUDED_STRING_H
#   include <string.h>
#  endif
#  undef bzero
#  define bzero(b,len) memset(b,0,len)
#  if defined WIN32 && defined __MINGW32__
#   define bcopy(b1,b2,len) memmove(b2, b1, (size_t)(len))
#  endif
# endif /* !(defined(XFree86LOADER) && defined(IN_MODULE)) */
#endif /* _XFUNCS_H_ */