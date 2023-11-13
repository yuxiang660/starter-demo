#include <stdexcept>
#include <iostream>

int main()
{
   auto a = 0;
   auto b = 0LL;
   auto c = 0ULL;
   std::cout << typeid(a).name() << ", " << sizeof(a) << std::endl;
   std::cout << typeid(b).name() << ", " << sizeof(b) << std::endl;
   std::cout << typeid(c).name() << ", " << sizeof(c) << std::endl;
   return 0;
}
