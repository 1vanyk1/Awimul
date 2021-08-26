#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../../headers/x.h"
#include "../../mi/mi.h"
#include "../../headers/xproto.h"
#include "../../headers/scrnintstr.h"
#include "../../headers/inputstr.h"
#include "../../headers/xos.h"
#include "../../mi/mipointer.h"
#include "../../headers/xkbsrv.h"
#include "../../headers/keysym.h"
#include "../../headers/xserver-properties.h"
#include "../../headers/exevents.h"
#include "../../headers/extinit.h"

void
ProcessInputEvents(void)
{
    mieqProcessInputEvents();
}

void
DDXRingBell(int volume, int pitch, int duration)
{
}

#define VFB_MIN_KEY 8
#define VFB_MAX_KEY 255

static int
vfbKeybdProc(DeviceIntPtr pDevice, int onoff)
{
    DevicePtr pDev = (DevicePtr) pDevice;

    switch (onoff) {
        case DEVICE_INIT:
            InitKeyboardDeviceStruct(pDevice, NULL, NULL, NULL);
            break;
        case DEVICE_ON:
            pDev->on = TRUE;
            break;
        case DEVICE_OFF:
            pDev->on = FALSE;
            break;
        case DEVICE_CLOSE:
            break;
    }
    return Success;
}

static int
vfbMouseProc(DeviceIntPtr pDevice, int onoff)
{
#define NBUTTONS 3
#define NAXES 2

    BYTE map[NBUTTONS + 1];
    DevicePtr pDev = (DevicePtr) pDevice;
    Atom btn_labels[NBUTTONS] = { 0 };
    Atom axes_labels[NAXES] = { 0 };

    switch (onoff) {
        case DEVICE_INIT:
            map[1] = 1;
            map[2] = 2;
            map[3] = 3;

            btn_labels[0] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_LEFT);
            btn_labels[1] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_MIDDLE);
            btn_labels[2] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_RIGHT);

            axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_X);
            axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_Y);

            InitPointerDeviceStruct(pDev, map, NBUTTONS, btn_labels,
                                    (PtrCtrlProcPtr) NoopDDA,
                                    GetMotionHistorySize(), NAXES, axes_labels);
            break;

        case DEVICE_ON:
            pDev->on = TRUE;
            break;

        case DEVICE_OFF:
            pDev->on = FALSE;
            break;

        case DEVICE_CLOSE:
            break;
    }
    return Success;

#undef NBUTTONS
#undef NAXES
}

void
InitInput(int argc, char *argv[])
{
    DeviceIntPtr p, k;
    Atom xiclass;

    p = AddInputDevice(serverClient, vfbMouseProc, TRUE);
    k = AddInputDevice(serverClient, vfbKeybdProc, TRUE);
    xiclass = MakeAtom(XI_MOUSE, sizeof(XI_MOUSE) - 1, TRUE);
    AssignTypeAndName(p, xiclass, "Xvfb mouse");
    xiclass = MakeAtom(XI_KEYBOARD, sizeof(XI_KEYBOARD) - 1, TRUE);
    AssignTypeAndName(k, xiclass, "Xvfb keyboard");
    (void) mieqInit();
}

void
CloseInput(void)
{
    mieqFini();
}
