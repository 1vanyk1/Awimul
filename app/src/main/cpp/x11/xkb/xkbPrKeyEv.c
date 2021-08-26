#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stdio.h>
#include <math.h>
#include "../headers/x.h"
#include "../headers/xproto.h"
#include "../headers/keysym.h"
#include "../headers/misc.h"
#include "../headers/inputstr.h"
#include "../headers/exevents.h"
#include "../headers/eventstr.h"
#include "../headers/xkbsrv.h"
#include <ctype.h>
#include "../headers/events.h"

/***====================================================================***/

void
XkbProcessKeyboardEvent(DeviceEvent *event, DeviceIntPtr keybd)
{
    KeyClassPtr keyc = keybd->key;
    XkbSrvInfoPtr xkbi;
    int key;
    XkbBehavior behavior;
    unsigned ndx;

    xkbi = keyc->xkbInfo;
    key = event->detail.key;
    if (xkbDebugFlags & 0x8)
        DebugF("[xkb] XkbPKE: Key %d %s\n", key,
               (event->type == ET_KeyPress ? "down" : "up"));

    if (xkbi->repeatKey == key && event->type == ET_KeyRelease &&
        !(xkbi->desc->ctrls->enabled_ctrls & XkbRepeatKeysMask))
        AccessXCancelRepeatKey(xkbi, key);

    behavior = xkbi->desc->server->behaviors[key];
    /* The "permanent" flag indicates a hard-wired behavior that occurs */
    /* below XKB, such as a key that physically locks.   XKB does not   */
    /* do anything to implement the behavior, but it *does* report that */
    /* key is hardwired */

    if (!(behavior.type & XkbKB_Permanent)) {
        switch (behavior.type) {
            case XkbKB_Default:
                /* Neither of these should happen in practice, but ignore them
                   anyway. */
                if (event->type == ET_KeyPress && !event->key_repeat &&
                    key_is_down(keybd, key, KEY_PROCESSED))
                    return;
                else if (event->type == ET_KeyRelease &&
                         !key_is_down(keybd, key, KEY_PROCESSED))
                    return;
                break;
            case XkbKB_Lock:
                if (event->type == ET_KeyRelease)
                    return;
                else if (key_is_down(keybd, key, KEY_PROCESSED))
                    event->type = ET_KeyRelease;
                break;
            case XkbKB_RadioGroup:
                ndx = (behavior.data & (~XkbKB_RGAllowNone));
                if (ndx < xkbi->nRadioGroups) {
                    XkbRadioGroupPtr rg;

                    if (event->type == ET_KeyRelease)
                        return;

                    rg = &xkbi->radioGroups[ndx];
                    if (rg->currentDown == event->detail.key) {
                        if (behavior.data & XkbKB_RGAllowNone) {
                            event->type = ET_KeyRelease;
                            XkbHandleActions(keybd, keybd, event);
                            rg->currentDown = 0;
                        }
                        return;
                    }
                    if (rg->currentDown != 0) {
                        int tmpkey = event->detail.key;

                        event->type = ET_KeyRelease;
                        event->detail.key = rg->currentDown;
                        XkbHandleActions(keybd, keybd, event);
                        event->type = ET_KeyPress;
                        event->detail.key = tmpkey;
                    }
                    rg->currentDown = key;
                }
                else
                    ErrorF("[xkb] InternalError! Illegal radio group %d\n", ndx);
                break;
            case XkbKB_Overlay1:
            case XkbKB_Overlay2:
            {
                unsigned which;
                unsigned overlay_active_now;
                unsigned is_keyrelease = (event->type == ET_KeyRelease) ? 1 : 0;
                /* Remembers whether the key was pressed while overlay was down,
                 * for when overlay is already released, but the key is not. */
                unsigned key_was_overlaid = 0;

                if (behavior.type == XkbKB_Overlay1)
                    which = XkbOverlay1Mask;
                else
                    which = XkbOverlay2Mask;
                overlay_active_now = (xkbi->desc->ctrls->enabled_ctrls & which) ? 1 : 0;

                if ((unsigned char)key == key) {
                    key_was_overlaid = BitIsOn(xkbi->overlay_perkey_state, key);
                    if (!is_keyrelease) {
                        if (overlay_active_now)
                            SetBit(xkbi->overlay_perkey_state, key);
                    } else {
                        if (key_was_overlaid)
                            ClearBit(xkbi->overlay_perkey_state, key);
                    }
                }

                if ((overlay_active_now || key_was_overlaid) &&
                    (behavior.data >= xkbi->desc->min_key_code) &&
                    (behavior.data <= xkbi->desc->max_key_code)) {
                    event->detail.key = behavior.data;
                }
            }
                break;
            default:
                ErrorF("[xkb] unknown key behavior 0x%04x\n", behavior.type);
                break;
        }
    }
    XkbHandleActions(keybd, keybd, event);
    return;
}

void
ProcessKeyboardEvent(InternalEvent *ev, DeviceIntPtr keybd)
{

    KeyClassPtr keyc = keybd->key;
    XkbSrvInfoPtr xkbi = NULL;
    ProcessInputProc backup_proc;
    xkbDeviceInfoPtr xkb_priv = XKBDEVICEINFO(keybd);
    DeviceEvent *event = &ev->device_event;
    int is_press = (event->type == ET_KeyPress);
    int is_release = (event->type == ET_KeyRelease);

    /* We're only interested in key events. */
    if (!is_press && !is_release) {
        UNWRAP_PROCESS_INPUT_PROC(keybd, xkb_priv, backup_proc);
        keybd->_public.processInputProc(ev, keybd);
        COND_WRAP_PROCESS_INPUT_PROC(keybd, xkb_priv, backup_proc,
                                     xkbUnwrapProc);
        return;
    }

    xkbi = keyc->xkbInfo;

    /* If AccessX filters are active, then pass it through to
     * AccessXFilter{Press,Release}Event; else, punt to
     * XkbProcessKeyboardEvent.
     *
     * If AXF[PK]E don't intercept anything (which they probably won't),
     * they'll punt through XPKE anyway. */
    if ((xkbi->desc->ctrls->enabled_ctrls & XkbAllFilteredEventsMask)) {
        if (is_press)
            AccessXFilterPressEvent(event, keybd);
        else if (is_release)
            AccessXFilterReleaseEvent(event, keybd);
        return;
    }
    else {
        XkbProcessKeyboardEvent(event, keybd);
    }

    return;
}
