#include <stdexcept>
#include <iostream>
#include <string>

void test()
{
   int a = 1;
   std::stol("x");
   printf("%d\n", a);
}

int main()
{
   try
   {
      test();
   }
   catch(const std::exception &e)
   {
      std::cout << "exception :" << e.what() << std::endl;
   }
   catch(...)
   {
      std::exception_ptr p = std::current_exception();
      std::cout << "Unexpected error happens when reading SPEF files: " << (p ? p.__cxa_exception_type()->name() : "null") << "\n";
   }
}
