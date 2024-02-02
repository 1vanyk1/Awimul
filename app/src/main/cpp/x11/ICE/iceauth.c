#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "ICElib.h"
#include "ICElibint.h"
#include "ICEutil.h"

#include <time.h>

#ifdef HAVE_LIBBSD
#include <bsd/stdlib.h>	/* for arc4random_buf() */
#endif

#include <unistd.h>

static int was_called_state;

#ifndef HAVE_ARC4RANDOM_BUF

static void
emulate_getrandom_buf (
        char *auth,
        int len
)
{
    long    ldata[2];
    int	    seed;
    int	    value;
    int	    i;

#ifdef ITIMER_REAL
    {
	struct timeval  now;
	X_GETTIMEOFDAY (&now);
	ldata[0] = now.tv_sec;
	ldata[1] = now.tv_usec;
    }
#else /* ITIMER_REAL */
    {
        long    time ();
        ldata[0] = time ((long *) 0);
        ldata[1] = getpid ();
    }
#endif /* ITIMER_REAL */
    seed = (ldata[0]) + (ldata[1] << 16);
    srand (seed);
    for (i = 0; i < len; i++)
    {
        value = rand ();
        auth[i] = value & 0xff;
    }
}

static void
arc4random_buf1 (
        char *auth,
        int len
)
{
#if HAVE_GETENTROPY
    int	    ret;

    /* weak emulation of arc4random through the entropy libc */
    ret = getentropy (auth, len);
    if (ret == 0)
	return;
#endif /* HAVE_GETENTROPY */

    emulate_getrandom_buf (auth, len);
}

#endif /* !defined(HAVE_ARC4RANDOM_BUF) */

/*
 * MIT-MAGIC-COOKIE-1 is a sample authentication method implemented by
 * the SI.  It is not part of standard ICElib.
 */


char *
IceGenerateMagicCookie (
        int len
)
{
    char    *auth;

    if ((auth = malloc (len + 1)) == NULL)
        return (NULL);

    arc4random_buf1 (auth, len);

    auth[len] = '\0';
    return (auth);
}



IcePoAuthStatus
_IcePoMagicCookie1Proc (
        IceConn		iceConn,
        IcePointer	*authStatePtr,
        Bool 		cleanUp,
        Bool		swap,
        int     	authDataLen,
        IcePointer	authData,
        int 		*replyDataLenRet,
        IcePointer	*replyDataRet,
        char    	**errorStringRet
)
{
    if (cleanUp)
    {
        /*
         * We didn't allocate any state.  We're done.
         */

        return (IcePoAuthDoneCleanup);
    }

    *errorStringRet = NULL;

    if (*authStatePtr == NULL)
    {
        /*
         * This is the first time we're being called.  Search the
         * authentication data for the first occurence of
         * MIT-MAGIC-COOKIE-1 that matches iceConn->connection_string.
         */

        unsigned short  length;
        char		*data;

        _IceGetPoAuthData ("ICE", iceConn->connection_string,
                           "MIT-MAGIC-COOKIE-1", &length, &data);

        if (!data)
        {
            const char *tempstr =
                    "Could not find correct MIT-MAGIC-COOKIE-1 authentication";

            *errorStringRet = strdup(tempstr);

            return (IcePoAuthFailed);
        }
        else
        {
            *authStatePtr = (IcePointer) &was_called_state;

            *replyDataLenRet = length;
            *replyDataRet = data;

            return (IcePoAuthHaveReply);
        }
    }
    else
    {
        /*
         * We should never get here for MIT-MAGIC-COOKIE-1 since it is
         * a single pass authentication method.
         */

        const char *tempstr =
                "MIT-MAGIC-COOKIE-1 authentication internal error";

        *errorStringRet = strdup(tempstr);

        return (IcePoAuthFailed);
    }
}

IcePoAuthProc	_IcePoAuthProcs[] = {_IcePoMagicCookie1Proc};


IcePaAuthStatus
_IcePaMagicCookie1Proc (
        IceConn		iceConn,
        IcePointer	*authStatePtr,
        Bool		swap,
        int     	authDataLen,
        IcePointer	authData,
        int 		*replyDataLenRet,
        IcePointer	*replyDataRet,
        char    	**errorStringRet
)
{
    *errorStringRet = NULL;
    *replyDataLenRet = 0;
    *replyDataRet = NULL;

    if (*authStatePtr == NULL)
    {
        /*
         * This is the first time we're being called.  We don't have
         * any data to pass to the other client.
         */

        *authStatePtr = (IcePointer) &was_called_state;

        return (IcePaAuthContinue);
    }
    else
    {
        /*
         * Search the authentication data for the first occurence of
         * MIT-MAGIC-COOKIE-1 that matches iceConn->connection_string.
         */

        unsigned short  length;
        char		*data;

        _IceGetPaAuthData ("ICE", iceConn->connection_string,
                           "MIT-MAGIC-COOKIE-1", &length, &data);

        if (data)
        {
            IcePaAuthStatus stat;

            if (authDataLen == length &&
                memcmp (authData, data, authDataLen) == 0)
            {
                stat = IcePaAuthAccepted;
            }
            else
            {
                const char *tempstr
                        = "MIT-MAGIC-COOKIE-1 authentication rejected";

                *errorStringRet = strdup(tempstr);

                stat = IcePaAuthRejected;
            }

            free (data);
            return (stat);
        }
        else
        {
            /*
             * We should never get here because in the ConnectionReply
             * we should have passed all the valid methods.  So we should
             * always find a valid entry.
             */

            const char *tempstr =
                    "MIT-MAGIC-COOKIE-1 authentication internal error";

            *errorStringRet = strdup(tempstr);

            return (IcePaAuthFailed);
        }
    }
}

IcePaAuthProc	_IcePaAuthProcs[] = {_IcePaMagicCookie1Proc};
