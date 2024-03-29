#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "Xtransint.h"
#include "Xtrans.h"
#include "../../../main_wm.h"

#ifdef HAVE_SYSTEMD_DAEMON
#include <systemd/sd-daemon.h>
#endif

/*
 * The transport table contains a definition for every transport (protocol)
 * family. All operations that can be made on the transport go through this
 * table.
 *
 * Each transport is assigned a unique transport id.
 *
 * New transports can be added by adding an entry in this table.
 * For compatiblity, the transport ids should never be renumbered.
 * Always add to the end of the list.
 */

#define TRANS_TLI_INET_INDEX		1
#define TRANS_TLI_TCP_INDEX		2
#define TRANS_TLI_TLI_INDEX		3
#define TRANS_SOCKET_UNIX_INDEX		4
#define TRANS_SOCKET_LOCAL_INDEX	5
#define TRANS_SOCKET_INET_INDEX		6
#define TRANS_SOCKET_TCP_INDEX		7
#define TRANS_DNET_INDEX		8
#define TRANS_LOCAL_LOCAL_INDEX		9
/* 10 used to be PTS, but that's gone. */
#define TRANS_LOCAL_NAMED_INDEX		11
/* 12 used to be ISC, but that's gone. */
/* 13 used to be SCO, but that's gone. */
#define TRANS_SOCKET_INET6_INDEX	14
#define TRANS_LOCAL_PIPE_INDEX		15

#include "Xtranssock_ice.c"
#include "Xtranslcl_ice.c"


#define ICE_t

#ifdef TRANS_SERVER
static const char * local_aliases[] = {
        "named",
        "pipe", /* compatibility with Solaris Xlib */
        NULL };
#endif

Xtransport	TRANS_ICE(LocalFuncs) = {
/* Local Interface */
        "local",
        TRANS_ALIAS | TRANS_LOCAL,
#ifdef TRANS_CLIENT
        TRANS_ICE(LocalOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
        local_aliases,
        TRANS_ICE(LocalOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
        TRANS_ICE(LocalReopenCOTSServer),
#endif
        TRANS_ICE(LocalSetOption),
#ifdef TRANS_SERVER
        TRANS_ICE(LocalCreateListener),
        TRANS_ICE(LocalResetListener),
        TRANS_ICE(LocalAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
        TRANS_ICE(LocalConnect),
#endif /* TRANS_CLIENT */
        TRANS_ICE(LocalBytesReadable),
        TRANS_ICE(LocalRead),
        TRANS_ICE(LocalWrite),
        TRANS_ICE(LocalReadv),
        TRANS_ICE(LocalWritev),
#if XTRANS_SEND_FDS
        TRANS_ICE(LocalSendFdInvalid),
	TRANS_ICE(LocalRecvFdInvalid),
#endif
        TRANS_ICE(LocalDisconnect),
        TRANS_ICE(LocalClose),
        TRANS_ICE(LocalCloseForCloning),
};


#ifdef LOCAL_TRANS_NAMED

Xtransport	TRANS_ICE(NAMEDFuncs) = {
        /* Local Interface */
        "named",
        TRANS_LOCAL,
#ifdef TRANS_CLIENT
        TRANS_ICE(LocalOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
        NULL,
        TRANS_ICE(LocalOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
        TRANS_ICE(LocalReopenCOTSServer),
#endif
        TRANS_ICE(LocalSetOption),
#ifdef TRANS_SERVER
        TRANS_ICE(LocalCreateListener),
        TRANS_ICE(LocalResetListener),
        TRANS_ICE(LocalAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
        TRANS_ICE(LocalConnect),
#endif /* TRANS_CLIENT */
        TRANS_ICE(LocalBytesReadable),
        TRANS_ICE(LocalRead),
        TRANS_ICE(LocalWrite),
        TRANS_ICE(LocalReadv),
        TRANS_ICE(LocalWritev),
#if XTRANS_SEND_FDS
        TRANS_ICE(LocalSendFdInvalid),
	TRANS_ICE(LocalRecvFdInvalid),
#endif
        TRANS_ICE(LocalDisconnect),
        TRANS_ICE(LocalClose),
        TRANS_ICE(LocalCloseForCloning),
};

Xtransport	TRANS_ICE(PIPEFuncs) = {
        /* Local Interface */
        "pipe",
        TRANS_ALIAS | TRANS_LOCAL,
#ifdef TRANS_CLIENT
        TRANS_ICE(LocalOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
        NULL,
        TRANS_ICE(LocalOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
        TRANS_ICE(LocalReopenCOTSServer),
#endif
        TRANS_ICE(LocalSetOption),
#ifdef TRANS_SERVER
        TRANS_ICE(LocalCreateListener),
        TRANS_ICE(LocalResetListener),
        TRANS_ICE(LocalAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
        TRANS_ICE(LocalConnect),
#endif /* TRANS_CLIENT */
        TRANS_ICE(LocalBytesReadable),
        TRANS_ICE(LocalRead),
        TRANS_ICE(LocalWrite),
        TRANS_ICE(LocalReadv),
        TRANS_ICE(LocalWritev),
#if XTRANS_SEND_FDS
        TRANS_ICE(LocalSendFdInvalid),
	TRANS_ICE(LocalRecvFdInvalid),
#endif
        TRANS_ICE(LocalDisconnect),
        TRANS_ICE(LocalClose),
        TRANS_ICE(LocalCloseForCloning),
};
#endif /* LOCAL_TRANS_NAMED */



#ifdef TCPCONN
# ifdef TRANS_SERVER
static const char* tcp_nolisten[] = {
        "inet",
#if defined(IPv6) && defined(AF_INET6)
        "inet6",
#endif
        NULL
};
# endif

Xtransport	TRANS_ICE(SocketTCPFuncs) = {
        /* Socket Interface */
        "tcp",
        TRANS_ALIAS,
#ifdef TRANS_CLIENT
        TRANS_ICE(SocketOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
        tcp_nolisten,
        TRANS_ICE(SocketOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
        TRANS_ICE(SocketReopenCOTSServer),
#endif
        TRANS_ICE(SocketSetOption),
#ifdef TRANS_SERVER
        TRANS_ICE(SocketINETCreateListener),
        NULL,		       			/* ResetListener */
        TRANS_ICE(SocketINETAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
        TRANS_ICE(SocketINETConnect),
#endif /* TRANS_CLIENT */
        TRANS_ICE(SocketBytesReadable),
        TRANS_ICE(SocketRead),
        TRANS_ICE(SocketWrite),
        TRANS_ICE(SocketReadv),
        TRANS_ICE(SocketWritev),
#if XTRANS_SEND_FDS
        TRANS_ICE(SocketSendFdInvalid),
        TRANS_ICE(SocketRecvFdInvalid),
#endif
        TRANS_ICE(SocketDisconnect),
        TRANS_ICE(SocketINETClose),
        TRANS_ICE(SocketINETClose),
};

Xtransport	TRANS_ICE(SocketINETFuncs) = {
        /* Socket Interface */
        "inet",
        0,
#ifdef TRANS_CLIENT
        TRANS_ICE(SocketOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
        NULL,
        TRANS_ICE(SocketOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
        TRANS_ICE(SocketReopenCOTSServer),
#endif
        TRANS_ICE(SocketSetOption),
#ifdef TRANS_SERVER
        TRANS_ICE(SocketINETCreateListener),
        NULL,		       			/* ResetListener */
        TRANS_ICE(SocketINETAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
        TRANS_ICE(SocketINETConnect),
#endif /* TRANS_CLIENT */
        TRANS_ICE(SocketBytesReadable),
        TRANS_ICE(SocketRead),
        TRANS_ICE(SocketWrite),
        TRANS_ICE(SocketReadv),
        TRANS_ICE(SocketWritev),
#if XTRANS_SEND_FDS
        TRANS_ICE(SocketSendFdInvalid),
        TRANS_ICE(SocketRecvFdInvalid),
#endif
        TRANS_ICE(SocketDisconnect),
        TRANS_ICE(SocketINETClose),
        TRANS_ICE(SocketINETClose),
};

#if defined(IPv6) && defined(AF_INET6)
Xtransport     TRANS_ICE(SocketINET6Funcs) = {
	/* Socket Interface */
	"inet6",
	0,
#ifdef TRANS_CLIENT
	TRANS_ICE(SocketOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
	NULL,
	TRANS_ICE(SocketOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
	TRANS_ICE(SocketReopenCOTSServer),
#endif
	TRANS_ICE(SocketSetOption),
#ifdef TRANS_SERVER
	TRANS_ICE(SocketINETCreateListener),
	NULL,					/* ResetListener */
	TRANS_ICE(SocketINETAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
	TRANS_ICE(SocketINETConnect),
#endif /* TRANS_CLIENT */
	TRANS_ICE(SocketBytesReadable),
	TRANS_ICE(SocketRead),
	TRANS_ICE(SocketWrite),
	TRANS_ICE(SocketReadv),
	TRANS_ICE(SocketWritev),
#if XTRANS_SEND_FDS
        TRANS_ICE(SocketSendFdInvalid),
        TRANS_ICE(SocketRecvFdInvalid),
#endif
	TRANS_ICE(SocketDisconnect),
	TRANS_ICE(SocketINETClose),
	TRANS_ICE(SocketINETClose),
	};
#endif /* IPv6 */
#endif /* TCPCONN */

#ifdef UNIXCONN
#if !defined(LOCALCONN)
Xtransport	TRANS_ICE(SocketLocalFuncs) = {
	/* Socket Interface */
	"local",
#ifdef HAVE_ABSTRACT_SOCKETS
	TRANS_ABSTRACT,
#else
	0,
#endif
#ifdef TRANS_CLIENT
	TRANS_ICE(SocketOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
	NULL,
	TRANS_ICE(SocketOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
	TRANS_ICE(SocketReopenCOTSServer),
#endif
	TRANS_ICE(SocketSetOption),
#ifdef TRANS_SERVER
	TRANS_ICE(SocketUNIXCreateListener),
	TRANS_ICE(SocketUNIXResetListener),
	TRANS_ICE(SocketUNIXAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
	TRANS_ICE(SocketUNIXConnect),
#endif /* TRANS_CLIENT */
	TRANS_ICE(SocketBytesReadable),
	TRANS_ICE(SocketRead),
	TRANS_ICE(SocketWrite),
	TRANS_ICE(SocketReadv),
	TRANS_ICE(SocketWritev),
#if XTRANS_SEND_FDS
        TRANS_ICE(SocketSendFd),
        TRANS_ICE(SocketRecvFd),
#endif
	TRANS_ICE(SocketDisconnect),
	TRANS_ICE(SocketUNIXClose),
	TRANS_ICE(SocketUNIXCloseForCloning),
	};
#endif /* !LOCALCONN */
# ifdef TRANS_SERVER
#  if !defined(LOCALCONN)
static const char* unix_nolisten[] = { "local" , NULL };
#  endif
# endif

Xtransport	TRANS_ICE(SocketUNIXFuncs) = {
        /* Socket Interface */
        "unix",
#if !defined(LOCALCONN) && !defined(HAVE_ABSTRACT_SOCKETS)
        TRANS_ALIAS,
#else
        0,
#endif
#ifdef TRANS_CLIENT
        TRANS_ICE(SocketOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
#if !defined(LOCALCONN)
        unix_nolisten,
#else
        NULL,
#endif
        TRANS_ICE(SocketOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
        TRANS_ICE(SocketReopenCOTSServer),
#endif
        TRANS_ICE(SocketSetOption),
#ifdef TRANS_SERVER
        TRANS_ICE(SocketUNIXCreateListener),
        TRANS_ICE(SocketUNIXResetListener),
        TRANS_ICE(SocketUNIXAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
        TRANS_ICE(SocketUNIXConnect),
#endif /* TRANS_CLIENT */
        TRANS_ICE(SocketBytesReadable),
        TRANS_ICE(SocketRead),
        TRANS_ICE(SocketWrite),
        TRANS_ICE(SocketReadv),
        TRANS_ICE(SocketWritev),
#if XTRANS_SEND_FDS
        TRANS_ICE(SocketSendFd),
        TRANS_ICE(SocketRecvFd),
#endif
        TRANS_ICE(SocketDisconnect),
        TRANS_ICE(SocketUNIXClose),
        TRANS_ICE(SocketUNIXCloseForCloning),
};

#endif /* UNIXCONN */




static
Xtransport_table Xtransports[] = {
#if defined(TCPCONN)
        { &TRANS_ICE(SocketTCPFuncs),	TRANS_SOCKET_TCP_INDEX },
#if defined(IPv6) && defined(AF_INET6)
    { &TRANS_ICE(SocketINET6Funcs),	TRANS_SOCKET_INET6_INDEX },
#endif /* IPv6 */
    { &TRANS_ICE(SocketINETFuncs),	TRANS_SOCKET_INET_INDEX },
#endif /* TCPCONN */
#if defined(UNIXCONN)
        #if !defined(LOCALCONN)
    { &TRANS_ICE(SocketLocalFuncs),	TRANS_SOCKET_LOCAL_INDEX },
#endif /* !LOCALCONN */
    { &TRANS_ICE(SocketUNIXFuncs),	TRANS_SOCKET_UNIX_INDEX },
#endif /* UNIXCONN */
#if defined(LOCALCONN)
        { &TRANS_ICE(LocalFuncs),	TRANS_LOCAL_LOCAL_INDEX },
#if defined(SVR4) || defined(__SVR4)
    { &TRANS_ICE(NAMEDFuncs),	TRANS_LOCAL_NAMED_INDEX },
#endif
#ifdef __sun
    { &TRANS_ICE(PIPEFuncs),	TRANS_LOCAL_PIPE_INDEX },
#endif /* __sun */
#endif /* LOCALCONN */
};

#define NUMTRANS	(sizeof(Xtransports)/sizeof(Xtransport_table))


//#ifdef ICE_t

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

//#endif /* ICE_t */


#ifdef WIN32
#define ioctl ioctlsocket
#endif



/*
 * These are a few utility function used by the public interface functions.
 */

void
TRANS_ICE(FreeConnInfo) (XtransConnInfo ciptr)

{
    prmsg (3,"FreeConnInfo(%p)\n", ciptr);

    if (ciptr->addr)
        free (ciptr->addr);

    if (ciptr->peeraddr)
        free (ciptr->peeraddr);

    if (ciptr->port)
        free (ciptr->port);

    free (ciptr);
}


#define PROTOBUFSIZE	20

static Xtransport *
TRANS_ICE(SelectTransport) (const char *protocol)

{
#ifndef HAVE_STRCASECMP
    char 	protobuf[PROTOBUFSIZE];
#endif
    int		i;

    prmsg (1,"SelectTransport(%s)\n", protocol);

#ifndef HAVE_STRCASECMP
    /*
     * Force Protocol to be lowercase as a way of doing
     * a case insensitive match.
     */

    strncpy (protobuf, protocol, PROTOBUFSIZE - 1);
    protobuf[PROTOBUFSIZE-1] = '\0';

    for (i = 0; i < PROTOBUFSIZE && protobuf[i] != '\0'; i++)
        if (isupper ((unsigned char)protobuf[i]))
            protobuf[i] = tolower ((unsigned char)protobuf[i]);
#endif

    /* Look at all of the configured protocols */

    for (i = 0; i < NUMTRANS; i++)
    {
#ifndef HAVE_STRCASECMP
        if (strcmp (protobuf, Xtransports[i].transport->TransName) == 0)
#else
            if (!strcasecmp (protocol, Xtransports[i].transport->TransName))
#endif
        {
            return Xtransports[i].transport;
        }
    }

    return NULL;
}

static int
TRANS_ICE(ParseAddress) (const char *address,
                     char **protocol, char **host, char **port)

{
    /*
     * For the font library, the address is a string formatted
     * as "protocol/host:port[/catalogue]".  Note that the catologue
     * is optional.  At this time, the catologue info is ignored, but
     * we have to parse it anyways.
     *
     * Other than fontlib, the address is a string formatted
     * as "protocol/host:port".
     *
     * If the protocol part is missing, then assume TCP.
     * If the protocol part and host part are missing, then assume local.
     * If a "::" is found then assume DNET.
     */

    char	*mybuf, *tmpptr;
    const char	*_protocol;
    char	*_host, *_port;
    char	hostnamebuf[256];
    int		_host_len;

    prmsg (3,"ParseAddress(%s)\n", address);

    /* Copy the string so it can be changed */

    tmpptr = mybuf = strdup (address);

    /* Parse the string to get each component */

    /* Get the protocol part */

    _protocol = mybuf;


    if ( ((mybuf = strchr (mybuf,'/')) == NULL) &&
         ((mybuf = strrchr (tmpptr,':')) == NULL) )
    {
        /* address is in a bad format */
        *protocol = NULL;
        *host = NULL;
        *port = NULL;
        free (tmpptr);
        return 0;
    }

    if (*mybuf == ':')
    {
        /*
         * If there is a hostname, then assume tcp, otherwise
         * it must be local.
         */
        if (mybuf == tmpptr)
        {
            /* There is neither a protocol or host specified */
            _protocol = "local";
        }
        else
        {
            /* There is a hostname specified */
            _protocol = "tcp";
            mybuf = tmpptr;	/* reset to the begining of the host ptr */
        }
    }
    else
    {
        /* *mybuf == '/' */

        *mybuf ++= '\0'; /* put a null at the end of the protocol */

        if (strlen(_protocol) == 0)
        {
            /*
             * If there is a hostname, then assume tcp, otherwise
             * it must be local.
             */
            if (*mybuf != ':')
                _protocol = "tcp";
            else
                _protocol = "local";
        }
    }

    /* Get the host part */

    _host = mybuf;

    if ((mybuf = strrchr (mybuf,':')) == NULL)
    {
        *protocol = NULL;
        *host = NULL;
        *port = NULL;
        free (tmpptr);
        return 0;
    }

    *mybuf ++= '\0';

    _host_len = strlen(_host);
    if (_host_len == 0)
    {
        TRANS_ICE(GetHostname) (hostnamebuf, sizeof (hostnamebuf));
        _host = hostnamebuf;
    }
#if defined(IPv6) && defined(AF_INET6)
    /* hostname in IPv6 [numeric_addr]:0 form? */
    else if ( (_host_len > 3) &&
      ((strcmp(_protocol, "tcp") == 0) || (strcmp(_protocol, "inet6") == 0))
      && (*_host == '[') && (*(_host + _host_len - 1) == ']') ) {
	struct sockaddr_in6 sin6;

	*(_host + _host_len - 1) = '\0';

	/* Verify address is valid IPv6 numeric form */
	if (inet_pton(AF_INET6, _host + 1, &sin6) == 1) {
	    /* It is. Use it as such. */
	    _host++;
	    _protocol = "inet6";
	} else {
	    /* It's not, restore it just in case some other code can use it. */
	    *(_host + _host_len - 1) = ']';
	}
    }
#endif


    /* Get the port */

    _port = mybuf;

#if defined(FONT_t) || defined(FS_t)
    /*
     * Is there an optional catalogue list?
     */

    if ((mybuf = strchr (mybuf,'/')) != NULL)
	*mybuf ++= '\0';

    /*
     * The rest, if any, is the (currently unused) catalogue list.
     *
     * _catalogue = mybuf;
     */
#endif

#ifdef HAVE_LAUNCHD
    /* launchd sockets will look like 'local//tmp/launch-XgkNns/:0' */
    if(address != NULL && strlen(address)>8 && (!strncmp(address,"local//",7))) {
      _protocol="local";
      _host="";
      _port=address+6;
    }
#endif

    /*
     * Now that we have all of the components, allocate new
     * string space for them.
     */

    if ((*protocol = strdup (_protocol)) == NULL)
    {
        /* Malloc failed */
        *port = NULL;
        *host = NULL;
        *protocol = NULL;
        free (tmpptr);
        return 0;
    }

    if ((*host = strdup (_host)) == NULL)
    {
        /* Malloc failed */
        *port = NULL;
        *host = NULL;
        free (*protocol);
        *protocol = NULL;
        free (tmpptr);
        return 0;
    }

    if ((*port = strdup (_port)) == NULL)
    {
        /* Malloc failed */
        *port = NULL;
        free (*host);
        *host = NULL;
        free (*protocol);
        *protocol = NULL;
        free (tmpptr);
        return 0;
    }

    free (tmpptr);

    return 1;
}


/*
 * TRANS_ICE(Open) does all of the real work opening a connection. The only
 * funny part about this is the type parameter which is used to decide which
 * type of open to perform.
 */

static XtransConnInfo
TRANS_ICE(Open) (int type, const char *address)

{
    char 		*protocol = NULL, *host = NULL, *port = NULL;
    XtransConnInfo	ciptr = NULL;
    Xtransport		*thistrans;

    prmsg (2,"Open(%d,%s)\n", type, address);

#if defined(WIN32) && defined(TCPCONN)
    if (TRANS_ICE(WSAStartup)())
    {
	prmsg (1,"Open: WSAStartup failed\n");
	return NULL;
    }
#endif

    /* Parse the Address */

    if (TRANS_ICE(ParseAddress) (address, &protocol, &host, &port) == 0)
    {
        prmsg (1,"Open: Unable to Parse address %s\n", address);
        return NULL;
    }

    /* Determine the transport type */

    if ((thistrans = TRANS_ICE(SelectTransport) (protocol)) == NULL)
    {
        prmsg (1,"Open: Unable to find transport for %s\n",
               protocol);

        free (protocol);
        free (host);
        free (port);
        return NULL;
    }

    /* Open the transport */

    switch (type)
    {
        case XTRANS_OPEN_COTS_CLIENT:
#ifdef TRANS_CLIENT
            ciptr = thistrans->OpenCOTSClient(thistrans, protocol, host, port);
#endif /* TRANS_CLIENT */
            break;
        case XTRANS_OPEN_COTS_SERVER:
            ciptr = thistrans->OpenCOTSServer(thistrans, protocol, host, port);
            break;
        default:
            prmsg (1,"Open: Unknown Open type %d\n", type);
    }

    if (ciptr == NULL)
    {
        if (!(thistrans->flags & TRANS_DISABLED))
        {
            prmsg (1,"Open: transport open failed for %s/%s:%s\n",
                   protocol, host, port);
        }
        free (protocol);
        free (host);
        free (port);
        return NULL;
    }

    ciptr->transptr = thistrans;
    ciptr->port = port;			/* We need this for TRANS_ICE(Reopen) */

    free (protocol);
    free (host);

    return ciptr;
}

/*
 * We might want to create an XtransConnInfo object based on a previously
 * opened connection.  For example, the font server may clone itself and
 * pass file descriptors to the parent.
 */

static XtransConnInfo
TRANS_ICE(Reopen) (int type, int trans_id, int fd, const char *port)

{
    XtransConnInfo	ciptr = NULL;
    Xtransport		*thistrans = NULL;
    char		*save_port;
    int			i;

    prmsg (2,"Reopen(%d,%d,%s)\n", trans_id, fd, port);

    /* Determine the transport type */

    for (i = 0; i < NUMTRANS; i++)
	if (Xtransports[i].transport_id == trans_id)
	{
	    thistrans = Xtransports[i].transport;
	    break;
	}

    if (thistrans == NULL)
    {
	prmsg (1,"Reopen: Unable to find transport id %d\n",
	       trans_id);

	return NULL;
    }

    if ((save_port = strdup (port)) == NULL)
    {
	prmsg (1,"Reopen: Unable to malloc port string\n");

	return NULL;
    }

    /* Get a new XtransConnInfo object */

    switch (type)
    {
    case XTRANS_OPEN_COTS_SERVER:
	ciptr = thistrans->ReopenCOTSServer(thistrans, fd, port);
	break;
    default:
	prmsg (1,"Reopen: Bad Open type %d\n", type);
    }

    if (ciptr == NULL)
    {
	prmsg (1,"Reopen: transport open failed\n");
	free (save_port);
	return NULL;
    }

    ciptr->transptr = thistrans;
    ciptr->port = save_port;

    return ciptr;
}

/*
 * These are the public interfaces to this Transport interface.
 * These are the only functions that should have knowledge of the transport
 * table.
 */

#ifdef TRANS_CLIENT

XtransConnInfo
TRANS_ICE(OpenCOTSClient) (const char *address)

{
    prmsg (2,"OpenCOTSClient(%s)\n", address);
    return TRANS_ICE(Open) (XTRANS_OPEN_COTS_CLIENT, address);
}

#endif /* TRANS_CLIENT */

XtransConnInfo
TRANS_ICE(OpenCOTSServer) (const char *address)

{
    prmsg (2,"OpenCOTSServer(%s)\n", address);
    return TRANS_ICE(Open) (XTRANS_OPEN_COTS_SERVER, address);
}

XtransConnInfo
TRANS_ICE(ReopenCOTSServer) (int trans_id, int fd, const char *port)

{
    prmsg (2,"ReopenCOTSServer(%d, %d, %s)\n", trans_id, fd, port);
    return TRANS_ICE(Reopen) (XTRANS_OPEN_COTS_SERVER, trans_id, fd, port);
}

int
TRANS_ICE(GetReopenInfo) (XtransConnInfo ciptr,
		      int *trans_id, int *fd, char **port)

{
    int i;

    for (i = 0; i < NUMTRANS; i++)
	if (Xtransports[i].transport == ciptr->transptr)
	{
	    *trans_id = Xtransports[i].transport_id;
	    *fd = ciptr->fd;

	    if ((*port = strdup (ciptr->port)) == NULL)
		return 0;
	    else
		return 1;
	}

    return 0;
}


int
TRANS_ICE(SetOption) (XtransConnInfo ciptr, int option, int arg)

{
    int	fd = ciptr->fd;
    int	ret = 0;

    prmsg (2,"SetOption(%d,%d,%d)\n", fd, option, arg);

    /*
     * For now, all transport type use the same stuff for setting options.
     * As long as this is true, we can put the common code here. Once a more
     * complicated transport such as shared memory or an OSI implementation
     * that uses the session and application libraries is implemented, this
     * code may have to move to a transport dependent function.
     *
     * ret = ciptr->transptr->SetOption (ciptr, option, arg);
     */

    switch (option)
    {
        case TRANS_NONBLOCKING:
            switch (arg)
            {
                case 0:
                    /* Set to blocking mode */
                    break;
                case 1: /* Set to non-blocking mode */

#if defined(O_NONBLOCK)
                    ret = fcntl (fd, F_GETFL, 0);
                    if (ret != -1)
                        ret = fcntl (fd, F_SETFL, ret | O_NONBLOCK);
#else
#ifdef FIOSNBIO
                    {
	    int arg;
	    arg = 1;
	    ret = ioctl (fd, FIOSNBIO, &arg);
	}
#else
#if defined(WIN32)
                    {
#ifdef WIN32
	    u_long arg;
#else
	    int arg;
#endif
	    arg = 1;
/* IBM TCP/IP understands this option too well: it causes TRANS_ICE(Read) to fail
 * eventually with EWOULDBLOCK */
	    ret = ioctl (fd, FIONBIO, &arg);
	}
#else
                    ret = fcntl (fd, F_GETFL, 0);
#ifdef FNDELAY
                    ret = fcntl (fd, F_SETFL, ret | FNDELAY);
#else
                    ret = fcntl (fd, F_SETFL, ret | O_NDELAY);
#endif
#endif /* AIXV3  || uniosu */
#endif /* FIOSNBIO */
#endif /* O_NONBLOCK */
                    break;
                default:
                    /* Unknown option */
                    break;
            }
            break;
        case TRANS_CLOSEONEXEC:
#ifdef F_SETFD
            #ifdef FD_CLOEXEC
            ret = fcntl (fd, F_SETFD, FD_CLOEXEC);
#else
            ret = fcntl (fd, F_SETFD, 1);
#endif /* FD_CLOEXEC */
#endif /* F_SETFD */
            break;
    }

    return ret;
}

int
TRANS_ICE(CreateListener) (XtransConnInfo ciptr, const char *port, unsigned int flags)

{
    return ciptr->transptr->CreateListener (ciptr, port, flags);
}

int
TRANS_ICE(Received) (const char * protocol)

{
   Xtransport *trans;
   int i = 0, ret = 0;

   prmsg (5, "Received(%s)\n", protocol);

   if ((trans = TRANS_ICE(SelectTransport)(protocol)) == NULL)
   {
	prmsg (1,"Received: unable to find transport: %s\n",
	       protocol);

	return -1;
   }
   if (trans->flags & TRANS_ALIAS) {
       if (trans->nolisten)
	   while (trans->nolisten[i]) {
	       ret |= TRANS_ICE(Received)(trans->nolisten[i]);
	       i++;
       }
   }

   trans->flags |= TRANS_RECEIVED;
   return ret;
}

int
TRANS_ICE(NoListen) (const char * protocol)

{
   Xtransport *trans = TRANS_ICE(SelectTransport)(protocol);
   int i = 0, ret = 0;

   if (trans == NULL)
   {
	prmsg (1,"TransNoListen: unable to find transport: %s\n",
	       protocol);

	return -1;
   }
   if (trans->flags & TRANS_ALIAS) {
       if (trans->nolisten) {
           while (trans->nolisten[i]) {
               ret |= TRANS_ICE(NoListen)(trans->nolisten[i]);
               i++;
           }
       }
   }
   trans->flags |= TRANS_NOLISTEN;
   return ret;
}

int
TRANS_ICE(Listen) (const char * protocol)
{
   Xtransport *trans;
   int i = 0, ret = 0;

   if ((trans = TRANS_ICE(SelectTransport)(protocol)) == NULL)
   {
	prmsg (1,"TransListen: unable to find transport: %s\n",
	       protocol);

	return -1;
   }
   if (trans->flags & TRANS_ALIAS) {
       if (trans->nolisten)
	   while (trans->nolisten[i]) {
	       ret |= TRANS_ICE(Listen)(trans->nolisten[i]);
	       i++;
       }
   }
   trans->flags &= ~TRANS_NOLISTEN;
   return ret;
}

int
TRANS_ICE(IsListening) (const char * protocol)
{
   Xtransport *trans;

   if ((trans = TRANS_ICE(SelectTransport)(protocol)) == NULL)
   {
	prmsg (1,"TransIsListening: unable to find transport: %s\n",
	       protocol);

	return 0;
   }

   return !(trans->flags & TRANS_NOLISTEN);
}

int
TRANS_ICE(ResetListener) (XtransConnInfo ciptr)

{
    if (ciptr->transptr->ResetListener)
    return ciptr->transptr->ResetListener(ciptr);
    else
	return TRANS_RESET_NOOP;
}


XtransConnInfo
TRANS_ICE(Accept) (XtransConnInfo ciptr, int *status)

{
    XtransConnInfo	newciptr;

    prmsg (2,"Accept(%d)\n", ciptr->fd);

    newciptr = ciptr->transptr->Accept (ciptr, status);

    if (newciptr)
	newciptr->transptr = ciptr->transptr;

    return newciptr;
}


#ifdef TRANS_CLIENT

int
TRANS_ICE(Connect) (XtransConnInfo ciptr, const char *address)

{
    char	*protocol;
    char	*host;
    char	*port;
    int		ret;

    prmsg (2,"Connect(%d,%s)\n", ciptr->fd, address);

    if (TRANS_ICE(ParseAddress) (address, &protocol, &host, &port) == 0)
    {
        prmsg (1,"Connect: Unable to Parse address %s\n",
               address);
        return -1;
    }

#ifdef HAVE_LAUNCHD
    if (!host) host=strdup("");
#endif

    if (!port || !*port)
    {
        prmsg (1,"Connect: Missing port specification in %s\n",
               address);
        if (protocol) free (protocol);
        if (host) free (host);
        return -1;
    }

    ret = ciptr->transptr->Connect (ciptr, host, port);

    if (protocol) free (protocol);
    if (host) free (host);
    if (port) free (port);

    return ret;
}

#endif /* TRANS_CLIENT */


int
TRANS_ICE(BytesReadable) (XtransConnInfo ciptr, BytesReadable_t *pend)

{
    return ciptr->transptr->BytesReadable (ciptr, pend);
}

int
TRANS_ICE(Read) (XtransConnInfo ciptr, char *buf, int size)

{
    return ciptr->transptr->Read (ciptr, buf, size);
}

int
TRANS_ICE(Write) (XtransConnInfo ciptr, char *buf, int size)

{
    return ciptr->transptr->Write (ciptr, buf, size);
}

int
TRANS_ICE(Readv) (XtransConnInfo ciptr, struct iovec *buf, int size)

{
    return ciptr->transptr->Readv (ciptr, buf, size);
}

int
TRANS_ICE(Writev) (XtransConnInfo ciptr, struct iovec *buf, int size)

{
    return ciptr->transptr->Writev (ciptr, buf, size);
}

#if XTRANS_SEND_FDS
int
TRANS_ICE(SendFd) (XtransConnInfo ciptr, int fd, int do_close)
{
    return ciptr->transptr->SendFd(ciptr, fd, do_close);
}

int
TRANS_ICE(RecvFd) (XtransConnInfo ciptr)
{
    return ciptr->transptr->RecvFd(ciptr);
}
#endif

int
TRANS_ICE(Disconnect) (XtransConnInfo ciptr)

{
    return ciptr->transptr->Disconnect (ciptr);
}

int
TRANS_ICE(Close) (XtransConnInfo ciptr)

{
    int ret;

    prmsg (2,"Close(%d)\n", ciptr->fd);

    ret = ciptr->transptr->Close (ciptr);

    TRANS_ICE(FreeConnInfo) (ciptr);

    return ret;
}

int
TRANS_ICE(CloseForCloning) (XtransConnInfo ciptr)

{
    int ret;

    prmsg (2,"CloseForCloning(%d)\n", ciptr->fd);

    ret = ciptr->transptr->CloseForCloning (ciptr);

    TRANS_ICE(FreeConnInfo) (ciptr);

    return ret;
}

int
TRANS_ICE(IsLocal) (XtransConnInfo ciptr)

{
    return (ciptr->family == AF_UNIX);
}

int
TRANS_ICE(GetPeerAddr) (XtransConnInfo ciptr, int *familyp, int *addrlenp,
                    Xtransaddr **addrp)

{
    prmsg (2,"GetPeerAddr(%d)\n", ciptr->fd);

    *familyp = ciptr->family;
    *addrlenp = ciptr->peeraddrlen;

    if ((*addrp = malloc (ciptr->peeraddrlen)) == NULL)
    {
        prmsg (1,"GetPeerAddr: malloc failed\n");
        return -1;
    }
    memcpy(*addrp, ciptr->peeraddr, ciptr->peeraddrlen);

    return 0;
}


int
TRANS_ICE(GetConnectionNumber) (XtransConnInfo ciptr)

{
    return ciptr->fd;
}


/*
 * These functions are really utility functions, but they require knowledge
 * of the internal data structures, so they have to be part of the Transport
 * Independant API.
 */

static int
complete_network_count (void)

{
    int count = 0;
    int found_local = 0;
    int i;

    /*
     * For a complete network, we only need one LOCALCONN transport to work
     */

    for (i = 0; i < NUMTRANS; i++)
    {
	if (Xtransports[i].transport->flags & TRANS_ALIAS
   	 || Xtransports[i].transport->flags & TRANS_NOLISTEN)
	    continue;

	if (Xtransports[i].transport->flags & TRANS_LOCAL)
	    found_local = 1;
	else
	    count++;
    }

    return (count + found_local);
}


static int
receive_listening_fds(const char* port, XtransConnInfo* temp_ciptrs,
                      int* count_ret)

{
#ifdef HAVE_SYSTEMD_DAEMON
    XtransConnInfo ciptr;
    int i, systemd_listen_fds;

    systemd_listen_fds = sd_listen_fds(1);
    if (systemd_listen_fds < 0)
    {
        prmsg (1, "receive_listening_fds: sd_listen_fds error: %s\n",
               strerror(-systemd_listen_fds));
        return -1;
    }

    for (i = 0; i < systemd_listen_fds && *count_ret < NUMTRANS; i++)
    {
        struct sockaddr_storage a;
        int ti;
        const char* tn;
        socklen_t al;

        al = sizeof(a);
        if (getsockname(i + SD_LISTEN_FDS_START, (struct sockaddr*)&a, &al) < 0) {
            prmsg (1, "receive_listening_fds: getsockname error: %s\n",
                   strerror(errno));
            return -1;
        }

        switch (a.ss_family)
        {
        case AF_UNIX:
            ti = TRANS_SOCKET_UNIX_INDEX;
            if (*((struct sockaddr_un*)&a)->sun_path == '\0' &&
                al > sizeof(sa_family_t))
                tn = "local";
            else
                tn = "unix";
            break;
        case AF_INET:
            ti = TRANS_SOCKET_INET_INDEX;
            tn = "inet";
            break;
#if defined(IPv6) && defined(AF_INET6)
        case AF_INET6:
            ti = TRANS_SOCKET_INET6_INDEX;
            tn = "inet6";
            break;
#endif /* IPv6 */
        default:
            prmsg (1, "receive_listening_fds:"
                   "Got unknown socket address family\n");
            return -1;
        }

        ciptr = TRANS_ICE(ReopenCOTSServer)(ti, i + SD_LISTEN_FDS_START, port);
        if (!ciptr)
        {
            prmsg (1, "receive_listening_fds:"
                   "Got NULL while trying to reopen socket received from systemd.\n");
            return -1;
        }

        prmsg (5, "receive_listening_fds: received listener for %s, %d\n",
               tn, ciptr->fd);
        temp_ciptrs[(*count_ret)++] = ciptr;
        TRANS_ICE(Received)(tn);
    }
#endif /* HAVE_SYSTEMD_DAEMON */
    return 0;
}

#ifdef XQUARTZ_EXPORTS_LAUNCHD_FD
extern int xquartz_launchd_fd;
#endif

int
TRANS_ICE(MakeAllCOTSServerListeners) (const char *port, int *partial,
                                   int *count_ret, XtransConnInfo **ciptrs_ret)

{
    char		buffer[256]; /* ??? What size ?? */
    XtransConnInfo	ciptr, temp_ciptrs[NUMTRANS];
    int			status, i, j;

#if defined(IPv6) && defined(AF_INET6)
    int		ipv6_succ = 0;
#endif
    prmsg (2,"MakeAllCOTSServerListeners(%s,%p)\n",
	   port ? port : "NULL", ciptrs_ret);

    *count_ret = 0;

#ifdef XQUARTZ_EXPORTS_LAUNCHD_FD
    fprintf(stderr, "Launchd socket fd: %d\n", xquartz_launchd_fd);
    if(xquartz_launchd_fd != -1) {
        if((ciptr = TRANS_ICE(ReopenCOTSServer(TRANS_SOCKET_LOCAL_INDEX,
                                           xquartz_launchd_fd, getenv("DISPLAY"))))==NULL)
            fprintf(stderr,"Got NULL while trying to Reopen launchd port\n");
        else
            temp_ciptrs[(*count_ret)++] = ciptr;
    }
#endif

    if (receive_listening_fds(port, temp_ciptrs, count_ret) < 0) {
        return -1;
    }

    for (i = 0; i < NUMTRANS; i++)
    {
        Xtransport *trans = Xtransports[i].transport;
        unsigned int flags = 0;
        if (trans->flags&TRANS_ALIAS || trans->flags&TRANS_NOLISTEN ||
            trans->flags&TRANS_RECEIVED)
            continue;

        snprintf(buffer, sizeof(buffer), "%s/:%s",
             trans->TransName, port ? port : "");

        prmsg (5,"MakeAllCOTSServerListeners: opening %s\n",
               buffer);

        if ((ciptr = TRANS_ICE(OpenCOTSServer(buffer))) == NULL)
        {
            if (trans->flags & TRANS_DISABLED)
                continue;

            prmsg (1,
          "MakeAllCOTSServerListeners: failed to open listener for %s\n",
              trans->TransName);
            continue;
        }
    #if defined(IPv6) && defined(AF_INET6)
            if ((Xtransports[i].transport_id == TRANS_SOCKET_INET_INDEX
                 && ipv6_succ))
                flags |= ADDR_IN_USE_ALLOWED;
    #endif

        if ((status = TRANS_ICE(CreateListener (ciptr, port, flags))) < 0)
        {
            if (status == TRANS_ADDR_IN_USE)
            {
                /*
                 * We failed to bind to the specified address because the
                 * address is in use.  It must be that a server is already
                 * running at this address, and this function should fail.
                 */

                prmsg (1,
                "MakeAllCOTSServerListeners: server already running\n");

                for (j = 0; j < *count_ret; j++)
                    TRANS_ICE(Close) (temp_ciptrs[j]);

                *count_ret = 0;
                *ciptrs_ret = NULL;
                *partial = 0;
                return -1;
            }
            else
            {
                prmsg (1,
            "MakeAllCOTSServerListeners: failed to create listener for %s\n",
                  trans->TransName);

                continue;
            }
        }

    #if defined(IPv6) && defined(AF_INET6)
        if (Xtransports[i].transport_id == TRANS_SOCKET_INET6_INDEX)
            ipv6_succ = 1;
    #endif
        prmsg (5,
              "MakeAllCOTSServerListeners: opened listener for %s, %d\n",
              trans->TransName, ciptr->fd);

        temp_ciptrs[*count_ret] = ciptr;
        (*count_ret)++;
    }

    *partial = (*count_ret < complete_network_count());
    prmsg (5,
     "MakeAllCOTSServerListeners: partial=%d, actual=%d, complete=%d \n",
	*partial, *count_ret, complete_network_count());

    if (*count_ret > 0)
    {
        if ((*ciptrs_ret = malloc (
            *count_ret * sizeof (XtransConnInfo))) == NULL)
        {
            return -1;
        }

        for (i = 0; i < *count_ret; i++)
        {
            (*ciptrs_ret)[i] = temp_ciptrs[i];
        }
    }
    else
	*ciptrs_ret = NULL;

    return 0;
}



/*
 * These routines are not part of the X Transport Interface, but they
 * may be used by it.
 */


#ifdef WIN32

/*
 * emulate readv
 */

static int TRANS_ICE(ReadV) (XtransConnInfo ciptr, struct iovec *iov, int iovcnt)

{
    int i, len, total;
    char *base;

    ESET(0);
    for (i = 0, total = 0;  i < iovcnt;  i++, iov++) {
	len = iov->iov_len;
	base = iov->iov_base;
	while (len > 0) {
	    register int nbytes;
	    nbytes = TRANS_ICE(Read) (ciptr, base, len);
	    if (nbytes < 0 && total == 0)  return -1;
	    if (nbytes <= 0)  return total;
	    ESET(0);
	    len   -= nbytes;
	    total += nbytes;
	    base  += nbytes;
	}
    }
    return total;
}


/*
 * emulate writev
 */

static int TRANS_ICE(WriteV) (XtransConnInfo ciptr, struct iovec *iov, int iovcnt)

{
    int i, len, total;
    char *base;

    ESET(0);
    for (i = 0, total = 0;  i < iovcnt;  i++, iov++) {
	len = iov->iov_len;
	base = iov->iov_base;
	while (len > 0) {
	    register int nbytes;
	    nbytes = TRANS_ICE(Write) (ciptr, base, len);
	    if (nbytes < 0 && total == 0)  return -1;
	    if (nbytes <= 0)  return total;
	    ESET(0);
	    len   -= nbytes;
	    total += nbytes;
	    base  += nbytes;
	}
    }
    return total;
}

#endif /* WIN32 */


#if defined(_POSIX_SOURCE) || defined(SVR4) || defined(__SVR4)
#ifndef NEED_UTSNAME
#define NEED_UTSNAME
#endif
#include <sys/utsname.h>
#endif

/*
 * TRANS_ICE(GetHostname) - similar to gethostname but allows special processing.
 */

int TRANS_ICE(GetHostname) (char *buf, int maxlen)

{
    int len;

#ifdef NEED_UTSNAME
    struct utsname name;

    uname (&name);
    len = strlen (name.nodename);
    if (len >= maxlen) len = maxlen - 1;
    strncpy (buf, name.nodename, len);
    buf[len] = '\0';
#else
    buf[0] = '\0';
    (void) gethostname (buf, maxlen);
    buf [maxlen - 1] = '\0';
    len = strlen(buf);
#endif /* NEED_UTSNAME */
    return len;
}
