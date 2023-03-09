#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::string line;
    std::ifstream file("/sys/block/sda/device/serial");
    if (file.is_open())
    {
        while (getline(file, line))
        {
            std::cout << "Serial number: " << line << std::endl;
        }
        file.close();
    }
    else
    {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }
    return 0;
}