#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <fcntl.h>
#include <unistd.h>

std::string get_disk_serial(const std::string& disk_device) {
    int fd = open(disk_device.c_str(), O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        std::cerr << "Error: could not open device " << disk_device << std::endl;
        return "";
    }
    unsigned char id_cmd[16];
    memset(&id_cmd, 0, sizeof(id_cmd));
    id_cmd[0] = 0xec;
    if (ioctl(fd, HDIO_DRIVE_CMD, &id_cmd) < 0) {
        std::cerr << "Error: could not get drive identity for device " << disk_device << std::endl;
        close(fd);
        return "";
    }
    unsigned char id_data[512];
    if (read(fd, &id_data, sizeof(id_data)) < 0) {
        std::cerr << "Error: could not read drive identity for device " << disk_device << std::endl;
        close(fd);
        return "";
    }
    close(fd);
    std::string serial_number(reinterpret_cast<const char*>(&id_data[20]), 20);
    // Remove trailing spaces
    serial_number.erase(serial_number.find_last_not_of(' ') + 1);
    return serial_number;
}

int main() {
    std::string disk_device = "/dev/sda1"; // Change this to the device file name of your disk
    std::string serial_number = get_disk_serial(disk_device);
    std::cout << "Serial number of " << disk_device << ": " << serial_number << std::endl;
    return 0;
}
