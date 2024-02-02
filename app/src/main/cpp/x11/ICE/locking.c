#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "ICElib.h"
#include "ICElibint.h"


/*
 * NOT IMPLEMENTED YET
 */


Status
IceInitThreads (
        void
)
{
    return (0);
}


void
IceAppLockConn (
        IceConn	iceConn
)
{
    IceLockConn (iceConn);
}


void
IceAppUnlockConn (
        IceConn iceConn
)
{
    IceUnlockConn (iceConn);
}
