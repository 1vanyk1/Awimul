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

static void
XkbDDXUpdateIndicators(DeviceIntPtr dev, CARD32 new)
{
    dev->kbdfeed->ctrl.leds = new;
    (*dev->kbdfeed->CtrlProc) (dev, &dev->kbdfeed->ctrl);
    return;
}

void
XkbDDXUpdateDeviceIndicators(DeviceIntPtr dev, XkbSrvLedInfoPtr sli, CARD32 new)
{
    if (sli->fb.kf == dev->kbdfeed)
        XkbDDXUpdateIndicators(dev, new);
    else if (sli->_class == KbdFeedbackClass) {
        KbdFeedbackPtr kf;

        kf = sli->fb.kf;
        if (kf && kf->CtrlProc) {
            (*kf->CtrlProc) (dev, &kf->ctrl);
        }
    }
    else if (sli->_class == LedFeedbackClass) {
        LedFeedbackPtr lf;

        lf = sli->fb.lf;
        if (lf && lf->CtrlProc) {
            (*lf->CtrlProc) (dev, &lf->ctrl);
        }
    }
    return;
}
