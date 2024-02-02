#ifdef WIN32
# define _WILLWINSOCK_
#endif
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "SMlib.h"
#include "SMlibint.h"
#ifdef XTHREADS
# include <X11/Xthreads.h>
#endif
#include <stdio.h>
#include <unistd.h>

#include <time.h>
#define Time_t time_t

#ifndef WIN32

# ifdef TCPCONN
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  define XOS_USE_NO_LOCKING
#  define X_INCLUDE_NETDB_H
#  include <X11/Xos_r.h>
# endif

#else /* WIN32 */

# include "../headers/xwinsock.h"
# include "../headers/xw32defs.h"
# define X_INCLUDE_NETDB_H
# define XOS_USE_MTSAFE_NETDBAPI
# include "../headers/xos_r.h"

#endif /* WIN32 */

#ifdef MNX_TCPCONN
# include <net/gen/netdb.h>

# define TCPCONN
#endif

#if defined(HAVE_UUID_CREATE)
# include <uuid.h>
#elif defined(HAVE_LIBUUID)
# include <uuid/uuid.h>
#endif


char *
SmsGenerateClientID(SmsConn smsConn)
{
#if defined(HAVE_UUID_CREATE)
    char *id;
    char *temp;
    uuid_t uuid;
    uint32_t status;
    size_t len;

    uuid_create(&uuid, &status);

    uuid_to_string(&uuid, &temp, &status);

    len = strlen(temp) + 2;
    if ((id = malloc(len)) != NULL)
        snprintf(id, len, "2%s", temp);

    free(temp);

    return id;
#elif defined(HAVE_LIBUUID)
    char *id;
    char temp[256];
    uuid_t uuid;

    uuid_generate(uuid);

    temp[0] = '2';
    temp[1] = '\0';
    uuid_unparse_lower(uuid, &temp[1]);

    id = strdup (temp);

    return id;
#else
# ifdef TCPCONN
    static const char hex[] = "0123456789abcdef";
    char hostname[256];
    char address[64], *addr_ptr = address;
    char temp[256];
    char *id;
    static int sequence = 0;
    char* inet_addr;
    char *ptr1;
    unsigned char decimal[4];
    int i;
    struct in_addr *haddr = NULL;
#  if defined(IPv6) && defined(AF_INET6)
    struct addrinfo *ai, *first_ai;
#  endif

    if (gethostname (hostname, sizeof (hostname)))
	return (NULL);

#  if defined(IPv6) && defined(AF_INET6)
    if (getaddrinfo(hostname,NULL,NULL,&ai) != 0)
	return NULL;

    for (first_ai = ai; ai != NULL; ai = ai->ai_next) {
	if ( (ai->ai_family == AF_INET) || (ai->ai_family == AF_INET6) )
	    break;
    }
    if (ai == NULL) {
	freeaddrinfo(first_ai);
	return NULL;
    }

    if (ai->ai_family == AF_INET6) {
	unsigned char *cp = (unsigned char *) &((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr.s6_addr;

	*addr_ptr++ = '6';	/* IPv6 address code */

	for (i = 0 ; i < 16 ; i++) {
            *addr_ptr++ = hex[cp[i] >> 4];
            *addr_ptr++ = hex[cp[i] & 0x0f];
	}

        *addr_ptr++ = '\0';

    } else { /* Fall through to IPv4 address handling */
	haddr = &((struct sockaddr_in *)ai->ai_addr)->sin_addr;
#  else
#   ifdef XTHREADS_NEEDS_BYNAMEPARAMS
	_Xgethostbynameparams hparams;
#   endif
	struct hostent *hostp;

	if ((hostp = _XGethostbyname (hostname,hparams)) != NULL)
	    haddr = (struct in_addr *)(hostp->h_addr);
	else
	    return NULL;
#  endif

	inet_addr = inet_ntoa (*haddr);
	for (i = 0, ptr1 = inet_addr; i < 3; i++)
	{
	    char temp4[4];
	    char *ptr2 = strchr (ptr1, '.');
	    size_t len = (size_t) (ptr2 - ptr1);

	    if (!ptr2 || len > 3) {
#  if defined(IPv6) && defined(AF_INET6)
		freeaddrinfo(first_ai);
#  endif
		return (NULL);
	    }
	    strncpy (temp4, ptr1, len);
	    temp4[len] = '\0';
	    decimal[i] = (unsigned char) atoi (temp4);
	    ptr1 = ptr2 + 1;
	}

	decimal[3] = (unsigned char) atoi (ptr1);

	*addr_ptr++ = '1';

	for (i = 0; i < 4; i++) {
	    *addr_ptr++ = hex[decimal[i] >> 4];
	    *addr_ptr++ = hex[decimal[i] & 0x0f];
	}

	*addr_ptr++ = '\0';

#  if defined(IPv6) && defined(AF_INET6)
    }
    freeaddrinfo(first_ai);
#  endif

    sprintf (temp, "1%s%.13ld%.10ld%.4d", address, (long)time((Time_t*)0),
	     (long)getpid(), sequence);

    if (++sequence > 9999)
	sequence = 0;

    id = strdup (temp);

    return (id);
# else
    return (NULL);
# endif
#endif
}
