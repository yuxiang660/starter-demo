#include <iostream>
#include <memory>

class FooInterface
{
public:
   virtual void bar(int a = 9) const = 0;

public:
   FooInterface() = default;
   virtual ~FooInterface() = default;
   FooInterface(const FooInterface &) = delete;
   FooInterface &operator=(const FooInterface &) = delete;
};

class FooReadInterface
{
public:
   FooReadInterface() = default;
   virtual ~FooReadInterface() = default;
   FooReadInterface(const FooReadInterface &) = delete;
   FooReadInterface &operator=(const FooReadInterface &) = delete;
};

class Foo : public FooInterface
{
private:
   friend class FooFactory;
   Foo(int f) : m_f(f) {} // create by FooFactory

public:
   ~Foo() override {}

public:
   void bar(int a) const { std::cout << a << "bar: " << m_f << std::endl; }

private:
   int m_f = 0;
};

class FooFactory
{
public:
   static std::unique_ptr<FooInterface> create(int f)
   {
      return std::unique_ptr<Foo>(new Foo(f));
   }

   static std::unique_ptr<FooInterface> create(int f, int d)
   {
      return std::unique_ptr<Foo>(new Foo(f+d));
   }
};

struct Header
{
   //static constexpr int LAST_MOD_TIME_OFF = 8;

   // 4 bytes: magic number to identify file type
   uint32_t magicNum = 123;
   // 4 bytes: file version
   uint32_t fileVersion = 0;
   // 8 bytes: last modification time (calendar time)
   uint64_t tcLastModTime = 0;
   // 4 bytes: A random value
   uint32_t tcRandom = 0;
   // 8 bytes: real start time
   uint64_t tcRealStartTime = 0;
   // 8 bytes: real end time
   uint64_t tcRealEndTime = 0;
   // 4 bytes: toggle type, enum TC_TYPE
   uint32_t tcType = 0;
   // 4 bytes: ppfdata format, enum PPF_FORMAT
   uint32_t ppfFormat = 33;
};

int main()
{
   std::unique_ptr<FooInterface> pFoo = FooFactory::create(5);
   std::unique_ptr<FooInterface> pFoo2 = FooFactory::create(5, 3);
   pFoo->bar();
   pFoo2->bar();

   std::cout << sizeof(Header) << std::endl;
   //std::cout << Header::LAST_MOD_TIME_OFF << std::endl;
   Header header;
   std::cout << header.magicNum << std::endl;
   std::cout << header.ppfFormat << std::endl;
}
