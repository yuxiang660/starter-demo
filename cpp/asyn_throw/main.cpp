#include <stdexcept>
#include <iostream>
#include <future>

class X: public std::runtime_error
{
public:
   X(std::string const& msg):
      runtime_error("X:" + msg)
   {}
};

class Y: public std::runtime_error
{
public:
   Y(std::string const& msg):
      runtime_error("Y:" + msg)
   {}
};

int x_thread(int a)
{
   printf("x_thread a=%d\n", a);
   //throw X("x error");
   return 1;
}

int y_thread(int b)
{
   printf("y_thread b=%d\n", b);
   //throw Y("y error");
   return 2;
}

int main()
{
   try
   {
      auto x_fut = std::async(std::launch::async, x_thread, 222);
      auto y_fut = std::async(std::launch::async, y_thread, 111);
      printf("x_fut=%d\n", x_fut.get());
      printf("y_fut=%d\n", y_fut.get());
      throw std::runtime_error("aaa");
   }
   catch(const X &e)
   {
      std::cout << "Message: " << e.what() << "\n";
      std::cout << "Type:    " << typeid(e).name() << "\n";
   }
   catch(const Y &e)
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

   return 0;
}
