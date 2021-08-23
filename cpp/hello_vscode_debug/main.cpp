#include <iostream>
#include <string>
#include <typeinfo>

void hello(std::string s) {
    std::cout << s << std::endl;
    std::cout << "begin" << std::endl;
    std::cout << "end" << std::endl;
}

int main(int argc ,char **argv)
{
    std::string s("hello");
    hello(s);

    return 0;
}
