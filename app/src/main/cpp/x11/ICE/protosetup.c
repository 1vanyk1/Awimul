#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "ICElib.h"
#include "ICElibint.h"


IceProtocolSetupStatus
IceProtocolSetup (
        IceConn	   iceConn,
        int 	   myOpcode,
        IcePointer clientData,
        Bool       mustAuthenticate,
        int	   *majorVersionRet,
        int	   *minorVersionRet,
        char	   **vendorRet,
        char	   **releaseRet,
        int  	   errorLength,
        char 	   *errorStringRet
)
{
    iceProtocolSetupMsg	*pMsg;
    char		*pData;
    _IceProtocol	*myProtocol;
    int			extra;
    Bool		gotReply, ioErrorOccured;
    int			accepted, i;
    int			hisOpcode;
    unsigned long	setup_sequence;
    IceReplyWaitInfo 	replyWait;
    _IceReply		reply;
    IcePoVersionRec	*versionRec = NULL;
    int			authCount;
    int			*authIndices;

    if (errorStringRet && errorLength > 0)
        *errorStringRet = '\0';

    *majorVersionRet = 0;
    *minorVersionRet = 0;
    *vendorRet = NULL;
    *releaseRet = NULL;

    if (myOpcode < 1 || myOpcode > _IceLastMajorOpcode)
    {
        if (errorStringRet && errorLength > 0) {
            strncpy (errorStringRet, "myOpcode out of range", errorLength);
            errorStringRet[errorLength - 1] = '\0';
        }
        return (IceProtocolSetupFailure);
    }

    myProtocol = &_IceProtocols[myOpcode - 1];

    if (myProtocol->orig_client == NULL)
    {
        if (errorStringRet && errorLength > 0) {
            strncpy (errorStringRet,
                     "IceRegisterForProtocolSetup was not called", errorLength);
            errorStringRet[errorLength - 1] = '\0';
        }
        return (IceProtocolSetupFailure);
    }


    /*
     * Make sure this protocol hasn't been activated already.
     */

    if (iceConn->process_msg_info)
    {
        for (i = iceConn->his_min_opcode; i <= iceConn->his_max_opcode; i++)
        {
            if (iceConn->process_msg_info[
                        i - iceConn->his_min_opcode].in_use &&
                iceConn->process_msg_info[
                        i - iceConn->his_min_opcode ].my_opcode == myOpcode)
                break;
        }

        if (i <= iceConn->his_max_opcode)
        {
            return (IceProtocolAlreadyActive);
        }
    }

    /*
     * Generate the message.
     */

    if (myProtocol->orig_client->auth_count > 0)
    {
        authIndices = malloc (
                myProtocol->orig_client->auth_count * sizeof (int));

        _IceGetPoValidAuthIndices (myProtocol->protocol_name,
                                   iceConn->connection_string,
                                   myProtocol->orig_client->auth_count,
                                   (const char **) myProtocol->orig_client->auth_names,
                                   &authCount, authIndices);

    }
    else
    {
        authCount = 0;
        authIndices = NULL;
    }

    extra = STRING_BYTES (myProtocol->protocol_name) +
            STRING_BYTES (myProtocol->orig_client->vendor) +
            STRING_BYTES (myProtocol->orig_client->release);

    for (i = 0; i < authCount; i++)
    {
        extra += STRING_BYTES (myProtocol->orig_client->auth_names[
                                       authIndices[i]]);
    }

    extra += (myProtocol->orig_client->version_count * 4);

    IceGetHeaderExtra (iceConn, 0, ICE_ProtocolSetup,
                       SIZEOF (iceProtocolSetupMsg), WORD64COUNT (extra),
                       iceProtocolSetupMsg, pMsg, pData);

    setup_sequence = iceConn->send_sequence;

    pMsg->protocolOpcode = myOpcode;
    pMsg->versionCount = myProtocol->orig_client->version_count;
    pMsg->authCount = authCount;
    pMsg->mustAuthenticate = mustAuthenticate;

    STORE_STRING (pData, myProtocol->protocol_name);
    STORE_STRING (pData, myProtocol->orig_client->vendor);
    STORE_STRING (pData, myProtocol->orig_client->release);

    for (i = 0; i < authCount; i++)
    {
        STORE_STRING (pData, myProtocol->orig_client->auth_names[
                authIndices[i]]);
    }

    for (i = 0; i < myProtocol->orig_client->version_count; i++)
    {
        STORE_CARD16 (pData,
                      myProtocol->orig_client->version_recs[i].major_version);
        STORE_CARD16 (pData,
                      myProtocol->orig_client->version_recs[i].minor_version);
    }

    IceFlush (iceConn);


    /*
     * Process messages until we get a Protocol Reply.
     */

    replyWait.sequence_of_request = setup_sequence;
    replyWait.major_opcode_of_request = 0;
    replyWait.minor_opcode_of_request = ICE_ProtocolSetup;
    replyWait.reply = (IcePointer) &reply;

    iceConn->protosetup_to_you = malloc (sizeof (_IceProtoSetupToYouInfo));
    iceConn->protosetup_to_you->my_opcode = myOpcode;
    iceConn->protosetup_to_you->my_auth_count = authCount;
    iceConn->protosetup_to_you->auth_active = 0;
    iceConn->protosetup_to_you->my_auth_indices = authIndices;

    gotReply = False;
    ioErrorOccured = False;
    accepted = 0;

    while (!gotReply && !ioErrorOccured)
    {
        ioErrorOccured = (IceProcessMessages (
                iceConn, &replyWait, &gotReply) == IceProcessMessagesIOError);

        if (ioErrorOccured)
        {
            if (errorStringRet && errorLength > 0) {
                strncpy (errorStringRet,
                         "IO error occured doing Protocol Setup on connection",
                         errorLength);
                errorStringRet[errorLength - 1] = '\0';
            }
            return (IceProtocolSetupIOError);
        }
        else if (gotReply)
        {
            if (reply.type == ICE_PROTOCOL_REPLY)
            {
                if (reply.protocol_reply.version_index >=
                    myProtocol->orig_client->version_count)
                {
                    if (errorStringRet && errorLength > 0) {
                        strncpy (errorStringRet,
                                 "Got a bad version index in the Protocol Reply",
                                 errorLength);
                        errorStringRet[errorLength - 1] = '\0';
                    }

                    free (reply.protocol_reply.vendor);
                    free (reply.protocol_reply.release);
                }
                else
                {
                    versionRec = &(myProtocol->orig_client->version_recs[
                            reply.protocol_reply.version_index]);

                    accepted = 1;
                }
            }
            else /* reply.type == ICE_PROTOCOL_ERROR */
            {
                /* Protocol Setup failed */

                if (errorStringRet && errorLength > 0) {
                    strncpy (errorStringRet, reply.protocol_error.error_message,
                             errorLength);
                    errorStringRet[errorLength - 1] = '\0';
                }

                free (reply.protocol_error.error_message);
            }

            if (iceConn->protosetup_to_you->my_auth_indices)
                free (iceConn->protosetup_to_you->my_auth_indices);
            free (iceConn->protosetup_to_you);
            iceConn->protosetup_to_you = NULL;
        }
    }

    if (accepted)
    {
        _IceProcessMsgInfo *process_msg_info;

        *majorVersionRet = versionRec->major_version;
        *minorVersionRet = versionRec->minor_version;
        *vendorRet = reply.protocol_reply.vendor;
        *releaseRet = reply.protocol_reply.release;


        /*
         * Increase the reference count for the number of active protocols.
         */

        iceConn->proto_ref_count++;


        /*
         * We may be using a different major opcode for this protocol
         * than the other client.  Whenever we get a message, we must
         * map to our own major opcode.
         */

        hisOpcode = reply.protocol_reply.major_opcode;

        _IceAddOpcodeMapping (iceConn, hisOpcode, myOpcode);

        process_msg_info = &iceConn->process_msg_info[hisOpcode -
                                                      iceConn->his_min_opcode];

        process_msg_info->client_data = clientData;
        process_msg_info->accept_flag = 0;

        process_msg_info->process_msg_proc.orig_client =
                versionRec->process_msg_proc;

        return (IceProtocolSetupSuccess);
    }
    else
    {
        return (IceProtocolSetupFailure);
    }
}
