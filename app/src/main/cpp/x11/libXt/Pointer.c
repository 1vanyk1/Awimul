#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"
#include "PassivGraI.h"

#define AllButtonsMask (Button1Mask | Button2Mask | Button3Mask | Button4Mask | Button5Mask)

Widget
_XtProcessPointerEvent(XButtonEvent *event,
                       Widget widget,
                       XtPerDisplayInput pdi)
{
    XtDevice device = &pdi->pointer;
    XtServerGrabPtr newGrab = NULL, devGrab = &device->grab;
    Widget dspWidget = NULL;
    Boolean deactivateGrab = FALSE;

    switch (event->type) {
        case ButtonPress:
        {
            if (!IsServerGrab(device->grabType)) {
                Cardinal i;

                for (i = (Cardinal) pdi->traceDepth; i > 0 && !newGrab; i--)
                    newGrab = _XtCheckServerGrabsOnWidget((XEvent *) event,
                                                          pdi->trace[i - 1],
                                                          POINTER);
            }
            if (newGrab) {
                /* Activate the grab */
                device->grab = *newGrab;
                device->grabType = XtPassiveServerGrab;
            }
        }
            break;

        case ButtonRelease:
        {
            if ((device->grabType == XtPassiveServerGrab) &&
                !(event->state & (unsigned) (~(Button1Mask << (event->button - 1)))
                  & AllButtonsMask))
                deactivateGrab = TRUE;
        }
            break;
    }

    if (IsServerGrab(device->grabType) && !(devGrab)->ownerEvents)
        dspWidget = (devGrab)->widget;
    else
        dspWidget = widget;

    if (deactivateGrab)
        device->grabType = XtNoServerGrab;

    return dspWidget;
}
