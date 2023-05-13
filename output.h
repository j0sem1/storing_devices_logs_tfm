#ifndef _OUTPUT_H
#define _OUTPUT_H

#include "hard_disk.h"
#include "usb.h"
#include <set> // For sets

std::string humanReadableByteCount(unsigned long long bytes, bool si);

void printHTML(std::map<std::string, usbInfo> usbs_map, std::map<std::string, hardDiskInfo> disks_map);

void printUSBsMapInTerminal(std::map<std::string, usbInfo> usbs_map);

void printDisksMapInTerminal(std::map<std::string, hardDiskInfo> disks_map);

#endif