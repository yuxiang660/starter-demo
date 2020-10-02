#include <iostream>
#include <unordered_map>
#include <string>

// Print unordered_map in gdb with pretty format, refer to https://sourceware.org/gdb/wiki/STLSupport
int main(int argc ,char **argv)
{
    std::cout << "Pretty STL with GDB" << std::endl;

    std::unordered_map<int, std::string> data;
    data[1] = "first";
    data[2] = "second";

    for (auto it = data.begin(); it != data.end(); ++it ) {
        std::cout << " " << it->first << ":" << it->second;
        std::cout << std::endl;
    }

    return 0;
}
