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

#define _GNU_SOURCE

#include <pci/pci.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "gpu.h"
#include "scanner.h"

/**
 * Return PCI id in format appropriate to X.Org (decimal, prefixed)
 */
static char *get_xorg_pci_id(LdmPCIAddress *addr)
{
        char *p = NULL;
        int ret = asprintf(&p, "PCI:%d:%d:%d", addr->bus, addr->dev, addr->func);
        if (ret < 0) {
                return NULL;
        }
        return p;
}

/**
 * Demo code follows
 */
static void print_device(LdmDevice *device)
{
        if (device->type != LDM_DEVICE_GPU) {
                fprintf(stderr, "Ignoring unknown device with name '%s'\n", device->device_name);
                return;
        }
        LdmGPU *dev = (LdmGPU *)device;
        const char *vendor = NULL;

        switch (dev->vendor_id) {
        case PCI_VENDOR_ID_INTEL:
                vendor = "Intel";
                break;
        case PCI_VENDOR_ID_NVIDIA:
                vendor = "NVIDIA";
                break;
        case PCI_VENDOR_ID_AMD:
                vendor = "AMD";
                break;
        default:
                vendor = "<unknown>";
                break;
        }
        fprintf(stderr,
                " %02x:%02x.%x: Discovered device\n",
                dev->address.bus,
                dev->address.dev,
                dev->address.func);
        fprintf(stderr, " \u251C Vendor: %s\n", vendor);
        fprintf(stderr, " \u251C Name: %s\n", device->device_name);
        char *pci_id = get_xorg_pci_id(&(dev->address));
        fprintf(stderr, " \u2514 X.Org ID: %s\n", pci_id ? pci_id : "<unknown>");

        if (pci_id) {
                free(pci_id);
        }
        fputs("\n", stderr);
}

int main(int argc, char **argv)
{
        LdmDevice *device = NULL;

        device = ldm_scan_devices();
        if (!device) {
                fputs("Unable to locate devices\n", stderr);
                return EXIT_FAILURE;
        }

        /* Iterate all discovered devices */
        for (LdmDevice *dev = device; dev; dev = dev->next) {
                print_device(dev);
        }

        ldm_device_free(device);

        return EXIT_SUCCESS;
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
