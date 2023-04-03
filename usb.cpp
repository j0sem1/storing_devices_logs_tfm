#include "usb.h"
#include <fstream>
//#include <iostream>
//#include <sstream> // this will allow you to use stringstream in your program
#include <tabulate/table.hpp> /* For print tables */

void example() {
    tabulate::Table table;

    table.add_row({""});

    table[0][0].format().width(20);
    table[0][1].format().width(50);

    std::cout << table << std::endl;
}


int stringToInt(std::string stringToConvert){

    // Stringstream object, to input/output strings
    std::stringstream ss;
    int result;

    // Extract the string from the str variable (input the string in the stream)
    ss << stringToConvert;
    // Place the converted value to the int variable
    ss >> result;

    return result;
}

std::map<std::string, usbinfo>::iterator replaceInMap(std::map<std::string, usbinfo>* map, std::string key, usbinfo usb_info_new){

    usbinfo usb_info_old;
    std::map<std::string, usbinfo>::iterator it;

    it = (*map).find(key);
    usb_info_old = it->second;
    (*map).erase(it);
    return ((*map).insert({key, usb_info_new})).first;
}

std::map<std::string, usbinfo> getUSBsInfo(){
    // Variables
    std::string line, idVendor_string, idProduct_string, serialNumber_string, productName_string, manufacturer_string;
    std::regex usb_regex, date_regex, info_regex, disconnectionFound_regex, idVendorAndIdProduct_regex, idVendor_regex, idProduct_regex, serialNumber_regex, productName_regex, manufacturer_regex;
    std::ifstream file;
    std::cmatch id_found, connectionTime_found, disconnectionTime_found, info_found, idVendor_found, idProduct_found, serialNumber_found, productName_found, manufacturer_found;
    usbinfo usb_info = usbinfo();
    // Create a map and its iterator
    std::map<std::string, usbinfo> usb_map;
    std::map<std::string, usbinfo>::iterator it;

    // Initialization
    usb_regex = std::regex("usb [0-9]-[0-9]");
    date_regex = std::regex("[0-9]{2}:[0-9]{2}:[0-9]{2}");
    info_regex = std::regex(": ([a-z]|[A-Z])");
    disconnectionFound_regex = std::regex("USB disconnect");
    idVendorAndIdProduct_regex = std::regex("USB device found");
    idVendor_regex = std::regex("idVendor=([0-9])*");
    idProduct_regex = std::regex("idProduct=([0-9])*");
    serialNumber_regex = std::regex("SerialNumber: .*");
    productName_regex = std::regex("Product: .*");
    manufacturer_regex = std::regex("Manufacturer: .*");
    
    // Open syslog file and save the pointer to it in "file" variable
    file.open("/var/log/syslog");

    if (file.is_open()){
        while (getline(file, line)){

            // Match regex to see if it is a usb
            if (regex_search(line.c_str(), id_found, usb_regex)){

                // If key is not found in the map (this usb has not been added yet)
                if (usb_map.find(id_found.str()) == usb_map.end()){
                    // Keep connection time and save it in usb structure
                    regex_search(line.c_str(), connectionTime_found, date_regex);
                    usb_info.connectionTime = connectionTime_found.str();
                    // Insert this usb log in map
                    usb_map.insert({id_found.str(), usb_info});
                    // Reset usb_info variable
                    usb_info = usbinfo();

                // If this line contains information about the disconnection of the usb of the system
                } else if (regex_search(line.c_str(), disconnectionFound_regex)) {
                    // Keep id of usb used as key in map
                    regex_search(line.c_str(), id_found, usb_regex);
                    // keep disconnection time and save it
                    regex_search(line.c_str(), disconnectionTime_found, date_regex);
                    // Recover usbinfo structure previously saved in map, and save inside it the disconnection Time
                    it = usb_map.find(id_found.str());
                    usb_info = it->second;
                    usb_info.disconnectionTime = disconnectionTime_found.str();
                    // Replace the previous usbinfo structure by the updated one in the map
                    replaceInMap(&usb_map, id_found.str(), usb_info);
                    // Reset usb_info variable
                    usb_info = usbinfo();

                // If this line contains the id of the product and of the Vendor
                } else if (regex_search(line.c_str(), idVendorAndIdProduct_regex)) {
                    // Keep id of usb used as key in map
                    regex_search(line.c_str(), id_found, usb_regex);
                    // keep id vendor and save it
                    regex_search(line.c_str(), idVendor_found, idVendor_regex);
                    // keep id product and save it
                    regex_search(line.c_str(), idProduct_found, idProduct_regex);
                    // Recover usbinfo structure previously saved in map, and save inside it the id product and the vendor
                    it = usb_map.find(id_found.str());
                    usb_info = it->second;
                    // Save Vendor id and Product id into usb_info structure
                    idVendor_string = idVendor_found.str();
                    idProduct_string = idProduct_found.str();
                    usb_info.idVendor = stringToInt(idVendor_string.substr(idVendor_string.find("=")+1, idVendor_string.length()));
                    usb_info.idProduct = stringToInt(idProduct_string.substr(idProduct_string.find("=")+1, idProduct_string.length()));
                    // Replace the previous usbinfo structure by the updated one in the map
                    replaceInMap(&usb_map, id_found.str(), usb_info);
                    // Reset usb_info variable
                    usb_info = usbinfo();
                
                // If this line contains the serial number
                } else if (regex_search(line.c_str(), serialNumber_regex)) {
                    // Keep id of usb used as key in map
                    regex_search(line.c_str(), id_found, usb_regex);
                    // keep id vendor and save it
                    regex_search(line.c_str(), serialNumber_found, serialNumber_regex);
                    // Recover usbinfo structure previously saved in map, and save inside it the serial number
                    it = usb_map.find(id_found.str());
                    usb_info = it->second;
                    // Save Serial Number into usb_info structure
                    serialNumber_string = serialNumber_found.str();
                    usb_info.serialNumber = serialNumber_string.substr(serialNumber_string.find(":")+2, serialNumber_string.length());
                    // Replace the previous usbinfo structure by the updated one in the map
                    replaceInMap(&usb_map, id_found.str(), usb_info);
                    // Reset usb_info variable
                    usb_info = usbinfo();

                // If this line contains the product name
                } else if (regex_search(line.c_str(), productName_regex)) {
                    // Keep id of usb used as key in map
                    regex_search(line.c_str(), id_found, usb_regex);
                    // Keep id vendor and save it
                    regex_search(line.c_str(), productName_found, productName_regex);
                    // Recover usbinfo structure previously saved in map, and save inside it the product name
                    it = usb_map.find(id_found.str());
                    usb_info = it->second;
                    // Save product name into usb_info structure
                    productName_string = productName_found.str();
                    usb_info.productName = productName_string.substr(productName_string.find(":")+2, productName_string.length());
                    // Replace the previous usbinfo structure by the updated one in the map
                    replaceInMap(&usb_map, id_found.str(), usb_info);
                    // Reset usb_info variable
                    usb_info = usbinfo();

                // If this line contains the manufacturer
                } else if (regex_search(line.c_str(), manufacturer_regex)) {
                    // Keep id of usb used as key in map
                    regex_search(line.c_str(), id_found, usb_regex);
                    // Keep id vendor and save it
                    regex_search(line.c_str(), manufacturer_found, manufacturer_regex);
                    // Recover usbinfo structure previously saved in map, and save inside it the manufacturer
                    it = usb_map.find(id_found.str());
                    usb_info = it->second;
                    // Save manufacturer into usb_info structure
                    manufacturer_string = manufacturer_found.str();
                    usb_info.manufacturer = manufacturer_string.substr(manufacturer_string.find(":")+2, manufacturer_string.length());
                    // Replace the previous usbinfo structure by the updated one in the map
                    replaceInMap(&usb_map, id_found.str(), usb_info);
                    // Reset usb_info variable
                    usb_info = usbinfo();
                }
            }
        }
    }

    return usb_map;
}