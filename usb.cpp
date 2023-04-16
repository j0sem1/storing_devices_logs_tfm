#include "usb.h"
#include <fstream>
//#include <tabulate/table.hpp> /* For print tables */


// Structre which contains information of USBs interfaces
struct usbInterface {
    bool isEmpty = true;
    bool isStorageDevice = false;
    std::string connectionTime;
    std::string disconnectionTime;
    int idVendor;
    int idProduct;
    std::string productName;
    std::string manufacturer;
    std::string serialNumber;
};

void initializeUsbInterface(usbInterface* usb_interface) {
    usb_interface->isEmpty = true;
    usb_interface->isStorageDevice = false;
    usb_interface->connectionTime = "";
    usb_interface->disconnectionTime = "";
    usb_interface->idVendor = 0;
    usb_interface->idProduct = 0;
    usb_interface->productName = "";
    usb_interface->manufacturer = "";
    usb_interface->serialNumber = "";
}

/*
void example() {
    tabulate::Table table;

    table.add_row({""});

    table[0][0].format().width(20);
    table[0][1].format().width(50);

    std::cout << table << std::endl;
}
*/


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

std::map<std::string, usbInfo>::iterator replaceInUSBsMap(std::map<std::string, usbInfo>* map, std::string key, usbInfo usb_info_new){

    usbInfo usb_info_old;
    std::map<std::string, usbInfo>::iterator it;

    it = (*map).find(key);
    usb_info_old = it->second;
    (*map).erase(it);
    return ((*map).insert({key, usb_info_new})).first;
}


std::map<std::string, usbInterface>::iterator replaceInInterfacesMap(std::map<std::string, usbInterface>* map, std::string key, usbInterface usb_interface_element_new){

    usbInterface usb_interface_element_old;
    std::map<std::string, usbInterface>::iterator it;

    it = (*map).find(key);
    usb_interface_element_old = it->second;
    (*map).erase(it);
    return ((*map).insert({key, usb_interface_element_new})).first;
}


std::string getInterfaceIdFromRegex(std::string regex_result) {

    std::regex interface_id_regex = std::regex("[0-9]-[0-9]");
    std::cmatch interface_id;

    if (regex_search(regex_result.c_str(), interface_id, interface_id_regex)) {
        return interface_id.str();
    }

    return "";
}


bool checkIfStorageDevice(std::string line, std::map<std::string, usbInterface>* usb_interfaces_map) {

    // Interfaces map iterator
    std::map<std::string, usbInterface>::iterator it;
    // USB Interface auxiliar element
    usbInterface usb_interface_element;

    std::regex storage_regex = std::regex("usb-storage [0-9]-[0-9]");
    std::cmatch id_storage_found;

    std::string interface_id;

    // Match regex to see if there is information in this line about storage of some usb
    if (regex_search(line.c_str(), id_storage_found, storage_regex)) {

        interface_id = getInterfaceIdFromRegex(id_storage_found.str());

        // If this usb interface has been added to map
        if ((*usb_interfaces_map).find(interface_id) != (*usb_interfaces_map).end()) {

            it = (*usb_interfaces_map).find(interface_id);
            usb_interface_element = it->second;
            usb_interface_element.isStorageDevice = true;
            // Replace the previous usbInterface structure by the updated one in the map
            replaceInInterfacesMap(usb_interfaces_map, interface_id, usb_interface_element);
        }

        return true;
    }

    return false;
}


bool checkIfNewDeviceConnected(std::string line, std::map<std::string, usbInterface>* usb_interfaces_map) {

    // Interfaces map iterator
    std::map<std::string, usbInterface>::iterator it;
    // USB Interface auxiliar element
    usbInterface usb_interface_element;

    std::regex new_usb_regex = std::regex("usb [0-9]-[0-9]: new");
    std::regex date_regex = std::regex("[0-9]{2}:[0-9]{2}:[0-9]{2}");
    std::cmatch regex_found;

    std::string interface_id;

    // Match regex to see if there is information in this line about a device connected
    if (regex_search(line.c_str(), regex_found, new_usb_regex)) {

        interface_id = getInterfaceIdFromRegex(regex_found.str());

        // If this usb interface has been added to map
        if ((*usb_interfaces_map).find(interface_id) != (*usb_interfaces_map).end()) {

            it = (*usb_interfaces_map).find(interface_id);
            usb_interface_element = it->second;

            
            if (usb_interface_element.isEmpty) {
                
                usb_interface_element.isEmpty = false;
                
                // Save connection time
                regex_search(line.c_str(), regex_found, date_regex);
                usb_interface_element.connectionTime = regex_found.str();
                // Replace the previous usbInterface structure by the updated one in the map
                replaceInInterfacesMap(usb_interfaces_map, interface_id, usb_interface_element);

                return true;
            
            }
            /* else {
                // If there is information about a previous usb there hasn't been generated a log when usb disconnected and info has not been saved

                // Si el campo vacío es FALSE
                // Si el usb no ha sido guardado en usbs
                    // Guardar elemento
                // Si el usb ya ha sido guardado antes
                    // Volcar todos los datos de interfaces a usbs si no están actualizados.
                    // Si ya tiene una hora conexión y desconexión pero son distintas, añadir las nuevas

                // Inicializar todos los campos del elemento de interfaces
                // Guardar hora de conexión
                // Setear campo vacío a FALSE
            } */

        } else {

            initializeUsbInterface(&usb_interface_element);

            usb_interface_element.isEmpty = false;

            // Save connection time
            regex_search(line.c_str(), regex_found, date_regex);
            usb_interface_element.connectionTime = regex_found.str();

            // Add interface element to elements map
            (*usb_interfaces_map).insert({interface_id, usb_interface_element});

            return true;

        }

    }

    return false;

}


bool checkIfDeviceDisconnected(std::string line, std::map<std::string, usbInterface>* usb_interfaces_map, std::map<std::string, usbInfo>* usb_map) {

    // Interfaces map iterator
    std::map<std::string, usbInterface>::iterator it;
    // USB Interface auxiliar element
    usbInterface usb_interface_element;

    std::regex disconnectionFound_regex = std::regex("USB disconnect");
    std::regex date_regex = std::regex("[0-9]{2}:[0-9]{2}:[0-9]{2}");
    std::regex usb_regex = std::regex("usb [0-9]-[0-9]");
    std::cmatch regex_found;
    std::cmatch usb_regex_found;

    std::string interface_id;

    // Usb map iterator
    std::map<std::string, usbInfo>::iterator usb_it;
    // USB auxiliar element
    usbInfo usb_element;

    // Match regex to see if there is information in this line about a device disconnected
    if (regex_search(line.c_str(), regex_found, disconnectionFound_regex)) {

        if (regex_search(line.c_str(), usb_regex_found, usb_regex)) {
            interface_id = getInterfaceIdFromRegex(usb_regex_found.str());
        }

        // If this usb interface has been added to map
        if ((*usb_interfaces_map).find(interface_id) != (*usb_interfaces_map).end()) {

            it = (*usb_interfaces_map).find(interface_id);
            usb_interface_element = it->second;

            // Save disconnection time
            regex_search(line.c_str(), regex_found, date_regex);

            if (!usb_interface_element.isEmpty && usb_interface_element.isStorageDevice) {

                // If usb already added in usbs map, add new connection and disconnection time
                //if ((*usb_map).find(usb_interface_element.serialNumber) != (*usb_map).end()) {
                if ((*usb_map).count(usb_interface_element.serialNumber) > 0) {

                    usb_it = (*usb_map).find(usb_interface_element.serialNumber);
                    usb_element = usb_it->second;

                    usb_element.connectionTimes.push_back(usb_interface_element.connectionTime);
                    usb_element.disconnectionTimes.push_back(regex_found.str());

                    // Replace the previous usbInterface structure by the updated one in the map
                    replaceInUSBsMap(usb_map, usb_interface_element.serialNumber, usb_element);

                } else {
                
                    // If usb not added previously, add it
                    usb_element = usbInfo();
                    usb_element.connectionTimes.push_back(usb_interface_element.connectionTime);
                    usb_element.disconnectionTimes.push_back(regex_found.str());
                    usb_element.idVendor = usb_interface_element.idVendor;
                    usb_element.idProduct = usb_interface_element.idProduct;
                    usb_element.productName = usb_interface_element.productName;
                    usb_element.manufacturer = usb_interface_element.manufacturer;
                    usb_element.serialNumber = usb_interface_element.serialNumber;

                    (*usb_map).insert({usb_interface_element.serialNumber, usb_element});

                }

                initializeUsbInterface(&usb_interface_element);

                // Replace the previous usbInterface structure by the updated one in the map
                replaceInInterfacesMap(usb_interfaces_map, interface_id, usb_interface_element);

            }

        }

        return true;

    }

    return false;

}


bool checkIfInformationAboutDeviceFound(std::string line, std::map<std::string, usbInterface>* usb_interfaces_map) {

    // Interfaces map iterator
    std::map<std::string, usbInterface>::iterator it;
    // USB Interface auxiliar element
    usbInterface usb_interface_element;

    std::regex usb_regex = std::regex("usb [0-9]-[0-9]");
    std::regex idVendorAndIdProduct_regex = std::regex("USB device found");
    std::regex idVendor_regex = std::regex("idVendor=([0-9])*");
    std::regex idProduct_regex = std::regex("idProduct=([0-9])*");
    std::regex serialNumber_regex = std::regex("SerialNumber: .*");
    std::regex productName_regex = std::regex("Product: .*");
    std::regex manufacturer_regex = std::regex("Manufacturer: .*");
    std::cmatch regex_found;

    std::string interface_id;

    // Match regex to see if there is information in this line about a device
    if (regex_search(line.c_str(), regex_found, usb_regex)) {

        interface_id = getInterfaceIdFromRegex(regex_found.str());
            
        // If this usb interface has been added to map
        if ((*usb_interfaces_map).find(interface_id) != (*usb_interfaces_map).end()) {

            it = (*usb_interfaces_map).find(interface_id);
            usb_interface_element = it->second;

            // If this line contains the id of the product and of the Vendor
            if (regex_search(line.c_str(), idVendorAndIdProduct_regex)) {

                // TODO
                /*
                // Save Vendor id and Product id into usb_info structure
                idVendor_string = idVendor_found.str();
                idProduct_string = idProduct_found.str();
                */

                // keep id vendor and save it
                regex_search(line.c_str(), regex_found, idVendor_regex);
                usb_interface_element.idVendor = stringToInt(regex_found.str().substr(regex_found.str().find("=")+1, regex_found.str().length()));

                // keep id product and save it
                regex_search(line.c_str(), regex_found, idProduct_regex);
                usb_interface_element.idProduct = stringToInt(regex_found.str().substr(regex_found.str().find("=")+1, regex_found.str().length()));
                
                // Replace the previous usbInterface structure by the updated one in the map
                replaceInInterfacesMap(usb_interfaces_map, interface_id, usb_interface_element);
            
            // If this line contains the serial number
            } else if (regex_search(line.c_str(), serialNumber_regex)) {

                // keep id vendor and save it
                regex_search(line.c_str(), regex_found, serialNumber_regex);
                usb_interface_element.serialNumber = regex_found.str().substr(regex_found.str().find(":")+2, regex_found.str().length());
                
                // Replace the previous usbInterface structure by the updated one in the map
                replaceInInterfacesMap(usb_interfaces_map, interface_id, usb_interface_element);

            // If this line contains the product name
            } else if (regex_search(line.c_str(), productName_regex)) {

                // Keep id vendor and save it
                regex_search(line.c_str(), regex_found, productName_regex);
                usb_interface_element.productName = regex_found.str().substr(regex_found.str().find(":")+2, regex_found.str().length());

                // Replace the previous usbInterface structure by the updated one in the map
                replaceInInterfacesMap(usb_interfaces_map, interface_id, usb_interface_element);

            // If this line contains the manufacturer
            } else if (regex_search(line.c_str(), manufacturer_regex)) {

                // Keep id vendor and save it
                regex_search(line.c_str(), regex_found, manufacturer_regex);
                usb_interface_element.manufacturer = regex_found.str().substr(regex_found.str().find(":")+2, regex_found.str().length());
                
                // Replace the previous usbInterface structure by the updated one in the map
                replaceInInterfacesMap(usb_interfaces_map, interface_id, usb_interface_element);
            }

        }

    }

    return false;
}


void saveInterfacesInfoInUSBsMap(std::map<std::string, usbInterface>* usb_interfaces_map, std::map<std::string, usbInfo>* usb_map) {

    // Interfaces map iterator
    std::map<std::string, usbInterface>::iterator it;
    // Usb map iterator
    std::map<std::string, usbInfo>::iterator usb_it;
    // USB Interface auxiliar element
    usbInterface usb_interface_element;
    // USB auxiliar element
    usbInfo usb_element;

    for (it = (*usb_interfaces_map).begin(); it != (*usb_interfaces_map).end(); ++it){

        if (!it->second.isEmpty){

            // Save information contained in interface in usb auxiliar element
            usb_interface_element = it->second;

            // Initialize usb auxiliar element
            usb_element = usbInfo();

            // If this USB device has already been added to USBs map
            if ((*usb_map).count(usb_interface_element.serialNumber)) {
                
                // Get element from map
                usb_it = (*usb_map).find(usb_interface_element.serialNumber);
                usb_element = usb_it->second;

                // Add new connection time to list
                usb_element.connectionTimes.push_back(usb_interface_element.connectionTime);
                // Set information found that was not set before
                if (usb_element.idVendor == 0){
                    usb_element.idVendor = usb_interface_element.idVendor;
                }
                if (usb_element.idProduct == 0){
                    usb_element.idProduct = usb_interface_element.idProduct;
                }
                if (usb_element.productName == ""){
                    usb_element.productName = usb_interface_element.productName;
                }
                if (usb_element.manufacturer == ""){
                    usb_element.manufacturer = usb_interface_element.manufacturer;
                }
                if (usb_element.serialNumber == ""){
                    usb_element.serialNumber = usb_interface_element.serialNumber;
                }

                // Replace the previous usbInterface structure by the updated one in the map
                replaceInUSBsMap(usb_map, usb_interface_element.serialNumber, usb_element);

            } else if (usb_interface_element.isStorageDevice) {

                // If usb not added previously, add it
                usb_element.connectionTimes.push_back(usb_interface_element.connectionTime);
                usb_element.idVendor = usb_interface_element.idVendor;
                usb_element.idProduct = usb_interface_element.idProduct;
                usb_element.productName = usb_interface_element.productName;
                usb_element.manufacturer = usb_interface_element.manufacturer;
                usb_element.serialNumber = usb_interface_element.serialNumber;

                (*usb_map).insert({usb_interface_element.serialNumber, usb_element});

            }

            // Clean interface element in interfaces map
            initializeUsbInterface(&usb_interface_element);

            // Replace the previous usbInterface structure by the updated one in the map
            replaceInInterfacesMap(usb_interfaces_map, it->first, usb_interface_element);

        }

    }

}


std::map<std::string, usbInfo> getUSBsInfo() {

    // Create a map and its iterator for usbs info
    std::map<std::string, usbInfo> usb_map;

    // Map for current interfaces info
    std::map<std::string, usbInterface> usb_interfaces_map;

    // File descriptor
    std::ifstream file;

    // Current line of file
    std::string line;

    // Variables for regex
    std::regex usb_regex = std::regex("usb [0-9]-[0-9]");

    // Open syslog file and save file descriptor
    file.open("/var/log/syslog");

    if (file.is_open()) {

        while (getline(file, line)) {

            // check if this line indicates that a device is for storage
            if (!checkIfStorageDevice(line, &usb_interfaces_map)) {

                // if not -> check if in this line there is information about an usb
                if (regex_search(line.c_str(), usb_regex)) {

                    // if this line has information about an usb device
                    if (!checkIfInformationAboutDeviceFound(line, &usb_interfaces_map)) {

                        // if this line has information about a new usb
                        if (!checkIfNewDeviceConnected(line, &usb_interfaces_map)) {

                            // if this line has information about an usb disconnected
                            checkIfDeviceDisconnected(line, &usb_interfaces_map, &usb_map);
                        }

                    }
                }
            }
        }
    }

    // Save interfaces information not synchronized with usbs map
    saveInterfacesInfoInUSBsMap(&usb_interfaces_map, &usb_map);

    return usb_map;
}