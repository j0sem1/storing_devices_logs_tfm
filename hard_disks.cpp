#include "hard_disks.h"
#include <iostream> /* Standard and error output */
#include <fcntl.h> /* open() function */
#include <sys/ioctl.h> /* ioctl() function */
#include <unistd.h> /* close() function */
#include <regex> /* memset() function and regex operations */
//#include <fstream>


std::string unsignedCharToString(unsigned char* src_char_array, int size){

    std::string final_string = "";

    for (int i = 0; i < size; i++){
        if (src_char_array[i] != ' '){
            final_string += src_char_array[i];
        }
    }

    return final_string;
}


void initializeHardDiskInfoStructure(hardDiskInfo* hard_disk_info){
    hard_disk_info->productName = "";
    hard_disk_info->manufacturer = "";
    hard_disk_info->serialNumber = "";
}


bool getInfoFromDisk(std::string device_name, hardDiskInfo* hard_disk_info){

    hd_driveid drive_info;

    memset(&drive_info, 0, sizeof(drive_info));
    // Function to initialize hard_disk_info
    // TODO

    // Open the disk's device file
    int diskFile = open(device_name.c_str(), O_RDONLY);
    if (diskFile < 0) {
        std::cerr << "Failed to open disk device file" << std::endl;
        return false;
    }

    // Get disk information with HDIO_GET_IDENTITY option in ioctl() function
    if (ioctl(diskFile, HDIO_GET_IDENTITY, &drive_info) != 0)
    {
        close(diskFile);
        return false;
    }
    
    hard_disk_info->serialNumber = unsignedCharToString(drive_info.serial_no, sizeof(drive_info.serial_no));

    /*
    for (int j = 0; j < sizeof(drive_info.serial_no); j++){
        std::cout << "DISK SERIAL BUENO char " << j << ": " << drive_info.serial_no[j] << std::endl;
    }
    */

    return true;
}