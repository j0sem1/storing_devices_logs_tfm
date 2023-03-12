#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <fcntl.h>
//#include </usr/include/linux/hdreg.h>

std::string get_disk_serial(const std::string& disk_device) {
    int fd = open(disk_device.c_str(), O_RDONLY);
    if (fd < 0) {
        std::cerr << "Error: could not open device " << disk_device << std::endl;
        return "";
    }
    hd_driveid_t drive_info;
    memset(&drive_info, 0, sizeof(drive_info));
    if (ioctl(fd, HDIO_GET_IDENTITY, &drive_info) < 0) {
        std::cerr << "Error: could not get drive identity for device " << disk_device << std::endl;
        close(fd);
        return "";
    }
    close(fd);
    std::string serial_number(reinterpret_cast<const char*>(drive_info.serial_no), sizeof(drive_info.serial_no));
    // Remove trailing spaces
    serial_number.erase(serial_number.find_last_not_of(' ') + 1);
    return serial_number;
}

int main() {
    std::string disk_device = "/dev/sda"; // Change this to the device file name of your disk
    std::string serial_number = get_disk_serial(disk_device);
    std::cout << "Serial number of " << disk_device << ": " << serial_number << std::endl;
    return 0;
}
