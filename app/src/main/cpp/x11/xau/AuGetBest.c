#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../headers/xauth.h"
#include "../headers/xos.h"
#ifdef XTHREADS
#include "../headers/xthreads.h"
#endif
#ifdef hpux
#define X_INCLUDE_NETDB_H
#define XOS_USE_NO_LOCKING
#include "../headers/xos_r.h"
#endif

#define binaryEqual(a, b, len) (memcmp(a, b, len) == 0)

Xauth *
XauGetBestAuthByAddr (
#if NeedWidePrototypes
        unsigned int	family,
    unsigned int	address_length,
#else
        unsigned short	family,
        unsigned short	address_length,
#endif
        _Xconst char*	address,
#if NeedWidePrototypes
        unsigned int	number_length,
#else
        unsigned short	number_length,
#endif
        _Xconst char*	number,
        int			types_length,
        char**		types,
        _Xconst int*	type_lengths)
{
    FILE    *auth_file;
    char    *auth_name;
    Xauth   *entry;
    Xauth   *best;
    int	    best_type;
    int	    type;
#ifdef hpux
    char		*fully_qual_address;
    unsigned short	fully_qual_address_length;
#endif

    auth_name = XauFileName ();
    if (!auth_name)
        return NULL;
    if (access (auth_name, R_OK) != 0)		/* checks REAL id */
        return NULL;
    auth_file = fopen (auth_name, "rb");
    if (!auth_file)
        return NULL;

#ifdef hpux
    if (family == FamilyLocal) {
#ifdef XTHREADS_NEEDS_BYNAMEPARAMS
	_Xgethostbynameparams hparams;
#endif
	struct hostent *hostp;

	/* make sure we try fully-qualified hostname */
	if ((hostp = _XGethostbyname(address,hparams)) != NULL) {
	    fully_qual_address = hostp->h_name;
	    fully_qual_address_length = strlen(fully_qual_address);
	}
	else
	{
	    fully_qual_address = NULL;
	    fully_qual_address_length = 0;
	}
    }
#endif /* hpux */

    best = NULL;
    best_type = types_length;
    for (;;) {
        entry = XauReadAuth (auth_file);
        if (!entry)
            break;
        /*
         * Match when:
         *   either family or entry->family are FamilyWild or
         *    family and entry->family are the same and
         *     address and entry->address are the same
         *  and
         *   either number or entry->number are empty or
         *    number and entry->number are the same
         *  and
         *   either name or entry->name are empty or
         *    name and entry->name are the same
         */

        if ((family == FamilyWild || entry->family == FamilyWild ||
             (entry->family == family &&
              ((address_length == entry->address_length &&
                binaryEqual (entry->address, address, address_length))
#ifdef hpux
                     || (family == FamilyLocal &&
		fully_qual_address_length == entry->address_length &&
	     	binaryEqual (entry->address, fully_qual_address,
		    fully_qual_address_length))
#endif
             ))) &&
            (number_length == 0 || entry->number_length == 0 ||
             (number_length == entry->number_length &&
              binaryEqual (entry->number, number, number_length))))
        {
            if (best_type == 0)
            {
                best = entry;
                break;
            }
            for (type = 0; type < best_type; type++)
                if (type_lengths[type] == entry->name_length &&
                    !(strncmp (types[type], entry->name, entry->name_length)))
                {
                    break;
                }
            if (type < best_type)
            {
                if (best)
                    XauDisposeAuth (best);
                best = entry;
                best_type = type;
                if (type == 0)
                    break;
                continue;
            }
        }
        XauDisposeAuth (entry);
    }
    (void) fclose (auth_file);
    return best;
}
