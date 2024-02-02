#ifndef _XTRANS_H_
#define _XTRANS_H_

#include "../xfuncproto.h"
#include "../xos.h"
#include "../xmd.h"

#ifndef WIN32
#include <sys/socket.h>
#endif

#define TRANS_CLIENT

#ifdef __clang__
/* Not all clients make use of all provided statics */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif

/*
 * Set the functions names according to where this code is being compiled.
 */

#ifdef X11_t
#define TRANS(func) _X11Trans##func
#ifdef XTRANSDEBUG
static const char *__xtransname = "_X11Trans";
#endif
#endif /* X11_t */

#define TRANS_XSERV(func) _XSERVTrans##func

#define TRANS_ICE(func) _IceTrans##func

#define TRANS(func) _XimXTrans##func

#ifdef FS_t
#define TRANS(func) _FSTrans##func
#ifdef XTRANSDEBUG
static const char *__xtransname = "_FSTrans";
#endif
#endif /* FS_t */

#ifdef FONT_t
#define TRANS(func) _FontTrans##func
#ifdef XTRANSDEBUG
static const char *__xtransname = "_FontTrans";
#endif
#endif /* FONT_t */

#ifdef ICE_t
#define TRANS(func) _IceTrans##func
#ifdef XTRANSDEBUG
static const char *__xtransname = "_IceTrans";
#endif
#endif /* ICE_t */

#ifndef TRANS
#define TRANS(func) _XTrans##func
#endif

#if defined(XSERV_t)
#ifdef XTRANSDEBUG
static const char *__xtransname = "_XSERVTrans";
#endif
#define X11_t
#elif defined(XIM_t)
#ifdef XTRANSDEBUG
static const char *__xtransname = "_XimTrans";
#endif
#else
#ifdef XTRANSDEBUG
static const char *__xtransname = "_XTrans";
#endif
#endif /* !TRANS */

#ifdef __clang__
#pragma clang diagnostic pop
#endif

/*
 * Create a single address structure that can be used wherever
 * an address structure is needed. struct sockaddr is not big enough
 * to hold a sockadd_un, so we create this definition to have a single
 * structure that is big enough for all the structures we might need.
 *
 * This structure needs to be independent of the socket/TLI interface used.
 */

#if defined(IPv6) && defined(AF_INET6)
typedef struct sockaddr_storage Xtransaddr;
#else
#define XTRANS_MAX_ADDR_LEN	128	/* large enough to hold sun_path */

typedef	struct {
    unsigned char	addr[XTRANS_MAX_ADDR_LEN];
} Xtransaddr;
#endif

#ifdef LONG64
typedef int BytesReadable_t;
#else
typedef long BytesReadable_t;
#endif


#if defined(WIN32)

/*
 *      TRANS(Readv) and TRANS(Writev) use struct iovec, normally found
 *      in Berkeley systems in <sys/uio.h>.  See the readv(2) and writev(2)
 *      manual pages for details.
 */

struct iovec {
    caddr_t iov_base;
    int iov_len;
};

#else
#include <sys/uio.h>
#endif

typedef struct _XtransConnInfo *XtransConnInfo;


/*
 * Transport Option definitions
 */

#define TRANS_NONBLOCKING	1
#define	TRANS_CLOSEONEXEC	2


/*
 * Return values of Connect (0 is success)
 */

#define TRANS_CONNECT_FAILED 	-1
#define TRANS_TRY_CONNECT_AGAIN -2
#define TRANS_IN_PROGRESS	-3


/*
 * Return values of CreateListener (0 is success)
 */

#define TRANS_CREATE_LISTENER_FAILED 	-1
#define TRANS_ADDR_IN_USE		-2


/*
 * Return values of Accept (0 is success)
 */

#define TRANS_ACCEPT_BAD_MALLOC			-1
#define TRANS_ACCEPT_FAILED 			-2
#define TRANS_ACCEPT_MISC_ERROR			-3


/*
 * ResetListener return values
 */

#define TRANS_RESET_NOOP	1
#define TRANS_RESET_NEW_FD	2
#define TRANS_RESET_FAILURE	3


/*
 * Function prototypes for the exposed interface
 */

void TRANS(FreeConnInfo) (
        XtransConnInfo 	/* ciptr */
);

#ifdef TRANS_CLIENT

XtransConnInfo TRANS(OpenCOTSClient)(
        const char *	/* address */
);

#endif /* TRANS_CLIENT */

XtransConnInfo TRANS(OpenCOTSServer)(
    const char *	/* address */
);

XtransConnInfo TRANS(ReopenCOTSServer)(
    int,		/* trans_id */
    int,		/* fd */
    const char *	/* port */
);

int TRANS(GetReopenInfo)(
    XtransConnInfo,	/* ciptr */
    int *,		/* trans_id */
    int *,		/* fd */
    char **		/* port */
);


int TRANS(SetOption)(
        XtransConnInfo,	/* ciptr */
        int,		/* option */
        int			/* arg */
);

#ifdef TRANS_SERVER

int TRANS(CreateListener)(
    XtransConnInfo,	/* ciptr */
    const char *,	/* port */
    unsigned int	/* flags */
);

int TRANS(Received) (
    const char*         /* protocol*/
);

int TRANS(NoListen) (
    const char*         /* protocol*/
);

int TRANS(Listen) (
    const char*         /* protocol*/
);

int TRANS(IsListening) (
    const char*         /* protocol*/
);

int TRANS(ResetListener)(
    XtransConnInfo	/* ciptr */
);

XtransConnInfo TRANS(Accept)(
    XtransConnInfo,	/* ciptr */
    int *		/* status */
);

#endif /* TRANS_SERVER */

#ifdef TRANS_CLIENT

int TRANS(Connect)(
        XtransConnInfo,	/* ciptr */
        const char *	/* address */
);

#endif /* TRANS_CLIENT */

int TRANS(BytesReadable)(
        XtransConnInfo,	/* ciptr */
        BytesReadable_t *	/* pend */
);

int TRANS(Read)(
        XtransConnInfo,	/* ciptr */
        char *,		/* buf */
        int			/* size */
);

int TRANS(Write)(
        XtransConnInfo,	/* ciptr */
        char *,		/* buf */
        int			/* size */
);

int TRANS(Readv)(
        XtransConnInfo,	/* ciptr */
        struct iovec *,	/* buf */
        int			/* size */
);

int TRANS(Writev)(
        XtransConnInfo,	/* ciptr */
        struct iovec *,	/* buf */
        int			/* size */
);

int TRANS(SendFd) (XtransConnInfo ciptr, int fd, int do_close);

int TRANS(RecvFd) (XtransConnInfo ciptr);

int TRANS(Disconnect)(
        XtransConnInfo	/* ciptr */
);

int TRANS(Close)(
        XtransConnInfo	/* ciptr */
);

int TRANS(CloseForCloning)(
        XtransConnInfo	/* ciptr */
);

int TRANS(IsLocal)(
        XtransConnInfo	/* ciptr */
);

int TRANS(GetPeerAddr)(
        XtransConnInfo,	/* ciptr */
        int *,		/* familyp */
        int *,		/* addrlenp */
        Xtransaddr **	/* addrp */
);

int TRANS(GetConnectionNumber)(
        XtransConnInfo	/* ciptr */
);

#ifdef TRANS_SERVER

int TRANS(MakeAllCOTSServerListeners)(
    const char *,	/* port */
    int *,		/* partial */
    int *,		/* count_ret */
    XtransConnInfo **	/* ciptrs_ret */
);

#endif /* TRANS_SERVER */


/*
 * Function Prototypes for Utility Functions.
 */

#ifdef X11_t

int TRANS(ConvertAddress)(
    int *,		/* familyp */
    int *,		/* addrlenp */
    Xtransaddr **	/* addrp */
);

#endif /* X11_t */

#ifdef ICE_t

char *
TRANS(GetMyNetworkId)(
    XtransConnInfo	/* ciptr */
);

char *
TRANS(GetPeerNetworkId)(
    XtransConnInfo	/* ciptr */
);

#endif /* ICE_t */

int
TRANS(GetHostname) (
        char *	/* buf */,
        int 	/* maxlen */
);

#if defined(WIN32) && defined(TCPCONN)
int TRANS(WSAStartup)();
#endif





void TRANS_ICE(FreeConnInfo) (
        XtransConnInfo 	/* ciptr */
);

#ifdef TRANS_CLIENT

XtransConnInfo TRANS_ICE(OpenCOTSClient)(
        const char *	/* address */
);

#endif /* TRANS_CLIENT */

XtransConnInfo TRANS_ICE(OpenCOTSServer)(
        const char *	/* address */
);

XtransConnInfo TRANS_ICE(ReopenCOTSServer)(
        int,		/* trans_id */
        int,		/* fd */
        const char *	/* port */
);

int TRANS_ICE(GetReopenInfo)(
        XtransConnInfo,	/* ciptr */
        int *,		/* trans_id */
        int *,		/* fd */
        char **		/* port */
);


int TRANS_ICE(SetOption)(
        XtransConnInfo,	/* ciptr */
        int,		/* option */
        int			/* arg */
);

int TRANS_ICE(CreateListener)(
        XtransConnInfo,	/* ciptr */
        const char *,	/* port */
        unsigned int	/* flags */
);

int TRANS_ICE(Received) (
        const char*         /* protocol*/
);

int TRANS_ICE(NoListen) (
        const char*         /* protocol*/
);

int TRANS_ICE(Listen) (
        const char*         /* protocol*/
);

int TRANS_ICE(IsListening) (
        const char*         /* protocol*/
);

int TRANS_ICE(ResetListener)(
        XtransConnInfo	/* ciptr */
);

XtransConnInfo TRANS_ICE(Accept)(
        XtransConnInfo,	/* ciptr */
        int *		/* status */
);

#ifdef TRANS_CLIENT

int TRANS_ICE(Connect)(
        XtransConnInfo,	/* ciptr */
        const char *	/* address */
);

#endif /* TRANS_CLIENT */

int TRANS_ICE(BytesReadable)(
        XtransConnInfo,	/* ciptr */
        BytesReadable_t *	/* pend */
);

int TRANS_ICE(Read)(
        XtransConnInfo,	/* ciptr */
        char *,		/* buf */
        int			/* size */
);

int TRANS_ICE(Write)(
        XtransConnInfo,	/* ciptr */
        char *,		/* buf */
        int			/* size */
);

int TRANS_ICE(Readv)(
        XtransConnInfo,	/* ciptr */
        struct iovec *,	/* buf */
        int			/* size */
);

int TRANS_ICE(Writev)(
        XtransConnInfo,	/* ciptr */
        struct iovec *,	/* buf */
        int			/* size */
);

int TRANS_ICE(SendFd) (XtransConnInfo ciptr, int fd, int do_close);

int TRANS_ICE(RecvFd) (XtransConnInfo ciptr);

int TRANS_ICE(Disconnect)(
        XtransConnInfo	/* ciptr */
);

int TRANS_ICE(Close)(
        XtransConnInfo	/* ciptr */
);

int TRANS_ICE(CloseForCloning)(
        XtransConnInfo	/* ciptr */
);

int TRANS_ICE(IsLocal)(
        XtransConnInfo	/* ciptr */
);

int TRANS_ICE(GetPeerAddr)(
        XtransConnInfo,	/* ciptr */
        int *,		/* familyp */
        int *,		/* addrlenp */
        Xtransaddr **	/* addrp */
);

int TRANS_ICE(GetConnectionNumber)(
        XtransConnInfo	/* ciptr */
);

int TRANS_ICE(MakeAllCOTSServerListeners)(
        const char *,	/* port */
        int *,		/* partial */
        int *,		/* count_ret */
        XtransConnInfo **	/* ciptrs_ret */
);


/*
 * Function Prototypes for Utility Functions.
 */

#ifdef X11_t

int TRANS_ICE(ConvertAddress)(
    int *,		/* familyp */
    int *,		/* addrlenp */
    Xtransaddr **	/* addrp */
);

#endif /* X11_t */

//#ifdef ICE_t

char *
TRANS_ICE(GetMyNetworkId)(
    XtransConnInfo	/* ciptr */
);

char *
TRANS_ICE(GetPeerNetworkId)(
    XtransConnInfo	/* ciptr */
);

//#endif /* ICE_t */

int
TRANS_ICE(GetHostname) (
        char *	/* buf */,
        int 	/* maxlen */
);

#if defined(WIN32) && defined(TCPCONN)
int TRANS_ICE(WSAStartup)();
#endif





void TRANS_XSERV(FreeConnInfo) (
        XtransConnInfo 	/* ciptr */
);

#ifdef TRANS_CLIENT

XtransConnInfo TRANS_XSERV(OpenCOTSClient)(
        const char *	/* address */
);

#endif /* TRANS_CLIENT */

XtransConnInfo TRANS_XSERV(OpenCOTSServer)(
        const char *	/* address */
);

XtransConnInfo TRANS_XSERV(ReopenCOTSServer)(
        int,		/* trans_id */
        int,		/* fd */
        const char *	/* port */
);

int TRANS_XSERV(GetReopenInfo)(
        XtransConnInfo,	/* ciptr */
        int *,		/* trans_id */
        int *,		/* fd */
        char **		/* port */
);


int TRANS_XSERV(SetOption)(
        XtransConnInfo,	/* ciptr */
        int,		/* option */
        int			/* arg */
);

int TRANS_XSERV(CreateListener)(
        XtransConnInfo,	/* ciptr */
        const char *,	/* port */
        unsigned int	/* flags */
);

int TRANS_XSERV(Received) (
        const char*         /* protocol*/
);

int TRANS_XSERV(NoListen) (
        const char*         /* protocol*/
);

int TRANS_XSERV(Listen) (
        const char*         /* protocol*/
);

int TRANS_XSERV(IsListening) (
        const char*         /* protocol*/
);

int TRANS_XSERV(ResetListener)(
        XtransConnInfo	/* ciptr */
);

XtransConnInfo TRANS_XSERV(Accept)(
        XtransConnInfo,	/* ciptr */
        int *		/* status */
);

#ifdef TRANS_CLIENT

int TRANS_XSERV(Connect)(
        XtransConnInfo,	/* ciptr */
        const char *	/* address */
);

#endif /* TRANS_CLIENT */

int TRANS_XSERV(BytesReadable)(
        XtransConnInfo,	/* ciptr */
        BytesReadable_t *	/* pend */
);

int TRANS_XSERV(Read)(
        XtransConnInfo,	/* ciptr */
        char *,		/* buf */
        int			/* size */
);

int TRANS_XSERV(Write)(
        XtransConnInfo,	/* ciptr */
        char *,		/* buf */
        int			/* size */
);

int TRANS_XSERV(Readv)(
        XtransConnInfo,	/* ciptr */
        struct iovec *,	/* buf */
        int			/* size */
);

int TRANS_XSERV(Writev)(
        XtransConnInfo,	/* ciptr */
        struct iovec *,	/* buf */
        int			/* size */
);

int TRANS_XSERV(SendFd) (XtransConnInfo ciptr, int fd, int do_close);

int TRANS_XSERV(RecvFd) (XtransConnInfo ciptr);

int TRANS_XSERV(Disconnect)(
        XtransConnInfo	/* ciptr */
);

int TRANS_XSERV(Close)(
        XtransConnInfo	/* ciptr */
);

int TRANS_XSERV(CloseForCloning)(
        XtransConnInfo	/* ciptr */
);

int TRANS_XSERV(IsLocal)(
        XtransConnInfo	/* ciptr */
);

int TRANS_XSERV(GetPeerAddr)(
        XtransConnInfo,	/* ciptr */
        int *,		/* familyp */
        int *,		/* addrlenp */
        Xtransaddr **	/* addrp */
);

int TRANS_XSERV(GetConnectionNumber)(
        XtransConnInfo	/* ciptr */
);

int TRANS_XSERV(MakeAllCOTSServerListeners)(
        const char *,	/* port */
        int *,		/* partial */
        int *,		/* count_ret */
        XtransConnInfo **	/* ciptrs_ret */
);


/*
 * Function Prototypes for Utility Functions.
 */

#ifdef X11_t

int TRANS_XSERV(ConvertAddress)(
    int *,		/* familyp */
    int *,		/* addrlenp */
    Xtransaddr **	/* addrp */
);

#endif /* X11_t */

#ifdef ICE_t

char *
TRANS_XSERV(GetMyNetworkId)(
    XtransConnInfo	/* ciptr */
);

char *
TRANS_XSERV(GetPeerNetworkId)(
    XtransConnInfo	/* ciptr */
);

#endif /* ICE_t */

int
TRANS_XSERV(GetHostname) (
        char *	/* buf */,
        int 	/* maxlen */
);

#if defined(WIN32) && defined(TCPCONN)
int TRANS_XSERV(WSAStartup)();
#endif





#endif /* _XTRANS_H_ */
