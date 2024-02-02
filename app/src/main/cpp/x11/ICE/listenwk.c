
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "ICElib.h"
#include "ICElibint.h"
#include "../headers/Xtrans/Xtrans.h"
#include <stdio.h>


Status
IceListenForWellKnownConnections (
        char		*port,
        int		*countRet,
        IceListenObj	**listenObjsRet,
        int		errorLength,
        char		*errorStringRet
)
{
    struct _IceListenObj	*listenObjs;
    char			*networkId;
    int				transCount, partial, i, j;
    Status			status = 1;
    XtransConnInfo		*transConns = NULL;


    if ((_IceTransMakeAllCOTSServerListeners (port, &partial,
                                              &transCount, &transConns) < 0) || (transCount < 1))
    {
        *listenObjsRet = NULL;
        *countRet = 0;

        if (errorStringRet && errorLength > 0) {
            strncpy (errorStringRet,
                     "Cannot establish any listening sockets", errorLength);
            errorStringRet[errorLength - 1] = '\0';
        }

        free (transConns);
        return (0);
    }

    if ((listenObjs = malloc (transCount * sizeof (struct _IceListenObj))) == NULL)
    {
        for (i = 0; i < transCount; i++)
            _IceTransClose (transConns[i]);
        free (transConns);
        return (0);
    }

    *countRet = 0;

    for (i = 0; i < transCount; i++)
    {
        networkId = (char *)_IceTransGetMyNetworkId (transConns[i]);

        if (networkId)
        {
            listenObjs[*countRet].trans_conn = transConns[i];
            listenObjs[*countRet].network_id = networkId;

            (*countRet)++;
        }
    }

    if (*countRet == 0)
    {
        *listenObjsRet = NULL;

        if (errorStringRet && errorLength > 0) {
            strncpy (errorStringRet,
                     "Cannot establish any listening sockets", errorLength);
            errorStringRet[errorLength - 1] = '\0';
        }

        status = 0;
    }
    else
    {
        *listenObjsRet = malloc (*countRet * sizeof (IceListenObj));

        if (*listenObjsRet == NULL)
        {
            if (errorStringRet && errorLength > 0) {
                strncpy (errorStringRet, "Malloc failed", errorLength);
                errorStringRet[errorLength - 1] = '\0';
            }

            status = 0;
        }
        else
        {
            for (i = 0; i < *countRet; i++)
            {
                (*listenObjsRet)[i] = malloc (sizeof (struct _IceListenObj));

                if ((*listenObjsRet)[i] == NULL)
                {
                    if (errorStringRet && errorLength > 0) {
                        strncpy (errorStringRet, "Malloc failed", errorLength);
                        errorStringRet[errorLength - 1] = '\0';
                    }

                    for (j = 0; j < i; j++)
                        free ((*listenObjsRet)[j]);

                    free (*listenObjsRet);
                    *listenObjsRet = NULL;

                    status = 0;
                    break;
                }
                else
                {
                    *((*listenObjsRet)[i]) = listenObjs[i];
                }
            }
        }
    }

    if (status == 1)
    {
        if (errorStringRet && errorLength > 0)
            *errorStringRet = '\0';

        for (i = 0; i < *countRet; i++)
        {
            (*listenObjsRet)[i]->host_based_auth_proc = NULL;
        }
    }
    else
    {
        for (i = 0; i < transCount; i++)
            _IceTransClose (transConns[i]);
    }

    free (listenObjs);
    free (transConns);

    return (status);
}
