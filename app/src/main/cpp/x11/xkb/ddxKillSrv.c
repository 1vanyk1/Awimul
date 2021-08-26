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
XkbDDXTerminateServer(DeviceIntPtr dev, KeyCode key, XkbAction *act)
{
    if (dev != inputInfo.keyboard)
        GiveUp(1);

    return 0;
}
