#ifndef _XOS_H_
# define _XOS_H_

# include "xosdefs.h"

/*
 * Get major data types (esp. caddr_t)
 */

# include <sys/types.h>

# if defined(__SCO__) || defined(__UNIXWARE__)
#  include <stdint.h>
# endif


/*
 * Just about everyone needs the strings routines.  We provide both forms here,
 * index/rindex and strchr/strrchr, so any systems that don't provide them all
 * need to have #defines here.
 *
 * These macros are defined this way, rather than, e.g.:
 *    #defined index(s,c) strchr(s,c)
 * because someone might be using them as function pointers, and such
 * a change would break compatibility for anyone who's relying on them
 * being the way they currently are. So we're stuck with them this way,
 * which can be really inconvenient. :-(
 */

# include <string.h>
# if defined(__SCO__) || defined(__UNIXWARE__) || defined(__sun)
#  include <strings.h>
# else
#  ifndef index
#   define index(s,c) (strchr((s),(c)))
#  endif
#  ifndef rindex
#   define rindex(s,c) (strrchr((s),(c)))
#  endif
# endif

/*
 * Get open(2) constants
 */
# if defined(X_NOT_POSIX)
#  include <fcntl.h>
#  if defined(USL) || defined(__i386__) && (defined(SYSV) || defined(SVR4))
#   include <unistd.h>
#  endif
#  ifdef WIN32
#   include "xw32defs.h"
#  else
#   include <sys/file.h>
#  endif
# else /* X_NOT_POSIX */
#  include <fcntl.h>
#  include <unistd.h>
# endif /* X_NOT_POSIX else */

/*
 * Get struct timeval and struct tm
 */

# if defined(_POSIX_SOURCE) && defined(SVR4)
/* need to omit _POSIX_SOURCE in order to get what we want in SVR4 */
#  undef _POSIX_SOURCE
#  include <sys/time.h>
#  define _POSIX_SOURCE
# elif defined(WIN32)
#  include <time.h>
#  if !defined(_WINSOCKAPI_) && !defined(_WILLWINSOCK_) && !defined(_TIMEVAL_DEFINED) && !defined(_STRUCT_TIMEVAL)
struct timeval {
    long    tv_sec;         /* seconds */
    long    tv_usec;        /* and microseconds */
};
#   define _TIMEVAL_DEFINED
#  endif
#  include <sys/timeb.h>
#  define gettimeofday(t) \
{ \
    struct _timeb _gtodtmp; \
    _ftime (&_gtodtmp); \
    (t)->tv_sec = _gtodtmp.time; \
    (t)->tv_usec = _gtodtmp.millitm * 1000; \
}
# else
#  include <sys/time.h>
#  include <time.h>
# endif /* defined(_POSIX_SOURCE) && defined(SVR4) */

/* define X_GETTIMEOFDAY macro, a portable gettimeofday() */
# if defined(_XOPEN_XPG4) || defined(_XOPEN_UNIX) /* _XOPEN_UNIX is XPG4.2 */
#  define X_GETTIMEOFDAY(t) gettimeofday(t, (struct timezone*)0)
# else
#  if defined(SVR4) || defined(__SVR4) || defined(WIN32)
#   define X_GETTIMEOFDAY(t) gettimeofday(t)
#  else
#   define X_GETTIMEOFDAY(t) gettimeofday(t, (struct timezone*)0)
#  endif
# endif /* XPG4 else */


# ifdef __GNU__
#  define PATH_MAX 4096
#  define MAXPATHLEN 4096
#  define OPEN_MAX 256 /* We define a reasonable limit.  */
# endif

/* use POSIX name for signal */
# if defined(X_NOT_POSIX) && defined(SYSV) && !defined(SIGCHLD)
#  define SIGCHLD SIGCLD
# endif

# include "xarch.h"

#endif /* _XOS_H_ */