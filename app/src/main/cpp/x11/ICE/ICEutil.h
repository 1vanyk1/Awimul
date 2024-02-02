#ifndef _ICEUTIL_H_
#define _ICEUTIL_H_

#include "../headers/xfuncproto.h"

#include <stdio.h>

_XFUNCPROTOBEGIN

/*
 * Data structure for entry in ICE authority file
 */

typedef struct {
    char    	    *protocol_name;
    unsigned short  protocol_data_length;
    char   	    *protocol_data;
    char    	    *network_id;
    char    	    *auth_name;
    unsigned short  auth_data_length;
    char   	    *auth_data;
} IceAuthFileEntry;


/*
 * Authentication data maintained in memory.
 */

typedef struct {
    char    	    *protocol_name;
    char	    *network_id;
    char    	    *auth_name;
    unsigned short  auth_data_length;
    char   	    *auth_data;
} IceAuthDataEntry;


/*
 * Return values from IceLockAuthFile
 */

#define IceAuthLockSuccess	0   /* lock succeeded */
#define IceAuthLockError	1   /* lock unexpectely failed, check errno */
#define IceAuthLockTimeout	2   /* lock failed, timeouts expired */


/*
 * Function Prototypes
 */

extern char *IceAuthFileName (
        void
);

extern int IceLockAuthFile (
        const char *	/* file_name */,
        int			/* retries */,
        int			/* timeout */,
        long		/* dead */
);

extern void IceUnlockAuthFile (
        const char *	/* file_name */
);

extern IceAuthFileEntry *IceReadAuthFileEntry (
        FILE *		/* auth_file */
);

extern void IceFreeAuthFileEntry (
        IceAuthFileEntry *	/* auth */
);

extern Status IceWriteAuthFileEntry (
        FILE *		/* auth_file */,
        IceAuthFileEntry *	/* auth */
);

extern IceAuthFileEntry *IceGetAuthFileEntry (
        const char *	/* protocol_name */,
        const char *	/* network_id */,
        const char *	/* auth_name */
);

extern char *IceGenerateMagicCookie (
        int			/* len */
);

extern void IceSetPaAuthData (
        int			/* numEntries */,
        IceAuthDataEntry *	/* entries */
);

_XFUNCPROTOEND

#endif /* _ICEUTIL_H_ */
