#include "hard_disk.h"
#include <iostream> /* Standard and error output */
#include <fcntl.h> /* open() function */
#include <sys/ioctl.h> /* ioctl() function */
#include <unistd.h> /* close() function */
#include <regex> /* memset() function and regex operations */
//#include <fstream>
#include <dirent.h> // For DIR type and functions

// TODO: Delte
//#include <iostream>
//#include <stdio.h>

#ifndef BLKGETSIZE
#define BLKGETSIZE _IO(0x12,96)                   /* return device size */
#endif
#ifndef BLKGETSIZE64
#define BLKGETSIZE64 _IOR(0x12,114,size_t)        /* size in bytes */
#endif

#ifndef BLKSSZGET
#define BLKSSZGET  _IO(0x12,104)                  /* get block device sector size */
#endif
#ifndef BLKPBSZGET
#define BLKPBSZGET _IO(0x12,123)
#endif

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
    hard_disk_info->isEmpty = true;
    hard_disk_info->product = "";
    hard_disk_info->serialNumber = "";
    hard_disk_info->version = "";
    hard_disk_info->size = 0;
}


hardDiskInfo getInfoFromDisk(std::string device_name){

    hd_driveid drive_info;
    hardDiskInfo hard_disk_info;

    memset(&drive_info, 0, sizeof(drive_info));
    initializeHardDiskInfoStructure(&hard_disk_info);

    // Open the disk's device file
    int diskFile = open(device_name.c_str(), O_RDONLY);
    if (diskFile < 0) {
        std::cerr << "Failed to open disk device file" << std::endl;
        return hard_disk_info;
    }

    // Get disk information with HDIO_GET_IDENTITY option in ioctl() function
    if (ioctl(diskFile, HDIO_GET_IDENTITY, &drive_info) != 0){
        close(diskFile);
        return hard_disk_info;
    }
    
    hard_disk_info.isEmpty = false;

    hard_disk_info.serialNumber = unsignedCharToString(drive_info.serial_no, sizeof(drive_info.serial_no));
    hard_disk_info.product = unsignedCharToString(drive_info.model, sizeof(drive_info.model));
    hard_disk_info.version = unsignedCharToString(drive_info.fw_rev, sizeof(drive_info.fw_rev));

    // Get disk information with HDIO_GET_IDENTITY option in ioctl() function
    if (ioctl(diskFile, BLKGETSIZE, &drive_info) != 0){
        close(diskFile);
        return hard_disk_info;
    }

    unsigned long long bytes = 0;
    long size = 0;
    int sectsize = 0;
    int physsectsize = 0;

    // BLKGETSIZE64 Get the size of the block device /dev/sd_. It produces a 64-bit result which is the size in bytes.
    if (ioctl(diskFile, BLKGETSIZE64, &bytes) == 0){
        hard_disk_info.size = bytes;
    } else {
        /*
            There is another ioctl with similar functionality to BLKGETSIZE64 called BLKGETSIZE.
            It returns the device size as a number of 512-byte blocks, but because it does so in the form of an unsigned long it may fail if the size is 2TB or greater.
            For this reason it is better to use BLKGETSIZE64 where possible, but if you require compatibility with very old versions of Linux (prior to 2.4.10)
            then it may be appropriate to use BLKGETSIZE as a fallback.
        */
        if (ioctl(diskFile, BLKSSZGET, &sectsize) < 0){
            sectsize = 0;
        } else {
        }
        if (ioctl(diskFile, BLKGETSIZE, &size) == 0){
        }
        if ((size > 0) && (sectsize > 0)){
            hard_disk_info.size = (unsigned long long) size * (unsigned long long) sectsize;
        }
    }

    return hard_disk_info;
}

std::map<std::string, hardDiskInfo> getDisksInfo(){
    std::map<std::string, hardDiskInfo> disks_map;
    std::set<std::string> disks_set;

    disks_set = getDisksSet();

    for (auto it : disks_set){
        disks_map.insert({it, getInfoFromDisk(it)});
    }

    return disks_map;
}

std::set<std::string> getDisksSet(){

    DIR *dir;
    struct dirent *ent;
    int n_disks = 0;
    std::string disks_string = "";
    //std::string *disks;
    char *ptr;

    std::set<std::string> disks;

    std::cmatch disk_found;
    std::regex sd_regex;

    sd_regex = std::regex("sd[a-z]");

    if ((dir = opendir("/dev/")) != NULL){

        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL){
            if (regex_search(ent->d_name, disk_found, sd_regex)){
                disks.insert("/dev/" + disk_found.str());
            }
        }

        closedir(dir);

    } else {
        /* could not open directory */
        perror("Could not open /dev/ directory. Try with supersu rights.");
    }

    return disks;
}