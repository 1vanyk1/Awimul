#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
//#include "../ipc/fcntl.h"


int
isastream (fildes)
        int fildes;
{
    /* In general we do not have a STREAMS implementation and therefore
       return 0.  But for invalid file descriptors we have to return an
       error.  */
    if (fcntl (fildes, F_GETFD) < 0)
        return -1;

    /* No STREAM.  */
    return 0;
}