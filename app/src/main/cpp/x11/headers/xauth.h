#ifndef _Xauth_h
#define _Xauth_h

typedef struct xauth {
    unsigned short   family;
    unsigned short   address_length;
    char    	    *address;
    unsigned short   number_length;
    char    	    *number;
    unsigned short   name_length;
    char    	    *name;
    unsigned short   data_length;
    char   	    *data;
} Xauth;

#ifndef _XAUTH_STRUCT_ONLY

# include  "xfuncproto.h"
# include  "xfuncs.h"

# include  <stdio.h>

# define FamilyLocal (256)	/* not part of X standard (i.e. X.h) */
# define FamilyWild  (65535)
# define FamilyNetname    (254)   /* not part of X standard */
# define FamilyKrb5Principal (253) /* Kerberos 5 principal name */
# define FamilyLocalHost (252)	/* for local non-net authentication */


_XFUNCPROTOBEGIN

char *XauFileName(void);

Xauth *XauReadAuth(
        FILE*	/* auth_file */
);

int XauLockAuth(
        _Xconst char*	/* file_name */,
        int		/* retries */,
        int		/* timeout */,
        long		/* dead */
);

int XauUnlockAuth(
        _Xconst char*	/* file_name */
);

int XauWriteAuth(
        FILE*		/* auth_file */,
        Xauth*		/* auth */
);

Xauth *XauGetAuthByAddr(
#if NeedWidePrototypes
        unsigned int	/* family */,
unsigned int	/* address_length */,
#else
        unsigned short	/* family */,
        unsigned short	/* address_length */,
#endif
        _Xconst char*	/* address */,
#if NeedWidePrototypes
        unsigned int	/* number_length */,
#else
        unsigned short	/* number_length */,
#endif
        _Xconst char*	/* number */,
#if NeedWidePrototypes
        unsigned int	/* name_length */,
#else
        unsigned short	/* name_length */,
#endif
        _Xconst char*	/* name */
);

Xauth *XauGetBestAuthByAddr(
#if NeedWidePrototypes
        unsigned int	/* family */,
unsigned int	/* address_length */,
#else
        unsigned short	/* family */,
        unsigned short	/* address_length */,
#endif
        _Xconst char*	/* address */,
#if NeedWidePrototypes
        unsigned int	/* number_length */,
#else
        unsigned short	/* number_length */,
#endif
        _Xconst char*	/* number */,
        int		/* types_length */,
        char**		/* type_names */,
        _Xconst int*	/* type_lengths */
);

void XauDisposeAuth(
        Xauth*		/* auth */
);

_XFUNCPROTOEND

/* Return values from XauLockAuth */

# define LOCK_SUCCESS	0	/* lock succeeded */
# define LOCK_ERROR	1	/* lock unexpectely failed, check errno */
# define LOCK_TIMEOUT	2	/* lock failed, timeouts expired */

#endif /* _XAUTH_STRUCT_ONLY */

#endif /* _Xauth_h */