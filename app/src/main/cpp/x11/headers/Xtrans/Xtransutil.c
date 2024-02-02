#ifndef _Xtransutil_H
#define _Xtransutil_H

#ifdef XTHREADS
#include "../xthreads.h"
#endif
#ifdef WIN32
#include "../xlibint.h"
#include "../xwinsock.h"
#endif

#define TRANS_CLIENT
#define TRANS_SERVER
#define TRANS_REOPEN
#define UNIXCONN

#include "Xtransint.h"

#include <sys/un.h>


#ifdef ICE_t

/* Needed for _XGethostbyaddr usage in TRANS(GetPeerNetworkId) */
# if defined(TCPCONN) || defined(UNIXCONN)
#  define X_INCLUDE_NETDB_H
#  define XOS_USE_NO_LOCKING
#  include "../xos_r.h"
# endif

#include <signal.h>

char *
TRANS_ICE(GetMyNetworkId) (XtransConnInfo ciptr)

{
    int		family = ciptr->family;
    char 	*addr = ciptr->addr;
    char	hostnamebuf[256];
    char 	*networkId = NULL;
    const char	*transName = ciptr->transptr->TransName;

    if (gethostname (hostnamebuf, sizeof (hostnamebuf)) < 0)
    {
	return (NULL);
    }

    switch (family)
    {
#if defined(UNIXCONN) || defined(LOCALCONN)
    case AF_UNIX:
    {
	struct sockaddr_un *saddr = (struct sockaddr_un *) addr;
	networkId = malloc (3 + strlen (transName) +
	    strlen (hostnamebuf) + strlen (saddr->sun_path));
	sprintf (networkId, "%s/%s:%s", transName,
	    hostnamebuf, saddr->sun_path);
	break;
    }
#endif /* defined(UNIXCONN) || defined(LOCALCONN) */

#if defined(TCPCONN)
    case AF_INET:
#if defined(IPv6) && defined(AF_INET6)
    case AF_INET6:
#endif
    {
	struct sockaddr_in *saddr = (struct sockaddr_in *) addr;
#if defined(IPv6) && defined(AF_INET6)
	struct sockaddr_in6 *saddr6 = (struct sockaddr_in6 *) addr;
#endif
	int portnum;
	char portnumbuf[10];


#if defined(IPv6) && defined(AF_INET6)
	if (family == AF_INET6)
	    portnum = ntohs (saddr6->sin6_port);
	else
#endif
	    portnum = ntohs (saddr->sin_port);

	snprintf (portnumbuf, sizeof(portnumbuf), "%d", portnum);
	networkId = malloc (3 + strlen (transName) +
	    strlen (hostnamebuf) + strlen (portnumbuf));
	sprintf (networkId, "%s/%s:%s", transName, hostnamebuf, portnumbuf);
	break;
    }
#endif /* defined(TCPCONN) */


    default:
	break;
    }

    return (networkId);
}

#include <setjmp.h>
static jmp_buf env;

#ifdef SIGALRM
static volatile int nameserver_timedout = 0;

static void
nameserver_lost(int sig _X_UNUSED)
{
  nameserver_timedout = 1;
  longjmp (env, -1);
  /* NOTREACHED */
}
#endif /* SIGALARM */


char *
TRANS_ICE(GetPeerNetworkId) (XtransConnInfo ciptr)

{
    int		family = ciptr->family;
    char	*peer_addr = ciptr->peeraddr;
    char	*hostname;
    char	addrbuf[256];
    const char	*addr = NULL;

    switch (family)
    {
    case AF_UNSPEC:
#if defined(UNIXCONN) || defined(LOCALCONN)
    case AF_UNIX:
    {
	if (gethostname (addrbuf, sizeof (addrbuf)) == 0)
	    addr = addrbuf;
	break;
    }
#endif /* defined(UNIXCONN) || defined(LOCALCONN) */

#if defined(TCPCONN)
    case AF_INET:
#if defined(IPv6) && defined(AF_INET6)
    case AF_INET6:
#endif
    {
	struct sockaddr_in *saddr = (struct sockaddr_in *) peer_addr;
#if defined(IPv6) && defined(AF_INET6)
	struct sockaddr_in6 *saddr6 = (struct sockaddr_in6 *) peer_addr;
#endif
	char *address;
	int addresslen;
#ifdef XTHREADS_NEEDS_BYNAMEPARAMS
	_Xgethostbynameparams hparams;
#endif
	struct hostent * volatile hostp = NULL;

#if defined(IPv6) && defined(AF_INET6)
	if (family == AF_INET6)
	{
	    address = (char *) &saddr6->sin6_addr;
	    addresslen = sizeof (saddr6->sin6_addr);
	}
	else
#endif
	{
	    address = (char *) &saddr->sin_addr;
	    addresslen = sizeof (saddr->sin_addr);
	}

#ifdef SIGALRM
	/*
	 * gethostbyaddr can take a LONG time if the host does not exist.
	 * Assume that if it does not respond in NAMESERVER_TIMEOUT seconds
	 * that something is wrong and do not make the user wait.
	 * gethostbyaddr will continue after a signal, so we have to
	 * jump out of it.
	 */

	nameserver_timedout = 0;
	signal (SIGALRM, nameserver_lost);
	alarm (4);
	if (setjmp(env) == 0) {
#endif
	    hostp = _XGethostbyaddr (address, addresslen, family, hparams);
#ifdef SIGALRM
	}
	alarm (0);
#endif
	if (hostp != NULL)
	  addr = hostp->h_name;
	else
#if defined(IPv6) && defined(AF_INET6)
	  addr = inet_ntop (family, address, addrbuf, sizeof (addrbuf));
#else
	  addr = inet_ntoa (saddr->sin_addr);
#endif
	break;
    }

#endif /* defined(TCPCONN) */


    default:
	return (NULL);
    }


    hostname = malloc (strlen (ciptr->transptr->TransName) + strlen (addr) + 2);
    strcpy (hostname, ciptr->transptr->TransName);
    strcat (hostname, "/");
    if (addr)
	strcat (hostname, addr);

    return (hostname);
}

#endif /* ICE_t */


#if defined(WIN32) && defined(TCPCONN)
int
TRANS(WSAStartup) (void)
{
    static WSADATA wsadata;

    prmsg (2,"WSAStartup()\n");

    if (!wsadata.wVersion && WSAStartup(MAKEWORD(2,2), &wsadata))
        return 1;
    return 0;
}
#endif

#include <ctype.h>
#include <string.h>

static int
is_numeric (const char *str)
{
    int i;

    for (i = 0; i < (int) strlen (str); i++)
        if (!isdigit (str[i]))
            return (0);

    return (1);
}

#ifdef TRANS_SERVER
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


#if !defined(S_IFLNK) && !defined(S_ISLNK)
#undef lstat
#define lstat(a,b) stat(a,b)
#endif

#define FAIL_IF_NOMODE  1
#define FAIL_IF_NOT_ROOT 2
#define WARN_NO_ACCESS 4

/*
 * We make the assumption that when the 'sticky' (t) bit is requested
 * it's not save if the directory has non-root ownership or the sticky
 * bit cannot be set and fail.
 */
static int
trans_mkdir(const char *path, int mode)
{
    struct stat buf;

    if (lstat(path, &buf) != 0) {
	if (errno != ENOENT) {
	    prmsg(1, "mkdir: ERROR: (l)stat failed for %s (%d)\n",
		  path, errno);
	    return -1;
	}
	/* Dir doesn't exist. Try to create it */

#if !defined(WIN32) && !defined(__CYGWIN__)
	/*
	 * 'sticky' bit requested: assume application makes
	 * certain security implications. If effective user ID
	 * is != 0: fail as we may not be able to meet them.
	 */
	if (geteuid() != 0) {
	    if (mode & 01000) {
		prmsg(1, "mkdir: ERROR: euid != 0,"
		      "directory %s will not be created.\n",
		      path);
#ifdef FAIL_HARD
		return -1;
#endif
	    } else {
		prmsg(1, "mkdir: Cannot create %s with root ownership\n",
		      path);
	    }
	}
#endif

#ifndef WIN32
	if (mkdir(path, mode) == 0) {
	    if (chmod(path, mode)) {
		prmsg(1, "mkdir: ERROR: Mode of %s should be set to %04o\n",
		      path, mode);
#ifdef FAIL_HARD
		return -1;
#endif
	    }
#else
	if (mkdir(path) == 0) {
#endif
	} else {
	    prmsg(1, "mkdir: ERROR: Cannot create %s\n",
		  path);
	    return -1;
	}

	return 0;

    } else {
	if (S_ISDIR(buf.st_mode)) {
		int updateOwner = 0;
	    int updateMode = 0;
	    int updatedOwner = 0;
	    int updatedMode = 0;
	    int status = 0;
	    /* Check if the directory's ownership is OK. */
	    if (buf.st_uid != 0)
		updateOwner = 1;

	    /*
	     * Check if the directory's mode is OK.  An exact match isn't
	     * required, just a mode that isn't more permissive than the
	     * one requested.
	     */
	    if ((~mode) & 0077 & buf.st_mode)
		updateMode = 1;

	    /*
	     * If the directory is not writeable not everybody may
	     * be able to create sockets. Therefore warn if mode
	     * cannot be fixed.
	     */
	    if ((~buf.st_mode) & 0022 & mode) {
		updateMode = 1;
		status |= WARN_NO_ACCESS;
	    }

	    /*
	     * If 'sticky' bit is requested fail if owner isn't root
	     * as we assume the caller makes certain security implications
	     */
	    if (mode & 01000) {
		status |= FAIL_IF_NOT_ROOT;
		if (!(buf.st_mode & 01000)) {
		    status |= FAIL_IF_NOMODE;
		    updateMode = 1;
		}
	    }

#ifdef HAS_FCHOWN
	    /*
	     * If fchown(2) and fchmod(2) are available, try to correct the
	     * directory's owner and mode.  Otherwise it isn't safe to attempt
	     * to do this.
	     */
	    if (updateMode || updateOwner) {
		int fd = -1;
		struct stat fbuf;
		if ((fd = open(path, O_RDONLY)) != -1) {
		    if (fstat(fd, &fbuf) == -1) {
			prmsg(1, "mkdir: ERROR: fstat failed for %s (%d)\n",
			      path, errno);
			close(fd);
			return -1;
		    }
		    /*
		     * Verify that we've opened the same directory as was
		     * checked above.
		     */
		    if (!S_ISDIR(fbuf.st_mode) ||
			buf.st_dev != fbuf.st_dev ||
			buf.st_ino != fbuf.st_ino) {
			prmsg(1, "mkdir: ERROR: inode for %s changed\n",
			      path);
			close(fd);
			return -1;
		    }
		    if (updateOwner && fchown(fd, 0, 0) == 0)
			updatedOwner = 1;
		    if (updateMode && fchmod(fd, mode) == 0)
			updatedMode = 1;
		    close(fd);
		}
	    }
#endif

	    if (updateOwner && !updatedOwner) {
#ifdef FAIL_HARD
		if (status & FAIL_IF_NOT_ROOT) {
		    prmsg(1, "mkdir: ERROR: Owner of %s must be set to root\n",
			  path);
		    return -1;
		}
#endif
#if !defined(__APPLE_CC__) && !defined(__CYGWIN__)
		prmsg(1, "mkdir: Owner of %s should be set to root\n",
		      path);
#endif
	    }

	    if (updateMode && !updatedMode) {
#ifdef FAIL_HARD
		if (status & FAIL_IF_NOMODE) {
		    prmsg(1, "mkdir: ERROR: Mode of %s must be set to %04o\n",
			  path, mode);
		    return -1;
		}
#endif
		prmsg(1, "mkdir: Mode of %s should be set to %04o\n",
		      path, mode);
		if (status & WARN_NO_ACCESS) {
		    prmsg(1, "mkdir: this may cause subsequent errors\n");
		}
	    }
	    return 0;
	}
	return -1;
    }

    /* In all other cases, fail */
    return -1;
}

#endif /* TRANS_SERVER */
#endif //_Xtransutil_H