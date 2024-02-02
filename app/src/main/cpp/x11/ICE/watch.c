#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "ICElib.h"
#include "ICElibint.h"


Status
IceAddConnectionWatch (
        IceWatchProc	watchProc,
        IcePointer	clientData
)
{
    /*
     * watchProc will be called each time an ICE connection is
     * created/destroyed by ICElib.
     */

    _IceWatchProc	*ptr = _IceWatchProcs;
    _IceWatchProc	*newWatchProc;
    int			i;

    if ((newWatchProc = malloc (sizeof (_IceWatchProc))) == NULL)
    {
        return (0);
    }

    newWatchProc->watch_proc = watchProc;
    newWatchProc->client_data = clientData;
    newWatchProc->watched_connections = NULL;
    newWatchProc->next = NULL;

    while (ptr && ptr->next)
        ptr = ptr->next;

    if (ptr == NULL)
        _IceWatchProcs = newWatchProc;
    else
        ptr->next = newWatchProc;


    /*
     * Invoke the watch proc with any previously opened ICE connections.
     */

    for (i = 0; i < _IceConnectionCount; i++)
    {
        _IceWatchedConnection *newWatchedConn =
                malloc (sizeof (_IceWatchedConnection));

        newWatchedConn->iceConn = _IceConnectionObjs[i];
        newWatchedConn->next = NULL;

        newWatchProc->watched_connections = newWatchedConn;

        (*newWatchProc->watch_proc) (_IceConnectionObjs[i],
                                     newWatchProc->client_data, True, &newWatchedConn->watch_data);
    }

    return (1);
}



void
IceRemoveConnectionWatch (
        IceWatchProc	watchProc,
        IcePointer	clientData
)
{
    _IceWatchProc	*currWatchProc = _IceWatchProcs;
    _IceWatchProc	*prevWatchProc = NULL;

    while (currWatchProc && (currWatchProc->watch_proc != watchProc ||
                             currWatchProc->client_data != clientData))
    {
        prevWatchProc = currWatchProc;
        currWatchProc = currWatchProc->next;
    }

    if (currWatchProc)
    {
        _IceWatchProc		*nextWatchProc = currWatchProc->next;
        _IceWatchedConnection 	*watchedConn;

        watchedConn = currWatchProc->watched_connections;
        while (watchedConn)
        {
            _IceWatchedConnection *nextWatchedConn = watchedConn->next;
            free (watchedConn);
            watchedConn = nextWatchedConn;
        }

        if (prevWatchProc == NULL)
            _IceWatchProcs = nextWatchProc;
        else
            prevWatchProc->next = nextWatchProc;

        free (currWatchProc);
    }
}



void
_IceConnectionOpened (
        IceConn	iceConn
)
{
    _IceWatchProc *watchProc = _IceWatchProcs;

    while (watchProc)
    {
        _IceWatchedConnection *newWatchedConn =
                malloc (sizeof (_IceWatchedConnection));
        _IceWatchedConnection *watchedConn;

        watchedConn = watchProc->watched_connections;
        while (watchedConn && watchedConn->next)
            watchedConn = watchedConn->next;

        newWatchedConn->iceConn = iceConn;
        newWatchedConn->next = NULL;

        if (watchedConn == NULL)
            watchProc->watched_connections = newWatchedConn;
        else
            watchedConn->next = newWatchedConn;

        (*watchProc->watch_proc) (iceConn,
                                  watchProc->client_data, True, &newWatchedConn->watch_data);

        watchProc = watchProc->next;
    }
}



void
_IceConnectionClosed (
        IceConn	iceConn
)
{
    _IceWatchProc *watchProc = _IceWatchProcs;

    while (watchProc)
    {
        _IceWatchedConnection *watchedConn = watchProc->watched_connections;
        _IceWatchedConnection *prev = NULL;

        while (watchedConn && watchedConn->iceConn != iceConn)
        {
            prev = watchedConn;
            watchedConn = watchedConn->next;
        }

        if (watchedConn)
        {
            (*watchProc->watch_proc) (iceConn,
                                      watchProc->client_data, False, &watchedConn->watch_data);

            if (prev == NULL)
                watchProc->watched_connections = watchedConn->next;
            else
                prev->next = watchedConn->next;

            free (watchedConn);
        }

        watchProc = watchProc->next;
    }
}
