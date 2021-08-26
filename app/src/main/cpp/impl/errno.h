#ifndef AWIMUL_ERRNO_H
#define AWIMUL_ERRNO_H
#include <errno.h>

# define __set_errno(val) (errno = (val))

extern int fattach (int fildes, const char *path);

#endif //AWIMUL_ERRNO_H
