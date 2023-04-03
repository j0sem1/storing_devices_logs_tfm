#ifndef _USB_H
#define _USB_H

#include <regex>    // For regex and map
#include <string>

// Structre which contains information of each USB
struct usbinfo {
    std::string connectionTime;
    std::string disconnectionTime;
    int idVendor;
    int idProduct;
    std::string productName;
    std::string manufacturer;
    std::string serialNumber;
};

std::map<std::string, usbinfo> getUSBsInfo();

void example();

#endif