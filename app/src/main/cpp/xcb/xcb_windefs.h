#ifndef _XCB_WINDEFS_H
#define _XCB_WINDEFS_H

#ifndef WINVER
#define WINVER 0x0501 /* required for getaddrinfo/freeaddrinfo defined only for WinXP and above */
#endif

#include "../windows/winsock2.h"
#include "../windows/ws2tcpip.h"
#include "../windows/windef.h"

struct iovec  {
    void *iov_base;    /* Pointer to data.  */
    int iov_len;       /* Length of data.  */
};

typedef unsigned int in_addr_t;

#endif /* xcb_windefs.h */
