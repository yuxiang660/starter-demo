#include <iostream>
#include <memory>

class Foo
{
public:
   Foo() : m_f(5) {}

   struct Bar
   {
      int m_b = 3;
      void bar(const Foo &foo) const { std::cout << foo.m_f << ", " << m_b << std::endl; std::cout << "test:" << foo.test() << std::endl; }
   };
public:
   void foo() const { m_bar.bar(*this); std::cout << "sizeof(bar): " << sizeof(m_bar) << std::endl; }
   virtual int test() const = 0;

private:
   Bar m_bar;
   int m_f = 2;
};

class FooD : public Foo
{
public:
   FooD() = default;

   int test() const override { return 2; }
};

class FooD2 : public Foo
{
public:
   FooD2() = default;

   int test() const override { return 3; }
};


int main()
{

   std::unique_ptr<Foo> f = std::make_unique<FooD>();
   std::unique_ptr<Foo> f2 = std::make_unique<FooD2>();
   f->foo();
   f2->foo();
   return 0;
}
