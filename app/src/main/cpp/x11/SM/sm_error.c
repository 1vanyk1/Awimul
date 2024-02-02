#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "SMlib.h"
#include "SMlibint.h"
#include <stdio.h>



/*
 * Default Smc error handler.
 */

void
_SmcDefaultErrorHandler(SmcConn smcConn, Bool swap, int offendingMinorOpcode,
                        unsigned long offendingSequence, int errorClass,
                        int severity, SmPointer values)
{
    char *pData = (char *) values;
    const char *str;

    switch (offendingMinorOpcode)
    {
        case SM_RegisterClient:
            str = "RegisterClient";
            break;
        case SM_InteractRequest:
            str = "InteractRequest";
            break;
        case SM_InteractDone:
            str = "InteractDone";
            break;
        case SM_SaveYourselfDone:
            str = "SaveYourselfDone";
            break;
        case SM_CloseConnection:
            str = "CloseConnection";
            break;
        case SM_SetProperties:
            str = "SetProperties";
            break;
        case SM_GetProperties:
            str = "GetProperties";
            break;
        default:
            str = "";
    }

    fprintf (stderr, "\n");

    fprintf (stderr, "XSMP error:  Offending minor opcode    = %d (%s)\n",
             offendingMinorOpcode, str);

    fprintf (stderr, "             Offending sequence number = %ld\n",
             offendingSequence);

    switch (errorClass)
    {
        case IceBadMinor:
            str = "BadMinor";
            break;
        case IceBadState:
            str = "BadState";
            break;
        case IceBadLength:
            str = "BadLength";
            break;
        case IceBadValue:
            str = "BadValue";
            break;
        default:
            str = "???";
    }

    fprintf (stderr, "             Error class               = %s\n", str);

    if (severity == IceCanContinue)
        str = "CanContinue";
    else if (severity == IceFatalToProtocol)
        str = "FatalToProtocol";
    else if (severity == IceFatalToConnection)
        str = "FatalToConnection";
    else
        str = "???";

    fprintf (stderr, "             Severity                  = %s\n", str);

    switch (errorClass)
    {
        case IceBadValue:
        {
            unsigned int offset, length, val;

            EXTRACT_CARD32 (pData, swap, offset);
            EXTRACT_CARD32 (pData, swap, length);

            fprintf (stderr,
                     "             BadValue Offset           = %d\n", offset);
            fprintf (stderr,
                     "             BadValue Length           = %d\n", length);

            if (length <= 4)
            {
                if (length == 1)
                    val = (unsigned int) *pData;
                else if (length == 2)
                {
                    EXTRACT_CARD16 (pData, swap, val);
                }
                else
                {
                    EXTRACT_CARD32 (pData, swap, val);
                }

                fprintf (stderr,
                         "             BadValue                  = %d\n", val);
            }
            break;
        }

        default:
            break;
    }

    fprintf (stderr, "\n");

    if (severity != IceCanContinue)
        exit (1);
}



/*
 * Default Sms error handler.
 */

void
_SmsDefaultErrorHandler(SmsConn smsConn, Bool swap, int offendingMinorOpcode,
                        unsigned long offendingSequence, int errorClass,
                        int severity, SmPointer values)
{
    char *pData = (char *) values;
    const char *str;

    switch (offendingMinorOpcode)
    {
        case SM_SaveYourself:
            str = "SaveYourself";
            break;
        case SM_Interact:
            str = "Interact";
            break;
        case SM_Die:
            str = "Die";
            break;
        case SM_ShutdownCancelled:
            str = "ShutdownCancelled";
            break;
        default:
            str = "";
    }

    fprintf (stderr, "\n");

    fprintf (stderr, "XSMP error:  Offending minor opcode    = %d (%s)\n",
             offendingMinorOpcode, str);

    fprintf (stderr, "             Offending sequence number = %ld\n",
             offendingSequence);

    switch (errorClass)
    {
        case IceBadMinor:
            str = "BadMinor";
            break;
        case IceBadState:
            str = "BadState";
            break;
        case IceBadLength:
            str = "BadLength";
            break;
        case IceBadValue:
            str = "BadValue";
            break;
        default:
            str = "???";
    }

    fprintf (stderr, "             Error class               = %s\n", str);

    if (severity == IceCanContinue)
        str = "CanContinue";
    else if (severity == IceFatalToProtocol)
        str = "FatalToProtocol";
    else if (severity == IceFatalToConnection)
        str = "FatalToConnection";
    else
        str = "???";

    fprintf (stderr, "             Severity                  = %s\n", str);

    switch (errorClass)
    {
        case IceBadValue:
        {
            unsigned int offset, length, val;

            EXTRACT_CARD32 (pData, swap, offset);
            EXTRACT_CARD32 (pData, swap, length);

            fprintf (stderr,
                     "             BadValue Offset           = %d\n", offset);
            fprintf (stderr,
                     "             BadValue Length           = %d\n", length);

            if (length <= 4)
            {
                if (length == 1)
                    val = (unsigned int) *pData;
                else if (length == 2)
                {
                    EXTRACT_CARD16 (pData, swap, val);
                }
                else
                {
                    EXTRACT_CARD32 (pData, swap, val);
                }

                fprintf (stderr,
                         "             BadValue                  = %d\n", val);
            }
            break;
        }

        default:
            break;
    }

    fprintf (stderr, "\n\n");

    /* don't exit() - that would kill the SM - pretty devastating */
}



/*
 * This procedure sets the Smc error handler to be the specified
 * routine.  If NULL is passed in the default error handler is restored.
 * The function's return value is the previous error handler.
 */

SmcErrorHandler
SmcSetErrorHandler(SmcErrorHandler handler)
{
    SmcErrorHandler oldHandler = _SmcErrorHandler;

    if (handler != NULL)
        _SmcErrorHandler = handler;
    else
        _SmcErrorHandler = _SmcDefaultErrorHandler;

    return (oldHandler);
}



/*
 * This procedure sets the Sms error handler to be the specified
 * routine.  If NULL is passed in the default error handler is restored.
 * The function's return value is the previous error handler.
 */

SmsErrorHandler
SmsSetErrorHandler(SmsErrorHandler handler)
{
    SmsErrorHandler oldHandler = _SmsErrorHandler;

    if (handler != NULL)
        _SmsErrorHandler = handler;
    else
        _SmsErrorHandler = _SmsDefaultErrorHandler;

    return (oldHandler);
}
