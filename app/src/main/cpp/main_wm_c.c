#include <time.h>
#include "x11/xlc/XimThai.h"
#include "x11/headers/extensions/XInput.h"
#include "main_wm.h"
#include "main_wm_c.h"
#include "x11/headers/extensions/XInput2.h"
#include "x11/xext/xacestr.h"
#include "x11/headers/inputstr.h"
#include "x11/headers/extensions/XTest.h"
#include "x11/headers/inpututils.h"
#include "x11/headers/keysym.h"
#include "x11/mi/mipointer.h"
#include "x11/headers/scrnintstr.h"

void get_mouse_device() {
    mouseDisplay = XOpenDisplay(NULL);
    int device_count;
    XDeviceInfo *devices = XListInputDevices(mouseDisplay, &device_count);
    XDeviceInfo device_mouse;
    for (int i = 0; i < device_count && devices->use != IsXPointer; ++i, devices++);
    if (!devices) {
        mouseDevice = NULL;
        return;
    }
    device_mouse = *devices;
    DeviceIntPtr d;
    int ii = 0;
    for (d = inputInfo.devices; d && d->id != device_mouse.id; d = d->next, ii++);
    if (!d) {
        mouseDevice = NULL;
        return;
    }
    mouseDevice = d;
}

void get_keyboard_device() {
    int device_count;
    XDeviceInfo *devices = XListInputDevices(mouseDisplay, &device_count);
    XDeviceInfo device_keyboard;
    for (int i = 0; i < device_count && devices->use != IsXKeyboard; ++i, devices++);
    if (!devices) {
        keyboardDevice = NULL;
        return;
    }
    device_keyboard = *devices;
    DeviceIntPtr d;
    int ii = 0;
    for (d = inputInfo.devices; d && d->id != device_keyboard.id; d = d->next, ii++);
    if (!d) {
        keyboardDevice = NULL;
        return;
    }
    keyboardDevice = d;
}

void move_mouse(int x, int y)
{
    int valuators[2];
    ValuatorMask mask;
    valuators[0] = x;
    valuators[1] = y;
    valuator_mask_set_range(&mask, 0, 2, valuators);
    int flags = 0;
    if (valuator_mask_num_valuators(&mask) > 0) {
        flags = POINTER_ABSOLUTE;
    }
    QueuePointerEvents(mouseDevice, MotionNotify, 0, flags, &mask);
    XSync(mouseDisplay, 0);
    usleep(1);
}


void mouse_button_action(int x, int y, int button, int is_down)
{
    int valuators[2];
    ValuatorMask mask;
    valuators[0] = x;
    valuators[1] = y;
    valuator_mask_set_range(&mask, 0, 2, valuators);
    int flags = 0;
    if (valuator_mask_num_valuators(&mask) > 0) {
        flags = POINTER_ABSOLUTE;
    }
    QueuePointerEvents(mouseDevice, is_down ? ButtonPress : ButtonRelease, button, flags, &mask);
    XSync(mouseDisplay, 0);
    usleep(100);
}


XKeyEvent createKeyEvent(Display *display, Window win,
                         Window winRoot, Bool press,
                         int keycode, int modifiers)
{
    XKeyEvent event;

    event.display     = display;
    event.window      = win;
    event.root        = winRoot;
    event.subwindow   = None;
    event.time        = CurrentTime;
    event.x           = 1;
    event.y           = 1;
    event.x_root      = 1;
    event.y_root      = 1;
    event.same_screen = True;
    event.keycode     = XKeysymToKeycode(display, keycode);
    event.state       = modifiers;
    if(press)
        event.type = KeyPress;
    else
        event.type = KeyRelease;

    return event;
}

void XFakeKeypress(Display *display, int keycode)
{
    XKeyEvent event;
    Window current_focus_window;
    int current_focus_revert;

    XGetInputFocus(/* display = */ display, /* focus_return = */
                                   &current_focus_window, /* revert_to_return = */ &current_focus_revert);

    event.type = /* (const) */ KeyPress;
    event.display = display;
    event.window = current_focus_window;
    event.root = DefaultRootWindow(/* display = */ display);
    event.subwindow = /* (const) */ None;
    event.time = 1000 * time(NULL);
    event.x = 0;
    event.y = 0;
    event.x_root = 0;
    event.y_root = 0;
    event.state = /* (const) */ ShiftMask;
    event.keycode = keycode;
    event.same_screen = /* (const) */ True;

    XSendEvent(/* display = */ display, /* w = (const) */ InputFocus,
            /* propagate = (const) */ True, /* event_mask = (const) */
                               KeyPressMask, /* event_send = */ (XEvent *)(&event));

    event.type = /* (const) */ KeyRelease;
    event.time = 1000 * time(/* tloc = */ NULL);

    XSendEvent(/* display = */ display, /* w = (const) */ InputFocus,
            /* propagate = (const) */ True, /* event_mask = (const) */
                               KeyReleaseMask, /* event_send = */ (XEvent *)(&event));
}

void key_press_c(int key_code, int is_down)
{

    int keycode = XK_q;
    ValuatorMask mask;
    ALOGE("%d", keycode);
    valuator_mask_zero(&mask);



    QueueKeyboardEvents(keyboardDevice, KeyPress, keycode);
    XSync(mouseDisplay, 0);
//    XFlush(mouseDisplay);
    usleep(100);
    QueueKeyboardEvents(keyboardDevice, KeyRelease, keycode);
    XSync(mouseDisplay, 0);
    usleep(100);
//    XFlush(mouseDisplay);

//    LockDisplay(mouseDisplay);
//    int keycode = XKeysymToKeycode(mouseDisplay, XStringToKeysym("q"));
//    keycode = XK_q;
//    ALOGE("%d", keycode);
//
//
//    Window winRoot = XDefaultRootWindow(mouseDisplay);
//
//// Find the window which has the current keyboard focus.
//    Window winFocus;
//    int    revert;
//    XGetInputFocus(mouseDisplay, &winFocus, &revert);
//
//// Send a fake key press event to the window.
//    XKeyEvent event = createKeyEvent(mouseDisplay, winFocus, winRoot, TRUE, XK_q, 0);
//    XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
//
//    XSync(mouseDisplay, 0);
//    XFlush(mouseDisplay);
//
//// Send a fake key release event to the window.
//    event = createKeyEvent(mouseDisplay, winFocus, winRoot, FALSE, XK_q, 0);
//    XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
//
//
//    XTestFakeKeyEvent(mouseDisplay, keycode, False, 0);
//    XSync(mouseDisplay, 0);
//    XFlush(mouseDisplay);
//    XTestFakeKeyEvent(mouseDisplay, keycode, True, 0);
//    XSync(mouseDisplay, 0);
//    XFlush(mouseDisplay);
//    XTestFakeKeyEvent(mouseDisplay, keycode, False, 0);
//    XSync(mouseDisplay, 0);
//    XFlush(mouseDisplay);
//
//    ValuatorMask mask;
//
//    valuator_mask_zero(&mask);
//
//    QueueKeyboardEvents(keyboardDevice, KeyPress, keycode);
//    XSync(mouseDisplay, 0);
//    XFlush(mouseDisplay);
//    QueueKeyboardEvents(keyboardDevice, KeyRelease, keycode);
//    XSync(mouseDisplay, 0);
//    XFlush(mouseDisplay);
//
//    XFakeKeypress(mouseDisplay, keycode);
//
////    for (int i = 0; i < 128; i++) {
////        sleep(1);
////        ALOGE("i^ %d", i);
////        QueueKeyboardEvents(mouseDevice, KeyPress, i);
////        XSync(mouseDisplay, 0);
////        QueueKeyboardEvents(mouseDevice, KeyRelease, i);
////        XSync(mouseDisplay, 0);
////    }
////    QueueKeyboardEvents(mouseDevice, is_down ? KeyPress : KeyRelease, key_code);
////    XSync(mouseDisplay, 0);
//    UnlockDisplay(mouseDisplay);
//    usleep(100);
}
