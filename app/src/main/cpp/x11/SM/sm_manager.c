#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "SMlib.h"
#include "SMlibint.h"
#include "../headers/Xtrans/Xtrans.h"

#ifdef __UNIXWARE__
#undef shutdown
#endif



static Status
_SmsProtocolSetupProc (IceConn    iceConn,
                       int majorVersion,
                       int minorVersion,
                       char *vendor,
                       char *release,
                       IcePointer *clientDataRet,
                       char **failureReasonRet)
{
    SmsConn  		smsConn;
    unsigned long 	mask;
    Status		status;

    /*
     * vendor/release are undefined for ProtocolSetup in XSMP.
     */

    if (vendor)
        free (vendor);
    if (release)
        free (release);


    /*
     * Allocate new SmsConn.
     */

    if ((smsConn = malloc (sizeof (struct _SmsConn))) == NULL)
    {
        const char *str = "Memory allocation failed";

        *failureReasonRet = strdup (str);

        return (0);
    }

    smsConn->iceConn = iceConn;
    smsConn->proto_major_version = majorVersion;
    smsConn->proto_minor_version = minorVersion;
    smsConn->client_id = NULL;

    smsConn->save_yourself_in_progress = False;
    smsConn->interaction_allowed = SmInteractStyleNone;
    smsConn->can_cancel_shutdown = False;
    smsConn->interact_in_progress = False;

    *clientDataRet = (IcePointer) smsConn;


    /*
     * Now give the session manager the new smsConn and get back the
     * callbacks to invoke when messages arrive from the client.
     *
     * In the future, we can use the mask return value to check
     * if the SM is expecting an older rev of SMlib.
     */

    bzero ((char *) &smsConn->callbacks, sizeof (SmsCallbacks));

    status = (*_SmsNewClientProc) (smsConn, _SmsNewClientData,
                                   &mask, &smsConn->callbacks, failureReasonRet);

    return (status);
}




Status
SmsInitialize(const char *vendor, const char *release,
              SmsNewClientProc newClientProc,
              SmPointer managerData, IceHostBasedAuthProc hostBasedAuthProc,
              int errorLength, char *errorStringRet)
{
    const char *auth_names[] = {"MIT-MAGIC-COOKIE-1"};
    IcePaAuthProc auth_procs[] = {_IcePaMagicCookie1Proc};
    int auth_count = 1;

    IcePaVersionRec versions[] = {
            {SmProtoMajor, SmProtoMinor, _SmsProcessMessage}
    };
    int version_count = 1;

    if (errorStringRet && errorLength > 0)
        *errorStringRet = '\0';

    if (!newClientProc)
    {
        if (errorStringRet && errorLength > 0) {
            strncpy (errorStringRet,
                     "The SmsNewClientProc callback can't be NULL",
                     errorLength);
            errorStringRet[errorLength - 1] = '\0';
        }

        return (0);
    }

    if (!_SmsOpcode)
    {

        if ((_SmsOpcode = IceRegisterForProtocolReply ("XSMP",
                                                       vendor, release, version_count, versions,
                                                       auth_count, auth_names, auth_procs, hostBasedAuthProc,
                                                       _SmsProtocolSetupProc,
                                                       NULL,	/* IceProtocolActivateProc - we don't care about
			   when the Protocol Reply is sent, because the
			   session manager can not immediately send a
			   message - it must wait for RegisterClient. */
                                                       NULL	/* IceIOErrorProc */
        )) < 0)
        {
            if (errorStringRet && errorLength > 0) {
                strncpy (errorStringRet,
                         "Could not register XSMP protocol with ICE",
                         errorLength);
                errorStringRet[errorLength - 1] = '\0';
            }
            return (0);
        }
    }

    _SmsNewClientProc = newClientProc;
    _SmsNewClientData = managerData;

    return (1);
}



char *
SmsClientHostName(SmsConn smsConn)
{
    return (IceGetPeerName (smsConn->iceConn));
}



Status
SmsRegisterClientReply(SmsConn smsConn, char *clientId)
{
    IceConn			iceConn = smsConn->iceConn;
    size_t			extra;
    smRegisterClientReplyMsg 	*pMsg;
    char 			*pData;

    if ((smsConn->client_id = strdup (clientId)) == NULL)
    {
        return (0);
    }

    extra = ARRAY8_BYTES (strlen (clientId));

    IceGetHeaderExtra (iceConn, _SmsOpcode, SM_RegisterClientReply,
                       SIZEOF (smRegisterClientReplyMsg), WORD64COUNT (extra),
                       smRegisterClientReplyMsg, pMsg, pData);

    STORE_ARRAY8 (pData, strlen (clientId), clientId);

    IceFlush (iceConn);

    return (1);
}



void
SmsSaveYourself(SmsConn smsConn, int saveType, Bool shutdown,
                int interactStyle, Bool fast)
{
    IceConn		iceConn = smsConn->iceConn;
    smSaveYourselfMsg	*pMsg;

    IceGetHeader (iceConn, _SmsOpcode, SM_SaveYourself,
                  SIZEOF (smSaveYourselfMsg), smSaveYourselfMsg, pMsg);

    pMsg->saveType = saveType;
    pMsg->shutdown = shutdown;
    pMsg->interactStyle = interactStyle;
    pMsg->fast = fast;

    IceFlush (iceConn);

    smsConn->save_yourself_in_progress = True;

    if (interactStyle == SmInteractStyleNone ||
        interactStyle == SmInteractStyleErrors ||
        interactStyle == SmInteractStyleAny)
    {
        smsConn->interaction_allowed = interactStyle;
    }
    else
    {
        smsConn->interaction_allowed = SmInteractStyleNone;
    }

    smsConn->can_cancel_shutdown = shutdown &&
                                   (interactStyle == SmInteractStyleAny ||
                                    interactStyle == SmInteractStyleErrors);
}



void
SmsSaveYourselfPhase2(SmsConn smsConn)
{
    IceConn	iceConn = smsConn->iceConn;

    IceSimpleMessage (iceConn, _SmsOpcode, SM_SaveYourselfPhase2);
    IceFlush (iceConn);
}



void
SmsInteract(SmsConn smsConn)
{
    IceConn	iceConn = smsConn->iceConn;

    IceSimpleMessage (iceConn, _SmsOpcode, SM_Interact);
    IceFlush (iceConn);

    smsConn->interact_in_progress = True;
}



void
SmsDie(SmsConn smsConn)
{
    IceConn	iceConn = smsConn->iceConn;

    IceSimpleMessage (iceConn, _SmsOpcode, SM_Die);
    IceFlush (iceConn);
}



void
SmsSaveComplete(SmsConn smsConn)
{
    IceConn	iceConn = smsConn->iceConn;

    IceSimpleMessage (iceConn, _SmsOpcode, SM_SaveComplete);
    IceFlush (iceConn);
}



void
SmsShutdownCancelled(SmsConn smsConn)
{
    IceConn	iceConn = smsConn->iceConn;

    IceSimpleMessage (iceConn, _SmsOpcode, SM_ShutdownCancelled);
    IceFlush (iceConn);

    smsConn->can_cancel_shutdown = False;
}



void
SmsReturnProperties(SmsConn smsConn, int numProps, SmProp **props)
{
    IceConn			iceConn = smsConn->iceConn;
    unsigned int		bytes;
    smPropertiesReplyMsg	*pMsg;
    char 			*pBuf;
    char			*pStart;

    IceGetHeader (iceConn, _SmsOpcode, SM_PropertiesReply,
                  SIZEOF (smPropertiesReplyMsg), smPropertiesReplyMsg, pMsg);

    LISTOF_PROP_BYTES (numProps, props, bytes);
    pMsg->length += WORD64COUNT (bytes);

    pBuf = pStart = IceAllocScratch (iceConn, bytes);

    STORE_LISTOF_PROPERTY (pBuf, numProps, props);

    IceWriteData (iceConn, bytes, pStart);
    IceFlush (iceConn);
}



void
SmsCleanUp(SmsConn smsConn)
{
    IceProtocolShutdown (smsConn->iceConn, _SmsOpcode);

    if (smsConn->client_id)
        free (smsConn->client_id);

    free (smsConn);
}
