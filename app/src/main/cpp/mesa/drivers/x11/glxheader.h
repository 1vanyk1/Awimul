#ifndef GLX_HEADER_H
#define GLX_HEADER_H

#include "../../main/glheader.h"


# include "../../../x11/headers/xlib.h"
# include "../../../x11/headers/xlibint.h"
# include "../../../x11/headers/xutil.h"
# ifdef USE_XSHM  /* was SHM */
#  include <sys/ipc.h>
#  include <sys/shm.h>
//#  include "../../../ipc/sys/ipc.h"
//#  include "../../../ipc/sys/shm.h"
#  include "../../../x11/headers/extensions/XShm.h"
# endif
# include "../../../GL/glx.h"
# include <sys/time.h>




/* this silences a compiler warning on several systems */
struct timespec;
struct itimerspec;


#endif /*GLX_HEADER*/
