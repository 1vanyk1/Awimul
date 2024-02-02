#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "ICElib.h"
#include "ICElibint.h"
#include "ICEutil.h"


/*
 * IceSetPaAuthData is not a standard part of ICElib, it is specific
 * to the sample implementation.
 *
 * For the client that initiates a Protocol Setup, we look in the
 * .ICEauthority file to get authentication data.
 *
 * For the client accepting the Protocol Setup, we get the data
 * from an in-memory database of authentication data (set by the
 * application calling IceSetPaAuthData).  We have to get the data
 * from memory because getting it directly from the .ICEauthority
 * file is not secure - someone can just modify the contents of the
 * .ICEauthority file behind our back.
 */

int		 _IcePaAuthDataEntryCount = 0;
IceAuthDataEntry _IcePaAuthDataEntries[ICE_MAX_AUTH_DATA_ENTRIES];


void
IceSetPaAuthData (
        int			numEntries,
        IceAuthDataEntry	*entries
)
{
    /*
     * _IcePaAuthDataEntries should really be a linked list.
     * On my list of TO DO stuff.
     */

    int i, j;

    for (i = 0; i < numEntries; i++)
    {
        for (j = 0; j < _IcePaAuthDataEntryCount; j++)
            if (strcmp (entries[i].protocol_name,
                        _IcePaAuthDataEntries[j].protocol_name) == 0 &&
                strcmp (entries[i].network_id,
                        _IcePaAuthDataEntries[j].network_id) == 0 &&
                strcmp (entries[i].auth_name,
                        _IcePaAuthDataEntries[j].auth_name) == 0)
                break;

        if (j < _IcePaAuthDataEntryCount)
        {
            free (_IcePaAuthDataEntries[j].protocol_name);
            free (_IcePaAuthDataEntries[j].network_id);
            free (_IcePaAuthDataEntries[j].auth_name);
            free (_IcePaAuthDataEntries[j].auth_data);
        }
        else
        {
            _IcePaAuthDataEntryCount++;
        }

        _IcePaAuthDataEntries[j].protocol_name
                = strdup(entries[i].protocol_name);

        _IcePaAuthDataEntries[j].network_id
                = strdup(entries[i].network_id);

        _IcePaAuthDataEntries[j].auth_name
                = strdup(entries[i].auth_name);

        _IcePaAuthDataEntries[j].auth_data_length =
                entries[i].auth_data_length;
        _IcePaAuthDataEntries[j].auth_data = malloc (
                entries[i].auth_data_length);
        memcpy (_IcePaAuthDataEntries[j].auth_data,
                entries[i].auth_data, entries[i].auth_data_length);
    }
}
