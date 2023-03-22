#ifndef _HARD_DISKS_H
#define _HARD_DISKS_H

#include <string>
#include <linux/hdreg.h>    /* Contains hd_driveid structure */

//using namespace std;

#define HDIO_GET_IDENTITY	0x030d	/* get IDE identification info */

// Structre which contains information of hard disks
struct hardDiskInfo {
    //int idVendor;
    //int idProduct;
    std::string productName;
    std::string manufacturer;
    std::string serialNumber;
};

void initializeHardDiskInfoStructure(hardDiskInfo* hard_disk_info);

bool getInfoFromDisk(std::string device_name, hardDiskInfo* hard_disk_info);

#endif