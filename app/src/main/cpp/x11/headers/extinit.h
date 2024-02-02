#ifndef EXTINIT_H
#define EXTINIT_H

#include "extnsionst.h"
#include "xlocale.h"
#include "xdefs.h"

extern _X_EXPORT Bool noCompositeExtension;
extern void CompositeExtensionInit(void);

#define XTEST 1
#define COMPOSITE 1
#define DAMAGE 1
#define MITSHM 1
#define DPMSExtension 1
#define GLXEXT 1

#ifdef DAMAGE
extern _X_EXPORT Bool noDamageExtension;
extern void DamageExtensionInit(void);
#endif

#if defined(DBE)
extern _X_EXPORT Bool noDbeExtension;
extern void DbeExtensionInit(void);
#endif

#if defined(DPMSExtension)
#include "extensions/dpmsconst.h"
extern _X_EXPORT Bool noDPMSExtension;
extern void DPMSExtensionInit(void);
#endif

extern Bool noGEExtension;
extern void GEExtensionInit(void);

#ifdef GLXEXT
extern _X_EXPORT Bool noGlxExtension;
extern void GlxExtensionInit(void);
#endif

#include "extensions/panoramiXproto.h"
extern _X_EXPORT Bool noPanoramiXExtension;
extern void PanoramiXExtensionInit(void);

#ifdef RANDR
extern _X_EXPORT Bool noRRExtension;
extern void RRExtensionInit(void);
#endif

#if defined(XRECORD)
extern void RecordExtensionInit(void);
#endif

extern _X_EXPORT Bool noRenderExtension;
extern void RenderExtensionInit(void);

#if defined(RES)
#include "extensions/XResproto.h"
extern _X_EXPORT Bool noResExtension;
extern void ResExtensionInit(void);
#endif

#if defined(SCREENSAVER)
#include "extensions/saver.h"
extern _X_EXPORT Bool noScreenSaverExtension;
extern void ScreenSaverExtensionInit(void);
#endif

#include "extensions/shapeproto.h"
extern void ShapeExtensionInit(void);

#ifdef MITSHM
#include "extensions/shm.h"
#include "extensions/shmproto.h"
extern _X_EXPORT Bool noMITShmExtension;
extern void ShmExtensionInit(void);
#endif

extern void SyncExtensionInit(void);

extern void XCMiscExtensionInit(void);

#ifdef XCSECURITY
#include <X11/extensions/secur.h>
#include "securitysrv.h"
extern _X_EXPORT Bool noSecurityExtension;
extern void SecurityExtensionInit(void);
#endif

#ifdef XF86BIGFONT
#include <X11/extensions/xf86bigfproto.h>
extern _X_EXPORT Bool noXFree86BigfontExtension;
extern void XFree86BigfontExtensionInit(void);
#endif

extern void BigReqExtensionInit(void);

extern _X_EXPORT Bool noXFixesExtension;
extern void XFixesExtensionInit(void);

extern void XInputExtensionInit(void);
extern _X_EXPORT void AssignTypeAndName(DeviceIntPtr dev,
                                        Atom type,
                                        const char *name);

#include "extensions/XKB.h"
extern void XkbExtensionInit(void);

#if defined(XSELINUX)
#include "xselinux.h"
extern _X_EXPORT Bool noSELinuxExtension;
extern void SELinuxExtensionInit(void);
#endif

#ifdef XTEST
#include "extensions/xtestconst.h"
#include "extensions/xtestproto.h"
extern void XTestExtensionInit(void);
#endif

#if defined(XV)
#include "extensions/Xv.h"
#include "extensions/XvMC.h"
extern _X_EXPORT Bool noXvExtension;
extern void XvExtensionInit(void);
extern void XvMCExtensionInit(void);
#endif

#if defined(DRI3)
#include "extensions/dri3proto.h"
extern void dri3_extension_init(void);
#endif

#if defined(PRESENT)
#include "extensions/presentproto.h"
#include "presentext.h"
#endif

#endif