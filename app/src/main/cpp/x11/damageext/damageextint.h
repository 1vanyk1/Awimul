#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _DAMAGEEXTINT_H_
#define _DAMAGEEXTINT_H_

#include "../headers/x.h"
#include "../headers/xproto.h"
#include "../headers/misc.h"
#include "../headers/os.h"
#include "../headers/dixstruct.h"
#include "../headers/extnsionst.h"
#include "../headers/extensions/damageproto.h"
#include "../headers/windowstr.h"
#include "../headers/selection.h"
#include "../headers/scrnintstr.h"
#include "../miext/damage/damage.h"
#include "../xfixes/xfixes.h"

typedef struct _DamageClient {
    CARD32 major_version;
    CARD32 minor_version;
    int critical;
} DamageClientRec, *DamageClientPtr;

#define GetDamageClient(pClient) ((DamageClientPtr)dixLookupPrivate(&(pClient)->devPrivates, DamageClientPrivateKey))

typedef struct _DamageExt {
    DamagePtr pDamage;
    DrawablePtr pDrawable;
    DamageReportLevel level;
    ClientPtr pClient;
    XID id;
    XID drawable;
} DamageExtRec, *DamageExtPtr;

#define VERIFY_DAMAGEEXT(pDamageExt, rid, client, mode) { \
    int rc = dixLookupResourceByType((void **)&(pDamageExt), rid, \
                                     DamageExtType, client, mode); \
    if (rc != Success) \
        return rc; \
}

void
DamageExtSetCritical(ClientPtr pClient, Bool critical);

void PanoramiXDamageInit(void);
void PanoramiXDamageReset(void);

#endif                          /* _DAMAGEEXTINT_H_ */
