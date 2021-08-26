#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _GEINT_H_
#define _GEINT_H_

#include "../headers/x.h"
#include "../headers/xproto.h"
#include "../headers/misc.h"
#include "../headers/os.h"
#include "../headers/dixstruct.h"
#include "../headers/extnsionst.h"
#include "../headers/extensions/geproto.h"

extern DevPrivateKeyRec GEClientPrivateKeyRec;

#define GEClientPrivateKey (&GEClientPrivateKeyRec)

typedef struct _GEClientInfo {
    CARD32 major_version;
    CARD32 minor_version;
} GEClientInfoRec, *GEClientInfoPtr;

#define GEGetClient(pClient)    ((GEClientInfoPtr)(dixLookupPrivate(&((pClient)->devPrivates), GEClientPrivateKey)))

#endif                          /* _GEINT_H_ */