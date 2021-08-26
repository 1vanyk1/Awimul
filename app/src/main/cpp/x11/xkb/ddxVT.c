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

int
XkbDDXSwitchScreen(DeviceIntPtr dev, KeyCode key, XkbAction *act)
{
    return 1;
}
