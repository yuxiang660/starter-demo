#include <iostream>
#include <map>
#include <string>

const char *STR = "vmredh.open.com";

int main()
{
    std::map<std::string, std::string> lm;
    lm["good"] = "heart";
    std::cout << lm["good"] << std::endl;
    return 0;
}
