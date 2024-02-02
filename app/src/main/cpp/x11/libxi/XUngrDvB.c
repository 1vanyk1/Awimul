#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

int
XUngrabDeviceButton(
        register Display	*dpy,
        XDevice		*dev,
        unsigned int	 button,	/* CARD8 */
        unsigned int	 modifiers,	/* CARD16 */
        XDevice		*modifier_dev,
        Window		 grab_window)
{
    register xUngrabDeviceButtonReq *req;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);
    GetReq(UngrabDeviceButton, req);

    req->reqType = info->codes->major_opcode;
    req->ReqType = X_UngrabDeviceButton;
    req->grabbed_device = dev->device_id;
    req->button = button;
    req->modifiers = modifiers;
    if (modifier_dev)
        req->modifier_device = modifier_dev->device_id;
    else
        req->modifier_device = UseXKeyboard;
    req->grabWindow = grab_window;
    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}
