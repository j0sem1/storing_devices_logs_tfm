#include <iostream>
#include <fstream>
#include <string>
//#include <regex>
#include <stdexcept>
#include <stdio.h>
#include <nlohmann/json.hpp>
#include "hard_disk.h"
#include "usb.h"
#include <set> // For sets

#include <unistd.h>
//#include <sys/types.h>

using namespace std;
using nlohmann::json;


void printUSBsMap(map<string, usbinfo> usbs_map){

    std::map<string, usbinfo>::iterator it;

    for (it = usbs_map.begin(); it != usbs_map.end(); ++it){
        cout << "||| " << it->first << " |||" << endl;
        cout << "Connection time: " << it->second.connectionTime << endl;
        cout << "Disconnection time: " << it->second.disconnectionTime << endl;
        if (!it->second.idVendor){
            cout << "Vendor ID not found" << endl;
        } else {
            cout << "Vendor ID: " << it->second.idVendor << endl;
        }
        if (!it->second.idProduct){
            cout << "Product ID not found" << endl;
        } else {
            cout << "Product ID: " << it->second.idProduct << endl;
        }
        if (it->second.productName == ""){
            cout << "Product name not found" << endl;
        } else {
            cout << "Product name: " << it->second.productName << endl;
        }
        if (it->second.manufacturer == ""){
            cout << "Manufacturer not found" << endl;
        } else {
            cout << "Manufacturer: " << it->second.manufacturer << endl;
        } if (it->second.serialNumber == ""){
            cout << "Serial number not found" << endl;
        } else {
            cout << "Serial number: " << it->second.serialNumber << endl;
        }
        cout << endl;
    }
}

// Get from: https://gist.github.com/haohaozaici/7fddba2d7f1038bd578cb6acf5c57ac0
// si - international system of units   
std::string humanReadableByteCount(unsigned long long bytes, bool si) {
    int unit = si ? 1000 : 1024;
    if (bytes < unit)
        return bytes + " B";
    int exp = (int) (log(bytes) / log(unit));
    std::string pre = ((std::string) (si ? "kMGTPE" : "KMGTPE"))[exp-1] + ((std::string) (si ? "" : "i"));
    char result[100];
    sprintf(result, "%.1llu %sB", bytes / (unsigned long long) pow(unit, exp), pre.c_str());
    return result;
}

void printDisksMap(std::map<std::string, hardDiskInfo> disks_map){

    std::map<std::string, hardDiskInfo>::iterator it;

    for (it = disks_map.begin(); it != disks_map.end(); ++it){
        if (!it->second.isEmpty){
            cout << "||| " << it->first << " |||" << endl;
            if (it->second.product == ""){
                cout << "Product not found" << endl;
            } else {
                cout << "Product: " << it->second.product << endl;
            }
            if (it->second.version == ""){
                cout << "Version not found" << endl;
            } else {
                cout << "Version: " << it->second.version << endl;
            }
            if (it->second.serialNumber == ""){
                cout << "Serial number not found" << endl;
            } else {
                cout << "Serial number: " << it->second.serialNumber << endl;
            }
            if (it->second.size == 0){
                cout << "Size not found" << endl;
            } else {
                cout << "Size: " << humanReadableByteCount(it->second.size, true) << endl;
            }
            cout << endl;
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
    
    example();

    return 0;
}