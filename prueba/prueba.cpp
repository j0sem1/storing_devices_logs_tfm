#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Split a string into a vector of substrings using a delimiter
std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(s);
    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main()
{
    std::string device_name = "sda"; // Name of the block device

    // Open the /proc/mounts file for reading
    std::ifstream mounts_file("/proc/mounts");
    if (!mounts_file) {
        std::cerr << "Failed to open /proc/mounts" << std::endl;
        return 1;
    }

    // Read the file line by line and look for the device name
    std::string line;
    while (std::getline(mounts_file, line)) {
        // Split the line into fields using whitespace as a delimiter
        std::vector<std::string> fields = split(line, ' ');

        // Check if the device name matches the specified name
        if (fields.size() >= 1 && fields[0] == "/dev/" + device_name) {
            // Extract the mount point and construct the sysfs path
            std::string mount_point = fields[1];
            std::string sysfs_path = mount_point.substr(0, mount_point.find_last_of('/'));

            // Print the sysfs path to the console
            std::cout << "Sysfs path for " << device_name << ": " << sysfs_path << std::endl;

            // Exit the loop since we found the device we were looking for
            break;
        }
    }

    return 0;
}