#ifdef HAVE_DIX_CONFIG_H
#include "../headers/dix-config.h"
#endif

#ifndef _DPMSPROC_H_
#define _DPMSPROC_H_

#include "../headers/dixstruct.h"
#include "../headers/xdefs.h"
#include "../headers/xmd.h"

extern int DPMSSet(ClientPtr client, int level);
extern Bool DPMSSupported(void);

extern CARD32 DPMSStandbyTime;
extern CARD32 DPMSSuspendTime;
extern CARD32 DPMSOffTime;
extern CARD16 DPMSPowerLevel;
extern Bool DPMSEnabled;
extern Bool DPMSDisabledSwitch;

#endif
