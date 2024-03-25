#include <iostream>

struct CppStruct
{
   char a;
   int b;
   double c;
};

class Base
{
public:
   virtual void foo() = 0;
};

class Child : public Base
{
public:
   void foo() override { printf("Child::foo"); }
   int m_child = 2;
};

int main()
{
   CppStruct s;
   return 0;
}
