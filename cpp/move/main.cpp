#include <stdexcept>
#include <iostream>

class Foo
{
public:
   Foo()
   {
      std::cout << "Foo Constructor\n";
   }

   ~Foo()
   {
      std::cout << "Foo Destructor\n";
   }

   Foo(const Foo &f)
   {
      std::cout << "Copy Constructor\n";
   }

   Foo(Foo &&f)
   {
      std::cout << "Move Constructor\n";
   }
};

Foo foo()
{
   std::cout << "1\n";
   Foo x;
   std::cout << "2\n";
   Foo z = x;
   std::cout << "3\n";
   Foo y = std::move(x);
   std::cout << "4\n";
   return z;
}

int main()
{
   std::cout << "0\n";
   auto f = foo();
   std::cout << "5\n";
   return 0;
}
