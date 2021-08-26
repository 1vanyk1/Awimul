#include "errno.h"

//int __set_errno(int n)
//{
//    errno = n;
//    return -1;
//}

int fattach (int fildes, const char *path)
{
    __set_errno (ENOSYS);
    return -1;
}