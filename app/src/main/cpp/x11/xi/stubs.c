#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/x.h"
#include "../headers/xproto.h"
#include "../headers/inputstr.h"
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/XIstubs.h"
#include "../xext/xace.h"

/****************************************************************************
 *
 * Caller:	ProcXSetDeviceMode
 *
 * Change the mode of an extension device.
 * This function is used to change the mode of a device from reporting
 * relative motion to reporting absolute positional information, and
 * vice versa.
 * The default implementation below is that no such devices are supported.
 *
 */

int
SetDeviceMode(ClientPtr client, DeviceIntPtr dev, int mode)
{
    return BadMatch;
}

/****************************************************************************
 *
 * Caller:	ProcXSetDeviceValuators
 *
 * Set the value of valuators on an extension input device.
 * This function is used to set the initial value of valuators on
 * those input devices that are capable of reporting either relative
 * motion or an absolute position, and allow an initial position to be set.
 * The default implementation below is that no such devices are supported.
 *
 */

int
SetDeviceValuators(ClientPtr client, DeviceIntPtr dev,
                   int *valuators, int first_valuator, int num_valuators)
{
    return BadMatch;
}

/****************************************************************************
 *
 * Caller:	ProcXChangeDeviceControl
 *
 * Change the specified device controls on an extension input device.
 *
 */

int
ChangeDeviceControl(ClientPtr client, DeviceIntPtr dev, xDeviceCtl * control)
{
    return BadMatch;
}

/****************************************************************************
 *
 * Caller: configAddDevice (and others)
 *
 * Add a new device with the specified options.
 *
 */
int
NewInputDeviceRequest(InputOption *options, InputAttributes * attrs,
                      DeviceIntPtr *pdev)
{
    return BadValue;
}

/****************************************************************************
 *
 * Caller: configRemoveDevice (and others)
 *
 * Remove the specified device previously added.
 *
 */
void
DeleteInputDeviceRequest(DeviceIntPtr dev)
{
    RemoveDevice(dev, TRUE);
}

/****************************************************************************
 *
 * Caller: configRemoveDevice (and others)
 *
 * Remove any traces of the input device specified in config_info.
 * This is only necessary if the ddx keeps information around beyond
 * the NewInputDeviceRequest/DeleteInputDeviceRequest
 *
 */
void
RemoveInputDeviceTraces(const char *config_info)
{
}
