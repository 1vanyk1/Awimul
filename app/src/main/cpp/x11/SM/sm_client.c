#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "SMlib.h"
#include "SMlibint.h"

int 	_SmcOpcode = 0;
int 	_SmsOpcode = 0;

SmsNewClientProc _SmsNewClientProc;
SmPointer        _SmsNewClientData;

SmcErrorHandler _SmcErrorHandler = _SmcDefaultErrorHandler;
SmsErrorHandler _SmsErrorHandler = _SmsDefaultErrorHandler;


static void
set_callbacks(SmcConn smcConn, unsigned long mask, SmcCallbacks *callbacks);


SmcConn
SmcOpenConnection(char *networkIdsList, SmPointer context,
                  int xsmpMajorRev, int xsmpMinorRev,
                  unsigned long mask, SmcCallbacks *callbacks,
                  const char *previousId, char **clientIdRet,
                  int errorLength, char *errorStringRet)
{
    SmcConn			smcConn;
    IceConn			iceConn;
    char 			*ids;
    IceProtocolSetupStatus	setupstat;
    int				majorVersion;
    int				minorVersion;
    char			*vendor = NULL;
    char			*release = NULL;
    smRegisterClientMsg 	*pMsg;
    char 			*pData;
    unsigned int		extra, len;
    IceReplyWaitInfo		replyWait;
    _SmcRegisterClientReply	reply;
    Bool			gotReply, ioErrorOccured;

    const char *auth_names[] = {"MIT-MAGIC-COOKIE-1"};
    IcePoAuthProc auth_procs[] = {_IcePoMagicCookie1Proc};
    int auth_count = 1;

    IcePoVersionRec versions[] = {
            {SmProtoMajor, SmProtoMinor, _SmcProcessMessage}
    };
    int version_count = 1;


    *clientIdRet = NULL;

    if (errorStringRet && errorLength > 0)
        *errorStringRet = '\0';

    if (!_SmcOpcode)
    {
        /*
         * For now, there is only one version of XSMP, so we don't
         * have to check {xsmpMajorRev, xsmpMinorRev}.  In the future,
         * we will check against versions and generate the list
         * of versions the application actually supports.
         */

        if ((_SmcOpcode = IceRegisterForProtocolSetup ("XSMP",
                                                       SmVendorString, SmReleaseString, version_count, versions,
                                                       auth_count, auth_names, auth_procs, NULL)) < 0)
        {
            if (errorStringRet && errorLength > 0) {
                strncpy (errorStringRet,
                         "Could not register XSMP protocol with ICE",
                         errorLength);
                errorStringRet[errorLength - 1] = '\0';
            }

            return (NULL);
        }
    }

    if (networkIdsList == NULL || *networkIdsList == '\0')
    {
        if ((ids = (char *) getenv ("SESSION_MANAGER")) == NULL)
        {
            if (errorStringRet && errorLength > 0) {
                strncpy (errorStringRet,
                         "SESSION_MANAGER environment variable not defined",
                         errorLength);
                errorStringRet[errorLength - 1] = '\0';
            }
            return (NULL);
        }
    }
    else
    {
        ids = networkIdsList;
    }

    if ((iceConn = IceOpenConnection (
            ids, context, 0, _SmcOpcode, errorLength, errorStringRet)) == NULL)
    {
        return (NULL);
    }

    if ((smcConn = malloc (sizeof (struct _SmcConn))) == NULL)
    {
        if (errorStringRet && errorLength > 0) {
            strncpy (errorStringRet, "Can't malloc", errorLength);
            errorStringRet[errorLength - 1] = '\0';
        }
        IceCloseConnection (iceConn);
        return (NULL);
    }

    setupstat = IceProtocolSetup (iceConn, _SmcOpcode,
                                  (IcePointer) smcConn,
                                  False /* mustAuthenticate */,
                                  &majorVersion, &minorVersion,
                                  &vendor, &release, errorLength, errorStringRet);

    if (setupstat == IceProtocolSetupFailure ||
        setupstat == IceProtocolSetupIOError)
    {
        IceCloseConnection (iceConn);
        free (smcConn);
        return (NULL);
    }
    else if (setupstat == IceProtocolAlreadyActive)
    {
        /*
         * This case should never happen, because when we called
         * IceOpenConnection, we required that the ICE connection
         * may not already have XSMP active on it.
         */

        free (smcConn);
        if (errorStringRet && errorLength > 0) {
            strncpy (errorStringRet, "Internal error in IceOpenConnection",
                     errorLength);
            errorStringRet[errorLength - 1] = '\0';
        }
        return (NULL);
    }

    smcConn->iceConn = iceConn;
    smcConn->proto_major_version = majorVersion;
    smcConn->proto_minor_version = minorVersion;
    smcConn->vendor = vendor;
    smcConn->release = release;
    smcConn->client_id = NULL;

    bzero ((char *) &smcConn->callbacks, sizeof (SmcCallbacks));
    set_callbacks (smcConn, mask, callbacks);

    smcConn->interact_waits = NULL;
    smcConn->phase2_wait = NULL;
    smcConn->prop_reply_waits = NULL;

    smcConn->save_yourself_in_progress = False;
    smcConn->shutdown_in_progress = False;


    /*
     * Now register the client
     */

    if (!previousId)
        previousId = "";
    len = strlen (previousId);
    extra = ARRAY8_BYTES (len);

    IceGetHeaderExtra (iceConn, _SmcOpcode, SM_RegisterClient,
                       SIZEOF (smRegisterClientMsg), WORD64COUNT (extra),
                       smRegisterClientMsg, pMsg, pData);

    STORE_ARRAY8 (pData, len, previousId);

    IceFlush (iceConn);

    replyWait.sequence_of_request = IceLastSentSequenceNumber (iceConn);
    replyWait.major_opcode_of_request = _SmcOpcode;
    replyWait.minor_opcode_of_request = SM_RegisterClient;
    replyWait.reply = (IcePointer) &reply;

    gotReply = False;
    ioErrorOccured = False;

    while (!gotReply && !ioErrorOccured)
    {
        ioErrorOccured = (IceProcessMessages (
                iceConn, &replyWait, &gotReply) == IceProcessMessagesIOError);

        if (ioErrorOccured)
        {
            if (errorStringRet && errorLength > 0) {
                strncpy (errorStringRet, "IO error occured opening connection",
                         errorLength);
                errorStringRet[errorLength - 1] = '\0';
            }
            free (smcConn->vendor);
            free (smcConn->release);
            free (smcConn);

            return (NULL);
        }
        else if (gotReply)
        {
            if (reply.status == 1)
            {
                /*
                 * The client successfully registered.
                 */

                *clientIdRet = reply.client_id;

                smcConn->client_id = strdup (*clientIdRet);
            }
            else
            {
                /*
                 * Could not register the client because the previous ID
                 * was bad.  So now we register the client with the
                 * previous ID set to NULL.
                 */

                extra = ARRAY8_BYTES (0);

                IceGetHeaderExtra (iceConn, _SmcOpcode, SM_RegisterClient,
                                   SIZEOF (smRegisterClientMsg), WORD64COUNT (extra),
                                   smRegisterClientMsg, pMsg, pData);

                STORE_ARRAY8 (pData, 0, "");

                IceFlush (iceConn);

                replyWait.sequence_of_request =
                        IceLastSentSequenceNumber (iceConn);

                gotReply = False;
            }
        }
    }

    return (smcConn);
}



SmcCloseStatus
SmcCloseConnection(SmcConn smcConn, int count, char **reasonMsgs)
{
    IceConn			iceConn = smcConn->iceConn;
    smCloseConnectionMsg 	*pMsg;
    char 			*pData;
    int				extra, i;
    IceCloseStatus	        closeStatus;
    SmcCloseStatus		statusRet;

    extra = 8;

    for (i = 0; i < count; i++)
        extra += ARRAY8_BYTES (strlen (reasonMsgs[i]));

    IceGetHeaderExtra (iceConn, _SmcOpcode, SM_CloseConnection,
                       SIZEOF (smCloseConnectionMsg), WORD64COUNT (extra),
                       smCloseConnectionMsg, pMsg, pData);

    STORE_CARD32 (pData, (CARD32) count);
    pData += 4;

    for (i = 0; i < count; i++)
    STORE_ARRAY8 (pData, strlen (reasonMsgs[i]), reasonMsgs[i]);

    IceFlush (iceConn);

    IceProtocolShutdown (iceConn, _SmcOpcode);
    IceSetShutdownNegotiation (iceConn, False);
    closeStatus = IceCloseConnection (iceConn);

    if (smcConn->vendor)
        free (smcConn->vendor);

    if (smcConn->release)
        free (smcConn->release);

    if (smcConn->client_id)
        free (smcConn->client_id);

    if (smcConn->prop_reply_waits)
    {
        _SmcPropReplyWait *ptr = smcConn->prop_reply_waits;
        _SmcPropReplyWait *next;

        while (ptr)
        {
            next = ptr->next;
            free (ptr);
            ptr = next;
        }

    }

    free (smcConn);

    if (closeStatus == IceClosedNow)
        statusRet = SmcClosedNow;
    else if (closeStatus == IceClosedASAP)
        statusRet = SmcClosedASAP;
    else
        statusRet = SmcConnectionInUse;

    return (statusRet);
}



void
SmcModifyCallbacks(SmcConn smcConn, unsigned long mask, SmcCallbacks *callbacks)
{
    set_callbacks (smcConn, mask, callbacks);
}



void
SmcSetProperties(SmcConn smcConn, int numProps, SmProp **props)
{
    IceConn		iceConn = smcConn->iceConn;
    smSetPropertiesMsg	*pMsg;
    char		*pBuf;
    char		*pStart;
    unsigned int	bytes;

    IceGetHeader (iceConn, _SmcOpcode, SM_SetProperties,
                  SIZEOF (smSetPropertiesMsg), smSetPropertiesMsg, pMsg);

    LISTOF_PROP_BYTES (numProps, props, bytes);
    pMsg->length += WORD64COUNT (bytes);

    pBuf = pStart = IceAllocScratch (iceConn, bytes);
    memset(pStart, 0, bytes);

    STORE_LISTOF_PROPERTY (pBuf, numProps, props);

    IceWriteData (iceConn, bytes, pStart);
    IceFlush (iceConn);
}



void
SmcDeleteProperties(SmcConn smcConn, int numProps, char **propNames)
{
    IceConn			iceConn = smcConn->iceConn;
    smDeletePropertiesMsg 	*pMsg;
    char 			*pData;
    int				extra, i;

    extra = 8;

    for (i = 0; i < numProps; i++)
        extra += ARRAY8_BYTES (strlen (propNames[i]));

    IceGetHeaderExtra (iceConn, _SmcOpcode, SM_DeleteProperties,
                       SIZEOF (smDeletePropertiesMsg), WORD64COUNT (extra),
                       smDeletePropertiesMsg, pMsg, pData);

    STORE_CARD32 (pData, numProps);
    pData += 4;

    for (i = 0; i < numProps; i++)
    STORE_ARRAY8 (pData, strlen (propNames[i]), propNames[i]);

    IceFlush (iceConn);
}



Status
SmcGetProperties(SmcConn smcConn, SmcPropReplyProc propReplyProc,
                 SmPointer clientData)
{
    IceConn		iceConn = smcConn->iceConn;
    _SmcPropReplyWait 	*wait, *ptr;

    if ((wait = malloc (sizeof (_SmcPropReplyWait))) == NULL)
    {
        return (0);
    }

    wait->prop_reply_proc = propReplyProc;
    wait->client_data = clientData;
    wait->next = NULL;

    ptr = smcConn->prop_reply_waits;
    while (ptr && ptr->next)
        ptr = ptr->next;

    if (ptr == NULL)
        smcConn->prop_reply_waits = wait;
    else
        ptr->next = wait;

    IceSimpleMessage (iceConn, _SmcOpcode, SM_GetProperties);
    IceFlush (iceConn);

    return (1);
}



Status
SmcInteractRequest(SmcConn smcConn, int dialogType,
                   SmcInteractProc interactProc, SmPointer clientData)
{
    IceConn			iceConn = smcConn->iceConn;
    smInteractRequestMsg	*pMsg;
    _SmcInteractWait 		*wait, *ptr;

    if ((wait = malloc (sizeof (_SmcInteractWait))) == NULL)
    {
        return (0);
    }

    wait->interact_proc = interactProc;
    wait->client_data = clientData;
    wait->next = NULL;

    ptr = smcConn->interact_waits;
    while (ptr && ptr->next)
        ptr = ptr->next;

    if (ptr == NULL)
        smcConn->interact_waits = wait;
    else
        ptr->next = wait;

    IceGetHeader (iceConn, _SmcOpcode, SM_InteractRequest,
                  SIZEOF (smInteractRequestMsg), smInteractRequestMsg, pMsg);

    pMsg->dialogType = dialogType;

    IceFlush (iceConn);

    return (1);
}



void
SmcInteractDone(SmcConn smcConn, Bool cancelShutdown)
{
    IceConn		iceConn = smcConn->iceConn;
    smInteractDoneMsg	*pMsg;

    IceGetHeader (iceConn, _SmcOpcode, SM_InteractDone,
                  SIZEOF (smInteractDoneMsg), smInteractDoneMsg, pMsg);

    pMsg->cancelShutdown = cancelShutdown;

    IceFlush (iceConn);
}



void
SmcRequestSaveYourself(SmcConn smcConn, int saveType, Bool shutdown,
                       int interactStyle, Bool fast, Bool global)
{
    IceConn			iceConn = smcConn->iceConn;
    smSaveYourselfRequestMsg	*pMsg;

    IceGetHeader (iceConn, _SmcOpcode, SM_SaveYourselfRequest,
                  SIZEOF (smSaveYourselfRequestMsg), smSaveYourselfRequestMsg, pMsg);

    pMsg->saveType = saveType;
    pMsg->shutdown = shutdown;
    pMsg->interactStyle = interactStyle;
    pMsg->fast = fast;
    pMsg->global = global;

    IceFlush (iceConn);
}



Status
SmcRequestSaveYourselfPhase2(SmcConn smcConn,
                             SmcSaveYourselfPhase2Proc saveYourselfPhase2Proc,
                             SmPointer clientData)
{
    IceConn		iceConn = smcConn->iceConn;
    _SmcPhase2Wait 	*wait;

    if (smcConn->phase2_wait)
        wait = smcConn->phase2_wait;
    else
    {
        if ((wait = malloc (sizeof (_SmcPhase2Wait))) == NULL)
        {
            return (0);
        }
    }

    wait->phase2_proc = saveYourselfPhase2Proc;
    wait->client_data = clientData;

    smcConn->phase2_wait = wait;

    IceSimpleMessage (iceConn, _SmcOpcode, SM_SaveYourselfPhase2Request);
    IceFlush (iceConn);

    return (1);
}



void
SmcSaveYourselfDone(SmcConn smcConn, Bool success)
{
    IceConn			iceConn = smcConn->iceConn;
    smSaveYourselfDoneMsg	*pMsg;

    IceGetHeader (iceConn, _SmcOpcode, SM_SaveYourselfDone,
                  SIZEOF (smSaveYourselfDoneMsg), smSaveYourselfDoneMsg, pMsg);

    pMsg->success = success;

    IceFlush (iceConn);
}



static void
set_callbacks(SmcConn smcConn, unsigned long mask, SmcCallbacks *callbacks)
{
    if (mask & SmcSaveYourselfProcMask)
    {
        smcConn->callbacks.save_yourself.callback =
                callbacks->save_yourself.callback;
        smcConn->callbacks.save_yourself.client_data =
                callbacks->save_yourself.client_data;
    }

    if (mask & SmcDieProcMask)
    {
        smcConn->callbacks.die.callback = callbacks->die.callback;
        smcConn->callbacks.die.client_data = callbacks->die.client_data;
    }

    if (mask & SmcSaveCompleteProcMask)
    {
        smcConn->callbacks.save_complete.callback =
                callbacks->save_complete.callback;
        smcConn->callbacks.save_complete.client_data =
                callbacks->save_complete.client_data;
    }

    if (mask & SmcShutdownCancelledProcMask)
    {
        smcConn->callbacks.shutdown_cancelled.callback =
                callbacks->shutdown_cancelled.callback;
        smcConn->callbacks.shutdown_cancelled.client_data =
                callbacks->shutdown_cancelled.client_data;
    }
}
