#include <iostream>
#include <libudev.h>

int main()
{
    struct udev* udev = udev_new();
    if (!udev)
    {
        std::cerr << "Failed to create udev context." << std::endl;
        return 1;
    }

    struct udev_enumerate* enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "block");
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry* entry;

    udev_list_entry_foreach(entry, devices)
    {
        const char* path = udev_list_entry_get_name(entry);
        struct udev_device* device = udev_device_new_from_syspath(udev, path);

        if (!device)
        {
            std::cerr << "Failed to create udev device." << std::endl;
            continue;
        }

        const char* serial = udev_device_get_property_value(device, "ID_SERIAL_SHORT");
        if (serial)
        {
            std::cout << "Path: " << path << std::endl;
            std::cout << "Device: " << device << std::endl;
            std::cout << "Serial number: " << serial << std::endl;
        }

        udev_device_unref(device);
    }

    udev_enumerate_unref(enumerate);
    udev_unref(udev);

    return 0;
}