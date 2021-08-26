#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stdio.h>
#include "../headers/x.h"
#include "../headers/xproto.h"
#include "../headers/keysym.h"
#include "../headers/inputstr.h"
#include "../headers/scrnintstr.h"
#include "../headers/windowstr.h"
#include "../headers/xkbsrv.h"
#include "../headers/extensions/XI.h"

void
XkbDDXKeybdCtrlProc(DeviceIntPtr dev, KeybdCtrl * ctrl)
{
    int realRepeat;

    realRepeat = ctrl->autoRepeat;
    if ((dev->kbdfeed) && (XkbDDXUsesSoftRepeat(dev)))
        ctrl->autoRepeat = 0;
    if (dev->key && dev->key->xkbInfo && dev->key->xkbInfo->kbdProc)
        (*dev->key->xkbInfo->kbdProc) (dev, ctrl);
    ctrl->autoRepeat = realRepeat;
    return;
}

int
XkbDDXUsesSoftRepeat(DeviceIntPtr pXDev)
{
    return 1;
}

void
XkbDDXChangeControls(DeviceIntPtr dev, XkbControlsPtr old, XkbControlsPtr new)
{
    unsigned changed, i;
    unsigned char *rep_old, *rep_new, *rep_fb;

    changed = new->enabled_ctrls ^ old->enabled_ctrls;
    for (rep_old = old->per_key_repeat,
         rep_new = new->per_key_repeat,
         rep_fb = dev->kbdfeed->ctrl.autoRepeats,
         i = 0; i < XkbPerKeyBitArraySize; i++) {
        if (rep_old[i] != rep_new[i]) {
            rep_fb[i] = rep_new[i];
            changed &= XkbPerKeyRepeatMask;
        }
    }

    if (changed & XkbPerKeyRepeatMask) {
        if (dev->kbdfeed->CtrlProc)
            (*dev->kbdfeed->CtrlProc) (dev, &dev->kbdfeed->ctrl);
    }
    return;
}
