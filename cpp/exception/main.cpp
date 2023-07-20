#include <stdexcept>
#include <iostream>

class X: public std::runtime_error
{
public:
   X(std::string const& msg):
      runtime_error(msg)
   {}
};

int main()
{
   try
   {
      //throw X("Test");
      throw std::runtime_error("abc");
   }
   catch(const X &e)
   {
      std::cout << "Message: " << e.what() << "\n";
      std::cout << "Type:    " << typeid(e).name() << "\n";
   }
   catch(const std::exception &e)
   {
      std::cout << "exception " << e.what() << std::endl;
   }
   catch(...)
   {
      std::exception_ptr p = std::current_exception();
      std::cout << "Unexpected error happens when reading SPEF files: " << (p ? p.__cxa_exception_type()->name() : "null") << "\n";
   }
}
