#include <iostream>
#include <fstream>
#include <regex>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>    /* Contains hd_driveid structure */

#define HDIO_GET_IDENTITY	0x030d	/* get IDE identification info */

int main() {
    // Define the regex pattern to match the serial number
    std::regex serialRegex(".*serial_short=(\\S+).*");

    // Open the sysfs file for the openind
    std::ifstream openindFile("/sys/bus/usb-serial/devices/ttyUSB0/../../serial");
    if (!openindFile) {
        std::cerr << "Failed to open openind sysfs file" << std::endl;
        //return 1;
    }

    // Read the openind's serial number from the file
    std::string openindSerial;
    std::getline(openindFile, openindSerial);

    // Open the disk's device file
    int diskFile = open("/dev/sda", O_RDONLY);
    if (diskFile < 0) {
        std::cerr << "Failed to open disk device file" << std::endl;
        //return 1;
    }

    // Read the disk's serial number from the device file
    char diskSerial[21];
    if (ioctl(diskFile, 0x80081272, diskSerial) < 0) {
        std::cerr << "Failed to read disk serial number" << std::endl;
        close(diskFile);
        //return 1;
    }

    struct hd_driveid id;
    //int fd = open(device.getLogicalName().c_str(), O_RDONLY | O_NONBLOCK);

    //if (fd < 0)
    //return false;

    memset(&id, 0, sizeof(id));
    if (ioctl(diskFile, HDIO_GET_IDENTITY, &id) != 0)
    {
        close(diskFile);
        return 1;
    }

    // Use regex to extract the serial number from the openind's sysfs file
    std::smatch serialMatch;
    if (!std::regex_match(openindSerial, serialMatch, serialRegex)) {
        std::cerr << "Failed to extract openind serial number" << std::endl;
        //return 1;
    }
    openindSerial = serialMatch[1];

    std::cout << "Openind serial: " << openindSerial << std::endl;
    std::cout << "Disk serial: " << diskSerial << std::endl;

    for (int i = 0; i < 21; i++){
        std::cout << "Disk serial char " << i << ": " << diskSerial[i] << std::endl;
    }

    std::cout << "SERIAL NUMBER BUENO?: " << id.serial_no << std::endl;
    for (int j = 0; j < sizeof(id.serial_no); j++){
        std::cout << "DISK SERIAL BUENO char " << j << ": " << id.serial_no[j] << std::endl;
    }

    close(diskFile);
    return 0;
}
