#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"
#include <limits.h>

XDeviceState *
XQueryDeviceState(
        register Display	*dpy,
        XDevice		*dev)
{
    int i, j;
    unsigned long rlen;
    size_t size = 0;
    xQueryDeviceStateReq *req;
    xQueryDeviceStateReply rep;
    XDeviceState *state = NULL;
    XInputClass *any, *Any;
    char *data = NULL, *end = NULL;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return NULL;

    GetReq(QueryDeviceState, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_QueryDeviceState;
    req->deviceid = dev->device_id;

    if (!_XReply(dpy, (xReply *) & rep, 0, xFalse))
        goto out;

    if (rep.length > 0) {
        if (rep.length < (INT_MAX >> 2)) {
            rlen = (unsigned long) rep.length << 2;
            data = Xmalloc(rlen);
            end = data + rlen;
        }
        if (!data) {
            _XEatDataWords(dpy, rep.length);
            goto out;
        }
        _XRead(dpy, data, rlen);

        for (i = 0, any = (XInputClass *) data; i < (int)rep.num_classes; i++) {
            if ((char *)any + sizeof(XInputClass) > end ||
                any->length == 0 || any->length > rlen)
                goto out;
            rlen -= any->length;

            switch (any->class) {
                case KeyClass:
                    size += sizeof(XKeyState);
                    break;
                case ButtonClass:
                    size += sizeof(XButtonState);
                    break;
                case ValuatorClass:
                {
                    xValuatorState *v = (xValuatorState *) any;
                    if ((char *)any + sizeof(xValuatorState) > end)
                        goto out;
                    size += (sizeof(XValuatorState) +
                             (v->num_valuators * sizeof(int)));
                }
                    break;
            }
            any = (XInputClass *) ((char *)any + any->length);
        }
        state = (XDeviceState *) Xmalloc(size + sizeof(XDeviceState));
        if (!state)
            goto out;

        state->device_id = dev->device_id;
        state->num_classes = rep.num_classes;
        state->data = (XInputClass *) (state + 1);

        Any = state->data;
        for (i = 0, any = (XInputClass *) data; i < (int)rep.num_classes; i++) {
            switch (any->class) {
                case KeyClass:
                {
                    xKeyState *k = (xKeyState *) any;
                    XKeyState *K = (XKeyState *) Any;

                    K->class = k->class;
                    K->length = sizeof(XKeyState);
                    K->num_keys = k->num_keys;
                    memcpy((char *)&K->keys[0], (char *)&k->keys[0], 32);
                    Any = (XInputClass *) (K + 1);
                }
                    break;
                case ButtonClass:
                {
                    xButtonState *b = (xButtonState *) any;
                    XButtonState *B = (XButtonState *) Any;

                    B->class = b->class;
                    B->length = sizeof(XButtonState);
                    B->num_buttons = b->num_buttons;
                    memcpy((char *)&B->buttons[0], (char *)&b->buttons[0], 32);
                    Any = (XInputClass *) (B + 1);
                }
                    break;
                case ValuatorClass:
                {
                    xValuatorState *v = (xValuatorState *) any;
                    XValuatorState *V = (XValuatorState *) Any;
                    CARD32 *valuators = (CARD32 *) (v + 1);

                    V->class = v->class;
                    V->length = sizeof(XValuatorState) +
                                v->num_valuators * sizeof(int);
                    V->num_valuators = v->num_valuators;
                    V->mode = v->mode;
                    Any = (XInputClass *) (V + 1);
                    V->valuators = (int *)Any;
                    for (j = 0; j < (int)V->num_valuators; j++)
                        *(V->valuators + j) = *valuators++;
                    Any = (XInputClass *) ((char *)Any +
                                           V->num_valuators * sizeof(int));
                }
                    break;
            }
            any = (XInputClass *) ((char *)any + any->length);
        }
    }
    out:
    Xfree(data);

    UnlockDisplay(dpy);
    SyncHandle();
    return (state);
}

void
XFreeDeviceState(XDeviceState *list)
{
    XFree((char *)list);
}
