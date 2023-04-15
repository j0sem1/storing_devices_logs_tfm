#ifndef _HARD_DISK_H
#define _HARD_DISK_H

#include <string>
#include <linux/hdreg.h>    /* Contains hd_driveid structure */
#include <regex>    // For regex and map
#include <set> // For sets

#define HDIO_GET_IDENTITY	0x030d	/* get IDE identification info */

// Structre which contains information of hard disks
struct hardDiskInfo {
    bool isEmpty;
    //int idVendor;¡
    std::string product;
    //std::string manufacturer;
    std::string serialNumber;
    std::string version;
    unsigned long long size;
};

void initializeHardDiskInfoStructure(hardDiskInfo* hard_disk_info);

hardDiskInfo getInfoFromDisk(std::string device_name);

std::map<std::string, hardDiskInfo> getDisksInfo();

std::set<std::string> getDisksSet();

#endif