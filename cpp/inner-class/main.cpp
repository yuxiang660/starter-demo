#include <iostream>
#include <memory>
#include <cstddef>

class Foo
{
public:
   Foo() : m_f(5) {}

   struct Bar
   {
      typedef int HEADER_DATA;
      HEADER_DATA m_b = 3;
      HEADER_DATA m_b2 = 5;
      HEADER_DATA m_b3 = 600;
      void bar(const Foo &foo) const
      {
         std::cout << foo.m_f << ", " << m_b << std::endl;
         std::cout << "test:" << foo.test() << std::endl;
      }
      void dump() const
      {
         const int *pBuf = &m_b;
         const size_t bufSize = sizeof(Bar) / sizeof(int);
         std::cout << "start\n";
         for (int i = 0; i < bufSize; i++)
            std::cout << *(pBuf + i) << std::endl;
         std::cout << "end\n";
      }
   };

public:
   void foo() const
   {
      m_bar.bar(*this);
      std::cout << "sizeof(bar): " << sizeof(m_bar) << std::endl;
      std::cout
          << "offset of m_b = " << offsetof(Bar, m_b) << '\n'
          << "offset of m_b2 = " << offsetof(Bar, m_b2) << '\n'
          << "offset of m_b3 = " << offsetof(Bar, m_b3) << '\n';
      m_bar.dump();
   }
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
