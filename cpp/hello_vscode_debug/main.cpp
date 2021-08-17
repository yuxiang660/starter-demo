#include <iostream>
#include <string>
#include <typeinfo>

int main(int argc ,char **argv)
{
    std::cout << "Test for CPP Syntax" << std::endl;
    auto uint_U = 0x1FFFU;
    auto long_uint_UL = 0x1FFFLU;
    auto long_long_uint_UL = 0x1FFFLLU;

    std::cout << "type of int_U: " << typeid(uint_U).name() << ", sizeof(U): " << sizeof(uint_U) << std::endl;
    std::cout << "type of long_uint_UL: " << typeid(long_uint_UL).name() << ", sizeof(LU): " << sizeof(long_uint_UL) << std::endl;
    std::cout << "type of long_long_uint_UL: " << typeid(long_long_uint_UL).name() << ", sizeof(LLU): " << sizeof(long_long_uint_UL) << std::endl;

    return 0;
}
