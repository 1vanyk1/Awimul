#ifndef _XSERVER_POLL_H_
#define _XSERVER_POLL_H_

#ifndef _DIX_CONFIG_H_
//#error must include dix-config.h to use xserver_poll.h
#endif

#ifdef HAVE_POLL
#include <poll.h>
#define xserver_poll(fds, nfds, timeout) poll(fds, nfds, timeout)
#else

#define POLLIN		0x01
#define POLLPRI		0x02
#define POLLOUT		0x04
#define POLLERR		0x08
#define POLLHUP		0x10
#define POLLNVAL	0x20

struct pollfd
{
    int     fd;
    short   events;
    short   revents;
};

typedef unsigned long nfds_t;

int xserver_poll (struct pollfd *pArray, nfds_t n_fds, int timeout);

#endif

#endif /* _XSERVER_POLL_H_ */
