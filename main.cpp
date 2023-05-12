#include <iostream>
#include <fstream>
#include <string>
//#include <regex>
#include <stdexcept>
#include <stdio.h>
//#include <nlohmann/json.hpp>
#include "hard_disk.h"
#include "usb.h"
#include "output.h"
#include <set> // For sets

#include <unistd.h>
//#include <sys/types.h>

#include <math.h>


void printUSBsMap(std::map<std::string, usbInfo> usbs_map){

    std::map<std::string, usbInfo>::iterator it;
    int n = 1;

    for (it = usbs_map.begin(); it != usbs_map.end(); ++it){
        std::cout << "||| USB " << n << " |||" << std::endl;

        std::list<std::string>::iterator it_time;

        std::cout << "Connection times:" << std::endl;
        for (it_time = it->second.connectionTimes.begin(); it_time != it->second.connectionTimes.end(); ++it_time){
            std::cout << "   " << (*it_time) << std::endl;
        }

        std::cout << "Disconnection times:" << std::endl;
        for (it_time = it->second.disconnectionTimes.begin(); it_time != it->second.disconnectionTimes.end(); ++it_time){
            std::cout << "   " << (*it_time) << std::endl;
        }
        
        if (!it->second.idVendor){
            std::cout << "Vendor ID not found" << std::endl;
        } else {
            std::cout << "Vendor ID: " << it->second.idVendor << std::endl;
        }
        if (!it->second.idProduct){
            std::cout << "Product ID not found" << std::endl;
        } else {
            std::cout << "Product ID: " << it->second.idProduct << std::endl;
        }
        if (it->second.productName == ""){
            std::cout << "Product name not found" << std::endl;
        } else {
            std::cout << "Product name: " << it->second.productName << std::endl;
        }
        if (it->second.manufacturer == "" || it->second.manufacturer.find_first_not_of(' ') == std::string::npos){
            std::cout << "Manufacturer not found" << std::endl;
        } else {
            std::cout << "Manufacturer: " << it->second.manufacturer << std::endl;
        } if (it->second.serialNumber == ""){
            std::cout << "Serial number not found" << std::endl;
        } else {
            std::cout << "Serial number: " << it->second.serialNumber << std::endl;
        }
        std::cout << std::endl;

        n++;
    }
}

void printDisksMap(std::map<std::string, hardDiskInfo> disks_map){

    std::map<std::string, hardDiskInfo>::iterator it;

    for (it = disks_map.begin(); it != disks_map.end(); ++it){
        if (!it->second.isEmpty){
            std::cout << "||| " << it->first << " |||" << std::endl;
            std::cout << "Disk interface: " << it->second.diskType << std::endl;
            if (it->second.product == ""){
                std::cout << "Product not found" << std::endl;
            } else {
                std::cout << "Product: " << it->second.product << std::endl;
            }
            if (it->second.version == ""){
                std::cout << "Version not found" << std::endl;
            } else {
                std::cout << "Version: " << it->second.version << std::endl;
            }
            if (it->second.firmware == ""){
                std::cout << "Firmware version not found" << std::endl;
            } else {
                std::cout << "Firmware version: " << it->second.firmware << std::endl;
            }
            if (it->second.serialNumber == ""){
                std::cout << "Serial number not found" << std::endl;
            } else {
                std::cout << "Serial number: " << it->second.serialNumber << std::endl;
            }
            if (it->second.size == 0){
                std::cout << "Size not found" << std::endl;
            } else {
                std::cout << "Size: " << humanReadableByteCount(it->second.size, true) << std::endl;
            }
            std::cout << std::endl;
        }
    }

}

int main(){

    /*
    if (argc != 2) {
		fprintf(stderr, "Usage: %s <devname>\n", program_invocation_short_name);
		return 1;
	}
    */

	if (geteuid() > 0) {
		fprintf(stderr, "ERROR: Must be root to use\n");
		return 1;
	}

    printUSBsMap(getUSBsInfo());

    printDisksMap(getDisksInfo());

    /*
    std::cout << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << std::endl;
    
    printUSBsInformation();
    */

    //printDisksInformation();

    //test2();

    //printHTML_first();
    printHTML(getUSBsInfo(), getDisksInfo());

    return 0;
}