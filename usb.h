#ifndef _USB_H
#define _USB_H

#include <regex>    // For regex and map
#include <string>
#include <list> /* For lists */

// Structre which contains information of each USB
struct usbInfo {
    std::list<std::string> connectionTimes;
    std::list<std::string> disconnectionTimes;
    int idVendor = 0;
    int idProduct = 0;
    std::string productName = "";
    std::string manufacturer = "";
    std::string serialNumber = "";
};

std::map<std::string, usbInfo> getUSBsInfo();

#endif