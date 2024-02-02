#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include "../headers/extensions/XI2proto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput2.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

#define FP1616toDBL(x) ((x) * 1.0 / (1 << 16))

Bool
XIQueryPointer(Display     *dpy,
               int         deviceid,
               Window      w,
               Window      *root,
               Window      *child,
               double      *root_x,
               double      *root_y,
               double      *win_x,
               double      *win_y,
               XIButtonState       *buttons,
               XIModifierState     *mods,
               XIGroupState        *group)
{
    xXIQueryPointerReq *req;
    xXIQueryPointerReply rep;

    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_2_0, info) == -1)
        return False;

    GetReq(XIQueryPointer, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_XIQueryPointer;
    req->deviceid = deviceid;
    req->win = w;

    if (!_XReply(dpy, (xReply *)&rep,
                 (sizeof(xXIQueryPointerReply) - sizeof(xReply))/4, xFalse)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return False;
    }


    *root = rep.root;
    *child = rep.child;
    *root_x = FP1616toDBL(cvtINT16toInt(rep.root_x));
    *root_y = FP1616toDBL(cvtINT16toInt(rep.root_y));
    *win_x = FP1616toDBL(cvtINT16toInt(rep.win_x));
    *win_y = FP1616toDBL(cvtINT16toInt(rep.win_y));

    mods->base          = rep.mods.base_mods;
    mods->latched       = rep.mods.latched_mods;
    mods->locked        = rep.mods.locked_mods;
    mods->effective     = mods->base | mods->latched | mods->locked;

    group->base         = rep.group.base_group;
    group->latched      = rep.group.latched_group;
    group->locked       = rep.group.locked_group;
    group->effective    = group->base | group->latched | group->locked;

    buttons->mask_len   = rep.buttons_len * 4;
    buttons->mask       = malloc(buttons->mask_len);
    if (buttons->mask)
        _XRead(dpy, (char*)buttons->mask, buttons->mask_len);

    UnlockDisplay(dpy);
    SyncHandle();
    return rep.same_screen;
}
