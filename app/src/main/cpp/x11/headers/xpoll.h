#ifndef _XPOLL_H_
#define _XPOLL_H_

#ifndef WIN32

#ifndef USE_POLL

#include "xos.h"

#include <sys/select.h>  /* Get the FD_* macros. */

#include "xmd.h"

#ifdef CSRG_BASED
#include <sys/param.h>
# if BSD < 199103
typedef long fd_mask;
# endif
#endif

#define XFD_SETSIZE	256

#ifndef FD_SETSIZE
#define FD_SETSIZE	XFD_SETSIZE
#endif

#ifndef NBBY
#define NBBY	8		/* number of bits in a byte */
#endif

#ifndef NFDBITS
#define NFDBITS (sizeof(fd_mask) * NBBY)	/* bits per mask */
#endif

#ifndef howmany
#define howmany(x,y)	(((x)+((y)-1))/(y))
#endif

#if defined(BSD) && BSD < 198911
typedef struct fd_set {
	fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;
#endif

# define Select(n,r,w,e,t) select(n,(fd_set*)r,(fd_set*)w,(fd_set*)e,(struct timeval*)t)

#define __X_FDS_BITS __fds_bits

#ifndef __FDS_BITS
# define __FDS_BITS(p)  ((p)->__X_FDS_BITS)
#endif

#define __XFDS_BITS(p, n) (__FDS_BITS(p))[n]

#ifndef FD_SET
#define FD_SET(n, p)    (__XFDS_BITS(p, ((n)/NFDBITS)) |= ((fd_mask)1 << ((n) % NFDBITS)))
#endif
#ifndef FD_CLR
#define FD_CLR(n, p)    (__XFDS_BITS((p), ((n)/NFDBITS)) &= ~((fd_mask)1 << ((n) % NFDBITS)))
#endif
#ifndef FD_ISSET
#define FD_ISSET(n, p)  ((__XFDS_BITS((p), ((n)/NFDBITS))) & ((fd_mask)1 << ((n) % NFDBITS)))
#endif
#ifndef FD_ZERO
#define FD_ZERO(p)      bzero((char *)(p), sizeof(*(p)))
#endif

/*
 * The howmany(FD_SETSIZE, NFDBITS) computes the number of elements in the
 * array. before accessing an element in the array we check it exists.
 * If it does not exist then the compiler discards the code to access it. 
 */
#define XFD_ANYSET(p) \
        ((howmany(FD_SETSIZE, NFDBITS) > 0 && (__XFDS_BITS(p, 0))) || \
        (howmany(FD_SETSIZE, NFDBITS) > 1 && (__XFDS_BITS(p, 1))) || \
        (howmany(FD_SETSIZE, NFDBITS) > 2 && (__XFDS_BITS(p, 2))) || \
        (howmany(FD_SETSIZE, NFDBITS) > 3 && (__XFDS_BITS(p, 3))) || \
        (howmany(FD_SETSIZE, NFDBITS) > 4 && (__XFDS_BITS(p, 4))) || \
        (howmany(FD_SETSIZE, NFDBITS) > 5 && (__XFDS_BITS(p, 5))) || \
        (howmany(FD_SETSIZE, NFDBITS) > 6 && (__XFDS_BITS(p, 6))) || \
        (howmany(FD_SETSIZE, NFDBITS) > 7 && (__XFDS_BITS(p, 7))))

#define XFD_COPYSET(src,dst) { \
        int __i__; \
		for (__i__ = 0; __i__ < howmany(FD_SETSIZE, NFDBITS); __i__++) \
            __XFDS_BITS((dst), __i__) = __XFDS_BITS((src), __i__); \
        }
#define XFD_ANDSET(dst,b1,b2) { \
        int __i__; \
        for (__i__ = 0; __i__ < howmany(FD_SETSIZE, NFDBITS); __i__++) \
            __XFDS_BITS((dst), __i__) = ((__XFDS_BITS((b1), __i__)) & (__XFDS_BITS((b2), __i__))); \
        }
#define XFD_ORSET(dst,b1,b2) { \
        int __i__; \
        for (__i__ = 0; __i__ < howmany(FD_SETSIZE, NFDBITS); __i__++) \
		__XFDS_BITS((dst), __i__) = ((__XFDS_BITS((b1), __i__)) | (__XFDS_BITS((b2), __i__))); \
        }
#define XFD_UNSET(dst,b1) { \
        int __i__; \
        for (__i__ = 0; __i__ < howmany(FD_SETSIZE, NFDBITS); __i__++) \
    		__XFDS_BITS((dst), __i__) &= ~(__XFDS_BITS((b1), __i__)); \
        }

#else /* USE_POLL */
#include <sys/poll.h>
#endif /* USE_POLL */

#else /* WIN32 */

#define XFD_SETSIZE	256
#ifndef FD_SETSIZE
#define FD_SETSIZE	XFD_SETSIZE
#endif
#include "xwinsock.h"

#define Select(n,r,w,e,t) select(0,(fd_set*)r,(fd_set*)w,(fd_set*)e,(struct timeval*)t)

#define XFD_SETCOUNT(p)	(((fd_set FAR *)(p))->fd_count)
#define XFD_FD(p,i) (((fd_set FAR *)(p))->fd_array[i])
#define XFD_ANYSET(p)	XFD_SETCOUNT(p)

#define XFD_COPYSET(src,dst) { \
    u_int __i; \
    FD_ZERO(dst); \
    for (__i = 0; __i < XFD_SETCOUNT(src) ; __i++) { \
        XFD_FD(dst,__i) = XFD_FD(src,__i); \
    } \
    XFD_SETCOUNT(dst) = XFD_SETCOUNT(src); \
}

#define XFD_ANDSET(dst,b1,b2) { \
    u_int __i; \
    FD_ZERO(dst); \
    for (__i = 0; __i < XFD_SETCOUNT(b1) ; __i++) { \
        if (FD_ISSET(XFD_FD(b1,__i), b2)) \
	   FD_SET(XFD_FD(b1,__i), dst); \
    } \
}

#define XFD_ORSET(dst,b1,b2) { \
    u_int __i; \
    if (dst != b1) XFD_COPYSET(b1,dst); \
    for (__i = 0; __i < XFD_SETCOUNT(b2) ; __i++) { \
        if (!FD_ISSET(XFD_FD(b2,__i), dst)) \
	   FD_SET(XFD_FD(b2,__i), dst); \
    } \
}

/* this one is really sub-optimal */
#define XFD_UNSET(dst,b1) { \
    u_int __i; \
    for (__i = 0; __i < XFD_SETCOUNT(b1) ; __i++) { \
	FD_CLR(XFD_FD(b1,__i), dst); \
    } \
}

/* we have to pay the price of having an array here, unlike with bitmasks
   calling twice FD_SET with the same fd is not transparent, so be careful */
#undef FD_SET
#define FD_SET(fd,set) do { \
    if (XFD_SETCOUNT(set) < FD_SETSIZE && !FD_ISSET(fd,set)) \
        XFD_FD(set,XFD_SETCOUNT(set)++)=(fd); \
} while(0)

#define getdtablesize() FD_SETSIZE 

#endif /* WIN32 */

#endif /* _XPOLL_H_ */