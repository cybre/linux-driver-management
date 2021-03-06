/*
 * This file is part of linux-driver-management.
 *
 * Copyright © 2016-2017 Ikey Doherty
 *
 * linux-driver-management is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */

#include "device.h"

void ldm_device_free(LdmDevice *device)
{
        /* We'll free from last to first, in effect. */
        if (device->next) {
                ldm_device_free(device->next);
                device->next = NULL;
        }

        if (device->device_name) {
                free(device->device_name);
                device->device_name = NULL;
        }

        /* Call any implementation specific destructor */
        if (device->dtor) {
                device->dtor(device);
        }

        free(device);
}

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */
