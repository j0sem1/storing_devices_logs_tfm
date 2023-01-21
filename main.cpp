#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream> // this will allow you to use stringstream in your program


using namespace std;


// Structre which contains information of each USB
struct usbinfo {
    string connectionTime;
    string disconnectionTime;
    int idVendor;
    int idProduct;
    string name;
    string manufacturer;
    string serialNumber;
};


std::map<string, usbinfo>::iterator replaceInMap(map<string, usbinfo>* map, string key, usbinfo usb_info_new){

    usbinfo usb_info_old;
    std::map<string, usbinfo>::iterator it;

    it = (*map).find(key);
    usb_info_old = it->second;
    (*map).erase(it);
    return ((*map).insert({key, usb_info_new})).first;
}

/*
std::map<string, usbinfo> replaceInMap2(map<string, usbinfo> map, string key, usbinfo usb_info_new){

    usbinfo usb_info_old;
    std::map<string, usbinfo>::iterator it;

    it = map.find(key);
    usb_info_old = it->second;
    map.erase(it);
    map.insert({key, usb_info_new});
    return map;
}
*/

void printMap(map<string, usbinfo> map){

    std::map<string, usbinfo>::iterator it;

    for (it = map.begin(); it != map.end(); ++it){
        cout << "||| " << it->first << " |||" << endl;
        cout << "Connection time: " << it->second.connectionTime << endl;
        cout << "Disconnection time: " << it->second.disconnectionTime << endl;
        if (!it->second.idVendor){
            cout << "id vendor not found" << endl;
        } else {
            cout << "vendor id: " << it->second.idVendor << endl;
        }
        if (!it->second.idProduct){
            cout << "id vendor not found" << endl;
        } else {
            cout << "product id: " << it->second.idProduct << endl;
        }
        cout << it->second.name << endl;
        cout << it->second.manufacturer << endl;
        cout << "Serial number: " << it->second.serialNumber << endl;
        cout << endl << endl;
    }
}


int stringToInt(string stringToConvert){

    // Stringstream object, to input/output strings
    stringstream ss;
    int result;

    // Extract the string from the str variable (input the string in the stream)
    ss << stringToConvert;
    // Place the converted value to the int variable
    ss >> result;

    return result;
}


int main(){

    // Variables
    string line, idVendor_string, idProduct_string, serialNumber_string;
    regex usb_regex, date_regex, info_regex, disconnectionFound_regex, idVendorAndIdProduct_regex, idVendor_regex, idProduct_regex, serialNumber_regex;
    ifstream file;
    cmatch id_found, connectionTime_found, disconnectionTime_found, info_found, idVendor_found, idProduct_found, serialNumber_found;
    usbinfo usb_info = usbinfo();
    // Create a map and its iterator
    map<string, usbinfo> usb_map;
    map<string, usbinfo>::iterator it;

    // Initialization
    usb_regex = regex("usb [0-9]-[0-9]");
    date_regex = regex("[0-9]{2}:[0-9]{2}:[0-9]{2}");
    info_regex = regex(": ([a-z]|[A-Z])");
    disconnectionFound_regex = regex("USB disconnect");
    idVendorAndIdProduct_regex = regex("USB device found");
    idVendor_regex = regex("idVendor=([0-9])*");
    idProduct_regex = regex("idProduct=([0-9])*");
    serialNumber_regex = regex("SerialNumber: ([a-z]|[A-Z]|[0-9])*");
    
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
                    // Recover usbinfo structure previously saved in map, and save inside it the disconnection Time
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
                
                } else if (regex_search(line.c_str(), serialNumber_regex)) {
                    // Keep id of usb used as key in map
                    regex_search(line.c_str(), id_found, usb_regex);
                    // keep id vendor and save it
                    regex_search(line.c_str(), serialNumber_found, serialNumber_regex);
                    // Recover usbinfo structure previously saved in map, and save inside it the disconnection Time
                    it = usb_map.find(id_found.str());
                    usb_info = it->second;
                    // Save Serial Number into usb_info structure
                    serialNumber_string = serialNumber_found.str();
                    usb_info.serialNumber = serialNumber_string.substr(serialNumber_string.find(":")+2, serialNumber_string.length());
                    // Replace the previous usbinfo structure by the updated one in the map
                    replaceInMap(&usb_map, id_found.str(), usb_info);
                    // Reset usb_info variable
                    usb_info = usbinfo();
                }
            }
        }

        printMap(usb_map);
        
    }

    return 0;
}