#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _XFIXES_H_
#define _XFIXES_H_

#include "../headers/resource.h"

extern RESTYPE RegionResType;
extern int XFixesErrorBase;

#define VERIFY_REGION(pRegion, rid, client, mode)			\
    do {								\
	int err;							\
	err = dixLookupResourceByType((void **) &pRegion, rid,	\
				      RegionResType, client, mode);	\
	if (err != Success) {						\
	    client->errorValue = rid;					\
	    return err;							\
	}								\
    } while (0)

#define VERIFY_REGION_OR_NONE(pRegion, rid, client, mode) { \
    pRegion = 0; \
    if (rid) VERIFY_REGION(pRegion, rid, client, mode); \
}

extern RegionPtr
XFixesRegionCopy(RegionPtr pRegion);

#include "../xi/xibarriers.h"

#endif                          /* _XFIXES_H_ */
