#include <iostream>
//#include <systemd/sd-device.h>
#include "/home/j0sem1/Git/systemd/src/systemd/sd-device.h"

int main() {
    sd_device_list *list = nullptr;
    sd_device *dev = nullptr;

    int r = sd_device_list_new(&list);
    if (r < 0) {
        std::cerr << "Failed to create device list: " << strerror(-r) << std::endl;
        return 1;
    }

    r = sd_device_list_by_subsystem(list, "block");
    if (r < 0) {
        std::cerr << "Failed to get block devices: " << strerror(-r) << std::endl;
        sd_device_list_unref(list);
        return 1;
    }

    SD_DEVICE_FOREACH(list, dev) {
        const char *devtype, *devname;
        const char *size_str;
        uint64_t size;

        r = sd_device_get_devtype(dev, &devtype);
        if (r < 0) {
            std::cerr << "Failed to get device type: " << strerror(-r) << std::endl;
            continue;
        }

        if (strcmp(devtype, "disk") != 0) {
            // Skip non-disk block devices
            continue;
        }

        r = sd_device_get_sysattr_value(dev, "size", &size_str);
        if (r < 0) {
            std::cerr << "Failed to get device size: " << strerror(-r) << std::endl;
            continue;
        }

        size = strtoull(size_str, nullptr, 10);
        std::cout << "Device: " << sd_device_get_sysname(dev) << std::endl;
        std::cout << "Size: " << size << " bytes" << std::endl;
    }

    sd_device_list_unref(list);
    return 0;
}