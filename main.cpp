#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

struct usbinfo {
    string connectionTime;
    string disconnectionTime;
    int idVendor;
    int idProduct;
    string name;
    string manufacturer;
    string serialNumber;
};

int main(){

    string line;
    regex usb_regex, date_regex, info_regex, disconnectionFound_regex;
    ifstream file;
    cmatch id_found, connectionTime_found, disconnectionTime_found, info_found;
    usbinfo usb_info;
    bool firstLine;

    usb_regex = regex("usb [0-9]-[0-9]");
    date_regex = regex("[0-9]{2}:[0-9]{2}:[0-9]{2}");
    info_regex = regex(": ([a-z]|[A-Z])");
    disconnectionFound_regex = regex("USB disconnect");
    
    file.open("/var/log/syslog");

    firstLine = true;

    if (file.is_open()){
        while (getline(file, line)){
            // match regex to see if it is a usb
            if (regex_search(line.c_str(), id_found, usb_regex)){
                // if id_found is not in table then enter in if
                if (firstLine){
                    // keep connection time and save it
                    regex_search(line.c_str(), connectionTime_found, date_regex);
                    usb_info.connectionTime = connectionTime_found.str();
                    firstLine = false;  // a eliminar cuando hash table implementada
                } else if (regex_search(line.c_str(), disconnectionFound_regex)) {
                    // keep disconnection time and save it
                    regex_search(line.c_str(), disconnectionTime_found, date_regex);
                    usb_info.disconnectionTime = disconnectionTime_found.str();
                } else {
                    // TODO
                }
                // Añadir USB al set/hash table con la key siendo el id_found
                // Almacenar la información de dicho usb
                // keep string after last ": "
                //regex_search(line.c_str(), info_found, info_regex);
                // keep info and save it
                
                // save the structure in the table
            }
        }
        cout << usb_info.connectionTime << " -> " << usb_info.disconnectionTime << endl;
        file.close();
    }

    return 0;
}