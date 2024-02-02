#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "SMlib.h"
#include "SMlibint.h"
#include <stdio.h>

/*
 * Free property
 */

void
SmFreeProperty(SmProp *prop)
{
    if (prop)
    {
        int i;

        if (prop->name)
            free (prop->name);
        if (prop->type)
            free (prop->type);
        if (prop->vals)
        {
            for (i = 0; i < prop->num_vals; i++)
                if (prop->vals[i].value)
                    free (prop->vals[i].value);
            free (prop->vals);
        }

        free (prop);
    }
}


/*
 * Free reason messages
 */

void
SmFreeReasons(int count, char **reasonMsgs)
{
    if (reasonMsgs)
    {
        int i;

        for (i = 0; i < count; i++)
            free (reasonMsgs[i]);

        free (reasonMsgs);
    }
}



/*
 * Smc informational functions
 */

int
SmcProtocolVersion(SmcConn smcConn)
{
    return (smcConn->proto_major_version);
}


int
SmcProtocolRevision(SmcConn smcConn)
{
    return (smcConn->proto_minor_version);
}


char *
SmcVendor(SmcConn smcConn)
{
    return strdup(smcConn->vendor);
}


char *
SmcRelease(SmcConn smcConn)
{
    return strdup(smcConn->release);
}


char *
SmcClientID(SmcConn smcConn)
{
    return strdup(smcConn->client_id);
}


IceConn
SmcGetIceConnection(SmcConn smcConn)
{
    return (smcConn->iceConn);
}



/*
 * Sms informational functions
 */

int
SmsProtocolVersion(SmsConn smsConn)
{
    return (smsConn->proto_major_version);
}


int
SmsProtocolRevision(SmsConn smsConn)
{
    return (smsConn->proto_minor_version);
}


char *
SmsClientID(SmsConn smsConn)
{
    return strdup(smsConn->client_id);
}


IceConn
SmsGetIceConnection(SmsConn smsConn)
{
    return (smsConn->iceConn);
}
