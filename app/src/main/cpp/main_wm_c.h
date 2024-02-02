#ifndef AWIMUL_MAIN_WM_C_H
#define AWIMUL_MAIN_WM_C_H
#include "x11/headers/xlibint.h"
#include "x11/xext/xacestr.h"

DeviceIntPtr mouseDevice;
DeviceIntPtr keyboardDevice;
Display *mouseDisplay;

#ifdef __cplusplus
extern "C" {
#endif

extern void move_mouse(int x, int y);

extern void mouse_button_action(int x, int y, int button, int is_down);

extern void key_press_c(int key_code, int is_down);

extern void get_mouse_device();

extern void get_keyboard_device();

#ifdef __cplusplus
};
#endif


#define MOUSE_ACTION_DOWN 1
#define MOUSE_ACTION_UP 0
#define MOUSE_ACTION_MOVE 2



#endif //AWIMUL_MAIN_WM_C_H
