#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../headers/xosdefs.h"
#include <string.h>
#include <unistd.h>

#ifdef WIN32
#include "../headers/xlibint.h"
#include "../headers/xwinsock.h"
#endif

#ifdef USG
#define NEED_UTSNAME
#endif

#ifdef NEED_UTSNAME
#include <sys/utsname.h>
#endif

#include "SysUtil.h"

int
XmuGetHostname(char *buf, int maxlen)
{
    int len;
#ifdef WIN32
    static WSADATA wsadata;

    if (!wsadata.wVersion && WSAStartup(MAKEWORD(2,2), &wsadata))
	return -1;
#endif

#ifdef NEED_UTSNAME
    /*
     * same host name crock as in server and xinit.
     */
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
#endif /* hpux */
    return len;
}
